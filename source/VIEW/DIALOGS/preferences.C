// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: preferences.C,v 1.10 2004/09/29 20:40:18 amoll Exp $
//

#include <BALL/VIEW/DIALOGS/preferences.h>
#include <BALL/VIEW/KERNEL/preferencesEntry.h>
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
									 << " of class Preferences" << std::endl;
			#endif 
		}

		bool Preferences::hasPages()
			throw()
		{
			return (list_box->count() > 0);
		}

		void Preferences::insertEntry(PreferencesEntry *child, const String& name)
			throw()
		{
			if (!RTTI::isKindOf<QWidget>(*child)) 
			{
				Log.error() << "PreferencesEntry not derived from QWidget in " 
										<< __FILE__ << "  " << __LINE__<< std::endl;
				return;
			}

			QWidget* widget = dynamic_cast<QWidget*>(child);

 			widget_stack->addWidget(widget, list_box->count() + 1);
			list_box->insertItem(name.c_str());

			// set size for all child tabs
			widget->setMinimumSize(560,400);
			widget->setMaximumSize(560,400);
			widget->resize(560,400);

			entries_.insert(child);
		}

		void Preferences::removeEntry(PreferencesEntry *child)
			throw()
		{
			if (!entries_.has(child)) return;
			list_box->removeItem(widget_stack->id((QWidget*)child) - 1);
			widget_stack->removeWidget((QWidget*)child);
			entries_.erase(child);
			delete child;
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

			HashSet<PreferencesEntry*>::Iterator it = entries_.begin();
			for (; it != entries_.end(); it++)
			{
				(**it).readPreferenceEntries(inifile);
			}
		}

		void Preferences::writePreferences(INIFile& inifile)
			throw()
		{
			// the window position
			inifile.insertValue("WINDOWS", "Preferences::x", String(x()));
			inifile.insertValue("WINDOWS", "Preferences::y", String(y()));
			HashSet<PreferencesEntry*>::Iterator it = entries_.begin();
			for (; it != entries_.end(); it++)
			{
				(**it).writePreferenceEntries(inifile);
			}
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

