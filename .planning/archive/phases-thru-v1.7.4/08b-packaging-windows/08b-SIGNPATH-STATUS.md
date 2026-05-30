# Phase 8b — SignPath / Windows Signing Status Log

Tracks the externally-gated SignPath Foundation enrollment that powers
Windows code-signing in `.github/workflows/release.yml`. Companion to
`SECRETS-RUNBOOK.md` and `SIGNPATH-APPLICATION-RUNBOOK.md`.

## Current state

**AWAITING SIGNPATH FOUNDATION APPROVAL** (application submitted 2026-05-21).

The CI signing chain is already wired and gated on `SIGNPATH_API_TOKEN`
(absent → unsigned NSIS installer + SmartScreen warning, the accepted v1.7
fallback). Windows signing is **not** a v1.7.1 blocker — it runs async and
turns on automatically once the two secrets land.

## Go-live checklist (when SignPath approves)

1. In the SignPath org, create the artifacts using the slugs **hardcoded in
   `release.yml`** (they must match exactly):
   - project slug: `ballview`
   - signing-policy slug: `release-signing`
   - artifact-configuration slug: `installer`
2. Generate an API token.
3. Set the two GitHub Actions repo secrets:
   - `SIGNPATH_API_TOKEN`
   - `SIGNPATH_ORG_ID`
4. Fill the bus-factor roles table in `SECRETS-RUNBOOK.md` §3 (≥2 humans for
   SignPath admin / GitHub admin / vault owner — P0 per v1.7-PLAN.md).
5. Tag a release (or re-tag a patch); the first signing request may need a
   one-time manual approval click in the SignPath UI (the workflow blocks on
   `wait-for-completion: true`).
6. Verify the published `.exe` is Authenticode-signed (right-click →
   Properties → Digital Signatures, or `signtool verify /pa`).

## Status log

| Date | Event | Action / next |
|---|---|---|
| 2026-05-17 | 8b runbooks drafted; enrollment pending | Waiting on human-driven application |
| 2026-05-21 | **SignPath Foundation application SUBMITTED** | Awaiting foundation review/approval (typically days–weeks). On approval → run the go-live checklist above. |
