// $Id: 

#include <BALL/VIEW/GUI/DIALOGS/preferences.h>

namespace BALL
{
	namespace VIEW
	{

		Preferences::Preferences(QWidget* parent, const char* name)
			:	QTabDialog(parent, name, FALSE, 208),
				number_of_tabs_(0)
		{
			setApplyButton();
			setCancelButton();
			setOKButton();
			
			resize(400,300);
			setMinimumSize(400, 300);
			setMaximumSize(400, 300);

			connect(this,
							SIGNAL(cancelButtonPressed()),
							SLOT(hide()));
		}
		
		Preferences::~Preferences()
		{
		}
		
		bool Preferences::hasTabs()
		{
			return (number_of_tabs_ > 0);
		}

		void Preferences::insertTab(QWidget *child, const QString &name)
		{
			++number_of_tabs_;
			addTab(child, name);
		}
		
		void Preferences::removeTab(QWidget *child)
		{
			--number_of_tabs_;
			removePage(child);
		}
		
		void Preferences::fetchPreferences(INIFile& inifile)
		{
			// 
			// the geometry of the preferences window
			//
			int x_pos = x();
			int y_pos = y();
			
			if (inifile.hasEntry("WINDOWS", "Preferences::x"))
			{
				x_pos = inifile.getValue("WINDOWS", "Preferences::x").toInt();
			}
			if (inifile.hasEntry("WINDOWS", "Preferences::y"))
			{
				y_pos = inifile.getValue("WINDOWS", "Preferences::y").toInt();
			}
			
			move(x_pos, y_pos);
		}
		
		void Preferences::writePreferences(INIFile& inifile)
		{
			//	
			// the display window position
			//
			inifile.setValue
				("WINDOWS", "Preferences::x", String(x()));
			inifile.setValue
				("WINDOWS", "Preferences::y", String(y()));
		}

		void Preferences::openDialog()
		{
			show();
			raise();
		}

	} // namespace VIEW

} // namespace BALL





