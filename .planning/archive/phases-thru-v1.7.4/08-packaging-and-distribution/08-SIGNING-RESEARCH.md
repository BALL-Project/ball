---
phase: 08-packaging-and-distribution
type: research
status: dormant (research only; Phase 8 not yet active)
researched: 2026-05-15
parent_phase: 08
---

# Phase 8: Code Signing & Notarization — Research

## Context

`v1.6.0` ships as **unsigned installers** ([`release.yml`](../../../.github/workflows/release.yml), commit `a186fb5`) — `release.yml` is intentionally best-effort because no signing infrastructure exists yet:

- macOS users see Gatekeeper warnings ("BALLView.app can't be opened because Apple cannot check it for malicious software") and must right-click → Open or run `xattr -dr com.apple.quarantine BALLView.app`
- Windows users see SmartScreen warnings ("Windows protected your PC") and must click "More info → Run anyway"
- Both are friction barriers for non-technical users; both block enterprise adoption

This document captures the **current-state research (2026-05-15)** on what it takes to fix this properly. Phase 8 ("Packaging & Distribution") owns the implementation. This is the prep work.

## BALL license note (informs signing-provider eligibility)

BALL is **LGPL v2.1** — OSI-approved, no commercial dual-licensing apparent. This makes the project eligible for free open-source code-signing programs (notably **SignPath Foundation**, see Windows section).

---

## macOS — code signing + notarization

### Requirements (non-negotiable)

- **Apple Developer Program membership: $99/year** — required to obtain the Developer ID Application certificate. No free path for distributing notarized macOS apps outside the App Store. (Distinct cert types: "Mac App Store" cert is for the store only; "Developer ID Application" is for direct distribution; "Developer ID Installer" is for `.pkg` installers — we want the first.)
- **Notarization required since June 2019** for binaries distributed outside the App Store on macOS 10.15+. Apple's notary service scans for malware + validates code-signing chain; on success it returns a "ticket" stapled into the bundle so Gatekeeper trusts it offline.
- **Hardened runtime** (`codesign --options runtime`) is required for notarization. This restricts dylib loading, debugger attachment, and other privileges; BALL's runtime behaviour needs to be tested under hardened runtime before notarization will succeed.

### The signing flow

```bash
# 1. Sign all embedded Qt frameworks via macdeployqt's built-in flag.
#    macdeployqt already runs in release.yml; just add the identity.
macdeployqt BALLView.app \
  -codesign="Developer ID Application: <Org Name> (<TEAMID>)" \
  -verbose=1

# 2. Sign the .app bundle itself with hardened runtime.
codesign --deep --force --options runtime \
  --sign "Developer ID Application: <Org Name> (<TEAMID>)" \
  --entitlements BALLView.entitlements \
  BALLView.app

# 3. Verify signature locally.
codesign --verify --deep --strict --verbose=2 BALLView.app
spctl --assess --type execute --verbose BALLView.app

# 4. Zip for submission (notary service wants a zip / dmg / pkg).
ditto -c -k --sequesterRsrc --keepParent BALLView.app BALLView.zip

# 5. Submit to Apple notary service and wait (~5-15 min after the first
#    submission; first submission for a new bundle ID can take 8-12h).
xcrun notarytool submit BALLView.zip \
  --apple-id "<email>" \
  --team-id "<TEAMID>" \
  --password "<app-specific-password>" \
  --wait

# 6. On success, staple the notarization ticket into the bundle so it
#    works offline (without a round-trip to Apple's servers).
xcrun stapler staple BALLView.app

# 7. Re-zip the now-stapled app for distribution.
ditto -c -k --sequesterRsrc --keepParent BALLView.app BALLView-v1.6.0-macos-arm64.zip
```

### Entitlements (`BALLView.entitlements`)

Likely-needed entitlements for BALLView under hardened runtime:

- `com.apple.security.cs.allow-jit` — Python embedding (when re-enabled in Phase 6)
- `com.apple.security.cs.allow-unsigned-executable-memory` — if any plugin path eval is dynamic
- `com.apple.security.cs.disable-library-validation` — Qt's plugin loading mechanism (Qt platforms, image formats, multimedia) loads dylibs at runtime that may not all be signed with the same Team ID

**Verify which are actually needed by running BALLView under hardened runtime locally before CI submission** — surplus entitlements weaken the security posture and look bad in App Review (if BALL ever decides to add Mac App Store distribution).

### GitHub Actions integration

Secrets to add to `BALL-Project/ball` repo settings:

