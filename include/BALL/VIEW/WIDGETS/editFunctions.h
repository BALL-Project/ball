// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: editFunctions.h,v 1.3 2005/11/04 17:57:48 anne Exp $
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

				BALL_EMBEDDABLE(EditFunctions, DockWidget)

				EditFunctions(QWidget* parent = 0, const char *name = 0)
					throw();

				~EditFunctions()
					throw();

			protected:
				PTEDialog pte_;
				EditOperationDialog edit_operations_;
				QTabWidget tab_;
		};
	}
}

#endif
