/****************************************************************************
** Form implementation generated from reading ui file 'atomsettings.ui'
**
** Created: Thu Nov 28 20:38:56 2002
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include <BALL/MOLVIEW/GUI/DIALOGS/atomSettingsData.h>

#include <qvariant.h>
#include <qframe.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/* 
 *  Constructs a AtomSettingsData which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
AtomSettingsData::AtomSettingsData( QWidget* parent,  const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "AtomSettingsData" );
    resize( 221, 506 ); 
    setCaption( trUtf8( "Atom-Settings" ) );

    QWidget* privateLayoutWidget = new QWidget( this, "Layout12" );
    privateLayoutWidget->setGeometry( QRect( 20, 470, 180, 30 ) ); 
    Layout12 = new QHBoxLayout( privateLayoutWidget, 0, 6, "Layout12"); 

    apply_button = new QPushButton( privateLayoutWidget, "apply_button" );
    apply_button->setText( trUtf8( "Apply" ) );
    Layout12->addWidget( apply_button );

    close_button = new QPushButton( privateLayoutWidget, "close_button" );
    close_button->setText( trUtf8( "Close" ) );
    Layout12->addWidget( close_button );

    Frame6 = new QFrame( this, "Frame6" );
    Frame6->setGeometry( QRect( 10, 10, 200, 41 ) ); 
    Frame6->setFrameShape( QFrame::StyledPanel );
    Frame6->setFrameShadow( QFrame::Sunken );

    TextLabel2 = new QLabel( Frame6, "TextLabel2" );
    TextLabel2->setGeometry( QRect( 12, 10, 52, 24 ) ); 
    TextLabel2->setText( trUtf8( "name:" ) );

    name_edit = new QLineEdit( Frame6, "name_edit" );
    name_edit->setGeometry( QRect( 70, 10, 125, 24 ) ); 

    Frame6_2 = new QFrame( this, "Frame6_2" );
    Frame6_2->setGeometry( QRect( 10, 50, 200, 41 ) ); 
    Frame6_2->setFrameShape( QFrame::StyledPanel );
    Frame6_2->setFrameShadow( QFrame::Sunken );

    TextLabel2_2 = new QLabel( Frame6_2, "TextLabel2_2" );
    TextLabel2_2->setGeometry( QRect( 12, 10, 52, 24 ) ); 
    TextLabel2_2->setText( trUtf8( "charge:" ) );

    charge_edit = new QLineEdit( Frame6_2, "charge_edit" );
    charge_edit->setGeometry( QRect( 70, 10, 125, 24 ) ); 

    Frame6_3 = new QFrame( this, "Frame6_3" );
    Frame6_3->setGeometry( QRect( 10, 90, 200, 41 ) ); 
    Frame6_3->setFrameShape( QFrame::StyledPanel );
    Frame6_3->setFrameShadow( QFrame::Sunken );

    TextLabel2_3 = new QLabel( Frame6_3, "TextLabel2_3" );
    TextLabel2_3->setGeometry( QRect( 10, 10, 52, 24 ) ); 
    TextLabel2_3->setText( trUtf8( "radius:" ) );

    radius_edit = new QLineEdit( Frame6_3, "radius_edit" );
    radius_edit->setGeometry( QRect( 70, 10, 125, 24 ) ); 

    Frame3_3 = new QFrame( this, "Frame3_3" );
    Frame3_3->setGeometry( QRect( 10, 130, 200, 110 ) ); 
    Frame3_3->setFrameShape( QFrame::StyledPanel );
    Frame3_3->setFrameShadow( QFrame::Sunken );

    TextLabel5_3 = new QLabel( Frame3_3, "TextLabel5_3" );
    TextLabel5_3->setGeometry( QRect( 10, 20, 60, 20 ) ); 
    TextLabel5_3->setText( trUtf8( "position:" ) );

    position_edit_1 = new QLineEdit( Frame3_3, "position_edit_1" );
    position_edit_1->setGeometry( QRect( 70, 20, 120, 24 ) ); 

    position_edit_2 = new QLineEdit( Frame3_3, "position_edit_2" );
    position_edit_2->setGeometry( QRect( 70, 50, 120, 24 ) ); 

    position_edit_3 = new QLineEdit( Frame3_3, "position_edit_3" );
    position_edit_3->setGeometry( QRect( 70, 80, 120, 24 ) ); 

    Frame3_2 = new QFrame( this, "Frame3_2" );
    Frame3_2->setGeometry( QRect( 10, 240, 200, 110 ) ); 
    Frame3_2->setFrameShape( QFrame::StyledPanel );
    Frame3_2->setFrameShadow( QFrame::Sunken );

    TextLabel5_2 = new QLabel( Frame3_2, "TextLabel5_2" );
    TextLabel5_2->setGeometry( QRect( 10, 20, 60, 20 ) ); 
    TextLabel5_2->setText( trUtf8( "force:" ) );

    force_edit_1 = new QLineEdit( Frame3_2, "force_edit_1" );
    force_edit_1->setGeometry( QRect( 70, 20, 120, 24 ) ); 

    force_edit_2 = new QLineEdit( Frame3_2, "force_edit_2" );
    force_edit_2->setGeometry( QRect( 70, 50, 120, 24 ) ); 

    force_edit_3 = new QLineEdit( Frame3_2, "force_edit_3" );
    force_edit_3->setGeometry( QRect( 70, 80, 120, 24 ) ); 

    Frame3 = new QFrame( this, "Frame3" );
    Frame3->setGeometry( QRect( 10, 350, 200, 110 ) ); 
    Frame3->setFrameShape( QFrame::StyledPanel );
    Frame3->setFrameShadow( QFrame::Sunken );

    TextLabel5 = new QLabel( Frame3, "TextLabel5" );
    TextLabel5->setGeometry( QRect( 10, 20, 60, 20 ) ); 
    TextLabel5->setText( trUtf8( "velocity:" ) );

    velocity_edit_1 = new QLineEdit( Frame3, "velocity_edit_1" );
    velocity_edit_1->setGeometry( QRect( 70, 20, 120, 24 ) ); 

    velocity_edit_2 = new QLineEdit( Frame3, "velocity_edit_2" );
    velocity_edit_2->setGeometry( QRect( 70, 50, 120, 24 ) ); 

    velocity_edit_3 = new QLineEdit( Frame3, "velocity_edit_3" );
    velocity_edit_3->setGeometry( QRect( 70, 80, 120, 24 ) ); 

    // signals and slots connections
    connect( name_edit, SIGNAL( textChanged(const QString&) ), this, SLOT( changed(const QString&) ) );
    connect( charge_edit, SIGNAL( textChanged(const QString&) ), this, SLOT( changed(const QString&) ) );
    connect( radius_edit, SIGNAL( textChanged(const QString&) ), this, SLOT( changed(const QString&) ) );
    connect( velocity_edit_1, SIGNAL( textChanged(const QString&) ), this, SLOT( changed(const QString&) ) );
    connect( velocity_edit_2, SIGNAL( textChanged(const QString&) ), this, SLOT( changed(const QString&) ) );
    connect( velocity_edit_3, SIGNAL( textChanged(const QString&) ), this, SLOT( changed(const QString&) ) );
    connect( force_edit_1, SIGNAL( textChanged(const QString&) ), this, SLOT( changed(const QString&) ) );
    connect( force_edit_2, SIGNAL( textChanged(const QString&) ), this, SLOT( changed(const QString&) ) );
    connect( force_edit_3, SIGNAL( textChanged(const QString&) ), this, SLOT( changed(const QString&) ) );
    connect( position_edit_1, SIGNAL( textChanged(const QString&) ), this, SLOT( changed(const QString&) ) );
    connect( position_edit_2, SIGNAL( textChanged(const QString&) ), this, SLOT( changed(const QString&) ) );
    connect( position_edit_3, SIGNAL( textChanged(const QString&) ), this, SLOT( changed(const QString&) ) );
    connect( apply_button, SIGNAL( clicked() ), this, SLOT( applyClicked() ) );
    connect( close_button, SIGNAL( clicked() ), this, SLOT( closeClicked() ) );
}

/*  
 *  Destroys the object and frees any allocated resources
 */
AtomSettingsData::~AtomSettingsData()
{
    // no need to delete child widgets, Qt does it all for us
}

void AtomSettingsData::changed( const QString & )
{
    qWarning( "AtomSettingsData::changed( const QString & ): Not implemented yet!" );
}

void AtomSettingsData::applyClicked()
{
    qWarning( "AtomSettingsData::applyClicked(): Not implemented yet!" );
}

void AtomSettingsData::closeClicked()
{
    qWarning( "AtomSettingsData::closeClicked(): Not implemented yet!" );
}

