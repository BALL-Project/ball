// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: main.C,v 1.3 2004/04/30 14:09:34 amoll Exp $
//

// order of includes is important: first qapplication, than BALL includes
#include <qapplication.h>
#include "mainframe.h"
#include <BALL/SYSTEM/path.h>
#include <iostream>
#include <qmessagebox.h>

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

	// Create the mainframe.
	BALL::Mainframe mainframe;
	application.setMainWidget(&mainframe);

	// Register the mainfram (required for Python support).
	mainframe.setIdentifier("MAIN");
	mainframe.registerThis();

	// Show the main window.
	mainframe.show();

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
