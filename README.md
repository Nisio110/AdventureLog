# AdventureLog

A personal C++ logger for caving and hiking trips. Single-user CLI: it loads your records from disk on startup, lets you browse and edit them through a menu, and flushes changes back to disk on exit.

The goal is a small, self-contained record-keeper — somewhere to write down where you went, who came along, how long you were underground, whether you rigged, what the weather was like — without a database, a web service, or an account anywhere but your own machine.

> Status: core persistence and the domain model are working. The interactive UI layer is partly stubbed; see [llm/PROGRESS.md](llm/PROGRESS.md) for an honest feature-by-feature breakdown.

## What it logs

Two kinds of outings, sharing a common `Log` base:

- **CaveLog** — cave name, area, duration, whether the trip was rigged, whether you led it, whether it was an SRT cave.
- **HikeLog** — area, duration, distance, weather.

Each log belongs to a **User** and carries zero or more **Participants** (the people you went with). Relationships are stored by ID; [disk-template.yaml](disk-template.yaml) shows the canonical on-disk shape.

## Build and run

Requirements: CMake ≥ 3.25, Ninja, `g++` with C++26 support.

```sh
cmake -S . -B build -G Ninja
cmake --build build
./AdventureLog
```

The executable is emitted into the project root (see [CMakeLists.txt](CMakeLists.txt)). `-Werror` is on; `-Wall -Wpedantic` are currently commented out.

Note: sources are picked up with `file(GLOB src/*.cpp)`, so adding a new `.cpp` under [src/](src/) requires re-running `cmake -S . -B build` before incremental builds will see it.

## Project structure

```text
AdventureLog/
├── include/            # Headers — one per class/module
├── src/                # Implementations, paired 1:1 with headers
├── docs/               # UML, class-flow graphs, Excalidraw planning
├── llm/                # Project knowledge for AI collaborators
├── disk-template.yaml  # Canonical example of the disk format
├── disk.yaml           # Live user data (gitignored)
└── CMakeLists.txt
```

### Layers

The dependency chain is strictly one-directional:

```text
UI  →  State  →  Disk  →  models (User, Log, Participant)
```

Each layer only knows about the one directly beneath it.

- **UI** ([src/UI.cpp](src/UI.cpp)) — interactive menu loop. Reads input, asks `State` to mutate data, asks `State` to save on explicit user action.
- **State** ([src/State.cpp](src/State.cpp)) — session orchestrator. Owns the `Disk` instance and the in-memory `vector<User*>`. Its constructor loads from disk and hands control to the UI; its destructor saves. This is the only place load and save are wired together.
- **Disk** ([src/Disk.cpp](src/Disk.cpp)) — parses and emits the on-disk format, and reconstructs the in-memory pointer graph on load (participants first, then logs, then users).
- **Models** — [User.h](include/User.h), [Log.h](include/Log.h) (with `CaveLog`/`HikeLog` subclasses), [Participant.h](include/Participant.h). Each uses a static ID counter to hand out fresh IDs.

## Persistence format

A flat, YAML-ish text format. Each object is a block of `key: value` lines, terminated by a `---` divider:

```yaml
object: CaveLog
id: 1
owner-id: 1
date: 28/02/2026
area: Near Lisdoonvarna, Clare
duration-mins: 210
cave-name: Faunarooska
rigging: false
cave-leader: false
srt-cave: false
---
```

The parser is hand-rolled — not a real YAML parser. Keys that don't match the constants in `namespace keys` ([include/Disk.h](include/Disk.h)) are silently ignored. Saves rewrite the whole file (`ios::trunc`); there is no incremental append.

## Further reading

The `llm/` directory is written for AI pair-programmers but is equally useful to a human reader:

- [llm/PROJECT.md](llm/PROJECT.md) — stable architectural overview.
- [llm/CONTEXT.md](llm/CONTEXT.md) — running log of specific, project-local gotchas.
- [llm/PROGRESS.md](llm/PROGRESS.md) — feature-level status tracker.

Design artifacts (UML, flow graphs, early planning) live in [docs/](docs/).
