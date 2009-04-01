// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
//

#ifndef BALL_VIEW_DIALOGS_EDITSINGLESHORTCUT_H
#define BALL_VIEW_DIALOGS_EDITSINGLESHORTCUT_H

#ifndef BALL_VIEW_KERNEL_PREFERENCESENTRY
# include <BALL/VIEW/KERNEL/preferencesEntry.h>
#endif

#ifndef BALL_VIEW_KERNEL_SHORTCUTREGISTRY_H
# include <BALL/VIEW/KERNEL/shortcutRegistry.h>
#endif

#ifndef BALL_VIEW_KERNEL_MAINCONTROL_H
# include <BALL/VIEW/KERNEL/mainControl.h>
#endif

#ifndef BALL_VIEW_KERNEL_MODULARWIDGET_H
# include <BALL/VIEW/KERNEL/modularWidget.h>
#endif

#ifdef BALL_COMPILER_MSVC
# include <BALL/VIEW/UIC/ui_editSingleShortcut.h>
#else
# include <BALL/VIEW/UIC/editSingleShortcutData.h>
#endif

namespace BALL
{
	namespace VIEW
	{
		//class MolecularStructure;

		/** Widget for customizing BALLView shortcuts 
				\ingroup ViewWidgets
		*/
		class BALL_VIEW_EXPORT EditSingleShortcut
			: public QWidget,
				public Ui_EditSingleShortcutData,
				public ModularWidget,
				public PreferencesEntry
		{
			// macro needed for Qt's slot mechanism:
			Q_OBJECT
			
			public:

			/// Constructor
			EditSingleShortcut(QWidget* parent = NULL, const char* name = "EditSingleShortcut", Qt::WFlags fl = 0 );

			/// Destructor
			virtual ~EditSingleShortcut();
			
			///
			virtual void initializeWidget(MainControl& main_control);

			public slots:
				///
				void accepted();

				///
				void rejected(); 

				///
				void setShortcutText(QString new_keysequence);
				
				///
				bool wasCustomMode(){return custom_shortcut_mode_ ;}
				
				///
				bool wasEdited(){return edited_;}

			protected slots:

				void modeChanged_(bool toggled);

			protected:
							
				bool custom_shortcut_mode_;
				bool edited_;
		};
	}
}
#endif
