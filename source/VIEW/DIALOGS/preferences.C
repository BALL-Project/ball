// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/DIALOGS/preferences.h>
#include <BALL/VIEW/KERNEL/preferencesEntry.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/WIDGETS/helpViewer.h>
#include <BALL/FORMAT/INIFile.h>

#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QPushButton>

namespace BALL
{
	namespace VIEW
	{

		Preferences::Preferences(const Preferences& /*preferences*/)
			:	QDialog(0),
				Ui_PreferencesData()
		{
			setupUi(this);
		}

		Preferences::Preferences(QWidget* parent, const char* name)
			:	QDialog(parent),
				Ui_PreferencesData()
		{
			setupUi(this);
			setObjectName(name);

			// TUT-02 (#523): document the Restore Defaults scope at the point of
			// confusion. The button resets only the currently-open page's
			// registered values; font/language/style are intentionally outside
			// the reset path (mainControlPreferences unregisters style_box_ and
			// languageComboBox_, and the font is held separately), so users who
			// expect those to revert are surprised. Spell out the scope here.
			// The button may be absent on some platform layouts — null-guard it.
			if (QPushButton* restore_button = buttonBox->button(QDialogButtonBox::RestoreDefaults))
			{
				restore_button->setToolTip(tr("Resets this page's settings to their defaults. "
				                              "Font, language, and interface style are not changed."));
			}

			// signals and slots connections
			connect( entries_listview, SIGNAL(itemSelectionChanged()), this, SLOT(entrySelected()));
			//for some reason the ok_button and the apply button is connected in VIEW/KERNEL/mainControl.C
		}

		Preferences::~Preferences()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.info() << "Destructing object " << this << " of class Preferences" << std::endl;
			#endif 
		}

		bool Preferences::hasPages()
		{
			return (widget_stack->count() > 1);
		}

		void Preferences::insertChildEntry(const std::string& category, PreferencesEntry* child)
		{
			for(HashSet<PreferencesEntry*>::iterator it = entries_.begin(); it != entries_.end(); ++it) {
				if((*it)->getINIFileSectionName() == String(category)) {
					insertChildEntry(*it, child);
				}
			}
		}

		void Preferences::insertChildEntry(PreferencesEntry* parent, PreferencesEntry* child)
		{
			QWidget* parent_widget = dynamic_cast<QWidget*>(parent);
			QWidget* child_widget  = dynamic_cast<QWidget*>(child);

			if(!parent_widget && !child_widget) return;

			if(!entries_.has(parent) || !widget_to_item_.has(parent_widget)) return;

			if(widget_to_item_.has(child_widget)) return;

			QStringList sl;
			sl << child->getINIFileSectionName().c_str();
			QTreeWidgetItem* new_item = new QTreeWidgetItem(widget_to_item_[parent_widget], sl);
			item_to_entry_.insert(std::make_pair(new_item, child));
			item_to_widget_.insert(std::make_pair(new_item, child_widget));
			widget_to_item_.insert(std::make_pair(child_widget, new_item));
			widget_stack->addWidget(child_widget);
			entries_.insert(child);
		}

		void Preferences::insertEntry(PreferencesEntry *child)
		{
            if (!RTTI::isKindOf<QWidget>(child))
			{
				Log.error() << "PreferencesEntry not derived from QWidget in " 
										<< __FILE__ << "  " << __LINE__<< std::endl;
				Log.error() << "Section name: " << child->getINIFileSectionName() << std::endl;
				return;
			
			}

			if (child->getStackPages().size() == 0) 
			{
				Log.error() << "No stack pages found" << std::endl;
				return;
			}

			PreferencesEntry::StackPages::iterator it = child->getStackPages().begin();
			entries_.insert(child);
			widget_stack->addWidget((*it).first);

			// the parent listview entry
			QStringList sl;
			sl << (*it).second.c_str();
			QTreeWidgetItem* item = new QTreeWidgetItem(entries_listview, sl);

			if (child->getStackPages().size() == 1)
			{
				item_to_widget_[item] = (*it).first;
				item_to_entry_[item] = child;
				widget_to_item_[(*it).first] = item;
				return;
			}
			
			item_to_widget_[item] = widget_stack->widget(0);
//   			widget_to_item_[widget_stack->widget(0)] = item;
			item_to_entry_[item] = child;
			it++;

			for (; it != child->getStackPages().end(); it++)
			{
				QStringList sl;
				sl << (*it).second.c_str();
				QTreeWidgetItem* new_item = new QTreeWidgetItem(item, sl);
				item_to_widget_[new_item] = (*it).first;
				widget_to_item_[(*it).first] = new_item;
			}
		}

		void Preferences::removeEntry(PreferencesEntry *child)
		{
			if (!entries_.has(child)) return;

			QWidget* widget = (dynamic_cast<QWidget*>(child));

			if(!widget)
			{
				return;
			}

			QTreeWidgetItem* item = widget_to_item_[widget];
			removeItem_(item, true);

			item_to_widget_.erase(item);
			item_to_entry_.erase(item);
			widget_to_item_.erase(widget);
			widget_stack->removeWidget(widget);
			entries_.erase(child);
		}

