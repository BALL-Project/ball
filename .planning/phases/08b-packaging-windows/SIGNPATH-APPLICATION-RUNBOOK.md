# SignPath Foundation OSS Application — Runbook

> **AGENT-DRAFTED — REQUIRES HUMAN SUBMISSION**
>
> This document was drafted by the Phase 8b execution agent. Agents cannot
> submit the SignPath Foundation application from inside the sandbox
> (`https://about.signpath.io/foundation` requires interactive human
> authentication and human acceptance of the Foundation terms). The runbook
> below is intended for the named primary admin to walk through manually,
> ideally within 24h of Phase 8b kickoff so the multi-week foundation
> review clock starts ASAP.

## TL;DR

Submit the BALL / BALLView project to the **SignPath Foundation OSS code-signing
program** so v1.7+ Windows installers can be Authenticode-signed without
purchasing a commercial OV cert. Foundation approval is async (2-6 week typical
SLA); v1.7 ships either way (unsigned-with-SmartScreen-warning is the
documented Plan B — see `packaging/windows/README-SMARTSCREEN.md`).

---

## 1. Application URL

**Primary entry point:** <https://about.signpath.io/foundation>

Click **"Apply for free code signing"** → fills the foundation intake form.

(Reference home: <https://signpath.org/>. Sponsorship is a commercial-tier
feature, NOT required for the Foundation OSS path — the foundation team
reviews directly.)

## 2. Required information for the application form

| Field | Value |
|---|---|
| Project name | `BALL / BALLView` |
| Homepage | `https://ball-project.org` |
| Repository | `https://github.com/BALL-Project/ball` |
| License | LGPL-2.1 (OSI-approved, no commercial dual-licensing — qualifies) |
| License URL | `https://github.com/BALL-Project/ball/blob/v1.7-modernization/COPYRIGHT` |
| Project description (2-3 sentences) | "BALL (Biochemical ALgorithms Library) is a C++ molecular modelling framework with academic origins. BALLView is its OpenGL-based 3D molecular-visualisation GUI application. Licensed LGPL-2.1; developed openly on GitHub with public CI." |
| CI evidence | Link to `.github/workflows/release.yml` at the v1.7-modernization tip — shows the build is reproducible from source via `cmake --preset ci-windows` + vcpkg manifest mode. |
| Maintainer contact | Primary admin email (see §4 below) |
| Notes | "Public-CI build pipeline; reproducible build via `cmake --preset ci-windows` + vcpkg manifest mode. Two named SignPath admins committed for bus-factor mitigation (see project's SECRETS-RUNBOOK.md)." |

## 3. Eligibility (pre-confirmed)

- **License**: LGPL-2.1 → OSI-approved, no commercial dual-licensing → **qualifies**.
- **Public source**: GitHub repo public → **qualifies**.
- **Public build**: GitHub Actions release.yml is public and reproducible → **qualifies**.
- **Governance**: academic origin, multiple contributors → **qualifies**.

Confirmed in `.planning/phases/08-packaging-and-distribution/08-SIGNING-RESEARCH.md`
(Path A SignPath section, decision locked 2026-05-15).

## 4. Named humans (FILL IN BEFORE SUBMITTING)

The foundation expects a single primary admin contact but the **project's own
bus-factor policy** (v1.7-PLAN.md risk register P0) requires a second named
human with recovery access before submission.

| Role | Name | Email | Confirmed by |
|---|---|---|---|
| Primary SignPath admin | TBD — name before v1.7 RC tag | TBD | — |
| Secondary admin (bus-factor) | TBD — name before v1.7 RC tag | TBD | — |

**Do NOT submit the application until both rows are filled.** A single-admin
SignPath account violates our own policy bar and creates a recovery-impossible
state if the primary is unavailable.

## 5. Expected review timeline (foundation SLA)

| Phase | Typical | Worst-case |
|---|---|---|
| Initial acknowledgment | within 1 week | 2 weeks |
| Foundation review + Q&A | 2-3 weeks | 4-6 weeks |
| Approval + org provisioning | days after approval | 1 week after approval |

BALL's posture (academic / public-CI / LGPL / clean governance) is the
**easy-yes shape** — expect closer to 2-3 weeks total. Foundation has no
hard SLA but historically responds.

## 6. Weekly status-check protocol

Once submitted, the **primary admin** is responsible for a weekly check-in
recorded in `.planning/phases/08b-packaging-windows/08b-SIGNPATH-STATUS.md`
(create on submission day with the template below).

```markdown
# SignPath Foundation OSS — Application Status Log

**Submitted:** YYYY-MM-DD
**Application reference:** <from foundation acknowledgment email>
**Primary admin:** <name + email>
**Secondary admin:** <name + email>

## Weekly status

| Week | Date | State | Action |
|---|---|---|---|
| 0 | YYYY-MM-DD | submitted | application filed; awaiting foundation review |
| 1 | YYYY-MM-DD | pending | (status check) |
| 2 | YYYY-MM-DD | (pending / approved / questions) | (typical-case approval window) |
| 4 | YYYY-MM-DD | DECISION-GATE | if not approved: start Path B (Azure Code Signing) provisioning |
| 6 | YYYY-MM-DD | DECISION-GATE | if still no signature: start Path C (commercial OV + HSM) or commit to unsigned-fallback |
```

