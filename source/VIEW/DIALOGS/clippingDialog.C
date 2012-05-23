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

ClippingDialog::ClippingDialog(QWidget* parent, const char* name)
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
}

ClippingDialog::~ClippingDialog()
{
#ifdef BALL_VIEW_DEBUG
	Log.error() << "deleting ClippingDialog " << this << std::endl;
#endif
}

int ClippingDialog::exec()
{
	if (clipping_plane_ == 0) return QDialog::Rejected;

	MainControl* mc = getMainControl();
	if (mc == 0) return QDialog::Rejected;

	RepresentationManager& pm = mc->getRepresentationManager();

 	listview->setColumnCount(3);
	listview->setRowCount(pm.getRepresentations().size());
	listview->setHorizontalHeaderItem(0, new QTableWidgetItem());
	listview->setHorizontalHeaderItem(1, new QTableWidgetItem());
	listview->setHorizontalHeaderItem(2, new QTableWidgetItem());
 	listview->horizontalHeaderItem(0)->setText("Model");
 	listview->horizontalHeaderItem(1)->setText("Coloring");
 	listview->horizontalHeaderItem(2)->setText("Properties");
	listview->setColumnWidth(0, 140);
	listview->setColumnWidth(1, 140);
	listview->setColumnWidth(2, 140);

	const ModelInformation& mi = mc->getModelInformation();

	Position row= 0;
	RepresentationList::const_iterator it = pm.getRepresentations().begin();
	for (; it != pm.getRepresentations().end(); it++)
	{
		const Representation& rep = **it;

		QTableWidgetItem* item = new QTableWidgetItem(rep.getName().c_str());
		item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
		item->setCheckState(Qt::Checked);

		if (!clipping_plane_->getRepresentations().has(*it)) item->setCheckState(Qt::Unchecked);
		listview->setItem(row, 0, item);

		item = new QTableWidgetItem(mi.getColoringName(rep.getColoringMethod()).c_str());
		listview->setItem(row, 1, item);

		item = new QTableWidgetItem(rep.getProperties().c_str());
		listview->setItem(row, 2, item);
		row++;
	}

	raise();
	return QDialog::exec();
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

	std::list<Representation*> lreps = getMainControl()->getRepresentationManager().getRepresentations();
	vector<const Representation*> reps;
	reps.resize(lreps.size());
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
