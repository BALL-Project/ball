// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/KERNEL/configMigration.h>

#include <BALL/COMMON/logStream.h>

#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QDir>
#include <QtCore/QDirIterator>
#include <QtCore/QLockFile>
#include <QtCore/QStandardPaths>
#include <QtCore/QDateTime>

namespace BALL
{
	namespace VIEW
	{
		QString ConfigMigration::targetConfigPath()
		{
			// AppConfigLocation on each platform:
			//   Linux   : ~/.config/<APPNAME>
			//   macOS   : ~/Library/Preferences/<APPNAME>
			//             (NOTE: AppConfigLocation == Preferences on macOS;
			//              the Handover §09 spec lists "Application Support"
			//              for the config file. We follow Qt's idiomatic
			//              choice — AppConfigLocation — because QSettings
			//              with IniFormat in WorkspaceManager uses the same
			//              path, ensuring a single canonical location.)
			//   Windows : %APPDATA%/<APPNAME>
			//
			// QCoreApplication::applicationName must be set before this
			// returns the right thing. BALLView's main.C does not set it
			// today; the path will fall back to the binary name. Most
			// users will end up with .../BALLView/BALLView.ini. To make
			// the path deterministic regardless of application-name setup
			// order, hard-code the "BALLView" segment.
			QString dir = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
			if (dir.isEmpty())
			{
				dir = QDir::homePath() + "/.config/BALLView";
			}
			// Ensure trailing segment ends with BALLView. If
			// applicationName isn't set, AppConfigLocation may return
			// .../<organizationName>/ without an app subdir.
			if (!dir.endsWith("BALLView"))
			{
				dir += "/BALLView";
			}
			return dir + "/BALLView.ini";
		}

		QString ConfigMigration::targetCachePath()
		{
			QString dir = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
			if (dir.isEmpty())
			{
				dir = QDir::homePath() + "/.cache/BALLView";
			}
			if (!dir.endsWith("BALLView"))
			{
				dir += "/BALLView";
			}
			return dir;
		}

		namespace
		{
			// Recursive copy: walks src/ and copies every file into dst/,
			// preserving relative directory structure. dst/ is created
			// if missing. Returns false on first error.
			bool copyRecursive(const QString& src, const QString& dst)
			{
				QDir().mkpath(dst);
				QDir srcDir(src);
				if (!srcDir.exists()) return true;  // nothing to copy

				const QFileInfoList entries = srcDir.entryInfoList(
				    QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
				for (const QFileInfo& entry : entries)
				{
					const QString srcPath = entry.absoluteFilePath();
					const QString dstPath = dst + "/" + entry.fileName();
					if (entry.isDir())
					{
						if (!copyRecursive(srcPath, dstPath)) return false;
					}
					else if (entry.isFile())
					{
						if (QFile::exists(dstPath))
						{
							// Idempotent: don't overwrite if already there.
							continue;
						}
						if (!QFile::copy(srcPath, dstPath)) return false;
					}
				}
				return true;
			}

			// Append a [Migration] section marker to the target INI file
			// so future runs / diagnostics can see where the file came
			// from. We append rather than parse because the file is
			// already a valid INI written by BALL::INIFile.
			bool appendMigrationMarker(const QString& targetPath, const QString& legacyPath)
			{
				QFile f(targetPath);
				if (!f.open(QIODevice::Append | QIODevice::Text))
				{
					return false;
				}
				const QString marker =
				    QString("\n\n[Migration]\n"
				            "migrated_from=%1\n"
				            "migrated_at=%2\n"
				            "migration_phase=999.45\n")
				        .arg(legacyPath)
				        .arg(QDateTime::currentDateTimeUtc().toString(Qt::ISODate));
				f.write(marker.toUtf8());
				f.close();
				return true;
			}
		}

