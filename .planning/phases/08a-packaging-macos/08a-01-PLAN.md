---
phase: 08a-packaging-macos
plan: 01
type: execute
wave: 1
depends_on: []
files_modified:
  - .github/workflows/release.yml
  - packaging/macos/BALLView.entitlements  # NEW — hardened-runtime entitlements
  - packaging/macos/dmg-background.png      # NEW (optional, drag-to-Applications hint)
  - BUILD-macos.md                          # signing/notarization runbook section
  - .planning/phases/08a-packaging-macos/08a-SECRETS-RUNBOOK.md  # NEW — 2-human recovery doc
autonomous: false  # cert procurement (Task 1) requires a human with Apple Developer Program enrollment
requirements:
  - PKG-MAC-01  # signed BALLView.app with Developer ID Application cert + hardened runtime
  - PKG-MAC-02  # notarytool submit + staple end-to-end in release.yml
  - PKG-MAC-03  # signed .dmg artifact via create-dmg, replacing the existing .zip
  - PKG-MAC-04  # ad-hoc resign workaround removed from release.yml
  - PKG-MAC-05  # 2-human recovery doc for Apple Developer credentials (bus-factor mitigation)
must_haves:
  truths:
    - "Apple Developer Program enrolled + 'Developer ID Application' cert issued + exported as .p12 + base64-encoded into the MACOS_CERTIFICATE_BASE64 GitHub Actions secret"
    - "Apple Developer credentials (account login, app-specific notarytool password, .p12 export password, keychain recovery) stored in 1Password (or org-equivalent) AND known to ≥2 humans — bus-factor risk explicit in v1.7-PLAN risk register"
    - "BALLView.app codesigned with `--options runtime` and a valid Developer ID Application identity; codesign --verify --deep --strict exits 0"
    - "Bundle notarized via `xcrun notarytool submit --wait`; notary returns Accepted; ticket stapled with `xcrun stapler staple` and validated with `xcrun stapler validate`"
    - "release.yml's ad-hoc resign step (`codesign --force --deep --sign -`) is DELETED; the new signed-identity codesign step replaces it"
    - "Final artifact is a signed + notarized .dmg (created via create-dmg with --codesign), NOT a .zip; release upload step updated accordingly"
    - "spctl --assess --type execute --verbose=4 on the downloaded .dmg's mounted .app reports 'accepted' + 'source=Notarized Developer ID'"
    - "If Apple Developer enrollment is BLOCKED (procurement delay > 1 week), fallback path: keep ad-hoc resign + ship unsigned-with-warning; document the block in 08a-SECRETS-RUNBOOK.md and escalate to v1.7 risk register"
  artifacts:
    - path: ".github/workflows/release.yml"
      provides: "macOS job: signed codesign step, notarytool submit+wait, stapler staple, create-dmg packaging, .dmg upload"
      contains: "Developer ID Application"
    - path: "packaging/macos/BALLView.entitlements"
      provides: "hardened-runtime entitlements (allow-jit + disable-library-validation for Qt plugin loading)"
      contains: "com.apple.security.cs.disable-library-validation"
    - path: ".planning/phases/08a-packaging-macos/08a-SECRETS-RUNBOOK.md"
      provides: "bus-factor mitigation: who holds Apple Developer credentials, where stored, recovery procedure, renewal cadence"
      contains: "1Password"
    - path: "BUILD-macos.md"
      provides: "developer note: local builds skip signing; runtime/notarization is CI-only"
      contains: "Developer ID"
  key_links:
    - from: ".planning/phases/08-packaging-and-distribution/08-SIGNING-RESEARCH.md (the macOS signing flow + secret list)"
      to: ".github/workflows/release.yml (the wired-up signing + notarization steps)"
      via: "research doc enumerates the 7 secrets + the 7-step signing flow; release.yml step blocks implement them in order"
      pattern: "MACOS_CERTIFICATE_BASE64|MACOS_SIGNING_IDENTITY|MACOS_NOTARY_PASSWORD"
    - from: ".planning/phases/08-packaging-and-distribution/08-INSTALLER-FORMATS-RESEARCH.md (create-dmg as the chosen DMG tool)"
      to: ".github/workflows/release.yml::create-dmg step (replaces ditto zip)"
      via: "create-dmg --codesign + --notarize flags compose with the signing identity already in the keychain"
      pattern: "create-dmg"
    - from: ".github/workflows/release.yml (current ad-hoc resign step, lines 172-195)"
      to: "DELETED — replaced by the signed-identity codesign step"
      via: "ad-hoc was the macOS 26 SIGKILL workaround; proper Developer ID signature satisfies CODESIGNING enforcement without the workaround"
      pattern: "codesign --force --deep --sign -"
