# macOS Signing & Notarization — Secrets Runbook

**Phase:** 08a-packaging-macos
**Owner:** TBD (Apple Developer Program enrollment holder)
**Co-owner:** TBD (bus-factor backup)
**Last reviewed:** 2026-05-17
**Status:** DRAFT — pending Apple Developer Program enrollment

This document is the 2-human recovery path for the Apple Developer credentials
that power the macOS code-signing + notarization pipeline in
`.github/workflows/release.yml`. It is **milestone-spanning** — it survives
commits, branches, and Phase 8a's PR. Keep it accurate.

If you are reading this because the cert expired, the laptop with the .p12
died, or the credential-holder left the org — see "Recovery procedure" below.

---

## 1. Enrollment shape — DECISION REQUIRED before cert procurement

Apple Developer Program enrollment is **per-legal-entity OR per-individual**.
This decision MUST be recorded here before any cert is procured. Changing it
later requires a fresh enrollment (Apple does not transfer cert ownership
between accounts).

| Shape | Pros | Cons |
|---|---|---|
| **Organization** (Project / University) | Org name in signed bundle metadata; survives maintainer turnover; D-U-N-S identity matches the institution users expect | Requires D-U-N-S number; legal-entity verification (days–weeks); needs institutional sponsor / billing approval |
| **Individual** | Fast (~1 day enrollment); only needs Apple ID + payment | Bundle signed by an individual's name; cert transfer on maintainer change requires full re-enrollment of successor |

**Decision:** `[ ] Organization` / `[x] Individual` — Oliver Kohlbacher
(Apple ID `okohlbacher@me.com`, Team ID `9WF4NVY9MY`).
**Rationale:** Individual enrollment was already active and is ~1-day vs the
days–weeks D-U-N-S path for org; chosen to unblock v1.7.1 signing now. The
signed bundle's developer name shows "Oliver Kohlbacher" rather than the
institution. The bundle ID stays `de.uni-tuebingen.ball.ballview` (bundle ID
need not match the cert org). Revisit org enrollment for a later milestone
if institutional identity in the signature becomes a requirement.

---

## 2. The 7 GitHub Actions secrets

All seven secrets must be present in **`BALL-Project/ball` → Settings →
Secrets and variables → Actions → Repository secrets** before
`release.yml`'s macOS job will produce a signed bundle. If any secret is
absent, the workflow's signing/notarization steps fail visibly (they are
**not** silently skipped — unsigned shipping is no longer the fallback path
in v1.7).

| Secret name | What it is | How to obtain |
|---|---|---|
| `MACOS_CERTIFICATE_BASE64` | Developer ID Application .p12 export, base64-encoded | Keychain Access → export cert+private-key as .p12 → `base64 -i cert.p12 -o cert.b64` → paste contents |
| `MACOS_CERTIFICATE_PASSWORD` | The .p12 export password | Set during the Keychain Access export dialog — use `openssl rand -base64 24` and store in 1Password |
| `MACOS_KEYCHAIN_PASSWORD` | Ephemeral CI keychain password (NOT the .p12 password) | `openssl rand -base64 24` — never reused outside CI; rotate freely |
| `MACOS_APPLE_ID` | Apple Developer account login email | The email used for Apple Developer Program enrollment |
| `MACOS_TEAM_ID` | 10-character Team Identifier | https://developer.apple.com/account → Membership → Team ID |
| `MACOS_NOTARY_PASSWORD` | App-specific password for `notarytool` (NOT the account password) | https://appleid.apple.com → Sign-In and Security → App-Specific Passwords → "BALL notarytool CI" |
| `MACOS_SIGNING_IDENTITY` | Common-name string of the signing identity | Format: `Developer ID Application: <Org or Person Name> (<TEAMID>)` — read from `security find-identity -p codesigning -v` after the .p12 imports |

### Why an app-specific password and not the account password?

Apple deprecated 2FA-bypass via primary-account-password for `notarytool`
in 2023. Account passwords no longer work; app-specific passwords are
required. See https://support.apple.com/en-us/HT204397.

### Verification

After uploading all 7, run:

