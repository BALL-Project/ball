// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: mainframe.C,v 1.60.2.13 2006/03/27 12:57:16 amoll Exp $
//

#include "mainframe.h"
#include "aboutDialog.h"
#include "icons.h"
#include "demoTutorialDialog.h"

#include <BALL/VIEW/KERNEL/moleculeObjectCreator.h>
#include <BALL/VIEW/KERNEL/server.h>
#include <BALL/VIEW/RENDERING/POVRenderer.h>
#include <BALL/VIEW/RENDERING/VRMLRenderer.h>
#include <BALL/VIEW/WIDGETS/molecularStructure.h>
#include <BALL/VIEW/WIDGETS/molecularControl.h>
#include <BALL/VIEW/WIDGETS/geometricControl.h>
#include <BALL/VIEW/WIDGETS/logView.h>
#include <BALL/VIEW/WIDGETS/helpViewer.h>
#include <BALL/VIEW/WIDGETS/datasetControl.h>
#include <BALL/VIEW/DIALOGS/downloadPDBFile.h>
#include <BALL/VIEW/DIALOGS/labelDialog.h>
#include <BALL/VIEW/DIALOGS/displayProperties.h>

#include <BALL/VIEW/DATATYPE/colorMap.h>

#include <BALL/VIEW/WIDGETS/dockingController.h>

#include <BALL/FORMAT/PDBFile.h>
#include <BALL/COMMON/version.h>

#ifdef BALL_PYTHON_SUPPORT
  #	include <BALL/VIEW/WIDGETS/pyWidget.h>
#endif

#include <QtGui/QPrintDialog>
#include <QtGui/qmenubar.h>
#include <QtGui/qlabel.h>
#include <QtGui/qprinter.h>
#include <QtGui/qpainter.h>
#include <QtGui/qimage.h>
#include <QtGui/qmessagebox.h>
#include <QtGui/qcursor.h>
#include <QtGui/QPixmap>
#include <QtGui/QKeyEvent>
#include <QtGui/QFileDialog>

#include <sstream>

#include <BALL/VIEW/PRIMITIVES/illuminatedLine.h>

using namespace std;

namespace BALL
{
	using namespace std;
	using namespace BALL::VIEW;

