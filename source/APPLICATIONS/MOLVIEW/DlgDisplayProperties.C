/**********************************************************************

	--- Qt Architect generated file ---

	File: DlgDisplayProperties.C
	Last generated: Mon Jan 10 18:55:27 2000

 *********************************************************************/

#include <qcolordialog.h>
#include "DlgDisplayProperties.h"

#define Inherited DlgDisplayPropertiesData

using namespace std;

DlgDisplayProperties::DlgDisplayProperties
(
	QWidget* parent,
	const char* name
)
	:
	Inherited( parent, name ),
  model_string_("stick"),
  precision_string_("high"),
  coloring_method_string_("by element")	
{
	setCaption("Display Settings");
}

DlgDisplayProperties::~DlgDisplayProperties()
{
}

void DlgDisplayProperties::setObjectProcessor
  (const MoleculeObjectProcessor& object_processor)
{
	object_processor_ = &(const_cast<MoleculeObjectProcessor&>(object_processor));
}

void DlgDisplayProperties::setPreferences(INIFile& inifile) const
{
	//	
	// the display window position
	//
	inifile.setValue
		("WINDOWS", "Display::x", String(x()));
	inifile.setValue
		("WINDOWS", "Display::y", String(y()));

	// 
	// the combobox values
	// 
	inifile.setValue
		("WINDOWS", "Display::model", model_string_.ascii());
	inifile.setValue
		("WINDOWS", "Display::precision", precision_string_.ascii());
	inifile.setValue
		("WINDOWS", "Display::colormethod", coloring_method_string_.ascii());
	inifile.setValue
		("WINDOWS", "Display::customcolor", custom_color_);
}

void DlgDisplayProperties::getPreferences(const INIFile& inifile)
{
	// 
	// the geometry of the main window
	//
	int x_pos = x();
	int y_pos = y();

	if (inifile.hasEntry("WINDOWS", "Display::x"))
	{
		x_pos = inifile.getValue("WINDOWS", "Display::x").toInt();
	}
	if (inifile.hasEntry("WINDOWS", "Display::y"))
	{
		y_pos = inifile.getValue("WINDOWS", "Display::y").toInt();
	}

	move(x_pos, y_pos);

	// 
	// the combobox values
	//

	if (inifile.hasEntry("WINDOWS", "Display::customcolor"))
	{
		custom_color_.set(inifile.getValue("WINDOWS", "Display::customcolor"));

		QColor qcolor(custom_color_.red(), 
									custom_color_.green(), 
									custom_color_.blue());

		color_sample->setBackgroundColor(qcolor);
	}

	if (inifile.hasEntry("WINDOWS", "Display::model"))
	{
		model_string_ = inifile.getValue("WINDOWS", "Display::model").c_str();

		setComboBoxIndex_(model_type_combobox_, model_string_);
		
		selectModel(model_string_);
	}
	
	if (inifile.hasEntry("WINDOWS", "Display::precision"))
	{
		precision_string_ = inifile.getValue("WINDOWS", "Display::precision").c_str();

		setComboBoxIndex_(mode_resolution_combobox_, precision_string_);

		selectPrecision(precision_string_);
	}
	
	if (inifile.hasEntry("WINDOWS", "Display::colormethod"))
	{
		coloring_method_string_ = inifile.getValue("WINDOWS", "Display::colormethod").c_str();

		setComboBoxIndex_(coloring_type_combobox_, coloring_method_string_);

		selectColoringMethod(coloring_method_string_);
	}
}

void DlgDisplayProperties::selectPrecision(const QString& string)
{
	precision_string_ = string;

	if (string == "low")
	{
		object_processor_
			->setValue(ADDRESS__STATIC_DRAWING_PRECISION, 
								 VALUE__DRAWING_PRECISION_LOW);
		object_processor_
			->setValue(ADDRESS__DYNAMIC_DRAWING_PRECISION, 
								 VALUE__DRAWING_PRECISION_LOW);
	}
	else if (string == "medium")
	{
		object_processor_
			->setValue(ADDRESS__STATIC_DRAWING_PRECISION, 
								 VALUE__DRAWING_PRECISION_MEDIUM);
		object_processor_
			->setValue(ADDRESS__DYNAMIC_DRAWING_PRECISION, 
								 VALUE__DRAWING_PRECISION_LOW);
	}
	else if (string == "high")
	{
		object_processor_
			->setValue(ADDRESS__STATIC_DRAWING_PRECISION, 
								 VALUE__DRAWING_PRECISION_HIGH);
		object_processor_
			->setValue(ADDRESS__DYNAMIC_DRAWING_PRECISION, 
								 VALUE__DRAWING_PRECISION_LOW);
	}
	else if (string == "ultra")
	{
		object_processor_
			->setValue(ADDRESS__STATIC_DRAWING_PRECISION, 
								 VALUE__DRAWING_PRECISION_ULTRA);
		object_processor_
			->setValue(ADDRESS__DYNAMIC_DRAWING_PRECISION, 
								 VALUE__DRAWING_PRECISION_LOW);
	}
}
 
