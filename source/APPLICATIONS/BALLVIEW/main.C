// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

// order of includes is important: first qapplication, then BALL includes
#include <QtWidgets/QApplication>
#include <QtWidgets/QMessageBox>
#include <QtCore/QTranslator>
#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QTimer>
#include <QtGui/QSurfaceFormat>

#include "mainframe.h"
#include <BALL/SYSTEM/path.h>
#include <BALL/SYSTEM/directory.h>
#include <BALL/VIEW/RENDERING/glRenderWindow.h>
#include <BALL/VIEW/WIDGETS/scene.h>

#include <iostream>

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
				translator->load(loc, s + "BALLView/translations");
				if (!translator->isEmpty())
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
