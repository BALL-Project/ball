// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_DIALOGS_PTEDIALOG_H
#define BALL_VIEW_DIALOGS_PTEDIALOG_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#include <BALL/VIEW/UIC/ui_PTEDialog.h>

#include <QtWidgets/QDialog>

namespace BALL
{
	namespace VIEW
	{

/** Dialog for choosing the element type for the edit mode of scene.
		\ingroups ViewDialogs
*/
class BALL_VIEW_EXPORT PTEDialog
	: public QDialog,
		public Ui_PTEDialogData
{
	Q_OBJECT

	public:

	///
	PTEDialog(QWidget* parent = 0);

	///
	~PTEDialog();
	
	public slots:
		
	///
	void newElementType(int elementNumber);

	protected slots:

	void elementClicked_();
	void elementChoosen_();

	protected:
		
	QString atomProperties_(int number);
	void connectChilds_(QObject* o);
};


	} // namespaces
}

#endif
