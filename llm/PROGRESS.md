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
- 🟡 **Disk-State mismatch check is structurally meaningless** ([src/State.cpp:80-97](../src/State.cpp#L80-L97)) — `State::users` and `disk.getUsers()` share the same `User*` pointers (State loads via `users = disk.getUsers()`), so the size-comparison check can only ever detect divergence from `State::addUser` / `State::removeUser`. With both add and remove paths now syncing into `Disk::users` (`State::createUser` calls `disk.addUser` at [src/State.cpp:143](../src/State.cpp#L143), `State::removeUser` calls `disk.removeUser`), the check can no longer trip via the normal API at all — its remaining failure modes are dev-introduced bugs (a future caller of `addUser` that forgets the sync). The second loop at [src/State.cpp:89-97](../src/State.cpp#L89-L97) also runs unconditionally and can throw out-of-range if the first check tripped. Worth removing the whole block, or replacing it with a real invariant (e.g. pointer-identity walk).
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

## UI layer — partially complete

`UI` is a class ([include/UI.h](../include/UI.h)) with a `State s` member; `main` constructs it and calls `UI::run()`. The core loop ([src/UI.cpp:15-62](../src/UI.cpp#L15-L62)) is now structurally sound, and the major read/create flows (auth, log browsing, log creation) work end-to-end. Outstanding work is concentrated in mutation/settings handlers and the broken sort helpers.

### Menu loop

- ✅ **Inner-switch dispatch.** `mainMenu` dispatch ([src/UI.cpp:43-56](../src/UI.cpp#L43-L56)) and `logMenu` dispatch ([src/UI.cpp:209-242](../src/UI.cpp#L209-L242)) both have proper `break`s — the case fall-through bug is gone.
- ✅ **Logout flow.** `case LOGOUT: quit = quitMenu(); break;` at [src/UI.cpp:53-55](../src/UI.cpp#L53-L55) — wired up correctly.
- 🟡 **User Settings is intentionally not exposed.** The `SETTINGS` enum value ([include/UI.h:71](../include/UI.h#L71)) and its dispatch case ([src/UI.cpp:50-52](../src/UI.cpp#L50-L52)) are both commented out, so the menu hides it from the user. This is a deliberate "wait until the underlying handlers exist" hide rather than a bug — once `changeUsername` / `changePassword` / `deleteAccount` are implemented, both comment-outs need to come back. `userSettings()` itself ([src/UI.cpp:247-258](../src/UI.cpp#L247-L258)) still prompts and immediately returns without acting on the choice.

### Auth

- ✅ `UI::logIn()` ([src/UI.cpp:100-117](../src/UI.cpp#L100-L117)) delegates to `State::logIn(username, password)` ([src/State.cpp:147-162](../src/State.cpp#L147-L162)), which does proper name+password verification and throws on mismatch. Caller catches `runtime_error` and reprompts.
- ✅ `UI::signUp()` ([src/UI.cpp:119-143](../src/UI.cpp#L119-L143)) has its own flow: unique-name check loop (via `s.isUniqueUsername`), password confirmation loop, `s.createUser`, then `s.save()`.

### Log browsing & creation

- ✅ `UI::logMenu()` ([src/UI.cpp:166-245](../src/UI.cpp#L166-L245)) — full listing of up to 6 logs per page with `currentPage`-based offset, prev/next paging, per-slot dispatch into `viewLog`, plus sort/back/quit options.
- ✅ `UI::viewLog(size_t)` ([src/UI.cpp:261-296](../src/UI.cpp#L261-L296)) — prints the selected log via `Log::print()`, then offers back-to-overview / back-to-main. Edit / Delete options are commented-out placeholders pending the mutation handlers.
- ✅ `UI::logCreator()` ([src/UI.cpp:298-406](../src/UI.cpp#L298-L406)) — formerly the unimplemented `addLog`. Builds a `CaveLog` or `HikeLog` from prompts, attaches participants, calls `User::addLog`, then `s.save()`.
- ❌ `UI::sortLogs()` ([src/UI.cpp:547-549](../src/UI.cpp#L547-L549)) — empty body. Wired to a `SORT_MENU` choice in `logMenu` but a no-op when invoked.
- 🟡 `UI::sortID(vector<Log*>)` ([src/UI.cpp:510-526](../src/UI.cpp#L510-L526)) — compiles but does not sort: stray `;` after the `if` makes the `pivot = test` block run unconditionally, and `outputs` is `.at()`-assigned without ever being sized. Same shape in `UI::sortDuration` ([src/UI.cpp:528-544](../src/UI.cpp#L528-L544)).

### Log / account mutation

All declared in [include/UI.h](../include/UI.h), none defined:

- ❌ `editLog()` ([src/UI.cpp:410-412](../src/UI.cpp#L410-L412)) — placeholder body.
- ❌ `deleteLog()` ([src/UI.cpp:414-416](../src/UI.cpp#L414-L416)) — placeholder body.
- ❌ `changeUsername()` — declared, no definition.
- ❌ `changePassword()` — declared, no definition.
- ❌ `deleteAccount()` — declared, no definition.

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
