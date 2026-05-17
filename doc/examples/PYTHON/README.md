# PYTHON examples — status note for v1.7

The 46 Python scripts in this tree

- `doc/examples/PYTHON/BALLView/`   (28 files — embedded-interpreter scripts)
- `doc/examples/PYTHON/UTILITIES/`  (16 files — standalone command-line utilities)
- `doc/examples/PYTHON/howtos/`     (2 files — how-to snippets)

all target the **SIP-based BALL/VIEW Python bindings**.

## v1.7 reality

The SIP Python bindings are **DISABLED in the v1.7 build**:

- `BALL_PYTHON_SUPPORT=OFF` in every `ci-*` CMake preset.
- `CLAUDE.md` lists "SIP Python bindings" under "Removed/disabled".
- The `pyWidget.C` PyBALL widget source is gated behind
  `IF(BALL_PYTHON_SUPPORT)` in `source/VIEW/WIDGETS/sources.cmake` and is
  therefore not compiled.

As a result, **none of the scripts in this tree are runnable under v1.7**.
A user who tries to run

```bash
python3 doc/examples/PYTHON/UTILITIES/PDBChecker.py
```

will hit `ModuleNotFoundError: No module named 'BALL'`. A user who tries to
load one of the `doc/examples/PYTHON/BALLView/*.py` scripts inside BALLView
via the PyBALL widget will not find the widget — it isn't compiled in.

## Why the scripts are kept anyway

These scripts are preserved as the **feature corpus** for the planned PyBALL
v2 rewrite:

- **Phase 999.15** (targeted for **v2.1**) — modernizes the BALL/VIEW Python
  binding surface using a current generator (autowrap+Cython or nanobind,
  decided by the Phase 6 bake-off — see `.planning/PYBALLV2.md`). When the
  rewrite is done, this corpus is the regression spec.
- **Phase 999.10** (targeted for **v2.2**) — replaces the in-process
  Python socket inside BALLView with a REST API + a PyBALL SDK that wraps
  the REST calls. At that point the `BALLView/*.py` scripts in this tree
  will need to be rewritten as REST-client scripts (or moved out of this
  directory into the PyBALL SDK's own samples).

Deleting them now would mean re-archaeology when 999.15 starts. Keeping
them costs near-nothing (a few dozen KB of plain text).

## When to revisit this README

- When Phase 999.15 lands → audit + rewrite or move the scripts.
- When Phase 999.10 lands → audit + rewrite the `BALLView/*.py` subset as
  REST-client scripts.
- When `BALL_PYTHON_SUPPORT` is re-enabled in the build → remove this
  README's "DISABLED" framing.

## Related artefacts

- `doc/TUTORIAL/python.tex` — same SIP-disabled banner is prepended there.
- `data/python/view_utils.py` — companion runtime helper; same status.
- `data/python/pyball_kernel/pyball_kernel.py` — companion Jupyter
  kernel; same status, plus an extra socket dependency on port 8897.
- `data/Amber/converter/{amberparser,parmConverter}.py` — Amber parameter
  conversion utilities with the same `import BALL` dependency.
