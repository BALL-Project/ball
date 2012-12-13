// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/DATATYPE/standardDatasets.h>
#include <BALL/VIEW/WIDGETS/datasetControl.h>
#include <BALL/VIEW/WIDGETS/scene.h>
#include <BALL/VIEW/KERNEL/common.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/VIEW/PRIMITIVES/mesh.h>
#include <BALL/VIEW/PRIMITIVES/quadMesh.h>

#include <BALL/VIEW/DIALOGS/snapShotVisualisation.h>
#include <BALL/VIEW/DIALOGS/contourSurfaceDialog.h>
#include <BALL/VIEW/DIALOGS/fieldLinesDialog.h>
#include <BALL/VIEW/DIALOGS/gridVisualizationDialog.h>

#include <BALL/FORMAT/trajectoryFile.h>
#include <BALL/FORMAT/trajectoryFileFactory.h>
#include <BALL/FORMAT/DSN6File.h>
#include <BALL/FORMAT/CCP4File.h>
#include <BALL/FORMAT/amiraMeshFile.h>
#include <BALL/MOLMEC/COMMON/snapShotManager.h>
#include <BALL/DATATYPE/contourSurface.h>
#include <BALL/DOCKING/COMMON/dockResult.h>
#include <BALL/DATATYPE/regularData3D.h>

#include <QtGui/QFileDialog>

using namespace std;

namespace BALL
{
	/////////////////////////////////////////////
	// Specialization for the default float type of TRegularData3D
	// possibly not the best location

	template<>
	void TRegularData3D<float>::binaryWriteRaw(const String& filename) const
	{	
		String coreName = filename;
		String::size_type dotIndex = filename.find_last_of(".");
		if (dotIndex != string::npos)
		{
			coreName = filename.getSubstring(0, dotIndex);				
		}
		
		String headerFN = coreName + ".vrt.vh";
		Log.info() << "Creating grid header '" << headerFN << "'" << std::endl;
		
		std::ofstream header(headerFN.c_str());
		if(!header.is_open()) 
		{
			throw Exception::FileNotFound(__FILE__, __LINE__, filename+".header");
		}	

		// dimensions
		header << size_.x << " " << size_.y << " " << size_.z << " ";
		// bit depth, for float datasets used to determine TFF table resolution
		header << 8 << " ";
		// voxel sizes
		header << spacing_.x << " " << spacing_.y << " " << spacing_.z << " ";		

		// unused header information
		//header << data_.size() << " ";
		//header << origin_.x << " " << origin_.y << " " << origin_.z << " ";
		//header << dimension_.x << " " << dimension_.y << " " << dimension_.z << " ";		

		header.close();

		Log.info() << "Header written" << std::endl;

		String rawFileFN = coreName + ".vrt";
		Log.info() << "Creating raw file '" << rawFileFN << "'" << std::endl;

		std::ofstream rawFile(rawFileFN.c_str(), std::ios::out | std::ios::binary);
		if(!rawFile.is_open())
		{
			throw Exception::FileNotFound(__FILE__, __LINE__, filename+".raw");
		}
		
		for (unsigned int i = 0; i < data_.size(); i++)
		{
			rawFile.write((const char*)&data_[i], sizeof(float));
		}
		
		rawFile.close();

		Log.info() << "Raw data written" << std::endl;		
		Log.info() << "File saved succesfully" << std::endl;
	}	

	/////////////////////////////////////////////

	namespace VIEW
	{

