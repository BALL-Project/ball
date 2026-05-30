# Phase 8b — Windows Signing Secrets + Bus-Factor Runbook

> Companion to `SIGNPATH-APPLICATION-RUNBOOK.md`. This document covers
> what secrets the release.yml workflow expects, where they live, and the
> 2-human recovery procedure required by the v1.7-PLAN.md risk register
> P0 entry ("Bus factor 1 on signing infrastructure").

## 1. Required GitHub Actions repo secrets

The Windows signing step in `.github/workflows/release.yml` is **gated** on
the presence of the SignPath API token. When the secret is absent, the
signing step skips silently and the workflow falls back to publishing the
unsigned installer (see `packaging/windows/README-SMARTSCREEN.md`).

| Secret name | Purpose | Owner | Value source |
|---|---|---|---|
| `SIGNPATH_API_TOKEN` | API token for the SignPath GitHub Action; authorises submitting an unsigned artifact for signing | Primary SignPath admin | Generated in SignPath UI after foundation approval |
| `SIGNPATH_ORG_ID` | The SignPath organisation slug (constant per project; foundation-issued) | Primary SignPath admin | Provided by foundation on approval |

**Configuration**: GitHub repo → Settings → Secrets and variables → Actions →
New repository secret. Both values are foundation-approval-gated; they
cannot be set until SignPath approves the project.

### Fallback-path secret names (Path B / Path C)

If we escalate to Path B (Azure Code Signing) or Path C (commercial OV + HSM),
the secret names change:

| Path | Secret names | Action used |
|---|---|---|
| Path B — Azure Code Signing | `AZURE_TENANT_ID`, `AZURE_CLIENT_ID` (OIDC federated; no client-secret) | `Azure/trusted-signing-action@v0` |
| Path C — SSL.com eSigner | `ESIGNER_USERNAME`, `ESIGNER_PASSWORD`, `ESIGNER_TOTP_SECRET`, `ESIGNER_CREDENTIAL_ID` | `ssl-com/esigner-codesign@v1` |
| Path C — DigiCert KeyLocker | `KEYLOCKER_API_KEY`, `KEYLOCKER_CLIENT_CERT`, `KEYLOCKER_HOST` | `digicert/keylocker-codesign@v1` |

When a path-switch happens, update both this runbook and the gated `if:`
predicate in `release.yml` to check the new secret name.

## 2. Where the credentials live (1Password vault)

**Primary vault**: `BALL/Windows-Signing/` in the project's 1Password account
(or org-equivalent secret manager — Bitwarden / KeePass / etc.; replace
"1Password" throughout this doc with the chosen tool).

**Stored items**:

| Item | Contents |
|---|---|
| `SignPath API Token` | The `SIGNPATH_API_TOKEN` value mirrored into GitHub repo secrets |
| `SignPath Org ID` | The `SIGNPATH_ORG_ID` value mirrored into GitHub repo secrets |
| `SignPath Foundation Account` | Login email + password for the foundation portal (BOTH admins have access) |
| `SignPath 2FA Backup Codes` | One-time recovery codes generated at 2FA enrollment |
| `Project Slug + Policy Slug` | `ballview` + `release-signing` (constants once provisioned; record for documentation) |
| `GitHub Repo Secrets Screenshot` | Screenshot of repo Secrets page for cross-verification (no values shown, just names) |

**Why mirror to 1Password**: GitHub repo secrets are write-only (cannot be
read back via the UI or API once set). If the primary admin's machine dies
and the GitHub-side secret is lost, the 1Password copy is the only recovery
path short of rotating the SignPath token + re-issuing.

## 3. Roles table — bus-factor commitment

**Both rows MUST have ≥2 humans filled before v1.7 RC tag.** Single-admin
state is a P0 violation per v1.7-PLAN.md risk register.

| Role | Primary | Secondary | Confirmed |
|---|---|---|---|
| SignPath Foundation admin | TBD — name before v1.7 RC tag | TBD — name before v1.7 RC tag | — |
| GitHub repo admin (manages `SIGNPATH_API_TOKEN` secret) | TBD — name before v1.7 RC tag | TBD — name before v1.7 RC tag | — |
| 1Password vault owner (`BALL/Windows-Signing/`) | TBD — name before v1.7 RC tag | TBD — name before v1.7 RC tag | — |

