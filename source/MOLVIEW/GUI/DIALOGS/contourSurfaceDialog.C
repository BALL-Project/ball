#include <BALL/MOLVIEW/GUI/DIALOGS/contourSurfaceDialog.h>
#include <BALL/SYSTEM/path.h>

#include <qlineedit.h>
#include <qpushbutton.h>
#include <qspinbox.h>
#include <qfiledialog.h>
#include <qcolordialog.h>

namespace BALL
{
	namespace MOLVIEW
	{

/* 
 *  Constructs a ContourSurfaceDialog which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
ContourSurfaceDialog::ContourSurfaceDialog( QWidget* parent,  const char* name, bool modal, WFlags fl )
    : ContourSurfaceDialogData( parent, name, modal, fl )
{
}

/*  
 *  Destroys the object and frees any allocated resources
 */
ContourSurfaceDialog::~ContourSurfaceDialog()
{
    // no need to delete child widgets, Qt does it all for us
}

/* 
 * public slot
 */
void ContourSurfaceDialog::apply_clicked()
{
	hide();
}
/* 
 * public slot
 */
void ContourSurfaceDialog::browse_clicked()
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
void ContourSurfaceDialog::cancel_clicked()
{
	hide();
}
/* 
 * public slot
 */
void ContourSurfaceDialog::choose_clicked()
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
void ContourSurfaceDialog::color_boxes_changed()
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
void ContourSurfaceDialog::location_changed()
{
	location = String(location_edit->text());
	if (location.size() != 0) apply_button->setEnabled(true);
		else	apply_button->setEnabled(false);
}
/* 
 * public slot
 */
void ContourSurfaceDialog::max_clicked()
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
void ContourSurfaceDialog::mid_clicked()
{
}
/* 
 * public slot
 */
void ContourSurfaceDialog::min_clicked()
{
}
/* 
 * public slot
 */
void ContourSurfaceDialog::min_min_clicked()
{
}
/* 
 * public slot
 */
void ContourSurfaceDialog::max_max_clicked()
{
}
/* 
 * public slot
 */
void ContourSurfaceDialog::tab_changed()
{
}


// NAMESPACE
	}
}
