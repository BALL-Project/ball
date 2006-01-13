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

	/*
ClippingDialog::SelectableListViewItem::SelectableListViewItem(const QString& text, 
																															 Representation* representation)
	throw()
	: QTableWidgetItem(text),
		representation_(representation)
{
	setFlags(Qt::ItemIsSelectable);
	setText(0, text);
	setText(1, representation->getColoringName().c_str());
	setText(2, representation->getProperties().c_str());
}
*/


ClippingDialog::ClippingDialog(QWidget* parent, const char* name)
	throw()
	:	QDialog(parent),
		Ui_ClippingDialogData()
{
#ifdef BALL_VIEW_DEBUG
	Log.error() << "new ClippingDialog " << this << std::endl;
#endif

	setupUi(this);
	
  // signals and slots connections
  connect( buttonOk, SIGNAL( clicked() ), this, SLOT( accept() ) );
  connect( buttonCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );

	setObjectName(name);

	/*
	listview->removeColumn(0);
	listview->insertColumn("Model");
	listview->insertColumn("Color");
	listview->insertColumn("Properties");
	listview->setColumnWidth(0, 60);
	listview->setColumnWidth(1, 60);
	listview->setColumnWidth(2, 60); 
	*/
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

	raise();
	MainControl* mc = getMainControl();
	if (mc == 0) return;

	PrimitiveManager& pm = mc->getPrimitiveManager();

	Position row= 0;
	PrimitiveManager::RepresentationList::ConstIterator it = pm.getRepresentations().begin();
	for (; it != pm.getRepresentations().end(); it++)
	{
		const Representation& rep = **it;

		QTableWidgetItem* item = new QTableWidgetItem(rep.getName().c_str());
		item->setFlags(Qt::ItemIsSelectable);
		if (clipping_plane_->getRepresentations().has(*it)) item->setCheckState(Qt::Checked);
		listview->setItem(row, 0, item);

		item = new QTableWidgetItem(rep.getColoringName().c_str());
		listview->setItem(row, 1, item);

		item = new QTableWidgetItem(rep.getProperties().c_str());
		listview->setItem(row, 2, item);
		row++;
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

	List<Representation*> lreps = getMainControl()->getPrimitiveManager().getRepresentations();
	vector<const Representation*> reps;
	copy(lreps.begin(), lreps.end(), reps.begin());

	for (Position pos = 0; pos < (Position)listview->rowCount(); pos++)
	{
		const QTableWidgetItem& item = *listview->item(pos, 0);
		if (item.checkState() == Qt::Checked)
		{
			clipping_plane_->getRepresentations().insert(reps[pos]);
		}
	}

	getMainControl()->redrawAllRepresentations();
}


} } // namespaces
