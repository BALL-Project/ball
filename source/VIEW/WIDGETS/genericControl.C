// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: genericControl.C,v 1.3 2003/09/08 02:21:29 amoll Exp $

#include <BALL/VIEW/WIDGETS/genericControl.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <qmenubar.h>
#include <qlistview.h>
#include <qlabel.h>

using std::endl;

namespace BALL
{
	namespace VIEW
	{

void GenericControl::onContextMenu_(QListViewItem* /*item*/, const QPoint& /*point*/, int /*column*/)
{}

GenericControl::GenericControl(QWidget* parent, const char* name)
	throw()
		:	GenericControlData(parent, name),
			ModularWidget(name),
			context_item_(0)
{
	// appearance
	listview->setRootIsDecorated(true);
	listview->setSorting(-1);
	listview->setSelectionMode(QListView::Extended);

	caption_label->setText(name);

	connect(listview, SIGNAL(rightButtonPressed(QListViewItem*, const QPoint&, int)), this,
					SLOT(onContextMenu_(QListViewItem*, const QPoint&, int)));

	registerWidget(this);
}


GenericControl::~GenericControl()
	throw()
{
  #ifdef BALL_VIEW_DEBUG
	  Log.error() << "Destructing object " << (void *)this << " of class " 
								<< RTTI::getName<GenericControl>() << endl;
  #endif 
}


void GenericControl::initializeWidget(MainControl& main_control)
	throw()
{
	window_menu_entry_id_ = 
		main_control.insertMenuEntry(MainControl::WINDOWS, getIdentifier(), this, SLOT(switchShowWidget()));
	getMainControl()->menuBar()->setItemChecked(window_menu_entry_id_, true);
}


void GenericControl::finalizeWidget(MainControl& main_control)
	throw()
{
	main_control.removeMenuEntry(MainControl::WINDOWS, getIdentifier(), this, SLOT(switchShowWidget()));
}


void GenericControl::switchShowWidget()
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

void GenericControl::writePreferences(INIFile& inifile)
	throw()
{
	inifile.insertValue("WINDOWS", getIdentifier() + "::on", 
		String(getMainControl()->menuBar()->isItemChecked(window_menu_entry_id_)));
}

void GenericControl::fetchPreferences(INIFile & inifile)
	throw()
{
	if (!inifile.hasEntry("WINDOWS", getIdentifier() + "::on")) return;
	if (inifile.getValue( "WINDOWS", getIdentifier() + "::on").toUnsignedInt() == 0) 
	{
		switchShowWidget();
	}
}

} } // namespaces