		// move this somewhere else ????
		Representation* createSphere(RegularData3D& grid)
		{
			int num_lines_theta = 50;
			int num_lines_phi   = 50;
			float theta_start   = 20./90. * M_PI;

			Vector3 origin = grid.getOrigin();

			Vector3 vw(1,0,0); 
			Scene* scene = Scene::getInstance(0);
			if (scene) vw = -scene->getStage()->getCamera().getViewVector();

			// rotate the z-vector onto the look_at - vector
			Matrix4x4 rotation;
			vw.normalize();
			
			Vector3 axis = vw % Vector3(0.,0.,1.);

			if (axis.getLength() > 1e-6)
			{
					rotation.setRotation(Angle(-acos(vw.z)), axis);
			}
			else
			{
					rotation.setIdentity();
			}

			Vector3 center = origin + grid.getDimension() * 0.5;
			RegularData3D::IndexType size = grid.getSize();
			Vector3 x = grid.getCoordinates(RegularData3D::IndexType(size.x - 1, 0, 0)) - origin;
			Vector3 y = grid.getCoordinates(RegularData3D::IndexType(0, size.y - 1, 0)) - origin;
			Vector3 z = grid.getCoordinates(RegularData3D::IndexType(0, 0, size.z - 1)) - origin;
			float min = BALL_MIN3(x.getLength(), y.getLength(), z.getLength());
			min /= 2.;
			min -= 0.01;

			if (min < 0) return 0;

			float radius = min;
			float delta_phi     = 2.*M_PI/(num_lines_phi-1);
			float delta_theta = M_PI/(num_lines_theta-1);

			int theta_start_index = (int)(theta_start / M_PI * num_lines_theta);
			vector<vector<Vector3> > points_on_sphere(num_lines_theta-theta_start_index);

			for (Index k = theta_start_index; k < num_lines_theta; k++)
			{
				for (Index i = 0; i < num_lines_phi; i++)
				{
					float phi   = i*delta_phi;
					float theta = k*delta_theta;

					Vector3 current_point;
					current_point.x = radius*cos(phi)*sin(theta);
					current_point.y = radius*sin(phi)*sin(theta);
					current_point.z = radius*cos(theta);
					
					Vector4 transformed_point(current_point.x, current_point.y, current_point.z, 1.);
					transformed_point = rotation*transformed_point;
					current_point.x = transformed_point.x;
					current_point.y = transformed_point.y;
					current_point.z = transformed_point.z;

					current_point += center;

					points_on_sphere[k-theta_start_index].push_back(current_point);        
				}
			}

			QuadMesh* qm = new QuadMesh();

			int num_theta = points_on_sphere.size();
			int num_phi   = points_on_sphere[0].size();

			for (int current_theta = 0; current_theta < num_theta; current_theta++)
			{
				for (int current_phi = 0; current_phi < num_phi; current_phi++)
				{
					Vector3& vc = points_on_sphere[current_theta][current_phi];
					qm->vertex.push_back(vc);
					Vector3 vx = -(vc - center);
					vx.normalize();
					qm->normal.push_back(vx);

					if (current_theta != num_theta-1)
					{
						Index next_theta = current_theta+1;
						Index next_phi = current_phi+1;
						if (current_phi == num_phi-1) next_phi = 0;

						qm->quad.push_back(QuadMesh::Quadruple(current_theta*num_phi + current_phi,
																										current_theta*num_phi + next_phi,
																										next_theta*num_phi    + next_phi,
																										next_theta*num_phi    + current_phi));
					}
				}
			}

			qm->colors.clear();
			qm->colors.push_back(ColorRGBA(0,0,1.));

			Representation* rep = new Representation();
			rep->insert(*qm);
			return rep;
		}


		// move this somewhere else ????
		VectorGrid* createVectorGrid(RegularData3D& potential)
		{
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

			return grid_ptr;
		}


		/////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////

		String RegularData3DController::type = "3D Grid";

		RegularData3DController::RegularData3DController()
			: DatasetController(),
				dialog_(0),
				grid_dialog_(0)
		{
			type_ = type;
			file_formats_.push_back("3dg");
			file_formats_.push_back("ccp4");
			file_formats_.push_back("omap");
			file_formats_.push_back("map");
			file_formats_.push_back("dsn6");
			file_formats_.push_back("am");
			setIdentifier("RegularData3DController");
			registerThis();
		}

		RegularData3DController::RegularData3DController(RegularData3DController& dc)
			: DatasetController(dc),
				dialog_(0)
		{
			setIdentifier("RegularData3DController");
			registerThis();
		}

		RegularData3DController::~RegularData3DController()
		{
		}

		bool RegularData3DController::write()
		{
			Dataset* set = getSelectedDataset();
			if (set == 0) return 0;

			QString file = QFileDialog::getSaveFileName(0,
			QString("Save as ") + type_.c_str(),
			getDatasetControl()->getWorkingDir().c_str(),
			QString("( *.3dg)"));

			if (file == QString::null) return false;

			vector<String> fields;
			String s(ascii(file));
			s.split(fields, ".");
			if (fields.size() == 0)
			{
				BALLVIEW_DEBUG
				return false;
			}

			return write(set, fields[fields.size() - 1], ascii(file));
		}

		bool RegularData3DController::write(Dataset* set, String /*filetype*/, String filename)
		{
			if (getData(set) == 0) return false;
			getData(set)->binaryWrite(filename);
			return true;
		}

		Dataset* RegularData3DController::open(String filetype, String filename)
		{
			RegularData3D* d3 = new RegularData3D;
			RegularData3DDataset* set = 0;
			bool read_success = false;
			String type = type_;
			String maptype = "3DG";
			if(filetype == "ccp4" || filetype == "map")
			{
				CCP4File* ccp4file = new CCP4File(filename);
				read_success = ccp4file->read(*d3);
				ccp4file->close();
				//type = "Electron Density";
				type = RegularData3DController::type;
				maptype = "CCP4";
			}
			else if(filetype == "omap" || filetype == "dsn6")
			{
				DSN6File* dsn6file = new DSN6File(filename);
				read_success = dsn6file->read(*d3);
				dsn6file->close();
				//type = "Electron Density";
				type = RegularData3DController::type;
				maptype = "DSN6";
			}
			else if(filetype == "am")
			{
				AmiraMeshFile* amirafile = new AmiraMeshFile(filename);
				read_success = amirafile->read(*d3);
				amirafile->close();
				type = RegularData3DController::type;
				maptype = "AMIRA";
			}
			else
			{
				read_success = true;
				(*d3).binaryRead(filename);
			}


			if (read_success)
			{
				set = new RegularData3DDataset;
				set->setData(d3);
				String mapname = getNameFromFileName_(filename);
				set->setName(mapname);
				set->setType(type);
				float dens_mean = d3->calculateMean();
				float dens_stddev = d3->calculateSD();
				setStatusbarText(tr("Successfully read ") + type.c_str() + tr(" from ") + maptype.c_str() + tr(" file: ") + filename.c_str(), true);
				Log.info() << mapname << " Map Density Mean: " << dens_mean << endl;
				Log.info() << mapname << " Map Density Sigma: " << dens_stddev << endl;
			}
			else
			{
				setStatusbarText("Error reading " + type + " from " + maptype + "file: " + filename, true);
			}
			return set;
		}

