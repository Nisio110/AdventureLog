# QUICK-SAVE.md

Scratch-pad for ideas captured mid-conversation that aren't yet acted on. Items here should either be promoted into [CONTEXT.md](CONTEXT.md) / [PROGRESS.md](PROGRESS.md) / a plan once work starts, or deleted once no longer relevant. Not a permanent record.

---

## Unit test proposals (gtest)

Saved 2026-04-21. Targets the critical non-UI parts of the codebase. Framework decision: **Google Test** via `FetchContent`.

### Key gtest gotchas for this codebase

- **Static ID counters persist across tests.** `User::numUsers`, `Log::numLogs`, `Participant::numParticipants` live in TU-scope statics ([src/User.cpp:5](../src/User.cpp#L5), [src/Log.cpp:9](../src/Log.cpp#L9), [src/Participant.cpp:6](../src/Participant.cpp#L6)). Every fixture's `SetUp()` must reset them via `setNumUsers(0)` / `setNumLogs(0)` / `setNumParticipants(0)` or tests will be order-dependent.
- **`Main.cpp` defines `main`.** For a gtest binary linking to `gtest_main`, exclude `src/Main.cpp` — either extract a static library (`AdventureLogLib`) containing everything except `Main.cpp` (preferred), or `list(REMOVE_ITEM ...)` in CMake.
- **`file(GLOB src/*.cpp)`** at [CMakeLists.txt:8](../CMakeLists.txt#L8) — adding `tests/` sources needs its own glob or explicit list.
- **No built-in leak detection.** Use `-fsanitize=address,undefined` on the test binary; double-frees and leaks surface as test failures.

### CMake scaffold

```cmake
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.15.2
)
FetchContent_MakeAvailable(googletest)

enable_testing()
file(GLOB TEST_SOURCES tests/*.cpp)
add_executable(AdventureLogTests ${TEST_SOURCES} ${LIB_SOURCES})
target_link_libraries(AdventureLogTests GTest::gtest_main)
target_include_directories(AdventureLogTests PRIVATE include)
include(GoogleTest)
gtest_discover_tests(AdventureLogTests)
```

### Suggested test layout

```
tests/
├── test_disk_helper.cpp      (parameterised — §1)
├── test_disk_init.cpp        (§2)
├── test_disk_load.cpp        (§3)
├── test_round_trip.cpp       (§4)
├── test_log_polymorphism.cpp (typed tests — §5)
├── test_sort.cpp             (§7)
└── fixtures/
    └── disk_fixture.h
```

Group by behaviour under test (loading, round-trip, serialisation), not by class — most failures cross class boundaries.

---

### §1. `DiskHelper` pure functions — `TEST_P` parameterised

One `TEST_P` per function with an `INSTANTIATE_TEST_SUITE_P` table of cases. Candidates:

- **`strToKVPair`** ([include/Disk.h:27](../include/Disk.h#L27)) — separator is `": "`.
  - `"name: Alice"` → `{"name", "Alice"}`
  - `"note: met at 10:30"` — does it split on first `": "` only?
  - `""`, missing separator — pin current behaviour
- **`strToBool`** — only `"true"`/`"false"` per [CONTEXT.md](CONTEXT.md). Test `"TRUE"`, `"1"`, `"yes"` trigger `printErr` and return `false`. Capture stderr with `testing::internal::CaptureStderr()`.
- **`strToNum`** — `stoul(base=10)`. Test negative, hex-looking, overflow, leading whitespace.
- **`intToStr` / `boolToStr`** — round-trip against their inverse.
- **`doesSubstrExist`**, **`getKeyLocationInObj`** — first-match-wins, not-found sentinel.

### §2. Object-level init

- **`Disk::initLog`** — `dynamic_cast<CaveLog*>` succeeds for `object: CaveLog` KVL, fails for `object: HikeLog`. Symmetric for HikeLog. Unknown `object:` value — pin current silent-ignore behaviour.
- **`Disk::initUser`** — verify `id` comes from the KV list via `setID`, not `generateID()` ([CONTEXT.md:104](CONTEXT.md#L104)).
- **`Disk::initParticipant`** — verify `logId` parsed into the field used later by `loadLogs` to wire up parents.

### §3. Load pipeline — one fixture, several `TEST_F`s

Fixture writes a canonical `disk.yaml` to `testing::TempDir()`:

```cpp
class DiskLoadTest : public testing::Test {
protected:
    std::string diskPath;
    void SetUp() override {
        User::setNumUsers(0);
        Log::setNumLogs(0);
        Participant::setNumParticipants(0);
        diskPath = testing::TempDir() + "/disk.yaml";
        writeFixtureYaml(diskPath);
    }
};
```

Tests against it:

- `WiresPointerGraphByID` — 1 user → 2 logs → 3 participants, assert full graph.
- `RestoresIdCountersToMax` — fixture with max IDs (7, 12, 20) → new objects get (8, 13, 21). Pins the fix at [src/Disk.cpp:183-185](../src/Disk.cpp#L183-L185).
- `OrphanParticipantDropped` — fixture with participant whose `log-id` points to nothing. Currently silently dropped ([PROGRESS.md](PROGRESS.md) ❌). Flip to assert `printErr` fires once the warning lands.
- `LoadsCaveLogAndHikeLogPolymorphically` — mixed fixture, both subclasses instantiated with their specific fields.

### §4. Round-trip — the highest-value single test

Build graph → save → load into fresh `Disk` → compare. Covers `userToStr`, `logToStr`, `partToStr`, `parseDisk`, `splitByObjects`, all three `initX`, all three `loadX`, and counter restoration.

This test would pin the **3-arg `Log` constructor bug** ([src/Log.cpp:69-73](../src/Log.cpp#L69-L73), [PROGRESS.md §Domain models](PROGRESS.md)): `EXPECT_EQ(log->getDate(), "2026-01-15")` fails because the constructor silently drops `date`. Mark `DISABLED_` until fixed so CI stays green while the expected-fail is committed.

Also: a test where an orphan `Log*` sits in `Disk::logs` but isn't reachable from any user — after save/reload, assert it's gone. Pins the intentional drop at [src/State.cpp:64-109](../src/State.cpp#L64-L109).

### §5. Polymorphic `logToStr` — `TYPED_TEST_SUITE`

```cpp
using LogTypes = testing::Types<CaveLog, HikeLog>;
TYPED_TEST_SUITE(LogSerialisationTest, LogTypes);

TYPED_TEST(LogSerialisationTest, FirstLineIsObjectHeader) {
    auto* log = this->make();
    Disk d;
    auto lines = d.logToStr(log);
    EXPECT_TRUE(lines[0].starts_with("object: "));
    delete log;
}
```

Applies to any property that should hold for any `Log` subclass — round-trip, `owner-id` present, `---` divider.

### §6. Memory / cascade — ASan, not gtest assertions

- Add a second target `AdventureLogTests_asan` with `-fsanitize=address,undefined`. CI runs both.
- `~User` cascades to `~Log` cascades to `~Participant` — build the graph, let it go out of scope, ASan catches double-frees (regression guard for `8954afc`).
- `Disk::removeUser` ([src/Disk.cpp:423-431](../src/Disk.cpp#L423-L431)) — after call, user absent from `disk.getUsers()`, subtree freed.
- Document the `User::removeLog` dangling-pointer wrinkle ([CONTEXT.md §Memory ownership](CONTEXT.md)) as a test: after `u->removeLog(l)`, `u->getLogs()` lacks `l` but `disk.getLogs()` still contains it.

### §7. Sort comparators — plain `TEST`s

`Log::sortByID`, `Log::sortByDuration` ([include/Log.h:53-54](../include/Log.h#L53-L54)). Pure-function tests, no fixture. Worth having independent of the broken `UI::sortID` so the comparator is trustworthy when the UI is rewritten.

### §8. Death test candidate

`EXPECT_DEATH` fits the size-mismatch second loop at [src/State.cpp:89-97](../src/State.cpp#L89-L97) if it genuinely throws out-of-range as [PROGRESS.md](PROGRESS.md) suggests. One test to pin current behaviour before the eventual rewrite.

---

### Suggested ordering

1. CMake wiring + `TEST(Sanity, Passes)` — proves toolchain.
2. `DiskHelper` parameterised (§1).
3. Sort comparators (§7) — zero-fixture quick win.
4. Object init (§2).
5. Load fixture (§3).
6. Round-trip (§4).
7. Typed tests (§5).
8. ASan target (§6).
