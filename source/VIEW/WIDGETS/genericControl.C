// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: genericControl.C,v 1.8 2004/01/13 00:44:47 amoll Exp $

#include <BALL/VIEW/WIDGETS/genericControl.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <qlistview.h>

using std::endl;

namespace BALL
{
	namespace VIEW
	{

void GenericControl::onContextMenu_(QListViewItem* /*item*/, const QPoint& /*point*/, int /*column*/)
{}

GenericControl::GenericControl(QWidget* parent, const char* name)
	throw()
		:	DockWidget(parent, name),
			context_item_(0),
			listview(new QListView(this))
{
	// appearance
	listview->setRootIsDecorated(true);
	listview->setSorting(-1);
	listview->setSelectionMode(QListView::Extended);
	setGuest(*listview);	

	connect(listview, SIGNAL(rightButtonPressed(QListViewItem*, const QPoint&, int)), this,
					SLOT(onContextMenu_(QListViewItem*, const QPoint&, int)));
}


GenericControl::~GenericControl()
	throw()
{
  #ifdef BALL_VIEW_DEBUG
	  Log.error() << "Destructing object " << (void *)this << " of class " 
								<< RTTI::getName<GenericControl>() << endl;
  #endif 
}


GenericControl::ItemList GenericControl::getSelectedItems()
	throw()
{
	ItemList selected;
	QListViewItemIterator it(listview);
	for (; it.current(); ++it)
	{
		if (it.current()->isSelected())
		{
 			selected.push_back(it.current());
		}
	}

	return selected;
}

void GenericControl::deselectOtherControls_()
{
	DeselectControlsMessage* dcm = new DeselectControlsMessage;
	notify_(dcm);
}

void GenericControl::updateSelection() 
{
	QListViewItemIterator it(listview);
	for (; it.current(); ++it)
	{
		QListViewItem* item = it.current();
		if (item->isSelected())
		{
			deselectOtherControls_();
			return;
		}
	}
}

void GenericControl::onNotify(Message *message)
	throw()
{
	if (!RTTI::isKindOf<DeselectControlsMessage>(*message))
	{
		return;
	}

	listview->selectAll(false);
}

} } // namespaces
