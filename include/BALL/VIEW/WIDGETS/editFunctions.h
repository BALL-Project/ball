// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: editFunctions.h,v 1.1 2005/02/07 17:46:52 anne Exp $
//

#ifndef BALL_VIEW_WIDGETS_EDITFUNCTIONS_H
#define BALL_VIEW_WIDGETS_EDITFUNCTIONS_H

#ifndef BALL_VIEW_WIDGETS_DOCKWIDGET_H
#	include <BALL/VIEW/WIDGETS/dockWidget.h>
#endif

#ifndef BALL_VIEW_DIALOGS_PTEDIALOG_H
#	include <BALL/VIEW/DIALOGS/PTEDialog.h>
#endif

#include <qtabwidget.h>
namespace BALL
{
	namespace VIEW
	{
		/** EditFunctions class.
		 		This DockWidget contains useful functions for the edit mode of the 
				\link BALL::EditableScene EditableScene. \endlink
		 		\ingroups ViewWidgets
		 */
		class BALL_EXPORT EditFunctions
			: public DockWidget
		{
			Q_OBJECT

			public:

//			BALL_EMBEDDABLE(DockWidget)

				EditFunctions(QWidget* parent = 0, const char *name = 0)
					throw();

				~EditFunctions()
					throw();

				/**	Setup the menu entry in "Edit->Clear Logs".
				*/
				virtual void initializeWidget(MainControl& main_control)
					throw();

				/**	Remove menu entries.
				*/
				virtual void finalizeWidget(MainControl& main_control)
					throw();

			protected:
				PTEDialog pte_;
				QTabWidget tab_;
		};
	}
}

#endif
