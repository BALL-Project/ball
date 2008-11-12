#include <BALL/APPLICATIONS/QSAR_GUI/mainWindow.h>
#include <BALL/APPLICATIONS/QSAR_GUI/connectionManager.h>
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
	MainWindow mainWin;
	mainWin.show();

	return app.exec();
 }
