#include <BALL/APPLICATIONS/QSAR_GUI/mainWindow.h>
#include <BALL/APPLICATIONS/QSAR_GUI/connectionManager.h>
#include <QtGui/QApplication>


using namespace BALL::VIEW;

 int main(int argc, char **argv)
 {
	QApplication app(argc, argv);
	MainWindow mainWin;
	mainWin.show();

	return app.exec();
 }
