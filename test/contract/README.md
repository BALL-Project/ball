# Contract tests (`test/contract/`) — v1.7.4 / phase 999.60

The cross-surface **contract-test harness** that enforces the BALLView
mutation contract (`.planning/v1.7.4-ARCHITECTURE-CONTRACT.md`) by
behavior. Grep gates miss aliases, helpers, lambdas, macros, manager
indirection, processor mutation, friends, Python, MOC and base-pointer
calls — **grep is not the contract; these tests are** (§11).

Run all contract tests:

```sh
ctest --test-dir <build> -L contract --output-on-failure
```

The CI `contract-tests` job (Linux-only, blocking) runs exactly that.

## Harness-first sequencing (Codex MEDIUM #7)

`999.60` lands the harness + compile-fail infra **FIRST**, before `999.59`
cuts the controllers over to the §2 `bool apply()` contract. This breaks
the `999.59 ↔ 999.60` circular dependency. Consequences you will see here:

- `apply()` returns `void` today; the `bool` (mutated / dropped) contract
  lands in `999.59-01`. Assertions that need the bool ship **disabled**.
- `CommandRegistry` is compiled out (`BALL_UI_V2` was removed), and the §8
  command-body migration (`999.59-03`) has not landed. So the
  **toolbar / menu surfaces report `surfaceAvailable() == false`** — they
  must invoke the *registered command body*, never `action->trigger()`
  indirection, so they stay unavailable until the body exists.
- The owner setters (`Stage::setX` / `Representation::setX`) are still
  **public**; `999.59-04` privatises them behind `StageMutation` /
  `RepresentationBuilder`. The negative compile tests therefore EXPECT
  `pass` today and flip to EXPECT `fail` after `999.59-04`.

### The disable switch

Disabled fixtures are gated on `CONTRACT_FIXTURE_DISABLED`
(`contractTestHarness.h`), which is `1` until a controller cut-over defines
`BALL_VIEW_CONTROLLER_APPLY_RETURNS_BOOL`. A disabled `CHECK` compiles,
registers, and runs, but reports a `STATUS` skip instead of failing — so
`ctest -L contract` is green today and each `999.59-NN` plan flips its
controller's fixture to enabled by removing the prefix / defining the
macro and watching it go green.

## Files

| File | Role |
|------|------|
| `contractTestHarness.{h,C}` | Fixture base: headless boot, 4 surface helpers, `snapshotOwner()` POD. |
| `harness_selftest.C` | Proves the harness boots + that toolbar/menu surfaces are *not* faked (the plan's harness self-test acceptance criterion). |
| `{model,coloring,material,clipping,label,stage,camera,light,stereo}_contract_test.C` | The **9** per-controller parity fixtures (one per state domain). Enabled portion asserts the Inspector→owner postcondition; cross-surface parity ships disabled. |
| `selection_consumer_test.C` | §7 selection source-of-truth + (disabled) four-consumer agreement. |
| `reentrancy_test.C` | §2/§11b re-entrancy: guard shields a nested apply today; the bool-drop assertion is disabled until `999.59-01`. |
| `compilefail/owner_narrowing_*.C` | §3/§11c owner-narrowing negative (must-not-compile after `999.59-04`) + positive (must-always-compile) sources. |
| `compilefail/run_compile_check.cmake` | ctest driver that asserts each compile-fail TU's outcome matches the per-phase expectation. |

## How to flip a fixture green (for `999.59-NN` plan authors)

1. Land the controller's `bool apply()` cut-over per the §13 cookbook.
2. Define `BALL_VIEW_CONTROLLER_APPLY_RETURNS_BOOL` (once, in `999.59-01`)
   so `CONTRACT_FIXTURE_DISABLED` becomes `0`.
3. Wire the controller's toolbar/menu surface to the registered command
   body (`999.59-03`) so `surfaceAvailable(Toolbar/Menu)` returns true.
4. Run `ctest -L contract`; the `DISABLED_*_parity_across_surfaces` check
   for that controller must go green. If not, you have found the residual
   bypass — fix it, that is the whole point.
5. After `999.59-04` privatises the owner setters, flip the negative
   compile tests from `EXPECT pass` to `EXPECT fail` + `DIAG_MATCH` in
   `test/CMakeLists.txt`.

## Out of scope (v1.7.5 / v2.1)

- Per-frame visual / pixel diff (v1.7.5).
- Performance contract ("no apply > 50 ms") (v1.7.5).
- Deep PyBALL parity — the Python surface skips when `BALL_HAS_PYBALL=OFF`,
  dropping to 3-surface parity (v2.1).
- macOS/Windows headless coverage — Linux-only at v1.7.4 (matches
  `tabOrder_test` discipline; headless BALLView is brittle on macOS).
