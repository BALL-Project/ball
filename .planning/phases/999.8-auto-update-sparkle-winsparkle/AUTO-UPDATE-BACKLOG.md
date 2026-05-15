---
phase: 999.8-auto-update-sparkle-winsparkle
type: backlog
status: dormant
gates: Phase 8 (Packaging & Distribution) must land first
parent_phase: post-Phase-8
created: 2026-05-15
---

# Phase 999.8: Auto-Update via Sparkle + WinSparkle

## Status: DORMANT — DEFERRED PER PHASE 8 RESEARCH DECISIONS

Captured 2026-05-15 from Phase 8's [`08-INSTALLER-FORMATS-RESEARCH.md`](../08-packaging-and-distribution/08-INSTALLER-FORMATS-RESEARCH.md) Decision #5: Phase 8 ships installers without auto-update; users manually re-download for v1.x → v1.x+1. Auto-update is non-trivial enough to be its own backlog phase.

## Why this exists

After Phase 8 ships signed/notarized DMG + signed NSIS `.exe` installers, BALLView users get a polished install experience — but to receive future updates (v1.7, v1.8, security fixes), they have to manually go back to GitHub Releases and download the new installer. That's friction. Mature desktop apps offer:
- "BALLView would like to install version 1.7. Update now?" prompt on launch
- Optional automatic background download + install on next launch
- Rollback to the previous version if the new one crashes

Sparkle (macOS) and WinSparkle (Windows) are the canonical OSS frameworks for this. Both consume an "appcast" — an XML/RSS feed listing available versions, their download URLs, release notes, and signed update manifests.

## Why this is non-trivial

1. **Appcast hosting** — needs a stable URL serving signed `appcast.xml`. Options: GitHub Pages, project website, S3 / Cloudflare R2 bucket. Each adds infrastructure.
2. **Signed update manifests** — Sparkle requires Ed25519-signed download checksums in the appcast so a compromised CDN can't ship malicious updates. Key management.
3. **Update rollout pacing** — Sparkle supports phased rollouts (5% → 25% → 100% over days) — needs orchestration if used.
4. **Rollback / crash recovery** — Sparkle has "delta updates" + "rollback on crash" features that need integration in BALL's main loop.
5. **Build pipeline integration** — every release needs to:
   - Generate the delta patch (Sparkle's binary diff)
   - Sign the new artifact with the Ed25519 update key
   - Update the appcast feed
   - Publish atomically (broken appcast = users see no update or a phantom one)
6. **Per-platform parity** — Sparkle and WinSparkle have slightly different appcast schemas, different signing key formats (EdDSA vs DSA), different UI conventions. Maintaining both consistently is work.
7. **License / privacy review** — auto-update implies the app phones home (the appcast URL is hit on launch). User opt-out + privacy-policy implications.

Phase 8 deliberately ships without this so v1.6 can release. Auto-update lands on a future v1.x+0.1 release once the foundation is stable.

## Dependencies (must be true before promotion)

- ✅ Phase 02.1 — Renderer boundary
- ❌ Phase 5 — Qt 6 migration (auto-update UI integrates with QApplication event loop)
- ❌ Phase 8 — Packaging & Distribution (Sparkle + WinSparkle integrate INTO the installer pipeline; signing infrastructure must exist first)
- ❌ Project decision on appcast hosting (GitHub Pages vs ball-project.org vs CDN bucket)

## Tools

### macOS — Sparkle

[github.com/sparkle-project/Sparkle](https://github.com/sparkle-project/Sparkle). MIT-licensed. The canonical macOS auto-update framework. Used by countless macOS apps (Transmission, Cyberduck, Slack-era pre-Electron, etc.).

Integration sketch:
- Add Sparkle.framework to BALLView.app's `Contents/Frameworks/`
- Add `SUFeedURL` to Info.plist pointing at the appcast XML
- Add `SUPublicEDKey` to Info.plist (Ed25519 public key for verifying updates)
- Hook Sparkle's `SUUpdater` into `MainFrame.cpp` startup
- Tools: `generate_appcast` (Sparkle's CLI) + `sign_update` for the Ed25519 signing in the release pipeline

### Windows — WinSparkle

[github.com/vslavik/winsparkle](https://github.com/vslavik/winsparkle). MIT-licensed. Sparkle-compatible appcast format, similar API surface.

Integration sketch:
- Link `WinSparkle.dll` into BALLView.exe
- Call `win_sparkle_set_appcast_url("https://...")` at startup
- Call `win_sparkle_check_update_with_ui()` on user request or `win_sparkle_check_update_without_ui()` on launch
- DSA signing key for update manifests (Sparkle moved to Ed25519; WinSparkle still uses DSA per its current docs — verify on adoption)

## Recommended structure when promoted

Three sub-plans:
1. **Plan 1: Appcast hosting + key management** — decide hosting, generate signing keys, set up CI/CD to publish appcast on every release
2. **Plan 2: macOS Sparkle integration** — add framework, Info.plist plumbing, UI hooks, release-pipeline integration
3. **Plan 3: Windows WinSparkle integration** — DLL link, API hooks, release-pipeline integration

Out of scope for this dormant entry:
- Delta-patch generation (might be a sub-plan or deferred to a later phase)
- Phased rollouts
- A/B testing infrastructure
- Telemetry / analytics on update adoption rates

## References

- [sparkle-project/Sparkle](https://github.com/sparkle-project/Sparkle) — macOS auto-update framework
- [Sparkle documentation](https://sparkle-project.org/documentation/)
- [vslavik/winsparkle](https://github.com/vslavik/winsparkle) — Windows port of Sparkle
- [WinSparkle wiki](https://github.com/vslavik/winsparkle/wiki) — usage docs
- Phase 8 [`08-INSTALLER-FORMATS-RESEARCH.md`](../08-packaging-and-distribution/08-INSTALLER-FORMATS-RESEARCH.md) Decision #5 — the source of this deferral