		RegularData3D* RegularData3DController::getData(Dataset* set)
		{
			RegularData3DDataset* dset = dynamic_cast<RegularData3DDataset*>(set);
			if (dset == 0) return 0;
			return dset->getData();
		}

		void RegularData3DController::deleteDataset_(Dataset* set)
		{
			RegularData3D* data = getData(set);
			if (data == 0) return;
			delete data;
		}

		QMenu* RegularData3DController::buildContextMenu(QTreeWidgetItem* item)
		{
			QMenu* menu = DatasetController::buildContextMenu(item);
			if (menu == 0) return 0;
			
			// In case the dataset is a non-orthogonal RegData3D, for now only allow calculation of contour surfaces
			bool is_ortho = true;
			RegularData3D* data = getData(getSelectedDataset());
			if (data != 0)
			{
				is_ortho = data->isOrthogonal();
			}
			menu->addAction("Render Grid", this, SLOT(visualizeGrid()))->setEnabled(is_ortho);
			menu->addAction("Create Sphere", this, SLOT(createSphere()))->setEnabled(is_ortho);
			menu->addAction("Render Contour Surface", this, SLOT(computeIsoContourSurface()));
			menu->addAction("Resize for Rendering", this, SLOT(resizeGrid()))->setEnabled(is_ortho);
			menu->addSeparator();
			menu->addAction("Create normalized Grid", this, SLOT(createHistogramGrid()))->setEnabled(is_ortho);
			menu->addAction("Create Gradient Grid", this, SLOT(createVectorGrid()))->setEnabled(is_ortho);
			
			// NOTE: raytraceable grids are deferred until 1.4/2.0
			// menu->addAction("Create raytraceable Grid", this, SLOT(createRaytraceableGrid()));

			return menu;
		}

		bool RegularData3DController::createMenuEntries()
		{
			if (!DatasetController::createMenuEntries()) return false;
			
			String description = "Shortcut|Tools|Grid|Calculate_normalized_grid";
			insertMenuEntry_(MainControl::TOOLS_GRID, tr("Calculate normalized Grid"), SLOT(createHistogramGrid()), 
			                 description, QKeySequence(), tr("Create a new grid with a histogram equalization"),
											 UIOperationMode::MODE_ADVANCED);

			description = "Shortcut|Tools|Grid|Resize_for_Rendering";
			QAction* action = insertMenuEntry_(MainControl::TOOLS_GRID, tr("Resize for Rendering"), SLOT(resizeGrid()), 
			                                   description, QKeySequence(), tr("Resize a grid for rendering"), 
																				 UIOperationMode::MODE_ADVANCED);
			getDatasetControl()->setMenuHelp(action, "datasetControl.html#volume");

			getDatasetControl()->getMainControl()->insertPopupMenuSeparator(MainControl::TOOLS_GRID, UIOperationMode::MODE_ADVANCED);

			// visualizations:
			description = "Shortcut|Tools|Grid|Visualize_3D_Grid";
			action = insertMenuEntry_(MainControl::TOOLS_GRID, tr("Visualize 3D Grid"), SLOT(visualizeGrid()), 
			                          description, QKeySequence(), tr("Visualize a grid"),
																UIOperationMode::MODE_ADVANCED);
			getDatasetControl()->setMenuHelp(action, "datasetControl.html#volume");

			description = "Shortcut|Tools|Grid|Render_Contour_Surface";
			action = insertMenuEntry_(MainControl::TOOLS_GRID, "Render Contour S&urface", SLOT(computeIsoContourSurface()), 
					                      description, QKeySequence("Ctrl+U"), 
																tr("Calculate an isocontour surface from a 3D grid. The grid has to be loaded in the DatasetControl."),
																UIOperationMode::MODE_ADVANCED);
			getDatasetControl()->setMenuHelp(action, "datasetControl.html#isocontour_surfaces");

			// TODO: add to Documentation (via setMenuHelp())
			description = "Shortcut|Tools|Grid|Create_a_Sphere";
			insertMenuEntry_(MainControl::TOOLS_GRID, "Create a sphere", SLOT(createSphere()), 
			                 description, QKeySequence(), tr("Create a sphere for the grid, e.g. for colorizing"),
											 UIOperationMode::MODE_ADVANCED);

			getDatasetControl()->getMainControl()->insertPopupMenuSeparator(MainControl::TOOLS_GRID, UIOperationMode::MODE_ADVANCED);

			return true;
		}


