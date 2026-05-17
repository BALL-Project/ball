// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

// order of includes is important: first qapplication, then BALL includes
#include <QtWidgets/QApplication>
#include <QtWidgets/QMessageBox>
#include <QtCore/QTranslator>
#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QSettings>
#include <QtCore/QTimer>
#include <QtGui/QSurfaceFormat>

#include "mainframe.h"
#include <BALL/SYSTEM/path.h>
#include <BALL/SYSTEM/directory.h>
#include <BALL/VIEW/RENDERING/glRenderWindow.h>
#include <BALL/VIEW/WIDGETS/scene.h>

#include <iostream>

// === Phase 999.40: ThemeManager init (BALL_UI_V2 build flag) ===============
// Single neutral theme per maintainer-Q3 — Handover Phase 0.
#  include <BALL/VIEW/KERNEL/theme/themeManager.h>
// === end Phase 999.40 ======================================================

// === Phase 999.45: Workspace consolidation + config migration ==============
// Pre-mainframe migration of legacy ~/.BALLView to platform-correct path
// + post-show first-run prompt asking the user to opt into the new
// Default workspace or keep their legacy 5-dock layout. .pre-v2.bak
// auto-backup written before either step touches anything.
// (Phase 999.49 retired the named "Classic" preset; the "Keep my layout"
// branch now just leaves Mainframe's pre-Default 5-dock construction
// in place and persists currentPreset=UserDefined.)
#  include <BALL/VIEW/KERNEL/configMigration.h>
#  include <BALL/VIEW/KERNEL/workspaceManager.h>
// === end Phase 999.45 ======================================================

// === Phase 999.8-full: Sparkle + WinSparkle auto-update ====================
// Re-applied 2026-05-17 after concurrent index-staging with Phase 999.40
// dropped these hunks from commit 75347315a4. See 999.8-SPIKE.md for design
// + ED25519-KEY-RUNBOOK.md for production-key procurement.
//
// macOS: extern "C" entry point exposed by sparkle_bridge.mm (.mm compiled
//        as Objective-C++). When BALL_HAS_SPARKLE is OFF the .mm compiles
//        to a no-op stub so the symbol exists either way and main.C does
//        not need a CMake-time conditional around the call site.
// Windows: WinSparkle C API called directly. BALL_HAS_WINSPARKLE gates the
//        #include + call at build time.
#ifdef Q_OS_MACOS
extern "C" void ball_sparkle_init(void);
#endif
#ifdef BALL_HAS_WINSPARKLE
#  include <winsparkle.h>
#endif
// === end Phase 999.8-full ==================================================

void logMessages(QtMsgType type, const QMessageLogContext& context, const QString& message)
{
	BALL::String s(message.toStdString());
	if (s.hasPrefix("QTextBrowser")) return;

	switch ( type ) {
		case QtDebugMsg:
            BALL::Log.info() << message.toStdString() << " " << (context.file ? context.file : "(unknown context)")
                                                      << " " << context.line
                                                      << " " << (context.function ? context.function : "(unknown function)") << std::endl;
				break;
		case QtWarningMsg:
            BALL::Log.warn() << message.toStdString() << " " << (context.file ? context.file : "(unknown context)")
                                                      << " " << context.line
                                                      << " " << (context.function ? context.function : "(unknown function)") << std::endl;
				break;
		case QtFatalMsg:
				fprintf( stderr, "Fatal: %s\n", message.toLatin1().constData() );
				abort();                    // deliberately core dump
		case QtCriticalMsg:
				fprintf( stderr, "Critical: %s\n", message.toLatin1().constData() );
				abort();                    // deliberately core dump
		default:
			break;
	}
}


// uncomment this to use debugging to std::cout!
//#undef BALL_OS_WINDOWS

