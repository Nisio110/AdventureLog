# CONTEXT.md

Active memory store of AdventureLog's current state — specific symbol locations, non-obvious invariants, gotchas, and conventions. Much more detailed than [PROJECT.md](PROJECT.md), and kept current: stale entries should be revised or removed as the project evolves. See [CLAUDE.md](../CLAUDE.md) for maintenance guidance.

## Disk format — exact shape

- Objects are blocks of `key: value` lines (exact separator is the two-char string `": "`), terminated by a `---` divider. The first line of every block is always `object: <TypeName>`.
- Whitespace: lines are read with `std::getline(diskFile >> std::ws, line)` at [src/Disk.cpp:139](../src/Disk.cpp#L139), so leading whitespace is stripped at read time — indentation in `disk.yaml` has no semantic meaning.
- The full key and value vocabulary lives in `namespace keys` at [include/Disk.h:112-141](../include/Disk.h#L112-L141). Unknown keys are silently ignored by `strToKVPair`.
- Object type values: `User`, `CaveLog`, `HikeLog`, `Participant`. CaveLog and HikeLog discriminate via the `object:` line — there is no separate `type:` field.
- `id` is an integer written as a string, parsed via `DiskHelper::strToNum` which calls `stoul(base=10)`. Booleans are literal `"true"` / `"false"` strings — anything else triggers `printErr` and returns `false`.

## Load pipeline — strict child-first order

`Disk::loadFromDisk` at [src/Disk.cpp:169-186](../src/Disk.cpp#L169-L186):

1. `parseDisk()` reads lines into `diskContents`, converts each to a `KeyValue` via `strToKVPair`, then `splitByObjects` cuts the list on `---` dividers.
2. Loads in order: `loadParticipants` → `loadLogs` (wires each `Log`'s participants by matching `Participant::logId == Log::id`) → `loadUsers` (wires each `User`'s logs by matching `Log::userId == User::id`).
3. Restores the three static ID counters to the max ID seen per type, so newly created objects don't collide with loaded ones.

## Save pipeline — user-rooted walk, full rewrite

`State::save()` at [src/State.cpp:64-109](../src/State.cpp#L64-L109):

- Walks `users → u->getLogs() → log->getParticipants()`. Anything not reachable from a `User` is silently dropped — intentional, since the domain forbids orphan logs/participants.
- Buffers are concatenated in group order: all users first, then all logs, then all participants. Save order (parent-first) is deliberately opposite to load order (child-first).
- `Disk::writeToDisk` opens the file with `ios::out | ios::trunc` at [src/Disk.cpp:570](../src/Disk.cpp#L570) — the file is fully rewritten each save, never appended to.
- `~State()` at [src/State.cpp:121-123](../src/State.cpp#L121-L123) calls `save()`, so autosave only happens on clean exit. A crash or SIGKILL drops every change made since the session started.
- In-memory check at [src/State.cpp:80-97](../src/State.cpp#L80-L97) compares `disk.getUsers()` sizes against `State::users` sizes. Important to understand: `State::users` is populated via `users = disk.getUsers()` at load ([src/State.cpp:10](../src/State.cpp#L10)), and `Disk::getUsers()` returns a reference — so both vectors hold the **same `User*` pointers**. The check compares sizes, not deep state, and can only ever detect divergence introduced by `State::addUser` / `State::removeUser`. It cannot catch a mutated-but-same-size graph.

## ID counters

- Static: `User::numUsers`, `Log::numLogs`, `Participant::numParticipants`. All initialised to 0 at translation-unit scope ([src/User.cpp:5](../src/User.cpp#L5), [src/Log.cpp:9](../src/Log.cpp#L9), [src/Participant.cpp:6](../src/Participant.cpp#L6)).
- On load, each counter is set to the **max ID found** in its own collection. Deletions do **not** decrement the counter — IDs only ever grow.
- `User::generateID` and `Log::generateID` bump the counter via `++numFoo`; `Participant::generateID` uses `setNumParticipants(numParticipants+1)`. Functionally equivalent, stylistically inconsistent.
- The comment above `Log::generateID` at [src/Log.cpp:43-49](../src/Log.cpp#L43-L49) ("by dereferencing the pointer") is misleading — the parameter is a reference, not a pointer, and the body also mutates `numLogs` directly.

## Memory ownership

- Raw `new` / `delete` throughout; no smart pointers.
- `Disk::~Disk` at [src/Disk.cpp:416-423](../src/Disk.cpp#L416-L423) is the **single bulk cleanup site** — it deletes every loaded `User*`, `Log*`, `Participant*`.
- `State` holds its `Disk` by value, so `~State()` triggers `~Disk()` after the final `save()`.
- `User::removeLog` and `Log::removeParticipant` call `delete` on the matched pointer without detaching from `Disk::logs` / `Disk::participants`, which causes a double-free at exit (both containers hold the same pointers after load). The safe pattern is the one used by `State::removeUser` → `Disk::removeUser` ([src/Disk.cpp:425-433](../src/Disk.cpp#L425-L433)): the parent layer erases from its own vector without deleting, then delegates the actual `delete` + erase to `Disk`'s coordinated remover. Equivalent `Disk::removeLog` / `Disk::removeParticipant` helpers don't exist yet — they'd be needed to make `User::removeLog` / `Log::removeParticipant` safe.
- `User::~User` and `Log::~Log` are trivial (the `Log` destructor is `virtual ~Log() = default;` at [include/Log.h:47](../include/Log.h#L47)) — children are not cascade-deleted from parent destructors.

## UI menu mechanics

Main dispatcher is `ui(State&)` at [src/UI.cpp:6](../src/UI.cpp#L6), driven by an `int menu` state variable:

| `menu` | screen                          | choice source        |
|--------|---------------------------------|----------------------|
| `0`    | startup (login / signup / exit) | `startupMenu()`      |
| `1`    | main menu                       | `mainMenu()`         |
| `2`    | log list + paging               | `logMenu(page)`      |

Menu-choice conventions from the printed menus in [src/UI.cpp](../src/UI.cpp):

- **startupMenu**: `1` login, `2` signup, `3` exit (the only transition that actually works end-to-end — exit calls `s.save()` and returns).
- **mainMenu**: `1` view logs, `2` add logs, `3` user settings, `4` logout.
- **logMenu**: `1–6` select log on current page, `7` next page, `8` prev page, `9` back to main, `0` sort.
- **userSettings** (currently unreachable): `1` change username, `2` change password, `3` delete account, `4` back.

All prompts use `cin >> choice;` with no validation. Non-integer input will leave `cin` in a failed state that affects subsequent reads — worth handling explicitly once the menus are actually wired up.

## Test framework quirks

Defined in [src/Tests.cpp](../src/Tests.cpp):

- `Tests::verifyTest()` prints the prompt `Was the test successful? [Y/n]` and reads a line from stdin. Returns `true` only for first-char `Y` or `y`. Tests that call it **block for interactive input**, so they can't be driven from a CI runner as-is.
- `Tests::div()` (no arg) returns a 24-char `=` bar plus newline.
- `Tests::div(int x)` returns `round(pow(e, x))` `=` characters — i.e. widths scale exponentially. Capped at `x > 5` with `"THE DIV IS TOO STRONG!!! ABORT! ABORT! ABORT! ABORT!"` ([src/Tests.cpp:28-35](../src/Tests.cpp#L28-L35)). The inline comment makes clear this is intentional.
- `Tests::ret` = `" : "`. Used as a label/value separator in printed test output.
- Test namespaces (`UserTests`, `LogTests`, `ParticipantTests`) are declared per-class in each class's header. See PROGRESS.md for which are actually implemented.

## Build specifics

- CMake 3.25+, C++26, compiler hardcoded to `/bin/g++` at [CMakeLists.txt:2](../CMakeLists.txt#L2). Portability to clang or a different g++ path currently requires editing the CMakeLists directly.
- Sources picked up via `file(GLOB src/*.cpp)` at [CMakeLists.txt:8](../CMakeLists.txt#L8) — adding a new `.cpp` requires re-running `cmake -S . -B build` before incremental builds find it.
- `-Werror` is on; `-Wall -Wpedantic` is present but commented out at [CMakeLists.txt:13-14](../CMakeLists.txt#L13-L14). Re-enabling would almost certainly surface warnings from the current UI code.
- Build dir uses Ninja; the binary is `build/AdventureLog`.

## Filesystem layout

- [disk-template.yaml](../disk-template.yaml) — canonical schema example, committed.
- `disk.yaml` — live user data, **gitignored**. Loaded by default from `defaultDiskPath = "../disk.yaml"` at [include/Disk.h:38](../include/Disk.h#L38) — i.e. the binary expects to be run from one level below the project root (typically `build/`).
- `disk copy.yaml` sometimes appears in the working tree — a user-made backup, not a project convention. Safe to ignore; not referenced by any code.
- `.gitignore` excludes: `build/`, `.DS_Store`, `.vscode/`, `.claude/`, `.cache/`, `disk.yaml`.

## Entry point

[src/Main.cpp](../src/Main.cpp) is intentionally minimal:

```cpp
int main() { State s; }
```

All lifecycle (load → UI → save) happens inside `State`'s constructor/destructor chain. A `basicTest()` free function is defined beneath `main` but never called — scratch code, not part of the runtime flow.

## Style inconsistencies worth knowing

- [src/UI.cpp:4](../src/UI.cpp#L4) has a full `using namespace std;` at file scope. Other `.cpp` files use selective `using std::foo;` declarations. Headers never pull `std` in.
- Constructor parameter naming: `Participant` uses underscore-prefixed names (`_name`, `_logID`), `User` and `Log` use short forms (`n`, `p`, `l`). No project-wide convention.
- The default `User()` no-arg constructor ([src/User.cpp:23](../src/User.cpp#L23)) does not call `generateID()` — it leaves `id` zero-initialised. `Disk::initUser` relies on this (it calls `u->setID(...)` manually after loading from disk). New-from-UI user creation should prefer `User(name, passwd)`, which does call `generateID()`.
- Headers in [include/](../include/) are included from [src/](../src/) using `#include "../include/Foo.h"` — not just `"Foo.h"`, even though `target_include_directories(... include)` would allow the shorter form.
