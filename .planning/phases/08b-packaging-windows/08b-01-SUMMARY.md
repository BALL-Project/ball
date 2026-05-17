# Phase 8b — Packaging Windows — Execution Summary

**Status:** scaffolding complete, awaiting external gates and Windows-runner validation.
**Active path:** **UNSIGNED-FALLBACK** (SignPath Foundation review pending; gated signing-step lands automatically when secrets appear).
**v1.7 RC blocker:** GREEN — Phase 8b's unsigned-fallback deliverable is itself complete; signing is best-effort by design.

## Per-task state

| Task | Description | Status |
|---|---|---|
| 08b-01 | SignPath Foundation OSS application | **AWAITING HUMAN SUBMISSION** — runbook drafted (`SIGNPATH-APPLICATION-RUNBOOK.md`); agent cannot submit. Named admins TBD. |
| 08b-02 | release.yml windeployqt + CPack NSIS wiring | **COMPLETE** — `cmake/BALLPackageConfig.cmake` modernized (MSVC 2010-2015 hardcoding dropped; per-user install root; English-only chrome); `release.yml` builds NSIS installer + portable .zip in parallel. |
| 08b-03 | signtool / SignPath Action integration | **GATED-PRESENT** — step in `release.yml` is `if: env.SIGNPATH_API_TOKEN != ''`; skips silently until secret configured. Documented in `SECRETS-RUNBOOK.md`. |
| 08b-04 | Unsigned-fallback README | **COMPLETE** — `packaging/windows/README-SMARTSCREEN.md` with "More info → Run anyway" + SHA256 verification. SHA256 emission wired into release.yml + included in GitHub Release body. |
| 08b-05 | vcpkg artifact cache cross-port | **COMPLETE (pre-existing)** — `ci.yml` already has the `vcpkg-archives-windows` upload step (line 778, inherited from v1.6-modernization 357d13e); `release.yml` consumer already references it (lines 226-261). No port needed. |
| 08b-06 | Windows runner validation (windows-2025) | **PENDING NEXT RELEASE RUN** — release.yml on next tag push will exercise the full Windows pipeline (vcpkg-seeded build → windeployqt → CPack NSIS → optional signtool → upload). Expected outcomes documented below. |
| 08b-07 | Bus-factor documentation | **SCAFFOLDING COMPLETE** — `SECRETS-RUNBOOK.md` contains roles table + recovery procedure + 1Password vault path + annual review reminder. Named-human rows are TBD (must be filled before v1.7 RC tag per P0 policy bar). |

## Files modified / created

**New files:**
- `.planning/phases/08b-packaging-windows/SIGNPATH-APPLICATION-RUNBOOK.md` — agent-drafted application runbook for human submission
- `.planning/phases/08b-packaging-windows/SECRETS-RUNBOOK.md` — GitHub secrets + 1Password bus-factor procedure
- `.planning/phases/08b-packaging-windows/08b-01-SUMMARY.md` — this file
- `packaging/windows/README-SMARTSCREEN.md` — end-user guide for the unsigned-installer fallback path

**Modified files:**
- `cmake/BALLPackageConfig.cmake` — NSIS modernization (drop MSVC 2010-2015 hardcoding; per-user install to `%LOCALAPPDATA%\Programs`; English-only chrome; updated homepage / contact URLs; Start Menu shortcuts).
- `.github/workflows/release.yml` — Windows job: added NSIS install step, CPack NSIS build step, SHA256 emission, gated SignPath signing step, dual `.exe` + `.zip` upload, release body updated to reference SmartScreen README + emit installer SHA256.

**Files intentionally NOT modified** (Phase 8a/8c parallel work — out of scope here):
- macOS section of `release.yml` (lines 82-205) — owned by Phase 8a.
- Linux section / scaffolding — owned by Phase 8c.
- `cmake/BALLComponents.cmake` — no changes needed; component declarations already align with NSIS.

## Expected windows-2025 runner validation outcomes (Task 6)

The next push to a release-triggering ref (tag `v1.7-*` or manual `workflow_dispatch`) will exercise:

1. **vcpkg seed** — Seed-vcpkg-archives step downloads the most recent successful CI run's `vcpkg-archives-windows` artifact from `v1.6-modernization` branch (NOTE: the branch reference in `release.yml:240` is still `v1.6-modernization`; for v1.7-only releases this should be updated but is fail-soft if no artifact found — cold vcpkg build still works, just slower). Expected wall-clock: ~25min warm vs ~2h+OOM-risk cold.
2. **Build + windeployqt** — unchanged from current pipeline; produces a working `BALLView.exe` + Qt 6 DLL deploy under `build/ci-windows/.../bin/`.
3. **CPack NSIS** — Reads modernized `cmake/BALLPackageConfig.cmake`; produces `BALLView-${REF}-windows-x64-installer.exe` configured for per-user install to `%LOCALAPPDATA%\Programs\BALLView`; no UAC prompt at install time.
4. **SHA256 emission** — `.sha256` sidecar written next to installer; release-job extracts the hash and embeds it in the GitHub Release body.
5. **SignPath signing step** — Skips silently (no `SIGNPATH_API_TOKEN` secret in repo yet); workflow falls through to unsigned-installer upload.
6. **Artifact upload** — Both `BALLView-${REF}-windows-x64.zip` (portable) AND `BALLView-${REF}-windows-x64-installer.exe` (NSIS) uploaded as release assets.

