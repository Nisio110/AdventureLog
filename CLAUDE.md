# CLAUDE.md

Two companion files hold project knowledge:

- [PROJECT.md](llm/PROJECT.md) — the broad, stable project overview (architecture, domain model, persistence format, conventions, current status).
- [CONTEXT.md](llm/CONTEXT.md) — an accumulating log of specific, project-local knowledge (exact symbol locations, non-obvious invariants, gotchas, conventions that surface in conversation).

Read both at the start of a session when relevant. The update rules below govern how each one evolves.

## Updating PROJECT.md

Do not modify [PROJECT.md](llm/PROJECT.md) silently. When new context emerges that would warrant an update — a structural change, a new convention, a layer's responsibilities shifting, an incomplete area becoming complete, etc. — first propose the change to the user in the conversation:

- State **what** you want to add, remove, or revise (the concrete edit).
- State **why** — what changed in the project that makes the current wording wrong or incomplete.

Wait for the user to agree before writing the edit. Small wording fixes count too; the goal is that `PROJECT.md` only drifts when the user has explicitly signed off on each drift.

## Maintaining CONTEXT.md

[CONTEXT.md](llm/CONTEXT.md) is your **active memory store** — a live, specific picture of the project's current state, much more detailed than `PROJECT.md`. Treat it as yours to curate: add, revise, reorganise, overwrite, or delete entries freely so the file always reflects what is true *right now*. No prior permission needed for any of these operations.

Guidance:

- **Keep it current.** If you learn that a prior entry is wrong, stale, or has been superseded by newer information, update or delete it — do not leave outdated claims sitting next to fresh ones.
- **Keep it specific.** Generic programming notes do not belong there — only facts, locations, quirks, and behaviours that are true *about AdventureLog*. If a note would apply equally to any C++ project, it is probably not worth saving here.
- **Keep it organised.** Group related entries under shared headings and reshape the structure as the knowledge base grows. A single flat list will stop being useful quickly.
- **Cite locations** where useful using [file.ext:line](path#Lline) form so entries stay verifiable against the current codebase.
- **Prefer revising over duplicating.** Before appending a new entry, check whether an existing one covers the same topic and should be extended or rewritten instead.
