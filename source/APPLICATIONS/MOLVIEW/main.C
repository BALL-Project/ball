#include <BALL/common.h>
#include <qapplication.h>

#include "mainframe.h"

using namespace BALL;
using namespace BALL::VIEW;
using namespace BALL::MOLVIEW;


int main(int argc, char **argv)
{
  QApplication application(argc, argv);

	// creating Mainframe
	Mainframe mainframe;
	application.setMainWidget(&mainframe);

	// start the application
	mainframe.show();
  return application.exec();
}