---

<objective>
Wire Apple Developer ID signing + Apple notarization + a signed .dmg artifact
into the release.yml macOS job, replacing the existing ad-hoc resign workaround
that v1.6.x shipped (release.yml lines 172-195). End-state: a double-click-clean
`.dmg` that mounts a `BALLView.app` Gatekeeper accepts as
"Notarized Developer ID" on a fresh macOS 14 (or macOS 26) install — no
right-click-Open dance, no `xattr -dr com.apple.quarantine`.

Phase 8a is the macOS slice of the post-review Phase 8 split (8a/8b/8c — see
[v1.7-PLAN.md "Phase list"](../../v1.7-PLAN.md)). 8b (Windows / SignPath) and
8c (Linux) run independently; this plan only touches macOS.

The work has two halves: (1) cert procurement + GitHub secrets setup — an
**external dependency that MUST start day-1 of Wave 1** because Apple Developer
enrollment can take days, and the bundle-ID's first notarytool submission can
take 8-12 hours (per [08-SIGNING-RESEARCH.md](../08-packaging-and-distribution/08-SIGNING-RESEARCH.md)
"Watch-outs"); (2) the release.yml workflow surgery, which is pure code and
fast once the cert exists.

The signing flow itself is well-documented in
[08-SIGNING-RESEARCH.md](../08-packaging-and-distribution/08-SIGNING-RESEARCH.md) —
this plan executes the recipe, doesn't re-derive it.

Fallback (per v1.7-PLAN risk register): if Apple Developer enrollment is
blocked > 1 week into Wave 1, ship unsigned-with-warning (keep the ad-hoc
resign), document the block, and reopen 8a in a follow-on cycle. Do NOT block
8c (Linux) or other Wave 1 phases on this.

Output: 1 entitlements file + 1 release.yml refactor + 1 secrets-runbook doc
+ 1 BUILD-macos.md note + end-to-end validation on a macOS 14 runner with a
green spctl assess on the released .dmg.
</objective>

<execution_context>
@$HOME/.claude/get-shit-done/workflows/execute-plan.md
@$HOME/.claude/get-shit-done/templates/summary.md
</execution_context>

<context>
@.planning/v1.7-PLAN.md
@.planning/ROADMAP.md
@.planning/phases/08-packaging-and-distribution/08-SIGNING-RESEARCH.md
@.planning/phases/08-packaging-and-distribution/08-INSTALLER-FORMATS-RESEARCH.md
@.github/workflows/release.yml

<interfaces>
**The 7 GitHub Actions secrets (from 08-SIGNING-RESEARCH.md):**
- `MACOS_CERTIFICATE_BASE64` — Developer ID Application .p12 export, base64-encoded
- `MACOS_CERTIFICATE_PASSWORD` — .p12 export password
- `MACOS_KEYCHAIN_PASSWORD` — ephemeral CI keychain password (generate fresh)
- `MACOS_APPLE_ID` — Apple Developer account email
- `MACOS_TEAM_ID` — 10-character team identifier
- `MACOS_NOTARY_PASSWORD` — app-specific password from appleid.apple.com (NOT account password)
- `MACOS_SIGNING_IDENTITY` — `"Developer ID Application: <Org> (<TEAMID>)"`

**Current release.yml macOS surgery sites:**
- Lines 154-158: `macdeployqt` step — extend with `-codesign=` flag once identity is in keychain
- Lines 172-195: ad-hoc resign step — REPLACE with signed-identity codesign + entitlements
- Lines 197-205: ditto-zip step — REPLACE with create-dmg producing the signed .dmg
- New step before macdeployqt: install Apple cert into ephemeral CI keychain
- New step after stapler: spctl assess sanity check (fail-fast if Gatekeeper rejects)

