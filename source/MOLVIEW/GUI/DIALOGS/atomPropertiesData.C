/****************************************************************************
** Form implementation generated from reading ui file 'atomProperties.ui'
**
** Created: Tue Dec 10 17:54:48 2002
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include <BALL/MOLVIEW/GUI/DIALOGS/atomProperties.h>
#include <qcombobox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qvariant.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/* 
 *  Constructs a AtomPropertiesData which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
AtomPropertiesData::AtomPropertiesData( QWidget* parent,  const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "AtomPropertiesData" );
    resize( 365, 350 ); 
    setMinimumSize( QSize( 365, 350 ) );
    setMaximumSize( QSize( 365, 350 ) );
    setCaption( tr( "Atom Properties" ) );

    TextLabel1 = new QLabel( this, "TextLabel1" );
    TextLabel1->setGeometry( QRect( 10, 10, 70, 21 ) ); 
    TextLabel1->setText( tr( "name:" ) );

    TextLabel4 = new QLabel( this, "TextLabel4" );
    TextLabel4->setGeometry( QRect( 10, 160, 70, 21 ) ); 
    TextLabel4->setText( tr( "charge:" ) );

    TextLabel8 = new QLabel( this, "TextLabel8" );
    TextLabel8->setGeometry( QRect( 10, 130, 70, 21 ) ); 
    TextLabel8->setText( tr( "radius:" ) );

    TextLabel3 = new QLabel( this, "TextLabel3" );
    TextLabel3->setGeometry( QRect( 10, 100, 70, 21 ) ); 
    TextLabel3->setText( tr( "element:" ) );

    apply_button = new QPushButton( this, "apply_button" );
    apply_button->setEnabled( FALSE );
    apply_button->setGeometry( QRect( 60, 300, 100, 34 ) ); 
    apply_button->setText( tr( "apply" ) );

    close_button = new QPushButton( this, "close_button" );
    close_button->setGeometry( QRect( 210, 300, 100, 34 ) ); 
    close_button->setText( tr( "cancel" ) );
    close_button->setDefault( TRUE );

    TextLabel6 = new QLabel( this, "TextLabel6" );
    TextLabel6->setGeometry( QRect( 10, 250, 70, 21 ) ); 
    TextLabel6->setText( tr( "force:" ) );

    TextLabel5 = new QLabel( this, "TextLabel5" );
    TextLabel5->setGeometry( QRect( 10, 220, 70, 21 ) ); 
    TextLabel5->setText( tr( "velocity:" ) );

    TextLabel7 = new QLabel( this, "TextLabel7" );
    TextLabel7->setGeometry( QRect( 10, 190, 70, 21 ) ); 
    TextLabel7->setFrameShape( QLabel::NoFrame );
    TextLabel7->setText( tr( "position:" ) );

    TextLabel2 = new QLabel( this, "TextLabel2" );
    TextLabel2->setGeometry( QRect( 10, 40, 70, 21 ) ); 
    TextLabel2->setText( tr( "type name:" ) );

    TextLabel2_2 = new QLabel( this, "TextLabel2_2" );
    TextLabel2_2->setGeometry( QRect( 10, 70, 70, 21 ) ); 
    TextLabel2_2->setText( tr( "type:" ) );

    name_edit = new QLineEdit( this, "name_edit" );
    name_edit->setGeometry( QRect( 90, 10, 260, 21 ) ); 

    radius_edit = new QLineEdit( this, "radius_edit" );
    radius_edit->setGeometry( QRect( 90, 130, 260, 21 ) ); 

    charge_edit = new QLineEdit( this, "charge_edit" );
    charge_edit->setGeometry( QRect( 90, 160, 260, 21 ) ); 

    position_edit = new QLineEdit( this, "position_edit" );
    position_edit->setEnabled( FALSE );
    position_edit->setGeometry( QRect( 90, 190, 260, 21 ) ); 
    position_edit->setText( tr( "" ) );

    velocity_edit = new QLineEdit( this, "velocity_edit" );
    velocity_edit->setEnabled( FALSE );
    velocity_edit->setGeometry( QRect( 90, 220, 260, 21 ) ); 

    force_edit = new QLineEdit( this, "force_edit" );
    force_edit->setEnabled( FALSE );
    force_edit->setGeometry( QRect( 90, 250, 260, 21 ) ); 

    type_name_edit = new QLineEdit( this, "type_name_edit" );
    type_name_edit->setGeometry( QRect( 90, 40, 260, 21 ) ); 

    type_edit = new QLineEdit( this, "type_edit" );
    type_edit->setGeometry( QRect( 90, 70, 60, 21 ) ); 

    element_box = new QComboBox( FALSE, this, "element_box" );
    element_box->setGeometry( QRect( 90, 100, 60, 21 ) ); 

    // signals and slots connections
    connect( apply_button, SIGNAL( clicked() ), this, SLOT( applyClicked() ) );
    connect( close_button, SIGNAL( clicked() ), this, SLOT( closeClicked() ) );
    connect( name_edit, SIGNAL( textChanged(const QString&) ), this, SLOT( changed() ) );
    connect( type_name_edit, SIGNAL( textChanged(const QString&) ), this, SLOT( changed() ) );
    connect( radius_edit, SIGNAL( textChanged(const QString&) ), this, SLOT( changed() ) );
    connect( charge_edit, SIGNAL( textChanged(const QString&) ), this, SLOT( changed() ) );
    connect( element_box, SIGNAL( activated(int) ), this, SLOT( changed() ) );
}

/*  
 *  Destroys the object and frees any allocated resources
 */
AtomPropertiesData::~AtomPropertiesData()
{
    // no need to delete child widgets, Qt does it all for us
}

void AtomPropertiesData::applyClicked()
{
    qWarning( "AtomPropertiesData::applyClicked(): Not implemented yet!" );
}

void AtomPropertiesData::closeClicked()
{
    qWarning( "AtomPropertiesData::closeClicked(): Not implemented yet!" );
}

void AtomPropertiesData::changed()
{
    qWarning( "AtomPropertiesData::changed(): Not implemented yet!" );
}

