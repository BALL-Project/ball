---
phase: 09-test-suite-triage
type: baseline
status: research / pre-phase (Phase 9 not yet active-planned)
recorded: 2026-05-15
source-ref: v1.6.0 tag (commit afa00c2)
platform: macOS arm64 (Apple Silicon M-class, macOS 26.4.1)
---

# Phase 9 baseline: BALL unit test suite on v1.6.0 (Qt 5.15)

## Headline

**With `BALL_DATA_PATH` correctly set, the BALL unit test suite is 99% green on v1.6.0 source. Three real test failures need triage.**

Initial run with `BALL_DATA_PATH` unset showed 83% pass (244/294) — but 47 of those 50 "failures" were just the env var being missing. After exporting `BALL_DATA_PATH=$REPO/data`, the suite is 291/294 = 99%.

This dramatically shrinks Phase 9's main-work scope from "triage 50 broken tests after 4 years of modernization" to "investigate 3 specific failures + add the env-var export to CI" — days of work, not weeks.

## Methodology

```bash
git worktree add /tmp/ball-v160-test v1.6.0          # checkout the tagged commit
cd /tmp/ball-v160-test
cmake --preset ci-macos -DBALL_LICENSE=GPL            # ~8 sec
cmake --build --preset ci-macos --target BALL VIEW BALLView -j16  # ~1m 17s
cmake --build build/ci-macos --target build_tests -j16             # ~45s — compile all 295 test binaries
cd build/ci-macos
BALL_DATA_PATH=/tmp/ball-v160-test/data \
  ctest --output-on-failure --timeout 120 --output-junit junit.xml   # 32 sec
```

Hardware: Apple M-series 16-core CPU (`hw.ncpu = 16`). Suite is tiny — the 32-second total test run is the time to invoke 294 small executables in series; the actual work per test is sub-second for most.

## Result summary

| Metric | Without BALL_DATA_PATH | With BALL_DATA_PATH=$REPO/data |
|---|---|---|
| Total tests run | 294 | 294 |
| Passed | 244 | **291** |
| Failed | 50 | **3** |
| Pass rate | 83.0% | **99.0%** |
| Total test wall-clock | 79 sec | 32 sec |

The 47 tests that pass once `BALL_DATA_PATH` is set were all reading parameter files / fragment databases / rotamer libraries / training data from BALL's `data/` tree. Without the env var, `BALL::Path::find()` falls back to the compile-time `BALL_DATA_PATH` macro, which on a CI worktree points at a build-time path that doesn't exist on the test runner.

## The 3 remaining real failures

1. **`Directory_test`** — runs but emits no informative output before failing (the verbose `-V` flag should give a line number). Likely a hard-coded path expectation that doesn't hold on macOS Tahoe (e.g. `/tmp` is a symlink to `/private/tmp`, sandbox-style `/var/folders` paths, etc.). Low-risk fix once the failing assertion is identified.

2. **`AmberFF_test`** — `TEST_REAL_EQUAL(... total_energy): got 1680.36, expected 1638.84` at line 551. Total Amber energy off by **2.6%**. Three plausible causes:
   - Tolerance too tight for FP differences between the test's expected value (computed on Intel circa 2018) and Apple Silicon's IEEE-754 with potential ARM-specific FMA reordering
   - An algorithmic regression introduced by the C++17 migration or the modern-Boost bridge
   - A parameter-file format expectation that drifted (less likely — most other force-field tests pass)
   - Triage: re-run on Linux x64 GCC to see if the energy matches expected there; if yes → ARM FP precision is the cause; if no → real regression.

3. **`AssignBondOrderProcessor_test2`** — `TEST_EQUAL(compareBondOrder(sys), true): got 0, expected 1` at line 1010, under `Option::USE_FINE_PENALTY`. One specific code path (fine-penalty bond order) produces wrong assignment in test 2; test 1 of the same processor passes. Likely a real algorithmic bug or a parameter that drifted; needs reading the bond-order ILP solver code (Plan: walk through the option-routing logic).

## Subsystem coverage breakdown

Tests by subsystem in the JUnit XML (`.planning/phases/09-test-suite-triage/v1.6.0-baseline-junit-macOS-arm64.xml`):

