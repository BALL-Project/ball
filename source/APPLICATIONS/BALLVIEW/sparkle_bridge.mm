// -*- Mode: ObjC++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.8-full — Sparkle (macOS) auto-update integration shim.
//
// Compiled ONLY on Apple platforms via the if(APPLE) guard in
// source/APPLICATIONS/BALLVIEW/CMakeLists.txt, and only when
// BALL_HAS_SPARKLE is defined (CMake auto-detects the vendored
// packaging/macos/Sparkle.xcframework directory).
//
// The shim exposes a single C-linkage entry point — ball_sparkle_init() —
// which main.C calls after QApplication construction. Sparkle's UI uses
// AppKit (NSAlert / NSWindow) and cohabits with Qt's Cocoa event loop
// without an integration shim per the Phase 999.8 spike (Sparkle/Qt-6
// risk RETIRED — see .planning/phases/999.8-auto-update-sparkle-winsparkle/
// 999.8-SPIKE.md §1).
//
// Prior art: KeePassXC, OBS Studio, Mumble — all ship the same pattern.

#ifdef BALL_HAS_SPARKLE

#import <Foundation/Foundation.h>
#import <Sparkle/Sparkle.h>

// Strong reference keeps the updater controller alive for the lifetime of the
// process. NSApp also retains it, but holding our own ref makes the
// ownership explicit and protects against future NSApp lifecycle changes.
static SPUStandardUpdaterController* g_ball_updater_controller = nil;

extern "C" void ball_sparkle_init(void) {
  if (g_ball_updater_controller != nil) {
    return;  // idempotent — main.C may call this from multiple paths
  }

  @autoreleasepool {
    // SPUStandardUpdaterController is Sparkle 2.x's recommended entry point.
    // It instantiates an SPUUpdater + default SPUStandardUserDriver, wires
    // them together, and starts background update checks per the bundle's
    // Info.plist (SUFeedURL, SUEnableAutomaticChecks, SUScheduledCheckInterval).
    //
    // initWithStartingUpdater:YES => starts immediately (per spike rec).
    // updaterDelegate:nil + userDriverDelegate:nil => use defaults (no
    // BALL-specific customisation; can be added in a v1.8 hardening pass).
    g_ball_updater_controller = [[SPUStandardUpdaterController alloc]
        initWithStartingUpdater:YES
                updaterDelegate:nil
             userDriverDelegate:nil];

    // Feed URL is normally read from Info.plist (SUFeedURL key). We also
    // set it programmatically here to keep the source of truth co-located
    // with the WinSparkle init in main.C — symmetric across platforms,
    // and survives if Info.plist's SUFeedURL is accidentally dropped.
    NSURL* feedURL = [NSURL URLWithString:
        @"https://ball-project.github.io/ball/appcast-macos.xml"];
    [[g_ball_updater_controller updater] setFeedURL:feedURL];
  }
}

#else  // !BALL_HAS_SPARKLE

// Stub when Sparkle is not vendored — main.C still calls ball_sparkle_init()
// but the call is a no-op. Keeps main.C free of #ifdef BALL_HAS_SPARKLE.
extern "C" void ball_sparkle_init(void) {}

#endif  // BALL_HAS_SPARKLE
