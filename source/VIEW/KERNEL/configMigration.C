// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/KERNEL/configMigration.h>

#include <BALL/COMMON/logStream.h>

#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QDir>
#include <QtCore/QDirIterator>
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

			if (QFile::exists(r.targetPath))
			{
				// Already migrated on a prior run — idempotent no-op.
				return r;
			}

			// mkpath target directory.
			const QString targetDir = QFileInfo(r.targetPath).absolutePath();
			if (!QDir().mkpath(targetDir))
			{
				r.error = QString("Could not create target directory: %1").arg(targetDir);
				BALL::Log.warn() << "ConfigMigration: " << r.error.toStdString() << std::endl;
				return r;
			}

			// Copy the main INI file.
			if (!QFile::copy(legacyPath, r.targetPath))
			{
				r.error = QString("Copy failed: %1 → %2").arg(legacyPath).arg(r.targetPath);
				BALL::Log.warn() << "ConfigMigration: " << r.error.toStdString() << std::endl;
				return r;
			}

			// Migrate the workspaces/ subdirectory if it exists. Legacy
			// layout convention: ~/.BALLView_workspaces/ as a sibling
			// directory. (BALL pre-v2 didn't actually have user
			// workspaces, but defensive: pick it up if present.)
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

			// Append migration marker.
			if (!appendMigrationMarker(r.targetPath, legacyPath))
			{
				BALL::Log.warn() << "ConfigMigration: could not append migration marker to "
				                 << r.targetPath.toStdString() << std::endl;
				// Don't fail the whole migration over a marker write.
			}

			r.performed = true;
			BALL::Log.info() << "ConfigMigration: " << legacyPath.toStdString()
			                 << " → " << r.targetPath.toStdString() << std::endl;
			return r;
		}

	} // namespace VIEW
} // namespace BALL
