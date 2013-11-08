
#include <QtGui/QApplication>
#include <BALL/COMMON/global.h>

#include <mainWindow.h>
#include <connectionManager.h>

#ifdef BALL_COMPILER_MSVC
#define WINDOWS_LEAN_AND_MEAN
#include <windows.h>
#endif

using namespace BALL::VIEW;

void set_fpu (unsigned int mode)
{
#if defined(BALL_COMPILER_GXX) ||defined(BALL_COMPILER_LLVM)
	asm ("fldcw %0" : : "m" (*&mode));
#elif defined(BALL_COMPILER_MSVC)
	// TODO: implement!
	//__asm ("fldcw %0" : : "m" (*&mode));
#endif
}


#ifndef BALL_OS_WINDOWS
int main(int argc, char **argv)
{
#else
int WINAPI WinMain(HINSTANCE, HINSTANCE, PSTR cmd_line, int)
{
	int argc = __argc;
	char** argv = __argv;
#endif
	 
	set_fpu (0x27F);  /* enforce IEEE754 double-precision */
	
	QApplication app(argc, argv);
	
	MainWindow mainWin(argv[0]);
	mainWin.show();

	return app.exec();
 }