	Mainframe::Mainframe(QWidget* parent, const char* name)
		:	MainControl(parent, name, ".BALLView"),
			scene_(0),
			fullscreen_(false)
	{
		#ifdef BALL_VIEW_DEBUG
		Log.error() << "new Mainframe " << this << std::endl;
		#endif

		// ---------------------
		// setup main window
		// ---------------------
		setWindowTitle("BALLView");
		setWindowIcon(QPixmap(bucky_64x64_xpm));
		resize(800,600);
		// make sure submenus are the first 
		initPopupMenu(FILE_OPEN);
		initPopupMenu(EDIT);
		initPopupMenu(BUILD);
		initPopupMenu(DISPLAY);
		initPopupMenu(DISPLAY_VIEWPOINT);
		initPopupMenu(MOLECULARMECHANICS);
		initPopupMenu(TOOLS);
	#ifdef BALL_PYTHON_SUPPORT
		initPopupMenu(TOOLS_PYTHON);
		initPopupMenu(MainControl::USER);
	#endif
		initPopupMenu(WINDOWS);

		// ---------------------
		// Logstream setup -----
		// ---------------------
//		Lo g.remove(std::cout);
// 		Log.remove(std::cerr);
		setLoggingFilename("BALLView.log");

 		addDockWidget(Qt::LeftDockWidgetArea, new MolecularControl(this, "Structures"));
		addDockWidget(Qt::LeftDockWidgetArea, new GeometricControl(this, "Representations"));
		addDockWidget(Qt::TopDockWidgetArea,  new DatasetControl(this, "Datasets"));

		HelpViewer* BALL_docu = new HelpViewer(this, "BALL Docu");
		addDockWidget(Qt::AllDockWidgetAreas, BALL_docu);
		String dirp = getDataPath() + ".." + FileSystem::PATH_SEPARATOR + "doc" + 
									FileSystem::PATH_SEPARATOR + "BALL" + FileSystem::PATH_SEPARATOR;
		BALL_docu->setBaseDirectory(dirp);
		BALL_docu->setWhatsThisEnabled(false);
		BALL_docu->setProject("BALL");
		BALL_docu->setDefaultPage("index.htm");

		addDockWidget(Qt::AllDockWidgetAreas, new HelpViewer(this, "BALLView Docu"));

		new DisplayProperties(	this, "DisplayProperties");
		new LabelDialog(				this, "LabelDialog");
		new MolecularFileDialog(this, "MolecularFileDialog");
		new DownloadPDBFile(		this, "DownloadPDBFile", false);
		new MolecularStructure(	this, "MolecularStructure");
 		addDockWidget(Qt::BottomDockWidgetArea, new LogView(this, "Logs"));
		new DockingController(  this, "DockingController");

		Scene::stereoBufferSupportTest();
		scene_ = new Scene(this, "3D View");
		setCentralWidget(scene_);

		// setup the VIEW server
		Server* server = new Server(this);
		// registering object generator
		MoleculeObjectCreator* object_creator = new MoleculeObjectCreator;
		server->registerObjectCreator(*object_creator);

		DemoTutorialDialog* demo = new DemoTutorialDialog(this, "BALLViewDemo");

		#ifdef BALL_PYTHON_SUPPORT
 			addDockWidget(Qt::BottomDockWidgetArea, new PyWidget(this, "Python Interpreter"));
		#endif

		// ---------------------
		// Menus ---------------
		// ---------------------
		String hint;

		// File Menu
		QAction* entry = insertMenuEntry(MainControl::FILE_EXPORT, "POVRa&y scene", this, SLOT(exportPOVRay()), Qt::CTRL+Qt::Key_Y);

		// registerMenuEntryForHelpSystem
		RegisterHelpSystemMessage* msg = new RegisterHelpSystemMessage();
		msg->setObject(entry);
		msg->setURL("tips.html#povray");
		notify_(msg);

		insertMenuEntry(MainControl::FILE, "Print", this, SLOT(printScene()));

		insertMenuEntry(MainControl::FILE_OPEN, "Project", this, SLOT(loadBALLViewProjectFile()));

		save_project_action_ = insertMenuEntry(MainControl::FILE, "Save Project", this, 
										SLOT(saveBALLViewProjectFile()));

		// Display Menu
		insertMenuEntry(MainControl::DISPLAY, "Toggle Fullscreen", this, SLOT(toggleFullScreen()),
										Qt::ALT+Qt::Key_X);

		// Help-Menu -------------------------------------------------------------------
		insertMenuEntry(MainControl::HELP, "Demo", demo, SLOT(showDemo()));
		insertMenuEntry(MainControl::HELP, "Tutorial", demo, SLOT(showTutorial()));
		insertPopupMenuSeparator(MainControl::HELP);
		insertMenuEntry(MainControl::HELP, "About", this, SLOT(about()));

		// Menu ------------------------------------------------------------------------
//   		menuBar()->setSeparator(QMenuBar::InWindowsStyle);

		stop_simulation_action_ = insertMenuEntry(MainControl::MOLECULARMECHANICS, "Abort Calculation", this, 
										SLOT(stopSimulation()), Qt::ALT+Qt::Key_C);
		insertPopupMenuSeparator(MainControl::MOLECULARMECHANICS);
		setMenuHint(stop_simulation_action_, "Abort a running simulation thread");

		complement_selection_action_ = insertMenuEntry(MainControl::EDIT, "Toggle Selection", this, SLOT(complementSelection()));

		setStatusbarText("Ready.");
	}

	Mainframe::~Mainframe()
		throw()
	{
	}