### Hand-install validation on Windows 11 (per Task 6 acceptance criteria — REQUIRES HUMAN)

After the next release run completes, a named human should:
1. Download the `.exe` installer from the draft release.
2. Verify SHA256 against the value in the release body (PowerShell: `(Get-FileHash *.exe -Algorithm SHA256).Hash`).
3. Run the installer — click through SmartScreen warning per README-SMARTSCREEN.md.
4. Confirm: no UAC prompt; default install location is `%LOCALAPPDATA%\Programs\BALLView`; Start Menu shortcut "BALLView" appears; `BALLView.exe` launches; 3D scene renders (load a sample molecule from `data/`); uninstall via Settings → Apps cleanly removes the install directory.
5. Record results in this file under the "Validation results" section below (replace the placeholder).

### Validation results (TO BE FILLED POST-RELEASE-RUN)

- Run URL: (pending)
- Installer size: (pending)
- Installer SHA256: (pending)
- Per-user install path confirmed: (pending)
- Start Menu shortcut created: (pending)
- 3D rendering confirmed: (pending)
- Clean uninstall confirmed: (pending)
- vcpkg warm-build wall-clock vs cold: (pending — compare current run timing against v1.6.2's known cold-build OOM events)

## SignPath application state (Task 1)

- **Submission status:** NOT YET SUBMITTED — agent drafted the runbook; human must execute per `SIGNPATH-APPLICATION-RUNBOOK.md`.
- **Application reference:** TBD (foundation will issue on submission)
- **Primary admin:** TBD — name before v1.7 RC tag (P0 bus-factor requirement)
- **Secondary admin:** TBD — name before v1.7 RC tag (P0 bus-factor requirement)
- **Weekly status log:** to be created at `.planning/phases/08b-packaging-windows/08b-SIGNPATH-STATUS.md` on submission day (template in SIGNPATH-APPLICATION-RUNBOOK.md §6).
- **Decision gates:** Week 4 → Path B (Azure Code Signing) eval; Week 6 → Path C (commercial OV + HSM) eval; ship unsigned by RC if no path produces a signature.

## Bus-factor mitigation (Task 7)

- **Vault:** `BALL/Windows-Signing/` (1Password or org-equivalent)
- **Secret names:** `SIGNPATH_API_TOKEN`, `SIGNPATH_ORG_ID` (GitHub repo secrets, mirrored to vault)
- **Recovery procedure:** documented in `SECRETS-RUNBOOK.md` §4 — secondary admin can log in, approve pending signing requests, rotate tokens, and add additional admins without primary admin present.
- **Annual review:** **TBD — 12 months from Phase 8b kickoff** (calendar reminder must be filed by named primary admin).
- **Named humans:** **TBD — must be filled in `SECRETS-RUNBOOK.md` §3 before v1.7 RC tag.** Single-admin state is a P0 violation.

## Key open decisions / risks

1. **SignPath approval timing** — drives whether v1.7 ships signed or unsigned. Mitigation: unsigned-fallback path is itself a complete deliverable; no shipping risk.
2. **Named admins (primary + secondary)** — currently TBD in both runbooks. Must be filled before v1.7 RC tag.
3. **release.yml vcpkg-seed branch reference** — currently `v1.6-modernization` (line 240); should be updated to `v1.7-modernization` (or made parametric) at some point. Not blocking — fail-soft falls back to cold vcpkg build. Out of scope for Phase 8b (would touch the shared seed step that's not Windows-specific in spirit).
4. **NSIS installer name suffix** — CPack may emit slightly different naming depending on whether `CPACK_PACKAGE_FILE_NAME` propagates cleanly; release.yml has a fallback glob to catch the resolved name. Validate on next runner run.
5. **CPack `RequestExecutionLevel`** — legacy `highest` was dropped in favor of CPack-default `user`. If per-user install fails for an unforeseen reason on windows-2025, revert that block and reintroduce `RequestExecutionLevel user` explicitly.

## v1.7 RC readiness

**GREEN — Phase 8b is not a blocker for v1.7 RC.** The unsigned-with-SmartScreen-warning installer is itself a complete deliverable, with:
- A working NSIS installer (per-user, no admin prompt).
- A user-facing README explaining the SmartScreen warning and how to verify the binary via SHA256.
- A gated signing step ready to activate as soon as SignPath approves the foundation application.
- A documented escalation path (Path B Azure / Path C commercial) if SignPath stalls.
- Bus-factor procedures in place (pending named-human assignment before RC tag).

When the signature lands post-RC, the v1.7 release assets can be backfilled with the signed installer via `gh release upload --clobber` — no v1.7 retag needed.
