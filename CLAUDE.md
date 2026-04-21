# AdventureLog

A personal C++ logger for caving and hiking trips. Single-user CLI app that loads records from disk on startup, lets the user browse and edit them through a menu-driven interface, and flushes changes back to disk on exit.

## Build

- **CMake** (min 3.25), generator is Ninja in [build/](build/).
- **C++26**, compiled with `g++` and `-Werror`.
- `file(GLOB src/*.cpp)` picks up all sources — adding a new `.cpp` under [src/](src/) requires re-running cmake configure.

Typical workflow: `cmake -S . -B build -G Ninja && cmake --build build`, then run `./build/AdventureLog`.

## Architecture

The dependency chain is strictly layered:

```
UI  ->  State  ->  Disk  ->  models (User, Log, Participant)
```

Each layer only knows about the one directly beneath it. This ordering is intentional and should be preserved.

- **`UI`** — interactive menu loop; reads input, calls into `State` to mutate data, calls `state.save()` on explicit user action. Currently the most incomplete layer (many handlers are stubs or placeholders).
- **`State`** — the session orchestrator. Owns the `Disk` instance and the in-memory `vector<User*>`. Its constructor triggers `loadSave()` and hands control to `ui(*this)`; its destructor calls `save()`. This is the only place load and save are wired together.
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

A custom flat YAML-ish format. Each object is a block of `key: value` lines, terminated by a `---` divider. See [disk-template.yaml](disk-template.yaml) for the canonical schema example.

- **`disk-template.yaml`** — committed, serves as the format reference.
- **`disk.yaml`** — the live user-data file, gitignored.
- Parser is hand-rolled — not a real YAML parser. Keys that don't match the constants in `namespace keys` (see [include/Disk.h](include/Disk.h)) are silently ignored.

Save is `ios::trunc` — the whole file is rewritten on every save, there is no incremental append.

## Directory layout

- [include/](include/) — header files, one per class/module.
- [src/](src/) — implementations, paired 1:1 with headers. `Main.cpp` is trivial and just constructs a `State`.
- [docs/](docs/) — UML diagrams, class-flow graphs, and planning artifacts (Excalidraw, PlantUML, Graphviz). Useful for understanding intended structure at a glance.
- [build/](build/) — CMake/Ninja output, gitignored.
- [CMakeLists.txt](CMakeLists.txt) — build config.

## Conventions

- Headers in `include/` are included from `src/` using relative paths (`#include "../include/Foo.h"`).
- Each model class has a matching `FooTests` namespace declared in its header with ad-hoc interactive tests; shared helpers live in [include/Tests.h](include/Tests.h) (`Tests::verifyTest`, `Tests::div`). There is no automated test runner — tests prompt the user via stdin.
- `DiskHelper` namespace ([src/Disk.cpp](src/Disk.cpp)) holds free-function utilities (string conversions, logging, KV parsing). `keys` namespace holds the disk format's string constants — always prefer those over string literals when touching persistence.
- Error reporting is via `DiskHelper::printErr` which prints red `[ ERROR ]` to stderr. No exceptions are thrown from the persistence layer.
- Memory: raw `new` / `delete`, with `Disk` and `User` destructors responsible for cleaning up owned pointers. No smart pointers in use.

## Current status

Core persistence and domain model are in place and functional. Known incomplete areas:

- **UI layer** is largely stubbed. Several handlers (`signUp`, `logIn`, `loadLog`, `sortLogs`, log-menu actions, user settings) are placeholders that don't yet mutate state.
- **Authentication** is a no-op — `logIn()` always returns true.
- **Sorting helpers** in UI (`sortID`, `sortDuration`) are partially written and don't produce correct output.
- **Editing and deletion flows** for logs and accounts exist as declarations but have no implementations.

The **shape** of the project (layering, model relationships, disk format) is stable; what's changing is the fill-in of UI behaviour on top of that foundation.
