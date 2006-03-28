// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: datasetControl.C,v 1.46.2.7 2006/03/28 13:55:03 amoll Exp $
//

#include <BALL/VIEW/WIDGETS/datasetControl.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/VIEW/DATATYPE/colorMap.h>
#include <BALL/VIEW/PRIMITIVES/illuminatedLine.h>

#include <BALL/VIEW/DIALOGS/snapShotVisualisation.h>
#include <BALL/VIEW/DIALOGS/contourSurfaceDialog.h>

//   #include <BALL/VIEW/WIDGETS/regularData1DWidget.h>
//   #include <BALL/VIEW/WIDGETS/regularData2DWidget.h>

#include <BALL/VIEW/PRIMITIVES/mesh.h>
#include <BALL/VIEW/PRIMITIVES/line.h>

#include <BALL/FORMAT/DCDFile.h>
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

			open_gradient_id_ = insertMenuEntry(MainControl::FILE_OPEN, "Gradient Grid", this, SLOT(addGradientGrid()));
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
				GradientGrid* grid = item_to_gradients_[&item];

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
					insertContextMenuEntry_("Create gradient grid", SLOT(createGradientGrid()));
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

			System* system = 0;
			List<Composite*>& sel = getMainControl()->getMolecularControlSelection();
			if (sel.size() != 0) system = dynamic_cast<System*>(*sel.begin());

			RegularData3D* dat = new RegularData3D;
			(*dat).binaryRead(filename);
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
		}

		void DatasetControl::insertGrid_(RegularData2D* data, System* system, const String& name)
			throw()
		{
			QTreeWidgetItem* item = createListViewItem_(system, name, "2D Grid");
			item_to_grid2_[item] = data;
		}

		void DatasetControl::insertGrid_(RegularData3D* data, System* system, const String& name)
			throw()
		{
			QTreeWidgetItem* item = createListViewItem_(system, name, "3D Grid");
			item_to_grid3_[item] = data;
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
		const GradientGrid& grid = *item_to_gradients_[context_item_];

		AtomContainer* ac = (AtomContainer*) item_to_composite_[context_item_];
		if (ac == 0) 
		{
			setStatusbarText("No System available for this gradient grid, aborting field line calculation!", true);
			return;
		}

		Representation* rep = new Representation;
 		rep->setTransparency(90);
		ColorMap table;
		ColorRGBA colors[3];
		colors[0] = ColorRGBA(1.0, 0.0, 0.0, 1.0);
		colors[1] = ColorRGBA(0.5, 0.0, 0.5, 0.2);
		colors[2] = ColorRGBA(0.0, 0.0, 1.0, 1.0);
		table.setRange(-1.5, 1.5);
		table.setBaseColors(colors,3);
		table.setNumberOfColors(100);
		table.setAlphaBlending(true);
		table.createMap();

		AtomIterator ait = ac->beginAtom();
		for (; +ait; ++ait)
		{
			for (Position p = 0; p < 50; p++)
			{
				Vector3 point = ait->getPosition();
				Vector3 diff(drand48(), drand48(), drand48());
				if (drand48() > 0.5) diff.x *= -1;
				if (drand48() > 0.5) diff.y *= -1;
				if (drand48() > 0.5) diff.z *= -1;
				diff.normalize();
				diff *= 0.4;
				point += diff;

				IlluminatedLine* line = new IlluminatedLine;
				vector<Vector3>& points = line->vertices;
				calculateLinePoints_(grid, point, points);

				const Size nrp = points.size();
				line->tangents.resize(nrp);
				line->colors.resize(nrp);

				for (Position v = 0; v < nrp - 1; v++)
				{
					(*line).tangents[v] = points[v+1] - points[v];
				}
				(*line).tangents[nrp -1] = (*line).tangents[nrp -2];

				for (Position v = 0; v < nrp; v++)
				{
	 				(*line).colors[v] = table.map(grid(points[v]).getLength());
				}

				rep->insert(*line);
			}
		}

		getMainControl()->insert(*rep);
		getMainControl()->update(*rep);
	}

	void DatasetControl::createGradientGrid()
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

		GradientGrid* grid_ptr = new GradientGrid(potential.getOrigin(), potential.getDimension(), spacing);
		GradientGrid& gradient_grid = *grid_ptr;

		TRegularData3D<Vector3>::IndexType index;

		for (index.x = 0; index.x < size.x; index.x++)
		{
			for (index.y = 0; index.y < size.y; index.y++)
			{
				for (index.z = 0; index.z < size.z; index.z++)
				{
					// This is a stupid way to do this... the whole gradient computation should
					// be moved to this loop
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

		QTreeWidgetItem* item = createListViewItem_(system, "Gradient Grid", "Gradient Grid");
		item_to_gradients_[item] = grid_ptr;
		item_to_composite_[item] = system;
	}

	void DatasetControl::addGradientGrid()
		throw()
	{
		List<Composite*>& sel = getMainControl()->getMolecularControlSelection();
		if (sel.size() == 0) return;
		System* system = dynamic_cast<System*>(*sel.begin());
		if (!system) return;

		String filename = chooseGridFileForOpen_();
		if (filename == "") return;

		GradientGrid* dat = new GradientGrid;
		(*dat).binaryRead(filename);

		QTreeWidgetItem* item = createListViewItem_(system, filename, "Gradient Grid");
		item_to_gradients_[item] = dat;
		item_to_composite_[item] = system;
	}

	inline void DatasetControl::calculateLinePoints_(const TRegularData3D<Vector3>& gradient_grid, 
																									 Vector3 point, vector<Vector3>& points)
	{
		points.clear();

		float h = 0.1;
		RegularData3D::CoordinateType spacing = gradient_grid.getSpacing();
		Vector3 k1, k2, k3, k4;
		float error_estimate;

		float min_spacing = std::min(std::min(spacing.x, spacing.y), spacing.z);
		float rho = 0.9; // chose sensible values
		float lower_limit = min_spacing * 0.01;
		float tolerance = 1.0;
		float h_max = min_spacing * 2;
		
		// use 5 interpolation points
		std::vector<Vector3> interpolated_values(5);

		Vector3 rk_estimate;

		Vector3 grad_current = gradient_grid(point);
		Vector3 grad_old     = grad_current;

		// Runge - Kutta of order 4 with adaptive step size and
		// error control
		for (int i=0; i<500; i++)
		{
			k1 = h*grad_current;
			k2 = h*gradient_grid(point+k1*0.5);
			k3 = h*gradient_grid(point+k2*0.5);
			k4 = h*gradient_grid(point+k3);

			rk_estimate = (k1+k2*2+k3*2+k4) * 1./6.;
			grad_old = grad_current;
			grad_current = gradient_grid(point+rk_estimate);
			
			cubicInterpolation(point, point+rk_estimate, grad_old, grad_current, interpolated_values);	

			points.push_back(point);

			for (Position p = 0; p < interpolated_values.size(); p++)
			{
				points.push_back(interpolated_values[p]);
			}

			point += rk_estimate;

			error_estimate = ((k4-h*gradient_grid(point)) * 1./6.).getLength();

			// update h using the error estimate
			float h_new = h * pow(rho*tolerance / error_estimate, 1./5.);
			if (error_estimate > tolerance)
				h = h_new;
			else
				h = std::min(h_new, h_max);

			if ((h < lower_limit) || (rk_estimate.getLength() < lower_limit))
			{
				break;
			}
		} // iteration over points
	}


	List<DatasetControl::GradientGrid*> DatasetControl::getGradientGrids()
		throw()
	{
		List<GradientGrid*> grids;
		HashMap<QTreeWidgetItem*, GradientGrid*>::Iterator it = item_to_gradients_.begin();
		for (; +it; ++it)
		{
			grids.push_back(it->second);
		}
		return grids;
	}


	} // namespace VIEW
} // namespace BALL
