// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#include <BALL/VIEW/GUI/DIALOGS/colorMeshDialog.h>
#include <BALL/SYSTEM/path.h>
#include <BALL/DATATYPE/regularData3D.h>
#include <BALL/SYSTEM/file.h>
#include <BALL/VIEW/DATATYPE/colorTable.h>
#include <BALL/VIEW/GUI/KERNEL/mainControl.h>

#include <qlineedit.h>
#include <qpushbutton.h>
#include <qspinbox.h>
#include <qfiledialog.h>
#include <qcolordialog.h>
#include <qtabwidget.h>

namespace BALL
{
	namespace VIEW
	{

/* 
 *  Constructs a ColorMeshDialog which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
ColorMeshDialog::ColorMeshDialog( QWidget* parent,  const char* name, bool modal, WFlags fl )
    : ColorMeshDialogData( parent, name, modal, fl )
{
}

ColorMeshDialog::~ColorMeshDialog()
{
 // no need to delete child widgets, Qt does it all for us
}

void ColorMeshDialog::apply_clicked()
{
	if (surface_tab->currentPage() == by_file)
	{
		// coloring by file
		RegularData3D dat;
		String filename = String(location_edit->text());
		File infile;
		
		try
		{
			infile.open(filename, std::ios::in);
		}
		catch(Exception::FileNotFound)
		{
			Log.error() << "File could not be found!" << std::endl;
			return;
		}

		infile >> dat;
		infile.close();

		// now do the colorizing stuff...
		mesh->colorList.resize(mesh->vertex.size());
		ColorRGBA list[3];
		list[0] = ColorRGBA(0.,0.,1.,1.);
		list[1] = ColorRGBA(0.,1.,1.,1.);
		list[2] = ColorRGBA(1.,1.,0.,1.);

		ColorTable table(list, 3);
		table.setNumberOfColors(levels_box->value());
		table.createTable();
		table.setRange(atof(min_box->text().latin1()), atof(max_box->text().latin1()));

		try 
		{
			for (Position i=0; i<mesh->colorList.size(); i++)
			{
				mesh->colorList[i] = table.map(dat[mesh->vertex[i]]);
			}
		}	
		catch (Exception::OutOfGrid)
		{
			Log.error() << "Error! There is a point contained in the surface that is not "
									<< "inside the grid! Aborting the coloring..." << std::endl;
			return;
		}
	}
	if (surface_tab->currentPage() == by_color)
	{
		ColorRGBA col(red_box->value(), green_box->value(), blue_box->value(), alpha_box->value());

		if (alpha_box->value() != 255)
		{
			mesh->setProperty(GeometricObject::PROPERTY__OBJECT_TRANSPARENT);
		}
		else
		{
			mesh->clearProperty(GeometricObject::PROPERTY__OBJECT_TRANSPARENT);
		}			

		mesh->colorList.resize(1);

		mesh->colorList[0] = col;
	}
		
	// repaint of the scene and the composites needed
	MainControl::getMainControl(this)->updateAll();
	MainControl::getMainControl(this)->repaint();

	hide();
}

void ColorMeshDialog::browse_clicked()
{
	// look up the full path of the parameter file
	Path p;
	String filename = p.find((String)location_edit->text());
	QString result = QFileDialog::getOpenFileName(filename.c_str(), "*", 0, "Select a RegularData file");
	if (!result.isEmpty())
	{
		// store the new filename in the lineedit field
		location_edit->setText(result);
	}
}

void ColorMeshDialog::cancel_clicked()
{
	hide();
}

QColor ColorMeshDialog::setColor(QPushButton* button)
{
	QPalette p = button->palette();
	QColor qcolor = QColorDialog::getColor(button->backgroundColor());
	p.setColor(QColorGroup::Button, qcolor);
	p.setColor(QColorGroup::Base, qcolor);
	p.setColor(QColorGroup::Light, qcolor);
	p.setColor(QColorGroup::Mid, qcolor);
	p.setColor(QColorGroup::Midlight, qcolor);
	p.setColor(QColorGroup::Shadow, qcolor);
	button->setPalette(p);
	return qcolor;
}

void ColorMeshDialog::choose_clicked()
{
	QColor qcolor = setColor(choose_button);
	selected_color.set(qcolor.red(), qcolor.green(), qcolor.blue());
	red_box->setValue(qcolor.red());
	blue_box->setValue(qcolor.blue());
	green_box->setValue(qcolor.green());
}

void ColorMeshDialog::color_boxes_changed()
{
	selected_color.set(red_box->value(),
										 green_box->value(),
										 blue_box->value(),
										 alpha_box->value());
	QColor qcolor(red_box->value(), green_box->value(), blue_box->value());
	QPalette p = choose_button->palette();
	p.setColor(QColorGroup::Button, qcolor);
	p.setColor(QColorGroup::Base, qcolor);
	p.setColor(QColorGroup::Light, qcolor);
	p.setColor(QColorGroup::Mid, qcolor);
	p.setColor(QColorGroup::Midlight, qcolor);
	p.setColor(QColorGroup::Shadow, qcolor);
	choose_button->setPalette(p);
}

void ColorMeshDialog::location_changed()
{
	if (String(location_edit->text()).size() != 0) apply_button->setEnabled(true);
		else	apply_button->setEnabled(false);
}

void ColorMeshDialog::max_clicked()
{
	QColor qcolor = setColor(max_button);
	max_color.set(qcolor.red(), qcolor.green(), qcolor.blue());;
}

void ColorMeshDialog::mid_clicked()
{
	QColor qcolor = setColor(mid_button);
	mid_color.set(qcolor.red(), qcolor.green(), qcolor.blue());;
}

void ColorMeshDialog::min_clicked()
{
	QColor qcolor = setColor(min_button);
	min_color.set(qcolor.red(), qcolor.green(), qcolor.blue());;

			
}

void ColorMeshDialog::min_min_clicked()
{
	QColor qcolor = setColor(min_min_button);
	min_min_color.set(qcolor.red(), qcolor.green(), qcolor.blue());;
}

void ColorMeshDialog::max_max_clicked()
{
	QColor qcolor = setColor(max_max_button);
	max_max_color.set(qcolor.red(), qcolor.green(), qcolor.blue());;
}

void ColorMeshDialog::tab_changed()
{
	if (surface_tab->currentPage() == by_file ||
			surface_tab->currentPage() == colormap_tab)
	{
		// in coloring by file, allow apply if filename set
		if (location_edit->text() == "")
		{
			apply_button->setEnabled(false);
		}
		else
		{
			apply_button->setEnabled(true);
		}
		return;
	}

	if (surface_tab->currentPage() == by_color)
	{
		// if coloring by selected color, always enabled
		apply_button->setEnabled(true);
	}
}


// NAMESPACE
	}
}