| Subsystem | Tests | Pass | Notes |
|---|---|---|---|
| BASIC (ClassTest, Debug, RegularExpression, RTTI, …) | 6 | 6/6 ✓ | Bedrock — proves the test framework itself works |
| MATHS (Vector*, Matrix44, Quaternion, geometry, …) | 21 | 21/21 ✓ | Pure numerical code; no data deps |
| SYSTEM (Directory, Path, File, FileSystem, …) | ~10 | 9/10 (Directory_test fail) | Filesystem layer |
| DATATYPE (Bit, BitVector, Contour, hash containers, …) | ~30 | 30/30 ✓ | |
| CONCEPT (Composite, Processor, RTTI, …) | ~20 | 20/20 ✓ | The pattern bedrock |
| KERNEL (Atom, Bond, Chain, Residue, System, …) | ~25 | 25/25 ✓ | The molecular hierarchy is green |
| FORMAT (PDB, MOL2, NMRStar, SCWRLRotamer, …) | ~25 | 25/25 ✓ | All file format I/O passes WITH `BALL_DATA_PATH` set |
| STRUCTURE (FragmentDB, AddHydrogen, AssignBondOrder, …) | ~25 | 24/25 (AssignBondOrderProcessor_test2 fail) | |
| MOLMEC (AmberFF, MMFF94, CharmmFF, energy minimizers, …) | ~15 | 14/15 (AmberFF_test fail) | |
| QSAR (Connectivity, PartialCharge, Simple, Surface, …) | ~50 | 50/50 ✓ | |
| ENERGY / SOLVATION / NMR / SCORING / DOCKING / XRAY | ~50 | 50/50 ✓ | |
| BALL_VIEW_TESTS (BallAndStickModel, AddCartoonModel, LabelModel, …) | ~10 | 10/10 ✓ | VIEW layer tests run headless and pass |

The three failures are concentrated in: SYSTEM/Directory (1), STRUCTURE/AssignBondOrder (1), MOLMEC/AmberFF (1). No subsystem-wide breakage.

## Implications for Phase 9 main work

- **Triage scope dramatically narrower than expected.** No need for "fix / quarantine / known-casualty" decisions on 50 tests. Three specific tests need investigation.
- **Gatekeeper can flip to BLOCKING sooner than expected.** Once the 3 failures are fixed or quarantined with `set_tests_properties(... WILL_FAIL TRUE)`, remove `continue-on-error: true` from the CI gatekeeper step. Possibly within Phase 9 main work, days not weeks.
- **CI must export `BALL_DATA_PATH`.** This is the most important deliverable from the baseline run — the CI gatekeeper as initially written (commit b2bb718) would have shown the 50-failure false-positive rate. The follow-up commit exports `BALL_DATA_PATH=$GITHUB_WORKSPACE/data` before ctest.
- **Linux re-run is the next investigation step.** Running the suite on Linux x64 with GCC will:
  - Confirm or rule out Apple-Silicon-FP-precision as the AmberFF_test cause
  - Confirm `Directory_test` is macOS-specific
  - Validate the BALL_DATA_PATH pattern works the same on Linux
  - Establish a multi-platform baseline before Phase 9 proper.

## Artifacts in this directory

- [`v1.6.0-baseline-junit-macOS-arm64.xml`](v1.6.0-baseline-junit-macOS-arm64.xml) — JUnit XML, BALL_DATA_PATH set, **the canonical baseline** (99.0%)
- [`v1.6.0-no-datapath-junit-macOS-arm64.xml`](v1.6.0-no-datapath-junit-macOS-arm64.xml) — JUnit XML, BALL_DATA_PATH unset, kept for the "before/after" comparison record (83.0%)

## Next steps

1. ✅ Wire test gatekeeper into `ci.yml` + `release.yml` (commits `b2bb718` + follow-up with `BALL_DATA_PATH` export)
2. ✅ Capture baseline (this document)
3. Add Linux Coverage job (`-DENABLE_COVERAGE_TESTING=ON`) — get code coverage metrics alongside the gatekeeper
4. Add `EnricoMi/publish-unit-test-result-action` to render the JUnit XML as a PR check summary
5. Once CI surfaces the Linux test results, **compare macOS vs Linux failures** — AmberFF energy mismatch is the most diagnostic
6. Triage the 3 failures (`Directory_test`, `AmberFF_test`, `AssignBondOrderProcessor_test2`) as Phase 9 main work
7. Flip gatekeeper to blocking once the 3 are fixed/quarantined
