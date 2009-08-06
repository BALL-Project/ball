#include <BALL/APPLICATIONS/QuEasyViz/mainWindow.h>
#include <BALL/APPLICATIONS/QuEasyViz/connectionManager.h>
#include <QtGui/QApplication>


using namespace BALL::VIEW;

void set_fpu (unsigned int mode)
{
	asm ("fldcw %0" : : "m" (*&mode));
}


 int main(int argc, char **argv)
 {
	 
	set_fpu (0x27F);  /* enforce IEEE754 double-precision */
	
	QApplication app(argc, argv);
	
	MainWindow mainWin(argv[0]);
	mainWin.show();

	return app.exec();
 }
