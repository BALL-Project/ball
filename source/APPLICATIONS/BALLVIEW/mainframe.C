// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: mainframe.C,v 1.17 2004/07/20 19:37:21 amoll Exp $
//

#include "mainframe.h"
#include "aboutDialog.h"
#include "icons.h"

#include <BALL/VIEW/PRIMITIVES/mesh.h>
#include <BALL/VIEW/KERNEL/moleculeObjectCreator.h>
#include <BALL/VIEW/RENDERING/POVRenderer.h>
#include <BALL/VIEW/RENDERING/VRMLRenderer.h>
#include <BALL/COMMON/version.h>

#include <BALL/DATATYPE/contourSurface.h>

#ifdef BALL_PYTHON_SUPPORT
#	include <BALL/VIEW/WIDGETS/pyWidget.h>
#endif

#include <qmenubar.h>
#include <qlabel.h>
#include <qprinter.h>
#include <qpainter.h>
#include <qimage.h>
#include <qapplication.h>

#include <BALL/CONCEPT/XDRPersistenceManager.h>
#include <sstream>

namespace BALL
{
	using namespace std;
	using namespace BALL::VIEW;

	Mainframe::Mainframe(QWidget* parent, const char* name)
		:	MainControl(parent, name, ".BALLView"),
			scene_(0),
			control_(0),
			dataset_control_(0),
			geometric_control_(0),
			display_properties_(0),
			label_dialog_(0),
			molecular_structure_(0),
			file_dialog_(0),
			server_(0),
			logview_(0),
			fullscreen_(false),
			menu_cs_(-1),
			menu_FPDB_(-1)
	{
	#ifdef BALL_VIEW_DEBUG
	Log.error() << "new Mainframe " << this << std::endl;
	#endif
		// ---------------------
		// setup main window
		// ---------------------
		setCaption("BALLView");
		setIcon(*new QPixmap(bucky_64x64_xpm));
		resize(640,400);
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
	#endif
		initPopupMenu(WINDOWS);

		// ---------------------
		// Logstream setup -----
		// ---------------------
//		Lo g.remove(std::cout);
// 		Log.remove(std::cerr);
		setLoggingFilename("BALLView.log");

		control_ = new MolecularControl(this, "Structures");
		CHECK_PTR(control_);

		geometric_control_ = new GeometricControl(this, "Representations");
		CHECK_PTR(geometric_control_);

		dataset_control_ = new DatasetControl(this, "Datasets");
		CHECK_PTR(dataset_control_);

		scene_ = new Scene(this, "3D View");
		CHECK_PTR(scene_);
		scene_->setMinimumSize(10, 10);
		setCentralWidget(scene_);

		display_properties_ = new DisplayProperties(this, "DisplayProperties");
		CHECK_PTR(display_properties_);

		surface_dialog_ = new ContourSurfaceDialog(this, "ContourSurfaceDialog");
		surface_dialog_->setDatasetControl(dataset_control_);
		CHECK_PTR(surface_dialog_);

		label_dialog_ = new LabelDialog(this, "LabelDialog");
		CHECK_PTR(label_dialog_);
		
		file_dialog_ = new MolecularFileDialog(this, "MolecularFileDialog");
		CHECK_PTR(file_dialog_);

		download_pdb_dialog_ = new DownloadPDBFile(this, "DownloadPDBFile", false);
		CHECK_PTR(download_pdb_dialog_);

		molecular_structure_ = new MolecularStructure(this, "MolecularStructure");
		CHECK_PTR(molecular_structure_);

		server_ = new Server(this);
		CHECK_PTR(server_);

		logview_ = new LogView(this, "Logs");
		CHECK_PTR(logview_);
		logview_->setMinimumSize(10, 10);

		FDPB_dialog_ = new FDPBDialog(this);
		CHECK_PTR(FDPB_dialog_);

		#ifdef BALL_PYTHON_SUPPORT
			PyWidget* pywidget = new PyWidget(this, "Python Interpreter");
			CHECK_PTR(pywidget);
			pywidget->startInterpreter();
		#endif

		// ---------------------
		// Menus ---------------
		// ---------------------
		String hint;

		// File Menu
		insertMenuEntry(MainControl::FILE_EXPORT, "POVRa&y scene", this, SLOT(exportPOVRay()), 
										CTRL+Key_Y);

		insertMenuEntry(MainControl::FILE, "Print", this, SLOT(printScene()));

		insertMenuEntry(MainControl::FILE_OPEN, "Project", this, SLOT(loadBALLViewProjectFile()));
		insertMenuEntry(MainControl::FILE, "Save Project", this, SLOT(saveBALLViewProjectFile()));

		// Display Menu
		insertMenuEntry(MainControl::DISPLAY, "Toggle Fullscreen", this, SLOT(toggleFullScreen()),
										ALT+Key_X);

		// Tools Menu -------------------------------------------------------------------
		hint = "Calculate the Electrostatics with FDPB, if one System selected.";
		menu_FPDB_ = insertMenuEntry(MainControl::TOOLS , "FDPB Electrostatics", FDPB_dialog_, SLOT(show()), 0,
				-1, hint);
				
		insertPopupMenuSeparator(MainControl::TOOLS);

		hint = "Calculate an isocontour surface from a 3D grid. The grid has to be loaded in the DatasetControl.";
		menu_cs_ = insertMenuEntry(MainControl::TOOLS, "Contour S&urface", this,  SLOT(computeIsoContourSurface()), 
										CTRL+Key_U,-1, hint);

		// Help-Menu -------------------------------------------------------------------
		insertMenuEntry(MainControl::HELP, "About", this, SLOT(about()));

		// Menu ------------------------------------------------------------------------
		menuBar()->setSeparator(QMenuBar::InWindowsStyle);

		//--------------------------------
		// setup the VIEW server
		//--------------------------------

		// registering object generator
		MoleculeObjectCreator* object_creator = new MoleculeObjectCreator;
		server_->registerObjectCreator(*object_creator);

		setStatusbarText("Ready.");
	}

