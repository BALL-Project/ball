// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#include <BALL/VIEW/DIALOGS/coloringSettingsDialog.h>
#include <BALL/VIEW/MODELS/standardColorProcessor.h>

#include <qpainter.h>
#include <qtabwidget.h>

namespace BALL
{
	namespace VIEW
	{

QColorTableItem::QColorTableItem(QTable *t, EditType et, const ColorRGBA& color)
	: QTableItem(t,et,""),
		color_(color)
{ 
}

void QColorTableItem::paint(QPainter *p, const QColorGroup &cg, const QRect &cr, bool selected)
{
  QColorGroup g( cg );
	g.setColor( QColorGroup::Base, QColor (color_.getRed(), color_.getGreen(), color_.getBlue()));
  QTableItem::paint( p, g, cr, selected );
}

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

void QColorTable::setNamesName(const String& name)
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
    QColorTableItem* c2 = new QColorTableItem(this, QTableItem::Never, colors_[p]);
    setText(p, 0, names_[p].c_str());
    setItem(p, 1, c2 );
	}
}


// =========================================================================================
ColoringSettingsDialog::ColoringSettingsDialog( QWidget* parent,  const char* name, WFlags fl )
    : ColoringSettingsDialogData( parent, name, fl )
{
	element_table_ = new QColorTable(tabs->page(1));
	setDefaults();
	show();
}

void ColoringSettingsDialog::writePreferences(INIFile& file)
	throw()
{
}

void ColoringSettingsDialog::fetchPreferences(const INIFile& file)
	throw()
{
}

void ColoringSettingsDialog::setDefaults()
	throw()
{
	vector<String> 		names;
	vector<ColorRGBA> colors;
	ElementColorProcessor elp;
	for (Position p = 0; p < 112; p++)
	{
		names.push_back(PTE.getElement(p).getName());
		colors.push_back(ColorRGBA(255,0,0));
	}
	element_table_->setContent(names, colors);
}

} } // NAMESPACE
