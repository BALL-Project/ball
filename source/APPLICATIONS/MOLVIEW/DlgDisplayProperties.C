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
	Inherited( parent, name )
{
    setCaption("Display Settings");
}

DlgDisplayProperties::~DlgDisplayProperties()
{
}

void DlgDisplayProperties::setObjectProcessor
  (const MoleculeObjectProcessor& object_processor)
{
	object_processor_ = &(const_cast<MoleculeObjectProcessor &>(object_processor));
}

void DlgDisplayProperties::selectPrecision(const QString& string)
{
	cerr << "precision" << endl;


	if (string == "low")
	{
		object_processor_
			->setValue(ADDRESS__STATIC_DRAWING_PRECISION, VALUE__DRAWING_PRECISION_LOW);
		object_processor_
			->setValue(ADDRESS__DYNAMIC_DRAWING_PRECISION, VALUE__DRAWING_PRECISION_LOW);
	}
	else if (string == "medium")
	{
		object_processor_
			->setValue(ADDRESS__STATIC_DRAWING_PRECISION, VALUE__DRAWING_PRECISION_MEDIUM);
		object_processor_
			->setValue(ADDRESS__DYNAMIC_DRAWING_PRECISION, VALUE__DRAWING_PRECISION_LOW);
	}
	else if (string == "high")
	{
		object_processor_
			->setValue(ADDRESS__STATIC_DRAWING_PRECISION, VALUE__DRAWING_PRECISION_HIGH);
		object_processor_
			->setValue(ADDRESS__DYNAMIC_DRAWING_PRECISION, VALUE__DRAWING_PRECISION_LOW);
	}
	else if (string == "ultra")
	{
		object_processor_
			->setValue(ADDRESS__STATIC_DRAWING_PRECISION, VALUE__DRAWING_PRECISION_ULTRA);
		object_processor_
			->setValue(ADDRESS__DYNAMIC_DRAWING_PRECISION, VALUE__DRAWING_PRECISION_LOW);
	}
}
 
void DlgDisplayProperties::selectModel(const QString& string)
{
	if (string == "none")
	{
		object_processor_->setValue(ADDRESS__STATIC_MODEL, VALUE__MODEL_REMOVE);
		object_processor_->setValue(ADDRESS__DYNAMIC_MODEL, VALUE__MODEL_REMOVE);
	}
	else if (string == "line")
	{
		object_processor_->setValue(ADDRESS__STATIC_MODEL, VALUE__MODEL_LINES);
		object_processor_->setValue(ADDRESS__DYNAMIC_MODEL, VALUE__MODEL_LINES);
	}
	else if (string == "stick")
	{
 		object_processor_->setValue(ADDRESS__STATIC_MODEL, VALUE__MODEL_STICK);
		object_processor_->setValue(ADDRESS__DYNAMIC_MODEL, VALUE__MODEL_LINES);
	}
	else if (string == "ball and stick")
	{
		object_processor_->setValue(ADDRESS__STATIC_MODEL, VALUE__MODEL_BALL_AND_STICK);
		object_processor_->setValue(ADDRESS__DYNAMIC_MODEL, VALUE__MODEL_LINES);
	}
	else if (string == "van der Waals")
	{
		object_processor_->setValue(ADDRESS__STATIC_MODEL, VALUE__MODEL_VAN_DER_WAALS);
		object_processor_->setValue(ADDRESS__DYNAMIC_MODEL, VALUE__MODEL_LINES);
	}
}

void DlgDisplayProperties::selectColoringMethod(const QString& string)
{
	if (string == "by element")
	{
		object_processor_->setColorCalculator(COLORCALCULATOR_VALUES__ELEMENT);
	}
	else if (string == "by residue name")
	{
		object_processor_->setColorCalculator(COLORCALCULATOR_VALUES__RESIDUE_NAME);
	}
	else if (string == "by atom charge")
	{
		object_processor_->setColorCalculator(COLORCALCULATOR_VALUES__ATOM_CHARGE);
	}
	else if (string == "custom")
	{
		ColorRGBA color;
		QColor qcolor = color_sample->backgroundColor();

		color.set(qcolor.red(), qcolor.green(), qcolor.blue());

		object_processor_->setColorCalculator(COLORCALCULATOR_VALUES__CUSTOM, color);
	}
}

void DlgDisplayProperties::applyButtonClicked()
{
	emit apply();
}

void DlgDisplayProperties::editColor()
{
	color_sample->setBackgroundColor(QColorDialog::getColor(color_sample->backgroundColor()));
}
