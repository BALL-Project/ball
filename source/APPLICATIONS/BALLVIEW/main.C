// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: main.C,v 1.1 2004/04/16 11:24:47 amoll Exp $
//

// order of includes is important: first qapplication, than BALL includes
#include <qapplication.h>
#include "mainframe.h"
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


	char*	BALLView_data_path = getenv("BALLVIEW_DATA_PATH");
	if (BALLView_data_path == 0)
	{
#ifdef BALL_PLATFORM_WINDOWS
		char*	ball_data_path = getenv("BALL_DATA_PATH");
		if (ball_data_path == 0)
		{
			std::cerr << "Warning: Neither BALL_DATA_PATH nor BALLVIEW_DATA_PATH environment variable are set." << std::endl;
		}
#endif
	}
	else
	{
 		putenv((char*)((BALL::String("BALL_DATA_PATH=") + BALL::String(BALLView_data_path)).c_str()));
	}

	QApplication application(argc, argv);

	// Create the mainframe.
	BALL::Mainframe mainframe;
	application.setMainWidget(&mainframe);

	// Register the mainfram (required for Python support).
	mainframe.setIdentifier("MAIN");
	mainframe.registerThis();

	// Show the main window.
	mainframe.show();

	// If there are additional command line arguments, interpret
	// them as files to open.
	for (BALL::Index i = 1; i < argc; ++i)
	{
		mainframe.openFile(argv[i]);
	}

    // Hand over control to the application.
    return application.exec();
}