	void Mainframe::exportPOVRay()
	{
		QString qresult = QFileDialog::getSaveFileName(
											0,
											"Export POVRay File",
											getWorkingDir().c_str(),
											"*.pov");

	 	if (qresult == QString::null) return;

		String result = ascii(qresult);

		if (!result.hasSuffix(".pov"))
		{
			result += ".pov";
		}

		POVRenderer pr(result);
		if (!scene_->exportScene(pr))
		{
			setStatusbarText("Could not export POV to " + result, true);
		}
		else
		{
			setStatusbarText("Exported POV to " + result);
		}
	}

	void Mainframe::toggleFullScreen()
	{
		if (!fullscreen_)
		{
			// This call is needed because showFullScreen won't work
			// correctly if the widget already considers itself to be fullscreen.
			last_size_ = size();
			last_point_ = pos();
			showNormal();	
			showFullScreen();
		}
		else
		{
			showNormal();
			resize(last_size_.width(), last_size_.height());
			move(last_point_);
		}
		fullscreen_ = !fullscreen_;
	}

	void Mainframe::openFile(const String& file)
		throw()
	{
		if (composites_locked_) return;

		if (file == "") return;

		setStatusbarText(String("Opening file ") + file + "...");

		if (file.hasSuffix(".bvp"))
		{
			MainControl::loadBALLViewProjectFile(file);
			return;
		}

#ifdef BALL_PYTHON_SUPPORT
		if (file.hasSuffix(".py"))
		{
 			PyWidget::getInstance(0)->runFile(file);
			return;
		}
#endif

		if (file.hasSuffix(".dcd"))
		{
 			DatasetControl::getInstance(0)->addTrajectory(file);
			return;
		}

		MolecularFileDialog::getInstance(0)->openFile(file);
	}


	void Mainframe::printScene()
	{
		QPrinter printer;
		QPrintDialog dialog(&printer, this);
		if (!dialog.exec()) return;
		
		setStatusbarText("printing..");

		QPainter p;
		if(!p.begin(&printer)) return; 

		QImage pic = scene_->grabFrameBuffer();
		p.drawImage(0,0, pic);	
		p.end();

		setStatusbarText("finished printing");
	}

	
	void Mainframe::saveBALLViewProjectFile()
		throw()
	{
		QString qresult = QFileDialog::getSaveFileName(
											0,
											"Select a BALLView project file",
											getWorkingDir().c_str(), 
											"*.bvp");

	 	if (qresult == QString::null) return;

		String result = ascii(qresult);
		if (result.isEmpty()) return;

		if (!result.hasSuffix(".bvp")) result += ".bvp";

 		MainControl::saveBALLViewProjectFile(result);
		setStatusbarText("Saved project to " + result);
	} 


	void Mainframe::loadBALLViewProjectFile()
		throw()
	{
		QString result = QFileDialog::getOpenFileName(
																		0,
																		"Select a BALLView project file",
																		getWorkingDir().c_str(), 
																		"*.bvp");
		if (result.isEmpty())
		{
			return;
		}

 		MainControl::loadBALLViewProjectFile(ascii(result));
	}


