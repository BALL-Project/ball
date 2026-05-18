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
