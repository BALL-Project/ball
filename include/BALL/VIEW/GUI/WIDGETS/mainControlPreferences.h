// $Id: 

#ifndef BALL_VIEW_GUI_WIDGETS_MAINCONTROLPREFERENCES_H
#define BALL_VIEW_GUI_WIDGETS_MAINCONTROLPREFERENCES_H

#ifndef BALL_FORMAT_INIFILE_H
# include <BALL/FORMAT/INIFile.h>
#endif

#include <qwidget.h>
#include <qradiobutton.h>

using namespace BALL; 

namespace BALL
{
	namespace VIEW
	{

		class MainControlPreferences : public QWidget
		{
			Q_OBJECT

			public:
			
			MainControlPreferences(QWidget *parent = NULL,	const char *name = NULL );
			
			virtual ~MainControlPreferences();

			QStyle* getStyle();
			
			void fetchPreferences(INIFile &inifile);
			void writePreferences(INIFile &inifile);

      private:
			
			QRadioButton *is_platinum_style;
			QRadioButton *is_windows_style;
			QRadioButton *is_motif_style;
			QRadioButton *is_cde_style;
		};

	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_GUI_WIDGETS_MAINCONTROLPREFERENCES_H
