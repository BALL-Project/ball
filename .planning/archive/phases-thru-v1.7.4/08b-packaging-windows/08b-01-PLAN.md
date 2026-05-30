---
phase: 08b-packaging-windows
plan: 01
type: execute
wave: 1
parent_phase: 08
sibling_phases: [08a-packaging-macos, 08c-packaging-linux]
depends_on:
  - ci-harden-w1  # ci.yml serialized resource — CI-HARDEN-W1 lands first
  - 357d13e        # v1.6.2 vcpkg artifact cache seed commit (cross-port source)
files_modified:
  - .github/workflows/release.yml          # windeployqt → CPack NSIS → (signtool | unsigned-fallback)
  - cmake/BALLPackageConfig.cmake          # NSIS modernization (drop MSVC 2010-2015 hardcoding)
  - cmake/BALLComponents.cmake             # CPack component cleanup if needed
  - packaging/windows/README-SMARTSCREEN.md  # NEW — fallback warning instructions for unsigned installer
  - .planning/phases/08b-packaging-windows/08b-SIGNPATH-STATUS.md  # weekly status log
  - .planning/phases/08b-packaging-windows/08b-BUSFACTOR.md  # 1Password + 2-human recovery doc
autonomous: false   # external-gated by SignPath Foundation approval; weekly human check-in needed
requirements:
  - PACK-WIN-INSTALLER  # functional Windows installer (signed if cert lands, unsigned-with-warning otherwise)
  - PACK-WIN-SIGN       # signed installer via SignPath OSS Foundation (best-effort, externally gated)
  - PACK-WIN-BUSFACTOR  # cert + signtool credentials stored with 2-human recovery
must_haves:
  truths:
    - "v1.7 RC tag ships a Windows installer (signed OR unsigned-with-SmartScreen-warning-README) — 8b is NOT a blocker for the RC"
    - "SignPath Foundation OSS application submitted within 24h of phase start (lead time is weeks, parallelise with all other 8b work)"
    - "CPack NSIS installer (.exe) builds green on windows-2025 runner — replaces the existing release.yml Compress-Archive .zip path"
    - "signtool integration is gated behind cert availability — workflow falls back cleanly to unsigned if SignPath hasn't approved by RC"
    - "Fallback Path B (Azure Code Signing $120/yr) decision-gate documented at week 4 of SignPath silence; Path C (commercial $200-500/yr) at week 6"
    - "v1.6.2's vcpkg artifact cache seed (commit 357d13e on v1.6-modernization) cross-ported to v1.7-modernization release.yml without regression"
    - "SmartScreen reputation-building strategy documented (the unsigned + warning fallback README, and the 'we accumulate downloads over time' acceptance)"
    - "Bus factor: SignPath admin + signtool credentials documented in 1Password (or org-equivalent) with 2 humans knowing the recovery path"
  artifacts:
    - path: ".github/workflows/release.yml"
      provides: "Windows job: vcpkg-seeded → cmake build → windeployqt → CPack NSIS → optional signtool (gated) → upload"
      contains: "cpack -G NSIS"
    - path: "cmake/BALLPackageConfig.cmake"
      provides: "NSIS variables modernized — drop MSVC 2010/2012/2013/2015 hardcoding; per-user install (no MODIFY_PATH); English-only chrome"
      contains: "CPACK_NSIS_DISPLAY_NAME"
    - path: "packaging/windows/README-SMARTSCREEN.md"
      provides: "End-user guidance for the unsigned-installer fallback path (More info → Run anyway)"
      contains: "SmartScreen"
    - path: ".planning/phases/08b-packaging-windows/08b-SIGNPATH-STATUS.md"
      provides: "Weekly status log of SignPath application: submitted-date, foundation-review state, escalation triggers"
      contains: "SignPath"
    - path: ".planning/phases/08b-packaging-windows/08b-BUSFACTOR.md"
      provides: "Cert + signtool credential storage location + 2-human recovery procedure"
      contains: "1Password"
  key_links:
    - from: ".planning/phases/08-packaging-and-distribution/08-SIGNING-RESEARCH.md (Path A SignPath chosen)"
      to: ".planning/phases/08b-packaging-windows/08b-SIGNPATH-STATUS.md (live application tracking)"
      via: "research → live tracking; status doc updated weekly until approved/rejected"
      pattern: "SignPath"
    - from: ".planning/phases/08-packaging-and-distribution/08-INSTALLER-FORMATS-RESEARCH.md (NSIS chosen)"
      to: "cmake/BALLPackageConfig.cmake (NSIS variable modernization)"
      via: "research → CMake variable updates; drop MSVC 2010-2015 hardcoding; align with Decisions 3+4 (per-user, English-only)"
      pattern: "CPACK_NSIS_"
    - from: "v1.6-modernization commit 357d13e (vcpkg artifact upload + release-side download)"
      to: "v1.7-modernization .github/workflows/release.yml (cross-port)"
      via: "cherry-pick or hand-port the artifact-seed step shape; verifies on first 8b release dry-run"
      pattern: "vcpkg-archives-windows"
---

<objective>
Deliver a functional Windows installer for BALLView v1.7 — signed via SignPath
Foundation OSS if the certificate clears in time, otherwise unsigned with a
documented SmartScreen-warning fallback README. **Phase 8b is async-gated** by
the SignPath Foundation approval timeline (weeks of lead time); v1.7 RC tag
ships when 8a + 8c complete, regardless of 8b's signing status. The unsigned
fallback is the explicit Plan B and is itself a deliverable, not a degraded
outcome.