If only one human is available for any row at v1.7 RC time, this deliverable
is **INCOMPLETE** — file a P0 issue blocking the RC tag.

## 4. Recovery procedure (if primary admin unavailable)

The secondary admin can fully recover release-signing capability without
the primary admin present:

1. **Log into 1Password** with the secondary admin's credentials → vault `BALL/Windows-Signing/`.
2. **Log into the SignPath Foundation portal** with the stored account credentials + 2FA backup code if needed.
3. **Approve any pending release signing requests** (the SignPath UI shows pending requests; the workflow blocks until one of them is clicked through).
4. **Rotate `SIGNPATH_API_TOKEN`** if compromised:
   - Generate a new token in the SignPath UI.
   - Update the GitHub repo secret (Settings → Secrets and variables → Actions → Edit `SIGNPATH_API_TOKEN`).
   - Update the 1Password item.
   - Old token auto-expires when the new one supersedes it (or revoke explicitly in SignPath UI).
5. **Add additional admins to the SignPath organisation** if the primary is permanently unavailable: SignPath UI → Members → Invite.
6. **Document the recovery event** in `.planning/phases/08b-packaging-windows/08b-SIGNPATH-STATUS.md` (append a row to the weekly status table).

## 5. Renewal cadence

- **SignPath Foundation membership**: auto-renews; verify annually that the project still qualifies (LGPL → no commercial dual-licensing).
- **API token**: no expiry by default. Best practice — rotate annually.
- **CA/B Forum cert validity** (post-March 2026): max 458 days. If on Path B or Path C, calendar reminder at 12 months for cert renewal.
- **Annual review** (whichever path active):
  - Verify both admins still active.
  - Verify both admins can still log into 1Password vault.
  - Verify the recovery procedure still works (dry-run: secondary admin opens SignPath UI, no need to actually sign anything).
  - **Reminder filed for: 12 months from Phase 8b kickoff** (calendar entry required).

## 6. What the release.yml workflow does with these secrets

When `SIGNPATH_API_TOKEN` is present in repo secrets, the Windows job in
`release.yml` runs the SignPath GitHub Action:

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

The step uploads the unsigned `.exe` to SignPath, then **blocks** until an
admin (primary or secondary) clicks "approve" in the SignPath UI — this
is the intentional manual-approval gate (anti-supply-chain-attack control;
foundation policy + project policy both require it).

After approval, SignPath returns the signed `.exe`; the workflow uploads
it as the release asset. The `if: env.SIGNPATH_API_TOKEN != ''` predicate
is what makes this step optional — when the secret is absent, the workflow
falls through to the unsigned-fallback path, no error.

## 7. Signed-vs-unsigned switch — operational summary

| State | Secret present? | Workflow behaviour | User-facing artifact |
|---|---|---|---|
| Pre-foundation-approval | No | Signing step skipped; unsigned installer uploaded | `BALLView-*-windows-x64-installer.exe` (unsigned + README-SMARTSCREEN.md) |
| Post-foundation-approval | Yes | Signing step uploads to SignPath; admin approves; signed installer uploaded | `BALLView-*-windows-x64-installer.exe` (signed) |
| Path-switch to Azure / Commercial | Different secret name | Different signing GitHub Action; same gating pattern | Signed installer |

The workflow shape is **identical** in all three states; only the secret
name (and the GitHub Action it gates) differs. No release-yml refactor
is needed to switch between unsigned, SignPath-signed, Azure-signed, or
commercial-signed paths.

## 8. SHA256 verification (always-on, regardless of signing state)

Even with no signature, every release publishes the SHA256 of the installer
in the GitHub Release body. End users can verify they downloaded the genuine
binary by comparing the SHA256:

```powershell
(Get-FileHash BALLView-v1.7-windows-x64-installer.exe -Algorithm SHA256).Hash
```

This is documented in `packaging/windows/README-SMARTSCREEN.md` for end
users; it gives an integrity check that's independent of Authenticode
signing.

---

**Status:** scaffolding complete; values populate when SignPath approves
and secrets are configured (see SIGNPATH-APPLICATION-RUNBOOK.md §8).