	void Mainframe::keyPressEvent(QKeyEvent* e)
	{
		if (e->key() == Qt::Key_Escape) 
		{
			Scene::getInstance(0)->switchToLastMode();
			return;
		}

		if (e->key() == Qt::Key_Enter) 
		{
			if (composite_manager_.getNumberOfComposites() == 0) return;

			if (getMolecularControlSelection().size() == 0)
			{
				control_selection_.push_back(*composite_manager_.begin());
			}
				
			MolecularStructure::getInstance(0)->centerCamera();
			return;
		}

		// check all menu entries if Alt or CTRL is pressed to enable shortcuts
		if (e->key() == Qt::Key_Alt ||
				e->key() == Qt::Key_Control)				
		{
			checkMenus();
			return;
		}

		#ifdef BALL_PYTHON_SUPPORT
 			PyWidget::getInstance(0)->reactTo(*e);
			e->accept();
		#endif
	}

	
	void Mainframe::reset()
	{
		if (composites_locked_ || getRepresentationManager().updateRunning()) return;

		DisplayProperties* dp = DisplayProperties::getInstance(0);
		dp->setDrawingPrecision(DRAWING_PRECISION_HIGH);
		dp->selectModel(MODEL_STICK);
		dp->selectColoringMethod(COLORING_ELEMENT);
		dp->selectMode(DRAWING_MODE_SOLID);
		dp->setTransparency(0);
		dp->setSurfaceDrawingPrecision(6.5);

		// remove all loaded Composites
		HashSet<Composite*> composites = getCompositeManager().getComposites();
		HashSet<Composite*>::Iterator it = composites.begin();
		
		for (; +it; ++it)
		{
			remove(**it, true, false);
		}

		// remove all Representations
		RepresentationManager::RepresentationList reps = getRepresentationManager().getRepresentations();
		RepresentationManager::RepresentationList::Iterator rit = reps.begin();

		for (; rit != reps.end(); ++rit)
		{
			remove(**rit);
		}
	}

	
	void Mainframe::checkMenus()
	{
		MainControl::checkMenus();
		save_project_action_->setEnabled(!composites_locked_);
	}

void createGradientGrid(const RegularData3D& potential, TRegularData3D<Vector3>& gradient_grid )
{
	RegularData3D::CoordinateType spacing = potential.getSpacing();
	RegularData3D::IndexType         size = potential.getSize();
	
	gradient_grid = TRegularData3D<Vector3>(potential.getOrigin(), potential.getDimension(), spacing);
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
}

inline void calculatePoints(TRegularData3D<Vector3>& gradient_grid, Vector3 point, vector<Vector3>& points)
{
	points.clear();

	float h = 0.1;
	RegularData3D::CoordinateType 			spacing = gradient_grid.getSpacing();
	TRegularData3D<Vector3>::IndexType  size 		= gradient_grid.getSize();
	Vector3 k1, k2, k3, k4;
	float error_estimate;

	float min_spacing = std::min(std::min(spacing.x, spacing.y), spacing.z);
	float rho = 0.9; // chose sensible values
	float lower_limit = min_spacing * 0.01;
	float tolerance = 1.0;
	float h_max = min_spacing * 2;
	int   number_of_interpolants = 2;
	Vector3 rk_estimate;
	Vector3 interpolation_s, interpolation_ua, interpolation_ub, interpolation_va, interpolation_vb;
	Vector3 interpolated_point;

	Vector3 grad_current = gradient_grid(point);
	Vector3 grad_old     = grad_current;

	// Runge - Kutta of order 4 with adaptive step size and
	// error control
	for (int i=0; i<1500; i++)
	{
		k1 = h*grad_current;
		k2 = h*gradient_grid(point+k1*0.5);
		k3 = h*gradient_grid(point+k2*0.5);
		k4 = h*gradient_grid(point+k3);

		rk_estimate = (k1+k2*2+k3*2+k4) * 1./6.;
		grad_old = grad_current;
		grad_current = gradient_grid(point+rk_estimate);
		
		// cubic Hermite interpolation of the resulting field line
		// between the old and the new point
		interpolation_s = Vector3(0.);

		Size nr_bak = number_of_interpolants ;
//   		if (i == 0) number_of_interpolants = 0;
		for (int j=0; j<number_of_interpolants; j++)
		{
			interpolation_s += Vector3(1. / (number_of_interpolants+1.));
		
			interpolation_va.x = interpolation_s.x*pow(1.-interpolation_s.x, 2);
			interpolation_va.y = interpolation_s.y*pow(1.-interpolation_s.y, 2);
			interpolation_va.z = interpolation_s.z*pow(1.-interpolation_s.z, 2);

			interpolation_vb.x = -pow(interpolation_s.x,2)*(1-interpolation_s.x);
			interpolation_vb.y = -pow(interpolation_s.y,2)*(1-interpolation_s.y);
			interpolation_vb.z = -pow(interpolation_s.z,2)*(1-interpolation_s.z);

			interpolation_ua.x = (1+2.*interpolation_s.x)*pow(1-interpolation_s.x, 2);
			interpolation_ua.y = (1+2.*interpolation_s.y)*pow(1-interpolation_s.y, 2);
			interpolation_ua.z = (1+2.*interpolation_s.z)*pow(1-interpolation_s.z, 2);

			interpolation_ub.x = (3-2.*interpolation_s.x)*pow(interpolation_s.x, 2);
			interpolation_ub.y = (3-2.*interpolation_s.y)*pow(interpolation_s.y, 2);
			interpolation_ub.z = (3-2.*interpolation_s.z)*pow(interpolation_s.z, 2);

			interpolated_point.x =  	point.x * interpolation_ua.x	
															+ (point.x+rk_estimate.x) * interpolation_ub.x
															+ rk_estimate.x * (  grad_old.x*interpolation_va.x
																									+grad_current.x*interpolation_vb.x);
			interpolated_point.y =  	point.y * interpolation_ua.y	
															+ (point.y+rk_estimate.y) * interpolation_ub.y
															+ rk_estimate.y * (  grad_old.y*interpolation_va.y
																									+grad_current.y*interpolation_vb.y);

			interpolated_point.z =  	point.z * interpolation_ua.z	
															+ (point.z+rk_estimate.z) * interpolation_ub.z
															+ rk_estimate.z * (  grad_old.z*interpolation_va.z
																									+grad_current.z*interpolation_vb.z);

			points.push_back(interpolated_point);
		}
		number_of_interpolants = nr_bak;

//   points.push_back(point);
		point += rk_estimate;

		error_estimate = ((k4-h*gradient_grid(point)) * 1./6.).getLength();

		// update h using the error estimate
		float h_new = h * pow(rho*tolerance / error_estimate, 1./5.);
		if (error_estimate > tolerance) h = h_new;
		else h = std::min(h_new, h_max);

		if ((h < lower_limit) || (rk_estimate.getLength() < lower_limit))
		{
			break;
		}
	}
}


