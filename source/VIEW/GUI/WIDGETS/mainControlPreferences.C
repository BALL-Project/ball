// $Id: 

#include <BALL/VIEW/GUI/WIDGETS/mainControlPreferences.h>

#include <qplatinumstyle.h>
#include <qwindowsstyle.h>
#include <qmotifstyle.h>
#include <qcdestyle.h>
#include <qapplication.h>
#include <qpixmap.h>
#include <qlayout.h>
#include <qvbuttongroup.h>

namespace BALL
{
	namespace VIEW
	{

		MainControlPreferences::MainControlPreferences(QWidget* parent, const char* name)
			: QWidget(parent, name, 0)
		{
			QVButtonGroup *qtarch_VButtonGroup_1 = new QVButtonGroup(this, "VButtonGroup_1");
			qtarch_VButtonGroup_1->setGeometry(52, 39, 102, 132);
			qtarch_VButtonGroup_1->setMinimumSize(102, 132);
			qtarch_VButtonGroup_1->setMaximumSize(102, 132);
			qtarch_VButtonGroup_1->setFocusPolicy(QWidget::NoFocus);
			qtarch_VButtonGroup_1->setBackgroundMode(QWidget::PaletteBackground);
			qtarch_VButtonGroup_1->setFontPropagation(QWidget::SameFont);
			qtarch_VButtonGroup_1->setPalettePropagation(QWidget::SameFont);
			qtarch_VButtonGroup_1->setFrameStyle( 33 );
			qtarch_VButtonGroup_1->setLineWidth( 1 );
			qtarch_VButtonGroup_1->setMidLineWidth( 0 );
			qtarch_VButtonGroup_1->QFrame::setMargin( 0 );
			qtarch_VButtonGroup_1->setTitle( tr( "GUI Style" ) );
			qtarch_VButtonGroup_1->setAlignment( AlignHCenter );
			qtarch_VButtonGroup_1->setExclusive( TRUE );
			
			is_platinum_style = new QRadioButton(qtarch_VButtonGroup_1, "RadioButton_1");
			is_platinum_style->setGeometry(8, 25, 91, 22);
			is_platinum_style->setMinimumSize(0, 0);
			is_platinum_style->setMaximumSize(32767, 32767);
			is_platinum_style->setFocusPolicy(QWidget::TabFocus);
			is_platinum_style->setBackgroundMode(QWidget::PaletteBackground);
			is_platinum_style->setFontPropagation(QWidget::SameFont);
			is_platinum_style->setPalettePropagation(QWidget::SameFont);
			is_platinum_style->setText( tr( "Platinum" ) );
			is_platinum_style->setAutoRepeat( FALSE );
			is_platinum_style->setAutoResize( FALSE );
			is_platinum_style->setChecked( TRUE );
			
			is_windows_style = new QRadioButton(qtarch_VButtonGroup_1, "RadioButton_2");
			is_windows_style->setGeometry(8, 52, 91, 22);
			is_windows_style->setMinimumSize(0, 0);
			is_windows_style->setMaximumSize(32767, 32767);
			is_windows_style->setFocusPolicy(QWidget::TabFocus);
			is_windows_style->setBackgroundMode(QWidget::PaletteBackground);
			is_windows_style->setFontPropagation(QWidget::SameFont);
			is_windows_style->setPalettePropagation(QWidget::SameFont);
			is_windows_style->setText( tr( "Windows " ) );
			is_windows_style->setAutoRepeat( FALSE );
			is_windows_style->setAutoResize( TRUE );
			is_windows_style->setChecked( FALSE );
			
			is_motif_style = new QRadioButton(qtarch_VButtonGroup_1, "RadioButton_3");
			is_motif_style->setGeometry(8, 79, 91, 22);
			is_motif_style->setMinimumSize(0, 0);
			is_motif_style->setMaximumSize(32767, 32767);
			is_motif_style->setFocusPolicy(QWidget::TabFocus);
			is_motif_style->setBackgroundMode(QWidget::PaletteBackground);
			is_motif_style->setFontPropagation(QWidget::SameFont);
			is_motif_style->setPalettePropagation(QWidget::SameFont);
			is_motif_style->setText( tr( "Motif" ) );
			is_motif_style->setAutoRepeat( FALSE );
			is_motif_style->setAutoResize( TRUE );
			is_motif_style->setChecked( FALSE );
			
			is_cde_style = new QRadioButton(qtarch_VButtonGroup_1, "RadioButton_4");
			is_cde_style->setGeometry(8, 106, 91, 22);
			is_cde_style->setMinimumSize(0, 0);
			is_cde_style->setMaximumSize(32767, 32767);
			is_cde_style->setFocusPolicy(QWidget::TabFocus);
			is_cde_style->setBackgroundMode(QWidget::PaletteBackground);
			is_cde_style->setFontPropagation(QWidget::SameFont);
			is_cde_style->setPalettePropagation(QWidget::SameFont);
			is_cde_style->setText( tr( "CDE" ) );
			is_cde_style->setAutoRepeat( FALSE );
			is_cde_style->setAutoResize( FALSE );
			is_cde_style->setChecked( FALSE );
			
			QBoxLayout* qtarch_layout_1 = new QBoxLayout( this, QBoxLayout::TopToBottom, 5, 5, NULL );
			qtarch_layout_1->addStrut( 0 );
			QBoxLayout* qtarch_layout_1_1 = new QBoxLayout( QBoxLayout::LeftToRight, 5, NULL );
			qtarch_layout_1->addLayout( qtarch_layout_1_1, 1 );
			qtarch_layout_1_1->addStrut( 0 );
			qtarch_layout_1_1->addSpacing( 7 );
			qtarch_layout_1_1->addWidget( qtarch_VButtonGroup_1, 1, 36 );
			qtarch_layout_1_1->addStretch( 1 );
			resize(380,210);
			setMinimumSize(0, 0);
			setMaximumSize(32767, 32767);
		}
		
		MainControlPreferences::~MainControlPreferences()
		{
		}
		
		QStyle* MainControlPreferences::getStyle()
		{
			QStyle* new_style = 0;
			if (is_platinum_style->isChecked())
			{
				new_style = new QPlatinumStyle();
			}
			if (is_windows_style->isChecked())
			{
				new_style = new QWindowsStyle();
			}
			if (is_motif_style->isChecked())
			{
				new_style = new QMotifStyle();
			}
			if (is_cde_style->isChecked())
			{
				new_style = new QCDEStyle();
			}
			
			return new_style;
		}
		
		void MainControlPreferences::fetchPreferences(INIFile& inifile)
		{
			String style = "platinum";
			if (inifile.hasEntry("WINDOWS", "style"))
			{
				style = inifile.getValue("WINDOWS", "style");
			}
			
			if (style == "motif")
			{
				is_motif_style->setChecked(TRUE);
			} 
			else if (style == "windows")
			{
				is_windows_style->setChecked(TRUE);
			}
			else if (style == "cde")
			{
				is_cde_style->setChecked(TRUE);
			} 
			else 
			{
				is_platinum_style->setChecked(TRUE);
			}
		}
		
		void MainControlPreferences::writePreferences(INIFile& inifile)
		{
			String style = "platinum";
			if (is_platinum_style->isChecked())
			{
				style = "platinum";
			}
			else if (is_windows_style->isChecked())
			{
				style = "windows";
			}
			else if (is_motif_style->isChecked())
			{
				style = "motif";
			}
			else if (is_cde_style->isChecked())
			{
				style = "cde";
			}	
			
			// save the style settings
			inifile.setValue("WINDOWS", "style", style);
		}
		
	} // namespace VIEW

} // namespace BALL


