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

#ifndef BALL_VIEW_KERNEL_MODULARWIDGET_H
# include <BALL/VIEW/KERNEL/modularWidget.h>
#endif

#include <BALL/VIEW/UIC/ui_editSingleShortcut.h>

#include <QtGui/QDialog>

class QModelIndex;

namespace BALL
{
	namespace VIEW
	{

		/** Widget for customizing BALLView shortcuts 
				\ingroup ViewWidgets
		*/
		class BALL_VIEW_EXPORT EditSingleShortcut
			: public QDialog,
		    public Ui_EditSingleShortcutData
		{
			// macro needed for Qt's slot mechanism:
			Q_OBJECT

			public:

				/// Constructor
				EditSingleShortcut(QWidget* parent = NULL, const char* name = "EditSingleShortcut", Qt::WFlags fl = 0 );

				QKeySequence const& getKeySequence() const { return new_sequence_; }

				void setIndex(const QModelIndex& index);
				void reset();
				void setup(const QString& shortcut);

			public slots:
				virtual void accept();
				virtual void reject();

			protected slots:
				void modeChanged_(bool toggled);

			protected:
				bool is_recording_;

				unsigned int modifiers_;
				int key_;
				QKeySequence new_sequence_;

				///
				void setErrorText(QString error);

				///
				void setShortcutText(QString new_keysequence);

				void updateText_();
				void changeMode_(bool mode);

				void keyPressEvent(QKeyEvent* evt);
				void keyReleaseEvent(QKeyEvent* evt);

				void startRecording_();
				void stopRecording_();
		};

	}
}

#endif