		bool RegularData3DController::createVectorGrid()
		{
			Dataset* old_set = getSelectedDataset();
			RegularData3D* grid3d = getData(old_set);
			if (grid3d == 0) 
			{
				BALLVIEW_DEBUG
				return false;
			}

			VectorGrid* vgrid = VIEW::createVectorGrid(*grid3d);

			VectorGridDataset* new_set = new VectorGridDataset();
			new_set->setComposite(old_set->getComposite());
			new_set->setName(old_set->getName());
			new_set->setType(VectorGridController::type);
			new_set->setData(vgrid);
			DatasetMessage* msg = new DatasetMessage(new_set, DatasetMessage::ADD);
			getDatasetControl()->getMainControl()->sendMessage(*msg);
			return true;
		}

		bool RegularData3DController::createRaytraceableGrid()
		{
			Dataset* old_set = getSelectedDataset();
			RegularData3D* grid3d = getData(old_set);

			RaytraceableGrid* new_grid = new RaytraceableGrid(grid3d);
			RaytraceableGridDataset* new_set = new RaytraceableGridDataset();
			new_set->setComposite(old_set->getComposite());
			new_set->setType(RaytraceableGridController::type);
			new_set->setName(old_set->getName());
			new_set->setData(new_grid);
			DatasetMessage* msg = new DatasetMessage(new_set, DatasetMessage::ADD);
			getDatasetControl()->getMainControl()->sendMessage(*msg);
			return true;
		}

		void RegularData3DController::visualizeGrid()
		{
			Dataset* data = (Dataset*)getSelectedDataset();
			if (getData(data) == 0)
			{
				BALLVIEW_DEBUG
				return;
			}

			if (grid_dialog_ == 0) 
			{
				grid_dialog_ = new GridVisualizationDialog(getDatasetControl());
			}
			grid_dialog_->setController(this);
			grid_dialog_->setGrid(getData(data));
			grid_dialog_->exec();
		}

		
		void RegularData3DController::createSphere()
		{
			Dataset* data = (Dataset*)getSelectedDataset();
			if (getData(data) == 0)
			{
				BALLVIEW_DEBUG
				return;
			}

			RegularData3D* grid = getData(data);
			Representation* rep = VIEW::createSphere(*grid);
			getDatasetControl()->getMainControl()->insert(*rep);
			getDatasetControl()->getMainControl()->update(*rep);
		}

		void RegularData3DController::computeIsoContourSurface()
		{
			// execute the surface dialog and abort if cancel is clicked
			if (dialog_ == 0) 
			{
				dialog_ = new ContourSurfaceDialog(getDatasetControl(), "ContourSurfaceDialog");
				dialog_->setController(this);
				getDatasetControl()->registerForHelpSystem(dialog_, "datasetControl.html#isocontour_surfaces");
			}
			dialog_->setGrid(getSelectedDataset());
			if (!dialog_->exec()) return;

			Dataset* data = dialog_->getGrid();
			if (data == 0) return;

			if (!computeIsoContourSurface(
				*data, dialog_->getColor(), dialog_->getThreshold()))
			{
				dialog_->exec();
			}
		}

