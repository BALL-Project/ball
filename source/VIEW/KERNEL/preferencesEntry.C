// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: preferencesEntry.C,v 1.16 2005/02/15 12:35:51 amoll Exp $
//

#include <BALL/VIEW/KERNEL/preferencesEntry.h>
#include <BALL/VIEW/DATATYPE/colorRGBA.h>

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
			if (preferences_objects_.size() == 0) return;

			if (inifile_section_name_ == "") 
			{
				Log.error() << "INIFile section name not set in " << this << " :"<< std::endl;
				HashSet<QWidget*>::Iterator it = preferences_objects_.begin();
				for (; it != preferences_objects_.end(); it++)
				{
					Log.error() << "   " << (**it).name() << std::endl;
				}
				return;
			}

			inifile.appendSection(inifile_section_name_);

			HashSet<QWidget*>::Iterator it = preferences_objects_.begin();
			for (; it != preferences_objects_.end(); it++)
			{
				String name = (**it).name();

				if (name == "")
				{
					Log.error() << "Unnamed Preferences object!" << std::endl;
					continue;
				}

				if (RTTI::isKindOf<QSlider>(**it))
				{
					inifile.insertValue(inifile_section_name_, name, String((dynamic_cast<QSlider*>(*it))->value()));
				}
				else if (RTTI::isKindOf<QLabel>(**it))
				{
					inifile.insertValue(inifile_section_name_, name, getLabelColor_(dynamic_cast<QLabel*>(*it)));
				}
				else if (RTTI::isKindOf<QLineEdit>(**it))
				{
					inifile.insertValue(inifile_section_name_, name, ((dynamic_cast<QLineEdit*>(*it)))->text().ascii());
				}
				else if (RTTI::isKindOf<QCheckBox>(**it))
				{
					inifile.insertValue(inifile_section_name_, name, String((dynamic_cast<QCheckBox*>(*it))->isChecked()));
				}
				else if (RTTI::isKindOf<QComboBox>(**it))
				{
					inifile.insertValue(inifile_section_name_, name, String((dynamic_cast<QComboBox*>(*it))->currentItem()));
				}
				else if (RTTI::isKindOf<QButtonGroup>(**it))
				{
					QButtonGroup* qbg = dynamic_cast<QButtonGroup*>(*it);
					int id = -1;
					if (qbg != 0)
					{
						if (qbg->selected() == 0)
						{
							id = qbg->id(qbg->selected());
						}
					}
					inifile.insertValue(inifile_section_name_, name, String(id));
				}
				else
				{
					Log.error() << "Unknown QWidget " << name << " in " 
											<< __FILE__ << " " << __LINE__ << std::endl;
					continue;
				}

			}
		}

		void PreferencesEntry::readPreferenceEntries(const INIFile& inifile)
		{
			HashSet<QWidget*>::Iterator it = preferences_objects_.begin();
			for (; it != preferences_objects_.end(); it++)
			{

				if (!inifile.hasEntry(inifile_section_name_, (**it).name())) continue;

				String value = inifile.getValue(inifile_section_name_, (**it).name());

				try
				{
					if (RTTI::isKindOf<QSlider>(**it))
					{
						(dynamic_cast<QSlider*>(*it))->setValue(value.toInt());
					}
					else if (RTTI::isKindOf<QLabel>(**it))
					{
						setLabelColor_((dynamic_cast<QLabel*>(*it)), ColorRGBA(value));
					}
					else if (RTTI::isKindOf<QLineEdit>(**it))
					{
						(dynamic_cast<QLineEdit*>(*it))->setText(value.c_str());
					}
					else if (RTTI::isKindOf<QCheckBox>(**it))
					{
						(dynamic_cast<QCheckBox*>(*it))->setChecked(value == "1");
					}
					else if (RTTI::isKindOf<QComboBox>(**it))
					{
						if (value.toUnsignedInt() >= (Position)(dynamic_cast<QComboBox*>(*it))->count()) continue;
						(dynamic_cast<QComboBox*>(*it))->setCurrentItem(value.toInt());
					}
					else if (RTTI::isKindOf<QButtonGroup>(**it))
					{
						(dynamic_cast<QButtonGroup*>(*it))->setButton(value.toInt());
					}
					else 
					{
						Log.error() << "Unknown QWidget in " << __FILE__ << __LINE__ << std::endl;
						continue;
					}
				}
				catch(...)
				{
					Log.error() << "Invalid entry in INIFile: " << (**it).name() << " " << value << std::endl;
				}
			}
		}

		void PreferencesEntry::registerObject_(QWidget* widget)
		{
			if (widget == 0) return;

			if (String(widget->name()) == "")
			{
				Log.error() << "Unnamed Preferences object!" << std::endl;
				return;
			}

			if (preferences_objects_.has(widget))
			{
				Log.error() << "Widget " << widget << " with name " << widget->name() << " was already added!" << std::endl;
				return;
			}

			preferences_objects_.insert(widget);
		}

		void PreferencesEntry::setLabelColor_(QLabel* label, const ColorRGBA& color)
		{
			label->setBackgroundColor(color.getQColor());
		}

		ColorRGBA PreferencesEntry::getLabelColor_(QLabel* label) const
		{
			return ColorRGBA(label->backgroundColor());
		}

		bool PreferencesEntry::fetchPreference_(const INIFile& inifile, 
																				const String& entry, ColorRGBA& color)
			throw()
		{
			try
			{
				if (!inifile.hasEntry(inifile_section_name_, entry)) return false;
				color = inifile.getValue(inifile_section_name_, entry);
				return true;
			}
			catch(...)
			{
				Log.error() << "Could not read preferences for coloring from INIFile: ";
				Log.error() << entry << std::endl; 
			}
			return false;
		}

		void PreferencesEntry::insertEntry(QWidget* widget, const String& name)
		{
			entries_.push_back(std::pair<QWidget*, String>(widget, name));
		}

		void PreferencesEntry::showEntry(Position nr) 
		{ 
			if (widget_stack_ == 0) return;
			widget_stack_->raiseWidget(nr);
		}

		void PreferencesEntry::showEntry(QWidget* widget) 
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
				insertEntry(widget_stack_->widget(p), name);
			}
		}


		Position PreferencesEntry::currentEntry() const
		{
			if (widget_stack_ == 0) return 0;
			return widget_stack_->id(widget_stack_->visibleWidget());
		}

	} // namespace VIEW
} // namespace BALL
