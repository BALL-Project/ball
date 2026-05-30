# Windows Code Signing — SignPath Foundation Secrets Runbook

**Phase:** 999.68-packaging-signing-license
**Requirement:** WINSIGN-01
**Owner:** TBD (SignPath Foundation account holder — the maintainer)
**Co-owner:** TBD (bus-factor backup)
**Last reviewed:** 2026-05-30
**Status:** DRAFT — **PENDING** SignPath Foundation OSS enrollment

This document is the step-by-step setup + recovery path for the SignPath
Foundation credentials that drive the Windows installer-signing steps in
`.github/workflows/release.yml` (the `Sign installer via SignPath Foundation`
step and its gated siblings). It mirrors the macOS signing runbook
(`.planning/archive/phases-thru-v1.7.4/08a-packaging-macos/08a-SECRETS-RUNBOOK.md`).

**Until enrollment completes and the secrets below are set, the release ships
the UNSIGNED NSIS installer.** The CI scaffold is OPTIONAL-skip: when
`SIGNPATH_API_TOKEN` is absent, the release emits a `::warning::` and ships
unsigned (it does **not** fail). Windows SmartScreen warns users on first run
— see `packaging/windows/README-SMARTSCREEN.md`.

---

## 0. Why SignPath Foundation (vs. a paid EV/OV cert)

SignPath Foundation runs a **free code-signing program for open-source
projects**. It provides an OV (or EV) Authenticode certificate and a
cloud-signing service whose private key never leaves SignPath's HSM — the CI
job submits the unsigned installer and receives a signed one back. This is
why the pipeline uses `signpath/github-action-submit-signing-request` rather
than a local `signtool.exe` + a self-held `.pfx`: **BALL never holds the
signing key.** That is the whole security model and the reason the foundation
requires a manual approval click per signing request.

Trade-off vs. macOS: unlike the Apple Developer ID flow (where we hold a
`.p12`), SignPath holds the key. There is therefore **no private-key backup
for us to manage** — recovery is account-recovery + re-issuing an API token,
not key re-export.

---

## 1. Enrollment — apply to SignPath Foundation (one-time, days–weeks)

SignPath Foundation reviews each project for OSS eligibility. Budget
**days to a few weeks** for approval.

1. Go to https://signpath.org/ (the Foundation, *not* the commercial
   signpath.io tiers) → **Apply for the open-source program**.
2. Provide:
   - Project name: **BALL / BALLView**
   - Public repository: `https://github.com/BALL-Project/ball`
   - OSI-approved license: **LGPL-2.1** (BALL's own license — see
     `LICENSE-REVIEW.md`; the macOS binary is GPL, also OSI-approved)
   - A short description (molecular modelling library + visualization GUI)
   - The maintainer as the responsible account holder
3. Wait for the Foundation to approve and provision a SignPath
   **organization** for the project. Record the **organization ID** (a GUID)
   — it becomes the `SIGNPATH_ORG_ID` secret.

