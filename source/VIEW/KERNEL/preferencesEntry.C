// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: preferencesEntry.C,v 1.1 2004/09/28 16:44:51 amoll Exp $
//

#include <BALL/VIEW/KERNEL/preferencesEntry.h>
#include <BALL/VIEW/DATATYPE/colorRGBA.h>

#include <qslider.h>
#include <qlabel.h>
#include <qcheckbox.h>
#include <qlistbox.h>

using namespace std;

namespace BALL
{
	namespace VIEW
	{

		PreferencesEntry::PreferencesEntry()
			throw()
		{
		}

		PreferencesEntry::~PreferencesEntry()
			throw()
		{
		}

		void PreferencesEntry::writePreferences(INIFile& inifile)
		{
			if (!inifile.hasSection(inifile_section_name_)) return;

			List<QWidget*>::Iterator it = preferences_objects_.begin();
			for (; it != preferences_objects_.end(); it++)
			{
				if ((**it).name() == "")
				{
					Log.error() << "Unnamed Preferences object!" << std::endl;
					continue;
				}

				if (RTTI::isKindOf<QSlider>(**it))
				{
					inifile.insertValue(inifile_section_name_, (**it).name(), String(((QSlider*)(*it))->value()));
				}
				else if (RTTI::isKindOf<QLabel>(**it))
				{
					// differ between colored and non colored labels
					QLabel* label = (QLabel*) *it;
					if (label->text() == "")
					{
						inifile.insertValue(inifile_section_name_, (**it).name(), ColorRGBA(label->backgroundColor()));
					}
					else
					{
						inifile.insertValue(inifile_section_name_, (**it).name(), String(label->text().ascii()));
					}
				}
				else if (RTTI::isKindOf<QCheckBox>(**it))
				{
					inifile.insertValue(inifile_section_name_, (**it).name(), String(((QCheckBox*)*it)->isChecked()));
				}
				else if (RTTI::isKindOf<QListBox>(**it))
				{
					inifile.insertValue(inifile_section_name_, (**it).name(), String(((QListBox*)*it)->currentItem()));
				}
			}
		}

		void PreferencesEntry::readPreferences(const INIFile& inifile)
		{
			List<QWidget*>::Iterator it = preferences_objects_.begin();
			for (; it != preferences_objects_.end(); it++)
			{
				if ((**it).name() == "")
				{
					Log.error() << "Unnamed Preferences object!" << std::endl;
					continue;
				}


				if (!inifile.hasEntry(inifile_section_name_, (**it).name())) continue;

				String value = inifile.getValue(inifile_section_name_, (**it).name());

				if (RTTI::isKindOf<QSlider>(**it))
				{
					((QSlider*) *it)->setValue(value.toInt());
				}
				else if (RTTI::isKindOf<QLabel>(**it))
				{
					// differ between colored and non colored labels
					QLabel* label = (QLabel*) *it;
					if (String((**it).name()).hasSuffix("_color"))
					{
						label->setBackgroundColor(ColorRGBA(value).getQColor());
					}
					else
					{
						label->setText(value);
					}
				}
				else if (RTTI::isKindOf<QCheckBox>(**it))
				{
					((QCheckBox*) *it)->setChecked(value == "1");
				}
				else if (RTTI::isKindOf<QListBox>(**it))
				{
// 					inifile.insertValue(inifile_section_name_, (**it).name(), String(((QListBox*)*it)->currentItem()));
				}
			}
		}


	} // namespace VIEW
} // namespace BALL
