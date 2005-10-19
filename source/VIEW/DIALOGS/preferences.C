// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: preferences.C,v 1.16.6.6 2005/10/19 21:43:20 amoll Exp $
//

#include <BALL/VIEW/DIALOGS/preferences.h>
#include <BALL/VIEW/KERNEL/preferencesEntry.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/WIDGETS/helpViewer.h>
#include <BALL/FORMAT/INIFile.h>
#include <qwidgetstack.h>
#include <qlistview.h>
#include <qpushbutton.h>

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

			HashSet<PreferencesEntry*>::Iterator it = entries_.begin();
			for (; it != entries_.end(); it++)
			{
				delete *it;
			}
			
			entries_.clear();
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
				Log.error() << "Section name: " << child->getINIFileSectionName() << std::endl;
				return;
			
			}

			if (child->getStackPages().size() == 0) return;

			QWidget* widget = dynamic_cast<QWidget*>(child);

 			widget_stack->addWidget(widget);

			// set size for all child tabs
			widget->setMinimumSize(widget_stack->width(), widget_stack->height());
			widget->setMaximumSize(widget_stack->width(), widget_stack->height());
			widget->resize(widget_stack->width(), widget_stack->height());

			entries_.insert(child);

			PreferencesEntry::StackPages::Iterator it = child->getStackPages().begin();
			QListViewItem* item = new QListViewItem(entries_listview, (*it).second.c_str());
			entries_listview->insertItem(item);
			item_to_widget_[item] = (*it).first;
			item_to_entry_[item] = child;
			widget_to_item_[widget] = item;
			it++;
			for (; it != child->getStackPages().end(); it++)
			{
				QListViewItem* new_item = new QListViewItem(item, (*it).second.c_str());
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
			removeItem_(widget_to_item_[widget], true);
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
			for (; +it; it++)
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
			for (; +it; it++)
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
			if (!widget_to_item_.has(child)) return;

			HelpViewer* hv = HelpViewer::getInstance(0);
			if (hv != 0)
			{
				help_button->setEnabled(hv->hasHelpFor(child));
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

			QWidget* widget = item_to_widget_[item->parent()];
			widget_stack->raiseWidget(widget);

			entry->showStackPage(child);
		}	

		const QWidget* Preferences::currentPage() const
			throw()
		{
			QListViewItem* item = entries_listview->selectedItem();
			if (item == 0) return 0;
			
			return item_to_widget_[item];
		}
		
		const QWidget* Preferences::currentEntry() const
			throw()
		{
			return widget_stack->visibleWidget();
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
				item_to_entry_[item]->restoreDefaultValues();
			}
			else
			{
				item_to_entry_[item->parent()]->restoreDefaultValues();
			}
		}

		void Preferences::removeItem_(QListViewItem* item, bool update)
			throw()
		{
			while (item->firstChild() != 0)
			{
				removeItem_(item->firstChild(), false);
			}

			delete item;

			if (update) entries_listview->triggerUpdate();
		}

		void Preferences::showHelp()
		{
			HelpViewer* hv = HelpViewer::getInstance(0);
			if (hv != 0)
			{
				hv->showHelpFor(item_to_widget_[entries_listview->selectedItem()]);
			}
		}

		void Preferences::cancelPreferences()
		{
			HashSet<PreferencesEntry*>::Iterator it = entries_.begin();
			for (; +it; ++it)
			{
				(**it).restoreValues(true);
			}
		}

		void Preferences::applyPreferences()
		{
			HashSet<PreferencesEntry*>::Iterator it = entries_.begin();
			for (; +it; ++it)
			{
				(**it).storeValues();
			}
		}

	} // namespace VIEW
} // namespace BALL

