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
- **Cascading destructors.** `~User()` ([src/User.cpp:34-37](../src/User.cpp#L34-L37)) deletes every `Log*` in `logs`; `~Log()` ([src/Log.cpp:64-67](../src/Log.cpp#L64-L67)) deletes every `Participant*` in `participants`. Deleting a `User` therefore tears down the whole subtree. `~Participant()` is trivial (leaf).
- `Disk::~Disk` at [src/Disk.cpp:416-421](../src/Disk.cpp#L416-L421) deletes only `users` and then `.clear()`s the `logs` and `participants` vectors without deleting — their objects are freed via the cascade. This is the **single root cleanup site**.
- `State` holds its `Disk` by value, so `~State()` triggers `~Disk()` after the final `save()`.
- **Orphan caveat.** A `Log` or `Participant` that sits in `Disk::logs`/`Disk::participants` but is not reachable from any `User` will leak on exit — nothing deletes it. Acceptable while the domain forbids orphans; would matter if a future load-time orphan check ever decides to preserve them.
- **Mid-session removers.** `User::removeLog`, `Log::removeParticipant`, and `Disk::removeUser` ([src/Disk.cpp:423-431](../src/Disk.cpp#L423-L431)) each do `delete x;` on the matched pointer — and because of the cascade, that delete tears down `x`'s subtree automatically. Known wrinkle: `User::removeLog` / `Log::removeParticipant` don't detach from `Disk::logs`/`Disk::participants`, so those vectors can hold dangling pointers mid-session. Currently harmless because nothing outside `Disk` reads them (`Disk::getLogs` / `getParticipants` are never called from other translation units), but would bite if external access were ever added.

## UI menu mechanics

`UI` is a class ([include/UI.h](../include/UI.h)) that owns a `State s` member by value and holds the menu-loop state (`int menu {0}`, `int page {1}`, `bool loop {true}`) as members. Construction order: `main` builds `UI(path)` → `State` member is constructed (loads disk) → `main` calls `ui.run()` to enter the loop. `~UI` triggers `~State` which calls `save()`.

Main dispatcher is `UI::run()` at [src/UI.cpp:12](../src/UI.cpp#L12), driven by the `menu` member:

| `menu` | screen                          | choice source        |
|--------|---------------------------------|----------------------|
| `0`    | startup (login / signup / exit) | `startupMenu()`      |
| `1`    | main menu                       | `mainMenu()`         |
| `2`    | log list + paging               | `logMenu()`          |

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
- Build dir uses Unix Makefiles (driven via the `cmake-configure` / `cmake-build` tasks in [.vscode/tasks.json](../.vscode/tasks.json)). The binary lands at the workspace root (`AdventureLog`), not in `build/`, because [CMakeLists.txt:13](../CMakeLists.txt#L13) sets `RUNTIME_OUTPUT_DIRECTORY` to `${CMAKE_SOURCE_DIR}`.

## Filesystem layout

- [disk-template.yaml](../disk-template.yaml) — canonical schema example, committed.
- `disk.yaml` — live user data, **gitignored**. The default path is `defaultDiskPath = "disk.yaml"` (cwd-relative), declared as `public static inline` on `State` at [include/State.h:11](../include/State.h#L11). It's referenced as the default argument of both `State::State` ([include/State.h:30](../include/State.h#L30)) and `UI::UI` ([include/UI.h:13](../include/UI.h#L13)), and `Main.cpp`'s no-argv branch lets that default fire (see "Entry point" below). Since the binary lands at the project root per [CMakeLists.txt:13](../CMakeLists.txt#L13), the cwd-relative `"disk.yaml"` resolves correctly when launched from there.
- `disk copy.yaml` sometimes appears in the working tree — a user-made backup, not a project convention. Safe to ignore; not referenced by any code.
- `.gitignore` excludes: `build/`, `.DS_Store`, `.vscode/`, `.claude/`, `.cache/`, `disk.yaml`.

## Entry point

[src/Main.cpp](../src/Main.cpp) is intentionally minimal:

```cpp
int main(int argc, char* argv[]){
    if (argc > 1){
        UI ui(argv[1]);
        ui.run();
    } else {
        UI ui;
        ui.run();
    }
    return 0;
}
```

`UI`'s constructor builds its `State` member (which loads from disk), `run()` drives the menu loop, and `~UI` → `~State` triggers the autosave. The two-branch shape is deliberate: passing `argv[1]` explicitly when present, and constructing `UI` with **no argument** when absent so that the `State::defaultDiskPath` default actually kicks in. The earlier `UI ui(path)` form (with a local `path` defaulted to `""`) bypassed every default arg in the chain — `State` would receive `""`, `Disk::openFile("")` would fail, and the reprompt loop would catch it. That bug is gone now; with no argv, the default `"disk.yaml"` is loaded directly.

## Style inconsistencies worth knowing

- Constructor parameter naming: `Participant` uses underscore-prefixed names (`_name`, `_logID`), `User` and `Log` use short forms (`n`, `p`, `l`). No project-wide convention.
- The default `User()` no-arg constructor ([src/User.cpp:23](../src/User.cpp#L23)) does not call `generateID()` — it leaves `id` zero-initialised. `Disk::initUser` relies on this (it calls `u->setID(...)` manually after loading from disk). New-from-UI user creation should prefer `User(name, passwd)`, which does call `generateID()`.
- Headers in [include/](../include/) are included from [src/](../src/) using `#include "../include/Foo.h"` — not just `"Foo.h"`, even though `target_include_directories(... include)` would allow the shorter form.