		bool RegularData3DController::computeIsoContourSurface(Dataset& data, 
																													const ColorRGBA& color, float value) 
		{
			RegularData3D* grid = getData(&data);
			if (grid == 0) return false;
			// Create a new contour surface.
			ContourSurface cs(*grid, value);

			if (cs.vertex.size() == 0)
			{
				setStatusbarText(tr("Could not calculate ContourSurface, no grid points found for threshold!"), true);
				return false;
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
			mesh->colors.push_back(color);

			//////////////////////////////////////////////
			// Create a new representation containing the contour surface.
			Representation* rep = new Representation();
			rep->insert(*mesh);
			rep->setModelType(MODEL_CONTOUR_SURFACE); 

			list<const Composite*> composites;
			if (data.getComposite() != 0) composites.push_back(data.getComposite());
			rep->setComposites(composites);

			// Make sure BALLView knows about the new representation.
			getMainControl()->insert(*rep);
			getMainControl()->getRepresentationManager().focusRepresentation(*rep);
			getMainControl()->update(*rep);

			return true;
		}


		Size RegularData3DController::getNextPowerOfTwo_(Size in) const
		{
			Size test = 2;
			while (test < in)
			{
				test = test * 2;
			}

			return test;
		}

		bool RegularData3DController::isGridSizePowerOfTwo(const RegularData3D& grid) const
		{
			RegularData3D::IndexType size = grid.getSize();
			RegularData3D::IndexType new_size;
			new_size.x = getNextPowerOfTwo_(size.x);
			new_size.y = getNextPowerOfTwo_(size.y);
			new_size.z = getNextPowerOfTwo_(size.z);
			return (new_size.x == size.x &&
							new_size.y == size.y &&
							new_size.z == size.z);
		}

		RegularData3D* RegularData3DController::resizeGrid(const RegularData3D& grid) 
		{
			RegularData3D::IndexType size = grid.getSize();
			RegularData3D::IndexType new_size;
			new_size.x = getNextPowerOfTwo_(size.x);
			new_size.y = getNextPowerOfTwo_(size.y);
			new_size.z = getNextPowerOfTwo_(size.z);
			if (new_size.x == size.x &&
					new_size.y == size.y &&
					new_size.z == size.z)
			{
				setStatusbarText(tr("Grid does not need to be resized!"), true);
				return 0;
			}

			// make sure new grid is a little bit smaller to prevent problems
			// with getInterpolatedValue
			Vector3 origin = grid.getOrigin();
			Vector3 dim = grid.getDimension();
			Vector3 epsilon = dim;
			epsilon.normalize();
			epsilon *= 0.0001;
			origin += epsilon;
			dim -= epsilon * 2.0;

			RegularData3D* grid_ptr = 0;
			try
			{
				grid_ptr = new RegularData3D(new_size, origin, dim);
			}
			catch(...)
			{
			}

			if (grid_ptr == 0)
			{
				setStatusbarText(tr("Not enough memory to resize grid!"), true);
				return 0;
			}

			RegularData3D& new_grid = *grid_ptr;

			bool problem = false;
			Size s = new_grid.getData().size();
			for (Position i = 0; i < s; i++)
			{
				const Vector3& v = new_grid.getCoordinates(i);
				try
				{
					new_grid[i] = grid.getInterpolatedValue(v);
				}
				catch(...)
				{
					problem = true;
				}
			}
			
			// should not happen:
			if (problem) setStatusbarText(tr("Grid may be inaccurate!"), true);

			return grid_ptr;
		}

		RegularData3D* RegularData3DController::createHistogramGrid(const RegularData3D& grid) 
		{
			RegularData3D* new_grid = new RegularData3D(grid);
			vector<float>& normalized =  *(vector<float>*)&new_grid->getData();
			calculateHistogramEqualization(grid.getData(), normalized, false);
			return new_grid;
		}

		void RegularData3DController::createHistogramGrid()
		{
			RegularData3DDataset* data = dynamic_cast<RegularData3DDataset*>(getSelectedDataset());
			if (data == 0 ||
			    getData(data) == 0)
			{
				BALLVIEW_DEBUG
				return;
			}

			RegularData3D* new_data = createHistogramGrid(*getData(data));

			RegularData3DDataset* new_set = new RegularData3DDataset(*data);
			new_set->setName(String("normalized ") + data->getName());
			new_set->setData(new_data);
			insertDataset(new_set);
		}

		void RegularData3DController::resizeGrid()
		{
			RegularData3DDataset* data = dynamic_cast<RegularData3DDataset*>(getSelectedDataset());
			if (data == 0 ||
			    getData(data) == 0)
			{
				BALLVIEW_DEBUG
				return;
			}

			RegularData3D* new_data = resizeGrid(*getData(data));

			RegularData3DDataset* new_set = new RegularData3DDataset(*data);
			new_set->setName(String("resized ") + data->getName());
			new_set->setData(new_data);
			insertDataset(new_set);
		}

		///////////////////////////////////////////////////////////////////////////

		String TrajectoryController::type = "Trajectory";
		
		TrajectoryController::TrajectoryController()
			: DatasetController()
		{
			type_ = type;
			file_formats_.push_back("dcd");
			file_formats_.push_back("trr");
			setIdentifier("TrajectoryController");
			registerThis();
		}

		TrajectoryController::TrajectoryController(TrajectoryController& dc)
			: DatasetController(dc)
		{
			setIdentifier("TrajectoryController");
			registerThis();
		}

		TrajectoryController::~TrajectoryController()
		{
		}

		SnapShotManager* TrajectoryController::getData(Dataset* set)
		{
			SnapShotManagerDataset* dset = dynamic_cast<SnapShotManagerDataset*>(set);
			if (dset == 0) return 0;
			return dset->getData();
		}

		void TrajectoryController::deleteDataset_(Dataset* set)
		{
			SnapShotManager* data = getData(set);
			if (data == 0) return;
			delete data;

			if (dialogs_per_ssm_.has(data))
				dialogs_per_ssm_.erase(dialogs_per_ssm_.find(data));
		}


		bool TrajectoryController::write(Dataset* set, String /*filetype*/, String filename)
		{
			if (getData(set) == 0) return false;
			getData(set)->getTrajectoryFile()->copyTo(filename);
			return true;
		}

		Dataset* TrajectoryController::open(String /*filetype*/, String filename)
		{
			System* system = getMainControl()->getSelectedSystem();
			if (system == 0) return 0;

			TrajectoryFile* trajectory_file = TrajectoryFileFactory::open(filename, std::ios::in);

			if (trajectory_file->getNumberOfAtoms() != system->countAtoms())
			{
				setStatusbarText(tr("Number of atoms do not match. Aborting..."));
				delete trajectory_file;
				return 0;
			}

			SnapShotManager* manager = new SnapShotManager(system, 0, trajectory_file);

			String name = getNameFromFileName_(filename);

			SnapShotManagerDataset* set = new SnapShotManagerDataset;
			set->setData(manager);
			set->setName(name);
			set->setType(type_);
			set->setComposite(system);

			return set;
		}


		QMenu* TrajectoryController::buildContextMenu(QTreeWidgetItem* item)
		{
			QMenu* menu = DatasetController::buildContextMenu(item);
			if (menu == 0) return 0;
			menu->addSeparator();
			menu->addAction("Buffer Trajectory", this, SLOT(bufferTrajectory()));
			menu->addAction("Visualize Trajectory", this, SLOT(visualizeTrajectory()));
			return menu;
		}


		bool TrajectoryController::createMenuEntries()
		{
			if (!DatasetController::createMenuEntries()) return false;

			getDatasetControl()->getMainControl()->insertPopupMenuSeparator(MainControl::TOOLS, UIOperationMode::MODE_ADVANCED);

			String description = "Shortcut|TrajectoryController|Buffer_Trajectory";
			QAction* action = insertMenuEntry_(MainControl::TOOLS, "Buffer Trajectory", SLOT(bufferTrajectory()), 
			                                   description, QKeySequence(), tr("Buffer Trajectory in RAM for faster access"),
																				 UIOperationMode::MODE_ADVANCED);

			description = "Shortcut|TrajectoryController|Visualize_Trajectory";
			insertMenuEntry_(MainControl::TOOLS, "Visualize Trajectory", SLOT(visualizeTrajectory()),
			                 description, QKeySequence(), tr(""), UIOperationMode::MODE_ADVANCED);

			getDatasetControl()->getMainControl()->insertPopupMenuSeparator(MainControl::TOOLS, UIOperationMode::MODE_ADVANCED);

			return true;
		}

		bool TrajectoryController::visualizeTrajectory()
		{
			SnapShotManager* ssm = getData(getSelectedDataset());
			if (ssm == 0) 
			{
				BALLVIEW_DEBUG
				return false;
			}

			if (!dialogs_per_ssm_.has(ssm))
			{
				boost::shared_ptr<SnapshotVisualisationDialog> dialog(new SnapshotVisualisationDialog(getDatasetControl()));

				dialog->setSnapShotManager(ssm);
				dialog->show();

				dialogs_per_ssm_[ssm] = dialog;
			}
			else
			{
				dialogs_per_ssm_[ssm]->show();
			}

			return true;
		}

		bool TrajectoryController::bufferTrajectory()
		{
			Dataset* data = getSelectedDataset();
			if (getData(data) == 0) return false;

			SnapShotManager* ssm = getData(data);
			if (ssm->getNumberOfSnapShotsInBuffer() == 0)
			{
				if (!ssm->readFromFile())
				{
					ssm->clearBuffer();
					setStatusbarText(tr("Could not read trajectories into buffer! Out of memory?"));
				}
			}
			return true;
		}

		///////////////////////////////////////////////////////////////////////////

		String DockResultController::type = "DockResult";
		
		DockResultController::DockResultController()
			: DatasetController()
		{
			type_ = type;
			file_formats_.push_back("bdr");
			setIdentifier("DockResultController");
			registerThis();
		}

		DockResultController::DockResultController(DockResultController& dc)
			: DatasetController(dc)
		{
			setIdentifier("DockResultController");
			registerThis();
		}

		DockResultController::~DockResultController()
		{
		}

		DockResult* DockResultController::getData(Dataset* set)
		{
			DockResultDataset* dset = dynamic_cast<DockResultDataset*>(set);
			if (dset == 0) return 0;
			return dset->getData();
		}

		void DockResultController::deleteDataset_(Dataset* set)
		{
			DockResult* data = getData(set);
			if (data == 0) return;
			delete data;
		}

		bool DockResultController::write(Dataset* set, String /*filetype*/, 
																												String filename)
		{
			if (getData(set) == 0) return false;
			getData(set)->writeDockResult(filename);
			return true;
		}

		Dataset* DockResultController::open(String /*filetype*/, String filename)
		{
			DockResult* dr = new DockResult();
			if (!dr->readDockResult(filename))
			{
				setStatusbarText(tr("Could not read DockResult file!"), true);
				return 0;
			}
		
			String name = getNameFromFileName_(filename);
			DockResultDataset* set = new DockResultDataset;
			set->setData(dr);
			set->setName(name);
			set->setType(type_);

			// get docked system, set its name to name of file and add it to BALLView structures
			// apply the first SnapShot to the system to get the positions of the docked structures
			SnapShot ss = (*(dr->getConformationSet()))[0];
			System* docked_system = new System(dr->getConformationSet()->getSystem());
			docked_system->setName(name);
			ss.applySnapShot(*docked_system);
			getMainControl()->insert(*docked_system);

			return set;
		}


		QMenu* DockResultController::buildContextMenu(QTreeWidgetItem* item)
		{
			QMenu* menu = DatasetController::buildContextMenu(item);
			if (menu == 0) return 0;
			menu->addSeparator();
			menu->addAction("Visualize DockResult", this, SLOT(showDockResult()));
			return menu;
		}


		bool DockResultController::createMenuEntries()
		{
			if (!DatasetController::createMenuEntries()) return false;

			insertMenuEntry_(MainControl::TOOLS, "Visualize DockResult", SLOT(showDockResult()), 
			                 "Shortcut|Tools|Visualize_DockResult", QKeySequence(), tr(""), UIOperationMode::MODE_ADVANCED);

			getDatasetControl()->getMainControl()->insertPopupMenuSeparator(MainControl::TOOLS, UIOperationMode::MODE_ADVANCED);

			return true;
		}

		bool DockResultController::saveDockTrajectory()
		{
			Dataset* data = getSelectedDataset();
			if (getData(data) == 0) return false;

			QString s = QFileDialog::getSaveFileName(0,
										"Choose a Trajectory file to save",
										getDatasetControl()->getWorkingDir().c_str(),
										"DCD files(*.dcd)");

			if (s == QString::null) return false;
			String filename = ascii(s);
			getDatasetControl()->setWorkingDirFromFilename_(filename);

			DockResult* res = getData(data);
			if (!res->getConformationSet()->writeDCDFile(filename))
			{
				setStatusbarText(tr("Could not write DCDFile."), true);
				return false;
			}

			setStatusbarText(tr("Written DCDFile."), true);
			return true;
		}


		void DockResultController::showDockResult()
		{	
			Dataset* set = getSelectedDataset();
			if (getData(set) == 0) return;

			DatasetMessage* msg = new DatasetMessage(set, DatasetMessage::VISUALIZE);
			getMainControl()->sendMessage(*msg);
		}


		/////////////////////////////////////////////////////////////////////
		// VectorGrid Controller:

		String VectorGridController::type = "Vector Grid";

		VectorGridController::VectorGridController()
			: DatasetController()
		{
			type_ = type;
			file_formats_.push_back("bvg");
			setIdentifier("VectorGridController");
			registerThis();
		}

		VectorGridController::VectorGridController(VectorGridController& dc)
			: DatasetController(dc)
		{
			setIdentifier("VectorGridController");
			registerThis();
		}

		VectorGridController::~VectorGridController()
		{
		}

		VectorGrid* VectorGridController::getData(Dataset* set)
		{
			VectorGridDataset* dset = dynamic_cast<VectorGridDataset*>(set);
			if (dset == 0) return 0;
			return dset->getData();
		}

		void VectorGridController::deleteDataset_(Dataset* set)
		{
			VectorGrid* data = getData(set);
			if (data == 0) return;
			delete data;
		}


		bool VectorGridController::write(Dataset* set, String /*filetype*/, 
																												String filename)
		{
			if (getData(set) == 0) return false;
			VectorGrid* dat = getData(set);
			(*dat).binaryWrite(filename);

			return true;
		}

		Dataset* VectorGridController::open(String /*filetype*/, String filename)
		{
			VectorGridDataset* set = new VectorGridDataset;
			VectorGrid* dat = new VectorGrid;
			(*dat).binaryRead(filename);
			set->setData(dat);
			set->setName(getNameFromFileName_(filename));
			set->setType(type_);

			return set;
		}


		QMenu* VectorGridController::buildContextMenu(QTreeWidgetItem* item)
		{
			QMenu* menu = DatasetController::buildContextMenu(item);
			if (menu == 0) return 0;
			menu->addSeparator();
			menu->addAction("Visualize VectorGrid", this, SLOT(visualizeFieldLines()));
			return menu;
		}


		bool VectorGridController::createMenuEntries()
		{
			if (!DatasetController::createMenuEntries()) return false;

			insertMenuEntry_(MainControl::TOOLS_GRID, "Visualize Field Lines", SLOT(visualizeFieldLines()), 
			                 "Shortcut|Tools|Grid|Visualize_Field_Lines", QKeySequence(),
											 tr("Visualise a gradient grid per field lines"), UIOperationMode::MODE_ADVANCED);

			getDatasetControl()->getMainControl()->insertPopupMenuSeparator(MainControl::TOOLS_GRID, UIOperationMode::MODE_ADVANCED);

			return true;
		}


		bool VectorGridController::visualizeFieldLines()
		{
			Dataset* data = getSelectedDataset();
			if (getData(data) == 0) return false;

			FieldLinesDialog dialog;

			dialog.vector_grid_ = getData(data);
			dialog.potential_grid_ = 0;

			vector<Dataset*> sets3d = getDatasetControl()->getDatasets(RegularData3DController::type);
			if (sets3d.size() > 0)
			{
				RegularData3DDataset* rset = dynamic_cast<RegularData3DDataset*>(sets3d[0]);
				if (rset == 0) return false;
 				dialog.potential_grid_ = rset->getData();
			}

			dialog.ac_ = dynamic_cast<AtomContainer*>(data->getComposite());
			if (dialog.ac_ == 0)
			{
				const CompositeManager& cm = getMainControl()->getCompositeManager();
				if (+cm.begin()) dialog.ac_ = (AtomContainer*) *cm.begin();
			}
			
			if (!dialog.exec()) return false;

			Representation* rep = dialog.createLines();
			if (rep == 0) 
			{
				setStatusbarText(tr("Creation of field line failed, see logs for further details..."), true);
				return false;
			}

			getMainControl()->insert(*rep);
			getMainControl()->update(*rep);
			setStatusbarText(String("Finished field line calculations. ") + 
											String(dialog.field_line_errors_) + " errorous positions tried.", true);
			return true;
		}

		/////////////////////////////////////////////////////////////////////
		// RaytraceableGrid Controller:

		String RaytraceableGridController::type = "Raytraceable Grid";

		RaytraceableGridController::RaytraceableGridController()
			: DatasetController(),
				rt_contour_surface_dialog_(0)
		{
			type_ = type;
			setIdentifier("RaytraceableGridController");
			registerThis();
			file_formats_.push_back("vrt");
		}

		RaytraceableGridController::RaytraceableGridController(RaytraceableGridController& rc)
			: DatasetController(rc),
				rt_contour_surface_dialog_(rc.rt_contour_surface_dialog_)
		{
			setIdentifier("RaytraceableGridController");
			registerThis();
			file_formats_.assign(rc.file_formats_.begin(), rc.file_formats_.end());
		}

		RaytraceableGridController::~RaytraceableGridController()
		{
			if (rt_contour_surface_dialog_)
				delete rt_contour_surface_dialog_;
		}

		bool RaytraceableGridController::write(Dataset* set, String /*filetype*/, String filename)
		{
			RaytraceableGrid* grid = getData(set);
			if (grid == NULL)
			{
				return false;
			}
			
			grid->binaryWrite(filename);			

			return true;
		}

		RaytraceableGrid* RaytraceableGridController::getData(Dataset* set)
		{
			RaytraceableGridDataset* dset = dynamic_cast<RaytraceableGridDataset*>(set);
			if (dset == 0) return 0;
			return dset->getData();
		}
			
		void RaytraceableGridController::deleteDataset_(Dataset* set)
		{
			RaytraceableGrid* data = getData(set);
			if (data == 0) return;
			delete data;
		}

		QMenu* RaytraceableGridController::buildContextMenu(QTreeWidgetItem* item)
		{
			QMenu* menu = DatasetController::buildContextMenu(item);
			if (menu == 0) return 0;
			menu->addSeparator();
			menu->addAction("Create Contour Surface", this, SLOT(visualizeRaytraceableContourSurface()));
			menu->addSeparator();
			return menu;
		}

		bool RaytraceableGridController::createMenuEntries()
		{
			if (!DatasetController::createMenuEntries()) return false;

			getDatasetControl()->getMainControl()->insertPopupMenuSeparator(MainControl::TOOLS_GRID, UIOperationMode::MODE_ADVANCED);

			return true;
		}

		void  RaytraceableGridController::visualizeRaytraceableContourSurface()
		{
			Dataset* data = (Dataset*)getSelectedDataset();
			if (getData(data) == 0)
			{
				BALLVIEW_DEBUG
				return;
			}

			RaytraceableGrid* grid = getData(data);
			RaytraceableContourSurfaceDialog rcsd(grid, getDatasetControl());
			rcsd.exec();
		
/*			if (rt_contour_surface_dialog_ == 0) 
			{
				rt_contour_surface_dialog_ = new RaytraceableContourSurfaceDialog(getDatasetControl());
			}

			//rt_contour_surface_dialog_->setController(this);
			//rt_contour_surface_dialog_->setGrid(getData(data));
			//since this is no modal window no exec but:
			///rt_contour_surface_dialog_->show();
			rt_contour_surface_dialog_->raise();
			rt_contour_surface_dialog_->activateWindow();
			//rt_contour_surface_dialog_->exec();

  TODO: something like
  	Representation* rep = new Representation();
			rep->insert(*mesh);
			rep->setModelType(MODEL_CONTOUR_SURFACE); 

			List<const Composite*> composites;
			if (data.getComposite() != 0) composites.push_back(data.getComposite());
			rep->setComposites(composites);

			// Make sure BALLView knows about the new representation.
			getMainControl()->insert(*rep);
			getMainControl()->update(*rep);

  */

		}

	} // namespace VIEW
} // namespace BALL
