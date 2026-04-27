# AdventureLog

A personal C++ logger for caving and hiking trips. Single-user CLI: it loads your records from disk on startup, lets you browse and create them through a menu, and flushes changes back to disk on exit.

The goal is a small, self-contained record-keeper — somewhere to write down where you went, who came along, how long you were underground, whether you rigged, what the weather was like — without a database, a web service, or an account anywhere but your own machine.

## Features

- Sign up and log in to your own per-user record set.
- Create cave or hike logs, tagged with the people you went with.
- Browse your logs in a paginated list and inspect any entry in detail.
- Auto-save on exit — your data lives in a plain-text file you can read, back up, or hand-edit with any text editor.
- Pass a path as `argv[1]` to keep separate save files (e.g. one per season, or one per project).

## What it logs

Two kinds of outings, sharing a common `Log` base:

- **CaveLog** — cave name, area, duration, whether the trip was rigged, whether you led it, whether it was an SRT cave.
- **HikeLog** — area, duration, distance, weather.

Each log belongs to a **User** and carries zero or more **Participants** (the people you went with). Relationships are stored by ID; [disk-template.yaml](disk-template.yaml) shows the canonical on-disk shape.

## Build and run

Requirements: CMake ≥ 3.25, `g++` with C++26 support.

```sh
cmake -B build
cmake --build build
./AdventureLog
```

The executable is emitted into the project root (see [CMakeLists.txt](CMakeLists.txt)). With no argument it loads `disk.yaml` from the current working directory; pass an alternate path to use a different save file:

```sh
./AdventureLog ~/saves/caving-2026.yaml
```

Sources are picked up with `file(GLOB src/*.cpp)`, so adding a new `.cpp` under [src/](src/) is a matter of dropping the file in and re-running `cmake -B build`.

## Project structure

```text
AdventureLog/
├── include/            # Headers — one per class/module
├── src/                # Implementations, paired 1:1 with headers
├── docs/               # UML, class-flow graphs, Excalidraw planning
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
- **State** ([src/State.cpp](src/State.cpp)) — session orchestrator. Owns the `Disk` instance and the in-memory `vector<User*>`. Its constructor loads from disk; its destructor saves. This is the only place load and save are wired together.
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

Save files are plain text — open them in any editor to read, back up, or move to another machine. The keys the format recognises are defined in `namespace keys` ([include/Disk.h](include/Disk.h)).

Design artifacts (UML, flow graphs, early planning) live in [docs/](docs/).