	Mainframe::~Mainframe()
		throw()
	{
	}

	void Mainframe::checkMenus()
	{
		if (menu_cs_ == -1) return;
		menuBar()->setItemEnabled( menu_FPDB_, (getSelectedSystem() != 0) && composites_muteable_);
 	  menuBar()->setItemEnabled(menu_cs_, composites_muteable_ && (dataset_control_->count3DGrids() != 0));
		MainControl::checkMenus();
	}

	void Mainframe::exportPOVRay()
	{
		QFileDialog fd(this, "", true);
		fd.setMode(QFileDialog::AnyFile);
		fd.setCaption("Export POVRay File");
		fd.setViewMode(QFileDialog::Detail);

		if (!fd.exec()== QDialog::Accepted) return;

		POVRenderer pr(fd.selectedFile().ascii());
		scene_->exportScene(pr);
	}

	void Mainframe::exportVRML()
	{
		QFileDialog *fd = new QFileDialog(this, "", true);
		fd->setMode(QFileDialog::AnyFile);
		fd->setCaption("Export POVRay File");
		fd->setViewMode(QFileDialog::Detail);

		if (!fd->exec()== QDialog::Accepted) return;

		String filename(fd->selectedFile().ascii());
		delete fd;

		VRMLRenderer pr(filename);
		pr.width  = scene_->width();
		pr.height = scene_->height(); 
		pr.init(*(scene_->getStage()));
		scene_->exportScene(pr);
		pr.finish();
	}

