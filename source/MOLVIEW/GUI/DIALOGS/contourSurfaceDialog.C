/****************************************************************************
** Form implementation generated from reading ui file 'contourSurfaceDialog.ui'
**
** Created: Thu Sep 5 09:25:57 2002
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include <BALL/MOLVIEW/GUI/DIALOGS/contourSurfaceDialog.h>

#include <BALL/SYSTEM/path.h>
#include <qframe.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qvariant.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qfiledialog.h>


/* 
 *  Constructs a contourSurfaceDialog which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */

namespace BALL
{
 namespace MOLVIEW
   {

ContourSurfaceDialog::ContourSurfaceDialog( QWidget* parent,  const char* name )
    : QDialog( parent, name, TRUE, 0 )
{
    if ( !name )
	setName( "contourSurfaceDialog" );
    resize( 424, 318 ); 
    setCaption( tr( "Contour Surface" ) );

    TextLabel1_2_2 = new QLabel( this, "TextLabel1_2_2" );
    TextLabel1_2_2->setGeometry( QRect( 20, 150, 80, 21 ) ); 
    TextLabel1_2_2->setText( tr( "Save file:" ) );

    Line1 = new QFrame( this, "Line1" );
    Line1->setGeometry( QRect( 10, 240, 401, 20 ) ); 
    Line1->setFrameStyle( QFrame::HLine | QFrame::Sunken );

    Line1_2 = new QFrame( this, "Line1_2" );
    Line1_2->setGeometry( QRect( 10, 50, 401, 20 ) ); 
    Line1_2->setFrameStyle( QFrame::HLine | QFrame::Sunken );

    TextLabel1 = new QLabel( this, "TextLabel1" );
    TextLabel1->setGeometry( QRect( 20, 20, 80, 40 ) ); 
    TextLabel1->setText( tr( "Threshold:" ) );

    threshold_ = new QLineEdit( this, "threshold_" );
    threshold_->setGeometry( QRect( 110, 20, 91, 31 ) ); 
    threshold_->setText( tr( "0.5" ) );

    TextLabel1_2 = new QLabel( this, "TextLabel1_2" );
    TextLabel1_2->setGeometry( QRect( 20, 110, 70, 21 ) ); 
    //TextLabel1_2->setFrameShape( QLabel::NoFrame );
    //TextLabel1_2->setFrameShadow( QLabel:: );
    TextLabel1_2->setText( tr( "Load file:" ) );

    Loadfile_ = new QLineEdit( this, "Loadfile_" );
    Loadfile_->setGeometry( QRect( 110, 100, 190, 31 ) ); 
    Loadfile_->setText( tr( "" ) );

    Savefile_ = new QLineEdit( this, "Savefile_" );
    Savefile_->setGeometry( QRect( 110, 140, 190, 31 ) ); 
    Savefile_->setText( tr( "" ) );

    Compute_ = new QPushButton( this, "Compute_" );
    Compute_->setGeometry( QRect( 20, 260, 133, 35 ) ); 
    Compute_->setText( tr( "Compute Surface" ) );
    connect(Compute_, SIGNAL(clicked()), SLOT(accept()));

    Cancel_ = new QPushButton( this, "Cancel_" );
    Cancel_->setGeometry( QRect( 170, 260, 133, 35 ) ); 
    Cancel_->setText( tr( "Cancel" ) );
    connect(Cancel_, SIGNAL(clicked()), SLOT(reject()));

    Browseload = new QPushButton( this, "Browseload" );
    Browseload->setGeometry( QRect( 320, 100, 81, 31 ) ); 
    Browseload->setText( tr( "Browse" ) );
    connect(Browseload, SIGNAL(clicked()), SLOT(browseLoadFiles()));

    Browsesave = new QPushButton( this, "Browsesave" );
    Browsesave->setGeometry( QRect( 320, 140, 81, 31 ) ); 
    Browsesave->setText( tr( "Browse" ) );
    connect(Browsesave, SIGNAL(clicked()), SLOT(browseSaveFiles()));

    // signals and slots connections
}

/*  
 *  Destroys the object and frees any allocated resources
 */
ContourSurfaceDialog::~ContourSurfaceDialog()
{
    // no need to delete child widgets, Qt does it all for us
}

void ContourSurfaceDialog::new_slot()
{
    qWarning( "contourSurfaceDialog::new_slot(): Not implemented yet!" );
}

/*double ContourSurfaceDialog::getThreshold() const	
{
	return (double)String(threshold_->text().toFloat());
}*/

const String& ContourSurfaceDialog::getSaveName() const
{
	static String filename;
	filename = Savefile_->text().ascii();
	return filename;
}

const String& ContourSurfaceDialog::getLoadName() const
{
	static String filename;
	filename = Loadfile_->text().ascii();
	return filename;
}

void ContourSurfaceDialog::setSaveName(const String& filename)
{
	Savefile_->setText(filename.c_str());
	Savefile_->update();
}

void ContourSurfaceDialog::setLoadName(const String& filename)
{
	Loadfile_->setText(filename.c_str());
	Loadfile_->update();
}

void ContourSurfaceDialog::browseLoadFiles()
{
	// look up the full path of the parameter file
	Path p;
	String filename = p.find(getLoadName());
	if (filename == "")
	{
		filename = getLoadName();
	}
	QString tmp = filename.c_str();
	QString result = QFileDialog::getOpenFileName(tmp, "*", 0, "Select a RegularData file");
	if (!result.isEmpty())
	{
		// store the new filename in the lineedit field
		setLoadName(String(result.ascii()));
	}
}

void ContourSurfaceDialog::browseSaveFiles()
{
	// look up the full path of the parameter file
	Path p;
	String filename = p.find(getSaveName());
	if (filename == "")
	{
		filename = getSaveName();
	}
	QString tmp = filename.c_str();
	QString result = QFileDialog::getOpenFileName(tmp, "*", 0, "Select a RegularData file");
	if (!result.isEmpty())
	{
		// store the new filename in the lineedit field
		setSaveName(String(result.ascii()));
	}

}
}
}
