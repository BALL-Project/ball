/****************************************************************************
** Form interface generated from reading ui file 'colormeshdialog.ui'
**
** Created: Tue Sep 24 17:27:55 2002
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef COLORMESHDIALOGDATA_H
#define COLORMESHDIALOGDATA_H

#include <qvariant.h>
#include <qdialog.h>
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QFrame;
class QLabel;
class QLineEdit;
class QPushButton;

namespace BALL
{
	namespace VIEW
	{

		class colorMeshDialogData : public QDialog
		{ 
			Q_OBJECT

			public:
				colorMeshDialogData( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
				~colorMeshDialogData();

				QLabel* TextLabel1;
				QLineEdit* Loadfile_;
				QPushButton* Browseload;
				QPushButton* Compute_;
				QPushButton* Cancel_;
				QFrame* Line3;


		};

	}
}
#endif // COLORMESHDIALOGDATA_H
