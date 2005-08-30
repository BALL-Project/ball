// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id:

#include <BALL/VIEW/DIALOGS/clippingDialog.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/VIEW/KERNEL/common.h>

namespace BALL
{
	namespace VIEW
	{

ClippingDialog::SelectableListViewItem::SelectableListViewItem(
		QListView* parent, const QString& text,
		Representation* representation)
	throw()
	: QCheckListItem(parent, text, QCheckListItem::CheckBox),
		representation_(representation)
{
	setText(0, text);
	setText(1, representation->getColoringName().c_str());
	setText(2, representation->getProperties().c_str());
}


ClippingDialog::ClippingDialog(QWidget* parent, const char* name)
	throw()
	:	ClippingDialogData( parent, name )
{
#ifdef BALL_VIEW_DEBUG
	Log.error() << "new ClippingDialog " << this << std::endl;
#endif

	listview->removeColumn(0);
	listview->addColumn("Model");
	listview->addColumn("Color");
	listview->addColumn("Properties");
	listview->setColumnWidth(0, 60);
	listview->setColumnWidth(1, 60);
	listview->setColumnWidth(2, 60);
}

ClippingDialog::~ClippingDialog()
	throw()
{
#ifdef BALL_VIEW_DEBUG
	Log.error() << "deleting ClippingDialog " << this << std::endl;
#endif
}

void ClippingDialog::show()
{
	if (clipping_plane_ == 0) return;

	ClippingDialogData::show();
	raise();
	MainControl* mc = getMainControl();
	if (mc == 0) return;

	PrimitiveManager& pm = mc->getPrimitiveManager();

	PrimitiveManager::RepresentationList::ConstIterator it = pm.getRepresentations().begin();
	for (; it != pm.getRepresentations().end(); it++)
	{
		SelectableListViewItem* item = new SelectableListViewItem(listview, (**it).getName().c_str(), *it);

		if (clipping_plane_->getRepresentations().has(*it)) item->setOn(true);
	}
}


void ClippingDialog::accept()
{
	hide();

	if (clipping_plane_ == 0 || getMainControl() == 0) 
	{
		BALLVIEW_DEBUG
		return;
	}

	clipping_plane_->getRepresentations().clear();

	QListViewItemIterator it(listview);
	for (; it.current(); ++it)
	{
		if (((SelectableListViewItem*)it.current())->isOn())
		{
			clipping_plane_->getRepresentations().insert((*(SelectableListViewItem*)*it).getRepresentation());
		}
	}

	getMainControl()->redrawAllRepresentations();
}


} } // namespaces