**Entitlements baseline (08-SIGNING-RESEARCH.md "Entitlements"):**
- `com.apple.security.cs.disable-library-validation` — Qt's plugin loading model (REQUIRED)
- `com.apple.security.cs.allow-jit` — defensive; Python embedding (Phase 6, future-proof now)
- `com.apple.security.cs.allow-unsigned-executable-memory` — defensive; only if Qt or BALL plugin path eval needs it (start WITHOUT, add only if hardened-runtime test reveals a SIGKILL)

**create-dmg recipe (08-INSTALLER-FORMATS-RESEARCH.md Option A):**
```bash
create-dmg \
  --volname "BALLView ${REF}" \
  --window-pos 200 120 --window-size 600 400 \
  --icon-size 100 --icon "BALLView.app" 175 190 \
  --hide-extension "BALLView.app" \
  --app-drop-link 425 190 \
  --codesign "${MACOS_SIGNING_IDENTITY}" \
  "BALLView-${REF}-macos-arm64.dmg" \
  "$(dirname ${APP})"
```
(Skip `--background` for v1 — add the PNG later if a designer offers one; not load-bearing.)

**Validation commands (must all pass on the released .dmg):**
- `codesign --verify --deep --strict --verbose=2 BALLView.app` → exit 0
- `spctl --assess --type execute --verbose=4 BALLView.app` → "source=Notarized Developer ID"
- `xcrun stapler validate BALLView.app` → "The validate action worked!"
- Visual: double-click on macOS 14 / macOS 26 → app launches without Gatekeeper prompt

**Rollback plan:** if any post-merge release-tag dry-run fails Gatekeeper assess,
revert the signing commit (NOT the secrets — secrets are reusable). Restore
the ad-hoc resign step from git history. File a follow-up. The secrets-runbook
doc stays — it's milestone-spanning, not commit-scoped.

**Bus-factor risk (08-SIGNING-RESEARCH.md Open Q1 + v1.7-PLAN risk register P0):**
Apple Developer Program enrollment is per-legal-entity OR per-individual.
Decision MUST be recorded in 08a-SECRETS-RUNBOOK.md before any cert procurement.
Project-level / institutional enrollment is preferable (org name appears in
signed bundle metadata, more durable across maintainer turnover) but requires
institutional sponsorship; individual enrollment is the fallback. Either way,
≥2 humans must know the recovery path (1Password share, account recovery
email, billing) BEFORE v1.7-RC-1 tag.
</interfaces>
</context>

<tasks>

