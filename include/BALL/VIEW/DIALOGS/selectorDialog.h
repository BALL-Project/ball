// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: selectorDialog.h,v 1.1 2003/12/20 15:28:08 amoll Exp $
//

#ifndef BALL_VIEW_DIALOGS_SELECTORDIALOG_H
#define BALL_VIEW_DIALOGS_SELECTORDIALOG_H

#include <BALL/VIEW/UIC/selectorDialogData.h>
#include <qstatusbar.h> 

namespace BALL
{
	namespace VIEW
	{
		/** Dialog to select atoms acording to a regular expression
				\ingroup ViewDialogs
		*/
		class SelectorDialog 
			: public SelectorDialogData
		{ 
				Q_OBJECT

			public:
				SelectorDialog( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
				~SelectorDialog();
						
			public slots:
				void accept();
				void clearSelection();
		};

} } // namespaces
#endif
