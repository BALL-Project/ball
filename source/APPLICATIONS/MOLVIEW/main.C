// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: main.C,v 1.19 2003/11/29 14:47:12 oliver Exp $
//

// order of includes is important:
#include <qapplication.h>
#include "mainframe.h"

int main(int argc, char **argv)
{
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
