#include <qapplication.h>

#include <BALL/common.h>

#include <BALL/VIEW/OPENGL/KERNEL/server.h>

#include "mainframe.h"
#include <BALL/MOLVIEW/OPENGL/KERNEL/moleculeObjectCreator.h>

using namespace BALL;
using namespace BALL::VIEW;
using namespace BALL::MOLVIEW;


int main(int argc, char **argv)
{
  QApplication application(argc, argv);

	// creating Mainframe
	Mainframe mainframe;
	application.setMainWidget(&mainframe);
	
  // creating server
  Server server;

	// connecting server with mainframe
	NotificationRegister(server, mainframe);

	// registering scene for creation
  server.registerCreationScene(mainframe.getScene());

	// registering object generator
	MoleculeObjectCreator object_creator;
	server.registerObjectCreator(object_creator);

	// registering object processor
	server.registerObjectProcessor(mainframe.getObjectProcessor());

	// activate the server and wait for client to connect
  server.activate();

	// start the application
	mainframe.show();
  return application.exec();
}


