// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: mainControlPreferences.C,v 1.6 2003/08/29 07:02:45 oliver Exp $
//

#include <BALL/VIEW/DIALOGS/mainControlPreferences.h>

#include <qcombobox.h>
#include <qstylefactory.h>

namespace BALL
{
	namespace VIEW
	{

		MainControlPreferences::MainControlPreferences(QWidget* parent, const char* name, WFlags fl)
			throw()
			: MainControlPreferencesData(parent, name, fl)
		{
			style_box_->setEditable(false);
			style_box_->insertStringList(QStyleFactory::keys());
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
			QStyle* new_style = QStyleFactory::create(style_box_->currentText());			
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
			if (QStyleFactory::keys().grep(style).size() > 0)
			{
				style_box_->setCurrentText(*QStyleFactory::keys().grep(style).begin());
			}
		}
		
		void MainControlPreferences::writePreferences(INIFile& inifile)
			throw()
		{
			// save the style settings
			String style = style_box_->currentText().ascii();
			inifile.insertValue("WINDOWS", "style", style);
		}
		
	} // namespace VIEW

} // namespace BALL

