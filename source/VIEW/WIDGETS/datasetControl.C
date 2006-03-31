// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: datasetControl.C,v 1.46.2.14 2006/03/31 16:42:15 amoll Exp $
//

#include <BALL/VIEW/WIDGETS/datasetControl.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/VIEW/DATATYPE/colorMap.h>
#include <BALL/VIEW/PRIMITIVES/illuminatedLine.h>
#include <BALL/VIEW/PRIMITIVES/point.h>

#include <BALL/VIEW/DIALOGS/snapShotVisualisation.h>
#include <BALL/VIEW/DIALOGS/contourSurfaceDialog.h>
#include <BALL/VIEW/DIALOGS/fieldLinesDialog.h>

//   #include <BALL/VIEW/WIDGETS/regularData1DWidget.h>
//   #include <BALL/VIEW/WIDGETS/regularData2DWidget.h>

#include <BALL/VIEW/PRIMITIVES/mesh.h>
#include <BALL/VIEW/PRIMITIVES/line.h>

#include <BALL/FORMAT/DCDFile.h>
#include <BALL/FORMAT/DSN6File.h>
#include <BALL/MOLMEC/COMMON/snapShotManager.h>
#include <BALL/DATATYPE/contourSurface.h>
#include <BALL/STRUCTURE/DOCKING/dockResult.h>

#include <QtGui/QFileDialog>

namespace BALL
{
	namespace VIEW
	{

		DatasetControl::DatasetControl(QWidget* parent, const char* name)
			throw()
			:	GenericControl(parent, name),
				dialog_(0),
				surface_dialog_(0)
		{
		#ifdef BALL_VIEW_DEBUG
			Log.error() << "new DatasetControl " << this << std::endl;
		#endif
			listview->setColumnCount(3);
			listview->headerItem()->setText(0, "Name");
			listview->headerItem()->setText(1, "from");
			listview->headerItem()->setText(2, "Type");
			default_visible_ = false;
			connect(listview, SIGNAL(selectionChanged()), this, SLOT(updateSelection()));
			setMinimumSize(50,50);
			resize(50,50);
			registerWidget(this);
		}


		DatasetControl::~DatasetControl()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.error() << "Destructing object " << this << " of class DatasetControl" << std::endl;
			#endif 

			if (dialog_ != 0) delete dialog_;

			listview->clear();
		}


		void DatasetControl::initializeWidget(MainControl& main_control)
			throw()
		{
			open_trajectory_id_ = insertMenuEntry(MainControl::FILE_OPEN, "Trajectory", 
														this, SLOT(addTrajectory()));
			setMenuHint("Open a trajectory file (1 System has to be selected)");

			insertMenuEntry(MainControl::FILE_OPEN, "1D Grid", this, SLOT(add1DGrid()));
			setMenuHint("Open a 1D data grid");

			insertMenuEntry(MainControl::FILE_OPEN, "2D Grid", this, SLOT(add2DGrid()));
			setMenuHint("Open a 2D data grid");

			insertMenuEntry(MainControl::FILE_OPEN, "3D Grid", this, SLOT(add3DGrid()));
			setMenuHint("Open a 3D data grid");

			open_gradient_id_ = insertMenuEntry(MainControl::FILE_OPEN, "Vector Grid", this, SLOT(addVectorGrid()));

			insertMenuEntry(MainControl::FILE_OPEN, "DSN6 electron density map", this, SLOT(addDSN6Grid()));
			setMenuHint("Open an electron density file in DSN6 format (e.g. .omap files)" );

			setMenuHint("Open a gradient grid");

			insertMenuEntry(MainControl::FILE_OPEN, "Dock Result", this, SLOT(addDockResult()));
			setMenuHint("Open a dock result file");

			menu_cs_ = insertMenuEntry(MainControl::TOOLS, "Contour S&urface", this,  
																							SLOT(computeIsoContourSurface()), Qt::CTRL+Qt::Key_U);
			setMenuHint("Calculate an isocontour surface from a 3D grid. The grid has to be loaded in the DatasetControl.");
			setMenuHelp("datasetControl.html#isocontour_surfaces");

			GenericControl::initializeWidget(main_control);

			registerForHelpSystem(this, "datasetControl.html");
		}


		void DatasetControl::checkMenu(MainControl& main_control)
			throw()
		{
			bool system = main_control.getSelectedSystem();
			open_trajectory_id_->setEnabled(system);
			open_gradient_id_->setEnabled(system);
			if (getSelectedItems().size() > 0) main_control.setDeleteEntryEnabled(true);

			menu_cs_->setEnabled(!getMainControl()->isBusy() && item_to_grid3_.size() > 0);
		}


