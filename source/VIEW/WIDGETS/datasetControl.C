// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: datasetControl.C,v 1.33 2004/11/15 17:33:29 anhi Exp $

#include <BALL/VIEW/WIDGETS/datasetControl.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/KERNEL/system.h>
#include <BALL/FORMAT/DCDFile.h>
#include <BALL/VIEW/DIALOGS/snapShotVisualisation.h>
#include <BALL/VIEW/WIDGETS/regularData1DWidget.h>
#include <BALL/VIEW/WIDGETS/regularData2DWidget.h>

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
	listview->addColumn("Type");
	listview->setColumnWidth(0, 120);
	listview->setColumnWidth(1, 60);
	listview->setColumnWidth(2, 60);
	default_visible_ = false;
	connect(listview, SIGNAL(selectionChanged()), this, SLOT(updateSelection()));
	registerWidget(this);
}


DatasetControl::~DatasetControl()
	throw()
{
  #ifdef BALL_VIEW_DEBUG
	  Log.error() << "Destructing object " << this << " of class DatasetControl" << std::endl;
  #endif 

	if (dialog_ != 0) delete dialog_;
}


void DatasetControl::initializeWidget(MainControl& main_control)
	throw()
{
	open_trajectory_id_ = 
		main_control.insertMenuEntry(MainControl::FILE_OPEN, "Trajectory", this, SLOT(addTrajectory()), 0, -1,
		String("Open a trajectory file (1 System has to be selected)"));
	main_control.insertMenuEntry(MainControl::FILE_OPEN, "1D Grid", this, SLOT(add1DGrid()), 0, -1,
		String("Open a 1D data grid"));
	main_control.insertMenuEntry(MainControl::FILE_OPEN, "2D Grid", this, SLOT(add2DGrid()), 0, -1,
		String("Open a 2D data grid"));
	main_control.insertMenuEntry(MainControl::FILE_OPEN, "3D Grid", this, SLOT(add3DGrid()), 0, -1,
		String("Open a 3D data grid"));
	GenericControl::initializeWidget(main_control);
}


void DatasetControl::finalizeWidget(MainControl& main_control)
	throw()
{
	main_control.removeMenuEntry(MainControl::FILE_OPEN, "Trajectory", this, SLOT(addTrajectory()));
	main_control.removeMenuEntry(MainControl::FILE_OPEN, "1D Grid", this, SLOT(add1DGrid()));
	main_control.removeMenuEntry(MainControl::FILE_OPEN, "2D Grid", this, SLOT(add2DGrid()));
	main_control.removeMenuEntry(MainControl::FILE_OPEN, "3D Grid", this, SLOT(add3DGrid()));
	GenericControl::finalizeWidget(main_control);
}


void DatasetControl::checkMenu(MainControl& main_control)
	throw()
{
	menuBar()->setItemEnabled(open_trajectory_id_, main_control.getSelectedSystem());
	ItemList item_list = getSelectedItems(); 
	if (item_list.size() > 0) main_control.setDeleteEntryEnabled(true);
}


void DatasetControl::addTrajectory()
	throw()
{
	if (!getMainControl()->getSelectedSystem()) return;

	QString file = QFileDialog::getOpenFileName(
											"DCD files(*.dcd)",
											getWorkingDir().c_str(),
											this,
											"Trajectory File Dialog",
											"Select a DCD file" );

	if (file == QString::null) return;
	setWorkingDirFromFilename_(file.ascii());

	// construct a name for the system(the filename without the dir path)
	DCDFile* dcd = new DCDFile(file.ascii(), std::ios::in);
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
	
	QListViewItem* item = new QListViewItem(listview, name.c_str(), system.getName().c_str(), "Trajectory");
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

	GenericControl::onNotify(message);
	if (RTTI::isKindOf<RegularDataMessage>(*message))
	{
		if (RTTI::isKindOf<RegularData1DMessage>(*message))
		{
			RegularData1DMessage* ntm = RTTI::castTo<RegularData1DMessage>(*message);
			insertGrid_(ntm->getData(), (System*)ntm->getComposite(), ntm->getCompositeName());
			return;
		}
		if (RTTI::isKindOf<RegularData2DMessage>(*message))
		{
			RegularData2DMessage* ntm = RTTI::castTo<RegularData2DMessage>(*message);
			insertGrid_(ntm->getData(), (System*)ntm->getComposite(), ntm->getCompositeName());
			return;
		}
		if (RTTI::isKindOf<RegularData3DMessage>(*message))
		{
			RegularData3DMessage* ntm = RTTI::castTo<RegularData3DMessage>(*message);
			insertGrid_(ntm->getData(), (System*)ntm->getComposite(), ntm->getCompositeName());
			return;
		}
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
			deleteItem_(**lit);
		}
	}   
}

