/****************************************************************************
** Form implementation generated from reading ui file 'colormeshdialog.ui'
**
** Created: Tue Sep 24 17:28:03 2002
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include <BALL/VIEW/GUI/DIALOGS/colorMeshDialogData.h>

#include <qvariant.h>
#include <qframe.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

using namespace BALL;
using namespace VIEW;

/* 
 *  Constructs a colorMeshDialogData which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
colorMeshDialogData::colorMeshDialogData( QWidget* parent,  const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "colorMeshDialogData" );
    resize( 407, 402 ); 
    setCaption( trUtf8( "Color Mesh" ) );
    setSizeGripEnabled( TRUE );

    TextLabel1 = new QLabel( this, "TextLabel1" );
    TextLabel1->setGeometry( QRect( 20, 150, 90, 30 ) ); 
    TextLabel1->setText( trUtf8( "Load File" ) );

    Loadfile_ = new QLineEdit( this, "Loadfile_" );
    Loadfile_->setGeometry( QRect( 110, 150, 190, 31 ) ); 
    Loadfile_->setText( trUtf8( "" ) );

    Browseload = new QPushButton( this, "Browseload" );
    Browseload->setGeometry( QRect( 310, 150, 81, 31 ) ); 
    Browseload->setText( trUtf8( "Browse" ) );

    Compute_ = new QPushButton( this, "Compute_" );
    Compute_->setGeometry( QRect( 30, 340, 133, 35 ) ); 
    Compute_->setText( trUtf8( "OK" ) );

    Cancel_ = new QPushButton( this, "Cancel_" );
    Cancel_->setGeometry( QRect( 240, 340, 133, 35 ) ); 
    Cancel_->setText( trUtf8( "Cancel" ) );

    Line3 = new QFrame( this, "Line3" );
    Line3->setGeometry( QRect( 0, 320, 400, 20 ) ); 
    Line3->setFrameShape( QFrame::HLine );
    Line3->setFrameShadow( QFrame::Sunken );
    Line3->setFrameShape( QFrame::HLine );
}

/*  
 *  Destroys the object and frees any allocated resources
 */
colorMeshDialogData::~colorMeshDialogData()
{
    // no need to delete child widgets, Qt does it all for us
}

