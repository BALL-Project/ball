// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: runGAMESSDialog.h,v 1.1.2.2 2006/01/27 00:39:31 amoll Exp $
//

#ifndef BALL_VIEW_DIALOGS_RUNGAMESSDIALOG_H
#define BALL_VIEW_DIALOGS_RUNGAMESSDIALOG_H

#ifndef BALL_SYSTEM_PATH_H
# include <BALL/SYSTEM/path.h>
#endif

#ifndef BALL_FORMAT_GAMESSDATFILE_H
# include <BALL/FORMAT/GAMESSDatFile.h>
#endif

#include <BALL/VIEW/UIC/runGAMESSDialogData.h>

namespace BALL
{
	namespace VIEW
	{
		class BALL_VIEW_EXPORT RunGAMESSDialog
			: public QDialog,
				public Ui_RunGAMESSDialogData
		{
			Q_OBJECT

			public:
				RunGAMESSDialog(QWidget* parent = NULL, const char* name = NULL);
				virtual ~RunGAMESSDialog();

			public slots:
				///
				void accept();		

				///
				virtual void saveGAMESSInput();
				
			protected:
				String extractValue_(const QComboBox* box);
				void setParameters_();
				GAMESSDatFile outfile_;
		};
	}
}

#endif // BALL_VIEW_DIALOGS_RUNGAMESSDIALOG_H
