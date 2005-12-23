// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: parsedFunctionDialog.h,v 1.5 2005/12/23 17:02:12 amoll Exp $
//

#ifndef BALL_VIEW_DIALOGS_PARSEDFUNCTIONDIALOG_H
#define BALL_VIEW_DIALOGS_PARSEDFUNCTIONDIALOG_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#include <BALL/VIEW/UIC/parsedFunctionDialogData.h>

namespace BALL
{
	namespace VIEW
	{
		/** Dialog to visualise a parsed function
				\ingroup ViewDialogs
		*/
		class BALL_VIEW_EXPORT ParsedFunctionDialog 
			: public ParsedFunctionDialogData
		{ 
				Q_OBJECT

			public:
				ParsedFunctionDialog( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
				~ParsedFunctionDialog();

				virtual void accept();
		};

} } //namespaces
#endif