**Block log:** if enrollment is blocked > 4 weeks, the project continues to
ship the unsigned installer (the scaffold's default path) and the
SmartScreen guidance stays in effect. No release is blocked by the absence of
signing.

---

## 2. Create the project, artifact configuration, and signing policy

Once the SignPath organization exists, in the SignPath web console:

1. **Create a project** with slug **`ballview`**.
   - This MUST match `project-slug: 'ballview'` in release.yml. If you choose
     a different slug, update the workflow `with:` input to match.
2. **Create an artifact configuration** with slug **`installer`**.
   - This MUST match `artifact-configuration-slug: 'installer'`.
   - Configure it for an **Authenticode** (`.exe`) artifact — the BALL NSIS
     installer is a single signed `.exe`.
3. **Create a signing policy** with slug **`release-signing`**.
   - This MUST match `signing-policy-slug: 'release-signing'`.
   - Keep the **manual approval** requirement ON (the Foundation's
     anti-supply-chain control — a human clicks "approve" per request). The
     workflow already sets `wait-for-completion: true`, so the CI job blocks
     until you approve in the SignPath UI.

> The three slugs (`ballview`, `installer`, `release-signing`) are hard-coded
> in release.yml. Either create them with exactly these slugs, or change the
> workflow inputs — they must agree.

---

## 3. Generate the API token

1. In the SignPath console → **your user → API tokens** (or the CI-user /
   service-account if the Foundation provisions one) → **Create token**.
2. Scope it to the `ballview` project + `release-signing` policy if scoping
   is offered (least privilege).
3. Copy the token **once** — SignPath shows it only at creation. This is the
   value for the `SIGNPATH_API_TOKEN` secret.

---

## 4. The GitHub Actions secrets

Add these in **`BALL-Project/ball` → Settings → Secrets and variables →
Actions → Repository secrets**:

| Secret name | What it is | Where to get it |
|---|---|---|
| `SIGNPATH_API_TOKEN` | SignPath API token for the CI signing request | §3 — created in the SignPath console; shown once |
| `SIGNPATH_ORG_ID` | SignPath **organization ID** (GUID) | §1 — provisioned by the Foundation; visible in the console org settings / URL |

The project / artifact-config / signing-policy slugs are **not** secrets —
they are public-safe identifiers hard-coded in release.yml (`ballview`,
`installer`, `release-signing`). Only the token and org ID are secret.

> If you scope the token per-project, `SIGNPATH_API_TOKEN` alone is enough for
> the action; `SIGNPATH_ORG_ID` is still required because the action takes
> `organization-id` explicitly.

### Verification

After adding both secrets:

```sh
gh secret list --repo BALL-Project/ball \
  | grep -E '^SIGNPATH_(API_TOKEN|ORG_ID)' \
  | wc -l
# Expected: 2
```

---

## 5. How the pipeline uses the secrets (what to expect after they land)

In `.github/workflows/release.yml`, AFTER the NSIS installer is built and the
**unsigned** installer is uploaded as a workflow artifact:

1. **`SignPath not configured — ship unsigned`** — runs only when the secret
   is ABSENT; emits a `::warning::` and ships unsigned. Once the secrets are
   set, this step is skipped.
2. **`Sign installer via SignPath Foundation`** — runs only when
   `secrets.SIGNPATH_API_TOKEN != ''`. Submits the unsigned installer's
   `github-artifact-id` to SignPath, then **blocks** (`wait-for-completion:
   true`) until an admin clicks **approve** in the SignPath UI. Downloads the
   signed `.exe` into `signed-installer/`.
3. **`Recompute SHA256 + re-upload signed installer`** — replaces the
   unsigned installer with the signed one and re-emits the `.sha256` sidecar
   (the signed binary has a different hash — Authenticode embeds the
   signature block).
4. **`Re-upload signed installer artifact`** — re-uploads under the same
   artifact name with `overwrite: true` so the release job picks up the
   **signed** installer.

**First signed release:** watch the SignPath UI — the job will sit at the
sign step waiting for your manual approval. Approve it; the job then
completes and the release attaches a signed installer.

---

## 6. Recovery procedures

**Scenario A — API token compromised or expired:**
1. Revoke the old token in the SignPath console.
2. Create a new token (§3).
3. Update `SIGNPATH_API_TOKEN` in GitHub secrets.
4. No workflow change needed.

**Scenario B — account holder unavailable (bus-factor):**
1. The co-owner logs into the SignPath organization (ensure ≥2 humans have
   SignPath console access — see §7).
2. Generates a fresh API token and updates the secret.
3. The signing cert itself is held by SignPath; no key recovery on our side.

**Scenario C — SignPath organization access lost entirely:**
1. Contact SignPath Foundation support to recover the organization.
2. If unrecoverable, re-apply (§1) — a new org ID + token; update both
   secrets. Releases ship unsigned in the interim (the scaffold's default).

**Scenario D — slug mismatch / "project not found" at sign time:**
1. Confirm the SignPath project/artifact-config/policy slugs are exactly
   `ballview` / `installer` / `release-signing` (§2), OR update the
   `with:` inputs in release.yml to match what you created.

---

## 7. Access + bus-factor

| Person | Role | SignPath console access |
|---|---|---|
| _TBD_ | Primary account holder | Admin |
| _TBD_ | Bus-factor backup | Admin |

**Bus-factor rule:** at least 2 humans MUST have admin access to the SignPath
organization at all times, so a single departure cannot strand signing. Add a
successor BEFORE removing a departing admin.

---

## 8. Block / status log

| Date | Event | Action |
|---|---|---|
| 2026-05-30 | SignPath CI scaffold added to release.yml (OPTIONAL-skip, gated on `SIGNPATH_API_TOKEN`); this runbook drafted | None — waiting on maintainer SignPath Foundation enrollment (WINSIGN-01) |

---

## 9. Verification checklist (for the account holder after setup)

- [ ] SignPath Foundation OSS application approved; organization provisioned
- [ ] Project `ballview` + artifact config `installer` + signing policy
      `release-signing` created with those exact slugs
- [ ] Signing policy has **manual approval** enabled
- [ ] `SIGNPATH_API_TOKEN` + `SIGNPATH_ORG_ID` present in
      `gh secret list --repo BALL-Project/ball` (expected: 2)
- [ ] ≥2 humans have admin access to the SignPath organization (§7)
- [ ] A throwaway-tag release signs end-to-end (job blocks at the sign step,
      you approve in the SignPath UI, signed `.exe` is attached)
- [ ] `signtool verify /pa BALLView-*-installer.exe` (or right-click →
      Properties → Digital Signatures) shows a valid SignPath signature
- [ ] Update `packaging/windows/README-SMARTSCREEN.md` to note signing is now
      live (SmartScreen reputation builds over subsequent signed releases)

---

## See also

- `.github/workflows/release.yml` — the SignPath signing steps (gated on `SIGNPATH_API_TOKEN`)
- `packaging/windows/README-SMARTSCREEN.md` — user-facing unsigned-installer guidance
- `LICENSE-REVIEW.md` — license posture (OSS eligibility: LGPL-2.1 / GPL macOS)
- `.planning/archive/phases-thru-v1.7.4/08a-packaging-macos/08a-SECRETS-RUNBOOK.md` — the macOS signing runbook this mirrors
