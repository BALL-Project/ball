#include <BALL/common.h>
#include <qapplication.h>

#include "mainframe.h"

int main(int argc, char **argv)
{
  QApplication application(argc, argv);

	// creating Mainframe
	Mainframe mainframe;
	application.setMainWidget(&mainframe);

	#ifdef BALL_PYTHON_SUPPORT
	// initialize the Python interface
	mainframe.setIdentifier("MAIN");
	mainframe.registerThis();
	#endif

	// start the application
	mainframe.show();
  return application.exec();
}