void DatasetControl::deleteItems_()
{
	ItemList item_list = getSelectedItems(); 
	ItemList::Iterator it = item_list.begin();
	for (; it != item_list.end(); it++)
	{
		deleteItem_(**it);
	}
}

void DatasetControl::deleteItem_(QListViewItem& item)
{
	if (!item_to_trajectory_.has(&item) &&
			!item_to_grid1_.has(&item) &&
			!item_to_grid2_.has(&item) &&
			!item_to_grid3_.has(&item))
	{
		return;
	}

	if (item_to_trajectory_.has(&item))
	{
		SnapShotManager* ssm = item_to_trajectory_[&item];
		item_to_trajectory_.erase(&item);
		delete ssm;
		setStatusbarText("deleted trajectory");
	}
	else if (item_to_grid1_.has(&item))
	{
		RegularData1D* ssm = item_to_grid1_[&item];

		RegularData1DMessage* msg = new RegularData1DMessage(RegularData1DMessage::REMOVE);
		msg->setData(*ssm);
		notify_(msg);

		item_to_grid3_.erase(&item);
		delete ssm;
		setStatusbarText("deleted 1D grid");
	}
	else if (item_to_grid2_.has(&item))
	{
		RegularData2D* ssm = item_to_grid2_[&item];

		RegularData2DMessage* msg = new RegularData2DMessage(RegularData2DMessage::REMOVE);
		msg->setData(*ssm);
		notify_(msg);

		item_to_grid2_.erase(&item);
		delete ssm;
		setStatusbarText("deleted 2D grid");
	}
	else
	{
		RegularData3D* ssm = item_to_grid3_[&item];

		RegularData3DMessage* msg = new RegularData3DMessage(RegularData3DMessage::REMOVE);
		msg->setData(*ssm);
		notify_(msg);

		item_to_grid3_.erase(&item);
		delete ssm;
		setStatusbarText("deleted 3D grid");
	}

	Composite* composite = item_to_composite_[&item];
	composite_to_items_[composite].erase(&item);
	item_to_composite_.erase(&item);
	GenericControl::removeItem_(&item, true);
}


void DatasetControl::onContextMenu_(QListViewItem* item,  const QPoint& point, int /* column */)
{
	if (item == 0) return;
	context_item_ = item;
	
	Position menu_entry_pos; 
	Size nr_items = getSelectedItems().size();
	QPopupMenu context_menu;
	if (item_to_trajectory_.has(item))
	{
		menu_entry_pos = context_menu.insertItem("Save", this, SLOT(saveTrajectory_()));
		if (nr_items > 1) context_menu.setItemEnabled(menu_entry_pos, false);
		menu_entry_pos = context_menu.insertItem("Visualise/Export", this, SLOT(visualiseTrajectory_()));
		if (nr_items > 1) context_menu.setItemEnabled(menu_entry_pos, false);
		menu_entry_pos = context_menu.insertItem("Load into RAM", this, SLOT(bufferTrajectory_()));
		if (nr_items > 1) context_menu.setItemEnabled(menu_entry_pos, false);
	}
	else if (item_to_grid1_.has(item))
	{
		menu_entry_pos = context_menu.insertItem("Save", this, SLOT(save1DGrid_()));
		if (nr_items > 1) context_menu.setItemEnabled(menu_entry_pos, false);
		menu_entry_pos = context_menu.insertItem("Visualise", this, SLOT(visualiseGrid_()));
		if (nr_items > 1) context_menu.setItemEnabled(menu_entry_pos, false);
	}
	else if (item_to_grid2_.has(item))
	{
		menu_entry_pos = context_menu.insertItem("Save", this, SLOT(save2DGrid_()));
		if (nr_items > 1) context_menu.setItemEnabled(menu_entry_pos, false);
		menu_entry_pos = context_menu.insertItem("Visualise", this, SLOT(visualiseGrid_()));
		if (nr_items > 1) context_menu.setItemEnabled(menu_entry_pos, false);
	}
	else if (item_to_grid3_.has(item))
	{
		menu_entry_pos = context_menu.insertItem("Save", this, SLOT(save3DGrid_()));
		if (nr_items > 1) context_menu.setItemEnabled(menu_entry_pos, false);
	}
	context_menu.insertItem("Delete", this, SLOT(deleteItems_()));

	// show the context menu if it is not empty
	//if (context_menu.count()) context_menu.popup(point);
	if (context_menu.count()) context_menu.exec(point);
}