void DlgDisplayProperties::selectModel(const QString& string)
{
	model_string_ = string;

	if (string == "none")
	{
		object_processor_
			->setValue(ADDRESS__STATIC_MODEL, VALUE__MODEL_REMOVE);
		object_processor_
			->setValue(ADDRESS__DYNAMIC_MODEL, VALUE__MODEL_REMOVE);
	}
	else if (string == "line")
	{
		object_processor_
			->setValue(ADDRESS__STATIC_MODEL, VALUE__MODEL_LINES);
		object_processor_
			->setValue(ADDRESS__DYNAMIC_MODEL, VALUE__MODEL_LINES);
	}
	else if (string == "stick")
	{
 		object_processor_
			->setValue(ADDRESS__STATIC_MODEL, VALUE__MODEL_STICK);
		object_processor_
			->setValue(ADDRESS__DYNAMIC_MODEL, VALUE__MODEL_LINES);
	}
	else if (string == "ball and stick")
	{
		object_processor_
			->setValue(ADDRESS__STATIC_MODEL, VALUE__MODEL_BALL_AND_STICK);
		object_processor_
			->setValue(ADDRESS__DYNAMIC_MODEL, VALUE__MODEL_LINES);
	}
	else if (string == "van der Waals")
	{
		object_processor_
			->setValue(ADDRESS__STATIC_MODEL, VALUE__MODEL_VAN_DER_WAALS);
		object_processor_
			->setValue(ADDRESS__DYNAMIC_MODEL, VALUE__MODEL_LINES);
	}
}

void DlgDisplayProperties::selectColoringMethod(const QString& string)
{
	coloring_method_string_ = string;

	if (string == "by element")
	{
		object_processor_
			->setColorCalculator(COLORCALCULATOR_VALUES__ELEMENT);
	}
	else if (string == "by residue name")
	{
		object_processor_
			->setColorCalculator(COLORCALCULATOR_VALUES__RESIDUE_NAME);
	}
	else if (string == "by atom charge")
	{
		object_processor_
			->setColorCalculator(COLORCALCULATOR_VALUES__ATOM_CHARGE);
	}
	else if (string == "custom")
	{
		ColorRGBA color;
		QColor qcolor = color_sample->backgroundColor();

		color.set((float)qcolor.red() / 255.0,
							(float)qcolor.green() / 255.0,
							(float)qcolor.blue() / 255.0);

		object_processor_
			->setColorCalculator(COLORCALCULATOR_VALUES__CUSTOM, color);
	}
}

void DlgDisplayProperties::applyButtonClicked()
{
	emit apply();
}

void DlgDisplayProperties::editColor()
{
	color_sample
		->setBackgroundColor(QColorDialog::getColor(color_sample->backgroundColor()));
	ColorRGBA color;
	QColor qcolor = color_sample->backgroundColor();
 	color.set((float)qcolor.red() / 255.0,
						(float)qcolor.green() / 255.0,
						(float)qcolor.blue() / 255.0);

	custom_color_ = color;

	object_processor_->setColorCalculator(COLORCALCULATOR_VALUES__CUSTOM, color);
}

void DlgDisplayProperties::setComboBoxIndex_(QComboBox* combo_box, QString& item_string)
{
	int index;
	bool found = false;

	for (index = 0; index < combo_box->count(); ++index)
	{
		if (combo_box->text(index) == item_string)
		{
			combo_box->setCurrentItem(index);

			found = true;
		}
	}

	if (found == false)
	{
		combo_box->setCurrentItem(0);
	}
}

