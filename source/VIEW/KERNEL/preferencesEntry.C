// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: preferencesEntry.C,v 1.3 2004/09/28 21:41:05 amoll Exp $
//

#include <BALL/VIEW/KERNEL/preferencesEntry.h>
#include <BALL/VIEW/DATATYPE/colorRGBA.h>

#include <qslider.h>
#include <qlabel.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qbuttongroup.h>

using namespace std;

namespace BALL
{
	namespace VIEW
	{

PreferencesEntry::PreferencesEntry()
{
}

PreferencesEntry::~PreferencesEntry()
{
}

void PreferencesEntry::writePreferenceEntries(INIFile& inifile)
{
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
			// differ between colored and non colored labels
			QLabel* label = (QLabel*) *it;
			if (label->text() == "")
			{
				inifile.insertValue(inifile_section_name_, name, ColorRGBA(label->backgroundColor()));
			}
			else
			{
				inifile.insertValue(inifile_section_name_, name, String(label->text().ascii()));
			}
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
			inifile.insertValue(inifile_section_name_, name, String(((QButtonGroup*)*it)->selectedId()));
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

	} // namespace VIEW
} // namespace BALL
