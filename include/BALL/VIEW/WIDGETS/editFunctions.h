// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_WIDGETS_EDITFUNCTIONS_H
#define BALL_VIEW_WIDGETS_EDITFUNCTIONS_H

#ifndef BALL_VIEW_WIDGETS_DOCKWIDGET_H
#	include <BALL/VIEW/WIDGETS/dockWidget.h>
#endif

#ifndef BALL_VIEW_DIALOGS_PTEDIALOG_H
#	include <BALL/VIEW/DIALOGS/PTEDialog.h>
#endif

#ifndef BALL_VIEW_DIALOGS_EDITOPERATIONDIALOG_H
#	include <BALL/VIEW/DIALOGS/editOperationDialog.h>
#endif

#include <QtGui/QTabWidget>
namespace BALL
{
	namespace VIEW
	{
		/** EditFunctions class.
		 		This DockWidget contains useful functions for the edit mode of the 
				\link BALL::Scene Scene. \endlink
		 		\ingroups ViewWidgets
		 */
		class BALL_VIEW_EXPORT EditFunctions
			: public DockWidget
		{
			Q_OBJECT

			public:

//			BALL_EMBEDDABLE(DockWidget)

				BALL_EMBEDDABLE(EditFunctions, DockWidget)

				EditFunctions(QWidget* parent = 0, const char *name = 0);

				~EditFunctions();

				/**	Setup the menu entry in "Edit->Clear Logs".
				*/
				virtual void initializeWidget(MainControl& main_control);

				/**	Remove menu entries.
				*/
				virtual void finalizeWidget(MainControl& main_control);

			protected:
				PTEDialog pte_;
				EditOperationDialog edit_operations_;
				QTabWidget tab_;
		};
	}
}

#endif
