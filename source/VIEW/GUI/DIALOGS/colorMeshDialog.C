#include <BALL/VIEW/GUI/DIALOGS/colorMeshDialog.h>
#include <BALL/SYSTEM/path.h>
#include <BALL/VIEW/GUI/DIALOGS/colorMeshDialog.h>
#include <BALL/DATATYPE/regularData3D.h>
#include <BALL/SYSTEM/file.h>
#include <BALL/VIEW/DATATYPE/colorTable.h>
#include <BALL/VIEW/GUI/KERNEL/mainControl.h>

#include <qlineedit.h>
#include <qpushbutton.h>
#include <qspinbox.h>
#include <qfiledialog.h>
#include <qcolordialog.h>

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

/*  
 *  Destroys the object and frees any allocated resources
 */
ColorMeshDialog::~ColorMeshDialog()
{
    // no need to delete child widgets, Qt does it all for us
}

/* 
 * public slot
 */
void ColorMeshDialog::apply_clicked()
{
	String fileName(location_edit->text().latin1());

	if (!fileName.isEmpty())
	{
		RegularData3D dat;
		File infile(fileName, File::IN);
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

	// update of the scene and the composites needed
	MainControl::getMainControl(this)->updateAll();

	hide();
}
/* 
 * public slot
 */
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
/* 
 * public slot
 */
void ColorMeshDialog::cancel_clicked()
{
	hide();
}
/* 
 * public slot
 */
void ColorMeshDialog::choose_clicked()
{
	choose_button->setBackgroundColor(QColorDialog::getColor(choose_button->backgroundColor()));
	red_box->setValue(choose_button->backgroundColor().red());
	blue_box->setValue(choose_button->backgroundColor().blue());
	green_box->setValue(choose_button->backgroundColor().green());
	color.set(red_box->value(),
						blue_box->value(),
						green_box->value());
}
/* 
 * public slot
 */
void ColorMeshDialog::color_boxes_changed()
{
	choose_button->setBackgroundColor(QColor(red_box->value(), blue_box->value(), green_box->value()));
			color.set(red_box->value(),
								blue_box->value(),
								green_box->value(),
								alpha_box->value());
}
/* 
 * public slot
 */
void ColorMeshDialog::location_changed()
{
	location = String(location_edit->text());
	if (location.size() != 0) apply_button->setEnabled(true);
		else	apply_button->setEnabled(false);
}
/* 
 * public slot
 */
void ColorMeshDialog::max_clicked()
{
	max_button->setBackgroundColor(QColorDialog::getColor(max_button->backgroundColor()));
	max_color.set(max_button->backgroundColor().red(),
								max_button->backgroundColor().blue(),
								max_button->backgroundColor().green());
	max_button->setBackgroundColor(QColor(max_color.getRed(), max_color.getBlue(), max_color.getGreen()));
}
/* 
 * public slot
 */
void ColorMeshDialog::mid_clicked()
{
}
/* 
 * public slot
 */
void ColorMeshDialog::min_clicked()
{
}
/* 
 * public slot
 */
void ColorMeshDialog::min_min_clicked()
{
}
/* 
 * public slot
 */
void ColorMeshDialog::max_max_clicked()
{
}
/* 
 * public slot
 */
void ColorMeshDialog::tab_changed()
{
}


// NAMESPACE
	}
}