void DatasetControl::visualiseTrajectory_()
{
	if (dialog_ != 0) 
	{
		dialog_->hide();
		delete dialog_;
		dialog_ = 0;
	}

	SnapShotManager* ssm = item_to_trajectory_[context_item_];

	dialog_ = new SnapshotVisualisationDialog(this);
	dialog_->setSnapShotManager(ssm);
	dialog_->show();
}

void DatasetControl::bufferTrajectory_()
{
	SnapShotManager* ssm = item_to_trajectory_[context_item_];

	if (ssm->getNumberOfSnapShotsInBuffer() == 0)
	{
		if (!ssm->readFromFile())
		{
			ssm->clearBuffer();
			setStatusbarText("Could not read trajectories into buffer! Out of memory?");
		}
	}
}

void DatasetControl::visualiseGrid_()
{
	if (item_to_grid1_.has(context_item_))
	{
		RegularData1D* grid = item_to_grid1_[context_item_];
		DockableRegularData1DWidget* widget = new DockableRegularData1DWidget(grid, getMainControl());
		widget->show();
		widget->zoomToFit();
		widget->undock();
	}
	else if (item_to_grid2_.has(context_item_))
	{
		RegularData2D* grid = item_to_grid2_[context_item_];
		DockableRegularData2DWidget* widget = new DockableRegularData2DWidget(grid, getMainControl());
		widget->show();
		widget->zoomToFit();
		widget->undock();
	}
}


void DatasetControl::saveTrajectory_()
{
	SnapShotManager* ssm = item_to_trajectory_[context_item_];

	QString s = QFileDialog::getSaveFileName(
								getWorkingDir().c_str(),
								"DCD files(*.dcd)",
								getMainControl(),
								"Trajectory File Dialog",
								"Choose a filename to save" );

	if (s == QString::null) return;
	String filename = s.ascii();
	setWorkingDirFromFilename_(filename);

	ssm->getTrajectoryFile()->copyTo(filename);
	setStatusbarText("Writen DCDFile");
}

String DatasetControl::chooseGridFileForOpen_()
	throw()
{
	QString result = QFileDialog::getOpenFileName("", "*", 0, "Select a RegularData file");
	if (result == QString::null) return "";
	setWorkingDirFromFilename_(result.ascii());

	File infile;
	
	try
	{
		infile.open(result.ascii(), std::ios::in);
	}
	catch(Exception::FileNotFound)
	{
		Log.error() << "File could not be found!" << std::endl;
		return "";
	}

	return result.ascii();
}


void DatasetControl::add1DGrid()
	throw()
{
	String filename = chooseGridFileForOpen_();
	if (filename == "") return;

	RegularData1D* dat = new RegularData1D;
	(*dat).binaryRead(filename);
	insertGrid_(dat, 0, filename);
	RegularData1DMessage* msg = new RegularData1DMessage(RegularData1DMessage::NEW);
	msg->setData(*dat);
	msg->setCompositeName(filename);
	notify_(msg);
}

void DatasetControl::add2DGrid()
	throw()
{
	String filename = chooseGridFileForOpen_();
	if (filename == "") return;

	RegularData2D* dat = new RegularData2D;
	(*dat).binaryRead(filename);
	insertGrid_(dat, 0, filename);
	RegularData2DMessage* msg = new RegularData2DMessage(RegularData2DMessage::NEW);
	msg->setData(*dat);
	msg->setCompositeName(filename);
	notify_(msg);
}


void DatasetControl::add3DGrid()
	throw()
{
	String filename = chooseGridFileForOpen_();
	if (filename == "") return;

	RegularData3D* dat = new RegularData3D;
	(*dat).binaryRead(filename);
	insertGrid_(dat, 0, filename);
	RegularData3DMessage* msg = new RegularData3DMessage(RegularData3DMessage::NEW);
	msg->setData(*dat);
	msg->setCompositeName(filename);
	notify_(msg);
}

