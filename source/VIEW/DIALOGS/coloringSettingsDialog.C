// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#include <BALL/VIEW/DIALOGS/coloringSettingsDialog.h>
#include <BALL/VIEW/MODELS/standardColorProcessor.h>
#include <BALL/DATATYPE/hashMap.h>

#include <qpainter.h>
#include <qtabwidget.h>
#include <qcolordialog.h>

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

void QColorTableItem::setContentFromEditor( QWidget* )
{
}

QWidget* QColorTableItem::createEditor() const
{
	return 0;
}

// ==============================================================================================
QColorTable::QColorTable(QWidget* parent) 
	: QTable(parent) 
{
  setNumCols(2);
  horizontalHeader()->setLabel(1, "Color");
	setGeometry(5,5, 260, 250);
}

void QColorTable::initTable()
{
	vector<ColorRGBA>::iterator color_it = colors_.begin();
	vector<String>::iterator name_it = names_.begin();
}

void QColorTable::setNamesTitle(const String& name)
{
  horizontalHeader()->setLabel(0, name.c_str());
}
	
void QColorTable::setContent(const vector<String>& names, const vector<ColorRGBA>& colors)
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
	file.isValid();
}

void ColoringSettingsDialog::fetchPreferences(const INIFile& file)
	throw()
{
	file.isValid();
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

	if (RTTI::isKindOf<ResidueIndexColorProcessor>(cp))
	{
		return;
	}

	if (RTTI::isKindOf<AtomChargeColorProcessor>(cp))
	{
		return;
	}

	if (RTTI::isKindOf<AtomDistanceColorProcessor>(cp))
	{
		return;
	}

	if (RTTI::isKindOf<OccupancyColorProcessor>(cp))
	{
		return;
	}

	if (RTTI::isKindOf<SecondaryStructureColorProcessor>(cp))
	{
		return;
	}

	if (RTTI::isKindOf<TemperatureFactorColorProcessor>(cp))
	{
		return;
	}
}
	

} } // NAMESPACE
