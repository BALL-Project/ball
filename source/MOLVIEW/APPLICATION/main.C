#include <qapplication.h>

#include <BALL/common.h>

#include <BALL/VIEW/OPENGL/KERNEL/server.h>

#include <BALL/MOLVIEW/APPLICATION/mainframe.h>
#include <BALL/MOLVIEW/OPENGL/KERNEL/moleculeObjectCreator.h>



using namespace BALL;
using namespace BALL::VIEW;
using namespace BALL::MOLVIEW;


int main(int argc, char **argv)
{
	Log.insert(cout);
	
  QApplication __QApplication(argc, argv);

	// creating Mainframe
	Mainframe mainframe;
	
	mainframe.resize(640, 480);

  // creating server
	//  Server server;

	// registering scene for creation
	//  server.registerCreationScene(mainframe.getScene());

	// registering object generator
	//	MoleculeObjectCreator object_creator;
	//	server.registerObjectCreator(object_creator);

	// registering object processor
	//	server.registerObjectProcessor(mainframe.getObjectProcessor());

	// activate the server and wait for client to connect
	//  server.activate();

	// activate Mainframe
	__QApplication.setMainWidget(&mainframe);
	mainframe.show();

	// start the application
  return __QApplication.exec();
}


