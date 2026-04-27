# AdventureLog

A personal C++ logger for caving and hiking trips. Single-user CLI app that loads records from disk on startup, lets the user browse and edit them through a menu-driven interface, and flushes changes back to disk on exit.

## Build

- **CMake** (min 3.25), generator is Unix Makefiles (the CMake default on Linux) in [build/](../build/).
- **C++26**, compiled with `g++` and `-Werror`.
- Sources are split into a static library `AdventureLogLib` (everything under `src/` except `Main.cpp`) plus the `AdventureLog` executable. The library exists so the test binary can link the project's code without a duplicate `main()`.
- `file(GLOB src/*.cpp)` picks up all sources — adding a new `.cpp` under [src/](../src/) or [tests/](../tests/) requires re-running cmake configure.
- **Tests** use GoogleTest, fetched via `FetchContent`. **Coverage** is opt-in via `-DENABLE_COVERAGE=ON` and produces an HTML report through `gcovr` when present.

Typical workflow:

```sh
cmake -B build && cmake --build build
./AdventureLog                   # run the program
ctest --test-dir build           # run the test suite
```

For a coverage report: `cmake -B build -DENABLE_COVERAGE=ON && cmake --build build --target coverage` (requires `gcovr`).

## Architecture

The dependency chain is strictly layered:

```
UI  ->  State  ->  Disk  ->  models (User, Log, Participant)
```

Each layer only knows about the one directly beneath it. This ordering is intentional and should be preserved.

- **`UI`** — class that owns a `State` member by value and drives the interactive menu loop via `UI::run()`. `main` constructs a `UI`, which in turn constructs its `State` (loading the save), then calls `run()`. Reads input, calls into `s` to mutate data, calls `s.save()` on explicit user action. Currently the most incomplete layer (many handlers are stubs or placeholders).
- **`State`** — the session orchestrator. Owns the `Disk` instance and the in-memory `vector<User*>`. Its constructor triggers `loadSave()`; its destructor calls `save()`. Lifecycle is driven from `UI`/`main` — `State` no longer reaches up into the UI layer.
- **`Disk`** — parses and emits the on-disk format, plus holds raw pointers to loaded objects. Knows about all three model types and wires their relationships on load.
- **Models** — `User`, `Log` (with subclasses `CaveLog` and `HikeLog`), `Participant`. Each has a static ID counter (`numUsers`, `numLogs`, `numParticipants`) used to hand out fresh IDs.

## Domain model

- **User** owns zero or more **Logs**.
- **Log** (abstract base, with **CaveLog** and **HikeLog** subclasses) owns zero or more **Participants**.
- **Participant** is scoped to a single Log.

Relationships are stored **by ID**, not by embedding:
- `Log::userId` points at the owning user (key: `owner-id` on disk).
- `Participant::logId` points at the owning log (key: `log-id` on disk).

On load, `Disk::loadFromDisk` reconstructs the pointer graph in dependency order: participants first, then logs (which wire up their participants), then users (which wire up their logs).

## Persistence format

A custom flat YAML-ish format. Each object is a block of `key: value` lines, terminated by a `---` divider. See [disk-template.yaml](../disk-template.yaml) for the canonical schema example.

- **`disk-template.yaml`** — committed, serves as the format reference.
- **`disk.yaml`** — the live user-data file, gitignored.
- Parser is hand-rolled — not a real YAML parser. Keys that don't match the constants in `namespace keys` (see [include/Disk.h](../include/Disk.h)) are silently ignored.

Save is `ios::trunc` — the whole file is rewritten on every save, there is no incremental append.

## Directory layout

- [include/](../include/) — header files, one per class/module.
- [src/](../src/) — implementations, paired 1:1 with headers. `Main.cpp` is trivial and just constructs a `UI` (which then constructs its `State` and runs the menu loop).
- [tests/](../tests/) — GoogleTest unit tests, one `test_*.cpp` per area (`test_user.cpp`, `test_disk_helper.cpp`, `test_participant.cpp`). Discovered automatically via `gtest_discover_tests`.
- [docs/](../docs/) — UML diagrams, class-flow graphs, and planning artifacts (Excalidraw, PlantUML, Graphviz). Useful for understanding intended structure at a glance.
- [build/](../build/) — CMake build output, gitignored. Includes the FetchContent'd gtest sources under `_deps/` and any `.gcno`/`.gcda` coverage artefacts when `ENABLE_COVERAGE=ON`.
- [CMakeLists.txt](../CMakeLists.txt) — build config.

## Conventions

- Headers in `include/` are included from `src/` using relative paths (`#include "../include/Foo.h"`).
- Automated tests live under [tests/](../tests/) and run through GoogleTest + CTest. The legacy `UserTests` / `LogTests` / `ParticipantTests` namespaces (declared in each model's header, with shared helpers in [include/Tests.h](../include/Tests.h)) are *interactive* checks that prompt the user via stdin — kept around but distinct from the automated suite, and not run by `ctest`.
- `DiskHelper` namespace ([src/Disk.cpp](../src/Disk.cpp)) holds free-function utilities (string conversions, logging, KV parsing). `keys` namespace holds the disk format's string constants — always prefer those over string literals when touching persistence.
- Error reporting is via `DiskHelper::printErr` which prints red `[ ERROR ]` to stderr. No exceptions are thrown from the persistence layer.
- Memory: raw `new` / `delete`, with `Disk` and `User` destructors responsible for cleaning up owned pointers. No smart pointers in use.

## Current status

Core persistence and domain model are in place and functional. Known incomplete areas:

- **UI layer** is largely stubbed. Several handlers (`signUp`, `logIn`, `loadLog`, `sortLogs`, log-menu actions, user settings) are placeholders that don't yet mutate state.
- **Authentication** is a no-op — `logIn()` always returns true.
- **Sorting helpers** in UI (`sortID`, `sortDuration`) are partially written and don't produce correct output.
- **Editing and deletion flows** for logs and accounts exist as declarations but have no implementations.

The **shape** of the project (layering, model relationships, disk format) is stable; what's changing is the fill-in of UI behaviour on top of that foundation.