	void Mainframe::about()
	{
		DatasetControl& dc = *DatasetControl::getInstance(0);
		RegularData3D& data = *((dc.get3DGrids()).begin())->first;
		TRegularData3D<Vector3> grid_data;
		createGradientGrid(data, grid_data);

		System& system = (*(System*)*composite_manager_.begin());
		Atom& atom = *system.beginAtom();

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

		for (Position p = 0; p < 500; p++)
		{
			Vector3 point = atom.getPosition();
			Vector3 diff(drand48(), drand48(), drand48());
			if (drand48() > 0.5) diff.x *= -1;
			if (drand48() > 0.5) diff.y *= -1;
			if (drand48() > 0.5) diff.z *= -1;
			diff.normalize();
			diff *= 0.4;
			point += diff;

			IlluminatedLine* line = new IlluminatedLine;
			vector<Vector3>& points = line->vertices;
			calculatePoints(grid_data, point, points);

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
				(*line).colors[v] = table.map(data(points[v]));

			}


			rep->insert(*line);
		}

		insert(*rep);
		update(*rep);
/*
		for (Position p = 0; p < 100; p++)
		{
			Vector3 point = atom.getPosition();
			Vector3 diff(drand48(), drand48(), drand48());
			if (drand48() > 0.5) diff.x *= -1;
			if (drand48() > 0.5) diff.y *= -1;
			if (drand48() > 0.5) diff.z *= -1;
			diff.normalize();
			point += diff;

			float h = 0.3;
			RegularData3D::CoordinateType spacing = data.getSpacing();
			RegularData3D::IndexType         size = data.getSize();
			Vector3 k1, k2, k3, k4;

			ColorMap table;
			ColorRGBA colors[3];
			colors[0] = ColorRGBA(1.0,0,0);
			colors[1] = ColorRGBA(.5,.0,5);
			colors[2] = ColorRGBA(.0,0,1.0);
			table.setRange(-1.5, 1.5);
			table.setBaseColors(colors,3);
			table.setNumberOfColors(100);
			IlluminatedLine* line = new IlluminatedLine;
			// and iterate for a while
			for (int i=0; i<5000; i++)
			{
				if (!data.isInside(point)) break;
				Vector3 v = grad(data, point, spacing, size);
			
				line->tangents.push_back(v);

				k1 = h*grad(data, point, spacing, size);
				k2 = h*grad(data, point+k1*0.5, spacing, size);
				k3 = h*grad(data, point+k2*0.5, spacing, size);
				k4 = h*grad(data, point+k3, spacing, size);

				point+=(k1+k2*2+k3*2+k4) * 1./6.;

				line->vertices.push_back(point);
				line->colors.push_back(table.map(data(point)));
			}

			rep->insert(*line);
		}


		insert(*rep);
		update(*rep);
*/

