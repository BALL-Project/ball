// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: mainframe.C,v 1.60.2.29 2006/10/19 22:08:22 amoll Exp $
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
#include <BALL/VIEW/WIDGETS/editableScene.h>
#include <BALL/VIEW/WIDGETS/fileObserver.h>
#include <BALL/VIEW/DIALOGS/downloadPDBFile.h>
#include <BALL/VIEW/DIALOGS/labelDialog.h>
#include <BALL/VIEW/DIALOGS/displayProperties.h>

#include <BALL/VIEW/DATATYPE/colorMap.h>

#include <BALL/VIEW/WIDGETS/dockingController.h>

#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/COMMON/version.h>

//   #undef BALL_PYTHON_SUPPORT

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

#include <BALL/VIEW/PRIMITIVES/tube.h>
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
//   		Log.remove(std::cout);
//   		Log.remove(std::cerr);
		setLoggingFilename("BALLView.log");

 		addDockWidget(Qt::LeftDockWidgetArea, new MolecularControl(this, "Structures"));
		addDockWidget(Qt::LeftDockWidgetArea, new GeometricControl(this, "Representations"));
		addDockWidget(Qt::TopDockWidgetArea,  new DatasetControl(this, "Datasets"));
		DatasetControl::getInstance(0)->hide();

		HelpViewer* BALL_docu = new HelpViewer(this, "BALL Docu");
		addDockWidget(Qt::BottomDockWidgetArea, BALL_docu);
		String dirp = getDataPath() + ".." + FileSystem::PATH_SEPARATOR + "doc" + 
									FileSystem::PATH_SEPARATOR + "BALL" + FileSystem::PATH_SEPARATOR;
		BALL_docu->setBaseDirectory(dirp);
		BALL_docu->setWhatsThisEnabled(false);
		BALL_docu->setProject("BALL");
		BALL_docu->setDefaultPage("index.htm");

		addDockWidget(Qt::BottomDockWidgetArea, new HelpViewer(this, "BALLView Docu"));

		new DisplayProperties(	this, "DisplayProperties");
		new LabelDialog(				this, "LabelDialog");
		new MolecularFileDialog(this, "MolecularFileDialog");
		new DownloadPDBFile(		this, "DownloadPDBFile", false);
		new MolecularStructure(	this, "MolecularStructure");
 		addDockWidget(Qt::BottomDockWidgetArea, new LogView(this, "Logs"));
		new DockingController(  this, "DockingController");
		addDockWidget(Qt::BottomDockWidgetArea, new FileObserver(  this, "FileObserver"));

		Scene::stereoBufferSupportTest();
		scene_ = new EditableScene(this, "3D View");
		setCentralWidget(scene_);
		setAcceptDrops(true);

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
		insertMenuEntry(MainControl::HELP, "How to cite", this, SLOT(howToCite()));

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
			last_state_ = saveState();
			// This call is needed because showFullScreen won't work
			// correctly if the widget already considers itself to be fullscreen.
			last_size_ = size();
			last_point_ = pos();
			List<ModularWidget*>::Iterator it = modular_widgets_.begin(); 
			for (; it != modular_widgets_.end(); ++it)
			{
				DockWidget* widget = dynamic_cast<DockWidget*>(*it);
				if (widget == 0) continue;
				widget->hide();
			}

			showNormal();	
			showFullScreen();
		}
		else
		{
			showNormal();
			resize(last_size_.width(), last_size_.height());
			move(last_point_);
			restoreState(last_state_);
		}
		fullscreen_ = !fullscreen_;
	}

	void Mainframe::openFile(const String& file)
		throw()
	{
		if (composites_locked_) return;

		if (file == "") return;

		// workaround for drag and drop under windows: damn QT sometimes adds slashes!
		String filename(file);
#ifdef BALL_PLATFORM_WINDOWS
		if (filename[0] == '/') 
		{
			filename.trimLeft("/");
		}
#endif

		setStatusbarText(String("Opening file ") + filename + "...");

		if (filename.hasSuffix(".bvp"))
		{
			MainControl::loadBALLViewProjectFile(filename);
			return;
		}

#ifdef BALL_PYTHON_SUPPORT
		if (filename.hasSuffix(".py"))
		{
 			PyWidget::getInstance(0)->openFile(filename, true);
			return;
		}
#endif

		if (filename.hasSuffix(".dcd"))
		{
 			DatasetControl::getInstance(0)->addTrajectory(filename);
			return;
		}

		MolecularFileDialog::getInstance(0)->openFile(filename);
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

		if (e->key() == Qt::Key_Delete)
		{
			deleteClicked();
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

	void Mainframe::howToCite()
	{
		HelpViewer::getInstance(1)->showHelp("tips.html", "cite");
	}


	void enlarge(String& text, Size size)
	{
		if (text.size() > size)
		{
			text.truncate(size);
			return;
		}
		while (text.size() < size) text = String(" ") + text;
	}

	void Mainframe::write()
	{
		File file("out.mrk", std::ios::out);
		System& system = *(System*) *getCompositeManager().begin();
		file << "***" << std::endl;
		file << "***" << std::endl;

		String tmp = String(system.countAtoms());
		enlarge(tmp, 5);
		file << tmp;

		tmp = String(system.countBonds());
		enlarge(tmp, 6);
		file << tmp << std::endl;
		
		HashMap<Atom*, Position> map;
		char buf[40];

		Position at_nr = 1;
		AtomIterator ait = system.beginAtom();
		for(; +ait; ++ait)
		{
			Atom& atom = *ait;
			sprintf(buf, "%.4f", atom.getPosition().x);
			tmp = buf;
			tmp.trimRight();
			enlarge(tmp, 10);
			file << tmp;

			sprintf(buf, "%.4f", atom.getPosition().y);
			tmp = buf;
			enlarge(tmp, 11);
			file << tmp;

			sprintf(buf, "%.4f", atom.getPosition().z);
			tmp = buf;
			enlarge(tmp, 11);
			file << tmp;

			tmp = String(atom.getElement().getAtomicNumber());
			enlarge(tmp, 6);
			file << tmp;

			tmp = String(atom.getType());
			enlarge(tmp, 3);
			file << tmp;

			tmp = String(atom.getFormalCharge());
			enlarge(tmp, 2);
			file << tmp;

			tmp = String(at_nr);
			enlarge(tmp, 6);
			file << tmp;

			tmp = String(atom.getName());
			enlarge(tmp, 2);
			enlarge(tmp, 3);
			file << tmp;

			file << "    XXX";

			sprintf(buf, "%.4f", atom.getCharge());
			tmp = buf;
			enlarge(tmp, 11);
			file << tmp;
			
			file << "      MMFF" << std::endl;
			

			map[&atom] = at_nr;
			at_nr++;
		}

		Size max_bond = system.countBonds();
		Size bond_nr = 0;
		AtomBondIterator abit;
		BALL_FOREACH_BOND(system, ait, abit)
		{
			Atom* first = (Atom*)abit->getFirstAtom();
			Atom* second = (Atom*)abit->getSecondAtom();
			Size order = abit->getOrder();

			tmp = map[first];
			enlarge(tmp, 5);
			file << tmp;

			tmp = map[second];
			enlarge(tmp, 6);
			file << tmp;

			tmp = String(order);
			enlarge(tmp, 3);
			file << tmp;

			bond_nr ++;
			if (bond_nr % 5 == 0) 
			{
				file << std::endl;
			}
			else
			{
				if (bond_nr < max_bond) file << "  ";
			}
		}
		file.close();
	}

	void Mainframe::about()
	{
		write();
		return;
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
