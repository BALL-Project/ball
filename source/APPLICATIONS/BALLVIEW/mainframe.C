// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: mainframe.C,v 1.7 2004/05/27 18:13:07 oliver Exp $
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
		Log.remove(std::cout);
		Log.remove(std::cerr);
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
 	  menuBar()->setItemEnabled(menu_cs_, composites_muteable_ && (dataset_control_->countGrids() != 0));
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
		about.BALL_version_label->setText(QString("(BALL ") + BALL_RELEASE_STRING + ")");
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

} 
