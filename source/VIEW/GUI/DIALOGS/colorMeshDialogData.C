/****************************************************************************
** Form implementation generated from reading ui file 'colorMeshDialog.ui'
**
** Created: Fri Nov 29 19:06:01 2002
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include <BALL/VIEW/GUI/DIALOGS/colorMeshDialogData.h>

#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qspinbox.h>
#include <qtabwidget.h>
#include <qwidget.h>
#include <qlayout.h>
#include <qvariant.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/* 
 *  Constructs a ColorMeshDialogData which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
ColorMeshDialogData::ColorMeshDialogData( QWidget* parent,  const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "ColorMeshDialogData" );
    resize( 413, 263 ); 
    setCaption( tr( "Form1" ) );

    apply_button = new QPushButton( this, "apply_button" );
    apply_button->setEnabled( FALSE );
    apply_button->setGeometry( QRect( 70, 220, 114, 32 ) ); 
    apply_button->setText( tr( "Apply" ) );
    apply_button->setDefault( TRUE );

    cancel_button = new QPushButton( this, "cancel_button" );
    cancel_button->setGeometry( QRect( 220, 220, 114, 32 ) ); 
    cancel_button->setText( tr( "Cancel" ) );
    cancel_button->setOn( FALSE );
    cancel_button->setDefault( FALSE );
    cancel_button->setOn( FALSE );

    surface_tab = new QTabWidget( this, "surface_tab" );
    surface_tab->setGeometry( QRect( 0, 0, 420, 200 ) ); 

    by_file = new QWidget( surface_tab, "by_file" );

    TextLabel1 = new QLabel( by_file, "TextLabel1" );
    TextLabel1->setGeometry( QRect( 10, 10, 49, 26 ) ); 
    TextLabel1->setText( tr( "Load file:" ) );

    browse_button = new QPushButton( by_file, "browse_button" );
    browse_button->setGeometry( QRect( 10, 80, 109, 32 ) ); 
    browse_button->setText( tr( "Browse" ) );

    location_edit = new QLineEdit( by_file, "location_edit" );
    location_edit->setGeometry( QRect( 10, 40, 390, 26 ) ); 
    surface_tab->insertTab( by_file, tr( "by file" ) );

    by_color = new QWidget( surface_tab, "by_color" );

    TextLabel5 = new QLabel( by_color, "TextLabel5" );
    TextLabel5->setGeometry( QRect( 10, 100, 59, 20 ) ); 
    TextLabel5->setText( tr( "alpha:" ) );

    TextLabel2 = new QLabel( by_color, "TextLabel2" );
    TextLabel2->setGeometry( QRect( 10, 10, 30, 20 ) ); 
    TextLabel2->setText( tr( "red:" ) );

    TextLabel3 = new QLabel( by_color, "TextLabel3" );
    TextLabel3->setGeometry( QRect( 8, 42, 59, 20 ) ); 
    TextLabel3->setText( tr( "green:" ) );

    TextLabel4 = new QLabel( by_color, "TextLabel4" );
    TextLabel4->setGeometry( QRect( 10, 70, 57, 20 ) ); 
    TextLabel4->setText( tr( "blue:" ) );

    TextLabel10 = new QLabel( by_color, "TextLabel10" );
    TextLabel10->setGeometry( QRect( 160, 20, 66, 20 ) ); 
    TextLabel10->setText( tr( "Choose:" ) );

    alpha_box = new QSpinBox( by_color, "alpha_box" );
    alpha_box->setGeometry( QRect( 60, 100, 50, 20 ) ); 
    alpha_box->setMaxValue( 255 );
    alpha_box->setValue( 255 );

    blue_box = new QSpinBox( by_color, "blue_box" );
    blue_box->setGeometry( QRect( 60, 70, 50, 20 ) ); 
    blue_box->setMaxValue( 255 );
    blue_box->setValue( 0 );

    green_box = new QSpinBox( by_color, "green_box" );
    green_box->setGeometry( QRect( 60, 40, 50, 20 ) ); 
    green_box->setMaxValue( 255 );
    green_box->setValue( 255 );

    red_box = new QSpinBox( by_color, "red_box" );
    red_box->setGeometry( QRect( 60, 10, 50, 20 ) ); 
    red_box->setMaxValue( 255 );
    red_box->setValue( 255 );

    choose_button = new QPushButton( by_color, "choose_button" );
    choose_button->setGeometry( QRect( 160, 40, 220, 80 ) ); 
    QPalette pal;
    QColorGroup cg;
    cg.setColor( QColorGroup::Foreground, black );
    cg.setColor( QColorGroup::Button, QColor( 255, 255, 0) );
    cg.setColor( QColorGroup::Light, QColor( 255, 255, 127) );
    cg.setColor( QColorGroup::Midlight, QColor( 255, 255, 63) );
    cg.setColor( QColorGroup::Dark, QColor( 127, 127, 0) );
    cg.setColor( QColorGroup::Mid, QColor( 170, 170, 0) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, black );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, QColor( 220, 220, 220) );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 10, 95, 137) );
    cg.setColor( QColorGroup::HighlightedText, white );
    pal.setActive( cg );
    cg.setColor( QColorGroup::Foreground, black );
    cg.setColor( QColorGroup::Button, QColor( 255, 255, 0) );
    cg.setColor( QColorGroup::Light, QColor( 255, 255, 127) );
    cg.setColor( QColorGroup::Midlight, QColor( 255, 255, 38) );
    cg.setColor( QColorGroup::Dark, QColor( 127, 127, 0) );
    cg.setColor( QColorGroup::Mid, QColor( 170, 170, 0) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, black );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, QColor( 220, 220, 220) );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 10, 95, 137) );
    cg.setColor( QColorGroup::HighlightedText, white );
    pal.setInactive( cg );
    cg.setColor( QColorGroup::Foreground, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Button, QColor( 255, 255, 0) );
    cg.setColor( QColorGroup::Light, QColor( 255, 255, 127) );
    cg.setColor( QColorGroup::Midlight, QColor( 255, 255, 38) );
    cg.setColor( QColorGroup::Dark, QColor( 127, 127, 0) );
    cg.setColor( QColorGroup::Mid, QColor( 170, 170, 0) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, QColor( 220, 220, 220) );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 10, 95, 137) );
    cg.setColor( QColorGroup::HighlightedText, white );
    pal.setDisabled( cg );
    choose_button->setPalette( pal );
    choose_button->setText( tr( "" ) );
    surface_tab->insertTab( by_color, tr( "by color" ) );

    colormap_tab = new QWidget( surface_tab, "colormap_tab" );

    TextLabel7 = new QLabel( colormap_tab, "TextLabel7" );
    TextLabel7->setGeometry( QRect( 10, 20, 100, 20 ) ); 
    TextLabel7->setText( tr( "Number of levels:" ) );

    TextLabel8 = new QLabel( colormap_tab, "TextLabel8" );
    TextLabel8->setGeometry( QRect( 180, 20, 30, 20 ) ); 
    TextLabel8->setText( tr( "Min:" ) );

    TextLabel9 = new QLabel( colormap_tab, "TextLabel9" );
    TextLabel9->setGeometry( QRect( 280, 20, 30, 20 ) ); 
    TextLabel9->setText( tr( "Max:" ) );

    TextLabel14_3 = new QLabel( colormap_tab, "TextLabel14_3" );
    TextLabel14_3->setGeometry( QRect( 32, 58, 40, 20 ) ); 
    TextLabel14_3->setText( tr( "< min" ) );

    TextLabel14 = new QLabel( colormap_tab, "TextLabel14" );
    TextLabel14->setGeometry( QRect( 112, 58, 20, 20 ) ); 
    TextLabel14->setText( tr( "min" ) );

    TextLabel14_2 = new QLabel( colormap_tab, "TextLabel14_2" );
    TextLabel14_2->setGeometry( QRect( 192, 58, 30, 20 ) ); 
    TextLabel14_2->setText( tr( "mid" ) );

    TextLabel15 = new QLabel( colormap_tab, "TextLabel15" );
    TextLabel15->setGeometry( QRect( 272, 58, 30, 20 ) ); 
    TextLabel15->setText( tr( "max" ) );

    TextLabel13 = new QLabel( colormap_tab, "TextLabel13" );
    TextLabel13->setGeometry( QRect( 342, 58, 40, 20 ) ); 
    TextLabel13->setText( tr( "> max" ) );

    min_button = new QPushButton( colormap_tab, "min_button" );
    min_button->setGeometry( QRect( 90, 80, 70, 32 ) ); 
    cg.setColor( QColorGroup::Foreground, black );
    cg.setColor( QColorGroup::Button, QColor( 0, 0, 255) );
    cg.setColor( QColorGroup::Light, QColor( 127, 127, 255) );
    cg.setColor( QColorGroup::Midlight, QColor( 63, 63, 255) );
    cg.setColor( QColorGroup::Dark, QColor( 0, 0, 127) );
    cg.setColor( QColorGroup::Mid, QColor( 0, 0, 170) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, black );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, QColor( 220, 220, 220) );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 10, 95, 137) );
    cg.setColor( QColorGroup::HighlightedText, white );
    pal.setActive( cg );
    cg.setColor( QColorGroup::Foreground, black );
    cg.setColor( QColorGroup::Button, QColor( 0, 0, 255) );
    cg.setColor( QColorGroup::Light, QColor( 127, 127, 255) );
    cg.setColor( QColorGroup::Midlight, QColor( 38, 38, 255) );
    cg.setColor( QColorGroup::Dark, QColor( 0, 0, 127) );
    cg.setColor( QColorGroup::Mid, QColor( 0, 0, 170) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, black );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, QColor( 220, 220, 220) );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 10, 95, 137) );
    cg.setColor( QColorGroup::HighlightedText, white );
    pal.setInactive( cg );
    cg.setColor( QColorGroup::Foreground, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Button, QColor( 0, 0, 255) );
    cg.setColor( QColorGroup::Light, QColor( 127, 127, 255) );
    cg.setColor( QColorGroup::Midlight, QColor( 38, 38, 255) );
    cg.setColor( QColorGroup::Dark, QColor( 0, 0, 127) );
    cg.setColor( QColorGroup::Mid, QColor( 0, 0, 170) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, QColor( 220, 220, 220) );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 10, 95, 137) );
    cg.setColor( QColorGroup::HighlightedText, white );
    pal.setDisabled( cg );
    min_button->setPalette( pal );
    min_button->setText( tr( "" ) );

    mid_button = new QPushButton( colormap_tab, "mid_button" );
    mid_button->setGeometry( QRect( 170, 80, 70, 32 ) ); 
    cg.setColor( QColorGroup::Foreground, black );
    cg.setColor( QColorGroup::Button, QColor( 255, 0, 0) );
    cg.setColor( QColorGroup::Light, QColor( 255, 127, 127) );
    cg.setColor( QColorGroup::Midlight, QColor( 255, 63, 63) );
    cg.setColor( QColorGroup::Dark, QColor( 127, 0, 0) );
    cg.setColor( QColorGroup::Mid, QColor( 170, 0, 0) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, black );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, QColor( 220, 220, 220) );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 10, 95, 137) );
    cg.setColor( QColorGroup::HighlightedText, white );
    pal.setActive( cg );
    cg.setColor( QColorGroup::Foreground, black );
    cg.setColor( QColorGroup::Button, QColor( 255, 0, 0) );
    cg.setColor( QColorGroup::Light, QColor( 255, 127, 127) );
    cg.setColor( QColorGroup::Midlight, QColor( 255, 38, 38) );
    cg.setColor( QColorGroup::Dark, QColor( 127, 0, 0) );
    cg.setColor( QColorGroup::Mid, QColor( 170, 0, 0) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, black );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, QColor( 220, 220, 220) );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 10, 95, 137) );
    cg.setColor( QColorGroup::HighlightedText, white );
    pal.setInactive( cg );
    cg.setColor( QColorGroup::Foreground, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Button, QColor( 255, 0, 0) );
    cg.setColor( QColorGroup::Light, QColor( 255, 127, 127) );
    cg.setColor( QColorGroup::Midlight, QColor( 255, 38, 38) );
    cg.setColor( QColorGroup::Dark, QColor( 127, 0, 0) );
    cg.setColor( QColorGroup::Mid, QColor( 170, 0, 0) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, QColor( 220, 220, 220) );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 10, 95, 137) );
    cg.setColor( QColorGroup::HighlightedText, white );
    pal.setDisabled( cg );
    mid_button->setPalette( pal );
    mid_button->setText( tr( "" ) );

    max_button = new QPushButton( colormap_tab, "max_button" );
    max_button->setGeometry( QRect( 250, 80, 70, 32 ) ); 
    cg.setColor( QColorGroup::Foreground, black );
    cg.setColor( QColorGroup::Button, QColor( 255, 255, 0) );
    cg.setColor( QColorGroup::Light, QColor( 255, 255, 127) );
    cg.setColor( QColorGroup::Midlight, QColor( 255, 255, 63) );
    cg.setColor( QColorGroup::Dark, QColor( 127, 127, 0) );
    cg.setColor( QColorGroup::Mid, QColor( 170, 170, 0) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, black );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, QColor( 220, 220, 220) );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 10, 95, 137) );
    cg.setColor( QColorGroup::HighlightedText, white );
    pal.setActive( cg );
    cg.setColor( QColorGroup::Foreground, black );
    cg.setColor( QColorGroup::Button, QColor( 255, 255, 0) );
    cg.setColor( QColorGroup::Light, QColor( 255, 255, 127) );
    cg.setColor( QColorGroup::Midlight, QColor( 255, 255, 38) );
    cg.setColor( QColorGroup::Dark, QColor( 127, 127, 0) );
    cg.setColor( QColorGroup::Mid, QColor( 170, 170, 0) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, black );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, QColor( 220, 220, 220) );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 10, 95, 137) );
    cg.setColor( QColorGroup::HighlightedText, white );
    pal.setInactive( cg );
    cg.setColor( QColorGroup::Foreground, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Button, QColor( 255, 255, 0) );
    cg.setColor( QColorGroup::Light, QColor( 255, 255, 127) );
    cg.setColor( QColorGroup::Midlight, QColor( 255, 255, 38) );
    cg.setColor( QColorGroup::Dark, QColor( 127, 127, 0) );
    cg.setColor( QColorGroup::Mid, QColor( 170, 170, 0) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, QColor( 220, 220, 220) );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 10, 95, 137) );
    cg.setColor( QColorGroup::HighlightedText, white );
    pal.setDisabled( cg );
    max_button->setPalette( pal );
    max_button->setText( tr( "" ) );

    max_max_button = new QPushButton( colormap_tab, "max_max_button" );
    max_max_button->setGeometry( QRect( 330, 80, 70, 32 ) ); 
    cg.setColor( QColorGroup::Foreground, black );
    cg.setColor( QColorGroup::Button, white );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, white );
    cg.setColor( QColorGroup::Dark, QColor( 127, 127, 127) );
    cg.setColor( QColorGroup::Mid, QColor( 170, 170, 170) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, black );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, QColor( 220, 220, 220) );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 10, 95, 137) );
    cg.setColor( QColorGroup::HighlightedText, white );
    pal.setActive( cg );
    cg.setColor( QColorGroup::Foreground, black );
    cg.setColor( QColorGroup::Button, white );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, white );
    cg.setColor( QColorGroup::Dark, QColor( 127, 127, 127) );
    cg.setColor( QColorGroup::Mid, QColor( 170, 170, 170) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, black );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, QColor( 220, 220, 220) );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 10, 95, 137) );
    cg.setColor( QColorGroup::HighlightedText, white );
    pal.setInactive( cg );
    cg.setColor( QColorGroup::Foreground, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Button, white );
    cg.setColor( QColorGroup::Light, white );
    cg.setColor( QColorGroup::Midlight, white );
    cg.setColor( QColorGroup::Dark, QColor( 127, 127, 127) );
    cg.setColor( QColorGroup::Mid, QColor( 170, 170, 170) );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, QColor( 220, 220, 220) );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 10, 95, 137) );
    cg.setColor( QColorGroup::HighlightedText, white );
    pal.setDisabled( cg );
    max_max_button->setPalette( pal );
    max_max_button->setText( tr( "" ) );

    max_box = new QLineEdit( colormap_tab, "max_box" );
    max_box->setGeometry( QRect( 310, 20, 50, 20 ) ); 
    max_box->setText( tr( "1.0" ) );

    levels_box = new QSpinBox( colormap_tab, "levels_box" );
    levels_box->setGeometry( QRect( 110, 20, 50, 20 ) ); 
    levels_box->setValue( 3 );

    min_box = new QLineEdit( colormap_tab, "min_box" );
    min_box->setGeometry( QRect( 210, 20, 50, 20 ) ); 
    min_box->setText( tr( "-1.0" ) );

    min_min_button = new QPushButton( colormap_tab, "min_min_button" );
    min_min_button->setGeometry( QRect( 10, 80, 70, 32 ) ); 
    cg.setColor( QColorGroup::Foreground, black );
    cg.setColor( QColorGroup::Button, black );
    cg.setColor( QColorGroup::Light, black );
    cg.setColor( QColorGroup::Midlight, black );
    cg.setColor( QColorGroup::Dark, black );
    cg.setColor( QColorGroup::Mid, black );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, black );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, QColor( 220, 220, 220) );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 10, 95, 137) );
    cg.setColor( QColorGroup::HighlightedText, white );
    pal.setActive( cg );
    cg.setColor( QColorGroup::Foreground, black );
    cg.setColor( QColorGroup::Button, black );
    cg.setColor( QColorGroup::Light, black );
    cg.setColor( QColorGroup::Midlight, black );
    cg.setColor( QColorGroup::Dark, black );
    cg.setColor( QColorGroup::Mid, black );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, black );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, QColor( 220, 220, 220) );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 10, 95, 137) );
    cg.setColor( QColorGroup::HighlightedText, white );
    pal.setInactive( cg );
    cg.setColor( QColorGroup::Foreground, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Button, black );
    cg.setColor( QColorGroup::Light, black );
    cg.setColor( QColorGroup::Midlight, black );
    cg.setColor( QColorGroup::Dark, black );
    cg.setColor( QColorGroup::Mid, black );
    cg.setColor( QColorGroup::Text, black );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, QColor( 220, 220, 220) );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 10, 95, 137) );
    cg.setColor( QColorGroup::HighlightedText, white );
    pal.setDisabled( cg );
    min_min_button->setPalette( pal );
    min_min_button->setText( tr( "" ) );

    gradient_label = new QLabel( colormap_tab, "gradient_label" );
    gradient_label->setGeometry( QRect( 20, 130, 381, 21 ) ); 
    gradient_label->setText( tr( "" ) );
    surface_tab->insertTab( colormap_tab, tr( "ColorMap" ) );

    // signals and slots connections
    connect( browse_button, SIGNAL( clicked() ), this, SLOT( browse_clicked() ) );
    connect( location_edit, SIGNAL( textChanged(const QString&) ), this, SLOT( location_changed() ) );
    connect( apply_button, SIGNAL( clicked() ), this, SLOT( apply_clicked() ) );
    connect( cancel_button, SIGNAL( clicked() ), this, SLOT( cancel_clicked() ) );
    connect( red_box, SIGNAL( valueChanged(int) ), this, SLOT( color_boxes_changed() ) );
    connect( green_box, SIGNAL( valueChanged(int) ), this, SLOT( color_boxes_changed() ) );
    connect( blue_box, SIGNAL( valueChanged(int) ), this, SLOT( color_boxes_changed() ) );
    connect( choose_button, SIGNAL( clicked() ), this, SLOT( choose_clicked() ) );
    connect( min_min_button, SIGNAL( clicked() ), this, SLOT( min_min_clicked() ) );
    connect( min_button, SIGNAL( clicked() ), this, SLOT( min_clicked() ) );
    connect( mid_button, SIGNAL( clicked() ), this, SLOT( mid_clicked() ) );
    connect( max_button, SIGNAL( clicked() ), this, SLOT( max_clicked() ) );
    connect( max_max_button, SIGNAL( clicked() ), this, SLOT( max_max_clicked() ) );
    connect( surface_tab, SIGNAL( selected(const QString&) ), this, SLOT( tab_changed() ) );
}

/*  
 *  Destroys the object and frees any allocated resources
 */