**Reminder cadence**: weekly until terminal state (approved / rejected /
escalated). Use a calendar reminder + project tracker issue.

## 7. Escalation gates

The Phase 8b plan defines two hard decision-gates if the foundation review
stalls or rejects. These are NOT optional — they're how we keep the v1.7
ship date stable regardless of the foundation's timeline.

### Week 4 — Path B decision gate (Azure Code Signing — ~$120/yr)

**Trigger**: 4 calendar weeks since submission AND no approval AND no
substantive Q&A from foundation.

**Action**:
1. Open the Azure Code Signing account (Azure subscription required — confirm org has one or provision a dedicated subscription).
2. Configure the SCEPman / Trusted Signing service (the renamed product since 2024).
3. Wire OIDC trust between GitHub Actions + Azure (federated identity, no long-lived secrets).
4. Swap the SignPath GitHub Action in `release.yml` for `Azure/trusted-signing-action@v0` (similar gated-on-secret pattern).
5. Update `08b-SIGNPATH-STATUS.md` with the path-switch event.
6. **Do NOT cancel** the SignPath application; let it run in parallel — if SignPath approves later, we can switch back (SignPath is free; Azure is ~$120/yr ongoing).

**Pre-research done**: see `.planning/phases/08-packaging-and-distribution/08-SIGNING-RESEARCH.md` Path B section. The GitHub Action exists; the OIDC setup is documented. Pre-research only — do NOT pay/provision Azure until this gate fires.

### Week 6 — Path C decision gate (commercial OV + cloud HSM — $200-500/yr)

**Trigger**: 6 calendar weeks since submission AND no approval AND Path B blocked (org policy, no Azure subscription, etc.).

**Action**:
1. Purchase commercial OV (Organization Validation) cert from SSL.com (eSigner) or DigiCert (KeyLocker) — both offer cloud-HSM bundles so the private key never leaves the HSM.
2. Provision the eSigner/KeyLocker API credentials.
3. Wire the corresponding GitHub Action (`ssl-com/esigner-codesign` or `digicert/keylocker-codesign`) into `release.yml`.
4. Update `08b-SIGNPATH-STATUS.md` with the path-switch event.
5. Update `SECRETS-RUNBOOK.md` with the new credentials.

**Caveat**: even with Path C, a brand-new signed binary still triggers SmartScreen until reputation accumulates (typically thousands of downloads or an EV cert which is ~$500/yr higher). The unsigned-fallback README remains relevant during the reputation-build window.

### Any week — Unsigned-fallback path (no cost, no gate)

If NO path produces a working signature by v1.7 RC tag, **ship unsigned** per the deliverable in `packaging/windows/README-SMARTSCREEN.md`. v1.7 ships either way; signing is best-effort by design.

When the signature eventually lands (post-v1.7 if needed), backfill the signed installer to v1.7 release assets via:

```bash
gh release upload v1.7 ./signed-installer/BALLView-v1.7-windows-x64-installer.exe --clobber --repo BALL-Project/ball
```

The released-tag artifact gets swapped; the next download is signed; SmartScreen reputation starts accumulating against the signed binary.

## 8. After approval — what changes

Once the foundation approves and provisions the SignPath organization:

1. Foundation gives you: `organization-id`, project slug (`ballview`), signing policy slug (`release-signing`).
2. Generate an API token in the SignPath UI; record it in 1Password (vault: `BALL/Windows-Signing/`).
3. Add GitHub repo secrets:
   - `SIGNPATH_API_TOKEN` = the API token
   - `SIGNPATH_ORG_ID` = the organization-id
4. The release.yml workflow already has the gated signing step (see Phase 8b Task 3 commit). When the secret appears, the next release run on a tag will automatically:
   - Build the unsigned NSIS installer.
   - Upload it to SignPath.
   - **Block on the SignPath UI** until an admin clicks "approve" (intentional anti-supply-chain-attack control).
   - Download the signed installer back into the workflow.
   - Upload signed installer as the release asset.

No workflow refactor needed — the `if: env.SIGNPATH_API_TOKEN != ''` predicate is the gating mechanism.

5. Update `08b-SIGNPATH-STATUS.md` to **approved** state.
6. Update `packaging/windows/README-SMARTSCREEN.md` to note signing is live (warning may still appear during reputation-build window).

## 9. Bus-factor checklist (before submitting)

- [ ] Primary admin identified and has GitHub admin on `BALL-Project/ball`.
- [ ] Secondary admin identified and confirmed willing to be the recovery contact.
- [ ] Both admins have 1Password access to vault `BALL/Windows-Signing/` (or equivalent).
- [ ] Annual review reminder filed for 12 months out (refresh credentials + verify both admins active).

See `SECRETS-RUNBOOK.md` (companion document in this directory) for the full
bus-factor procedure.

---

**Status:** AGENT-DRAFTED — awaiting human to fill §4 (named admins) and
execute §1-§6 (submit the application).
