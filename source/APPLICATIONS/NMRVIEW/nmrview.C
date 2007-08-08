// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: nmrview.C,v 1.10.20.1 2007/08/08 09:38:45 bertsch Exp $
//

#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/DIALOGS/openBruker2DFile.h>
#include <BALL/VIEW/WIDGETS/regularData2DWidget.h>

using namespace BALL::VIEW;

int main(int argc, char **argv)
{
  // creating mainframe and application
  QApplication application(argc, argv);

  MainControl mainframe;

	// create widgets
	mainframe.setCentralWidget(new RegularData2DWidget(0, &mainframe));

	new OpenBruker2DFile(&mainframe);
        
  // start the application
  mainframe.show();

  return application.exec();
}
