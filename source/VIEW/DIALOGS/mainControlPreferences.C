// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: 

#include <BALL/VIEW/DIALOGS/mainControlPreferences.h>

#include <qradiobutton.h>
#include <qplatinumstyle.h>
#include <qwindowsstyle.h>
#include <qmotifstyle.h>
#include <qcdestyle.h>
#include <qsgistyle.h>

namespace BALL
{
	namespace VIEW
	{

		MainControlPreferences::MainControlPreferences(QWidget* parent, const char* name, WFlags fl)
			throw()
			: MainControlPreferencesData(parent, name, fl)
		{
			show();
		}
		
		MainControlPreferences::~MainControlPreferences()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.error() << "Destructing object " << (void *)this 
										<< " of class MainControlPreferences" << endl;
			#endif 
		}
		
		QStyle* MainControlPreferences::getStyle()
			throw()
		{
			QStyle* new_style = 0;
			if (is_platinum_style->isChecked())
			{
				new_style = new QPlatinumStyle();
			}
			if (is_windows_style->isChecked())
			{
				new_style = new QWindowsStyle();
			}
			if (is_motif_style->isChecked())
			{
				new_style = new QMotifStyle();
			}
			if (is_cde_style->isChecked())
			{
				new_style = new QCDEStyle();
			}
			if (is_sgi_style->isChecked())
			{
				new_style = new QSGIStyle;
			}
			
			return new_style;
		}
		
		void MainControlPreferences::fetchPreferences(INIFile& inifile)
			throw()
		{
			String style = "platinum";
			if (inifile.hasEntry("WINDOWS", "style"))
			{
				style = inifile.getValue("WINDOWS", "style");
			}
			
			if (style == "motif")
			{
				is_motif_style->setChecked(TRUE);
			} 
			else if (style == "windows")
			{
				is_windows_style->setChecked(TRUE);
			}
			else if (style == "cde")
			{
				is_cde_style->setChecked(TRUE);
			} 
			else if (style == "sgi")
			{
				is_sgi_style->setChecked(true);
			}
			else 
			{
				is_platinum_style->setChecked(TRUE);
			}
		}
		
		void MainControlPreferences::writePreferences(INIFile& inifile)
			throw()
		{
			String style = "platinum";
			if (is_platinum_style->isChecked())
			{
				style = "platinum";
			}
			else if (is_windows_style->isChecked())
			{
				style = "windows";
			}
			else if (is_motif_style->isChecked())
			{
				style = "motif";
			}
			else if (is_cde_style->isChecked())
			{
				style = "cde";
			}	
			else if (is_sgi_style->isChecked())
			{
				style = "sgi";
			}
			
			// save the style settings
			inifile.insertValue("WINDOWS", "style", style);
		}
		
	} // namespace VIEW
} // namespace BALL

