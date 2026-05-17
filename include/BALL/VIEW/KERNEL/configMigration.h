// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_KERNEL_CONFIGMIGRATION_H
#define BALL_VIEW_KERNEL_CONFIGMIGRATION_H

// Phase 999.45 — BALLView Refresh: ConfigMigration
// =================================================
//
// First-launch migration of the legacy flat ~/.BALLView INI file into
// platform-correct standard locations (Handover §09 — "File system
// paths"):
//
//   Linux   : ~/.config/BALLView/BALLView.ini    (XDG)
//   macOS   : ~/Library/Application Support/BALLView/BALLView.ini
//   Windows : %APPDATA%/BALLView/BALLView.ini
//
// Required because the [Inspector] INI schema added by Phase 999.44
// and the [Appearance] schema added by Phase 999.48 introduce keys
// that didn't exist in the v1.6 INI format — the migration step is
// the right place to bridge the on-disk format change. The
// migration also fixes a long-standing portability bug: dropping a
// dot-file at the user's $HOME root has never been correct for
// macOS or Windows.
//
// The class is platform-portable and ifdef-free — Qt's
// QStandardPaths::writableLocation(AppConfigLocation) produces the
// correct path on each OS.
//
// Idempotent: if the target file already exists, run() is a no-op
// (returns performed=false). The legacy file is NOT deleted — the
// user retains it as a self-service backup; Mainframe additionally
// copies it to ~/.BALLView.pre-v2.bak before invoking the
// first-run prompt (see main.C 999.45-Task-7 wiring).

#include <BALL/COMMON/global.h>

#include <QtCore/QString>

namespace BALL
{
	namespace VIEW
	{
		/**
		 *  Result of a ConfigMigration::run() invocation.
		 */
		struct BALL_VIEW_EXPORT MigrationResult
		{
			/// True if a copy was actually performed.
			bool performed = false;

			/// True iff another BALLView instance held the migration
			/// lock and this run deferred — callers should treat as
			/// "try again next launch", not as an error to surface.
			/// (v1.7-RC1 C-6 — QLockFile TOCTOU guard.)
			bool another_instance = false;

			/// Absolute path of the legacy source file.
			QString sourcePath;

			/// Absolute path of the new target file (post-migration).
			QString targetPath;

			/// Empty on success; populated on copy/IO failure.
			QString error;
		};

		/**
		 *  Legacy `~/.BALLView` to platform-correct standard-paths
		 *  migration helper.
		 *
		 *  Called once on first launch from main.C under BALL_UI_V2.
		 */
		class BALL_VIEW_EXPORT ConfigMigration
		{
			public:
				/**
				 *  Perform the migration if needed.
				 *
				 *  Steps:
				 *    1. Check legacyPath exists. If not, no-op.
				 *    2. Compute target path via
				 *       QStandardPaths::AppConfigLocation + "/BALLView.ini".
				 *    3. If target already exists, no-op (idempotent).
				 *    4. mkpath(target dir).
				 *    5. Copy legacy file to target path.
				 *    6. If legacy workspaces dir
				 *       (legacyPath + "_workspaces") exists, copy it to
				 *       target's "workspaces/" sibling subdirectory.
				 *    7. Append/write a [Migration] migrated_from=<legacy>
				 *       marker key to the target file.
				 *
				 *  Legacy file is NOT deleted (caller decides whether to
				 *  back it up as ~/.BALLView.pre-v2.bak first).
				 *
				 *  @param legacyPath absolute path to ~/.BALLView
				 *  @return MigrationResult with performed=true iff
				 *          something was copied
				 */
				static MigrationResult run(const QString& legacyPath);

				/**
				 *  Compute the target path for a given platform without
				 *  running the migration. Useful for the first-run
				 *  prompt to display the new location to the user.
				 */
				static QString targetConfigPath();

				/**
				 *  Compute the target cache path for the platform.
				 *  Not migrated (apps regenerate cache on first run)
				 *  but exposed so callers can display it.
				 */
				static QString targetCachePath();
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_KERNEL_CONFIGMIGRATION_H
