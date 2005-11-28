// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: preferencesEntry.C,v 1.16.2.5 2005/11/28 15:16:28 amoll Exp $
//

#include <BALL/VIEW/KERNEL/preferencesEntry.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/VIEW/KERNEL/mainControl.h>

#include <qslider.h>
#include <qlabel.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qlineedit.h>
#include <qbuttongroup.h>
#include <qwidgetstack.h>

using namespace std;

namespace BALL
{
	namespace VIEW
	{

		PreferencesEntry::PreferencesEntry()
			: widget_stack_(0)
		{
			#ifdef BALL_VIEW_DEBUG
				Log.info() << "New PreferencesEntry " << (void *)this << std::endl;
			#endif 
		}

		PreferencesEntry::~PreferencesEntry()
		{
		}

		void PreferencesEntry::writePreferenceEntries(INIFile& inifile)
		{
			if (registered_objects_.size() == 0) return;

			QWidget* widget = dynamic_cast<QWidget*>(this);
			if (widget == 0)
			{
				Log.error() << "This PreferencesEntry is not a QWidget! " << std::endl;
				return;
			}

			if (inifile_section_name_ == "") 
			{
				Log.error() << "INIFile section name not set in " << widget->name() << std::endl;
				return;
			}

			if (!inifile.hasSection(inifile_section_name_))
			{
				inifile.appendSection(inifile_section_name_);
			}

			String value, name;
			
			HashSet<QWidget*>::Iterator it = registered_objects_.begin();
			for (; it != registered_objects_.end(); it++)
			{
				name = (**it).name();

				if (name == "" || !getValue_(*it, value))
				{
					if (name == "")
					{
						Log.error() << "Unnamed Preferences object!";
					}

					Log.error() << " in PreferencesEntry: " << widget->name() << " ";
				  Log.error() << __FILE__ << " " << __LINE__ << std::endl;

					continue;
				}

				inifile.insertValue(inifile_section_name_, name, value);
			}
		}


		bool PreferencesEntry::getValue_(const QWidget* widget, String& value)
		{
			const ExtendedPreferencesObject* epo = dynamic_cast<const ExtendedPreferencesObject*>(widget);
			if (epo != 0)
			{
				if (!epo->getValue(value))
				{
					BALLVIEW_DEBUG;
				}
			}
			else if (RTTI::isKindOf<QSlider>(*widget))
			{
				value = String((dynamic_cast<const QSlider*>(widget))->value());
			}
			else if (RTTI::isKindOf<QLabel>(*widget))
			{
				value = ColorRGBA((dynamic_cast<const QLabel*>(widget))->backgroundColor());	
			}
			else if (RTTI::isKindOf<QLineEdit>(*widget))
			{
				value = (dynamic_cast<const QLineEdit*>(widget))->text().ascii();
			}
			else if (RTTI::isKindOf<QCheckBox>(*widget))
			{
				value = String((dynamic_cast<const QCheckBox*>(widget))->isChecked());
			}
			else if (RTTI::isKindOf<QComboBox>(*widget))
			{
				value = String((dynamic_cast<const QComboBox*>(widget))->currentItem());
			}
			else if (RTTI::isKindOf<QButtonGroup>(*widget))
			{
				const QButtonGroup* qbg = dynamic_cast<const QButtonGroup*>(widget);

				// no button selected (should not happen) -> select first
				if (qbg->selectedId() == -1)
				{
					value="0";
					return true;
				}

				value = String(qbg->selectedId());
			}
			else
			{
				Log.error() << "Unknown Preferences object " << widget->name() << std::endl;
				return false;
			}

			return true;
		}


		void PreferencesEntry::readPreferenceEntries(const INIFile& inifile)
		{
			HashSet<QWidget*>::Iterator it = registered_objects_.begin();
			for (; it != registered_objects_.end(); it++)
			{
				if (!inifile.hasEntry(inifile_section_name_, (**it).name())) continue;

				String value = inifile.getValue(inifile_section_name_, (**it).name());

				if (!setValue_(*it, value))
				{
					Log.error() << "Invalid entry in INIFile: " << (**it).name() << " " << value << std::endl;
				}
			}
		}

		
		bool PreferencesEntry::setValue_(QWidget* widget, const String& value)
		{
			try
			{
				ExtendedPreferencesObject* epo = dynamic_cast<ExtendedPreferencesObject*>(widget);
				if (epo != 0)
				{
					if (!epo->setValue(value))
					{
						BALLVIEW_DEBUG;
					}
				}
				else if (RTTI::isKindOf<QSlider>(*widget))
				{
					(dynamic_cast<QSlider*>(widget))->setValue(value.toInt());
				}
				else if (RTTI::isKindOf<QLabel>(*widget))
				{
					QLabel& label = *dynamic_cast<QLabel*>(widget);
					label.setBackgroundColor(ColorRGBA(value).getQColor());
				}
				else if (RTTI::isKindOf<QLineEdit>(*widget))
				{
					(dynamic_cast<QLineEdit*>(widget))->setText(value.c_str());
				}
				else if (RTTI::isKindOf<QCheckBox>(*widget))
				{
					(dynamic_cast<QCheckBox*>(widget))->setChecked(value == "1");
				}
				else if (RTTI::isKindOf<QComboBox>(*widget))
				{
					if (value.toUnsignedInt() >= (Position)(dynamic_cast<QComboBox*>(widget))->count()) return false;
					(dynamic_cast<QComboBox*>(widget))->setCurrentItem(value.toInt());
				}
				else if (RTTI::isKindOf<QButtonGroup>(*widget))
				{
					(dynamic_cast<QButtonGroup*>(widget))->setButton(value.toInt());
				}
				else 
				{
					Log.error() << "Unknown QWidget " << widget->name() << " in " 
											<< __FILE__ << __LINE__ << std::endl;
					return false;
				}
			}
			catch(...)
			{
				return false;
			}

			return true;
		}

		
		void PreferencesEntry::registerObject_(QWidget* widget)
		{
			if (widget == 0) return;

			if (widget->name() == "")
			{
				Log.error() << "Unnamed Preferences object!" << std::endl;
				return;
			}

			if (registered_objects_.has(widget))
			{
				Log.error() << "Widget " << widget << " with name " << widget->name() 
										<< " was already added!" << std::endl;
				return;
			}

			registered_objects_.insert(widget);

			String value;
			getValue_(widget, value);
			
			default_values_[widget] = value;
			last_values_[widget] = value;
		}

		
		void PreferencesEntry::setWidgetStackName(const String& name)
		{
			QWidget* widget = dynamic_cast<QWidget*>(this);

			if (widget == 0)
			{
				BALLVIEW_DEBUG;
				return;
			}

			insertEntry_(widget, name);
		}
			