		void DatasetControl::addTrajectory()
			throw()
		{
			if (!getMainControl()->getSelectedSystem()) return;

			QString file = QFileDialog::getOpenFileName(0,
													"Save as a DCD Trajectory File",
													getWorkingDir().c_str(),
													"DCD files(*.dcd)");

			if (file == QString::null) return;

			addTrajectory(ascii(file));
		}

		void DatasetControl::addTrajectory(const String& filename)
		{
			if (getMainControl()->getSelectedSystem() == 0) return;

			// construct a name for the system(the filename without the dir path)
			DCDFile* dcd = new DCDFile(filename, std::ios::in);
			insertTrajectory_(dcd, *getMainControl()->getSelectedSystem());
			setWorkingDirFromFilename_(filename);
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

			QStringList sl;
			sl << name.c_str() << system.getName().c_str() << "Trajectory";
			QTreeWidgetItem* item = addRow(sl);
			item_to_trajectory_[item] = manager;
			insertComposite_(&system, item);
		}


		void DatasetControl::insertComposite_(Composite* composite, QTreeWidgetItem* item)
			throw()
		{
//   			listview->triggerUpdate();
			item_to_composite_[item] = composite;
			if (composite_to_items_.has(composite))
			{
				composite_to_items_[composite].insert(item);
			}
			else
			{
				HashSet<QTreeWidgetItem*> set;
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
			else if (RTTI::isKindOf<NewDockResultMessage>(*message))
			{
				NewDockResultMessage* dock_res_m = RTTI::castTo<NewDockResultMessage>(*message);
				insertDockResult_(dock_res_m->getDockResult(), *(System*)dock_res_m->getComposite());
				return;
			}  
			else if (RTTI::isKindOf<CompositeMessage>(*message))
			{
				CompositeMessage *composite_message = RTTI::castTo<CompositeMessage>(*message);
				if (composite_message->getType() != CompositeMessage::REMOVED_COMPOSITE) return;
				Composite* composite = (Composite *)composite_message->getComposite();
				if (!composite_to_items_.has(composite)) return;

				// create a copy of the hashset, because it changes
				HashSet<QTreeWidgetItem*> to_delete = composite_to_items_[composite];

				HashSet<QTreeWidgetItem*>::Iterator lit = to_delete.begin();
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
				removeItem_(*it);
			}
		}

		bool DatasetControl::deleteItem_(QTreeWidgetItem& item)
		{
			if (item_to_trajectory_.has(&item))
			{
				SnapShotManager* ssm = item_to_trajectory_[&item];
				item_to_trajectory_.erase(&item);
				delete ssm;
				setStatusbarText("deleted trajectory");
			}
			else if (item_to_dock_result_.has(&item))
			{
				DockResult* dock_res = item_to_dock_result_[&item];
				item_to_dock_result_.erase(&item);
				delete dock_res;
				setStatusbarText("deleted dock result");
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
			else if (item_to_grid3_.has(&item))
			{
				RegularData3D* ssm = item_to_grid3_[&item];

				RegularData3DMessage* msg = new RegularData3DMessage(RegularData3DMessage::REMOVE);
				msg->setData(*ssm);
				notify_(msg);

				item_to_grid3_.erase(&item);
				delete ssm;
				setStatusbarText("deleted 3D grid");
			}
			else if (item_to_gradients_.has(&item))
			{
				VectorGrid* grid = item_to_gradients_[&item];

				item_to_gradients_.erase(&item);
				delete grid;
				setStatusbarText("deleted gradient grid");
			}

			else
			{
				return false;
			}

			Composite* composite = item_to_composite_[&item];
			composite_to_items_[composite].erase(&item);
			item_to_composite_.erase(&item);
			return true;
		}


		void DatasetControl::createContextMenu_()
		{
			context_menu_.clear();

			insertContextMenuEntry_("Delete", SLOT(deleteCurrentItems()));

			if (item_to_trajectory_.has(context_item_))
			{
				insertContextMenuEntry_("Visualise/Export", SLOT(visualiseTrajectory_()));
				insertContextMenuEntry_("Load into RAM", SLOT(bufferTrajectory_()));
				return;
			}

			if (item_to_dock_result_.has(context_item_))
			{
				insertContextMenuEntry_("Save Trajectories", SLOT(saveDockTrajectories_()));
				insertContextMenuEntry_("Save Dock Result", SLOT(saveDockResult_()));
				insertContextMenuEntry_("Show Dock Result", SLOT(showDockResult_()));
				return;
			}
			
			if (item_to_grid1_.has(context_item_) ||
		      item_to_grid2_.has(context_item_) ||
					item_to_grid3_.has(context_item_))
			{
				insertContextMenuEntry_("Save", SLOT(saveGrid_()));
				if (!item_to_grid3_.has(context_item_))
				{
					insertContextMenuEntry_("Visualise", SLOT(visualiseGrid_()));
				}
				else
				{
					insertContextMenuEntry_("ContourSurface", SLOT(computeIsoContourSurface()));
					insertContextMenuEntry_("Create gradient grid", SLOT(createVectorGrid()));
				}
			}

			if (item_to_gradients_.has(context_item_))
			{
				insertContextMenuEntry_("Save", SLOT(saveGrid_()));
				insertContextMenuEntry_("Visualise field lines", SLOT(visualiseFieldLines_()));
			}
		}

		void DatasetControl::insertContextMenuEntry_(const QString & text, const char* member)
		{
			QAction* menu_entry_pos = context_menu_.addAction(text, this, member);
			if (getSelectedItems().size() > 1) menu_entry_pos->setEnabled(false);
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
			/*
			if (item_to_grid1_.has(context_item_))
			{
				RegularData1D* grid = item_to_grid1_[context_item_];
				DockableRegularData1DWidget* widget = new DockableRegularData1DWidget(grid, getMainControl());
				widget->show();
				widget->zoomToFit();
				widget->setFloating(true);
			}
			else if (item_to_grid2_.has(context_item_))
			{
				RegularData2D* grid = item_to_grid2_[context_item_];
				DockableRegularData2DWidget* widget = new DockableRegularData2DWidget(grid, getMainControl());
				widget->show();
				widget->zoomToFit();
				widget->setFloating(true);
			}
			*/
		}


		void DatasetControl::saveTrajectory_()
		{
			SnapShotManager* ssm = item_to_trajectory_[context_item_];

			QString s = QFileDialog::getSaveFileName(
										0,
										"Choose a DCD file to save",
										getWorkingDir().c_str(),
										"DCD files(*.dcd)");

			if (s == QString::null) return;
			String filename = ascii(s);

			setWorkingDirFromFilename_(filename);

			if (!ssm->getTrajectoryFile()->copyTo(filename))
			{
				if (ssm->getTrajectoryFile()->getName() == filename)
				{
					setStatusbarText("Could not write DCDFile, you tried to save the file onto itself.", 
													 true);
				}
				else
				{
					setStatusbarText("Could not write DCDFile.", true);
				}

				return;
			}
													

			setStatusbarText("Written DCDFile", true);
		}

		String DatasetControl::chooseGridFileForOpen_()
			throw()
		{
			QString result = QFileDialog::getOpenFileName(
					0, "Select a RegularData file", getWorkingDir().c_str(), "*");
			if (result == QString::null) return "";
			String filename = ascii(result);
			setWorkingDirFromFilename_(filename);

			File infile;
			
			try
			{
				infile.open(filename, std::ios::in);
			}
			catch(Exception::FileNotFound)
			{
				Log.error() << "File could not be found!" << std::endl;
				return "";
			}

			return filename;
		}


		void DatasetControl::add1DGrid()
			throw()
		{
			String filename = chooseGridFileForOpen_();
			if (filename == "") return;

			System* system = getMainControl()->getSelectedSystem();

			RegularData1D* dat = new RegularData1D;
			(*dat).binaryRead(filename);
			insertGrid_(dat, system, filename);
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

			System* system = getMainControl()->getSelectedSystem();

			RegularData2D* dat = new RegularData2D;
			(*dat).binaryRead(filename);
			insertGrid_(dat, system, filename);
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

			System* system = getMainControl()->getSelectedSystem();

			RegularData3D* dat = new RegularData3D;
			(*dat).binaryRead(filename);
			insertGrid_(dat, system, filename);
			RegularData3DMessage* msg = new RegularData3DMessage(RegularData3DMessage::NEW);
			msg->setData(*dat);
			msg->setCompositeName(filename);
			notify_(msg);
		}

		void DatasetControl::addDSN6Grid()
			throw()
		{
			String filename = chooseGridFileForOpen_();
			if (filename == "") return;

			System* system = 0;
			List<Composite*>& sel = getMainControl()->getMolecularControlSelection();
			if (sel.size() != 0) system = dynamic_cast<System*>(*sel.begin());

			RegularData3D* dat = new RegularData3D;
			DSN6File infile(filename, std::ios::in|std::ios::binary);
			infile.read(*dat);
			infile.close();

			insertGrid_(dat, system, filename);
			RegularData3DMessage* msg = new RegularData3DMessage(RegularData3DMessage::NEW);
			msg->setData(*dat);
			msg->setCompositeName(filename);
			notify_(msg);
		}
		
		void DatasetControl::insertGrid_(RegularData1D* data, System* system, const String& name)
			throw()
		{
			QTreeWidgetItem* item = createListViewItem_(system, name, "1D Grid");
			item_to_grid1_[item] = data;
			item_to_composite_[item] = system;
		}

		void DatasetControl::insertGrid_(RegularData2D* data, System* system, const String& name)
			throw()
		{
			QTreeWidgetItem* item = createListViewItem_(system, name, "2D Grid");
			item_to_grid2_[item] = data;
			item_to_composite_[item] = system;
		}

		void DatasetControl::insertGrid_(RegularData3D* data, System* system, const String& name)
			throw()
		{
			QTreeWidgetItem* item = createListViewItem_(system, name, "3D Grid");
			item_to_grid3_[item] = data;
			item_to_composite_[item] = system;
		}


		QTreeWidgetItem* DatasetControl::createListViewItem_(System* system, const String& name, const String& type)
			throw()
		{
			QStringList sl;
			sl << name.c_str();

			if (system != 0) 	sl << system->getName().c_str();
			else 							sl << "";

			sl << type.c_str();

			QTreeWidgetItem* item = addRow(sl);

			if (system != 0) insertComposite_(system, item);
			return item;
		}


		String DatasetControl::chooseGridFileForSave_()
			throw()
		{
			QString qs = QFileDialog::getSaveFileName(
					0, "Select a RegularData file", getWorkingDir().c_str(), "*");
			if (qs == QString::null) return "";

			String result = ascii(qs);
			if (result.isEmpty()) return 0;

			setWorkingDirFromFilename_(result);

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

		void DatasetControl::saveGrid_()
			throw()
		{ 
			String filename = chooseGridFileForSave_();
			if (filename == "") return;

			if (item_to_grid1_.has(context_item_))
			{
				item_to_grid1_[context_item_]->binaryWrite(filename);
			}
			else if (item_to_grid2_.has(context_item_))
			{
				item_to_grid2_[context_item_]->binaryWrite(filename);
			}
			else if (item_to_grid3_.has(context_item_))
			{
				item_to_grid3_[context_item_]->binaryWrite(filename);
			}
			else if (item_to_gradients_.has(context_item_))
			{
				item_to_gradients_[context_item_]->binaryWrite(filename);
			}

			setStatusbarText("Grid successfully written...");
		}

		void DatasetControl::updateSelection()
			throw()
		{
			GenericControl::updateSelection();

			ItemList list = getSelectedItems();
			ItemList::iterator it = list.begin();
			for (; it != list.end(); it++)
			{
				QTreeWidgetItem* item = *it;

				if (item_to_grid1_.has(item))
				{
					RegularData1DMessage* message = new RegularData1DMessage(RegularData1DMessage::SELECTED);
					message->setData(*item_to_grid1_[item]);
					message->setCompositeName(ascii(item->text(1)));
					notify_(message);
					break;
				}

				if (item_to_grid2_.has(item))
				{
					RegularData2DMessage* message = new RegularData2DMessage(RegularData2DMessage::SELECTED);
					message->setData(*item_to_grid2_[item]);
					message->setCompositeName(ascii(item->text(1)));
					notify_(message);
					break;
				}

				if (item_to_grid3_.has(item))
				{
					RegularData3DMessage* message = new RegularData3DMessage(RegularData3DMessage::SELECTED);
					message->setData(*item_to_grid3_[item]);
					message->setCompositeName(ascii(item->text(1)));
					notify_(message);
					break;
				}
			}
		}

		List<std::pair<RegularData3D*, String> > DatasetControl::get3DGrids()
			throw()
		{
			List<std::pair<RegularData3D*,String> > grids;
			HashMap<QTreeWidgetItem*, RegularData3D*>::Iterator it = item_to_grid3_.begin();
			for (; it != item_to_grid3_.end(); it++)
			{
				std::pair<RegularData3D*, String> p((*it).second, ascii((*it).first->text(0)));
				grids.push_back(p);
			}
			return grids;
		}


		void DatasetControl::computeIsoContourSurface()
			throw()
		{
			// execute the surface dialog and abort if cancel is clicked
			if (surface_dialog_ == 0) 
			{
				surface_dialog_ = new ContourSurfaceDialog(this, "ContourSurfaceDialog");
				surface_dialog_->setDatasetControl(this);
				registerForHelpSystem(surface_dialog_, "datasetControl.html#isocontour_surfaces");
			}
			if (!surface_dialog_->exec()) return;

			// Create a new contour surface.
			ContourSurface cs(*surface_dialog_->getGrid(), surface_dialog_->getThreshold());

			if (cs.vertex.size() == 0)
			{
				setStatusbarText("Could not calculate ContourSurface, no grid points found for threshold!", true);
				surface_dialog_->show();
				return;
			}

			Mesh* mesh = new Mesh;
			
			// reorient the vertices for OpenGL
			for (Position t = 0; t < cs.triangle.size(); t++)
			{
				const Mesh::Triangle& org = cs.triangle[t];
				Mesh::Triangle tri;
				tri.v1 = org.v1;
				tri.v2 = org.v3;
				tri.v3 = org.v2;
				mesh->triangle.push_back(tri);
			}

			mesh->vertex = cs.vertex;
			mesh->normal = cs.normal;

			mesh->colors.clear(); 
			mesh->colors.push_back(surface_dialog_->getColor());

			//////////////////////////////////////////////
			// Create a new representation containing the contour surface.
			Representation* rep = new Representation();
			rep->insert(*mesh);
			rep->setModelType(MODEL_CONTOUR_SURFACE); 

			// Make sure BALLView knows about the new representation.
			getMainControl()->insert(*rep);
			getMainControl()->update(*rep);
		}


		DatasetControl::DatasetControl(const DatasetControl& control)
			throw()
			: GenericControl(control)
		{
		}

	// DOCKING =================================>
	void DatasetControl::addDockResult()
		throw()
	{
		// open dialog to select result file
		QString file = QFileDialog::getOpenFileName(
												0,
												"Select a Dock Result file",
												getWorkingDir().c_str(),
												"DockResult files(*.dr)");

		String filename = ascii(file);
												
		if (file == QString::null) return;
		setWorkingDirFromFilename_(filename);
		
		// read the DockResult from the file
		DockResult* dock_res = new DockResult();
		if (!dock_res->readDockResult(filename))
		{
			setStatusbarText("Could not read DockResult file!");
			return;
		}
		
		/* // you can also use the operator>>
		File input(file.ascii(), std::ios::in | std::ios::binary);
		input >> *dock_res;
		input.close();
		*/
		
		// get docked system, set its name to name of file and add it to BALLView structures
		// apply the first SnapShot to the system to get the positions of the docked structures
		SnapShot ss = (*(dock_res->getConformationSet()))[0];
		System* docked_system = new System(dock_res->getConformationSet()->getSystem());
		String name = filename;
		vector<String> s;
		name.split(s, "/");
		docked_system->setName(s[s.size()-1].before("."));
		ss.applySnapShot(*docked_system);
		getMainControl()->insert(*docked_system);
		insertDockResult_(dock_res, *docked_system);
	}

	void DatasetControl::insertDockResult_(DockResult* dock_res, System& system)
		throw()
	{
		QString name = dock_res->getDockingAlgorithm().c_str();
		
		QStringList sl;
		sl << name << system.getName().c_str() << "DockResult";
		QTreeWidgetItem* item = new QTreeWidgetItem(listview, sl);
		item_to_dock_result_[item] = dock_res;
		insertComposite_(&system, item);
	}


	void DatasetControl::showDockResult_()
	{	
		ShowDockResultMessage* msg = new ShowDockResultMessage(item_to_dock_result_[context_item_], (System*) item_to_composite_[context_item_]);
		notify_(msg);
	}


	void DatasetControl::saveDockTrajectories_()
	{
		DockResult* dock_res = item_to_dock_result_[context_item_];
		
		QString s = QFileDialog::getSaveFileName(
									0,
									"Choose a Trajectory file to save",
									getWorkingDir().c_str(),
									"DCD files(*.dcd)");

		if (s == QString::null) return;
		String filename = ascii(s);

		setWorkingDirFromFilename_(filename);

		if (!dock_res->getConformationSet()->writeDCDFile(filename))
		{
			setStatusbarText("Could not write DCDFile.", true);
			return;
		}

		setStatusbarText("Written DCDFile.", true);
	}

	void DatasetControl::saveDockResult_()
	{
		DockResult* dock_res = item_to_dock_result_[context_item_];
		
		QString s = QFileDialog::getSaveFileName(0,
																							"Choose a file to save the docking result",
																							getWorkingDir().c_str(),
																							"DockResult files(*.dr)");
		if (s == QString::null) return;
		String filename = ascii(s);
		
		if (!dock_res->writeDockResult(filename))
		{
			setStatusbarText("Could not write DockResultFile.", true);
			return;
		}
		
		/* // you can also use the operator<<
		File file(filename, std::ios_base::out | std::ios::binary);
		file << *dock_res;
		file.close();
		*/
		
		setStatusbarText("Written DockResultFile.", true);
	}

	void DatasetControl::showGuestContextMenu(const QPoint& pos)
	{
		getSelectedItems();
		if (context_item_ == 0) return;
		createContextMenu_();
		context_menu_.exec(mapToGlobal(pos));
	}

	/////////////////////////////////////////////////////////////////////////
	//  GRADIENT GRIDS:
	/////////////////////////////////////////////////////////////////////////

	void DatasetControl::visualiseFieldLines_()
	{
		FieldLinesDialog dialog;
		if (!dialog.exec()) return;

		tolerance_ = dialog.getTolerance();
		atom_distance_ = dialog.getAtomsDistance();
		icosaeder_steps_ = dialog.getIcosaederInterplationSteps();
		max_steps_ = dialog.getMaxSteps();
		interpolation_steps_ = dialog.getInterpolationSteps();

		vector_grid_ = item_to_gradients_[context_item_];
    		bool use_atoms = false; //????
// 		bool use_atoms = true;
		Size nr_lines = 100;
		RegularData3D* potential_grid = (*get3DGrids().begin()).first;
		
		AtomContainer* ac = (AtomContainer*) item_to_composite_[context_item_];
		if (use_atoms && ac == 0) 
		{
			setStatusbarText("No System available for this gradient grid, aborting field line calculation!", true);
			return;
		}

		if (use_atoms)
		{
			// seed points from spheres around atoms:
			vector<Vector3> start_diffs = createSphere(icosaeder_steps_ - 1);
			AtomIterator ait = ac->beginAtom();
			for (; +ait; ++ait)
			{
				for (Position p = 0; p < start_diffs.size(); p++)
				{
					const Vector3& point = ait->getPosition();
					const Vector3& diff = start_diffs[p];
					createFieldLine_(point + diff * atom_distance_, *rep);
				}
			}
		}
		else
		{
			// method from "Fast Display of Illuminated Field Lines"
			// from Stalling, Zöckler, Hege; 1997
			// Monte Carlo Approach in relation to potential strenght at the individual points
			Vector3 origin = vector_grid_->getOrigin();
			Vector3 dimension = vector_grid_->getDimension();
			VectorGrid::IndexType size = vector_grid_->getSize();

			Size sx = (Size)(size.x / 3.0 + 1);
			Size sy = (Size)(size.y / 3.0 + 1);
			Size sz = (Size)(size.z / 3.0 + 1);
			RegularData3D::IndexType st(sx, sy, sz);

			Vector3 diff = Vector3(0.1, 0.1, 0.1);
			RegularData3D grid(st, origin - diff, vector_grid_->getDimension() + diff * 2);
			const Size new_grid_size = sx * sy * sz;
			for (Position p = 0; p < new_grid_size; p++)
			{
				grid[p] = 0;
			}
			
			const vector<float>& values =  potential_grid->getData();
			for (Position p = 0; p < values.size(); p++)
			{
				grid.getClosestValue((potential_grid->getCoordinates(p))) += values[p];
			}

			const vector<float>& values2 =  grid.getData();
			vector<float> normalized_values;

			calculateHistogramEqualization(values2, normalized_values);

			float current = 0;
			for (Position p = 0; p < normalized_values.size(); p++)
			{
				current += normalized_values[p];
				normalized_values[p] = current;
			}

			const float spacing = vector_grid_->getSpacing().x / 2.0;
			const float half_spacing = spacing / 2.0;
			Vector3 s2 = grid.getSpacing() / 2.0;
			
			for (Position p = 0; p < nr_lines; p++)
			{
				float x = drand48();
				x *= current;
				for (Position i = 0; i < normalized_values.size(); i++)
				{
					if (normalized_values[i] > x)
					{
						Vector3 point = grid.getCoordinates(i);
						point.x += drand48() * spacing - half_spacing;
						point.y += drand48() * spacing - half_spacing;
						point.z += drand48() * spacing - half_spacing;
						try
						{
							vector_grid_->getClosestValue(point);
							createFieldLine_(point, *rep);
							Point* p = new Point();
							p->setVertex(point);
							rep->insert(*p);
							break;
						}
						catch(...)
						{
						}
					}
					else if (p == normalized_values.size() -1)
					{
					}
				} // search point
			} // all lines
		}

		getMainControl()->insert(*rep);
		getMainControl()->update(*rep);
	}

	void DatasetControl::createFieldLine_(const Vector3& point, Representation& rep)
	{
		IlluminatedLine* line = new IlluminatedLine;
		vector<Vector3>& points = line->vertices;

		for (int backwards = 0; backwards < 1; backwards++)
		{
			calculateLinePoints_(point, points, (backwards==0) ? 1. : -1.);

			const Size nrp = points.size();
			if (points.size() < 2)
			{
				delete line;
				return;
			}

			line->tangents.resize(nrp);

			for (Position v = 0; v < nrp - 1; v++)
			{
				(*line).tangents[v] = points[v+1] - points[v];
			}
			(*line).tangents[nrp -1] = (*line).tangents[nrp -2];

			(*line).colors.push_back(ColorRGBA(0.,0.,1.));
		}

		rep.insert(*line);
	}

	void DatasetControl::createVectorGrid()
		throw()
	{
		RegularData3D& potential = *item_to_grid3_[context_item_];
		System* system = 0;
		if (item_to_composite_.has(context_item_) && 
				item_to_composite_[context_item_] != 0)
		{
			system = dynamic_cast<System*>(item_to_composite_[context_item_]);
		}

		RegularData3D::CoordinateType spacing = potential.getSpacing();
		RegularData3D::IndexType         size = potential.getSize();

		VectorGrid* grid_ptr = new VectorGrid(potential.getOrigin(), potential.getDimension(), spacing);
		VectorGrid& gradient_grid = *grid_ptr;

		TRegularData3D<Vector3>::IndexType index;

		for (index.x = 0; index.x < size.x; index.x++)
		{
			for (index.y = 0; index.y < size.y; index.y++)
			{
				for (index.z = 0; index.z < size.z; index.z++)
				{
					RegularData3D::IndexType next, last;

					next.x = index.x; next.y = index.y; next.z = index.z;
					last = next;

					float factor = 1.;
					if (index.x == 0)
					{
						// onlx forward difference possible
						next.x++;
					} 
					else if (index.x == size.x-1) {
						// onlx backward difference possible
						last.x--;
					}	
					else {
						// mid point formula
						next.x++;
						last.x--;
						factor = 0.5;
					}

					gradient_grid[index].x = factor * spacing.x * (potential[next] - potential[last]);
					factor = 1.; next.x = index.x; next.y = index.y; next.z = index.z; last = next;

					if (index.y == 0)
					{
						// only forward difference possible
						next.y++;
					} 
					else if (index.y == size.y-1) {
						// only backward difference possible
						last.y--;
					}	
					else {
						// mid point formula
						next.y++;
						last.y--;
						factor = 0.5;
					}

					gradient_grid[index].y = factor * spacing.y * (potential[next] - potential[last]);
					factor = 1.; next.x = index.x; next.y = index.y; next.z = index.z; last = next;

					if (index.z == 0)
					{
						// only forward difference possible
						next.z++;
					} 
					else if (index.z == size.z-1) {
						// only backward difference possible
						last.z--;
					}	
					else {
						// mid point formula
						next.z++;
						last.z--;
						factor = 0.5;
					}

					gradient_grid[index].z = factor * spacing.z * (potential[next] - potential[last]);
				}
			}
		}

		QTreeWidgetItem* item = createListViewItem_(system, "Vector Grid", "Vector Grid");
		item_to_gradients_[item] = grid_ptr;
		item_to_composite_[item] = system;
	}

	void DatasetControl::addVectorGrid()
		throw()
	{
		List<Composite*>& sel = getMainControl()->getMolecularControlSelection();
		if (sel.size() == 0) return;
		System* system = dynamic_cast<System*>(*sel.begin());
		if (!system) return;

		String filename = chooseGridFileForOpen_();
		if (filename == "") return;

		VectorGrid* dat = new VectorGrid;
		(*dat).binaryRead(filename);

		QTreeWidgetItem* item = createListViewItem_(system, filename, "Vector Grid");
		item_to_gradients_[item] = dat;
		item_to_composite_[item] = system;
	}

	/** Compute a field line using a Runge-Kutta of fourth order with adaptive step
	 *  size control. factor can be used to iterate _against_ the gradient, i.e. backwards in time.
	 */
	inline void DatasetControl::calculateLinePoints_(Vector3 point, vector<Vector3>& points, float factor)
	{
		points.clear();

		VectorGrid& gradient_grid = *vector_grid_;

		TRegularData3D<Vector3>::CoordinateType spacing = gradient_grid.getSpacing();
		TRegularData3D<Vector3>::IndexType         size = gradient_grid.getSize();
		Vector3 k1, k2, k3, k4, k5, k6;
		Vector3 		p2, p3, p4, p5, p6;
		Vector3 error_estimate_vector;
		Vector3 scaling;
		float error_estimate = 0.;

		float min_spacing = std::min(std::min(spacing.x, spacing.y), spacing.z);
		float rho = 0.9; // chose sensible values
		float lower_limit = min_spacing * 0.0001;
		float h = min_spacing;// * 0.1;

		// use interpolation_steps interpolation points
		std::vector<Vector3> interpolated_values(interpolation_steps_);

		Vector3 rk_estimate;

		Vector3 grad_current = gradient_grid(point) * factor;
		Vector3 grad_old     = grad_current;

		// Runge - Kutta of order 4 with adaptive step size and
		// error control as described in Schwarz: "Numerische Mathematik"
		// with step size control taken from Numerical Recipes
		for (Size i = 0; i < max_steps_; i++)
		{
			// compute scaling values for the step size computation (see Numerical Recipes)
			scaling.x = fabs(point.x) + fabs(grad_current.x*h) + 1e-30;
			scaling.y = fabs(point.y) + fabs(grad_current.y*h) + 1e-30;
			scaling.z = fabs(point.z) + fabs(grad_current.z*h) + 1e-30;

			// repeat the Runge-Kutta until the step size is either accepted or completely rejected
			bool accepted = false;

			while (!accepted)
			{
				try
				{
					k1 = h*grad_current;

					p2 = point + k1*2./9.;
					k2 = h*gradient_grid.getInterpolatedValue(p2)*factor;

					p3 = point + k1*1./12. + k2*1./4.;
					k3 = h*gradient_grid.getInterpolatedValue(p3)*factor;

					p4 = point + k1*69./128. - k2*243./128. + k3*135./64.;
					k4 = h*gradient_grid.getInterpolatedValue(p4)*factor;

					p5 = point - k1*17./12. + k2*27./4. - k3*27./5.+ k4*16./15.;
					k5 = h*gradient_grid.getInterpolatedValue(p5)*factor;

					p6 = point + k1*65./432. - k2*5./16. + k3 * 13./16. + k4*4./27. + k5*5./144.;
					k6 = h*gradient_grid.getInterpolatedValue(p6)*factor;

					// let's see if the steps have become that small that we don't proceed at all...
					if (p6 == point)
					{
						return;
					}

					rk_estimate = (k1 / 9. + k3 * 9./20. + k4*16./45. + k5 / 12.);

					error_estimate_vector = (-k1*2. + k3*9. - k4*64. - k5*15. + k6*72.) / 300.;
					error_estimate = std::max(fabs(error_estimate_vector.x/scaling.x), 
												 	 std::max(fabs(error_estimate_vector.y/scaling.y), 
														 				fabs(error_estimate_vector.z/scaling.z)));

					error_estimate /= tolerance_;

					if (error_estimate > 1.0)
					{
						// update h using the error estimate
						double h_new = h * rho * pow(error_estimate, -0.25);

						h = (h >= 0) ? std::max(h_new, 0.1*h) : std::min(h_new, 0.1*h);
					}
					else 
					{
						accepted = true;
					}
				} catch (Exception::OutOfGrid e)
				{
					h /= 2.;
					// horrible heuristic... :-)
					if (fabs(h) < 1e-10)
					{
						return;
					}
				}
			}

			// compute a step size for the next step (the magic numbers are taken from Numerical Recipes)
			if (error_estimate > 1.89e-4) h = rho * h * pow(error_estimate, -0.2);
			else 												  h = 5.*h;

			grad_old = grad_current;
			grad_current = gradient_grid(point+rk_estimate)*factor;

			cubicInterpolation(point, point+rk_estimate, grad_old, grad_current, interpolated_values);	
			points.push_back(point);

			for (Position p = 0; p < interpolated_values.size(); p++)
			{
				points.push_back(interpolated_values[p]);
			}

			point += rk_estimate;

			if (rk_estimate.getLength() < lower_limit) break;
		}
	}
	
	List<DatasetControl::VectorGrid*> DatasetControl::getVectorGrids()
		throw()
	{
		List<VectorGrid*> grids;
		HashMap<QTreeWidgetItem*, VectorGrid*>::Iterator it = item_to_gradients_.begin();
		for (; +it; ++it)
		{
			grids.push_back(it->second);
		}
		return grids;
	}


	} // namespace VIEW
} // namespace BALL