	return;

	/*
//   	Representation* rep = new Representation;
	String filename("/local/amoll/velocity.dat");
	std::FILE* file;
	file = fopen(filename.c_str(), "rb");
	if (file == NULL)
	{
		cerr << "Error while opening data file '" << filename << "'." << endl;
		return;
	}

	cout << "Reading data from '" << filename << "'." << endl;

	Size lineCount;
	// Get the number of polylines.
	fread(&lineCount, sizeof(int), 1, file);
	cout << "Preparing to read " << lineCount << " polylines..." << flush;

	// Get the size of each polyline.
	int* vertCount = new int[lineCount];
	fread(vertCount, sizeof(int), lineCount, file);

	int totalSize = 0;
	for (int i = 0; i < lineCount; i++)
	{
		totalSize += vertCount[i];
	}

	// Read the vertices.
	float* vertices = new float[3 * totalSize];
	fread(vertices, sizeof(float), 3 * totalSize, file);

	float* colors = 0;
	// Read the colors.
	colors = new float[4 * totalSize];
	fread(colors, sizeof(float), 4 * totalSize, file);

	int vpos = 0;
	int cpos = 0;
	for (Position l = 0; l < lineCount; l++)
	{
		IlluminatedLine* line = new IlluminatedLine;
		line->vertices.resize(vertCount[l]);
		line->tangents.resize(vertCount[l]);
 		line->colors.resize(vertCount[l]);
		for (Position v = 0; v < vertCount[l]; v++)
		{
			(*line).vertices[v].set(vertices[vpos],
														vertices[vpos+1],
														vertices[vpos+2]);
			vpos += 3;

			(*line).colors[v].set(colors[cpos],
													colors[cpos+1],
													colors[cpos+2],
													colors[cpos+3]);
			cpos += 4;
		}

		for (Position v = 0; v < vertCount[l] - 1; v++)
		{
			(*line).tangents[v] = (*line).vertices[v+1] - (*line).vertices[v];
		}

		(*line).tangents[vertCount[l] -1] = (*line).tangents[vertCount[l] -2];


		rep->insert(*line);
	}

	delete vertices;
	delete colors;

	cout << totalSize << " vertices have been read in." << endl;

	fclose(file);
	insert(*rep);
	update(*rep);
	return;
	*/

		// Display about dialog
		QDialog w;
 		Ui_AboutDialog about;
		about.setupUi(&w);
		QString version = QString("QT ") + qVersion();
#ifdef BALL_QT_HAS_THREADS
		version += "(mt)";
#endif
		about.qt_version_label->setText(version);
		QFont font = about.BALLView_version_label->font();
		about.BALLView_version_label->setText(QString("BALLView ") + BALL_RELEASE_STRING);
		font.setPixelSize(18);
		about.BALLView_version_label->setFont(font);
		about.BALL_version_label->setText(__DATE__);
		w.exec(); 
	}

}
