// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: trajectoryControl.C,v 1.10 2003/09/17 22:16:41 amoll Exp $

#include <BALL/VIEW/WIDGETS/trajectoryControl.h>
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

TrajectoryControl::TrajectoryControl(QWidget* parent, const char* name)
	throw()
	:	GenericControl(parent, name),
		context_trajectory_(0),
		dialog_(0)
{
	listview->addColumn("Name");
	listview->addColumn("from");
	listview->setColumnWidth(0, 120);
	listview->setColumnWidth(1, 60);
	default_visible_ = false;
}


TrajectoryControl::~TrajectoryControl()
	throw()
{
  #ifdef BALL_VIEW_DEBUG
	  Log.error() << "Destructing object " << (void *)this << " of class " 
								<< RTTI::getName<TrajectoryControl>() << endl;
  #endif 

	if (dialog_) delete dialog_;
}


void TrajectoryControl::initializeWidget(MainControl& main_control)
	throw()
{
	open_trajectory_id_ = 
		main_control.insertMenuEntry(MainControl::FILE, "Open Trajectory", this, SLOT(addTrajectory()));
	main_control.menuBar()->
		setWhatsThis(open_trajectory_id_, "To open a trajectory file, one System has to be selected.");
	GenericControl::initializeWidget(main_control);
}


void TrajectoryControl::finalizeWidget(MainControl& main_control)
	throw()
{
	main_control.removeMenuEntry(MainControl::FILE, "Open Trajectory", this, SLOT(addTrajectory()));
	GenericControl::finalizeWidget(main_control);
}


void TrajectoryControl::checkMenu(MainControl& main_control)
	throw()
{
	getMainControl()->menuBar()->setItemEnabled(open_trajectory_id_, main_control.getSelectedSystem());
}


void TrajectoryControl::addTrajectory()
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

void TrajectoryControl::insertTrajectory_(TrajectoryFile* file, System& system)
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
	listview->triggerUpdate();
}


void TrajectoryControl::onNotify(Message *message)
	throw()
{
	if (RTTI::isKindOf<RemovedCompositeMessage>(*message))
  {
    RemovedCompositeMessage *composite_message = RTTI::castTo<RemovedCompositeMessage>(*message);
    Composite* composite = (Composite *)composite_message->getComposite();
		List<QListViewItem*> to_delete;
		HashMap<QListViewItem*, SnapShotManager*>::Iterator it = item_to_trajectory_.begin();
		for(;it != item_to_trajectory_.end(); it++)
		{
			if (it->second->getSystem() == composite)
			{
				to_delete.push_back(it->first);
			}
		}

		List<QListViewItem*>::Iterator lit = to_delete.begin();
		for (;lit != to_delete.end(); lit++)
		{
			context_item_ = *lit;
			deleteTrajectory_();
		}
	}   
	else if (RTTI::isKindOf<NewTrajectoryMessage>(*message))
	{
		NewTrajectoryMessage* ntm = RTTI::castTo<NewTrajectoryMessage>(*message);
		insertTrajectory_(ntm->getTrajectoryFile(), *(System*)ntm->getComposite());
		return;
	}
}

void TrajectoryControl::deleteTrajectory_()
{
	SnapShotManager* ssm = item_to_trajectory_[context_item_];
	if (!ssm) Log.error() << "Could not delete trajectory" << std::endl;
	delete ssm;
	item_to_trajectory_.erase(context_item_);
	delete context_item_;
	listview->triggerUpdate();

	setStatusbarText("deleted trajectory");
}


void TrajectoryControl::onContextMenu_(QListViewItem* item,  const QPoint& point, int /* column */)
{
	if (item == 0) return;
	context_item_ = item;

	QPopupMenu context_menu;
	context_menu.insertItem("Save", this, SLOT(saveTrajectory_()));
	context_menu.insertItem("Visualise/Export", this, SLOT(visualiseTrajectory_()));
	context_menu.insertItem("Delete", this, SLOT(deleteTrajectory_()));
//	insertContextMenuEntry("Export to PNGs", this, SLOT(visualiseTrajectory_()));

	// show the context menu if it is not empty
	if (context_menu.count()) context_menu.exec(point);
}

void TrajectoryControl::visualiseTrajectory_()
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

void TrajectoryControl::saveTrajectory_()
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

} } // namespaces
