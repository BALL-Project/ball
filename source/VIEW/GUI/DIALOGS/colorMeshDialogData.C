/****************************************************************************
** Form implementation generated from reading ui file 'colormeshdialog.ui'
**
** Created: Tue Sep 24 19:07:08 2002
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include <BALL/VIEW/GUI/DIALOGS/colorMeshDialog.h>

#include <qvariant.h>
#include <qframe.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qspinbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

using namespace BALL;
using namespace std;

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
    setCaption( ( "Color Mesh" ) );
    setSizeGripEnabled( TRUE );

    Compute_ = new QPushButton( this, "Compute_" );
    Compute_->setGeometry( QRect( 30, 340, 133, 35 ) ); 
    Compute_->setText( ( "OK" ) );

    Cancel_ = new QPushButton( this, "Cancel_" );
    Cancel_->setGeometry( QRect( 240, 340, 133, 35 ) ); 
    Cancel_->setText( ( "Cancel" ) );

    Line3 = new QFrame( this, "Line3" );
    Line3->setGeometry( QRect( 0, 320, 400, 20 ) ); 
    Line3->setFrameShape( QFrame::HLine );
    Line3->setFrameShadow( QFrame::Sunken );
    Line3->setFrameShape( QFrame::HLine );

    QWidget* privateLayoutWidget = new QWidget( this, "Layout2" );
    privateLayoutWidget->setGeometry( QRect( 40, 40, 340, 28 ) ); 
    Layout2 = new QHBoxLayout( privateLayoutWidget, 0, 6, "Layout2"); 

    TextLabel1 = new QLabel( privateLayoutWidget, "TextLabel1" );
    TextLabel1->setText( ( "Load File" ) );
    Layout2->addWidget( TextLabel1 );

    Loadfile_ = new QLineEdit( privateLayoutWidget, "Loadfile_" );
    Loadfile_->setText( ( "" ) );
    Layout2->addWidget( Loadfile_ );

    Browseload = new QPushButton( privateLayoutWidget, "Browseload" );
    Browseload->setText( ( "Browse" ) );
    Layout2->addWidget( Browseload );

    TextLabel2 = new QLabel( this, "TextLabel2" );
    TextLabel2->setGeometry( QRect( 40, 80, 110, 21 ) ); 
    TextLabel2->setText( ( "Number of Colors" ) );

    numberOfColors = new QSpinBox( this, "numberOfColors" );
    numberOfColors->setGeometry( QRect( 320, 80, 61, 21 ) ); 
    numberOfColors->setMinValue( 3 );

    TextLabel3 = new QLabel( this, "TextLabel3" );
    TextLabel3->setGeometry( QRect( 40, 120, 101, 21 ) ); 
    TextLabel3->setText( ( "Minimal value" ) );

    TextLabel3_2 = new QLabel( this, "TextLabel3_2" );
    TextLabel3_2->setGeometry( QRect( 40, 160, 101, 21 ) ); 
    TextLabel3_2->setText( ( "Maximal value" ) );

    MinVal = new QLineEdit( this, "MinVal" );
    MinVal->setGeometry( QRect( 190, 120, 191, 22 ) ); 
    MinVal->setText( ( "-1.0" ) );
    MinVal->setAlignment( int( QLineEdit::AlignRight ) );

    MaxVal = new QLineEdit( this, "MaxVal" );
    MaxVal->setGeometry( QRect( 191, 160, 190, 22 ) ); 
    MaxVal->setText( ( "1.0" ) );
    MaxVal->setAlignment( int( QLineEdit::AlignRight ) );
}

/*  
 *  Destroys the object and frees any allocated resources
 */
colorMeshDialogData::~colorMeshDialogData()
{
    // no need to delete child widgets, Qt does it all for us
}

