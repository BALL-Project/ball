// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_DIALOGS_STEREOSETTINSGDIALOG_H
#define BALL_VIEW_DIALOGS_STEREOSETTINSGDIALOG_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#ifndef BALL_VIEW_KERNEL_MODULARWIDGET_H
# include <BALL/VIEW/KERNEL/modularWidget.h>
#endif

#include <BALL/VIEW/UIC/ui_stereoSettingsDialog.h>

#include <QtGui/QDialog>

namespace BALL
{
	namespace VIEW
	{
		class Camera;

		/** Dialog to set the focal plane to a given value
				\ingroup ViewDialogs
		*/
		class BALL_VIEW_EXPORT StereoSettingsDialog 
			: public QDialog,
				public Ui_StereoSettingsDialogData,
				public ModularWidget
		{ 
				Q_OBJECT
				BALL_EMBEDDABLE(StereoSettingsDialog, ModularWidget)

			public:
				StereoSettingsDialog(QWidget* parent = 0, const char* name = "StereoSettingsDialog");
				~StereoSettingsDialog();

				/** Initialization. This method is called automatically before the main application is started. 
						It adds the	dialog's menu entries and connections.
				*/
				virtual void initializeWidget(MainControl& main_control);
						
			public slots:
				void okPressed();
				virtual void apply();
				virtual void show();
				void computeSettingsFromModelDistance();
		};

} } // namespaces
#endif