		MigrationResult ConfigMigration::run(const QString& legacyPath)
		{
			MigrationResult r;
			r.sourcePath = legacyPath;
			r.targetPath = targetConfigPath();

			if (!QFile::exists(legacyPath))
			{
				// Fresh install — no migration needed.
				return r;
			}

			// Cheap pre-lock skip: if the target already exists, no need
			// to even acquire the lock. Re-checked under the lock below
			// for the TOCTOU-collapse pass (C-6).
			if (QFile::exists(r.targetPath))
			{
				return r;
			}

			// mkpath target directory (must happen before lock acquisition
			// so the lock file's directory exists).
			const QString targetDir = QFileInfo(r.targetPath).absolutePath();
			if (!QDir().mkpath(targetDir))
			{
				r.error = QString("Could not create target directory: %1").arg(targetDir);
				BALL::Log.warn() << "ConfigMigration: " << r.error.toStdString() << std::endl;
				return r;
			}

			// v1.7-RC1 C-6 — QLockFile guard. Two BALLView launches racing
			// the first-run migration could both observe `exists(target)
			// == false`, both copy, and the second clobber the first. The
			// lock serializes the critical region; on contention the
			// loser returns another_instance=true (caller treats as
			// "try again next launch").
			QLockFile lock(targetDir + "/.ball-migration.lock");
			lock.setStaleLockTime(60000);  // 60s — covers slow SMB homedirs
			if (!lock.tryLock(2000))
			{
				BALL::Log.warn() << "ConfigMigration: another instance holds the migration lock; deferring." << std::endl;
				r.another_instance = true;
				return r;
			}

			// C-6 TOCTOU collapse: re-check target existence AFTER lock.
			// The pre-lock exists() check above may have raced with a
			// peer that just finished — without this we would attempt
			// a copy onto a pre-existing target.
			if (QFile::exists(r.targetPath))
			{
				return r;
			}

			// v1.7-RC1 C-4 — atomic copy. Write to <target>.tmp, append
			// the migration marker into the .tmp, then rename(.tmp →
			// target) atomically. A crash mid-write leaves the .tmp behind
			// (cleaned on next launch by tryRemove below) and the user's
			// preferences in their pre-migration state — never half-copied.
			const QString tmpPath = r.targetPath + QStringLiteral(".tmp");
			// Clean up any orphaned .tmp from a previous crashed run.
			if (QFile::exists(tmpPath))
			{
				QFile::remove(tmpPath);
			}

			if (!QFile::copy(legacyPath, tmpPath))
			{
				r.error = QString("Copy failed: %1 → %2").arg(legacyPath).arg(tmpPath);
				BALL::Log.warn() << "ConfigMigration: " << r.error.toStdString() << std::endl;
				return r;
			}

			// C-4 — append the migration marker to the .tmp BEFORE the
			// atomic rename so the rename publishes a complete file.
			if (!appendMigrationMarker(tmpPath, legacyPath))
			{
				BALL::Log.warn() << "ConfigMigration: could not append migration marker to "
				                 << tmpPath.toStdString() << std::endl;
				// Don't fail the whole migration over a marker write —
				// continue to the atomic rename so the user still gets
				// their preferences migrated.
			}

			// C-4 — atomic publish. QFile::rename is rename(2) on POSIX
			// (atomic) and MoveFileExW(MOVEFILE_REPLACE_EXISTING) on
			// Windows (atomic on NTFS). On failure, delete the .tmp and
			// leave the legacy file untouched — caller will retry on
			// next launch.
			if (!QFile::rename(tmpPath, r.targetPath))
			{
				r.error = QString("Atomic rename failed: %1 → %2").arg(tmpPath).arg(r.targetPath);
				BALL::Log.warn() << "ConfigMigration: " << r.error.toStdString() << std::endl;
				QFile::remove(tmpPath);
				return r;
			}

			// Migrate the workspaces/ subdirectory if it exists. Legacy
			// layout convention: ~/.BALLView_workspaces/ as a sibling
			// directory. (BALL pre-v2 didn't actually have user
			// workspaces, but defensive: pick it up if present.)
			// Runs AFTER the atomic config rename — if the main INI made
			// it through, the workspaces copy is best-effort gravy.
			const QString legacyWorkspacesDir = legacyPath + "_workspaces";
			if (QDir(legacyWorkspacesDir).exists())
			{
				const QString targetWorkspacesDir = targetDir + "/workspaces";
				if (!copyRecursive(legacyWorkspacesDir, targetWorkspacesDir))
				{
					r.error = QString("Workspaces copy failed: %1 → %2")
					              .arg(legacyWorkspacesDir)
					              .arg(targetWorkspacesDir);
					BALL::Log.warn() << "ConfigMigration: " << r.error.toStdString() << std::endl;
					// Continue — the main INI did land; workspaces is a
					// best-effort copy.
				}
			}

			r.performed = true;
			BALL::Log.info() << "ConfigMigration: " << legacyPath.toStdString()
			                 << " → " << r.targetPath.toStdString() << std::endl;
			return r;
		}

	} // namespace VIEW
} // namespace BALL
