// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: mainframe.C,v 1.47 2004/11/29 13:53:46 amoll Exp $
//

#include "mainframe.h"
#include "aboutDialog.h"
#include "icons.h"

#include <BALL/VIEW/KERNEL/moleculeObjectCreator.h>
#include <BALL/VIEW/KERNEL/server.h>
#include <BALL/VIEW/RENDERING/POVRenderer.h>
#include <BALL/VIEW/RENDERING/VRMLRenderer.h>
#include <BALL/VIEW/WIDGETS/molecularStructure.h>
#include <BALL/VIEW/WIDGETS/molecularControl.h>
#include <BALL/VIEW/WIDGETS/geometricControl.h>
#include <BALL/VIEW/WIDGETS/logView.h>
#include <BALL/VIEW/DIALOGS/downloadPDBFile.h>
#include <BALL/VIEW/DIALOGS/labelDialog.h>

#include <BALL/COMMON/version.h>

#ifdef BALL_PYTHON_SUPPORT
#	include <BALL/VIEW/WIDGETS/pyWidget.h>
#endif

#include <qmenubar.h>
#include <qlabel.h>
#include <qprinter.h>
#include <qpainter.h>
#include <qimage.h>

#include <sstream>

namespace BALL
{
	using namespace std;
	using namespace BALL::VIEW;

	Mainframe::Mainframe(QWidget* parent, const char* name)
		:	MainControl(parent, name, ".BALLView"),
			scene_(0),
			dataset_control_(0),
			display_properties_(0),
			file_dialog_(0),
			fullscreen_(false)
	{
		#ifdef BALL_VIEW_DEBUG
		Log.error() << "new Mainframe " << this << std::endl;
		#endif

		// ---------------------
		// setup main window
		// ---------------------
		setCaption("BALLView");
		setIcon(QPixmap(bucky_64x64_xpm));
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
		setAcceptDrops(true);

		CHECK_PTR(new MolecularControl(this, "Structures"));
		CHECK_PTR(new GeometricControl(this, "Representations"));

		dataset_control_ = new DatasetControl(this, "Datasets");
		CHECK_PTR(dataset_control_);

		scene_ = new Scene(this, "3D View");
		CHECK_PTR(scene_);
		scene_->setMinimumSize(10, 10);
		setCentralWidget(scene_);

		display_properties_ = new DisplayProperties(this, "DisplayProperties");
		CHECK_PTR(display_properties_);

		CHECK_PTR(new LabelDialog(this, "LabelDialog"));
		
		file_dialog_ = new MolecularFileDialog(this, "MolecularFileDialog");
		CHECK_PTR(file_dialog_);

		CHECK_PTR(new DownloadPDBFile(this, "DownloadPDBFile", false));

		CHECK_PTR(new MolecularStructure(this, "MolecularStructure"));

		// setup the VIEW server
		Server* server = new Server(this);
		CHECK_PTR(server);
		// registering object generator
		MoleculeObjectCreator* object_creator = new MoleculeObjectCreator;
		server->registerObjectCreator(*object_creator);

		LogView* logview = new LogView(this, "Logs");
		CHECK_PTR(logview);
		logview->setMinimumSize(10, 10);

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

		// Help-Menu -------------------------------------------------------------------
		insertMenuEntry(MainControl::HELP, "About", this, SLOT(about()));

		// Menu ------------------------------------------------------------------------
		menuBar()->setSeparator(QMenuBar::InWindowsStyle);

		setStatusbarText("Ready.");
	}

	Mainframe::~Mainframe()
		throw()
	{
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

	void Mainframe::about()
	{
		// Display about dialog
		AboutDialog about;
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
		setStatusbarText(String("Opening file ") + file + "...");

		if (file.hasSuffix(".bvp"))
		{
			MainControl::loadBALLViewProjectFile(file);
			return;
		}

#ifdef BALL_PYTHON_SUPPORT
		if (file.hasSuffix(".py"))
		{
			PyWidget::getInstance(0)->run(file);
			return;
		}
#endif

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
		QString result = QFileDialog::getSaveFileName(
				getWorkingDir().c_str(), "*.bvp", 0, "Select a BALLView project file");
		if (result.isEmpty())
		{
			return;
		}

		MainControl::saveBALLViewProjectFile(result.ascii());
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

 		MainControl::loadBALLViewProjectFile(result.ascii());
	}


	void Mainframe::keyPressEvent(QKeyEvent* e)
	{
		if (e->key() == Key_Escape) 
		{
			Scene::getInstance(0)->switchToLastMode();
			return;
		}

		#ifdef BALL_PYTHON_SUPPORT
			PyWidget::getInstance(0)->reactTo(*e);
			e->accept();
		#endif
	}

}
