// $Id: 

#ifndef BALL_VIEW_GUI_WIDGETS_SERVERPREFERENCES_H
#define BALL_VIEW_GUI_WIDGETS_SERVERPREFERENCES_H

#ifndef BALL_FORMAT_INIFILE_H
# include <BALL/FORMAT/INIFile.h>
#endif

#include <qwidget.h>
#include <qcheckbox.h>
#include <qlineedit.h>

using namespace BALL;

namespace BALL
{
	namespace VIEW
	{

		class ServerPreferences : public QWidget
		{
			Q_OBJECT
				
			public:

			ServerPreferences(QWidget *parent = NULL, const char *name = NULL );

			virtual ~ServerPreferences();
			
			void fetchPreferences(INIFile& inifile);
			void writePreferences(INIFile& inifile);
			
			int getPort();
			bool getServerStatus();
			

			public slots:

			void resetPort();


		  private:

			QLineEdit *port_;
			QCheckBox *server_status_;
		};

	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_GUI_WIDGETS_SERVERPREFERENCES_H
