// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: preferencesEntry.C,v 1.19.2.2 2006/02/01 13:23:49 amoll Exp $
//

#include <BALL/VIEW/KERNEL/preferencesEntry.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/VIEW/KERNEL/mainControl.h>

#include <QtGui/qslider.h>
#include <QtGui/qlabel.h>
#include <QtGui/qcheckbox.h>
#include <QtGui/qlineedit.h>
#include <QtGui/QStackedWidget>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QGroupBox>
#include <QtGui/QRadioButton>

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
				Log.error() << "INIFile section name not set in " << ascii(widget->objectName()) << std::endl;
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
				name = ascii((**it).objectName());

				if (name == "" || !getValue_(*it, value))
				{
					if (name == "")
					{
						Log.error() << "Unnamed Preferences object!";
					}

					Log.error() << " in PreferencesEntry: " << ascii(widget->objectName()) << " ";
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
				value = getColor(dynamic_cast<const QLabel*>(widget));
			}
			else if (RTTI::isKindOf<QLineEdit>(*widget))
			{
				value = ascii((dynamic_cast<const QLineEdit*>(widget))->text());
			}
			else if (RTTI::isKindOf<QCheckBox>(*widget))
			{
				value = String((dynamic_cast<const QCheckBox*>(widget))->isChecked());
			}
			else if (RTTI::isKindOf<QComboBox>(*widget))
			{
				value = String((dynamic_cast<const QComboBox*>(widget))->currentIndex());
			}
			else if (RTTI::isKindOf<QButtonGroup>(*widget))
			{
				const QButtonGroup* qbg = dynamic_cast<const QButtonGroup*>(widget);

				// no button selected (should not happen) -> select first
				if (qbg->checkedId() == -1)
				{
					value="0";
					return true;
				}

				value = String(qbg->checkedId());
			}
			else if (RTTI::isKindOf<QGroupBox>(*widget))
			{
				value.clear();
				// QGroupBoxes can either be checkable and/or have QRadioButtons as childs
				const QGroupBox* gb = dynamic_cast<const QGroupBox*>(widget);
				if (gb->isCheckable() && gb->isChecked()) 
				{
					value = "#";
				}
				QObjectList ol = widget->children();
				QObjectList::iterator it = ol.begin();
				for (; it != ol.end(); it++)
				{
					if (RTTI::isKindOf<QRadioButton>(**it))
					{
						if (((QRadioButton*) *it)->isChecked())
						{
							value += ascii((**it).objectName());
							return true;
						}
					}
				}

				if (gb->isCheckable()) return true;

				Log.error() << "Warning: QGroupBox " << ascii(widget->objectName())
										<< " in PreferencesEntry without QRadioButton" << std::endl;
				return false;
			}
			else
			{
				Log.error() << "Unknown Preferences object " << ascii(widget->objectName()) << std::endl;
				return false;
			}

			return true;
		}


		void PreferencesEntry::readPreferenceEntries(const INIFile& inifile)
		{
			HashSet<QWidget*>::Iterator it = registered_objects_.begin();
			for (; it != registered_objects_.end(); it++)
			{
				if (!inifile.hasEntry(inifile_section_name_, ascii((**it).objectName()))) continue;

				String value = inifile.getValue(inifile_section_name_, ascii((**it).objectName()));

				if (!setValue_(*it, value))
				{
					Log.error() << "Invalid entry in INIFile: " << ascii((**it).objectName()) << " " << value << std::endl;
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
					setColor(dynamic_cast<QLabel*>(widget), ColorRGBA(value));
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
					(dynamic_cast<QComboBox*>(widget))->setCurrentIndex(value.toInt());
				}
				else if (RTTI::isKindOf<QButtonGroup>(*widget))
				{
					(dynamic_cast<QButtonGroup*>(widget))->button(value.toInt())->setChecked(true);
				}
				else if (RTTI::isKindOf<QGroupBox>(*widget))
				{
					QGroupBox* gb = dynamic_cast<QGroupBox*> (widget);
					String v = value;
					if (gb->isCheckable())
					{
						gb->setChecked(v[0] == '#');
					}
						
					v.trimLeft("#");

					QRadioButton* button = widget->findChild<QRadioButton*>(v.c_str());
					if (button != 0) 
					{
						button->setChecked(true);
						return true;
					}

					if (!gb->isCheckable())
					{
						Log.error() << "Warning: Could not find QRadioButton " << value 
												<< " in GroupBox in PreferencesEntry" << std::endl;
						return false;
					}
				}
				else 
				{
					Log.error() << "Unknown QWidget " << ascii(widget->objectName()) << " in " 
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

			if (ascii(widget->objectName()) == "")
			{
				Log.error() << "Unnamed Preferences object!" << std::endl;
				return;
			}

			if (registered_objects_.has(widget))
			{
				Log.error() << "Widget " << widget << " with name " << ascii(widget->objectName()) 
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
			widget_stack_->setCurrentWidget(widget_stack_->widget(nr));
		}

		void PreferencesEntry::showStackPage(QWidget* widget) 
		{ 
			if (widget_stack_ == 0) return;
			
			// workaround for damn QT 4.1!
			for (Index p = 0; p < widget_stack_->count(); p++)
			{
				widget_stack_->widget(p)->hide();
			}

			widget_stack_->setCurrentWidget(widget);
			widget_stack_->show();
			widget->show();
		}

		
		void PreferencesEntry::setWidgetStack(QStackedWidget* stack)
		{
			widget_stack_ = stack;

			if (widget_stack_ == 0) return;

			for (Position p = 0; p < 9999; p++)
			{
				if (widget_stack_->widget(p) == 0) break;

				String name = ascii(widget_stack_->widget(p)->objectName());
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
			return widget_stack_->currentIndex();
		}


		void PreferencesEntry::registerWidgetForHelpSystem_(const QWidget* widget, const String& url)
		{
			if (getMainControl() == 0) return;

			RegisterHelpSystemMessage* msg = new RegisterHelpSystemMessage();
			msg->setObject(widget);
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

					if (!RTTI::isKindOf<QStackedWidget>(*parent)) 
					{
						widget = parent;
						continue;
					}

					Index id = widget_stack_->indexOf(widget);

					if (id == -1) continue;

					stack_entries_[id].push_back(*it);
					widget = 0;
					break;
				} // look for parent 

				// if we dont find the parent widget stack entry, debug
				if (widget != 0) BALLVIEW_DEBUG;

			} // all objects
		}

	} // namespace VIEW
} // namespace BALL
