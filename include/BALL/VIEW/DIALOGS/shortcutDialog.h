// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_DIALOGS_SHORTCUTDIALOG_H
#define BALL_VIEW_DIALOGS_SHORTCUTDIALOG_H

#ifndef BALL_VIEW_KERNEL_PREFERENCESENTRY
# include <BALL/VIEW/KERNEL/preferencesEntry.h>
#endif

#ifndef BALL_VIEW_KERNEL_SHORTCUTREGISTRY_H
# include <BALL/VIEW/KERNEL/shortcutRegistry.h>
#endif

#ifndef BALL_VIEW_KERNEL_MODULARWIDGET_H
# include <BALL/VIEW/KERNEL/modularWidget.h>
#endif

#include <BALL/VIEW/UIC/ui_shortcutDialog.h>

namespace BALL
{
	namespace VIEW
	{
		class MainControl;

		/** Dialog for changing the BALLView shortcuts
				\ingroup ViewDialogs
		*/
		class BALL_VIEW_EXPORT ShortcutDialog
			: public QWidget,
				public Ui_ShortcutDialogData,
				public ModularWidget,
				public PreferencesEntry
		{
			// macro needed for Qt's slot mechanism:
			Q_OBJECT

			BALL_EMBEDDABLE(ShortcutDialog,ModularWidget)
			
			public:

			/// Constructor
			ShortcutDialog(QWidget* parent = NULL, const char* name = "ShortcutDialog", Qt::WFlags fl = 0 );

			/// Destructor
			virtual ~ShortcutDialog();

			
			/** Initialization. 
			 		This method is called automatically before the main 
					application is started. 
					It adds the	dialog's menu entries and connections.
			*/
			virtual void initializeWidget(MainControl& main_control);

			public slots:
				virtual void searchTextChanged(QString filter);

			protected slots:
				virtual void browseImportFile_();
				virtual void browseExportFile_();

				virtual void shortcutChanged_();

				void loadPredefinedShortcuts_(QString entry);
				void loadShortcutsFromFile_(const String& filename);
		};

	}
}

#endif //BALL_VIEW_DIALOGS_SHORTCUTDIALOG_H

