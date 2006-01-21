// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: parsedFunctionDialog.h,v 1.5.2.2 2006/01/21 14:34:02 amoll Exp $
//

#ifndef BALL_VIEW_DIALOGS_PARSEDFUNCTIONDIALOG_H
#define BALL_VIEW_DIALOGS_PARSEDFUNCTIONDIALOG_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#include <BALL/VIEW/UIC/parsedFunctionDialogData.h>

#include <qdialog.h>

namespace BALL
{
	namespace VIEW
	{
		/** Dialog to visualise a parsed function
				\ingroup ViewDialogs
		*/
		class BALL_VIEW_EXPORT ParsedFunctionDialog 
			: public QDialog,
				public Ui_ParsedFunctionDialogData
		{ 
				Q_OBJECT

			public:
				ParsedFunctionDialog( QWidget* parent = 0, const char* name = 0);
				~ParsedFunctionDialog();
		};

} } //namespaces
#endif
