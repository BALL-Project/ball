# K0.6 Adversarial Review — Codex CLI Round 7 (2026-05-18)

**Status:** Complete. Codex verdict: **fix-first** — 4 OPEN items
block K0.6.5; 2 FYI items tracked for K0.7.

**Reviewer:** Codex CLI 0.128.0 (`codex exec`).
**Subject:** K0.6.0–K0.6.3 (commits `ae22fe260..472e39c52`).

---

## Findings

| # | Sev | Probe | Verdict | Rationale |
|---|---|---|---|---|
| 1 | CLOSED | `MoleculeStore::clear()` invariants | OK | All columns + pool + intern + bonds + CSR + free-list cleared; `generation_` / `selection_generation_` bumped; `next_stable_id_` reset; `borrowed_ref_count_` is debug-only and aborts in `assert_no_borrowed_refs_` before clear runs if non-zero. Existing `Atom*` handles surviving clear is the K0.4 lifetime-contract issue, not new. |
| 2 | OPEN | stable_id loader hooks visibility | UNSAFE | `set_stable_id_for_load_` + `reseed_next_stable_id_` are inline public; any caller can break "unique stable_id per live atom" invariant. Trailing underscore is documentation, not protection. Make private + friend the loader, OR consolidate to one checked bulk-restore method. |
| 3 | OPEN | JSON major/minor version policy | UNSAFE for K0.6.5 | Higher-minor + unknown-key silent-accept policy is only safe for store-only additive data. When K0.6.5 writes a richer System document, a K0.6.3 `loadStoreJSON` could silently partial-load and drop the System tree. Add a `document_type: "MoleculeStore"` vs `"System"` discriminator OR a critical-extension list before K0.6.5. |
| 4 | OPEN | Bit-exact float parse error leak | UNSAFE | `hex_to_float_` calls `std::stoul` which throws `invalid_argument` / `out_of_range`. `decode_float_` doesn't catch. More broadly, every `.get<T>()` call in `loadStoreJSON` post-parse can leak nlohmann type-errors across the public API. Wrap the post-parse body in try/catch and rethrow as `Exception::ParseError`. |
| 5 | OPEN | `is_freed` laxity | INCONSISTENT | `selection` is strictly 0/1, but `is_freed` treats any non-zero as freed. Tighten to strict 0/1 + also validate `live_atom_count == count(is_freed==0)` for cross-field consistency. |
| 6 | CLOSED | Bond handling on freed-slot atoms | OK | Loader adds bonds first, frees last; CSR skips bonds touching freed slots; final state matches the writer's serialized in-memory state. The "must be live" comment in the code is misleading — actual check is `< n`; update the comment but not the behaviour. |
| 7 | FYI | Performance | K0.7 | Writer builds full nlohmann DOM then dumps. 13 column-level reserves (not 650k — I miscounted in the prompt). Fine for K0.6 ship-readiness; K0.7 should profile and consider direct streaming if persistence shows up on the hot path. |
| 8 | FYI | API ergonomics | Worth fixing now | `saveStoreJSON(s, os, -1, BIT_EXACT_HEX)` awkward (`-1` magic for compact). Options struct cleaner now, before System-level APIs mirror the shape. Not a blocker. |

---

## Remediation Plan (K0.6.3b)

| Item | Fix | Effort |
|---|---|---|
| #2 stable_id hooks | Make `set_stable_id_for_load_` + `reseed_next_stable_id_` private; expose one checked `restore_stable_ids_for_load_(vector<StableId>)` bulk helper; friend the loader translation unit (or a free function in the BALL namespace) | 1 hr |
| #3 document_type | Add `document_type: "MoleculeStore"` top-level field in writer; reader requires the field if format_minor > 0 OR explicitly rejects documents with unknown discriminator. K0.6.5 will write `"System"`. | 1 hr |
| #4 parse-error boundary | Wrap loadStoreJSON body in try/catch (json::exception, std::invalid_argument, std::out_of_range) → rethrow as Exception::ParseError with field context. | 1 hr |
| #5 is_freed strict | Validate strict 0/1; cross-check `live_atom_count == count(is_freed == 0)`. | 30 min |
| #6 comment | Fix misleading "bond endpoints must be live slots" comment. | 5 min |

Total: ~3.5 hours. Lands as **K0.6.3b** before K0.6.5 begins.

FYI items #7 + #8 deferred (perf to K0.7; options struct may
piggyback on K0.6.5 API additions for the System-level path).

---

## Verdict (Codex, verbatim)

> Fix X first. K0.6.5 should not land until the reader's exception
> boundary is closed and the v1 store-only vs future System JSON
> compatibility policy is made explicit. Stable-id loader hooks are
> also worth tightening before more persistence code starts
> depending on them.