#ifndef BALL_OS_WINDOWS
int main(int argc, char **argv)
{
#else
int WINAPI WinMain(HINSTANCE, HINSTANCE, PSTR cmd_line, int)
{
	int argc = __argc;
	char** argv = __argv;
#endif

	qInstallMessageHandler(logMessages);

	// QOpenGLWidget: all GL contexts must share (display lists / textures across
	// multiple scene windows) and inherit the compatibility-profile 2.1 format.
	// Both must be set BEFORE the QApplication is constructed.
	QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
	QSurfaceFormat::setDefaultFormat(BALL::VIEW::GLRenderWindow::gl_format_);

	QApplication application(argc, argv);

	// === Phase 999.40: ThemeManager init (BALL_UI_V2 build flag) ===========
	// Apply the neutral QSS stylesheet immediately after QApplication ctor
	// and BEFORE any widgets are constructed so the first paint is themed.
	// Single neutral theme per maintainer-Q3 — Handover Phase 0.
	// No-op when BALL_UI_V2 is OFF (the default in v1.7).
	BALL::VIEW::ThemeManager::instance().init(&application);
	// === end Phase 999.40 ==================================================

#ifdef Q_OS_MACOS
	// Resolve BALL_DATA_PATH for the macOS .app bundle.
	//
	// The MacOSXBundleInfo.plist.in template sets LSEnvironment.BALL_DATA_PATH
	// to "BALLView.app/Contents/Resources/data" — a RELATIVE path. macOS
	// Launch Services consults LSEnvironment when the app is launched via
	// double-click, but it passes the value through literally; there's no
	// path-substitution mechanism. The user's cwd at that point is /, so
	// "BALLView.app/Contents/Resources/data" resolves to nothing real and
	// BALL::Path can't find the bundled data tree.
	//
	// Fix: override the env var here with the absolute Resources/data path
	// computed from the executable location. Runs BEFORE any BALL include
	// touches getenv("BALL_DATA_PATH") (BALL::Path consults it lazily on
	// first use, which is after this point). Works for double-click launches,
	// drag-into-Applications, /Applications, ~/Downloads — wherever the .app
	// lives. Shell-launched runs (BALLView.app/Contents/MacOS/BALLView from a
	// terminal) also benefit: this overrides any pre-set BALL_DATA_PATH only
	// when the bundled data/ exists, so a developer's manually-set value to
	// a source-tree data/ dir is preserved (we use qputenv unconditionally
	// when the bundled tree is present — choose: prefer the bundle's data
	// over any env var, OR respect the env var if already set).
	{
		QString data_path = QDir::cleanPath(
			QCoreApplication::applicationDirPath() + "/../Resources/data");
		if (QDir(data_path).exists())
		{
			qputenv("BALL_DATA_PATH", data_path.toLocal8Bit());
		}
	}
#endif

	QStringList arguments = application.arguments();
	QStringList::const_iterator arg_it;

	bool kiosk_mode = false;
	for (arg_it = arguments.constBegin(); arg_it != arguments.constEnd(); ++arg_it)
	{
		if (arg_it->toLocal8Bit() == "-kiosk")
		{
			kiosk_mode = true;
		}
	}

	if (kiosk_mode)
	{
		BALL::VIEW::UIOperationMode::instance().setMode(BALL::VIEW::UIOperationMode::MODE_KIOSK);
	}

	// =============== testing for opengl support ======================================
	// QGLFormat::hasOpenGL() is removed with QGLWidget. QOpenGLWidget negotiates the
	// context itself and reports failure at widget-creation time; an early static probe
	// is no longer available nor necessary.

	BALL::String home_dir = BALL::Directory::getUserHomeDir();

	// =============== load translations =====================
	BALL::INIFile f(home_dir + BALL::FileSystem::PATH_SEPARATOR + ".BALLView");
	f.read();

	if (f.hasEntry("GENERAL", "language")) 
	{
		QString str = f.getValue("GENERAL", "language").c_str();

		if (str != "en_US")
		{
			QString loc = "BALLView-" + str;

			BALL::Path p;
			QStringList dpaths = QString(p.getDataPath().c_str()).split("\n");

			QTranslator* translator = new QTranslator(&application);
			Q_FOREACH(QString s, dpaths)
			{
				// Fold load() result into the guard — Qt 6 marks
				// QTranslator::load() [[nodiscard]]; relying on isEmpty()
				// alone discards the bool indicating load success.
				if (translator->load(loc, s + "BALLView/translations") && !translator->isEmpty())
				{
					QCoreApplication::installTranslator(translator);
					break;
				}
			}
		}
	}

	// =============== testing if we can write in current directory =====================
	if (home_dir == "")
	{
		try
		{
			BALL::String temp_file_name;
			BALL::File::createTemporaryFilename(temp_file_name);
			BALL::File out(temp_file_name, std::ios::out);
			out << "test" << std::endl;
			out.remove();
		}
		catch(...)
		{
			QMessageBox::warning(0, "Error while starting BALLView",
					QString("You dont have write access to the current working directory\n") + 
					"and BALLView can not find your home directory. This can cause\n" + 
					"unexpected behaviour. Please start BALLView from your homedir with\n" + 
					"absolute path (e.g. C:\\Programs\\BALLView\\BALLView).\n");
		}
	}

	// =============== Phase 999.45: pre-mainframe config migration =====================
	// Run the legacy ~/.BALLView → platform-correct standard-paths
	// migration once, BEFORE constructing Mainframe (which loads
	// preferences from the legacy file via MainControl). Also write
	// a .pre-v2.bak auto-backup of the legacy file so even a failed
	// migration cannot corrupt the user's existing preferences.
	//
	// All gated on BALL_UI_V2 — pre-flag-flip builds keep the legacy
	// ~/.BALLView path verbatim.
	{
		const QString legacy_path = QString::fromStdString(home_dir.c_str()) + "/.BALLView";
		const QString backup_path = QString::fromStdString(home_dir.c_str()) + "/.BALLView.pre-v2.bak";
		const QString target_path = BALL::VIEW::ConfigMigration::targetConfigPath();

		// Step 1: auto-backup if legacy exists AND target doesn't (i.e.
		// we are about to migrate). Don't clobber an existing .bak —
		// user may have multiple v2-upgrade attempts.
		if (QFile::exists(legacy_path) && !QFile::exists(target_path) && !QFile::exists(backup_path))
		{
			if (!QFile::copy(legacy_path, backup_path))
			{
				BALL::Log.warn() << "Phase 999.45: could not write pre-v2 backup to "
				                 << backup_path.toStdString() << std::endl;
			}
		}

		// Step 2: run migration. Idempotent — no-op if target exists.
		const BALL::VIEW::MigrationResult mres = BALL::VIEW::ConfigMigration::run(legacy_path);
		if (mres.performed)
		{
			BALL::Log.info() << "Phase 999.45: migrated " << mres.sourcePath.toStdString()
			                 << " → " << mres.targetPath.toStdString() << std::endl;
		}
	}
	// =============== end Phase 999.45 pre-mainframe migration =========================

	// =============== initialize Mainframe ============================================
	// Create the mainframe.
	BALL::Mainframe mainframe(0, "Mainframe");

	// can we use the users homedir as working dir?
	if (home_dir != "")
	{
		mainframe.setWorkingDir(home_dir);
	}

	// Register the mainfram (required for Python support).
	mainframe.setIdentifier("Mainframe");
	mainframe.registerThis();

	// Show the main window.
	mainframe.show();

	// =============== Phase 999.45: first-run workspace prompt =========================
	// After Mainframe is constructed AND shown (so the user sees the
	// new Default workspace as the backdrop for the choice), prompt
	// the user once to opt into the new layout or keep their legacy
	// 5-dock layout. (Phase 999.49: the named "Classic" preset is
	// retired; the keep-legacy branch persists currentPreset=UserDefined
	// rather than naming a retired built-in preset.)
	//
	// Persisted via QSettings under [Workspace]/firstRunPromptSeen.
	// Only fires when:
	//   1. A migration actually performed (i.e. user had a legacy
	//      ~/.BALLView — fresh installs get Default silently), AND
	//   2. The prompt has not been answered before.
	{
		const QString target_path = BALL::VIEW::ConfigMigration::targetConfigPath();
		QSettings prefs(target_path, QSettings::IniFormat);

		// === Phase 999.49: legacy preset auto-migration =====================
		// The 5-dock preset was retired in v1.7 (maintainer-Q2 commitment).
		// Users carrying [Workspace]/currentPreset="Classic" from a v1.6.x
		// install — or from a v1.7-pre install that exercised the
		// Phase 999.45 first-run "Keep my layout" branch under the old
		// labelling — get silently remapped to "Default" here, BEFORE
		// any downstream code path tries to look up the (now-deleted)
		// enum value via a QString round-trip.
		//
		// User-saved presets ([Workspace]/userDefined__*) are untouched —
		// only the canonical built-in name is rewritten.
		//
		// Idempotent: subsequent launches read "Default" and no-op the
		// remap. RELEASE-NOTES-v1.7.md documents the change.
		if (prefs.value("Workspace/currentPreset").toString() == QLatin1String("Classic"))
		{
			prefs.setValue("Workspace/currentPreset", "Default");
			prefs.sync();
			BALL::Log.info() << "v1.7 migration: legacy 5-dock workspace preset retired — falling back to Default. (User-defined presets are unaffected.)" << std::endl;
		}
		// === end Phase 999.49 auto-migration ================================

		const bool prompt_seen = prefs.value("Workspace/firstRunPromptSeen", false).toBool();
		const bool migrated = prefs.contains("Migration/migrated_from");

		if (migrated && !prompt_seen)
		{
			// Phase 999.49 simplified this prompt: the original two-button
			// "Try new / Keep my layout (Classic)" choice no longer makes
			// sense once the Classic preset is deleted — there's no
			// restorable named preset to "keep" anymore. Surface the
			// change as a one-button informational notice; users who
			// want to rearrange docks can do so via View → Workspace
			// → Save current as… afterwards.
			QMessageBox box(&mainframe);
			box.setIcon(QMessageBox::Information);
			box.setWindowTitle(QCoreApplication::translate("Phase999_49", "Refreshed workspace"));
			box.setText(QCoreApplication::translate("Phase999_49",
			    "BALLView's default workspace has been refreshed in v1.7."));
			box.setInformativeText(QCoreApplication::translate("Phase999_49",
			    "The legacy 5-dock layout has been retired. You can switch "
			    "between Default and Focused presets, or save your own, "
			    "from View → Workspace."));
			box.addButton(
			    QCoreApplication::translate("Phase999_49", "Got it"),
			    QMessageBox::AcceptRole);
			box.exec();

			// Default preset is already applied by Mainframe::show().
			prefs.setValue("Workspace/currentPreset", "Default");
			prefs.setValue("Workspace/firstRunPromptSeen", true);
			prefs.sync();
		}
	}
	// =============== end Phase 999.45 first-run prompt ================================

	// === Phase 999.8-full: Sparkle + WinSparkle init =======================
	// Both init after mainframe.show() per spike caveat — Sparkle/WinSparkle
	// expect the main window to exist before they wire their update-check UI.
	// Both kick a background update check that surfaces UI only if a newer
	// release is available; quiet startup on the happy path.
#ifdef Q_OS_MACOS
	ball_sparkle_init();
#endif
#ifdef BALL_HAS_WINSPARKLE
	win_sparkle_set_appcast_url("https://ball-project.github.io/ball/appcast-windows.xml");
	// EdDSA Ed25519 public key registered as a GitHub secret + injected via
	// release.yml. See ED25519-KEY-RUNBOOK.md for production-key procurement.
	win_sparkle_init();
	// Cleanup at app exit — paired with init above.
	QObject::connect(&application, &QCoreApplication::aboutToQuit,
	                 [] { win_sparkle_cleanup(); });
#endif
	// === end Phase 999.8-full ==============================================

	// =============== parsing command line arguments ==================================
	// If there are additional command line arguments, interpret them as files to open or logging flag.
	//
	// -export-png <path> : headless render smoke-check trigger (Phase 02.2, DIAG-01
	//   smoke check). After the molecule(s) given on the command line have loaded and
	//   the Scene has had a few paint cycles, the Scene's existing exportPNG() path is
	//   invoked to write a PNG of the rendered scene, and BALLView quits. This is the
	//   non-invasive hook the render-smoke-check.sh script uses on every CI runner —
	//   it reuses Scene::exportPNG(), it does not build a new GL test harness.
	BALL::String export_png_path;
	for (BALL::Index i = 1; i < argc; ++i)
	{
		BALL::String argument(argv[i]);
		if (argument == "-l")
		{
			mainframe.enableLoggingToFile();
			continue;
		}
		else if (argument == "-kiosk")
		{
			// the kiosk mode has already been handled
			continue;
		}
		else if (argument == "-export-png")
		{
			if (i + 1 < argc)
			{
				export_png_path = BALL::String(argv[++i]);
			}
			else
			{
				std::cerr << "BALLVIEW_SMOKE_ERROR -export-png requires a path argument" << std::endl;
				return 2;
			}
			continue;
		}

		mainframe.openFile(argument);
	}

	// If -export-png was requested, schedule the headless export + quit once the
	// event loop is running and the scene has had time to load and render.
	if (export_png_path != "")
	{
		QTimer::singleShot(4000, &application, [&export_png_path, &application]() {
			BALL::VIEW::Scene* scene = BALL::VIEW::Scene::getInstance(0);
			if (scene == 0)
			{
				std::cerr << "BALLVIEW_SMOKE_ERROR no Scene instance available for -export-png" << std::endl;
				application.exit(3);
				return;
			}
			bool ok = scene->exportPNG(export_png_path);
			std::cout << "BALLVIEW_SMOKE_EXPORT path=\"" << export_png_path << "\" ok="
			          << (ok ? "1" : "0") << std::endl;
			application.exit(ok ? 0 : 4);
		});
	}

	// enable ending of program from python script
	if (mainframe.isAboutToQuit()) 
	{
		mainframe.aboutToExit();
		return 0;
	}
	
	// Hand over control to the application.
	return application.exec();
}
