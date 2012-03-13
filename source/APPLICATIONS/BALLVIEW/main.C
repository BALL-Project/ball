// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

// order of includes is important: first qapplication, than BALL includes
#include <QtGui/QApplication>
#include <BALL/CONFIG/config.h>

#ifdef BALL_HAS_GLEW
#	include <GL/glew.h>
#endif

#include <QtCore/QLocale>
#include <QtCore/QTranslator>

#include <QtGui/QMessageBox>
#include <QtGui/QSplashScreen>
#include <QtOpenGL/qgl.h>

#include "mainframe.h"
#include <BALL/SYSTEM/path.h>
#include <BALL/SYSTEM/directory.h>
#include <BALL/FORMAT/INIFile.h>
#include <BALL/SYSTEM/fileSystem.h>
#include <BALL/COMMON/logStream.h>

#include <BALL/VIEW/KERNEL/UIOperationMode.h>

#include <iostream>

#ifdef Q_WS_X11
#include <X11/Xlib.h>
#endif

void logMessages(QtMsgType type, const char *msg)
{
	BALL::String s(msg);
	if (s.hasPrefix("QTextBrowser")) return;

	switch ( type ) {
		case QtDebugMsg:
				BALL::Log.info() << msg << std::endl;
				break;
		case QtWarningMsg:
				BALL::Log.warn() << msg << std::endl;
				break;
		case QtFatalMsg:
				fprintf( stderr, "Fatal: %s\n", msg );
				abort();                    // deliberately core dump
		case QtCriticalMsg:
				fprintf( stderr, "Critical: %s\n", msg );
				abort();                    // deliberately core dump
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

#ifdef Q_WS_X11
    XInitThreads();
#endif

	qInstallMsgHandler(logMessages);

	// *sigh* this is required as long as Qt does not correctly paint on OpenGL 2 contexts
	QGL::setPreferredPaintEngine(QPaintEngine::OpenGL);

	QApplication application(argc, argv);

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

  QPixmap splash_pm(":BALLView-1.4-Splashscreen.png");
  QSplashScreen* splash = new QSplashScreen(splash_pm);
  splash->show();

	// =============== testing for opengl support ======================================
	if (!QGLFormat::hasOpenGL())
	{
		QMessageBox::critical(0, "Error while starting BALLView", 
				"Your computer has no OpenGL support, please install the correct drivers. Aborting for now...",
				QMessageBox::Ok, Qt::NoButton, Qt::NoButton);
		return -1;
	}

	BALL::String home_dir = BALL::Directory::getUserHomeDir();

	// =============== load translations =====================
	BALL::INIFile f(home_dir + BALL::FileSystem::PATH_SEPARATOR + ".BALLView");
	f.read();

	if (f.hasEntry("GENERAL", "language")) 
	{
		QString str = f.getValue("GENERAL", "language").c_str();

		if (!(str == "Default")) 
		{
			QString loc = "BALLView." + str;

			BALL::Path p;
			QStringList dpaths = QString(p.getDataPath().c_str()).split("\n");

			QTranslator* translator = new QTranslator(&application);
			foreach(QString str, dpaths) 
			{
				translator->load(loc, str + "BALLView/translations");
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

		mainframe.openFile(argument);
	}

	// enable ending of program from python script
	if (mainframe.isAboutToQuit()) 
	{
		mainframe.aboutToExit();
		return 0;
	}
	
	// Remove the splashscreen
	splash->finish(&mainframe);
	delete splash;
	
	// Hand over control to the application.
	return application.exec();
}
