# RELEASE-PROCESS.md

Durable conventions for the BALL release cycle. Captured 2026-05-17 during
the v1.7 tail-end so future release cycles (v1.7.x patch, v1.8, v2.x) inherit
the discipline without re-deriving it.

---

## Post-release branch hygiene (NEW — adopted from v1.7 onward)

**After tagging any release `vX.Y.0`, `master` is rebased / fast-forwarded
to the release tag's commit before the next development cycle begins.**

Rationale: prior to v1.7, work happened on long-lived `vN.M-modernization`
branches (`v1.6-modernization`, `v1.7-modernization`) while `master`
silently drifted. By v1.7 tail-end `master` was many months behind, making
emergency hotfixes off `master` (the GitHub default branch) impossible
without a complex rebase dance. Going forward:

### Procedure

After `gh release create vX.Y.0 ...` succeeds and the release artifacts
(DMG / NSIS / AppImage / DEB / RPM / .tar.gz) have been validated:

```bash
git fetch origin --tags
git checkout master
git pull --ff-only origin master           # ensure local master is clean
git merge --ff-only vX.Y.0                 # fast-forward to the release tag
git push origin master
```

If the fast-forward fails (master has commits not in the release tag —
should never happen in this workflow but possible if a hotfix landed
out-of-band on master), STOP and resolve manually rather than forcing
a merge commit. The release tag is authoritative.

After master catches up, **open the next development branch off the
release tag**, not off the previous modernization branch:

```bash
git checkout -b vX.{Y+1}-modernization vX.Y.0
git push -u origin vX.{Y+1}-modernization
```

This guarantees:
1. `master` is always the latest tagged release — emergency hotfix PRs
   target `master` cleanly without a 6-month rebase.
2. CI `paths-ignore` + concurrency-group definitions on `master` reflect
   what's actually shipping, not what was shipping a year ago.
3. The `v2.x-planning-handover` style "fresh branch off latest" pattern
   becomes trivial — branch from `master` instead of fishing for the
   right modernization-branch HEAD.

### What this changes vs. pre-v1.7

| Before (v1.6 and earlier) | After (v1.7 onward) |
|---|---|
| Work on `vN-modernization`; `master` drifts | Work on `vN-modernization`; **master fast-forwards on release** |
| Hotfixes require manual rebase off stale master | Hotfixes branch off master cleanly |
| Default branch on GitHub doesn't reflect shipped code | Default branch == latest release tag |
| Next-release branch starts from previous-modernization HEAD | Next-release branch starts from release tag |

### Sequencing within the v1.7 release closure

The v1.7 release closure (whenever 999.42-999.49 land + RC tag + final
tag) adds **one step** before the existing close-out:

1. Final phase commits + push (existing)
2. v1.7-RC1 tag → release.yml → smoke artifacts (existing)
3. **NEW (UI-heavy releases):** user-feedback gate — see
   `.planning/v1.7-USER-FEEDBACK-GATE.md`. After RC artifacts land,
   the user exercises the UI across all 4 platforms and signs off OR
   surfaces blockers. Final tag is BLOCKED until the gate closes.
   This is now a standing convention for any release whose changeset
   includes user-visible UI churn (BALLView Refresh in v1.7;
   future analogues).
4. Address RC findings → v1.7.0 tag → release.yml → ship (existing)
5. **NEW:** `master` fast-forward to v1.7.0; open v1.8-modernization off
   the v1.7.0 tag
6. Update `.github/workflows/ci.yml` `branches: [...]` lists to drop
   completed-cycle branches if they're no longer being patched
