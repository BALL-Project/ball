/* main.C
 * 
 * Copyright (C) 2009 Marcel Schumann
 * 
 * This file is part of QuEasy -- A Toolbox for Automated QSAR Model
 * Construction and Validation.
 * QuEasy is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 * 
 * QuEasy is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

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