void DatasetControl::insertGrid_(RegularData1D* data, System* system, const String& name)
	throw()
{
	QListViewItem* item;
	if (system != 0) 
	{
		item = new QListViewItem(listview, name.c_str(), system->getName().c_str(), "1D Grid");
	}
	else
	{ 	
		item = new QListViewItem(listview, name.c_str(), "", "1D Grid");
	}
	item_to_grid1_[item] = data;
	insertComposite_(system, item);
}

void DatasetControl::insertGrid_(RegularData2D* data, System* system, const String& name)
	throw()
{
	QListViewItem* item;
	if (system != 0) 
	{
		item = new QListViewItem(listview, name.c_str(), system->getName().c_str(), "2D Grid");
	}
	else
	{ 	
		item = new QListViewItem(listview, name.c_str(), "", "2D Grid");
	}
	item_to_grid2_[item] = data;
	insertComposite_(system, item);
}


void DatasetControl::insertGrid_(RegularData3D* data, System* system, const String& name)
	throw()
{
	QListViewItem* item;
	if (system != 0) 
	{
		item = new QListViewItem(listview, name.c_str(), system->getName().c_str(), "3D Grid");
	}
	else
	{ 	
		item = new QListViewItem(listview, name.c_str(), "", "3D Grid");
	}
	item_to_grid3_[item] = data;
	insertComposite_(system, item);
}


String DatasetControl::chooseGridFileForSave_()
	throw()
{
	QString qs = QFileDialog::getSaveFileName("", "*", 0, "Select a RegularData file");
	if (qs == QString::null) return "";
	setWorkingDirFromFilename_(qs.ascii());

	String result = qs.ascii();
	if (result.isEmpty()) return 0;

	File test;
	
	try
	{
		test.open(result, std::ios::out);
	}
	catch(Exception::GeneralException)
	{
		Log.error() << "File could not be written!" << std::endl;
		return "";
	}

	return result;
}

void DatasetControl::save1DGrid_()
	throw()
{ 
	String filename = chooseGridFileForSave_();
	if (filename == "") return;

	item_to_grid1_[context_item_]->binaryWrite(filename);
	setStatusbarText("Grid successfully written...");
}

void DatasetControl::save2DGrid_()
	throw()
{
	String filename = chooseGridFileForSave_();
	if (filename == "") return;

	item_to_grid2_[context_item_]->binaryWrite(filename);
	setStatusbarText("Grid successfully written...");
}

void DatasetControl::save3DGrid_()
	throw()
{
	String filename = chooseGridFileForSave_();
	if (filename == "") return;

	item_to_grid3_[context_item_]->binaryWrite(filename);
	setStatusbarText("Grid successfully written...");
}

void DatasetControl::updateSelection()
	throw()
{
	GenericControl::updateSelection();

	QListViewItemIterator it(listview);
	for (; it.current(); ++it)
	{
		QListViewItem* item = it.current();
		if (!item->isSelected()) continue;

		if (item_to_grid1_.has(item))
		{
			RegularData1DMessage* message = new RegularData1DMessage(RegularData1DMessage::SELECTED);
			message->setData(*item_to_grid1_[item]);
			message->setCompositeName(item->text(0).ascii());
			notify_(message);
			break;
		}

		if (item_to_grid2_.has(item))
		{
			RegularData2DMessage* message = new RegularData2DMessage(RegularData2DMessage::SELECTED);
			message->setData(*item_to_grid2_[item]);
			message->setCompositeName(item->text(0).ascii());
			notify_(message);
			break;
		}

		if (item_to_grid3_.has(item))
		{
			RegularData3DMessage* message = new RegularData3DMessage(RegularData3DMessage::SELECTED);
			message->setData(*item_to_grid3_[item]);
			message->setCompositeName(item->text(0).ascii());
			notify_(message);
			break;
		}
	}
}

List<std::pair<RegularData3D*, String> > DatasetControl::get3DGrids()
	throw()
{
	List<std::pair<RegularData3D*,String> > grids;
	HashMap<QListViewItem*, RegularData3D*>::Iterator it = item_to_grid3_.begin();
	for (; it != item_to_grid3_.end(); it++)
	{
		std::pair<RegularData3D*, String> p((*it).second, (*it).first->text(0).ascii());
		grids.push_back(p);
	}
	return grids;
}

} } // namespaces