		void Preferences::fetchPreferences(INIFile& inifile)
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
			
			x_pos = std::max(x_pos, 20);
			y_pos = std::max(y_pos, 20);

			move(x_pos, y_pos);

			HashSet<PreferencesEntry*>::Iterator it = entries_.begin();
			for (; +it; it++)
			{
				(**it).readPreferenceEntries(inifile);
			}
		}

		void Preferences::writePreferences(INIFile& inifile)
		{
			// the window position
			inifile.insertValue("WINDOWS", "Preferences::x", String(pos().x()));
			inifile.insertValue("WINDOWS", "Preferences::y", String(pos().y()));

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

		bool Preferences::showStackByName(const String& stack_name)
		{
			// Phase 999.44 Plan 02 — Tools › Legacy Settings handler.
			// Walk every registered PreferencesEntry's stack pages and
			// open the one whose label matches @p stack_name.
			QWidget* target = 0;
			for (HashSet<PreferencesEntry*>::Iterator it = entries_.begin(); +it; ++it)
			{
				PreferencesEntry* entry = *it;
				if (entry == 0) continue;
				for (auto& p : entry->getStackPages())
				{
					if (p.second == stack_name)
					{
						target = p.first;
						break;
					}
				}
				if (target != 0) break;
			}
			if (target == 0) return false;
			showEntry(target);
			return true;
		}

		void Preferences::showEntry(QWidget* child)
		{
			if (!widget_to_item_.has(child))
			{
				widget_stack->setCurrentIndex(0);
				return;
			}

			// set the listview entry
			QTreeWidgetItem* item = widget_to_item_[child];
			entries_listview->setCurrentItem(item);
			// Qt 6: QTreeWidget::setItemSelected / setItemExpanded removed; call
			// directly on the QTreeWidgetItem (which inherits the per-item helpers).
			item->setSelected(true);

			if (item->parent() != 0) item->parent()->setExpanded(true);

			// is the child a direct child of the stacked widget?
			if (widget_stack->indexOf(child) != -1)
			{
				// show the child 
				widget_stack->setCurrentWidget(child);
				widget_stack->currentWidget()->show();

				return;
			}

 			if (!item_to_entry_.has(item->parent()))
			{
				BALLVIEW_DEBUG
				return;
			}

 			PreferencesEntry* entry= item_to_entry_[item->parent()];
			QWidget* cw = (*entry->getStackPages().begin()).first;
			widget_stack->setCurrentWidget(cw);
			cw->show();
 			entry->showStackPage(child);
		}	

		const QWidget* Preferences::currentPage() const
		{
			QList<QTreeWidgetItem*> sel = entries_listview->selectedItems();
			if (sel.size() == 0) return 0;
			QTreeWidgetItem* item =  *sel.begin();

			if(!item_to_widget_.has(item)) {
				return widget_stack->widget(0);
			}

			return item_to_widget_[item];
		}
		
		const QWidget* Preferences::currentEntry() const
		{
			return widget_stack->currentWidget();
		}
		
		void Preferences::entrySelected()
		{
			QWidget* to_show = (QWidget*)currentPage();
			if (to_show == currentEntry()) return;
			showEntry(to_show);
		}


		void Preferences::setDefaultValues()
		{
			QList<QTreeWidgetItem*> sel = entries_listview->selectedItems();
			if (sel.size() == 0) return;
			QTreeWidgetItem* item =  *sel.begin();

			if (item_to_entry_.has(item))
			{
				item_to_entry_[item]->restoreDefaultValues();
			}
			else
			{
				if (item_to_entry_.has(item->parent()))
				{
					item_to_entry_[item->parent()]->restoreDefaultValues();
				}
			}
		}

		void Preferences::removeItem_(QTreeWidgetItem* item, bool)
		{
			while (item->child(0) != 0)
			{
				removeItem_(item->child(0), false);
				delete item->takeChild(0);
			}

			if (item->parent() != 0) delete item;
		}

		void Preferences::showHelp()
		{ }

		void Preferences::reject()
		{
			HashSet<PreferencesEntry*>::Iterator it = entries_.begin();
			for (; +it; ++it)
			{
				(**it).restoreValues(true);
			}

			QDialog::reject();
		}

		void Preferences::applyPreferences()
		{
			HashSet<PreferencesEntry*>::Iterator it = entries_.begin();
			for (; +it; ++it)
			{
				(**it).storeValues();
			}
		}

		void Preferences::setApplyEnabled(bool enabled)
		{
			buttonBox->button(QDialogButtonBox::Ok)->setEnabled(enabled);
			buttonBox->button(QDialogButtonBox::Apply)->setEnabled(enabled);
		}

		void Preferences::dialogButtonsClicked_(QAbstractButton* button)
		{
			if(!button) {
				return;
			}

			switch(buttonBox->buttonRole(button)) {
				case QDialogButtonBox::ApplyRole:
					Q_EMIT applied();
					break;
				case QDialogButtonBox::ResetRole:
					setDefaultValues();
					break;
				default:
					return;
			}

		}

	} // namespace VIEW
} // namespace BALL

