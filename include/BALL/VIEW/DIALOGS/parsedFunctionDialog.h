// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_DIALOGS_PARSEDFUNCTIONDIALOG_H
#define BALL_VIEW_DIALOGS_PARSEDFUNCTIONDIALOG_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#include <BALL/VIEW/UIC/ui_parsedFunctionDialog.h>

#include <QtGui/QDialog>

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
				ParsedFunctionDialog( QWidget* parent = 0, const char* name = "ParsedFunctionDialog");
				~ParsedFunctionDialog();
		};

} } //namespaces
#endif