<task type="manual-then-auto">
  <name>Task 08a-01: Apple Developer cert procurement + GitHub secrets storage + 2-human recovery doc</name>
  <files>
    .planning/phases/08a-packaging-macos/08a-SECRETS-RUNBOOK.md
  </files>
  <read_first>
    - .planning/phases/08-packaging-and-distribution/08-SIGNING-RESEARCH.md (sections "Requirements" + "GitHub Actions integration" + "Cost" + Open Q1)
    - .planning/v1.7-PLAN.md (risk register row: "Bus factor 1 on signing infrastructure (NEW post-review P0)")
  </read_first>
  <action>
    **EXTERNAL DEPENDENCY — start day-1 of Wave 1. This is the bus-factor critical path.**

    1. Decide enrollment shape (project-level vs individual) — record in 08a-SECRETS-RUNBOOK.md.
    2. Enroll in Apple Developer Program ($99/year, https://developer.apple.com/programs/) — needs Apple ID, payment method, and (if org) D-U-N-S number + legal-entity verification (can take days).
    3. In Apple Developer portal: Certificates → "+" → "Developer ID Application" → CSR from Keychain Access → download `developerID_application.cer`.
    4. Double-click the .cer to install in login keychain. In Keychain Access, export the cert + private key as a single `.p12` file with a strong password.
    5. Generate an app-specific password at https://appleid.apple.com → Sign-In and Security → App-Specific Passwords. Label it "BALL notarytool CI".
    6. Find the 10-character Team ID at https://developer.apple.com/account → Membership.
    7. base64-encode the .p12: `base64 -i developerID_application.p12 -o cert.b64`.
    8. Add the 7 secrets to `BALL-Project/ball` repo settings → Secrets and variables → Actions:
       - `MACOS_CERTIFICATE_BASE64` (paste cert.b64 contents)
       - `MACOS_CERTIFICATE_PASSWORD` (the .p12 export password)
       - `MACOS_KEYCHAIN_PASSWORD` (generate fresh, e.g. `openssl rand -base64 24`)
       - `MACOS_APPLE_ID` (your Apple Developer account email)
       - `MACOS_TEAM_ID` (the 10-char ID)
       - `MACOS_NOTARY_PASSWORD` (the app-specific password from step 5)
       - `MACOS_SIGNING_IDENTITY` (string: `"Developer ID Application: <Org> (<TEAMID>)"`)
    9. Write 08a-SECRETS-RUNBOOK.md covering:
       - Who owns the Apple Developer Program account (name + role)
       - Who has 1Password access to the credentials (≥2 humans named)
       - Recovery procedure (Apple ID account recovery email, billing contact)
       - Renewal cadence ($99/year + cert renewal at expiry)
       - Where the .p12 + CSR + private key are backed up
       - Procedure to rotate the app-specific password if compromised
    10. Securely delete the local .p12 + cert.b64 + CSR after upload — `rm -P` (overwrite-on-delete on macOS).
    11. Locally test the keychain-import step (Task 2's first sub-step) against a throwaway keychain to confirm the .p12 + password round-trip works BEFORE the CI integration.

    **Block on:** Apple Developer enrollment approval. If > 1 week, escalate to v1.7-PLAN risk register and switch to fallback path (keep ad-hoc resign, ship unsigned-with-warning, defer 8a to v1.7.1).
  </action>
  <verify>
    <automated>gh secret list --repo BALL-Project/ball | grep -E '^MACOS_(CERTIFICATE_BASE64|CERTIFICATE_PASSWORD|KEYCHAIN_PASSWORD|APPLE_ID|TEAM_ID|NOTARY_PASSWORD|SIGNING_IDENTITY)' | wc -l | grep -q 7 &amp;&amp; test -f .planning/phases/08a-packaging-macos/08a-SECRETS-RUNBOOK.md</automated>
  </verify>
  <acceptance_criteria>
    - All 7 secrets present in `gh secret list` output for BALL-Project/ball.
    - 08a-SECRETS-RUNBOOK.md exists and names ≥2 humans with credential access.
    - Local round-trip test: `security import` of the .p12 into a throwaway keychain succeeds with `MACOS_CERTIFICATE_PASSWORD`.
  </acceptance_criteria>
  <done>
    Cert procured, secrets uploaded, 2-human recovery documented; Task 2 unblocked.
  </done>
</task>

<task type="auto">
  <name>Task 08a-02: release.yml signing wiring — keychain import + entitlements + signed codesign</name>
  <files>
    .github/workflows/release.yml
    packaging/macos/BALLView.entitlements
  </files>
  <read_first>
    - .github/workflows/release.yml (current macOS path, lines 82-205)
    - .planning/phases/08-packaging-and-distribution/08-SIGNING-RESEARCH.md (sections "The signing flow" + "Entitlements" + "GitHub Actions integration")
  </read_first>
  <action>
    1. Author `packaging/macos/BALLView.entitlements` (plist XML):
       ```xml
       <?xml version="1.0" encoding="UTF-8"?>
       <!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN"
         "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
       <plist version="1.0">
       <dict>
         <key>com.apple.security.cs.disable-library-validation</key>
         <true/>
         <key>com.apple.security.cs.allow-jit</key>
         <true/>
       </dict>
       </plist>
       ```
       (`allow-unsigned-executable-memory` deferred — add only if hardened-runtime test reveals a SIGKILL.)
    2. Insert a new `Install Apple cert into CI keychain` step in release.yml BEFORE the existing `macdeployqt` step (lines 154-158). Use the recipe from 08-SIGNING-RESEARCH.md "GitHub Actions integration" verbatim.
    3. Update the `macdeployqt` step (line 158) to pass `-codesign="${{ secrets.MACOS_SIGNING_IDENTITY }}"`.
    4. DELETE the existing ad-hoc resign step (lines 172-195). Replace with:
       ```yaml
       - name: Codesign BALLView.app with Developer ID + hardened runtime (macOS)
         if: matrix.os == 'macos-arm64'
         run: |
           codesign --deep --force --options runtime \
             --sign "${{ secrets.MACOS_SIGNING_IDENTITY }}" \
             --entitlements packaging/macos/BALLView.entitlements \
             --timestamp \
             "${{ steps.locate-mac.outputs.app }}"
           codesign --verify --deep --strict --verbose=2 \
             "${{ steps.locate-mac.outputs.app }}"
       ```
    5. Validate workflow YAML: `actionlint .github/workflows/release.yml` exits 0.
    6. Commit: `feat(release): sign macOS bundle with Developer ID + hardened runtime (Phase 8a, PKG-MAC-01)`.
  </action>
  <verify>
    <automated>cd /Users/kohlbach/Claude/BALL/ball &amp;&amp; test -f packaging/macos/BALLView.entitlements &amp;&amp; grep -q 'disable-library-validation' packaging/macos/BALLView.entitlements &amp;&amp; grep -q 'MACOS_SIGNING_IDENTITY' .github/workflows/release.yml &amp;&amp; grep -q 'options runtime' .github/workflows/release.yml &amp;&amp; ! grep -q 'codesign --force --deep --sign -' .github/workflows/release.yml &amp;&amp; (command -v actionlint &amp;&amp; actionlint .github/workflows/release.yml || python3 -c "import yaml; yaml.safe_load(open('.github/workflows/release.yml'))")</automated>
  </verify>
  <acceptance_criteria>
    - packaging/macos/BALLView.entitlements exists with at least `disable-library-validation`.
    - release.yml has a keychain-import step before macdeployqt.
    - release.yml's codesign step uses `--options runtime` + entitlements + Developer ID identity.
    - The old ad-hoc `codesign --force --deep --sign -` line is GONE from release.yml.
    - actionlint exits 0.
  </acceptance_criteria>
  <done>
    Signing wired into release.yml; the bundle gets a real Developer ID signature with hardened runtime; ad-hoc workaround removed.
  </done>
</task>

<task type="auto">
  <name>Task 08a-03: Notarization (notarytool submit + wait) + stapler + spctl assess validation</name>
  <files>
    .github/workflows/release.yml
  </files>
  <read_first>
    - .planning/phases/08-packaging-and-distribution/08-SIGNING-RESEARCH.md ("The signing flow" steps 4-7 + "Watch-outs": first-submission 8-12h delay)
    - .github/workflows/release.yml (the Task 2 wiring as just landed)
  </read_first>
  <action>
    Add three new steps to release.yml AFTER the Task 2 codesign step and BEFORE Task 4's create-dmg step:

    1. **Zip-for-notary** (notarytool wants .zip/.dmg/.pkg):
       ```yaml
       - name: Zip .app for notary submission (macOS)
         if: matrix.os == 'macos-arm64'
         run: |
           ditto -c -k --sequesterRsrc --keepParent \
             "${{ steps.locate-mac.outputs.app }}" \
             "${{ runner.temp }}/BALLView-notarize.zip"
       ```
    2. **notarytool submit + wait**:
       ```yaml
       - name: Submit to Apple notary + wait (macOS)
         if: matrix.os == 'macos-arm64'
         timeout-minutes: 60
         run: |
           xcrun notarytool submit \
             "${{ runner.temp }}/BALLView-notarize.zip" \
             --apple-id "${{ secrets.MACOS_APPLE_ID }}" \
             --team-id "${{ secrets.MACOS_TEAM_ID }}" \
             --password "${{ secrets.MACOS_NOTARY_PASSWORD }}" \
             --wait
       ```
       Note: first-ever submission for a fresh bundle ID can take 8-12 hours per 08-SIGNING-RESEARCH.md. 60-min timeout is for steady-state; FIRST release after Task 1 may need a manual `notarytool submit` priming run done locally (document this in 08a-SECRETS-RUNBOOK.md).
    3. **Staple ticket + validate**:
       ```yaml
       - name: Staple notarization ticket (macOS)
         if: matrix.os == 'macos-arm64'
         run: |
           xcrun stapler staple "${{ steps.locate-mac.outputs.app }}"
           xcrun stapler validate "${{ steps.locate-mac.outputs.app }}"
           spctl --assess --type execute --verbose=4 \
             "${{ steps.locate-mac.outputs.app }}" 2>&1 | tee /tmp/spctl-assess.log
           grep -q 'source=Notarized Developer ID' /tmp/spctl-assess.log
       ```
       (The `grep -q 'source=Notarized Developer ID'` fail-fasts the job if Gatekeeper would not accept the bundle — caught at build time, not at user install time.)
    4. Validate workflow YAML: `actionlint .github/workflows/release.yml`.
    5. Commit: `feat(release): notarize + staple macOS bundle (Phase 8a, PKG-MAC-02)`.
  </action>
  <verify>
    <automated>cd /Users/kohlbach/Claude/BALL/ball &amp;&amp; grep -q 'notarytool submit' .github/workflows/release.yml &amp;&amp; grep -q 'stapler staple' .github/workflows/release.yml &amp;&amp; grep -q 'source=Notarized Developer ID' .github/workflows/release.yml &amp;&amp; (command -v actionlint &amp;&amp; actionlint .github/workflows/release.yml || python3 -c "import yaml; yaml.safe_load(open('.github/workflows/release.yml'))")</automated>
  </verify>
  <acceptance_criteria>
    - release.yml has notarytool submit step with `--wait`.
    - release.yml has stapler staple + validate step.
    - spctl assess fail-fast grep is present (`source=Notarized Developer ID`).
    - actionlint exits 0.
    - Wet run: a dry release on a throwaway tag (`vDEV-08a-notarize-smoke`) completes the notarize+staple steps green; spctl reports Notarized Developer ID.
  </acceptance_criteria>
  <done>
    Notarization end-to-end wired; Gatekeeper acceptance fail-fasted at CI time.
  </done>
</task>

<task type="auto">
  <name>Task 08a-04: create-dmg packaging + macOS-14 runner end-to-end verification + ad-hoc fallback removal</name>
  <files>
    .github/workflows/release.yml
    BUILD-macos.md
  </files>
  <read_first>
    - .planning/phases/08-packaging-and-distribution/08-INSTALLER-FORMATS-RESEARCH.md (Option A — create-dmg recipe)
    - .github/workflows/release.yml (post-Task 3 state — ditto-zip step at lines 197-205 is the one to replace)
    - BUILD-macos.md (developer-facing build doc — needs a "signing/notarization is CI-only" note)
  </read_first>
  <action>
    1. **Install create-dmg** — append to the existing Homebrew install line (release.yml line ~85):
       ```
       brew install qt boost eigen fftw tbb glew open-babel lp_solve libsvm flex bison ccache create-dmg
       ```
    2. **Replace the ditto-zip step (lines 197-205)** with the create-dmg recipe from 08-INSTALLER-FORMATS-RESEARCH.md Option A:
       ```yaml
       - name: Build signed .dmg via create-dmg (macOS)
         if: matrix.os == 'macos-arm64'
         shell: bash
         run: |
           APP_DIR=$(dirname "${{ steps.locate-mac.outputs.app }}")
           DMG="${{ github.workspace }}/BALLView-${{ steps.resolve.outputs.ref }}-macos-arm64.dmg"
           create-dmg \
             --volname "BALLView ${{ steps.resolve.outputs.ref }}" \
             --window-pos 200 120 --window-size 600 400 \
             --icon-size 100 --icon "BALLView.app" 175 190 \
             --hide-extension "BALLView.app" \
             --app-drop-link 425 190 \
             --codesign "${{ secrets.MACOS_SIGNING_IDENTITY }}" \
             "$DMG" "$APP_DIR/BALLView.app"
           ls -lh "$DMG"
       ```
    3. **Update the `resolve` step's `asset=` line** (line 61) — change the `.zip` suffix to `.dmg` so the artifact-upload step picks up the new file shape. Also update the release-job's `gh release upload` glob (line ~540) from `*.zip` to `*.zip *.dmg` (Windows still ships .zip until Phase 8b).
    4. **Verify end-to-end on a fresh tag**: push `vDEV-08a-e2e-smoke` to trigger release.yml; `gh run watch` to completion. Download the .dmg artifact; on a clean macOS 14 VM (or `xattr -dr com.apple.quarantine` on a runner-equivalent path):
       - Mount the .dmg, drag BALLView.app to /Applications.
       - `spctl --assess --type execute --verbose=4 /Applications/BALLView.app` → must report `source=Notarized Developer ID`.
       - Double-click BALLView.app → must launch with NO Gatekeeper prompt.
       - `xcrun stapler validate /Applications/BALLView.app` → must report "The validate action worked!".
    5. **Confirm the ad-hoc fallback is fully gone**: `grep 'codesign --force --deep --sign -' .github/workflows/release.yml` must return zero matches.
    6. **Update BUILD-macos.md** — add a "Code signing" subsection:
       > Local builds do NOT sign or notarize the bundle. Signing + notarization happen only in CI (`.github/workflows/release.yml`) on tag pushes. To test the unsigned local build, run `xattr -dr com.apple.quarantine path/to/BALLView.app` once after the first launch warning. Developers do NOT need an Apple Developer ID for local development.
    7. Delete the throwaway `vDEV-08a-e2e-smoke` tag + draft release after verification.
    8. Commit: `feat(release): ship signed .dmg via create-dmg; remove ad-hoc resign fallback (Phase 8a, PKG-MAC-03/04)`.
  </action>
  <verify>
    <automated>cd /Users/kohlbach/Claude/BALL/ball &amp;&amp; grep -q 'create-dmg' .github/workflows/release.yml &amp;&amp; grep -q 'macos-arm64.dmg' .github/workflows/release.yml &amp;&amp; ! grep -q 'codesign --force --deep --sign -' .github/workflows/release.yml &amp;&amp; grep -q 'Code signing' BUILD-macos.md &amp;&amp; (command -v actionlint &amp;&amp; actionlint .github/workflows/release.yml || python3 -c "import yaml; yaml.safe_load(open('.github/workflows/release.yml'))")</automated>
  </verify>
  <acceptance_criteria>
    - release.yml produces a signed `.dmg` (NOT a `.zip`) for macos-arm64.
    - Ad-hoc resign step is fully removed (grep returns nothing).
    - End-to-end smoke tag verified: spctl reports Notarized Developer ID + double-click launches clean on macOS 14.
    - BUILD-macos.md has a "Code signing" subsection covering local-dev behavior.
    - Release-job's `gh release upload` glob updated to include `.dmg`.
    - actionlint exits 0.
  </acceptance_criteria>
  <done>
    Signed + notarized + stapled .dmg is the released macOS artifact; ad-hoc fallback gone; Phase 8a COMPLETE on macOS-14 runner.
  </done>
</task>

</tasks>

<verification>
- All 7 MACOS_* secrets present in BALL-Project/ball repo (`gh secret list`).
- 08a-SECRETS-RUNBOOK.md names ≥2 humans with credential access.
- packaging/macos/BALLView.entitlements exists with hardened-runtime-compatible keys.
- release.yml: keychain-import → macdeployqt-with-codesign → Developer-ID-codesign → notarytool → stapler → spctl-assess fail-fast → create-dmg pipeline is wired in order.
- release.yml ad-hoc resign step (`codesign --force --deep --sign -`) is GONE.
- Smoke tag (`vDEV-08a-e2e-smoke`) produces a .dmg that passes spctl Notarized Developer ID + double-click-clean on macOS 14.
- BUILD-macos.md mentions signing is CI-only for local-dev clarity.
- actionlint on release.yml exits 0.
</verification>

<success_criteria>
- PKG-MAC-01 satisfied: BALLView.app signed with Developer ID Application + hardened runtime.
- PKG-MAC-02 satisfied: notarytool submit + stapler staple end-to-end in release.yml.
- PKG-MAC-03 satisfied: signed `.dmg` shipped via create-dmg, replacing `.zip`.
- PKG-MAC-04 satisfied: ad-hoc resign workaround deleted from release.yml.
- PKG-MAC-05 satisfied: 08a-SECRETS-RUNBOOK.md documents 2-human credential recovery path.
- Released .dmg passes spctl assess + double-click launches without Gatekeeper prompt on a clean macOS 14 install.
- Phase 8a closure: 8b (Windows) + 8c (Linux) remain independent and unblocked.
</success_criteria>

<output>
Create `.planning/phases/08a-packaging-macos/08a-01-SUMMARY.md` per template. Include:
- Apple Developer Program enrollment shape (project-level vs individual) + named credential holders (≥2).
- Smoke-tag release URL with all post-sign verification command outputs (codesign --verify, spctl --assess, stapler validate).
- Diff blocks for the release.yml signing + notarization + create-dmg steps.
- Confirmation that the ad-hoc resign step is removed.
- PKG-MAC-01..05 ready-to-mark-Complete flag.
- Cross-reference to 8b/8c PLAN.md files (Wave 1 parallel siblings).
</output>