ColorMeshDialogData::~ColorMeshDialogData()
{
    // no need to delete child widgets, Qt does it all for us
}

void ColorMeshDialogData::apply_clicked()
{
    qWarning( "ColorMeshDialogData::apply_clicked(): Not implemented yet!" );
}

void ColorMeshDialogData::browse_clicked()
{
    qWarning( "ColorMeshDialogData::browse_clicked(): Not implemented yet!" );
}

void ColorMeshDialogData::cancel_clicked()
{
    qWarning( "ColorMeshDialogData::cancel_clicked(): Not implemented yet!" );
}

void ColorMeshDialogData::choose_clicked()
{
    qWarning( "ColorMeshDialogData::choose_clicked(): Not implemented yet!" );
}

void ColorMeshDialogData::color_boxes_changed()
{
    qWarning( "ColorMeshDialogData::color_boxes_changed(): Not implemented yet!" );
}

void ColorMeshDialogData::location_changed()
{
    qWarning( "ColorMeshDialogData::location_changed(): Not implemented yet!" );
}

void ColorMeshDialogData::max_clicked()
{
    qWarning( "ColorMeshDialogData::max_clicked(): Not implemented yet!" );
}

void ColorMeshDialogData::mid_clicked()
{
    qWarning( "ColorMeshDialogData::mid_clicked(): Not implemented yet!" );
}

void ColorMeshDialogData::min_clicked()
{
    qWarning( "ColorMeshDialogData::min_clicked(): Not implemented yet!" );
}

void ColorMeshDialogData::min_min_clicked()
{
    qWarning( "ColorMeshDialogData::min_min_clicked(): Not implemented yet!" );
}

void ColorMeshDialogData::max_max_clicked()
{
    qWarning( "ColorMeshDialogData::max_max_clicked(): Not implemented yet!" );
}

void ColorMeshDialogData::tab_changed()
{
    qWarning( "ColorMeshDialogData::tab_changed(): Not implemented yet!" );
}

