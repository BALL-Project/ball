// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: preferences.C,v 1.9 2004/09/01 14:28:29 amoll Exp $
//

#include <BALL/VIEW/DIALOGS/preferences.h>
#include <BALL/FORMAT/INIFile.h>
#include <qwidgetstack.h>
#include <qlistbox.h>

namespace BALL
{
	namespace VIEW
	{

		Preferences::Preferences(const Preferences& /*preferences*/)
			throw()
			:	PreferencesData(0, "", FALSE, 208)
		{
		}

		Preferences::Preferences(QWidget* parent, const char* name)
			throw()
			:	PreferencesData(parent, name, FALSE, 208)
		{
			setCaption(name);
		}

		Preferences::~Preferences()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.info() << "Destructing object " << (void *)this 
									 << " of class " << RTTI::getName<Preferences>() << std::endl;
			#endif 
		}

		bool Preferences::hasPages()
			throw()
		{
			return (list_box->count() > 0);
		}

		void Preferences::insertPage(QWidget *child, const String& name)
			throw()
		{
 			widget_stack->addWidget(child, list_box->count() + 1);
			list_box->insertItem(name.c_str());

			// set size for all child tabs
			child->resize(380,210);
		}

		void Preferences::removePage(QWidget *child)
			throw()
		{
			list_box->removeItem(widget_stack->id(child) - 1);
			widget_stack->removeWidget(child);
		}

		void Preferences::fetchPreferences(INIFile& inifile)
			throw()
		{
			// the position of the window
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
			// the window position
			inifile.insertValue("WINDOWS", "Preferences::x", String(x()));
			inifile.insertValue("WINDOWS", "Preferences::y", String(y()));
		}

		void Preferences::show()
		{
			QDialog::show();
			raise();
		}

		void Preferences::showPage(QWidget* child)
		{
			if (widget_stack->id(child) == -1) return;

 			list_box->setCurrentItem(widget_stack->id(child) - 1);
			widget_stack->raiseWidget(child);
		}	

		void Preferences::showPage(int nr)
		{
			if (widget_stack->widget(nr + 1) == 0)
			{
				return;
			}

			if (list_box->currentItem() != nr)
			{
				list_box->setCurrentItem(nr);
			}
			widget_stack->raiseWidget(nr + 1);
		}

		const QWidget* Preferences::currentPage() const
			throw()
		{
			return widget_stack->visibleWidget();
		}
		
	} // namespace VIEW
} // namespace BALL

