// $Id: 

#ifndef BALL_VIEW_GUI_DIALOGS_PREFERENCES_H
#define BALL_VIEW_GUI_DIALOGS_PREFERENCES_H

#ifndef BALL_FORMAT_INIFILE_H
# include <BALL/FORMAT/INIFile.h>
#endif

#include <qtabdialog.h>
#include <qwidget.h>
#include <qstring.h>

using namespace BALL;

namespace BALL
{
	namespace VIEW
	{

		class Preferences : public QTabDialog
		{
			Q_OBJECT
				
			public:

			Preferences(QWidget *parent = NULL,	const char *name = NULL );
			virtual ~Preferences();

			bool hasTabs();

			void insertTab(QWidget *child, const QString &name);
			void removeTab(QWidget *child);

			void fetchPreferences(INIFile &inifile);
			void writePreferences(INIFile &inifile);


			public slots:

			void openDialog();


		  private:

			int number_of_tabs_;
		};

	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_GUI_DIALOGS_PREFERENCES_H