		void PreferencesEntry::insertEntry_(QWidget* widget, const String& name)
		{
			stack_pages_.push_back(std::pair<QWidget*, String>(widget, name));
		}

		
		void PreferencesEntry::showStackPage(Position nr) 
		{ 
			if (widget_stack_ == 0) return;
			widget_stack_->raiseWidget(nr);
		}

		void PreferencesEntry::showStackPage(QWidget* widget) 
		{ 
			if (widget_stack_ == 0) return;
			widget_stack_->raiseWidget(widget);
		}

		
		void PreferencesEntry::setWidgetStack(QWidgetStack* stack)
		{
			widget_stack_ = stack;

			if (widget_stack_ == 0) return;

			for (Position p = 0; p < 9999; p++)
			{
				if (widget_stack_->widget(p) == 0) break;

				String name = widget_stack_->widget(p)->name();
				for (Position i = 0; i < name.size(); i++)
				{
					if (name[i] == '_') name[i] = ' ';
				}
				insertEntry_(widget_stack_->widget(p), name);
			}

			storeStackEntries_();
		}


		Position PreferencesEntry::currentStackPage() const
		{
			if (widget_stack_ == 0) return 0;
			return widget_stack_->id(widget_stack_->visibleWidget());
		}


		void PreferencesEntry::registerWidgetForHelpSystem_(const QWidget* widget, const String& url)
		{
			if (getMainControl() == 0) return;

			RegisterHelpSystemMessage* msg = new RegisterHelpSystemMessage();
			msg->setWidget(widget);
			msg->setURL(url);
			getMainControl()->sendMessage(*msg);
		}
		

		void PreferencesEntry::storeValues()
		{
			String value;

			HashSet<QWidget*>::Iterator it = registered_objects_.begin();
			for (; it != registered_objects_.end(); it++)
			{
			 	getValue_(*it, value);
				last_values_[*it] = value;
			}
		}

		void PreferencesEntry::restoreValues_(bool all, const ValueMap& map)
		{
			// no widget stack, or (all == true) -> all items are restored
			if (widget_stack_ == 0 || all)
			{
				HashSet<QWidget*>::Iterator it = registered_objects_.begin();
				for (; +it; it++)
				{
					setValue_(*it, map[*it]);
				}
			}
			// entries for current widget stack item
			else if (!all)
			{
				Index id = currentStackPage();
				if (id == -1) return;
				for (Position p = 0; p < stack_entries_[id].size(); p++)
				{
					QWidget* widget = stack_entries_[id][p];
					setValue_(widget, map[widget]);
				}
			}
		}


		void PreferencesEntry::restoreValues(bool all)
		{
			restoreValues_(all, last_values_);
		}


		void PreferencesEntry::restoreDefaultValues(bool all)
		{
			restoreValues_(all, default_values_);
		}

		
		void PreferencesEntry::storeStackEntries_()
		{
			if (widget_stack_ == 0) return;

			stack_entries_.clear();
			stack_entries_.resize(stack_pages_.size());
			
			HashSet<QWidget*>::Iterator it = registered_objects_.begin();
			for (; it != registered_objects_.end(); it++)
			{
				QWidget* widget = *it;
				while (widget->parent() != 0)
				{
					QWidget* parent = (QWidget*)widget->parent();

					if (!RTTI::isKindOf<QWidgetStack>(*parent)) 
					{
						widget = parent;
						continue;
					}

					Index id = widget_stack_->id(widget);

					if (id == -1) continue;

					stack_entries_[id].push_back(*it);
					widget = 0;
					break;
				} // look for parent 

				// if we dont find the parent widget stack entry, debug
				if (widget != 0) BALLVIEW_DEBUG;

			} // all objects
		}

		ColorRGBA PreferencesEntry::getLabelColor_(const QLabel* label) const
		{
			return ColorRGBA(label->backgroundColor());
		}
			
		void PreferencesEntry::setLabelColor_(QLabel* label, const ColorRGBA& color)
		{
			label->setBackgroundColor(color.getQColor());
		}
			
	} // namespace VIEW
} // namespace BALL
