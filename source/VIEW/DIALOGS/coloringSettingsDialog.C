// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#include <BALL/VIEW/DIALOGS/coloringSettingsDialog.h>
#include <BALL/VIEW/MODELS/standardColorProcessor.h>

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

QWidget* QColorTable::beginEdit(int row, int col, bool replace)
{
	if (col == 0) return 0;
	QColor color = QColorDialog::getColor();
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
	setDefaults();
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
	const HashMap<Position, ColorRGBA>& color_hash_map = elp.getColorMap();

	for (Position p = 0; p < 111; p++)
	{
		names.push_back(PTE.getElement(p).getName());
		colors.push_back(color_hash_map[p]);
	}
	element_table_->setNamesTitle("Element");
	element_table_->setContent(names, colors);
}

} } // NAMESPACE