This phase also cross-ports v1.6.2's vcpkg artifact cache seed
([commit 357d13e](https://github.com/BALL-Project/ball/commit/357d13e) on
`v1.6-modernization`) to `v1.7-modernization`'s release.yml so the Windows
release pipeline doesn't OOM during cold vcpkg Qt6+Boost builds — a recurring
v1.6.2 release-attempt failure mode.

Scope summary:
- SignPath OSS Foundation application (external, async, day-1 start)
- CPack NSIS installer build wired into release.yml (works signed OR unsigned)
- signtool integration gated on cert availability
- Unsigned-fallback README for the SmartScreen-warning path
- vcpkg artifact cache seed cross-port from v1.6.2
- Validation on windows-2025 runner
- Bus-factor documentation (1Password storage + 2-human recovery)

Output: 1 working CPack NSIS workflow + 1 SignPath application in flight +
1 fallback README + 1 bus-factor doc + 1 weekly-status doc. The
v1.7 RC blocker is "installer builds + uploads"; signing is best-effort.
</objective>

<execution_context>
@$HOME/.claude/get-shit-done/workflows/execute-plan.md
@$HOME/.claude/get-shit-done/templates/summary.md
</execution_context>

<context>
@.planning/PROJECT.md
@.planning/ROADMAP.md
@.planning/v1.7-PLAN.md
@.planning/v1.7-CODEX-REVIEW.md
@.planning/phases/08-packaging-and-distribution/08-SIGNING-RESEARCH.md
@.planning/phases/08-packaging-and-distribution/08-INSTALLER-FORMATS-RESEARCH.md
@.github/workflows/release.yml
@cmake/BALLPackageConfig.cmake
@cmake/BALLComponents.cmake

<interfaces>
**SignPath Foundation OSS application — the external gate.**

- Home: <https://signpath.org/> (foundation portal) — application form at <https://about.signpath.io/foundation>
- Eligibility: OSI-approved license without commercial dual-licensing. BALL is **LGPL v2.1** → qualifies (confirmed in [08-SIGNING-RESEARCH.md](../08-packaging-and-distribution/08-SIGNING-RESEARCH.md#path-a--signpath-foundation-free-for-oss--chosen-2026-05-15)).
- Sponsor: not required for the Foundation tier. The foundation team reviews directly. (Sponsorship is a feature of the commercial SignPath product, not the Foundation OSS path.)
- Application inputs needed: project name (`BALL` / `BALLView`), homepage (`https://ball-project.org`), GitHub URL (`https://github.com/BALL-Project/ball`), license URL, maintainer contact (project email), short description, public-CI evidence (link to `.github/workflows/release.yml` showing the build is reproducible from source).
- Review SLA per the foundation: **2-6 weeks typical**, occasionally longer for projects with non-trivial governance questions. BALL's academic / public-CI / LGPL posture is the easy-yes shape; expect closer to 2-3 weeks.
- On approval: project gets a SignPath organization slug + a manual-approval signing policy. Each release CI run uploads the unsigned artifact to SignPath via the [`signpath/github-action-submit-signing-request`](https://github.com/SignPath/github-action-submit-signing-request) action; a SignPath admin clicks "approve" in the foundation UI; the action downloads the signed artifact back into the workflow.
- Bus-factor: the SignPath admin role MUST have ≥2 humans with login (foundation rules + our own bus-factor policy per [v1.7-PLAN.md risk register](../../v1.7-PLAN.md#risk-register-post-review-revision)).

**Fallback paths (escalation if SignPath stalls):**

- **Week 0:** submit SignPath application; start all other 8b work in parallel.
- **Week 2:** check status. Typical-case SignPath approval lands here; if so, switch to signed-path immediately.
- **Week 4:** if SignPath silent or in review, **decision-gate Path B**: open Azure Code Signing account ($120/yr). Pre-research the Azure setup (OIDC trust, RBAC, `Azure/artifact-signing-action`); do NOT pay/provision yet — only on the week 4 decision-gate.
- **Week 6:** if SignPath rejected OR Path B blocked by org Azure-subscription constraints, **decision-gate Path C**: purchase commercial OV + cloud HSM cert (~$200-500/yr; SSL.com eSigner or DigiCert KeyLocker). This is the last-resort path.
- **Any time before v1.7 RC tag:** if NO path has produced a working signature, ship the **unsigned-with-SmartScreen-warning** installer (the Plan B deliverable from this phase). v1.7 ships either way.

**release.yml shape post-phase (Windows job, simplified):**

```yaml
- name: Seed vcpkg archives from CI artifact (cross-port from v1.6.2)  # task 5
  # ... [357d13e cross-port: download vcpkg-archives-windows artifact, stage under %LOCALAPPDATA%\vcpkg\archives]

- name: Configure / Build (existing — vcpkg + ninja + msvc-dev-cmd)
  # unchanged from current release.yml

- name: windeployqt (existing — Qt6 deploy)
  # unchanged

- name: Bundle data/ (existing)
  # unchanged

- name: Build NSIS installer (task 2)
  shell: pwsh
  run: |
    cd build/ci-windows
    cpack -G NSIS -C Release
    Get-ChildItem -Filter "BALLView-*.exe" | Select-Object Name,Length

- name: Sign installer via SignPath (task 3 — gated on secret presence)
  if: env.SIGNPATH_API_TOKEN != ''
  uses: signpath/github-action-submit-signing-request@v1
  with:
    api-token: ${{ secrets.SIGNPATH_API_TOKEN }}
    organization-id: ${{ secrets.SIGNPATH_ORG_ID }}
    project-slug: 'ballview'
    signing-policy-slug: 'release-signing'
    artifact-configuration-slug: 'installer'
    github-artifact-id: ${{ steps.upload-installer.outputs.artifact-id }}
    wait-for-completion: true
    output-artifact-directory: 'signed-installer'

- name: Upload installer (task 4 — works signed OR unsigned)
  uses: actions/upload-artifact@v6
  with:
    name: BALLView-${{ steps.resolve.outputs.ref }}-windows-x64-installer
    path: |
      signed-installer/*.exe
      build/ci-windows/BALLView-*.exe
    if-no-files-found: error
```

The `if: env.SIGNPATH_API_TOKEN != ''` predicate is the gating mechanism — until
SignPath secrets are configured in repo settings, the signing step skips
cleanly and the unsigned installer ships. No workflow refactor needed when
the cert lands; just add the secrets.

**CPack NSIS — modernization scope in `cmake/BALLPackageConfig.cmake`:**

- DROP: hardcoded MSVC 2010/2012/2013/2015 `CPACK_PACKAGE_FILE_NAME` branches (historical, vcpkg + MSVC 2022 only).
- ADD: `CPACK_GENERATOR "NSIS"` on `WIN32`, English-only chrome (Decision #4 in [08-INSTALLER-FORMATS-RESEARCH.md](../08-packaging-and-distribution/08-INSTALLER-FORMATS-RESEARCH.md#decisions-locked-2026-05-15)), per-user install root `$LOCALAPPDATA\Programs` (Decision #3), do NOT set `CPACK_NSIS_MODIFY_PATH` (avoids UAC admin prompt).
- ADD: `CPACK_NSIS_DISPLAY_NAME`, `CPACK_NSIS_PACKAGE_NAME`, `CPACK_NSIS_HELP_LINK`, `CPACK_NSIS_URL_INFO_ABOUT`, `CPACK_NSIS_CONTACT`, `CPACK_NSIS_MENU_LINKS`, `CPACK_NSIS_INSTALLED_ICON_NAME` per the research file's CMake sketch.
- VERIFY: existing `Welcome.rtf` + `COPYRIGHT` referenced from `cmake/BALLPackageConfig.cmake:11-13` still exist and have current v1.7 content (update if stale).

**Bus-factor doc — `08b-BUSFACTOR.md` minimum contents:**

1. SignPath admin accounts: who-1, who-2 (≥2 humans with login).
2. 1Password vault path: `BALL/Windows-Signing/` (or org-equivalent).
3. Stored items: SignPath API token (`SIGNPATH_API_TOKEN`), org-id (`SIGNPATH_ORG_ID`), project slug, signing policy slug, foundation account recovery email.
4. Recovery procedure: if primary admin is unavailable, secondary admin can (a) trigger a release.yml run, (b) approve via SignPath foundation UI, (c) rotate the API token if compromised.
5. Annual review reminder filed: 2027-05-17 (12mo from phase start) — refresh credentials + verify both admins still active.

**SmartScreen reputation strategy (unsigned-fallback path).**

If we ship v1.7 unsigned, SmartScreen will warn on every user's first download
until enough downloads accumulate to build reputation (typically thousands of
downloads). For an academic OSS project this can take months. The pragmatic
position is:

- Acknowledge the warning in release notes + `README-SMARTSCREEN.md` (clear "click More info → Run anyway" instructions).
- Treat reputation-building as a slow-burn process; don't promise users a fast fix.
- When SignPath approves (post-v1.7 if needed), backfill the signed installer to v1.7 release assets via `gh release upload --clobber`; the released-tag artifact gets swapped, the next download is signed, and SmartScreen reputation starts accumulating against the signed binary.

**windows-2025 runner validation:**

The full pipeline (vcpkg-seeded build → NSIS installer → optional signtool →
upload) MUST run green on `windows-2025` (the existing release.yml runner). A
dry-run can be triggered via `gh workflow run release.yml -f tag=v1.7-test-pkg`
on a throwaway tag; the resulting installer .exe is the validation artifact.
Hand-install on a Windows 11 VM and confirm: (a) installer launches, (b) chooses
`%LOCALAPPDATA%\Programs\BALLView` by default (per-user), (c) Start Menu shortcut
created, (d) BALLView.exe launches and renders the 3D scene, (e) uninstall via
Add/Remove Programs cleanly removes.

**ci.yml is a serialized resource (v1.7-PLAN.md sequencing rule).**

CI-HARDEN-W1 lands first, then 8a/8b/8c serially (any order among themselves —
they touch the macOS / windows / linux job sections respectively, low conflict
risk on `release.yml` since each job section is distinct). 8b edits ONLY the
Windows job section of release.yml; should not collide with 8a or 8c.

**Out of scope for 08b:**

- WiX MSI generation (deferred to a hypothetical 8.1 / v2 enterprise upgrade per [08-INSTALLER-FORMATS-RESEARCH.md Decision #2](../08-packaging-and-distribution/08-INSTALLER-FORMATS-RESEARCH.md#decisions-locked-2026-05-15)).
- WinSparkle auto-update integration (Phase 999.8 Wave 2; needs the signed installer to exist first).
- Linux installer formats (Phase 8c).
- macOS notarization / DMG (Phase 8a).
- Microsoft Store / MSIX distribution (out of v1.x scope).
- Windows code-signing for individual DLLs beyond the installer .exe + BALLView.exe (out of scope; signtool walks the deploy dir but we sign the installer wrapper as the user-facing artifact).
</interfaces>
</context>

<tasks>

<task type="manual">
  <name>Task 1: Submit SignPath Foundation OSS application (Day 1; weeks of lead time)</name>
  <files>
    .planning/phases/08b-packaging-windows/08b-SIGNPATH-STATUS.md  (NEW)
  </files>
  <read_first>
    - .planning/phases/08-packaging-and-distribution/08-SIGNING-RESEARCH.md (Path A section)
    - signpath.org Foundation application form (web-fetch if needed)
  </read_first>
  <action>
    1. Identify the application owner — the human who will be the primary SignPath admin (recorded in 08b-BUSFACTOR.md task 7). Default: project maintainer with GitHub admin on `BALL-Project/ball`.
    2. Identify the secondary admin — a SECOND human who can recover access if primary is unavailable. **REQUIRED** before submitting (the bus-factor mitigation is the policy bar, not the foundation's).
    3. Open the SignPath Foundation application: <https://about.signpath.io/foundation> → "Apply for free code signing".
    4. Fill the application:
       - Project name: `BALL / BALLView`
       - Homepage: `https://ball-project.org`
       - Repository: `https://github.com/BALL-Project/ball`
       - License: LGPL-2.1 — paste the LICENSE file URL
       - Description: 2-3 sentences from `.planning/PROJECT.md` (molecular modelling library + GUI; LGPL OSS; academic origin)
       - CI evidence: link to `.github/workflows/release.yml` at the tip of `v1.7-modernization`
       - Contact: primary admin email
       - Notes: mention public-CI build pipeline + reproducible build via `cmake --preset ci-windows` + vcpkg manifest mode
    5. Submit. Capture the application reference number / acknowledgment email.
    6. Create `.planning/phases/08b-packaging-windows/08b-SIGNPATH-STATUS.md` with initial entry:
       ```markdown
       # SignPath Foundation OSS — Application Status Log

       **Submitted:** 2026-05-17 (planned)
       **Application reference:** <fill in from foundation reply>
       **Primary admin:** <name + email>
       **Secondary admin:** <name + email>

       ## Weekly status

       | Week | Date | State | Action |
       |---|---|---|---|
       | 0 | 2026-05-17 | submitted | application form filed; awaiting foundation review |
       | 1 | 2026-05-24 | pending | (status check) |
       | 2 | 2026-05-31 | (pending / approved / questions) | (typical-case approval window) |
       | 4 | 2026-06-14 | DECISION-GATE | if not approved: start Path B (Azure Code Signing) provisioning |
       | 6 | 2026-06-28 | DECISION-GATE | if still no signature: start Path C (commercial OV + HSM) or commit to unsigned-fallback |
       ```
    7. Set a recurring reminder (calendar / project tracker) for weekly status check.
  </action>
  <verify>
    <automated>test -f .planning/phases/08b-packaging-windows/08b-SIGNPATH-STATUS.md &amp;&amp; grep -q "Submitted:" .planning/phases/08b-packaging-windows/08b-SIGNPATH-STATUS.md</automated>
  </verify>
  <acceptance_criteria>
    - SignPath Foundation application submitted within 24h of phase kickoff.
    - 08b-SIGNPATH-STATUS.md exists with submission date + application reference + both admins identified.
    - Weekly status reminder set in project tracker.
  </acceptance_criteria>
  <done>
    Foundation application in flight; weekly tracking machinery in place.
  </done>
</task>

<task type="auto">
  <name>Task 2: release.yml — wire CPack NSIS installer build (works signed OR unsigned)</name>
  <files>
    .github/workflows/release.yml
    cmake/BALLPackageConfig.cmake
    cmake/BALLComponents.cmake
  </files>
  <read_first>
    - .github/workflows/release.yml lines 207-470 (current Windows job: windeployqt + Compress-Archive zip)
    - cmake/BALLPackageConfig.cmake (existing CPack scaffolding to modernize)
    - cmake/BALLComponents.cmake (component declarations)
    - .planning/phases/08-packaging-and-distribution/08-INSTALLER-FORMATS-RESEARCH.md (CMake / CPack integration sketch + Decisions 1-5)
  </read_first>
  <action>
    1. Modernize `cmake/BALLPackageConfig.cmake`:
       - DELETE the MSVC 2010 / 2012 / 2013 / 2015 hardcoded `CPACK_PACKAGE_FILE_NAME` branches.
       - REPLACE with a single platform-aware `CPACK_PACKAGE_FILE_NAME "BALLView-${PROJECT_VERSION}-${BALL_PLATFORM_TAG}"` (define `BALL_PLATFORM_TAG` as `win64` / `macos-arm64` / `linux-x64` as appropriate).
       - ADD the Windows NSIS block per [08-INSTALLER-FORMATS-RESEARCH.md CMake sketch](../08-packaging-and-distribution/08-INSTALLER-FORMATS-RESEARCH.md#cmake--cpack-integration-sketch):
         ```cmake
         IF(WIN32)
           SET(CPACK_GENERATOR "NSIS")
           SET(CPACK_NSIS_DISPLAY_NAME "BALLView ${PROJECT_VERSION}")
           SET(CPACK_NSIS_PACKAGE_NAME "BALLView ${PROJECT_VERSION}")
           SET(CPACK_NSIS_HELP_LINK "https://ball-project.org")
           SET(CPACK_NSIS_URL_INFO_ABOUT "https://github.com/BALL-Project/ball")
           SET(CPACK_NSIS_CONTACT "https://github.com/BALL-Project/ball/issues")
           SET(CPACK_NSIS_INSTALL_ROOT "$LOCALAPPDATA\\Programs")  # Decision #3 — per-user, no admin
           SET(CPACK_NSIS_MENU_LINKS
             "bin/BALLView.exe" "BALLView"
             "https://ball-project.org" "BALL Project Homepage")
           SET(CPACK_NSIS_INSTALLED_ICON_NAME "bin/BALLView.exe")
           # NOTE: CPACK_NSIS_MODIFY_PATH intentionally NOT set (avoids UAC admin prompt; Decision #3).
           # NOTE: chrome stays English-only (Decision #4).
         ENDIF()
         ```
       - VERIFY `Welcome.rtf` + `COPYRIGHT` files referenced from this CMake module still exist and have current content; update copyright year + version reference if stale.
    2. Edit `.github/workflows/release.yml` Windows job section (after the `Bundle data/ next to .exe (Windows)` step around L452-463):
       - INSERT a new step `Install NSIS (Windows)` that runs `choco install nsis -y` (~30sec install).
       - INSERT a new step `Build NSIS installer (Windows)` that runs `cd build/ci-windows; cpack -G NSIS -C Release` and lists the resulting `BALLView-*.exe`.
       - REPLACE the existing `Zip deploy dir (Windows)` step (L465-470) with a step that uploads BOTH the installer .exe AND keeps the .zip as a secondary asset (some users prefer the portable .zip path; cheap to keep both during the v1.7 transition).
       - The new asset name shape: `BALLView-${REF}-windows-x64-installer.exe` (installer) + `BALLView-${REF}-windows-x64.zip` (portable).
    3. Validate the YAML before commit: `actionlint .github/workflows/release.yml` (or PyYAML fallback).
    4. Commit:
       ```
       packaging(win): wire CPack NSIS installer build (Phase 8b, PACK-WIN-INSTALLER)

       Modernize BALLPackageConfig.cmake (drop MSVC 2010-2015 hardcoding;
       per-user install root; English-only chrome per Decisions 3+4).
       release.yml Windows job now builds NSIS .exe installer alongside the
       existing portable .zip. Installer build is unsigned at this point;
       signtool integration follows in a separate commit (Task 3) gated on
       SignPath secret availability.
       ```
  </action>
  <verify>
    <automated>cd /Users/kohlbach/Claude/BALL/ball &amp;&amp; grep -c "CPACK_NSIS_DISPLAY_NAME" cmake/BALLPackageConfig.cmake &amp;&amp; grep -c "cpack -G NSIS" .github/workflows/release.yml &amp;&amp; ! grep -E "MSVC1[0-5]" cmake/BALLPackageConfig.cmake &amp;&amp; (command -v actionlint &amp;&amp; actionlint .github/workflows/release.yml || python3 -c "import yaml; yaml.safe_load(open('.github/workflows/release.yml'))")</automated>
  </verify>
  <acceptance_criteria>
    - cmake/BALLPackageConfig.cmake has NSIS variable block; MSVC 2010-2015 hardcoding removed.
    - release.yml Windows job has `choco install nsis` + `cpack -G NSIS` steps.
    - Resulting workflow artifact list includes BOTH `BALLView-${REF}-windows-x64-installer.exe` AND `BALLView-${REF}-windows-x64.zip` (portable kept during transition).
    - actionlint passes.
    - Dry-run on a throwaway `v1.7-test-pkg` tag produces a valid `.exe` installer artifact in the release job's downloaded artifacts.
  </acceptance_criteria>
  <done>
    NSIS installer build green on windows-2025; installer .exe uploaded as release asset; unsigned at this stage.
  </done>
</task>

<task type="auto">
  <name>Task 3: signtool integration via SignPath GitHub Action (GATED on cert availability)</name>
  <files>
    .github/workflows/release.yml
  </files>
  <read_first>
    - .planning/phases/08-packaging-and-distribution/08-SIGNING-RESEARCH.md (Path A SignPath section + GitHub Actions integration notes)
    - https://github.com/SignPath/github-action-submit-signing-request (action README — input schema)
    - .planning/phases/08b-packaging-windows/08b-SIGNPATH-STATUS.md (cert availability check)
  </read_first>
  <action>
    1. **Pre-condition gate:** this task ONLY lands its workflow change when 08b-SIGNPATH-STATUS.md shows SignPath approved AND repo secrets `SIGNPATH_API_TOKEN` + `SIGNPATH_ORG_ID` are configured. Until then, this task stays open and the workflow runs unsigned (Task 4 path).
    2. Add the signing step to `.github/workflows/release.yml` Windows job, INSERTED between the `Build NSIS installer` step (Task 2) and the `Upload workflow artifact` step:
       ```yaml
       - name: Sign installer via SignPath Foundation (Windows)
         if: matrix.os == 'windows-x64' && env.SIGNPATH_API_TOKEN != ''
         env:
           SIGNPATH_API_TOKEN: ${{ secrets.SIGNPATH_API_TOKEN }}
         uses: signpath/github-action-submit-signing-request@v1
         with:
           api-token: ${{ secrets.SIGNPATH_API_TOKEN }}
           organization-id: ${{ secrets.SIGNPATH_ORG_ID }}
           project-slug: 'ballview'
           signing-policy-slug: 'release-signing'
           artifact-configuration-slug: 'installer'
           github-artifact-id: ${{ steps.upload-unsigned.outputs.artifact-id }}
           wait-for-completion: true
           output-artifact-directory: 'signed-installer'
       ```
    3. Adjust the upload step to point at `signed-installer/*.exe` when SignPath ran, otherwise the original `build/ci-windows/BALLView-*.exe`:
       ```yaml
       - name: Upload installer (Windows — signed if SignPath ran, unsigned otherwise)
         if: matrix.os == 'windows-x64'
         uses: actions/upload-artifact@v6
         with:
           name: BALLView-${{ steps.resolve.outputs.ref }}-windows-x64-installer
           path: |
             signed-installer/*.exe
             build/ci-windows/BALLView-*.exe
           if-no-files-found: error
       ```
       The dual-path is intentional: if SignPath ran, `signed-installer/*.exe` exists (signed); if not, it's empty and the `build/ci-windows/*.exe` glob picks up the unsigned installer. `if-no-files-found: error` guarantees AT LEAST ONE installer exists.
    4. Document the signtool ceremony in `08b-BUSFACTOR.md` (the manual-approval-in-SignPath-UI step) — the action will block on the foundation UI until a SignPath admin clicks "approve". This is intentional supply-chain-attack control.
    5. Verify on a dry-run release: trigger `gh workflow run release.yml -f tag=v1.7-test-sign-1`; observe the signing step pauses; approve in SignPath UI; observe the signed `.exe` lands in workflow artifacts. Run `signtool verify /pa /v signed-installer/BALLView-v1.7-test-sign-1-windows-x64-installer.exe` on a Windows VM to confirm chain-of-trust.
    6. Commit (only when cert is live):
       ```
       packaging(win): integrate SignPath Foundation signing (Phase 8b, PACK-WIN-SIGN)

       Gated on SIGNPATH_API_TOKEN secret presence — workflow runs unsigned
       cleanly when the secret is absent, switches to signed-via-SignPath
       when the secret is configured. Manual-approval gate per release
       (intentional anti-supply-chain-attack control).
       ```
  </action>
  <verify>
    <automated>cd /Users/kohlbach/Claude/BALL/ball &amp;&amp; (grep -q "signpath/github-action-submit-signing-request" .github/workflows/release.yml || echo "GATED — task stays open until SignPath approval lands") &amp;&amp; test -f .planning/phases/08b-packaging-windows/08b-SIGNPATH-STATUS.md</automated>
  </verify>
  <acceptance_criteria>
    - WHEN GATE OPENS (SignPath approved + secrets configured): release.yml has the `signpath/github-action-submit-signing-request@v1` step gated on `env.SIGNPATH_API_TOKEN != ''`.
    - WHEN GATE OPENS: dry-run release produces a signed `.exe` verified by `signtool verify /pa /v`.
    - WHEN GATE CLOSED: this task remains open; Task 4 (unsigned-fallback) is the active path.
  </acceptance_criteria>
  <done>
    Signed-installer path operational, OR task documented as still-gated with weekly status updates in 08b-SIGNPATH-STATUS.md.
  </done>
</task>

<task type="auto">
  <name>Task 4: Unsigned-installer fallback + SmartScreen-warning README</name>
  <files>
    packaging/windows/README-SMARTSCREEN.md  (NEW)
    .github/workflows/release.yml  (release notes body update)
  </files>
  <read_first>
    - .planning/phases/08-packaging-and-distribution/08-SIGNING-RESEARCH.md (SmartScreen reputation watch-out)
    - .github/workflows/release.yml lines 522-533 (existing release-body BODY heredoc)
  </read_first>
  <action>
    1. Create `packaging/windows/README-SMARTSCREEN.md`:
       ```markdown
       # Windows SmartScreen Warning — How to install BALLView v1.7 (unsigned)

       This release of BALLView is **not yet code-signed for Windows**. The
       SignPath Foundation application is in-flight; once approved, future
       releases (and this one, retroactively) will be signed.

       When you run `BALLView-<version>-windows-x64-installer.exe`,
       Windows SmartScreen will show:

       > Windows protected your PC
       > Microsoft Defender SmartScreen prevented an unrecognized app from starting.

       **To install BALLView:**
       1. Click **More info** in the warning dialog.
       2. Click **Run anyway**.
       3. The installer launches; follow the prompts.

       **Why this happens:** unsigned binaries (and even newly-signed binaries
       without download history) trigger SmartScreen until enough users have
       downloaded the binary to build "reputation". This is normal for
       small-volume OSS projects.

       **Verifying the installer is the genuine BALLView release:**
       - Check the GitHub Release page: <https://github.com/BALL-Project/ball/releases>
       - Compare the SHA256 of the installer to the value listed in the release notes.

       Once the SignPath Foundation signature lands (tracked in
       `.planning/phases/08b-packaging-windows/08b-SIGNPATH-STATUS.md`), this
       README will be updated and the warning will go away as SmartScreen
       reputation accumulates against the signed binary.
       ```
    2. Compute SHA256 of the installer in the workflow + emit it into the GitHub Release body. Add to the release job (around L516-533):
       ```bash
       INSTALLER_SHA256=$(sha256sum artifacts/*/BALLView-*-windows-x64-installer.exe | awk '{print $1}')
       ```
       And include in the BODY heredoc.
    3. Update the Release BODY heredoc (L522) to point Windows users at the README-SMARTSCREEN.md path:
       ```
       - **Windows:** download the `.exe` installer. Windows SmartScreen will warn ("More info → Run anyway"). See
         [packaging/windows/README-SMARTSCREEN.md](https://github.com/BALL-Project/ball/blob/v1.7/packaging/windows/README-SMARTSCREEN.md)
         for context + SHA256 verification.
         SHA256: `${INSTALLER_SHA256}`
       ```
    4. Commit:
       ```
       packaging(win): add SmartScreen-warning README for unsigned-fallback path (Phase 8b)

       Plan-B deliverable: if SignPath Foundation approval doesn't land by
       v1.7 RC tag, ship unsigned + this README explains the SmartScreen
       warning + SHA256 verification path. Release body links the README +
       emits SHA256 per installer.
       ```
  </action>
  <verify>
    <automated>cd /Users/kohlbach/Claude/BALL/ball &amp;&amp; test -f packaging/windows/README-SMARTSCREEN.md &amp;&amp; grep -q "SmartScreen" packaging/windows/README-SMARTSCREEN.md &amp;&amp; grep -q "More info" packaging/windows/README-SMARTSCREEN.md &amp;&amp; grep -q "SHA256" .github/workflows/release.yml</automated>
  </verify>
  <acceptance_criteria>
    - packaging/windows/README-SMARTSCREEN.md exists with clear "More info → Run anyway" instructions + SHA256 verification guidance.
    - release.yml emits SHA256 in the GitHub Release body.
    - Release body links to the README.
  </acceptance_criteria>
  <done>
    Unsigned-fallback path is itself a complete deliverable; v1.7 can ship the unsigned installer with clear user guidance.
  </done>
</task>

<task type="auto">
  <name>Task 5: Cross-port v1.6.2's vcpkg artifact cache seed from commit 357d13e</name>
  <files>
    .github/workflows/release.yml
    .github/workflows/ci.yml  (verify the artifact-upload counterpart exists on v1.7-modernization)
  </files>
  <read_first>
    - v1.6-modernization commit 357d13e + companion commit 2de6511791 (`release(win): cross-ref vcpkg cache via CI artifact (Option B for v1.6.2)`)
    - .github/workflows/release.yml lines 207-260 (existing vcpkg seed step already partially exists in v1.7 release.yml — verify completeness)
    - .github/workflows/ci.yml — confirm the `vcpkg-archives-windows` artifact-upload step exists on v1.7-modernization (it's the producer side; release.yml is the consumer)
  </read_first>
  <action>
    1. Check v1.7-modernization branch for the artifact-upload step in ci.yml. Already grep-confirmed during plan drafting that release.yml has the seed step (L226-261); verify the matching producer step exists in ci.yml (uploads `vcpkg-archives-windows` artifact after every successful Windows build).
    2. If the producer is MISSING from v1.7-modernization's ci.yml: cherry-pick or hand-port commit 357d13e (`ci(win): upload vcpkg archives artifact on every successful build`). The shape:
       ```yaml
       - name: Upload vcpkg archives (Windows — release.yml cross-ref source)
         if: always() && matrix.os == 'windows-x64'
         uses: actions/upload-artifact@v6
         with:
           name: vcpkg-archives-windows
           path: C:\Users\runneradmin\AppData\Local\vcpkg\archives
           if-no-files-found: warn
           retention-days: 14
       ```
       Insert after the Windows Build step in ci.yml.
    3. If the producer ALREADY exists: verify the consumer (release.yml L226-261) references it correctly — same `name: vcpkg-archives-windows`, same path, same retention assumption.
    4. Dry-run a Windows release on a throwaway tag to confirm the seed populates `%LOCALAPPDATA%\vcpkg\archives` and the subsequent `cmake --preset ci-windows` step hits the seeded archives (look for "Using cached binary" lines in vcpkg output, not "Building from source").
    5. If a regression appears (e.g., the v1.7 branch's vcpkg.json hash differs and invalidates the cache), document the gap in 08b-SIGNPATH-STATUS.md's appendix; the seed is fail-soft (continues without it).
    6. Commit IF cherry-pick needed:
       ```
       ci(win): port vcpkg artifact upload from v1.6.2 (Phase 8b, supports release.yml seed)

       Cross-port of v1.6-modernization commit 357d13e. ci.yml's Windows job
       uploads %LOCALAPPDATA%\vcpkg\archives as a workflow artifact on every
       successful build; release.yml's seed step downloads the latest such
       artifact to skip cold vcpkg Qt6+Boost compile (the v1.6.2 OOM
       recurring failure mode).
       ```
  </action>
  <verify>
    <automated>cd /Users/kohlbach/Claude/BALL/ball &amp;&amp; grep -c "vcpkg-archives-windows" .github/workflows/release.yml &amp;&amp; grep -c "vcpkg-archives-windows" .github/workflows/ci.yml</automated>
  </verify>
  <acceptance_criteria>
    - ci.yml has the vcpkg-archives-windows upload-artifact step on the Windows job.
    - release.yml seed step (already present) successfully downloads the artifact on dry-run.
    - vcpkg cold-build avoided on release; total Windows release wall-clock drops back toward the warm-cache ~25min vs the cold-vcpkg ~2h+OOM-risk.
  </acceptance_criteria>
  <done>
    vcpkg artifact cache cross-ported; release.yml Windows job warm-builds reliably.
  </done>
</task>

<task type="auto">
  <name>Task 6: Validate full pipeline on windows-2025 runner (dry-run release)</name>
  <files>(none — validation only)</files>
  <read_first>
    - Outputs of Tasks 2 + 4 + 5 (signed gate is separately validated in Task 3 when it opens)
  </read_first>
  <action>
    1. Push all preceding-task commits to v1.7-modernization.
    2. Tag a throwaway test: `git tag v1.7-test-pkg-1 && git push origin v1.7-test-pkg-1`.
    3. Trigger and watch: `gh workflow run release.yml -f tag=v1.7-test-pkg-1 -f draft=true; gh run watch <id>`.
    4. Verify the resulting GitHub Release (draft) has:
       - `BALLView-v1.7-test-pkg-1-windows-x64-installer.exe` — the NSIS installer
       - `BALLView-v1.7-test-pkg-1-windows-x64.zip` — the portable .zip (kept during transition)
       - Release body contains SHA256 + SmartScreen README link
    5. Hand-install on a Windows 11 VM (or, lacking access, on a windows-2025 ephemeral VM via cloud). Confirm:
       - Installer launches without UAC admin prompt (per-user install).
       - Default install location: `%LOCALAPPDATA%\Programs\BALLView\`.
       - Start Menu shortcut "BALLView" created.
       - `BALLView.exe` launches; the 3D scene renders (cores: load a sample molecule and confirm visible rendering).
       - Uninstall via Settings → Apps cleanly removes the install directory.
    6. Delete the throwaway tag + draft release: `gh release delete v1.7-test-pkg-1 -y; git tag -d v1.7-test-pkg-1; git push origin :refs/tags/v1.7-test-pkg-1`.
    7. Document the validation run URL + Windows VM observations in `.planning/phases/08b-packaging-windows/08b-01-SUMMARY.md`.
  </action>
  <verify>
    <automated>echo "MANUAL — see 08b-01-SUMMARY.md windows-2025 validation run URL + VM install observations"</automated>
  </verify>
  <acceptance_criteria>
    - Dry-run release on v1.7-test-pkg-1 produces both .exe installer + .zip portable as Release assets.
    - Installer launches on Windows 11 VM without admin prompt; per-user install to `%LOCALAPPDATA%\Programs\BALLView`.
    - BALLView.exe launches post-install + renders a sample molecule.
    - Uninstall is clean (no orphan files in install dir or registry).
  </acceptance_criteria>
  <done>
    Full pipeline validated on windows-2025 runner + Windows 11 VM; ready for v1.7 RC.
  </done>
</task>

<task type="manual">
  <name>Task 7: Bus-factor documentation — 1Password + 2-human recovery procedure</name>
  <files>
    .planning/phases/08b-packaging-windows/08b-BUSFACTOR.md  (NEW)
  </files>
  <read_first>
    - .planning/v1.7-PLAN.md risk register (the "Bus factor 1 on signing infrastructure" P0 entry)
    - .planning/phases/08b-packaging-windows/08b-SIGNPATH-STATUS.md (Task 1 — primary + secondary admin identified)
  </read_first>
  <action>
    1. Create `.planning/phases/08b-packaging-windows/08b-BUSFACTOR.md`:
       ```markdown
       # Phase 8b — Bus-Factor Mitigation for Windows Signing Infrastructure

       **Status:** required deliverable per v1.7-PLAN.md risk register P0 entry
       ("Bus factor 1 on signing infrastructure (NEW post-review P0)").

       ## Roles

       | Role | Primary | Secondary |
       |---|---|---|
       | SignPath Foundation admin | <name + email> | <name + email> |
       | GitHub repo admin (manages SIGNPATH_API_TOKEN secret) | <name + email> | <name + email> |
       | 1Password vault owner (BALL/Windows-Signing/) | <name + email> | <name + email> |

       Both roles MUST have at least 2 humans. If only one human is available
       at v1.7 RC time, this deliverable is INCOMPLETE — file a P0 issue.

       ## Credential storage

       All credentials stored in **1Password vault `BALL/Windows-Signing/`**
       (or org-equivalent secret manager — Bitwarden, KeePass, etc.; doc the
       chosen tool here).

       Stored items:
       - `SIGNPATH_API_TOKEN` — the GitHub Actions secret; SignPath API access
       - `SIGNPATH_ORG_ID` — the foundation organization slug
       - `SIGNPATH_PROJECT_SLUG` — `ballview`
       - `SIGNPATH_POLICY_SLUG` — `release-signing`
       - SignPath Foundation account recovery email
       - SignPath Foundation account 2FA backup codes
       - GitHub repo secrets configuration screenshot (for verification)

       ## Recovery procedure (if primary admin unavailable)

       1. Secondary admin logs into 1Password / equivalent.
       2. Secondary admin logs into SignPath Foundation portal with stored credentials.
       3. Secondary admin can:
          a. Approve pending release signing requests.
          b. Rotate `SIGNPATH_API_TOKEN` if compromised (regenerate in SignPath UI; update GitHub secret).
          c. Add additional admins to the SignPath organization.
       4. Document the recovery event in 08b-SIGNPATH-STATUS.md.

       ## Fallback-path bus-factor (Path B Azure / Path C commercial)

       If we end up on Path B (Azure Code Signing): document the Azure
       subscription owner + RBAC role assignments here (Azure RBAC supports
       multiple owners natively — easier than SignPath).

       If we end up on Path C (commercial OV + cloud HSM): document the cert
       reseller account credentials + cloud HSM access (SSL.com eSigner /
       DigiCert KeyLocker / etc.).

       ## Renewal cadence reminder

       - SignPath: foundation auto-renews; verify annually.
       - CA/B cert max validity post-March 2026: **458 days**. If on Path B or
         Path C, calendar reminder at 12 months for cert renewal.
       - Annual review (whichever path): 2027-05-17 — verify both admins still
         active + credentials still valid + recovery procedure still works.
       ```
    2. Fill in the actual names + emails (post-Task-1 — Task 1 identifies who).
    3. Add the file to .planning/ (already inside .planning/, so it's git-tracked by default; no extra step).
    4. Commit:
       ```
       packaging(win): document signing-infrastructure bus-factor (Phase 8b, PACK-WIN-BUSFACTOR)

       Per v1.7-PLAN.md P0 risk register entry. 2-human recovery procedure
       for SignPath admin + GitHub secrets + 1Password vault. Annual renewal
       reminder set for 2027-05-17.
       ```
  </action>
  <verify>
    <automated>cd /Users/kohlbach/Claude/BALL/ball &amp;&amp; test -f .planning/phases/08b-packaging-windows/08b-BUSFACTOR.md &amp;&amp; grep -q "Primary" .planning/phases/08b-packaging-windows/08b-BUSFACTOR.md &amp;&amp; grep -q "Secondary" .planning/phases/08b-packaging-windows/08b-BUSFACTOR.md &amp;&amp; grep -q "1Password" .planning/phases/08b-packaging-windows/08b-BUSFACTOR.md &amp;&amp; grep -q "2027-05-17" .planning/phases/08b-packaging-windows/08b-BUSFACTOR.md</automated>
  </verify>
  <acceptance_criteria>
    - 08b-BUSFACTOR.md exists with roles table populated (both Primary AND Secondary filled in).
    - 1Password vault path documented.
    - Recovery procedure documented.
    - Annual renewal reminder set.
  </acceptance_criteria>
  <done>
    Bus-factor mitigation complete; v1.7 RC blocker on signing infra resolved.
  </done>
</task>

</tasks>

<verification>
- SignPath Foundation OSS application submitted day-1 + weekly status tracked in 08b-SIGNPATH-STATUS.md.
- release.yml builds CPack NSIS installer; works signed (if cert available) OR unsigned (fallback).
- packaging/windows/README-SMARTSCREEN.md exists with SmartScreen warning instructions + SHA256 verification.
- vcpkg artifact cache seed cross-ported from v1.6.2 commit 357d13e; ci.yml producer + release.yml consumer aligned.
- Dry-run release on windows-2025 produces installable .exe + Windows VM validates per-user install + 3D rendering + clean uninstall.
- 08b-BUSFACTOR.md documents 2-human recovery for SignPath admin + signtool credentials.
- v1.7 RC tag NOT blocked by 8b — unsigned-with-warning fallback is itself a complete deliverable.
</verification>

<success_criteria>
- Windows installer (.exe) ships in v1.7 RC release — signed if SignPath approved, unsigned-with-SmartScreen-warning README if not.
- All 7 tasks have recorded state (in-progress / complete / gated-pending-cert).
- SignPath application is in-flight from day-1 with weekly status updates.
- Bus-factor mitigation documented + verified (2 humans with recovery access).
- vcpkg artifact cache cross-port eliminates cold-build OOM risk on Windows release pipeline.
- Phase 8b can complete async post-v1.7-RC if SignPath approval lands later — backfill the signed installer to v1.7 release assets via `gh release upload --clobber`.
</success_criteria>

<output>
Create `.planning/phases/08b-packaging-windows/08b-01-SUMMARY.md` per template. Include:
- SignPath application reference number + current foundation review state.
- Per-task completion state (which tasks complete / which gated / which pending external action).
- Windows VM validation observations (per-user install path, Start Menu shortcut, 3D-rendering confirmation, clean uninstall).
- vcpkg cache seed wall-clock improvement (cold vs warm release-pipeline timing).
- Bus-factor table (Primary + Secondary admins; 1Password vault path).
- Active path indicator: SIGNED (SignPath approved) | UNSIGNED-FALLBACK (Plan B README path) | PENDING (still in foundation review).
- v1.7 RC readiness flag — green regardless of signing state.
</output>
