// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: main.C,v 1.18 2003/09/19 18:43:50 amoll Exp $
//

// order of includes is important:
#include <qapplication.h>
#include "mainframe.h"

int main(int argc, char **argv)
{
	#ifdef Q_WS_X11
		// doesnt work on some X11 servers
		//XInitThreads();
		//BALL::Log.error() << "Enabling multi threads in Xlib" << std::endl;
	#endif
		
  QApplication application(argc, argv);

	// Create the mainframe.
	BALL::Mainframe mainframe;
	application.setMainWidget(&mainframe);

	#ifdef BALL_PYTHON_SUPPORT
		// initialize the Python interface
		mainframe.setIdentifier("MAIN");
		mainframe.registerThis();
	#endif

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
