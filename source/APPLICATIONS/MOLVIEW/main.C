// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: main.C,v 1.16 2003/08/26 09:17:36 oliver Exp $
//

// order of includes is important:
#include <qapplication.h>
#include "mainframe.h"
#include <BALL/common.h>



int main(int argc, char **argv)
{
	#ifdef Q_WS_X11
		// doesnt work on some X11 servers
		//XInitThreads();
		//BALL::Log.error() << "Enabling multi threads in Xlib" << std::endl;
	#endif
		
  QApplication application(argc, argv);

	// creating Mainframe
	BALL::Mainframe mainframe;
	application.setMainWidget(&mainframe);

	#ifdef BALL_PYTHON_SUPPORT
		// initialize the Python interface
		mainframe.setIdentifier("MAIN");
		mainframe.registerThis();
	#endif

	// start the application
	mainframe.show();
	for (BALL::Index i = 1; i<argc;i++)
	{
		mainframe.openFile(argv[i]);
	}
  return application.exec();
	
}