- `MACOS_CERTIFICATE_BASE64` — `Developer ID Application` cert exported as `.p12`, base64-encoded
- `MACOS_CERTIFICATE_PASSWORD` — the `.p12` export password
- `MACOS_KEYCHAIN_PASSWORD` — ephemeral password for the CI-only keychain
- `MACOS_APPLE_ID` — Apple Developer account email
- `MACOS_TEAM_ID` — 10-character team identifier
- `MACOS_NOTARY_PASSWORD` — app-specific password generated at https://appleid.apple.com (NOT the Apple ID account password)
- `MACOS_SIGNING_IDENTITY` — string like `"Developer ID Application: <Org> (<TEAMID>)"`

Workflow step before `macdeployqt` (replace the current `release.yml` macOS path):

```yaml
- name: Install Apple cert into CI keychain
  if: matrix.os == 'macos-arm64'
  run: |
    echo "${{ secrets.MACOS_CERTIFICATE_BASE64 }}" | base64 --decode > /tmp/cert.p12
    security create-keychain -p "${{ secrets.MACOS_KEYCHAIN_PASSWORD }}" build.keychain
    security default-keychain -s build.keychain
    security unlock-keychain -p "${{ secrets.MACOS_KEYCHAIN_PASSWORD }}" build.keychain
    security import /tmp/cert.p12 -P "${{ secrets.MACOS_CERTIFICATE_PASSWORD }}" \
      -T /usr/bin/codesign -T /usr/bin/security
    security set-key-partition-list -S apple-tool:,apple: \
      -s -k "${{ secrets.MACOS_KEYCHAIN_PASSWORD }}" build.keychain
    rm /tmp/cert.p12
```

### Cost

- **Apple Developer Program: $99/year (recurring)** — non-negotiable for distributing notarized macOS apps outside the App Store
- **Notarization itself: $0** — free service for Apple Developer Program members
- **CI compute: ~5-15 min added per release** (notarization wait, depending on first-time-for-bundle-ID vs subsequent)

### Watch-outs

- **`com.apple.LaunchServices.OpenWith` / quarantine attribute drift** — first launch after notarization should remove the warning; if it persists, run `xattr -dr com.apple.quarantine` to clear cached quarantine state from before the staple
- **First submission for a new bundle ID takes 8-12 hours** — start the notarization pipeline well before the planned release date
- **Universal binaries** — BALL currently builds macOS arm64 only; if we add Intel x86_64, both slices need the same signing identity
- **Sparkle / auto-update** — out of scope for v1.6, but if Phase 8 adds auto-update, the appcast feed must serve notarized DMGs / zips

---

## Windows — code signing

### What changed recently (and why it's harder than it used to be)

Three industry shifts in the last ~3 years:

1. **June 2023 — CA/Browser Forum: all OV code-signing certs must be on hardware.** Plain `.pfx` files in CI secrets are no longer issued. New OV certs ship on USB tokens OR cloud HSM. EV certs were already on hardware.

2. **2024 — Windows SmartScreen change: EV no longer bypasses on first download.** Pre-2024, EV-signed binaries went straight through SmartScreen with no warning ever. Post-2024, EV-signed binaries go through the same reputation-building process as OV (downloads accumulate; eventually SmartScreen stops warning). EV still helps for enterprise trust signals and is required for Windows drivers, but it's no longer a SmartScreen-skip card.

3. **March 2026 — CA/B Forum: max code-signing cert validity capped at 458 days** (was 3 years). Rolling renewal is now a higher-frequency operational concern. Already enforced by SSL.com as of Feb 2026.

Net effect: the cheapest "Windows signing" path no longer exists. Every option below involves either ongoing money or surrendering signing to a third-party service.

### Decision (2026-05-15)

**Windows signing path LOCKED to Path A — SignPath Foundation (free OSS program).** BALL's LGPL-2.1 license, GitHub-public-CI, and academic OSS posture fit the SignPath Foundation eligibility criteria; the foundation underwrites the cert + HSM at no cost. The manual approval gate per release is acceptable (and arguably desirable as an anti-supply-chain-attack control on an academic project). Paths B (Azure Artifact Signing) and C (commercial EV/OV) preserved below as documented escalation paths if SignPath rejects the application or if fully-automated nightly releases become a hard requirement later.

### Three viable paths for BALL

#### Path A — SignPath Foundation (FREE for OSS) — **CHOSEN 2026-05-15**

- Free for OSI-approved licenses without commercial dual-licensing — **BALL's LGPL-2.1 qualifies**
- HSM-backed certificate (FIPS 140-2 Level 3), managed by the foundation
- Manual approval gate per release — release maintainer clicks "approve" in the SignPath UI after the CI build, foundation signs and returns the signed binary
- GitHub Actions integration via `SignPath/GitHubActionTestRepo` action / `signpath/github-action-submit-signing-request`
- Used in production by: DB Browser for SQLite, Super Productivity, KeePass, others
- Trade-off: manual approval breaks fully-automated release (intentional — anti-supply-chain-attack control)

**Cost:** $0. **Setup time:** ~2 weeks (foundation approval + GH Actions wiring).

