// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: main.C,v 1.7 2004/05/05 18:03:53 amoll Exp $
//

// order of includes is important: first qapplication, than BALL includes
#include <qapplication.h>
#include <qmessagebox.h>

#include "mainframe.h"
#include <BALL/SYSTEM/path.h>
#include <BALL/SYSTEM/directory.h>

#include <iostream>

#ifndef BALL_PLATFORM_WINDOWS
int main(int argc, char **argv)
{
#else
int WINAPI WinMain( HINSTANCE, HINSTANCE, PSTR cmd_line, int )
{
	int argc = __argc;
	char** argv = __argv;
#endif


	QApplication application(argc, argv);

	// =============== looking for the BALL_DATA_PATH variable =========================
	char*	BALLView_data_path = getenv("BALLVIEW_DATA_PATH");
	if (BALLView_data_path != 0)
	{
 		putenv((char*)((BALL::String("BALL_DATA_PATH=") + BALL::String(BALLView_data_path)).c_str()));
	}


	BALL::Path d;
	if (BALL::String(d.find("CHARMM")) == "")
	{
		QMessageBox::critical(0, "Error while starting BALLView",
				QString("Could not find the BALL data directory!\n") + 
				"Please set the BALL_DATA_PATH or BALLVIEW_DATA_PATH\n" + 
				"environment variable to the directory containing the\n" + 
				"BALL or BALLView data directory (e.g. to C:\\BALL\\data).", 
				QMessageBox::Abort,  QMessageBox::NoButton);
		exit(-1);
	}

	// =============== testing if we can write in current directoy =====================
	bool dir_error = false;
	char* home_dir = 0;
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
		// oh, we have a problem, look for the users home dir
		dir_error = true;

		// default for UNIX/LINUX
		home_dir = getenv("HOME");
		if (home_dir == 0) 
		{
			// windows
			home_dir = getenv("HOMEPATH");
		}

		// changedir to the homedir
		if (home_dir != 0)
		{
			BALL::Directory dir(home_dir);
			dir_error = !dir.setCurrent();
		}

		if (dir_error)
		{
			QMessageBox::warning(0, "Error while starting BALLView",
					QString("You dont have write access to the current working directory\n") + 
					"and BALLView can not find your home directory. This can cause\n" + 
					"unexpected behaviour. Please start BALLView from your homedir with\n" + 
					"absolute path (e.g. C:\\Windows\\BALLView\\BALLView).\n");
		}
	}

	// =============== initialize Mainframe ============================================
	// Create the mainframe.
	BALL::Mainframe mainframe;
	application.setMainWidget(&mainframe);

	// if we need to use the users homedir as working dir, do so
	if (home_dir != 0 && !dir_error)
	{
		mainframe.setWorkingDir(home_dir);
	}

	// Register the mainfram (required for Python support).
	mainframe.setIdentifier("MAIN");
	mainframe.registerThis();

	// Show the main window.
	mainframe.show();

	// =============== parsing command line arguments ==================================
	// If there are additional command line arguments, interpret them as files to open or logging flag.
	for (BALL::Index i = 1; i < argc; ++i)
	{
		if (BALL::String(argv[i]) == "-l") 
		{
			mainframe.enableLoggingToFile();
			continue;
		}
		mainframe.openFile(argv[i]);
	}

  // Hand over control to the application.
  return application.exec();
}