7. Archive `v1.7-modernization` branch (don't delete — keeps git-blame
   navigable for the release window's commits)

### Applies to all release flavors

- **Major** (vX.0.0) — full procedure above
- **Minor** (vX.Y.0, Y > 0) — full procedure above
- **Patch** (vX.Y.Z, Z > 0) — patch is built off the previous tag's
  branch (`vX.Y-modernization` or a temporary `vX.Y.Z-hotfix` branch);
  master fast-forwards to the new patch tag after release

---

## Related conventions (already in practice, captured here for reference)

- **Release notes:** `RELEASE-NOTES-vX.Y.Z.md` at repo root per release;
  v1.6.1, v1.6.2 already follow this pattern. v1.7.0 should produce
  `RELEASE-NOTES-v1.7.md`.
- **Appcast:** Sparkle (macOS) + WinSparkle (Windows) appcast XML
  regenerated + pushed to GitHub Pages by `release.yml` `publish-appcast`
  job. Ed25519 signing key required.
- **Tag signing:** All release tags should be GPG- or SSH-signed
  (`git tag -s vX.Y.0 -m "..."`). CI does NOT enforce this; release
  maintainer responsibility.
- **Branch naming:** `vX.Y-modernization` for dev branches; `vX.Y.Z`
  immutable tags only. Don't reuse a release tag's name for a branch.

---

## Cross-references

- `.planning/ROADMAP.md` — current milestone status; see "Revised" footer
- `.github/workflows/release.yml` — authoritative release-build commands
- `.github/workflows/ci.yml` — branch trigger list (update on each
  release per step 5 above)
- `.planning/v2.x/HANDOVER.md` — v2.x planning context (a recent
  precedent for branching off the working modernization HEAD; under the
  new convention, future analogous handovers branch off `master` once
  it tracks the latest release)
- `.planning/v1.7.x-PATCH-QUEUE.md` — items deferred during v1.7.0 RC
  cycle; source of truth for v1.7.x patch-cycle planning

---

## Release history

| Tag        | Date       | Commit       | Notes                                                                     |
|------------|------------|--------------|---------------------------------------------------------------------------|
| `v1.6.0`   | 2026-05-13 |              | Initial v1.6 ship — unsigned                                              |
| `v1.6.1`   | 2026-05-16 |              | Ninja switch + smoke harness                                              |
| `v1.6.2`   | 2026-05-17 |              | Precompiled headers groundwork, action pins, build accel                  |
| `v1.7.0`   | 2026-05-19 | `97d4c4d62d` | BALLView Refresh ship — Wave 4 (999.40-999.49) + 32 UFG fixes; unsigned   |

**v1.7.0 release notes:**
- Final ship after 6 RCs (rc1 → rc6) addressing 32 UFGs from user-feedback validation
- Wave 4 phases 999.40-999.49 all complete: design system, SVG icons, QSS theming,
  simple-dialog cleanup, Unified Inspector, Workspace consolidation, Menus + Command
  Palette, Onboarding, accessibility + BALL_UI_V2 flag removal, Classic-dock retirement
- Phase 8 (signing/notarization) explicitly deferred to v1.7.1 pending institutional
  Apple Developer Program enrollment for Universität Tübingen
- Phase 999.8 (auto-update via Sparkle/WinSparkle) deferred to v1.7.1, blocked by Phase 8
- `master` fast-forwarded to v1.7.0 via force-push (new convention per this doc § Why);
  4 prior master-only commits superseded or already cherry-picked into v1.7.0
- UFG-23 (SES surface triangle vanishing during rotation) deferred to v1.7.1 — needs
  empirical GPU-bisection session

---

## Lessons learned (v1.7.0 RC cycle — pin to memory)

These are the root-cause patterns that bit during the v1.7.0 RC
iteration cycle (rc1 → rc4) and must not be repeated in subsequent
UI-heavy releases. Each pattern is named so a future release engineer
can grep for it in a fresh RC bug report.

### LL-01 — "Hidden QOpenGLWidget" trap

**Pattern:** Qt's `QOpenGLWidget` defers `initializeGL()` until the
widget receives its first `show` event. If a startup central-widget
swap keeps a `QOpenGLWidget` hidden (e.g. v1.7's UFG-08 WelcomeScreen
swap), Qt **never** creates the GL context, so the renderer's
backing state stays zero-initialized. The first asynchronous
Representation message that hits `GLRenderer::renderSphere_` then null
derefs.

**Surface area:** any code path that creates a Representation
(WelcomeScreen sample-click, Build-Peptide, File→Open, drag-drop)
crashes with the same `GLRenderer::renderSphere_+offset` signature.

**Fix shape:** force lazy GL init synchronously during startup by
calling `QOpenGLWidget::grabFramebuffer()` on the hidden widget —
this is Qt's only documented sync-force path. Do this in the
Mainframe ctor BEFORE the central-widget swap. Idempotent guard via
the widget's existing `gl_initialized_` flag.

**Reference fix:** commit `45d6e2819b` ("force Scene initializeGL
before WelcomeScreen swap"), `Scene::forceGLContextRealization()`.

### LL-02 — "Render-smoke needs the widget visible" trap

**Pattern:** A render-smoke check that calls `Scene::exportPNG()` to
verify GL rendering will not work if the build is configured for a
WelcomeScreen-as-startup-central-widget pattern, because the
QOpenGLWidget never receives a show event (same LL-01 root cause).

**Fix shape:** an `BALLVIEW_NO_WELCOME=1` env var that
`Mainframe::showWelcomeScreen_()` early-returns on, set by the smoke
check before launching BALLView. Preserves the user-facing
WelcomeScreen flow.

**Reference fix:** commit `65859dbccc` ("env-var to skip WelcomeScreen
in smoke-check") + LL-01 fix (which makes the env var theoretically
unnecessary, but keep the env-var path for smoke-check isolation
independent of GL-init plumbing).

### LL-03 — "Opaque-paint contract gets MISSED on placeholder widgets"

**Pattern:** Custom paint widgets that opt into composition via
`Qt::WA_OpaquePaintEvent` + `setAutoFillBackground` + a `paintEvent`
override are easy to forget on empty-state / placeholder widgets,
because those widgets only render when their owning container is
otherwise empty — they don't appear in the common-case visual review.
Result: previous-frame artifacts ghost through (UFG-10, UFG-19).

**Fix shape:** every widget that participates in opaque-paint
composition (including empty-state factories like
`InspectorEmptyState::forNoRepresentation`) must apply the same
4-line opaque-paint contract. Centralize via a helper or via shared
ctors so opt-in is the default for the widget family.

**Reference fix:** commit `a2b856d461` (UFG-19,
`InspectorEmptyState`), patterned after `c14428d98b` (UFG-10) +
`1b4a8dd299` (UFG-05).

### LL-04 — "ccache `include_file_mtime` masks config.h regeneration"

**Pattern:** A PROJECT VERSION bump in `CMakeLists.txt` regenerates
`include/BALL/CONFIG/config.h` (it embeds the version), but ccache
configured with `include_file_mtime` sloppiness ignores config.h
content changes — so `VersionInfo_test` compiles against the stale
old version and fails on Linux. Symptoms: only Linux fails, only
`VersionInfo_test`, only on a version-bump commit, only when ccache
is warm.

**Fix shape:**
1. Bump the CI ccache cache key (e.g. v1 → v2) on the
   version-bump commit so all platforms get cold ccache.
2. Set `CCACHE_EXTRAFILES=include/BALL/CONFIG/config.h.in` in the CI
   env so ccache invalidates cached objects whenever the .in template
   changes.

**Reference fix:** commit `65d334792f` ("bump ccache key v1→v2 +
CCACHE_EXTRAFILES").
