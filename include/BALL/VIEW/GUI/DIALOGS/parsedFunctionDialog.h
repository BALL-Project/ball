// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: parsedFunctionDialog.h,v 1.2 2003/08/26 08:04:59 oliver Exp $
//

#ifndef BALL_VIEW_GUI_DIALOGS_PARSEDFUNCTIONDIALOG_H
#define BALL_VIEW_GUI_DIALOGS_PARSEDFUNCTIONDIALOG_H

#include <BALL/VIEW/UIC/parsedFunctionDialogData.h>

namespace BALL
{
	namespace VIEW
	{
		class ParsedFunctionDialog 
			: public ParsedFunctionDialogData
		{ 
				Q_OBJECT

			public:
				ParsedFunctionDialog( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
				~ParsedFunctionDialog();

				virtual void accept();
		};
	}
}

#endif
