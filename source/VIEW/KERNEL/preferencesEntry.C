// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: preferencesEntry.C,v 1.9 2004/09/29 21:14:25 amoll Exp $
//

#include <BALL/VIEW/KERNEL/preferencesEntry.h>
#include <BALL/VIEW/DATATYPE/colorRGBA.h>

#include <qslider.h>
#include <qlabel.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qlineedit.h>
#include <qbuttongroup.h>
#include <qcolordialog.h>

using namespace std;

namespace BALL
{
	namespace VIEW
	{

		PreferencesEntry::PreferencesEntry()
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
					inifile.insertValue(inifile_section_name_, name, String(((QSlider*)(*it))->value()));
				}
				else if (RTTI::isKindOf<QLabel>(**it))
				{
					inifile.insertValue(inifile_section_name_, name, getLabelColor_((QLabel*)*it));
				}
				else if (RTTI::isKindOf<QLineEdit>(**it))
				{
					inifile.insertValue(inifile_section_name_, name, ((QLineEdit*)(*it))->text().ascii());
				}
				else if (RTTI::isKindOf<QCheckBox>(**it))
				{
					inifile.insertValue(inifile_section_name_, name, String(((QCheckBox*)*it)->isChecked()));
				}
				else if (RTTI::isKindOf<QComboBox>(**it))
				{
					inifile.insertValue(inifile_section_name_, name, String(((QComboBox*)*it)->currentItem()));
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
						((QSlider*) *it)->setValue(value.toInt());
					}
					else if (RTTI::isKindOf<QLabel>(**it))
					{
						setLabelColor_((QLabel*)*it, ColorRGBA(value));
					}
					else if (RTTI::isKindOf<QLineEdit>(**it))
					{
						((QLineEdit*)(*it))->setText(value);
					}
					else if (RTTI::isKindOf<QCheckBox>(**it))
					{
						((QCheckBox*) *it)->setChecked(value == "1");
					}
					else if (RTTI::isKindOf<QComboBox>(**it))
					{
						if (value.toUnsignedInt() >= (Position)((QComboBox*) *it)->count()) continue;
						((QComboBox*) *it)->setCurrentItem(value.toInt());
					}
					else if (RTTI::isKindOf<QButtonGroup>(**it))
					{
						((QButtonGroup*) *it)->setButton(value.toInt());
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

			if (widget->name() == "")
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

		ColorRGBA PreferencesEntry::chooseColor_(QLabel* label)
		{
			QColor qcolor = QColorDialog::getColor(label->backgroundColor());
			if (!qcolor.isValid()) return ColorRGBA();

			label->setBackgroundColor(qcolor);
			return ColorRGBA(qcolor);
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

	} // namespace VIEW
} // namespace BALL
