// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_DIALOGS_RUNGAMESSDIALOG_H
#define BALL_VIEW_DIALOGS_RUNGAMESSDIALOG_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#ifndef BALL_SYSTEM_PATH_H
# include <BALL/SYSTEM/path.h>
#endif

#ifndef BALL_FORMAT_GAMESSDATFILE_H
# include <BALL/FORMAT/GAMESSDatFile.h>
#endif

#include <BALL/VIEW/UIC/ui_runGAMESSDialog.h>

namespace BALL
{
	namespace VIEW
	{
		/**
			\ingroup ViewDialogs
		*/
		class BALL_VIEW_EXPORT RunGAMESSDialog
			: public QDialog,
				public Ui_RunGAMESSDialogData
		{
			Q_OBJECT

			public:
				///
				RunGAMESSDialog(QWidget* parent = NULL, const char* name = "RunGAMESSDialog");
				///
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