```sh
gh secret list --repo BALL-Project/ball \
  | grep -E '^MACOS_(CERTIFICATE_BASE64|CERTIFICATE_PASSWORD|KEYCHAIN_PASSWORD|APPLE_ID|TEAM_ID|NOTARY_PASSWORD|SIGNING_IDENTITY)' \
  | wc -l
# Expected: 7
```

---

## 3. Cert procurement walkthrough (one-time, ~30 min after enrollment)

1. **Enroll** in Apple Developer Program (https://developer.apple.com/programs/) — $99/year. Org enrollment may take days due to D-U-N-S verification.
2. **Generate a CSR** from Keychain Access:
   - Keychain Access → Certificate Assistant → Request a Certificate From a Certificate Authority
   - User Email: `<MACOS_APPLE_ID>`
   - Common Name: `BALL Project signing CSR`
   - "Saved to disk" — produces `CertificateSigningRequest.certSigningRequest`
3. **Request the Developer ID Application cert** at https://developer.apple.com/account → Certificates → "+":
   - Type: **Developer ID Application** (NOT "Apple Development"; NOT "Developer ID Installer")
   - Upload the CSR from step 2
   - Download `developerID_application.cer`
4. **Install** the .cer by double-clicking — it lands in your login keychain and pairs with the private key from step 2.
5. **Export as .p12** in Keychain Access:
   - Select BOTH the cert and its private key (cmd-click)
   - Right-click → Export 2 items → Format: Personal Information Exchange (.p12)
   - Set a strong password — this becomes `MACOS_CERTIFICATE_PASSWORD`
6. **base64-encode**:
   ```sh
   base64 -i developerID_application.p12 -o cert.b64
   pbcopy < cert.b64    # contents now in clipboard, ready to paste into GH
   ```
7. **App-specific password** at https://appleid.apple.com → Sign-In and Security → App-Specific Passwords → "+" → label "BALL notarytool CI". Capture it once — Apple never shows it again.
8. **Find the Team ID** at https://developer.apple.com/account → Membership.
9. **Determine the signing identity string** after importing the .p12 to a clean keychain:
   ```sh
   security find-identity -p codesigning -v
   # Look for: "Developer ID Application: <Org> (<TEAMID>)"
   ```
10. **Upload all 7 secrets** via `gh secret set` or the GitHub web UI.
11. **Securely delete local copies**:
    ```sh
    rm -P developerID_application.p12 cert.b64
    # -P overwrites before unlinking (HFS+/APFS doesn't honor it but the
    # syscall still goes through; defense-in-depth)
    ```
12. **Store backups** in the org 1Password vault:
    - The .p12 itself (yes, backup — see "Recovery" below)
    - The .p12 password
    - The app-specific password
    - The CSR + private key (so a fresh .p12 can be re-exported if needed)

---

## 4. 1Password vault layout

All credentials live in a shared 1Password vault named **`BALL — macOS signing`**.
Access list:

| Person | Role | 1Password access | Apple ID 2FA recovery device |
|---|---|---|---|
| _TBD_ | Primary credential holder | Read/Write | Phone + backup phone |
| _TBD_ | Bus-factor backup | Read/Write | Phone |

**Bus-factor rule:** at least 2 humans MUST have Read/Write access to this
vault at all times. If one leaves the project, add a successor BEFORE
removing the departing person.

Vault items:
- `Apple Developer Program account` — login email, password, recovery email, billing card last-4
- `Developer ID Application .p12` — file attachment + password
- `notarytool app-specific password` — value + creation date
- `MACOS_KEYCHAIN_PASSWORD (CI)` — value (regenerate any time; not load-bearing)
- `CSR + private key (export-source)` — file attachments

---

## 5. First-submission priming (one-time per bundle ID)

Per `08-SIGNING-RESEARCH.md` "Watch-outs": the **first-ever** `notarytool
submit` for a fresh bundle identifier can take **8–12 hours** to process
on Apple's side. CI's 60-minute timeout will fail this initial submission.

**Procedure for the first release after secrets land:**

1. From the credential-holder's machine, with the .p12 imported and the
   secrets exported as env vars, manually submit a throwaway build:
   ```sh
   xcrun notarytool submit BALLView-priming.zip \
     --apple-id "$MACOS_APPLE_ID" \
     --team-id "$MACOS_TEAM_ID" \
     --password "$MACOS_NOTARY_PASSWORD" \
     --wait
   ```
2. Once it returns Accepted (could be overnight), subsequent CI submissions
   for the same bundle ID typically resolve in 5–15 minutes.
3. Document the priming-submission timestamp in this file (Section 8).

---

## 6. Renewal cadence

| Item | Cadence | Owner reminder |
|---|---|---|
| Apple Developer Program membership | Annual ($99/year) | Calendar event 30 days before expiry |
| Developer ID Application cert | 5 years | Calendar event 60 days before expiry |
| App-specific password | On compromise only (no scheduled rotation) | — |
| `MACOS_KEYCHAIN_PASSWORD` | On compromise only | — |

When the cert renews: repeat Section 3 steps 2–11; rotate `MACOS_CERTIFICATE_BASE64`,
`MACOS_CERTIFICATE_PASSWORD`, and `MACOS_SIGNING_IDENTITY`. Old cert can be
revoked at https://developer.apple.com/account → Certificates after the new
cert is in CI and a release has signed successfully.

---

## 7. Recovery procedure

**Scenario A — primary credential holder is unavailable, cert is intact in 1Password:**
1. Backup holder logs into 1Password vault.
2. Retrieves the .p12 + password attachment.
3. Re-uploads to GitHub secrets if rotated; or just downloads for local use.
4. No Apple-side action needed.

**Scenario B — .p12 is lost but Apple Developer account intact:**
1. Log in at https://developer.apple.com/account.
2. Revoke the old Developer ID Application cert.
3. Repeat Section 3 from step 2 (new CSR → new cert → new .p12).
4. Rotate the 3 cert-dependent secrets (`MACOS_CERTIFICATE_BASE64`, `MACOS_CERTIFICATE_PASSWORD`, `MACOS_SIGNING_IDENTITY`).

**Scenario C — Apple Developer account access lost (primary holder gone, 2FA device gone):**
1. Apple Account Recovery: https://iforgot.apple.com — uses recovery email + billing info.
2. Recovery email and billing should both be on file in 1Password vault.
3. If account is unrecoverable: enroll a fresh Apple Developer Program account; new Team ID; users will see a "developer changed" warning on first run of the next release (expected; one-time).

**Scenario D — app-specific password compromised:**
1. Revoke at https://appleid.apple.com → Sign-In and Security → App-Specific Passwords.
2. Generate a new one (same label).
3. Update `MACOS_NOTARY_PASSWORD` in GitHub secrets.
4. No release-pipeline changes needed.

---

## 8. Block / fallback log

If Apple Developer enrollment is **blocked > 1 week** into Wave 1, the
v1.7-PLAN risk register escalates and the project ships unsigned-with-warning
for v1.7 (fall back to the ad-hoc resign step from Phase 8a's predecessor
state — recoverable via `git revert` of the signing commits).

| Date | Event | Action |
|---|---|---|
| 2026-05-17 | 8a-01 runbook drafted; enrollment pending | None — waiting on human-driven enrollment |
| 2026-05-21 | Individual enrollment confirmed; Developer ID Application cert issued (`469F50AE…`, valid → 2031-05-22); all 7 `MACOS_*` secrets uploaded to `BALL-Project/ball` | Local notarized build produced |
| 2026-05-21 | **First notarization Accepted** — submission `082e2f2c-ef0a-47e7-b506-37cb1130c549`, processed in minutes (not the feared 8–12h). Bundle ID `de.uni-tuebingen.ball.ballview` is now primed. `spctl --assess` → `source=Notarized Developer ID`. | Priming complete; CI steady-state. Next tagged release should sign+notarize+staple automatically. |

---

## 9. Verification checklist (for the credential holder after upload)

- [ ] All 7 `MACOS_*` secrets present in `gh secret list --repo BALL-Project/ball`
- [ ] 1Password vault `BALL — macOS signing` has ≥2 humans with Read/Write access
- [ ] Local .p12 + base64 file securely deleted (`rm -P`)
- [ ] Throwaway-tag release (`vDEV-08a-signing-smoke`) green end-to-end
- [ ] `spctl --assess` on downloaded .dmg reports `source=Notarized Developer ID`
- [ ] This runbook's Section 1 (enrollment shape) + Section 4 (vault access table) filled in with real names
