// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: dockWidget.C,v 1.7 2003/09/11 16:41:04 amoll Exp $

#include <BALL/VIEW/WIDGETS/dockWidget.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <qmenubar.h>
#include <qlabel.h>

using std::endl;

namespace BALL
{
	namespace VIEW
	{
		
DockWidget::DockWidget(QWidget* parent, const char* name)
: QDockWindow(QDockWindow::InDock, parent),
	ModularWidget(name)
{
	layout_ = new QVBoxLayout(this);
  caption_label_ = new QLabel(this, "caption_label");
	caption_label_->resize(120, 12);
  caption_label_->setSizePolicy( QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed, 0, 0, false));
  caption_label_->setPaletteBackgroundColor( QColor( 255, 255, 127 ) );
  QFont caption_label_font(caption_label_->font());
  caption_label_font.setFamily( "Helvetica" );
  caption_label_font.setPointSize( 11 );
  caption_label_->setFont( caption_label_font ); 
  caption_label_->setFrameShape(QLabel::NoFrame);
  caption_label_->setAlignment(QLabel::AlignCenter);
	layout_->addWidget(caption_label_);
	boxLayout()->addItem(layout_);

	setOrientation(Qt::Vertical);
  resize( QSize(132, 293));

	if (name != 0) 
	{ 
		setCaption(name);
		setName(name);
		caption_label_->setText(name);
	}
	else 
	{
		setName("DockWidget");
	}

	registerWidget(this);
}

void DockWidget::setGuest(QWidget& guest)
{
	QPoint p;
	guest.reparent(this, p, true);
	guest.resize(120,1000);
  guest.setSizePolicy( QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding, 0, 0, false));
	layout_->addWidget(&guest);
	setMinimumSize(20, 20);
	setCloseMode(QDockWindow::Always);
	setResizeEnabled(true);
}

void DockWidget::initializeWidget(MainControl& main_control)
	throw()
{
	window_menu_entry_id_ = 
		main_control.insertMenuEntry(MainControl::WINDOWS, getIdentifier(), this, SLOT(callSwitchShowWidget()));
	getMainControl()->menuBar()->setItemChecked(window_menu_entry_id_, true);
}


void DockWidget::finalizeWidget(MainControl& main_control)
	throw()
{
	main_control.removeMenuEntry(MainControl::WINDOWS, getIdentifier(), this, SLOT(callSwitchShowWidget()));
}


void DockWidget::callSwitchShowWidget()
	throw()
{
	switchShowWidget();
}

void DockWidget::writePreferences(INIFile& inifile)
	throw()
{
	ModularWidget::writePreferences(inifile);

	inifile.insertValue("WINDOWS", getIdentifier() + "::docked", String(place()));
}

void DockWidget::fetchPreferences(INIFile & inifile)
	throw()
{
	if (inifile.hasEntry("WINDOWS", getIdentifier() + "::docked") &&
			inifile.getValue("WINDOWS", getIdentifier() + "::docked").toUnsignedInt() != 0)
	{
		undock();
		show();
	}

	ModularWidget::fetchPreferences(inifile);
}

} } // namespaces
