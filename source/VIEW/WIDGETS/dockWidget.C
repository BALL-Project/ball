// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: dockWidget.C,v 1.2 2003/09/08 16:27:08 amoll Exp $

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
	if (name != 0) 
	{ 
		setCaption(name);
	}
	else 
	{
		setName( "DockWidget" );
	}

  caption_label_ = new QLabel( this, "caption_label" );
	caption_label_->resize(120, 12);
  caption_label_->setSizePolicy( QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed, 0, 0, false));
  caption_label_->setPaletteBackgroundColor( QColor( 255, 255, 127 ) );
  QFont caption_label_font(  caption_label_->font() );
  //caption_label_font.setFamily( "Helvetica" );
  caption_label_font.setPointSize( 11 );
  caption_label_->setFont( caption_label_font ); 
  caption_label_->setFrameShape( QLabel::NoFrame );
  caption_label_->setAlignment(QLabel::AlignCenter);
	setOrientation(Qt::Horizontal);
	boxLayout()->addWidget(caption_label_);
	
  resize( QSize(132, 293).expandedTo(minimumSizeHint()) );

	registerWidget(this);
}

void DockWidget::setGuest(QWidget& guest)
{
	QPoint p;
	guest.reparent(this, p, true);
	guest.resize(120,1000);
	setWidget(&guest);
	boxLayout()->addWidget(&guest);
  guest.setSizePolicy( QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed, 0, 0, false));
	setMinimumSize(20, 20);
	setCloseMode(QDockWindow::Always);
	setResizeEnabled(true);
}

void DockWidget::initializeWidget(MainControl& main_control)
	throw()
{
	window_menu_entry_id_ = 
		main_control.insertMenuEntry(MainControl::WINDOWS, getIdentifier(), this, SLOT(switchShowWidget()));
	getMainControl()->menuBar()->setItemChecked(window_menu_entry_id_, true);
}


void DockWidget::finalizeWidget(MainControl& main_control)
	throw()
{
	main_control.removeMenuEntry(MainControl::WINDOWS, getIdentifier(), this, SLOT(switchShowWidget()));
}


void DockWidget::switchShowWidget()
	throw()
{
	QMenuBar* menu = getMainControl()->menuBar();
	if (menu->isItemChecked(window_menu_entry_id_))
	{
		hide();
		menu->setItemChecked(window_menu_entry_id_, false);
	}
	else
	{
		show();
		menu->setItemChecked(window_menu_entry_id_, true);
	}
}

void DockWidget::writePreferences(INIFile& inifile)
	throw()
{
	inifile.insertValue("WINDOWS", getIdentifier() + "::on", 
		String(getMainControl()->menuBar()->isItemChecked(window_menu_entry_id_)));
}

void DockWidget::fetchPreferences(INIFile & inifile)
	throw()
{
	if (!inifile.hasEntry("WINDOWS", getIdentifier() + "::on")) return;
	if (inifile.getValue( "WINDOWS", getIdentifier() + "::on").toUnsignedInt() == 0) 
	{
		switchShowWidget();
	}
}


void DockWidget::close()
{
Log.error() << "#~~#   1" << std::endl;
	QDockWindow::close();
}

} } // namespaces
