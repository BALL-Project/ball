// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: datasetControl.C,v 1.7 2003/10/04 12:23:52 amoll Exp $

#include <BALL/VIEW/WIDGETS/datasetControl.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/KERNEL/system.h>
#include <BALL/FORMAT/DCDFile.h>
#include <BALL/VIEW/DIALOGS/snapShotVisualisation.h>
#include <qpopupmenu.h>
#include <qmenubar.h>
#include <qfiledialog.h>
#include <qlistview.h>
#include <qtooltip.h>

using std::endl;

namespace BALL
{
	namespace VIEW
	{

DatasetControl::DatasetControl(QWidget* parent, const char* name)
	throw()
	:	GenericControl(parent, name),
		dialog_(0)
{
#ifdef BALL_VIEW_DEBUG
	Log.error() << "new DatasetControl " << this << std::endl;
#endif
	listview->addColumn("Name");
	listview->addColumn("from");
	listview->setColumnWidth(0, 120);
	listview->setColumnWidth(1, 60);
	default_visible_ = false;
	connect(listview, SIGNAL(selectionChanged()), this, SLOT(updateSelection()));
}


DatasetControl::~DatasetControl()
	throw()
{
  #ifdef BALL_VIEW_DEBUG
	  Log.error() << "Destructing object " << this << " of class DatasetControl" << std::endl;
  #endif 

	if (dialog_) delete dialog_;
}


void DatasetControl::initializeWidget(MainControl& main_control)
	throw()
{
	open_trajectory_id_ = 
		main_control.insertMenuEntry(MainControl::FILE, "Open Trajectory", this, SLOT(addTrajectory()), 0, -1,
		String("To open a trajectory file, one System has to be selected."));
	open_grid_id_ = 
		main_control.insertMenuEntry(MainControl::FILE, "Open 3D Grid", this, SLOT(add3DGrid()), 0, -1,
		String("To open a 3D grid, one System has to be selected."));
	GenericControl::initializeWidget(main_control);
}


void DatasetControl::finalizeWidget(MainControl& main_control)
	throw()
{
	main_control.removeMenuEntry(MainControl::FILE, "Open Trajectory", this, SLOT(addTrajectory()));
	main_control.removeMenuEntry(MainControl::FILE, "Open 3D Grid", this, SLOT(add3DGrid()));
	GenericControl::finalizeWidget(main_control);
}


void DatasetControl::checkMenu(MainControl& main_control)
	throw()
{
	getMainControl()->menuBar()->setItemEnabled(open_trajectory_id_, main_control.getSelectedSystem());
	getMainControl()->menuBar()->setItemEnabled(      open_grid_id_, main_control.getSelectedSystem());
}


void DatasetControl::addTrajectory()
	throw()
{
	if (!getMainControl()->getSelectedSystem()) return;
	QFileDialog *fd = new QFileDialog(this, "", true);
	fd->setMode(QFileDialog::ExistingFile);
	fd->setFilter("DCD files(*.dcd)");
	fd->setCaption("Select a DCD file");
	fd->setViewMode(QFileDialog::Detail);

	if (!fd->exec()== QDialog::Accepted) return;

	String filename(fd->selectedFile().ascii());
	delete fd;

	// construct a name for the system(the filename without the dir path)
	DCDFile* dcd = new DCDFile(filename, File::IN);
	insertTrajectory_(dcd, *getMainControl()->getSelectedSystem());
}

void DatasetControl::insertTrajectory_(TrajectoryFile* file, System& system)
	throw()
{
	if (file->getNumberOfAtoms() != system.countAtoms())
	{
		setStatusbarText("Number of atoms do not match. Aborting...");
		delete file;
		return;
	}

	SnapShotManager* manager = new SnapShotManager(&system, 0, file);

	String name = file->getName();

	Position pos = 0;
	for (Position p = 0; p < name.size(); p++)
	{
		if (name[p] == FileSystem::PATH_SEPARATOR) pos = p;
	}
	
	if (pos) pos++;
	name = name.getSubstring(pos);
	
	QListViewItem* item = new QListViewItem(listview, name.c_str(), system.getName().c_str());
	item_to_trajectory_[item] = manager;
	insertComposite_(&system, item);
}


void DatasetControl::insertComposite_(Composite* composite, QListViewItem* item)
	throw()
{
	listview->triggerUpdate();
	item_to_composite_[item] = composite;
	if (composite_to_items_.has(composite))
	{
		composite_to_items_[composite].insert(item);
	}
	else
	{
		HashSet<QListViewItem*> set;
		set.insert(item);
		composite_to_items_[composite] = set;
	}
}

void DatasetControl::onNotify(Message *message)
	throw()
{
#ifdef BALL_VIEW_DEBUG
	Log.error() << "DatasetControl "<<this<<  " onNotify " << message << std::endl;
#endif
	if (RTTI::isKindOf<RegularData3DMessage>(*message))
	{
		RegularData3DMessage* ntm = RTTI::castTo<RegularData3DMessage>(*message);
		insertGrid_(ntm->getRegularData3D(), *(System*)ntm->getComposite(), ntm->getCompositeName());
		return;
	}
	else if (RTTI::isKindOf<NewTrajectoryMessage>(*message))
	{
		NewTrajectoryMessage* ntm = RTTI::castTo<NewTrajectoryMessage>(*message);
		insertTrajectory_(ntm->getTrajectoryFile(), *(System*)ntm->getComposite());
		return;
	}
	else if (RTTI::isKindOf<CompositeMessage>(*message))
  {
    CompositeMessage *composite_message = RTTI::castTo<CompositeMessage>(*message);
		if (composite_message->getType() != CompositeMessage::REMOVED_COMPOSITE) return;
    Composite* composite = (Composite *)composite_message->getComposite();
		if (!composite_to_items_.has(composite)) return;

		// create a copy of the hashset, because it changes
		HashSet<QListViewItem*> to_delete = composite_to_items_[composite];

		HashSet<QListViewItem*>::Iterator lit = to_delete.begin();
		for (;lit != to_delete.end(); lit++)
		{
			context_item_ = *lit;
			deleteTrajectory_();
			deleteGrid_();
		}
	}   
}

void DatasetControl::deleteTrajectory_()
{
	if (!item_to_trajectory_.has(context_item_)) return;
	SnapShotManager* ssm = item_to_trajectory_[context_item_];
	delete ssm;
	deleteItem_(context_item_);
	setStatusbarText("deleted trajectory");
}

void DatasetControl::deleteItem_(QListViewItem* item)
	throw()
{
	Composite* composite = item_to_composite_[item];
	composite_to_items_[composite].erase(item);
	item_to_trajectory_.erase(item);
	item_to_grid_.erase(item);
	item_to_composite_.erase(item);
	delete item;
	listview->triggerUpdate();
}

void DatasetControl::deleteGrid_()
{
	if (!item_to_grid_.has(context_item_)) return;

	RegularData3D* ssm = item_to_grid_[context_item_];

	RegularData3DMessage* msg = new RegularData3DMessage(RegularData3DMessage::REMOVE);
	msg->setRegularData3D(ssm);
	notify_(msg);

	delete ssm;
	deleteItem_(context_item_);
	setStatusbarText("deleted 3D grid");
}

void DatasetControl::onContextMenu_(QListViewItem* item,  const QPoint& point, int /* column */)
{
	if (item == 0) return;
	context_item_ = item;

	QPopupMenu context_menu;
	if (item_to_trajectory_.has(item))
	{
		context_menu.insertItem("Save", this, SLOT(saveTrajectory_()));
		context_menu.insertItem("Visualise/Export", this, SLOT(visualiseTrajectory_()));
		context_menu.insertItem("Delete", this, SLOT(deleteTrajectory_()));
		//	insertContextMenuEntry("Export to PNGs", this, SLOT(visualiseTrajectory_()));
	}
	else if (item_to_grid_.has(item))
	{
		context_menu.insertItem("Save", this, SLOT(save3DGrid_()));
		context_menu.insertItem("Delete", this, SLOT(deleteGrid_()));
	}

	// show the context menu if it is not empty
	if (context_menu.count()) context_menu.exec(point);
}

void DatasetControl::visualiseTrajectory_()
{
	if (dialog_) 
	{
		dialog_->hide();
		delete dialog_;
	}

	SnapShotManager* ssm = item_to_trajectory_[context_item_];

	dialog_ = new SnapshotVisualisationDialog(this);
	dialog_->setSnapShotManager(ssm);
	dialog_->show();
}

void DatasetControl::saveTrajectory_()
{
	SnapShotManager* ssm = item_to_trajectory_[context_item_];

	QFileDialog *fd = new QFileDialog(this, "", true);
	fd->setMode(QFileDialog::AnyFile);
	fd->setFilter("DCD files(*.dcd)");
	fd->setCaption("Write to a DCD file");
	fd->setViewMode(QFileDialog::Detail);

	if (!fd->exec()== QDialog::Accepted) return;

	String filename(fd->selectedFile().ascii());
	ssm->getTrajectoryFile()->copyTo(filename);
	setStatusbarText("Writen DCDFile");
}

void DatasetControl::add3DGrid()
	throw()
{
	QString result = QFileDialog::getOpenFileName("", "*", 0, "Select a RegularData file");
	if (result.isEmpty()) return;

	RegularData3D* dat = new RegularData3D;
	File infile;
	
	try
	{
		infile.open(result.ascii(), std::ios::in);
	}
	catch(Exception::FileNotFound)
	{
		Log.error() << "File could not be found!" << std::endl;
		return;
	}

	infile >> *dat;
	infile.close();
	insertGrid_(dat, *getMainControl()->getSelectedSystem(), String(result.ascii()));
	RegularData3DMessage* msg = new RegularData3DMessage(RegularData3DMessage::NEW);
	msg->setRegularData3D(dat);
	msg->setComposite(getMainControl()->getSelectedSystem());
	msg->setCompositeName(result.ascii());
	notify_(msg);
}

void DatasetControl::insertGrid_(RegularData3D* data, System& system, const String& name)
	throw()
{
	QListViewItem* item = new QListViewItem(listview, name.c_str(), system.getName().c_str());
	item_to_grid_[item] = data;
	insertComposite_(&system, item);
}

void DatasetControl::save3DGrid_()
	throw()
{
	String result = QFileDialog::getSaveFileName("", "*", 0, "Select a RegularData file").ascii();
	if (result.isEmpty()) return;

	File outfile;
	
	try
	{
		outfile.open(result, std::ios::out);
	}
	catch(Exception::GeneralException)
	{
		Log.error() << "File could not be written!" << std::endl;
		return;
	}

	outfile << * item_to_grid_[context_item_];
	outfile.close();
	setStatusbarText("3D Grid successfully written...");
}

void DatasetControl::updateSelection()
	throw()
{
	RegularData3DMessage* message = new RegularData3DMessage(RegularData3DMessage::SELECTED);
	message->setRegularData3D(0);
	QListViewItemIterator it(listview);
	for (; it.current(); ++it)
	{
		QListViewItem* item = it.current();
		if (item->isSelected() &&
				item_to_grid_.has(item))
		{
			message->setRegularData3D(item_to_grid_[item]);
			message->setCompositeName(item->text(0).ascii());
			break;
		}
	}
	notify_(message);
}

} } // namespaces
