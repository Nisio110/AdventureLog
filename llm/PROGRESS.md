# PROGRESS.md

Feature-level status for AdventureLog. Kept current as work lands — prefer revising entries to stacking "now outdated" notes. See [PROJECT.md](PROJECT.md) for the broad overview and [CONTEXT.md](CONTEXT.md) for specific project knowledge.

Status key: ✅ working · 🟡 partial / has known issues · ❌ stub or not implemented

---

## Persistence (Disk layer)

- ✅ Parsing `disk.yaml` into in-memory objects (`Disk::loadFromDisk`).
- ✅ Writing back to disk with `ios::trunc` via `Disk::writeToDisk` ([src/Disk.cpp:575](../src/Disk.cpp#L575)).
- ✅ Serialization of User / CaveLog / HikeLog / Participant via the `*ToStr` methods.
- ✅ Polymorphic log dispatch in `logToStr` — `dynamic_cast` selects the CaveLog vs HikeLog branch.
- ✅ Static ID counters correctly restored on load (`User::numUsers`, `Log::numLogs`, `Participant::numParticipants`) — was buggy, now fixed at [src/Disk.cpp:183-185](../src/Disk.cpp#L183-L185).
- ✅ **Save walks user → logs → participants and drops unreachable children.** Previously flagged as "orphan drop"; confirmed intentional — the domain forbids orphan logs/participants, and the user-rooted walk enforces that invariant in the written file.
- ❌ **No load-time orphan check.** `Disk::loadLogs` and `Disk::loadUsers` silently skip participants/logs whose `logId` / `owner-id` doesn't match anything — the orphans stay in `Disk::`'s vectors but never attach to a parent, then get dropped on the next save. A hand-edited or corrupted `disk.yaml` loses data with no warning. Proposed fix: after `loadUsers`, count unattached entries in `Disk::logs` / `Disk::participants` and emit a single `printErr` if non-zero.
- 🟡 **Disk-State mismatch check is structurally meaningless** ([src/State.cpp:80-97](../src/State.cpp#L80-L97)) — `State::users` and `disk.getUsers()` share the same `User*` pointers (State loads via `users = disk.getUsers()`), so the size-comparison check can only ever detect divergence from `State::addUser` / `State::removeUser`, not real corruption or tampering. The second loop at [src/State.cpp:89-97](../src/State.cpp#L89-L97) also runs unconditionally and can throw out-of-range if the first check tripped.
- ❌ **`Disk::getLogs()` / `getParticipants()` should be made private** ([include/Disk.h:62-63](../include/Disk.h#L62-L63)). Under the cascade-ownership model, those vectors can hold dangling pointers mid-session (after `User::removeLog` / `Log::removeParticipant`), so external iteration is unsafe. Currently nobody outside `Disk` calls them, but the public visibility is an attractive-nuisance. Move under `protected:` alongside the storage vectors. `getUsers()` stays public — `State` needs it at load time.

## Domain models

- ✅ `User`, `Log`, `CaveLog`, `HikeLog`, `Participant` — fields, getters/setters, print methods.
- ✅ Add / remove helpers: `User::addLog`/`removeLog`, `Log::addParticipant`/`removeParticipant`.
- ✅ Static sort comparators declared on `Log` (`sortByID`, `sortByDuration`).
- 🟡 `Log(int uid, string date, string note)` 3-arg constructor ([src/Log.cpp:69-73](../src/Log.cpp#L69-L73)) silently drops its `date` argument — sets `userId` and `note` but never calls `setDate`. `HikeLog(uid, date, note)` at [src/Log.cpp:184](../src/Log.cpp#L184) inherits the bug via delegation.

## State layer

- ✅ `loadSave()` / `loadSave(diskPath)` — loads from disk and populates `users`.
- ✅ `addUser`, `removeUser` — `State::removeUser` now delegates the actual `delete` to `Disk::removeUser`, so the pointer is removed from `Disk::users` before `~Disk()` iterates. No more double-free.
- ✅ `save()` called from `~State()` (best-effort autosave on clean exit).
- ✅ `save()` also callable explicitly (used from UI exit path).

## UI layer — substantially incomplete

`UI` is now a class ([include/UI.h](../include/UI.h)) with a `State s` member; `main` constructs it and calls `UI::run()`. The core loop ([src/UI.cpp:12-70](../src/UI.cpp#L12-L70)) is structurally wrong in several ways, and most action handlers are empty. This is the primary area of ongoing work.

### Menu loop bugs

- ❌ **Case fall-through throughout the inner switches.** In `case 1:` (main menu dispatch, [src/UI.cpp:39-47](../src/UI.cpp#L39-L47)) and `case 2:` (log menu dispatch, [src/UI.cpp:49-65](../src/UI.cpp#L49-L65)), almost every inner `case` is missing its `break`, so control falls through into every subsequent case — every selection runs multiple handlers and ends at the `default` "Invalid Input" branch.
- ❌ **User Settings is unreachable.** `mainMenu()` returns `3` for that option ([src/UI.cpp:90](../src/UI.cpp#L90)), but the outer `switch (menu)` has no `case 3:` — only 0, 1, 2. Selecting settings does nothing.
- ❌ **Logout flow** — `mainMenu()` returns `4` for Logout, with no corresponding case.

### Auth

- 🟡 `UI::logIn()` ([src/UI.cpp:139-155](../src/UI.cpp#L139-L155)) reads username/password via `takeInput()` but does no credential verification — it unconditionally calls `s.createUser` (creating a fresh user from whatever was typed) and returns `true`. Effectively indistinguishable from signup.
- ❌ `UI::signUp()` ([src/UI.cpp:157-159](../src/UI.cpp#L157-L159)) just delegates to `logIn()`. With logIn's current behavior this happens to add a user, but for the wrong reasons — no separate signup-vs-login pathway exists.

### Log browsing

- ❌ `UI::logMenu()` ([src/UI.cpp:97-101](../src/UI.cpp#L97-L101)) prints the page number (using the `page` member) and returns `-1` — no log listing, no pagination logic, no selection.
- ❌ `UI::loadLog(int)` ([src/UI.cpp:198-200](../src/UI.cpp#L198-L200)) — empty body.
- ❌ `UI::sortLogs()` ([src/UI.cpp:202-204](../src/UI.cpp#L202-L204)) — empty body.
- 🟡 `UI::sortID(vector<Log*>)` ([src/UI.cpp:162-178](../src/UI.cpp#L162-L178)) — compiles but does not sort: stray `;` after the `if` makes the `pivot = test` block run unconditionally, and `outputs` is `.at()`-assigned without ever being sized. Same shape in `UI::sortDuration` ([src/UI.cpp:180-196](../src/UI.cpp#L180-L196)).

### Log / account mutation

All declared in [include/UI.h](../include/UI.h), none defined:

- ❌ `editLog()`
- ❌ `deleteLog()`
- ❌ `addLog()`
- ❌ `changeUsername()`
- ❌ `changePassword()`
- ❌ `deleteAccount()`

## Tests

Ad-hoc, interactive — each test prints data and prompts `Was the test successful? [Y/n]`. No automated runner; nothing in [src/Main.cpp](../src/Main.cpp) invokes them.

- ✅ `ParticipantTests::testConstructors` — implemented.
- ✅ `LogTests::testCaveLogConstructors` — implemented.
- ❌ `LogTests::testHikeLogConstructors`, `testParticipantsIO`, `testDisplay` — declared only. Calling them would cause a linker error.
- ❌ `UserTests::testCaveLogsIO`, `testHikeLogsIO`, `testConstructors`, `testGenerateID` — all empty function bodies.
- ℹ️ `basicTest()` in [src/Main.cpp:9-18](../src/Main.cpp#L9-L18) is defined but never called.

---

## Rough edges worth tracking

Not tied to a single feature, but worth not forgetting:

- Memory ownership is raw `new`/`delete`. Adding a new owning container anywhere means also writing the matching destructor cleanup.
- `CMakeLists.txt` uses `file(GLOB ...)` — adding a new `.cpp` requires re-running `cmake -S . -B build` before incremental builds pick it up.
- `-Werror` is on; `-Wall -Wpedantic` is commented out. Turning those back on will likely surface warnings in the current UI code.