#### Path B — Azure Artifact Signing (formerly Azure Trusted Signing, renamed Jan 2026)

- Microsoft's managed code-signing service. Certs valid for **only 3 days** (sic) — strict timestamping discipline required (use `http://timestamp.acs.microsoft.com/`)
- **Basic plan: ~$10/month** with 5,000 signatures included (enough for 100+ releases/month)
- GitHub Actions integration: [`Azure/artifact-signing-action`](https://github.com/Azure/artifact-signing-action) — runs on Windows runners, authenticates via OpenID Connect (no long-lived secrets)
- Cleanly integrated with Azure RBAC if BALL already has an Azure subscription; account creation is trivial otherwise

**Cost:** ~$120/year + Azure subscription overhead. **Setup time:** ~3-5 days (Azure resource setup, OIDC trust, workflow integration).

#### Path C — Commercial EV/OV certificate (Sectigo, DigiCert, SSL.com, GlobalSign)

- OV: $200-300/year. EV: $300-500/year. Both now require cloud HSM (USB tokens still exist but break CI). Cloud HSM signing ~$0-5/signature depending on vendor (SSL.com eSigner, DigiCert KeyLocker, GlobalSign HSM).
- Direct control over the cert; no third-party approval gate
- EV doesn't bypass SmartScreen anymore (post-2024), so the EV premium mostly buys enterprise-trust signal and driver-signing capability
- BALL doesn't ship Windows drivers — EV is over-spec; OV + cloud HSM is the cost-rational choice if going commercial

**Cost:** $200-500/year + per-signature fees. **Setup time:** ~1-2 weeks (cert vetting, HSM provisioning, workflow integration).

### Fallback paths (not chosen — preserved for the record)

If the SignPath Foundation application is rejected, or if a future requirement makes the manual approval gate unworkable, the fallback order is:

1. **Path B (Azure Artifact Signing, ~$120/yr)** — fully automated, OIDC-authenticated. Pick this first if SignPath falls through.
2. **Path C (commercial OV cert + cloud HSM, $200-500/yr)** — only if both A and B are unavailable, or if a specific enterprise distribution requirement demands direct cert ownership.

Avoid the commercial EV path entirely — BALL ships no Windows drivers and the EV premium no longer buys SmartScreen-bypass (post-2024).

### The signing flow (any path)

```powershell
# Common pattern: sign .exe + all bundled .dll files + the final installer
signtool sign /tr http://timestamp.digicert.com /fd sha256 /td sha256 `
  /sha1 <CERT_THUMBPRINT> `
  /n "<Subject Name>" `
  BALLView.exe

# Or via Azure Artifact Signing's action wrapper
- uses: Azure/artifact-signing-action@v1
  with:
    endpoint: <region>.codesigning.azure.net
    trusted-signing-account-name: <account>
    certificate-profile-name: <profile>
    files-folder: ${{ github.workspace }}\dist
    files-folder-filter: exe,dll,msi
```

### Watch-outs

- **Timestamping is mandatory** — without a timestamp signature, the binary stops being trusted the moment the cert expires (458 days max post-March 2026). Always pass `/tr` (or the Azure equivalent).
- **Reputation-building lag** — even with a perfect signature, brand-new signed binaries hit SmartScreen warnings until enough users download them. Plan for "Run anyway" friction on the first few v1.6.x releases regardless of which path.
- **Signing ALL embedded DLLs**, not just `BALLView.exe` — `windeployqt` pulls in Qt + Boost + others. signtool/Azure-Action need to walk the deploy directory and sign every binary, otherwise users see warnings on Qt's plugin DLLs loading at runtime.
- **MSI vs zip** — current `release.yml` ships a zip. For a proper signed installer, Phase 8 also wants WiX or NSIS to wrap into an `.msi` / setup `.exe`. The installer itself also gets signed.

---

## Cross-platform: what Phase 8 needs to deliver

When Phase 8 is promoted from research to active, the plan should cover:

1. **Apple Developer Program enrollment** ($99/year, one-time setup + annual renewal)
2. **macOS signing + notarization wiring in `release.yml`** (~80 lines added to the macOS job)
3. **macOS entitlements file authored + tested under hardened runtime** locally before CI submission
4. **Windows signing provider — DECIDED: SignPath Foundation (Path A)**. Submit the foundation application early in the phase; foundation reviews take real time. If rejected, fall back to Path B (Azure Artifact Signing).
5. **Windows signing wiring in `release.yml`** (~30 lines added to the Windows job + the SignPath manual-approval gate). The SignPath GitHub Action ([`signpath/github-action-submit-signing-request`](https://github.com/SignPath/github-action-submit-signing-request)) submits the build artifact to the foundation, pauses for human approval in the SignPath UI, then downloads the signed artifact back into the workflow for attachment to the GitHub Release.
6. **Installer format decision: keep zips, or upgrade to DMG (macOS) + MSI/NSIS (Windows)** (separate scope from signing, but typically bundled in Phase 8)
7. **Verification: run `spctl` (macOS) and `signtool verify` (Windows) on the released artifacts to confirm chain-of-trust integrity**
8. **Documentation update: `BUILD-macos.md` etc. mention how to skip signing for local dev builds (no `--options runtime`, etc.)**

## Open questions for Phase 8 to resolve

1. **Who owns the Apple Developer Program account?** Project-level (TÜBINGEN / BALL maintainers as the legal entity) vs. an individual maintainer. Legal entity is the right answer if BALL has institutional sponsorship; individual is the fallback. The cert's "Org" string is publicly visible in the signed bundle's metadata.
2. **SignPath Foundation application — when to submit.** Decision to use SignPath is LOCKED (2026-05-15); open question is the *timing* of submission. Foundation reviews the project's licensing, governance, and supply-chain posture and takes ~weeks to clear. Optimal timing: submit at the start of Phase 8 so the approval window overlaps with the macOS notarization wiring work (parallelism). If rejected, fall back to Path B (Azure Artifact Signing) without losing wall-clock time on the phase.
3. **MSI vs zip on Windows** — does Phase 8 do MSI/NSIS at the same time, or split into 8.1 (signing) + 8.2 (installer formats)?
4. **Auto-update path** — out of scope for Phase 8 v1.6? Sparkle (macOS) + WinSparkle (Windows) typically come after first signed release.
5. **Cert renewal cadence post-March-2026** — 458-day cap means renewal every ~15 months. Document the renewal runbook so it doesn't surprise the project at expiry.
6. **Reproducibility** — the SignPath Foundation review may ask for a verifiable build pipeline (deterministic builds, SBOM). Worth tracking even if not required at first.

## References

### macOS
- [Apple — Notarizing macOS software before distribution](https://developer.apple.com/documentation/security/notarizing-macos-software-before-distribution)
- [Federico Terzi — Automatic Code-signing and Notarization for macOS apps using GitHub Actions](https://federicoterzi.com/blog/automatic-code-signing-and-notarization-for-macos-apps-using-github-actions/)
- [rsms — macOS distribution: code signing, notarization, quarantine](https://gist.github.com/rsms/929c9c2fec231f0cf843a1a746a416f5)
- [Apple Developer Forums — Notarization topic](https://developer.apple.com/forums/topics/code-signing-topic/code-signing-topic-notarization)
- [ddev/signing_tools — Signing and Notarization tools for macOS and Windows (production-tested)](https://github.com/ddev/signing_tools)

### Windows
- [Microsoft Learn — Code signing options for Windows app developers](https://learn.microsoft.com/en-us/windows/apps/package-and-deploy/code-signing-options)
- [Melatonin — How to code sign Windows installers with an EV cert on GitHub Actions](https://melatonin.dev/blog/how-to-code-sign-windows-installers-with-an-ev-cert-on-github-actions/)
- [Melatonin — Code signing on Windows with Azure Artifact Signing](https://melatonin.dev/blog/code-signing-on-windows-with-azure-trusted-signing/)
- [Microsoft Tech Community — Simplifying signing integration for Trusted Signing](https://techcommunity.microsoft.com/blog/microsoft-security-blog/simplifying-signing-integration-for-trusted-signing/4293292)
- [Scott Hanselman — Automatically Signing a Windows EXE with Azure Trusted Signing](https://www.hanselman.com/blog/automatically-signing-a-windows-exe-with-azure-trusted-signing-dotnet-sign-and-github-actions)
- [SSL.com — Code Signing Certificates, Cloud Signing Options and Signing Operations Integration](https://www.ssl.com/guide/code-signing-certificates-cloud-signing-options-and-signing-operations-integration/)
- [SSL Insights — OV vs EV Code Signing Certificate Guide](https://sslinsights.com/best-code-signing-certificate-windows-applications/)
- [SignPath Foundation — terms for open-source projects](https://signpath.org/terms.html)
- [SignPath Knowledge Base — Code Signing Introduction](https://signpath.io/knowledge-base/introduction)
- [DB Browser for SQLite — Signing Windows executables with SignPath](https://sqlitebrowser.org/blog/signing-windows-executables-our-journey-with-signpath/)
- [forelens.com — GitHub Actions Code Signing with Azure Key Vault HSM, RBAC, OIDC](https://forelens.com/blog/github-actions-code-signing-with-azure-key-vault-hsm-rbac-oidc-and-managed-identity/)

### Industry context
- [QCE — Best Code Signing Platforms 2026 (industry comparison)](https://www.qcecuring.com/blog/best-code-signing-platforms-2026)
- [Azure Artifact Signing product page (renamed from Trusted Signing Jan 2026)](https://azure.microsoft.com/en-us/products/artifact-signing)
