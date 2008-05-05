// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: parsedFunctionDialog.h,v 1.5.16.1 2007/03/25 21:25:53 oliver Exp $
//

#ifndef BALL_VIEW_DIALOGS_PARSEDFUNCTIONDIALOG_H
#define BALL_VIEW_DIALOGS_PARSEDFUNCTIONDIALOG_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#ifdef BALL_COMPILER_MSVC
# include <BALL/VIEW/UIC/ui_parsedFunctionDialog.h>
#else
# include <BALL/VIEW/UIC/parsedFunctionDialogData.h>
#endif


#include <QtGui/qdialog.h>

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
