// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: testDialog.h,v 1.2 2002/02/27 12:25:07 sturm Exp $
// view tutorial example
// ---------------------
// creation of a dialog

#include

namespace BALL
{

	namespace VIEW
	{

		class TestDialog: public QDialog, public ModularWidget
		{
			Q_OBJECT

			public:

				TestDialog(QWidget* parent = 0, const char* name = 0)
					throw();
				virtual ~TestDialog()
					throw();

				virtual void initializeWidget(MainControl& main_control)
					throw();
				virtual void checkMenu(MainControl& main_control)
					throw();
				virtual void finalizeWidget(MainControl& main_control)
					throw();
				virtual void initializePreferencesTab(Preferences &preferences)
					throw();
				virtual void finalizePreferencesTab(Preferences &preferences)
					throw();
				virtual void applyPreferences(Preferences &preferences)
					throw();
				virtual void fetchPreferences(INIFile &inifile)
					throw();
				virtual void writePreferences(INIFile &inifile)
					throw();
				virtual void onNotify(Message *message)
					throw();

			protected slots:

				void applyButtonClicked()
					throw();
				void openDialog()
					throw();

			private:

				TestPreferences *test_preferences_;
				QPushButton *apply_button_;
				int id_;
				List<Composite*> selection_;
				ColorRGBA color_;
		};

	}

}
