// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: trajectoryControl.C,v 1.1 2003/09/01 22:28:10 amoll Exp $

#include <BALL/VIEW/WIDGETS/trajectoryControl.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/VIEW/KERNEL/common.h>
#include <BALL/KERNEL/system.h>
#include <BALL/FORMAT/DCDFile.h>
#include <BALL/VIEW/DIALOGS/snapShotVisualisation.h>
#include <qpopupmenu.h>
#include <qmenubar.h>
#include <qfiledialog.h>

using std::endl;

namespace BALL
{
	namespace VIEW
	{

TrajectoryControl::TrajectoryControl(QWidget* parent, const char* name)
	throw()
		:	QListView(parent, name),
			ModularWidget(name),
			context_menu_(),
			context_trajectory_(0),
			context_item_(0),
			dialog_(0),
			visualise_id_(0)
{
	// appearance
	setRootIsDecorated(TRUE);
	setSorting(-1);
	setSelectionMode(QListView::Extended);
	addColumn("Name");
	addColumn("Properties");
	setColumnWidth(0, 120);
	setColumnWidth(1, 60);

	// if the selection of any item changed,
	// mark the complete selection as invalid
	// it is then re-determined by getSelection()
//	connect(this, SIGNAL(selectionChanged()), this, SLOT(updateSelection()));

	connect(this, SIGNAL(rightButtonPressed(QListViewItem*, const QPoint&, int)), this,
					SLOT(onContextMenu_(QListViewItem*, const QPoint&, int)));

	// register ModularWidget
	registerWidget(this);
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
	visualise_id_ = 
		main_control.insertMenuEntry(MainControl::FILE, "Load Trajectory", this, SLOT(addTrajectory()));
	window_menu_entry_id_ = 
		main_control.insertMenuEntry(MainControl::WINDOWS, "TrajectoryControl", this, SLOT(switchShowWidget()));
	getMainControl()->menuBar()->setItemChecked(window_menu_entry_id_, true);
}


void TrajectoryControl::finalizeWidget(MainControl& main_control)
	throw()
{
	main_control.removeMenuEntry(MainControl::FILE, "Load Trajectory", this, SLOT(addTrajectory()));
	main_control.removeMenuEntry(MainControl::WINDOWS, "TrajectoryControl", this, SLOT(switchShowWidget()));
}


void TrajectoryControl::checkMenu(MainControl& main_control)
	throw()
{
	getMainControl()->menuBar()->setItemEnabled(visualise_id_, main_control.getSelectedSystem());
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
	if (dcd->getNumberOfAtoms() != getMainControl()->getSelectedSystem()->countAtoms())
	{
		setStatusbarText("Number of atoms do not match. Aborting...");
		delete dcd;
		return;
	}

	SnapShotManager* manager = new SnapShotManager(getMainControl()->getSelectedSystem(), 0, dcd);
	QListViewItem* item = new QListViewItem(this, filename.c_str(), QString(" "));
	item_to_trajectory_[item] = manager;
	// update the view
	updateContents();
}


void TrajectoryControl::onNotify(Message *message)
	throw()
{
	if (RTTI::isKindOf<RemovedCompositeMessage>(*message))
  {
    RemovedCompositeMessage *composite_message = RTTI::castTo<RemovedCompositeMessage>(*message);
    Composite* composite = (Composite *)composite_message->getComposite();
		HashMap<QListViewItem*, SnapShotManager*>::Iterator it = item_to_trajectory_.begin();
		for(;it != item_to_trajectory_.end(); it++)
		{
			if (it->second->getSystem() == composite)
			{
				context_item_ = it->first;
				deleteTrajectory_();
				return;
			}
		}
	}   
}


void TrajectoryControl::insertContextMenuEntry(const String& name, const QObject* receiver, const char* slot, int entry_ID, int accel)
	throw()
{
	context_menu_.insertItem(name.c_str(), receiver, slot, accel, entry_ID);
}


void TrajectoryControl::invalidateSelection()
{
	QListViewItemIterator it(this);
	for (; it.current(); ++it)
	{
		it.current()->setSelected(FALSE);
	}

	updateContents();
}


void TrajectoryControl::deleteTrajectory_()
{
	SnapShotManager* ssm = item_to_trajectory_[context_item_];
	if (!ssm) Log.error() << "Could not delete trajectory" << std::endl;
	delete ssm;
	item_to_trajectory_.erase(context_item_);
	delete context_item_;
	updateContents();

	setStatusbarText("deleted trajectory");
}


void TrajectoryControl::onContextMenu_(QListViewItem* item,  const QPoint& point, int /* column */)
{
	// clear the context menu
	context_menu_.clear();

	// get composite address
	if (item == 0) return;

	context_item_ = item;

	insertContextMenuEntry("Save", this, SLOT(saveTrajectory_()));
	insertContextMenuEntry("Visualise", this, SLOT(visualiseTrajectory_()));
	insertContextMenuEntry("Delete", this, SLOT(deleteTrajectory_()));
//	insertContextMenuEntry("Export to PNGs", this, SLOT(visualiseTrajectory_()));

	// show the context menu if it is not empty
	if (context_menu_.count())
	{
		context_menu_.exec(point);
	}
}


void TrajectoryControl::switchShowWidget()
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

void TrajectoryControl::writePreferences(INIFile& inifile)
	throw()
{
	inifile.insertValue("WINDOWS", "TrajectoryControl::on", 
		String(getMainControl()->menuBar()->isItemChecked(window_menu_entry_id_)));
}

void TrajectoryControl::fetchPreferences(INIFile & inifile)
	throw()
{
	if (!inifile.hasEntry("WINDOWS", "TrajectoryControl::on")) return;
	if (inifile.getValue("WINDOWS", "TrajectoryControl::on").toUnsignedInt() == 0) 
	{
		switchShowWidget();
	}
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
	delete fd;
Log.error() << "#~~#   17  " << ssm->getNumberOfSnapShotsInBuffer()<< std::endl;
	DCDFile dcd(filename, File::OUT);
	ssm->setTrajectoryFile(&dcd);
	ssm->flushToDisk();
	setStatusbarText("Writen DCDFile");
	ssm->setTrajectoryFile(0);
}


	} // namespace VIEW
} // namespace BALL