	void Mainframe::computeIsoContourSurface()
	{
		// execute the surface dialog and abort if cancel is clicked
		if (!surface_dialog_->exec()) return;

		// Create a new contour surface.
		ContourSurface cs(*surface_dialog_->getGrid(), surface_dialog_->getThreshold());
		Mesh* mesh = new Mesh;
		mesh->Surface::operator = (static_cast<Surface&>(cs));

		// Create a new representation containing the contour surface.
		Representation* rep = getPrimitiveManager().createRepresentation();
		rep->insert(*mesh);
		rep->setModelType(MODEL_CONTOUR_SURFACE); 

		// Make sure BALLView knows about the new representation.
		RepresentationMessage* message = new RepresentationMessage(*rep, RepresentationMessage::ADD);
		notify_(message);
	}

	void Mainframe::about()
	{
		// Display about dialog
		AboutDialog about;
		about.qt_version_label->setText(QString("QT ") + qVersion());
		QFont font = about.BALLView_version_label->font();
		about.BALLView_version_label->setText(QString("BALLView ") + BALL_RELEASE_STRING);
		font.setPixelSize(18);
		about.BALLView_version_label->setFont(font);
		about.BALL_version_label->setText(__DATE__);
		about.exec(); 
	}

	void Mainframe::toggleFullScreen()
	{
		if (!fullscreen_)
		{
			// This call is needed because showFullScreen won't work
			// correctly if the widget already considers itself to be fullscreen.
			showNormal();	
			showFullScreen();
			setGeometry(qApp->desktop()->screenGeometry());
		}
		else
		{
			showNormal();
		}
		fullscreen_ = !fullscreen_;
	}

	void Mainframe::openFile(const String& file)
		throw()
	{
		file_dialog_->openFile(file);
	}


	void Mainframe::printScene()
	{
		QPrinter printer;
		if (!printer.setup(this)) return;
		
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
		if (getSelectedSystem() == 0)
		{
			setStatusbarText("Exactly one System has to be selected! Aborting...");
			return;
		}

		QString result = QFileDialog::getSaveFileName(
				getWorkingDir().c_str(), "*.bvp", 0, "Select a BALLView project file");
		if (result.isEmpty())
		{
			return;
		}

		INIFile out(result.ascii());
		out.appendSection("WINDOWS");

		// check menu entries, fetch and apply preferences
		List<ModularWidget*>::Iterator mit = modular_widgets_.begin(); 
		for (mit = modular_widgets_.begin(); mit != modular_widgets_.end(); ++mit)
		{
			(*mit)->writePreferences(out);
		}

		System& system = *getSelectedSystem();
 		String molecular_file = String(result.ascii())+"_molecule.xdr";
		out.appendSection("BALLVIEW_PROJECT");
		out.insertValue("BALLVIEW_PROJECT", "MolecularFile", molecular_file);

	 	ofstream outfile(molecular_file.c_str(), std::ios::out);
		TextPersistenceManager pm(outfile);
		system >> pm;
		outfile.close();

		out.insertValue("BALLVIEW_PROJECT", "Camera", scene_->getStage()->getCamera().toString());

		Position nr_of_representations = 0;
		PrimitiveManager::RepresentationsConstIterator it = getPrimitiveManager().begin();
		for (; it != getPrimitiveManager().end(); it++)
		{
			bool ok = true;
			Representation::CompositesConstIterator cit = (**it).begin();
			for (; cit != (**it).end(); cit++)
			{
				if ((**cit).getRoot() != system)
				{
					ok = false;
					break;
				}
			}

			if (!ok) continue;

			out.insertValue("BALLVIEW_PROJECT", 
											String("Representation") + String(nr_of_representations),
											(**it).toString());
			nr_of_representations++;
		}
				
		writePreferences(out);
	} 


	void Mainframe::loadBALLViewProjectFile()
		throw()
	{
		QString result = QFileDialog::getOpenFileName(
				getWorkingDir().c_str(), "*.bvp", 0, "Select a BALLView project file");
		if (result.isEmpty())
		{
			return;
		}

		loadBALLViewProjectFile(result.ascii());
	}


