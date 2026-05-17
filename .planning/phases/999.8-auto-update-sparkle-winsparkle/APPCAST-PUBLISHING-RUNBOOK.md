# Appcast Publishing Runbook — `gh-pages` Branch on `BALL-Project/ball`

> **AGENT-DRAFTED — Phase 999.8-full scaffolding.**
> Branch setup (§1) is a ONE-TIME human action. Once gh-pages exists with the
> initial appcast XMLs, the `publish-appcast` job in `release.yml` maintains
> it automatically on every tagged release.

## 1. One-time setup — initialize `gh-pages` branch

Performed by a repo admin with push rights to `BALL-Project/ball`:

```sh
# Create an orphan branch with NO history from main — gh-pages stays small
# and disjoint from the dev branches, mirroring the upstream OSS pattern.
git checkout --orphan gh-pages
git rm -rf .   # wipe the working tree (clean slate)

# Seed with the template appcast XMLs from main:
git show v1.7-modernization:packaging/appcast/appcast-macos.xml > appcast-macos.xml
git show v1.7-modernization:packaging/appcast/appcast-windows.xml > appcast-windows.xml

# Optional but recommended: an index.html so GH Pages serves something
# friendly at the bare domain. A 4-line stub is enough.
cat > index.html <<'EOF'
<!DOCTYPE html>
<title>BALLView Auto-Update Feeds</title>
<h1>BALLView Auto-Update</h1>
<ul>
  <li><a href="appcast-macos.xml">macOS appcast</a></li>
  <li><a href="appcast-windows.xml">Windows appcast</a></li>
</ul>
EOF

git add appcast-macos.xml appcast-windows.xml index.html
git commit -m "gh-pages: seed appcast feeds for Phase 999.8 auto-update"
git push -u origin gh-pages
```

Then in GitHub UI:
1. Repo Settings → Pages → Source = `Deploy from a branch`.
2. Branch = `gh-pages`, folder = `/ (root)`.
3. Save. GH Pages provisions the Fastly CDN; live URL is
   `https://ball-project.github.io/ball/` within ~1 minute.

Verify:
```sh
curl -sI https://ball-project.github.io/ball/appcast-macos.xml | head -3
# Expect: HTTP/2 200, content-type: application/xml or text/xml
```

---

## 2. CI auto-maintenance — `publish-appcast` job

Added to `.github/workflows/release.yml` by Phase 999.8-full. Runs as a
new job AFTER `release` (which uploads the .dmg + .exe to GitHub Releases),
on TAG-push refs only.

Logic:
1. Checkout `gh-pages` into a sibling worktree.
2. Download release artifacts metadata (`gh release view --json assets`).
3. For each macOS .dmg + Windows .exe-installer:
   - Compute SHA256 + file size.
   - Sign with Ed25519 private key (from `AUTOUPDATE_ED25519_PRIVATE_KEY` secret).
   - Build a new `<item>` block.
4. Insert new `<item>` blocks at the TOP of `appcast-{macos,windows}.xml`
   (Sparkle reads top-to-bottom and presents the first item ≥ installed
   version).
5. Commit + push gh-pages with message `appcast: <tag>`.

The job is **fail-fast on signing-key absence in prod mode**: if
`AUTOUPDATE_ED25519_PUBLIC_KEY` is unset AND the tag does not begin with
`dev-`, the job errors out with a clear message pointing at this runbook.

---

## 3. Manual republish (recovery path)

If the `publish-appcast` job fails (network blip, gh-pages lock contention,
etc.) the maintainer republishes from a workstation:

```sh
# 1. Fetch the tag's release artifacts
gh release download v1.7.x --repo BALL-Project/ball --dir /tmp/release/

# 2. Sign the .dmg + .exe with the Ed25519 private key
#    (Sparkle: ./sign_update /tmp/release/BALLView-*.dmg)
#    (OpenSSL fallback: openssl pkeyutl -sign ...)

# 3. Hand-edit packaging/appcast/appcast-{macos,windows}.xml in a gh-pages
#    checkout, inserting the new <item> at the top.

# 4. git add + commit + push gh-pages.
```

The pattern is identical to the CI flow — `publish-appcast` is just a
scripted automation of the manual recipe.

---

## 4. Why GitHub Pages (not a custom domain or S3)

Decided in 999.8-SPIKE.md §3 (REC #3). Summary:
- **Zero cost.**
- **Same trust boundary as the binaries** — compromised GitHub account is
  already game-over for the .dmg/.exe; the appcast adds no new failure mode.
- **Atomic per-release publication** — `release.yml` job commits gh-pages
  inside the same workflow run as the artifact upload, making the appcast
  entry visible exactly when the binary is downloadable.
- **CDN-fronted by default** (Fastly) — Sparkle's once-a-day check load is
  trivially absorbed.
- **No DNS / org coordination required** — defers the unresolved
  institutional-sponsorship question from Phase 8a Open Q #1.

If a custom domain is later wanted (e.g., `updates.ball-project.org`), GH
Pages supports CNAME aliases out of the box — switching is a one-line DNS
change with no code impact.

---

## 5. Appcast schema reference

See `packaging/appcast/appcast-{macos,windows}.xml` for the committed
templates. Sparkle's authoritative schema doc is at
<https://sparkle-project.org/documentation/publishing/>.

Key fields the publish-appcast job must populate per release:

| Field | Source |
|---|---|
| `<title>` | "BALLView vX.Y.Z" — from `github.ref_name` |
| `<pubDate>` | RFC 822 timestamp of the release |
| `<sparkle:minimumSystemVersion>` (macOS) | "13.0" — bumped only on Phase-level macOS-target changes |
| `<enclosure url=…>` | GH Releases asset URL (`gh release view --json assets`) |
| `<enclosure sparkle:version=…>` | strip leading `v` from `github.ref_name` |
| `<enclosure length=…>` | bytes (from `gh release view` assets array) |
| `<enclosure sparkle:edSignature=…>` | base64 from `sign_update` / openssl |

---

## 6. Out of scope for v1.7

- Phased rollout (`<sparkle:phasedRolloutInterval>`) — see SPIKE §5 R-rollback. Defer to v1.8 hardening.
- Delta updates (`<sparkle:deltas>`) — saves bandwidth on incremental releases but adds CI complexity. Defer.
- Release-notes HTML hosted alongside appcast — current `<sparkle:releaseNotesLink>` points at the GH Releases page (the existing release notes UI). Defer custom HTML to v1.8.
