# AdventureLog

A personal C++ logger for caving and hiking trips. Single-user CLI: it loads your records from disk on startup, lets you browse and create them through a menu, and flushes changes back to disk on exit.

The goal is a small, self-contained record-keeper — somewhere to write down where you went, who came along, how long you were underground, whether you rigged, what the weather was like — without a database, a web service, or an account anywhere but your own machine.

## What it logs

Two kinds of outings, sharing a common `Log` base:

- **CaveLog** — cave name, area, duration, whether the trip was rigged, whether you led it, whether it was an SRT cave.
- **HikeLog** — area, duration, distance, weather.

Each log belongs to a **User** and carries zero or more **Participants** (the people you went with). Relationships are stored by ID; [disk-template.yaml](disk-template.yaml) shows the canonical on-disk shape.

## Status

What works today:

- Sign up and log in (name + password — see *Caveats* for the storage model).
- Create cave or hike logs with optional participants.
- Browse logs in a paginated list and inspect individual entries.
- Auto-save on clean exit; alternate save file via `argv[1]`.

Known gaps:

- Editing and deleting existing logs are placeholder stubs.
- Log sorting compiles but does not sort correctly.
- User-settings flow (change username / change password / delete account) is unimplemented; the menu entry is hidden until those handlers exist.
- Saves only happen on a clean exit — a crash, kill, or `Ctrl-C` mid-session drops every change made since startup.

## Build and run

Requirements: CMake ≥ 3.25, Ninja, `g++` with C++26 support.

```sh
cmake -S . -B build -G Ninja
cmake --build build
./AdventureLog
```

The executable is emitted into the project root (see [CMakeLists.txt](CMakeLists.txt)). With no argument, it loads `disk.yaml` from the current working directory. Pass an alternate path as `argv[1]` to use a different save file:

```sh
./AdventureLog ~/saves/caving-2026.yaml
```

If the path can't be opened, you'll be reprompted for a valid one rather than the program exiting.

`-Werror` is on; `-Wall -Wpedantic` are currently commented out. Sources are picked up with `file(GLOB src/*.cpp)`, so adding a new `.cpp` under [src/](src/) requires re-running `cmake -S . -B build` before incremental builds will see it.

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

The parser is hand-rolled — not a real YAML parser. Keys that don't match the constants in `namespace keys` ([include/Disk.h](include/Disk.h)) are silently ignored. Saves rewrite the whole file in full (`ios::trunc`); there is no incremental append.

## Caveats

- **Passwords are stored in plain text** alongside the rest of the data. This is a personal-use logger for trusted, single-user setups — treat `disk.yaml` like any other private document on your machine, not like a credentials store.
- **Hand-editing the save file is allowed but unforgiving.** Lines that don't match the expected `key: value` shape are silently dropped, and any log or participant whose owning ID doesn't resolve is discarded on the next save.

Design artifacts (UML, flow graphs, early planning) live in [docs/](docs/).
