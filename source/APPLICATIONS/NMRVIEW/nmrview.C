#include <BALL/VIEW/GUI/KERNEL/mainControl.h>
#include <BALL/MOLVIEW/GUI/DIALOGS/openBruker2DFile.h>
#include <BALL/VIEW/GUI/WIDGETS/regularData2DWidget.h>

using namespace BALL::VIEW;
using namespace BALL::MOLVIEW;

int main(int argc, char **argv)
{
  // creating mainframe and application
  QApplication application(argc, argv);
  MainControl mainframe;
  application.setMainWidget(&mainframe);

	// create widgets
	mainframe.setCentralWidget(new RegularData2DWidget(4096, 4096, 0.0, 1.0, &mainframe));
	new OpenBruker2DFile(&mainframe);
        
  // start the application
  mainframe.show();
  return application.exec();
}
