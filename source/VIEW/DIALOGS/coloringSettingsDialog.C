// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#include <BALL/VIEW/DIALOGS/coloringSettingsDialog.h>
#include <BALL/VIEW/MODELS/standardColorProcessor.h>
#include <BALL/FORMAT/INIFile.h>
#include <BALL/KERNEL/PTE.h>

#include <qtabwidget.h>
#include <qcolordialog.h>
#include <qslider.h>
#include <qlabel.h>

namespace BALL
{
	namespace VIEW
	{

QColorTableItem::QColorTableItem(QTable *t, EditType et, const ColorRGBA& color)
	: QTableItem(t,et,""),
		color_rgba_(color)
{ 
}

void QColorTableItem::paint(QPainter *p, const QColorGroup &cg, const QRect &cr, bool selected)
{
  QColorGroup g( cg );
	g.setColor( QColorGroup::Base, QColor (color_rgba_.getRed(), color_rgba_.getGreen(), color_rgba_.getBlue()));
  QTableItem::paint( p, g, cr, selected );
}

// ==============================================================================================
QColorTable::QColorTable(QWidget* parent) 
	throw()
	: QTable(parent) 
{
  setNumCols(2);
  horizontalHeader()->setLabel(1, "Color");
	setGeometry(5,5, 260, 250);
}

void QColorTable::setNamesTitle(const String& name)
	throw()
{
  horizontalHeader()->setLabel(0, name.c_str());
}
	
void QColorTable::setContent(const vector<String>& names, const vector<ColorRGBA>& colors)
	throw()
{
	colors_ = colors;
	names_ = names;

  setNumRows(colors_.size());
	for (Position p = 0; p < names_.size(); p++)
	{
    QColorTableItem* c2 = new QColorTableItem(this, QTableItem::WhenCurrent, colors_[p]);
    setText(p, 0, names_[p].c_str());
    setItem(p, 1, c2 );
	}
}

void QColorTable::setColors(const vector<ColorRGBA>& colors)
	throw()
{
	colors_ = colors;

  setNumRows(colors_.size());
	for (Position p = 0; p < names_.size(); p++)
	{
    QColorTableItem* c2 = new QColorTableItem(this, QTableItem::WhenCurrent, colors_[p]);
    setItem(p, 1, c2 );
	}
}

QWidget* QColorTable::beginEdit(int row, int col, bool)
{
	if (col == 0) return 0;
	ColorRGBA old_rgba(((QColorTableItem*)item(row,col))->getColor());
	QColor old(old_rgba.getRed(), old_rgba.getGreen(), old_rgba.getBlue());
	QColor color = QColorDialog::getColor(old);
	if (!color.isValid()) return 0;

	((QColorTableItem*)item(row,col))->setColor(
		ColorRGBA((Size)color.red(),(Size) color.green(),(Size) color.blue(),(Size) 255));
	updateCell(row, col);
	return 0;
}
				
// =========================================================================================
ColoringSettingsDialog::ColoringSettingsDialog( QWidget* parent,  const char* name, WFlags fl )
    : ColoringSettingsDialogData( parent, name, fl )
{
	element_table_ = new QColorTable(tabs->page(0));
	residue_table_ = new QColorTable(tabs->page(2));
	setDefaults();
}

void ColoringSettingsDialog::writePreferences(INIFile& file)
	throw()
{
	file.appendSection("COLORING_OPTIONS");
	writePreference_(file, "minimum_occupancy_color", minimum_occupancy_color_);
	writePreference_(file, "middle_residue_color", middle_residue_color_);
	writePreference_(file, "last_residue_color", last_residue_color_);
	writePreference_(file, "negative_charge_color", negative_charge_color_);
	writePreference_(file, "neutral_charge_color", neutral_charge_color_);
	writePreference_(file, "positive_charge_color", positive_charge_color_);
	writePreference_(file, "null_distance_color", null_distance_color_);
	writePreference_(file, "max_distance_color", max_distance_color_);
	writePreference_(file, "minimum_tf_color", minimum_tf_color_);
	writePreference_(file, "maximum_tf_color", maximum_tf_color_);
	writePreference_(file, "unassigned_tf_color", unassigned_tf_color_);
	writePreference_(file, "maximum_occupancy_color", maximum_occupancy_color_);
	writePreference_(file, "unassigned_occupancy_color", unassigned_occupancy_color_);
	writePreference_(file, "first_residue_color", first_residue_color_);
	writePreference_(file, "helix_color", helix_color_);
	writePreference_(file, "coil_color", coil_color_);
	writePreference_(file, "strand_color", strand_color_);
	writePreference_(file, "turn_color", turn_color_);

	for (Position p = 0; p < element_table_->getColors().size(); p ++)
	{
		writePreference_(file, String(p).c_str(), element_table_->getColors()[p]);
	}

	for (Position p = 0; p < residue_table_->getColors().size(); p ++)
	{
		writePreference_(file, residue_table_->getNames()[p].c_str(), 
										 residue_table_->getColors()[p]);
	}

	file.insertValue("COLORING_OPTIONS", "max_distance", 
			String((float)max_distance_slider->value() / 10.0).c_str() );
	file.insertValue("COLORING_OPTIONS", "max_tf", 
			String((float)max_tf_slider->value() / 10.0).c_str());
}

void ColoringSettingsDialog::fetchPreferences(const INIFile& file)
	throw()
{
	if (!file.hasSection("COLORING_OPTIONS"))
	{
		//setDefaults();
		return;
	}

	fetchPreference_(file, "minimum_occupancy_color", minimum_occupancy_color_);
	fetchPreference_(file, "middle_residue_color", middle_residue_color_);
	fetchPreference_(file, "last_residue_color", last_residue_color_);
	fetchPreference_(file, "negative_charge_color", negative_charge_color_);
	fetchPreference_(file, "neutral_charge_color", neutral_charge_color_);
	fetchPreference_(file, "positive_charge_color", positive_charge_color_);
	fetchPreference_(file, "null_distance_color", null_distance_color_);
	fetchPreference_(file, "max_distance_color", max_distance_color_);
	fetchPreference_(file, "minimum_tf_color", minimum_tf_color_);
	fetchPreference_(file, "maximum_tf_color", maximum_tf_color_);
	fetchPreference_(file, "unassigned_tf_color", unassigned_tf_color_);
	fetchPreference_(file, "maximum_occupancy_color", maximum_occupancy_color_);
	fetchPreference_(file, "unassigned_occupancy_color", unassigned_occupancy_color_);
	fetchPreference_(file, "first_residue_color", first_residue_color_);
	fetchPreference_(file, "helix_color", helix_color_);
	fetchPreference_(file, "coil_color", coil_color_);
	fetchPreference_(file, "strand_color", strand_color_);
	fetchPreference_(file, "turn_color", turn_color_);

	vector<ColorRGBA> colors;
	for (Position p = 0; p < 112; p ++)
	{
		ColorRGBA color;
		if (!fetchPreference_(file, String(p), color)) break;
		colors.push_back(color);
	}
	if (colors.size() == element_table_->getNames().size())
	{
		element_table_->setColors(colors);
	}

	colors.clear();
	for (Position p = 0; p < residue_table_->getColors().size(); p ++)
	{
		ColorRGBA color;
		if (!fetchPreference_(file, residue_table_->getNames()[p], color)) break;
		colors.push_back(color);
	}
	if (colors.size() == residue_table_->getColors().size())
	{
		residue_table_->setColors(colors);
	}
	
	
	if (file.hasEntry("COLORING_OPTIONS", "max_distance")) 
	{
		max_distance_slider->setValue((Size)(file.getValue("COLORING_OPTIONS", "max_distance").toFloat() * 10.0));
	}

	if (file.hasEntry("COLORING_OPTIONS", "max_tf")) 
	{
		max_tf_slider->setValue((Size)(file.getValue("COLORING_OPTIONS", "max_tf").toFloat() * 10.0));
	}

	setLabelColorsFromValues_();
}

void ColoringSettingsDialog::setDefaults()
	throw()
{
	vector<String> 		names;
	vector<ColorRGBA> colors;

	// setting residue colors
	// create a dummy processor to get the default values
	ElementColorProcessor elp;
	const HashMap<Position, ColorRGBA>& color_hash_map = elp.getColorMap();
	HashMap<Position, ColorRGBA>::ConstIterator it = color_hash_map.begin();

	for(; it != color_hash_map.end(); it++)
	{
		if (it->first == 0) continue;
		names.push_back(PTE[it->first].getSymbol());
		colors.push_back(it->second);
	}

	names.push_back(PTE[0].getSymbol());
	colors.push_back(color_hash_map[0]);
	element_table_->setNamesTitle("Element");
	element_table_->setContent(names, colors);
	names.clear();
	colors.clear();

	// =============================================================
	// setting residue name colors
	// create a dummy processor to get the default values
	ResidueNameColorProcessor rcp;
	const StringHashMap<ColorRGBA>& color_map = rcp.getColorMap();
	StringHashMap<ColorRGBA>::ConstIterator it2 = color_map.begin();

	for(; it2 != color_map.end(); it2++)
	{
		names.push_back(it2->first);
		colors.push_back(it2->second);
	}

	residue_table_->setNamesTitle("Residue");
	residue_table_->setContent(names, colors);
	
	// =============================================================
	first_residue_color_.set(255,255,0);
	middle_residue_color_.set(0,255,0);
	last_residue_color_.set(0,0,255);
	// =============================================================
	negative_charge_color_.set(255,0,0);
	neutral_charge_color_.set(255,255,255);
	positive_charge_color_.set(0,0,255);
	// =============================================================
	null_distance_color_.set(255,0,0);
	max_distance_color_.set(0,0,255);
	max_distance_slider->setValue(10 * 10);
	// =============================================================
	minimum_tf_color_.set(0,0,255);
	maximum_tf_color_.set(255,255,0);
	unassigned_tf_color_.set(255,255,255);
	max_tf_slider->setValue(50 * 10);
	// =============================================================
	minimum_occupancy_color_.set(0,0,255);
	maximum_occupancy_color_.set(255,255,0);
	unassigned_occupancy_color_.set(255,255,255);
	// =============================================================
  helix_color_.set(0,0,255);
	coil_color_.set(0,155,155);
	strand_color_.set(255,0,0);
	turn_color_.set(255,255,0);

	// =============================================================
	setLabelColorsFromValues_();
}

vector<ColorRGBA> ColoringSettingsDialog::getElementColors() const
	throw()
{
	vector<ColorRGBA> colors;
	colors.push_back(((QColorTableItem*)element_table_->item(element_table_->numRows()-1, 1))->getColor());
	for (Position p = 0; p < (Position)element_table_->numRows(); p++)
	{
		colors.push_back(((QColorTableItem*)element_table_->item(p, 1))->getColor());
	}

	return colors;
}

vector<ColorRGBA> ColoringSettingsDialog::getResidueColors() const
	throw()
{
	vector<ColorRGBA> colors;
	colors.push_back(((QColorTableItem*)residue_table_->item(residue_table_->numRows()-1, 1))->getColor());
	for (Position p = 0; p < (Position)residue_table_->numRows(); p++)
	{
		colors.push_back(((QColorTableItem*)residue_table_->item(p, 1))->getColor());
	}

	return colors;
}

void ColoringSettingsDialog::applySettingsTo(ColorProcessor& cp) const
	throw()
{
	if (RTTI::isKindOf<CustomColorProcessor>(cp))
	{
		return;
	}

	if (RTTI::isKindOf<ElementColorProcessor>(cp))
	{
		vector<ColorRGBA> colors = getElementColors();
		for (Position p = 0; p < colors.size(); p++)
		{
			(*(ElementColorProcessor*)&cp).getColorMap()[p] = colors[p];
		}
		return;
	}
	
	if (RTTI::isKindOf<ResidueNameColorProcessor>(cp))
	{
		for (Position p = 0; p < (Position)residue_table_->numRows(); p++)
		{
			(*(ResidueNameColorProcessor*)&cp).getColorMap()[residue_table_->item(p,0)->text().ascii()] = 
				((QColorTableItem*)residue_table_->item(p,1))->getColor();
		}
		return;
	}

	if (RTTI::isKindOf<ResidueNumberColorProcessor>(cp))
	{
		ResidueNumberColorProcessor& dp = (*(ResidueNumberColorProcessor*)&cp);
		dp.setFirstColor(first_residue_color_);
		dp.setMiddleColor(middle_residue_color_);
		dp.setLastColor(last_residue_color_);
		return;
	}

	if (RTTI::isKindOf<AtomChargeColorProcessor>(cp))
	{
		AtomChargeColorProcessor& dp = (*(AtomChargeColorProcessor*)&cp);
		dp.setPositiveColor(positive_charge_color_);
		dp.setNegativeColor(negative_charge_color_);
		dp.setNeutralColor(neutral_charge_color_);
		return;
	}

	if (RTTI::isKindOf<AtomDistanceColorProcessor>(cp))
	{
		AtomDistanceColorProcessor& dp = (*(AtomDistanceColorProcessor*)&cp);
		dp.setNullDistanceColor(null_distance_color_);
		dp.setMaxDistanceColor(max_distance_color_);
		dp.setDistance(((float)max_distance_slider->value()) / 10.0);
		return;
	}

	if (RTTI::isKindOf<OccupancyColorProcessor>(cp))
	{
		OccupancyColorProcessor& dp = (*(OccupancyColorProcessor*)&cp);
		dp.setMinColor(minimum_occupancy_color_);
		dp.setMaxColor(maximum_occupancy_color_);
		return;
	}

	if (RTTI::isKindOf<SecondaryStructureColorProcessor>(cp))
	{
		SecondaryStructureColorProcessor& dp = (*(SecondaryStructureColorProcessor*)&cp);

		dp.setHelixColor(helix_color_);
		dp.setCoilColor(coil_color_);
		dp.setStrandColor(strand_color_);
		dp.setTurnColor(turn_color_);

		return;
	}

	if (RTTI::isKindOf<TemperatureFactorColorProcessor>(cp))
	{
		TemperatureFactorColorProcessor& dp = (*(TemperatureFactorColorProcessor*)&cp);
		dp.setMinMinColor(unassigned_tf_color_);
		dp.setMinColor(minimum_tf_color_);
		dp.setMaxColor(maximum_tf_color_);
		dp.setMaxMaxColor(unassigned_tf_color_);
		dp.setMaxValue(((float)max_tf_slider->value()) / 10.0);
		return;
	}
}
	

void ColoringSettingsDialog::setNewColor_(QLabel* label, ColorRGBA& to)
	throw()
{
	QColor qcolor = QColorDialog::getColor(label->backgroundColor());
	if (!qcolor.isValid()) return;

	label->setBackgroundColor(qcolor);
	to.set((float)qcolor.red() / 255.0,
				 (float)qcolor.green() / 255.0,
				 (float)qcolor.blue() / 255.0);
	update();
}

void ColoringSettingsDialog::minimumOccupancyColorPressed()
{
	setNewColor_(minimum_o_label, minimum_occupancy_color_);
}
	
void ColoringSettingsDialog::middleResidueColorPressed()
{
	setNewColor_(middle_residue_label, middle_residue_color_);
}
	
void ColoringSettingsDialog::lastResidueColorPressed()
{
	setNewColor_(last_residue_label, last_residue_color_);
}
	
void ColoringSettingsDialog::negativeChargeColorPressed()
{
	setNewColor_(negative_charge_label, negative_charge_color_);
}
	
void ColoringSettingsDialog::neutralChargeColorPressed()
{
	setNewColor_(neutral_charge_label, neutral_charge_color_);
}
	
void ColoringSettingsDialog::positiveChargeColorPressed()
{
	setNewColor_(positive_charge_label, positive_charge_color_);
}
	
void ColoringSettingsDialog::nullDistanceColorPressed()
{
	setNewColor_(null_distance_label, null_distance_color_);
}
	
void ColoringSettingsDialog::maxDistanceColorPressed()
{
	setNewColor_(max_distance_label, max_distance_color_);
}
	
void ColoringSettingsDialog::minimumTFColorPressed()
{
	setNewColor_(minimum_tf_label, minimum_tf_color_);
}
	
void ColoringSettingsDialog::maximumTFColorPressed()
{
	setNewColor_(maximum_tf_label, maximum_tf_color_);
}
	
void ColoringSettingsDialog::unassignedTFColorPressed()
{
	setNewColor_(unassigned_tf_label, unassigned_tf_color_);
}
	
void ColoringSettingsDialog::maximumOccupancyColorPressed()
{
	setNewColor_(maximum_o_label, maximum_occupancy_color_);
}
	
void ColoringSettingsDialog::unassignedOccupancyColorPressed()
{
	setNewColor_(unassigned_o_label, unassigned_occupancy_color_);
}

void ColoringSettingsDialog::firstResidueColorPressed()
{
	setNewColor_(first_residue_label, first_residue_color_);
}

void ColoringSettingsDialog::helixColorPressed()
{
	setNewColor_(helix_color_label, helix_color_);
}

void ColoringSettingsDialog::turnColorPressed()
{
	setNewColor_(turn_color_label, turn_color_);
}

void ColoringSettingsDialog::strandColorPressed()
{
	setNewColor_(strand_color_label, strand_color_);
}

void ColoringSettingsDialog::coilColorPressed()
{
	setNewColor_(coil_color_label, coil_color_);
}


void ColoringSettingsDialog::maxDistanceChanged()
{
	String text = String(((float)max_distance_slider->value()) / 10.0);
	text = text.trimRight("0");
	if (text.hasSuffix(".")) text += "0";
	max_distance_value_label->setText(text.c_str());
}
	
void ColoringSettingsDialog::maxTFChanged()
{
	String text = String(((float)max_tf_slider->value()) / 10.0);
	text = text.trimRight("0");
	if (text.hasSuffix(".")) text += "0";
	max_tf_label->setText(text.c_str());
}

void ColoringSettingsDialog::setColorToLabel_(QLabel* label, const ColorRGBA& color)
	throw()
{
	label->setBackgroundColor(QColor(color.getRed(), color.getGreen(), color.getBlue()));
}

void ColoringSettingsDialog::setLabelColorsFromValues_()
	throw()
{
	setColorToLabel_(minimum_o_label, minimum_occupancy_color_);
	setColorToLabel_(middle_residue_label, middle_residue_color_);
	setColorToLabel_(last_residue_label, last_residue_color_);
	setColorToLabel_(negative_charge_label, negative_charge_color_);
	setColorToLabel_(neutral_charge_label, neutral_charge_color_);
	setColorToLabel_(positive_charge_label, positive_charge_color_);
	setColorToLabel_(null_distance_label, null_distance_color_);
	setColorToLabel_(max_distance_label, max_distance_color_);
	setColorToLabel_(minimum_tf_label, minimum_tf_color_);
	setColorToLabel_(maximum_tf_label, maximum_tf_color_);
	setColorToLabel_(unassigned_tf_label, unassigned_tf_color_);
	setColorToLabel_(maximum_o_label, maximum_occupancy_color_);
	setColorToLabel_(unassigned_o_label, unassigned_occupancy_color_);
	setColorToLabel_(first_residue_label, first_residue_color_);
	setColorToLabel_(helix_color_label, helix_color_);
	setColorToLabel_(coil_color_label, coil_color_);
	setColorToLabel_(strand_color_label, strand_color_);
	setColorToLabel_(turn_color_label, turn_color_);
}
	

bool ColoringSettingsDialog::fetchPreference_(const INIFile& inifile, const String& entry, 
																						  ColorRGBA& color)
	throw()
{
	if (!inifile.hasEntry("COLORING_OPTIONS", entry)) return false;
	color = inifile.getValue("COLORING_OPTIONS", entry);
	return true;
}

void ColoringSettingsDialog::writePreference_(INIFile& inifile, const String& entry, 
																							const ColorRGBA& color) const
	throw()
{
	inifile.insertValue("COLORING_OPTIONS", entry, color);
}



} } // NAMESPACE
