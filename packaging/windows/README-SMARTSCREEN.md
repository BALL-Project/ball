# Windows SmartScreen Warning — How to install BALLView (unsigned installer)

This release of BALLView ships with an **unsigned** Windows installer. The
SignPath Foundation OSS code-signing application is in flight (setup +
status: `.planning/phases/999.68-packaging-signing-license/SIGNPATH-SECRETS-RUNBOOK.md`);
once approved and the `SIGNPATH_API_TOKEN` secret is configured, future
releases will be Authenticode-signed.

Until then, Windows will warn the first time you run the installer.
This README explains why, how to install safely, and how to verify the
installer is the genuine BALLView release.

## What you'll see

When you double-click `BALLView-<version>-windows-x64-installer.exe`,
Windows SmartScreen will show:

> **Windows protected your PC**
>
> Microsoft Defender SmartScreen prevented an unrecognized app from
> starting. Running this app might put your PC at risk.

There is **no "Run anyway" button visible by default** — Microsoft
intentionally hides it behind one extra click to make sure you've thought
about it.

## How to install

1. In the SmartScreen warning dialog, click **More info**.
   The dialog expands and a **Run anyway** button appears.
2. Click **Run anyway**.
3. The installer launches; follow the prompts (per-user install — no admin
   prompt required; default location `%LOCALAPPDATA%\Programs\BALLView`).
4. Once installed, "BALLView" appears in your Start Menu.

**Alternate path** if the warning persists on a subsequent download
(e.g., on a different machine):
- Right-click the `.exe` → **Properties** → check **Unblock** at the bottom
  of the General tab → **OK**. Then double-click to launch.

## Why this happens

Windows SmartScreen rejects any executable that does not have an
established **download reputation**. Reputation is built from two signals:

1. **Authenticode signature** (a code-signing certificate that chains to a
   trusted CA). BALLView v1.7 does not yet have one — see status above.
2. **Download volume + age** for the specific binary hash. Brand-new
   binaries trigger SmartScreen even if signed, until thousands of users
   have downloaded the same hash without flagging it.

For a small-volume academic OSS project, reputation accumulates slowly
(months, sometimes longer). The fastest fix is the cert, which is what
we're working on.

## Verifying the installer is the genuine BALLView release

Even without a code-signing signature, you can verify the installer
matches what was built on GitHub Actions from the public source code.

1. Open the GitHub Release page:
   <https://github.com/BALL-Project/ball/releases>
2. Locate the release matching your version (e.g., `v1.7`).
3. In the release notes, find the **SHA256** line for the Windows
   installer — it looks like:
   `SHA256: a1b2c3d4...` (64 hex characters).
4. Compute the SHA256 of your downloaded installer:

   PowerShell:
   ```powershell
   (Get-FileHash BALLView-v1.7-windows-x64-installer.exe -Algorithm SHA256).Hash
   ```

   Or Command Prompt:
   ```cmd
   certutil -hashfile BALLView-v1.7-windows-x64-installer.exe SHA256
   ```

5. Compare the two values. They MUST be identical. If they differ, the
   download was corrupted or tampered with — re-download from the
   GitHub Release page directly.

The SHA256 in the release notes is itself published over HTTPS by GitHub
and signed (via GitHub's own infrastructure) — that's the anchor of
trust for the unsigned-fallback path.

## When will the signed installer be available?

Tracking lives in `.planning/phases/08b-packaging-windows/08b-SIGNPATH-STATUS.md`
in this repo. The foundation review typically takes 2-6 weeks. Once
approved:

- New releases will be signed automatically (the workflow has the gated
  signing step already in place — only the SignPath organisation
  credentials need to be added as a GitHub Actions secret).
- v1.7's installer will be re-signed and re-uploaded to the v1.7 release
  page (`gh release upload --clobber`); the existing v1.7 download link
  will then deliver the signed installer to anyone who installs after the
  swap.
- SmartScreen reputation begins accumulating against the signed binary;
  the warning gradually disappears for new downloaders.

## Reporting installer problems

If the SmartScreen warning doesn't disappear after **More info → Run
anyway**, OR the installer fails to launch at all, OR you suspect the
binary has been tampered with, please open an issue:

<https://github.com/BALL-Project/ball/issues>

Include:
- The exact version (e.g., `v1.7`).
- The SHA256 you computed.
- The Windows version you're on (`winver` in Run dialog).
- A screenshot of the warning if it differs from the description above.
