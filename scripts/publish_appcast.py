#!/usr/bin/env python3
"""Phase 999.8-full — appcast publish helper.

Invoked by the `publish-appcast` job in .github/workflows/release.yml after a
tagged release is published. Signs the release's .dmg + .exe with the
project's Ed25519 private key and inserts a new <item> at the top of the
gh-pages-checkout/appcast-{macos,windows}.xml files.

Reads (env vars set by the CI job):
    APPCAST_REF        — git tag (e.g. "v1.7.0")
    APPCAST_VERSION    — version without leading "v" (e.g. "1.7.0")
    APPCAST_PUBDATE    — RFC 822 timestamp
    APPCAST_REPO       — owner/name (e.g. "BALL-Project/ball")
    APPCAST_KEY_FILE   — path to the Ed25519 private key PEM (temp file)
    APPCAST_MACOS_DMG  — path to local .dmg (empty string if missing)
    APPCAST_WIN_EXE    — path to local .exe installer (empty string if missing)

Writes back to:
    gh-pages-checkout/appcast-macos.xml
    gh-pages-checkout/appcast-windows.xml
"""

from __future__ import annotations

import base64
import os
import pathlib
import re
import subprocess
import sys


def ed25519_sign(file_path: str, key_file: str) -> str:
    """Return the base64-encoded Ed25519 signature of file_path."""
    result = subprocess.run(
        ["openssl", "pkeyutl", "-sign", "-inkey", key_file, "-rawin", "-in", file_path],
        check=True,
        capture_output=True,
    )
    return base64.b64encode(result.stdout).decode("ascii")


def build_item(*, ref: str, version: str, pubdate: str, repo: str,
               platform: str, local_path: str, key_file: str) -> str:
    """Build a single appcast <item> block."""
    asset_name = pathlib.Path(local_path).name
    url = f"https://github.com/{repo}/releases/download/{ref}/{asset_name}"
    size = pathlib.Path(local_path).stat().st_size
    sig = ed25519_sign(local_path, key_file)
    return (
        "    <item>\n"
        f"      <title>BALLView {ref}</title>\n"
        f"      <pubDate>{pubdate}</pubDate>\n"
        f"      <sparkle:releaseNotesLink>https://github.com/{repo}/releases/tag/{ref}</sparkle:releaseNotesLink>\n"
        "      <enclosure\n"
        f"        url=\"{url}\"\n"
        f"        sparkle:version=\"{version}\"\n"
        f"        sparkle:os=\"{platform}\"\n"
        f"        length=\"{size}\"\n"
        "        type=\"application/octet-stream\"\n"
        f"        sparkle:edSignature=\"{sig}\" />\n"
        "    </item>\n"
    )


def insert_into_appcast(xml_path: pathlib.Path, item: str) -> None:
    """Insert item immediately after the <language>...</language> element."""
    xml = xml_path.read_text()
    new_xml, n = re.subn(
        r"(</language>\s*\n)",
        lambda m: m.group(1) + item,
        xml,
        count=1,
    )
    if n == 0:
        raise SystemExit(f"::error::Could not find <language> insertion point in {xml_path}")
    xml_path.write_text(new_xml)
    print(f"Inserted appcast item into {xml_path}")


def main() -> int:
    ref = os.environ["APPCAST_REF"]
    version = os.environ["APPCAST_VERSION"]
    pubdate = os.environ["APPCAST_PUBDATE"]
    repo = os.environ["APPCAST_REPO"]
    key_file = os.environ["APPCAST_KEY_FILE"]
    macos_dmg = os.environ.get("APPCAST_MACOS_DMG", "")
    win_exe = os.environ.get("APPCAST_WIN_EXE", "")

    common = dict(ref=ref, version=version, pubdate=pubdate, repo=repo, key_file=key_file)

    if macos_dmg and pathlib.Path(macos_dmg).is_file():
        item = build_item(platform="macos", local_path=macos_dmg, **common)
        insert_into_appcast(
            pathlib.Path("gh-pages-checkout/appcast-macos.xml"), item,
        )
    else:
        print(f"::warning::No .dmg asset; skipping macOS appcast update.")

    if win_exe and pathlib.Path(win_exe).is_file():
        item = build_item(platform="windows", local_path=win_exe, **common)
        insert_into_appcast(
            pathlib.Path("gh-pages-checkout/appcast-windows.xml"), item,
        )
    else:
        print(f"::warning::No .exe installer asset; skipping Windows appcast update.")

    return 0


if __name__ == "__main__":
    sys.exit(main())
