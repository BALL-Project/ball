# TOOLS-AVAILABILITY.md

Quick reference for external CLI tools available on the dev machine,
captured so future orchestrator / agent runs don't re-discover them.

## Codex CLI

**Path:** `/opt/homebrew/bin/codex`
**Captured:** 2026-05-18 (post v1.7.0-rc1 UFG triage).

**Use for:** adversarial review / second-opinion on code changes,
analysis, plan reviews.

**Important note from prior session:** an earlier codex invocation hung
80+ minutes on an auth prompt. Always wrap with a timeout AND ensure
auth is already cached. Sanity-check sequence before any real call:

```bash
/opt/homebrew/bin/codex --version
/opt/homebrew/bin/codex --help 2>&1 | head -30
```

If the help text appears, codex is usable. If the help hangs, codex
is in an auth-prompt state — kill the process, run `codex login` (or
whatever the auth subcommand is) interactively in a real terminal,
then resume. Don't try to drive codex through an unauthenticated
session from an agent.

**Recommended adversarial-review invocation pattern:**

```bash
# Cap each call at 5 minutes; redirect stderr; capture exit code
gtimeout 300 /opt/homebrew/bin/codex review \
  --files <comma-separated-paths> \
  --prompt "<focused question>" 2>&1 | tee /tmp/codex-review.out
echo "exit=$?"
```

(If `gtimeout` not present on macOS, install via `brew install
coreutils`. Otherwise wrap with the agent's own timeout mechanism.)

**Don't use codex for:**
- Trivial / single-file lookups (use Read tool)
- Build-output parsing (use grep)
- Long-running iterative coding (codex CLI is review-oriented, not
  agentic in the way Claude is)

## Other tools

- `gh` — GitHub CLI, used heavily for CI/release ops
- `git` — standard
- `cmake`, `ninja`, `ccache` — build toolchain
- `brew` — package manager (Homebrew on macOS)

## See also

- `.planning/RELEASE-PROCESS.md` — release-cycle conventions
- `.planning/v1.7-USER-FEEDBACK-GATE.md` — current gate state
