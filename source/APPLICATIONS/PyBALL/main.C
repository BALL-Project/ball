// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

// order of includes is important: first qapplication, than BALL includes
#include <QtWidgets/QApplication>
#include <BALL/CONFIG/config.h>

#include "mainframe.h"
#include <BALL/SYSTEM/path.h>
#include <BALL/SYSTEM/directory.h>

#include <QtWidgets/QMessageBox>
#include <iostream>

void myMessageOutput( QtMsgType type, const char *msg )
{
	switch ( type ) {
		case QtDebugMsg:
				fprintf( stderr, "Debug: %s\n", msg );
				break;
		case QtWarningMsg:
				fprintf( stderr, "Warning: %s\n", msg );
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
int WINAPI WinMain( HINSTANCE, HINSTANCE, PSTR cmd_line, int )
{
	int argc = __argc;
	char** argv = __argv;
#endif

	qInstallMsgHandler(myMessageOutput);

	QApplication application(argc, argv);

	// =============== testing if we can write in current directoy =====================
	BALL::String home_dir = BALL::Directory::getUserHomeDir();

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
	mainframe.setIdentifier("PyBALL");
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

		mainframe.openFile(argument);
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

