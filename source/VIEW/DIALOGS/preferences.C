// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: preferences.C,v 1.1 2003/08/26 12:38:16 oliver Exp $
//

#include <BALL/VIEW/GUI/DIALOGS/preferences.h>
#include <BALL/FORMAT/INIFile.h>

namespace BALL
{
	namespace VIEW
	{

		Preferences::Preferences(QWidget* parent, const char* name, int width, int height)
			throw()
			:	QTabDialog(parent, name, FALSE, 208),
				number_of_tabs_(0)
		{
			setApplyButton();
			setCancelButton();
			
			resize(width,height);
			setMinimumSize(width, height);
			setMaximumSize(width, height);

			connect(this, SIGNAL(cancelButtonPressed()), SLOT(hide()));
		}
		
		Preferences::~Preferences()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.info() << "Destructing object " << (void *)this 
									 << " of class " << RTTI::getName<Preferences>() << std::endl;
			#endif 

			clear();
		}
		
		void Preferences::clear()
			throw()
		{
		}

		bool Preferences::hasTabs()
			throw()
		{
			return (number_of_tabs_ > 0);
		}

		void Preferences::insertTab(QWidget *child, const QString &name)
			throw()
		{
			++number_of_tabs_;
			addTab(child, name);
		}
		
		void Preferences::removeTab(QWidget *child)
			throw()
		{
			--number_of_tabs_;
			removePage(child);
		}
		
		void Preferences::fetchPreferences(INIFile& inifile)
			throw()
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
			throw()
		{
			// the display window position
			inifile.insertValue("WINDOWS", "Preferences::x", String(x()));
			inifile.insertValue("WINDOWS", "Preferences::y", String(y()));
		}

		void Preferences::openDialog()
		{
			show();
			raise();
		}

	} // namespace VIEW
} // namespace BALL

