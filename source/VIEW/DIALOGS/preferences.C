// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: preferences.C,v 1.12 2004/10/01 14:20:15 amoll Exp $
//

#include <BALL/VIEW/DIALOGS/preferences.h>
#include <BALL/VIEW/KERNEL/preferencesEntry.h>
#include <BALL/FORMAT/INIFile.h>
#include <qwidgetstack.h>
#include <qlistview.h>

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
			 return (entries_listview->childCount() > 0);
		}

		void Preferences::insertEntry(PreferencesEntry *child)
			throw()
		{
			if (!RTTI::isKindOf<QWidget>(*child)) 
			{
				Log.error() << "PreferencesEntry not derived from QWidget in " 
										<< __FILE__ << "  " << __LINE__<< std::endl;
				return;
			
			}

			if (child->getEntries().size() == 0) return;

			QWidget* widget = dynamic_cast<QWidget*>(child);

 			widget_stack->addWidget(widget);

			// set size for all child tabs
			widget->setMinimumSize(widget_stack->width(), widget_stack->height());
			widget->setMaximumSize(widget_stack->width(), widget_stack->height());
			widget->resize(widget_stack->width(), widget_stack->height());

			entries_.insert(child);

			PreferencesEntry::EntriesList::Iterator it = child->getEntries().begin();
			QListViewItem* item = new QListViewItem(entries_listview, (*it).second);
			entries_listview->insertItem(item);
			item_to_widget_[item] = (*it).first;
			item_to_entry_[item] = child;
			widget_to_item_[widget] = item;
			it++;
			for (; it != child->getEntries().end(); it++)
			{
				QListViewItem* new_item = new QListViewItem(item, (*it).second);
				item->insertItem(new_item);
				item_to_widget_[new_item] = (*it).first;
				widget_to_item_[(*it).first] = new_item;
			}
		}

		void Preferences::removeEntry(PreferencesEntry *child)
			throw()
		{
			if (!entries_.has(child)) return;

			QWidget* widget = (dynamic_cast<QWidget*>(child));
			entries_listview->removeItem(widget_to_item_[widget]);
			widget_stack->removeWidget(widget);
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

		void Preferences::showEntry(QWidget* child)
		{
			if (!widget_to_item_.has(child)) 
			{
				return;
			}

			QListViewItem* item = widget_to_item_[child];
			entries_listview->setSelected(item, true);

			if (item->parent() != 0)
			{
				item->parent()->setOpen(true);
			}

			if (widget_stack->id(child) != -1)
			{
				if (item->firstChild() == 0)
				{
					widget_stack->raiseWidget(child);
				}
				else
				{
					widget_stack->raiseWidget(0);
				}
				return;
			}

			if (!item_to_entry_.has(item->parent())) return;
			PreferencesEntry* entry= item_to_entry_[item->parent()];

			widget_stack->raiseWidget(item_to_widget_[item->parent()]);

			entry->showEntry(child);
		}	

		const QWidget* Preferences::currentPage() const
			throw()
		{
			QListViewItem* item = entries_listview->selectedItem();
			if (item == 0) return 0;
			
			return item_to_widget_[item];
		}
		
		void Preferences::entrySelected(QListViewItem* item)
		{
			showEntry(item_to_widget_[item]);
		}

		void Preferences::setDefaultValues()
		{
			QListViewItem* item = entries_listview->selectedItem();
			if (item == 0) return;
			
			if (item_to_entry_.has(item))
			{
				item_to_entry_[item]->setDefaultValues();
			}
			else
			{
				item_to_entry_[item->parent()]->setDefaultValues();
			}
		}

	} // namespace VIEW
} // namespace BALL

