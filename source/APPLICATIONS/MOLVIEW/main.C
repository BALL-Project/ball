// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: main.C,v 1.20 2004/03/21 11:48:35 amoll Exp $
//

// order of includes is important: first qapplication, than BALL includes
#include <qapplication.h>
#include "mainframe.h"

#ifndef BALL_PLATFORM_WINDOWS
int main(int argc, char **argv)
#else
int WINAPI WinMain( HINSTANCE, HINSTANCE, PSTR cmd_line, int )
#endif
{

#ifdef BALL_PLATFORM_WINDOWS
	int argc = __argc;
	char** argv = __argv;
#endif
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