	void Mainframe::loadBALLViewProjectFile(const String& filename)
		throw()
	{
		INIFile in(filename);
		in.read();

		// check menu entries, fetch and apply preferences
		List<ModularWidget*>::Iterator it = modular_widgets_.begin(); 
		for (it = modular_widgets_.begin(); it != modular_widgets_.end(); ++it)
		{
			(*it)->fetchPreferences(in);
			(*it)->applyPreferences();
		}

		System* new_system = 0;
		String molecular_file;
		if (in.hasEntry("BALLVIEW_PROJECT", "MolecularFile"))
		{
			molecular_file = in.getValue("BALLVIEW_PROJECT", "MolecularFile");
			display_properties_->enableCreationForNewMolecules(false);

			ifstream infile(molecular_file.c_str(), std::ios::in);
			TextPersistenceManager pm(infile);
			PersistentObject* po = pm.readObject();
			if (!RTTI::isKindOf<System>(*po))
			{
				setStatusbarText("Error while reading project file! Aborting...");
				Log.error() << "Error while reading project file! Aborting..." << std::endl;
				return;
			}
			new_system = (System*) po;
			infile.close();
			insert(*new_system);
		}

		try
		{
			for (Position p = 0; p < 9999999; p++)
			{
				if (!in.hasEntry("BALLVIEW_PROJECT", "Representation" + String(p)))
				{
					break;
				}

				String data_string = in.getValue("BALLVIEW_PROJECT", "Representation" + String(p));


				vector<String> string_vector;
				Size split_size = data_string.split(string_vector, "[]");
				if (split_size == 1) 
				{
					Size split_size = data_string.split(string_vector);
					if (split_size != 4)
					{
						continue;
					}

					Representation* rep = new Representation();
					rep->setModelType(MODEL_CLIPPING_PLANE);
					rep->setProperty("AX", string_vector[0].toFloat());
					rep->setProperty("BY", string_vector[1].toFloat());
					rep->setProperty("CZ", string_vector[2].toFloat());
					rep->setProperty("D", string_vector[3].toFloat());

					insert(*rep);
					continue;
				}
				display_properties_->getSettingsFromString(data_string);
				
				data_string = string_vector[1];
				data_string.split(string_vector, ",");
				HashSet<Position> hash_set;
				for (Position p = 0; p < string_vector.size(); p++)
				{
					hash_set.insert(string_vector[p].toUnsignedInt());
				}
	
				getSelection().clear();
				Position current = 0;
				setSelection_(new_system, hash_set, current);
				NewSelectionMessage* msg = new NewSelectionMessage();
				notify_(msg);
			
				display_properties_->applyButtonClicked();
			}
		}
		catch(...)
		{
			setStatusbarText("Error while reading project file! Aborting...");
			Log.error() << "Error while reading project file! Aborting..." << std::endl;
			return;
		}
	
		getSelection().clear();
		NewSelectionMessage* msg = new NewSelectionMessage();
		notify_(msg);
 	
		fetchPreferences(in);

		if (in.hasEntry("BALLVIEW_PROJECT", "Camera"))
		{
			Stage stage;
			Camera c;
			if (!c.readFromString(in.getValue("BALLVIEW_PROJECT", "Camera")))
			{
				setStatusbarText("Could not read Camera position from project");
				Log.error() << "Could not read Camera position from project" << std::endl;
				return;
			}
			stage.setCamera(c);
			SceneMessage* msg = new SceneMessage(SceneMessage::UPDATE_CAMERA);
			msg->setStage(stage);
			notify_(msg);
		}

		display_properties_->enableCreationForNewMolecules(true);

		Scene::getInstance(0)->fetchPreferences(in);
		Scene::getInstance(0)->applyPreferences();
	}

	void Mainframe::setSelection_(Composite* c, HashSet<Position>& hash_set, Position& current)
		throw()
	{
		if (hash_set.has(current))
		{
			getSelection().insert(c);
			hash_set.erase(current);
		}

		current++;

		for (Position p = 0; p < c->getDegree() && hash_set.size() > 0; p++)
		{
			setSelection_(c->getChild(p), hash_set, current);
		}
	}

}
