// $Id: molecularFileDialog.C,v 1.1.2.1 2002/12/06 18:55:57 anhi Exp $

#include <BALL/MOLVIEW/GUI/DIALOGS/molecularFileDialog.h>

#include <BALL/FORMAT/PDBFile.h>
#include <BALL/FORMAT/HINFile.h>
#include <BALL/FORMAT/MOL2File.h>
#include <BALL/VIEW/GUI/PRIMITIV/glsimpleBox.h>
#include <BALL/MATHS/box3.h>
#include <BALL/KERNEL/system.h>
#
using namespace BALL::VIEW;

namespace BALL
{
	namespace MOLVIEW
	{
		MolecularFileDialog::MolecularFileDialog(QWidget *parent)
			throw()
			:	QWidget(parent),
				ModularWidget()
		{
			// register the widget with the MainControl
			ModularWidget::registerWidget(this);
		}

		MolecularFileDialog::~MolecularFileDialog()
			throw()
		{	
			#ifdef BALL_MOLVIEW_DEBUG
			std::cout << "Destructing object " << (void *)this 
								<< " of class MolecularFileDialog" << std::endl; 
			#endif 

			destroy();
		}

		void MolecularFileDialog::destroy()
			throw()
		{
		}

		void MolecularFileDialog::initializeWidget(MainControl& main_control)
			throw()
		{
			main_control.insertMenuEntry(MainControl::FILE_IMPORT, "&Read System", (QObject *)this, SLOT(readFile()), QFileDialog::CTRL+QFileDialog::Key_R);
			main_control.insertMenuEntry(MainControl::FILE, "&Write System", (QObject *)this, SLOT(writeFile()), QFileDialog::CTRL+QFileDialog::Key_W);
		}
		
		void MolecularFileDialog::finalizeWidget(MainControl& main_control)
			throw()
		{
			main_control.removeMenuEntry(MainControl::FILE, "&Read System", (QObject *)this, SLOT(readFile()), QFileDialog::CTRL+QFileDialog::Key_R);
			main_control.removeMenuEntry(MainControl::FILE, "&Write System", (QObject *)this, SLOT(writeFile()), QFileDialog::CTRL+QFileDialog::Key_W);
		}

		void MolecularFileDialog::readFile()
			throw()
		{
			QFileDialog *fd = new QFileDialog(MainControl::getMainControl((QObject *)this), "Molecular File Dialog", true);
			fd->setMode(QFileDialog::ExistingFile);
			fd->addFilter("PDB Files (*.pdb *.brk *.ent)");
			fd->addFilter("HIN Files (*.hin)");
			fd->addFilter("MOL2 Files (*.mol2)");

			fd->setSelectedFilter(1);

			fd->setCaption("Select a molecular file");
			fd->setViewMode(QFileDialog::Detail);

			fd->exec();

			String filename(fd->selectedFile());
			String filter(fd->selectedFilter());

			if (filter.hasSubstring("PDB"))
			{
				// open a PDB File
				WindowMessage window_message;
				window_message.setStatusBar("reading PDB file...");
				notify_(window_message);

				// reading PDB File
				System *system = new System();

				// memory allocation failed ?
				if (system == 0)
				{
					Log.info() << "> system memory allocation failed." << endl;
					return;
				}

				try
				{
					PDBFile pdb_file(filename);
					pdb_file >> *system;
					pdb_file.close();
				}
				catch(...)
				{
					Log.info() << "> read PDB file failed." << endl;
					delete system;
					return;
				}

				// writing info to log
				Log.info() << "> read " << system->countAtoms() << " atoms from PDB file \"" << filename << "\"" << endl;

				QString qfilename = fd->selectedFile();

				// construct a name (the filename without the dir path)
				qfilename.remove(0, fd->dirPath().length() + 1);

				if (qfilename.find('.') != -1)
				{
					qfilename = qfilename.left(qfilename.find('.'));
				}

				// notify main window
				NewCompositeMessage new_message;
				new_message.setComposite(system);
				new_message.setCompositeName(qfilename.ascii());

				if (system->getName() == "")
				{
					system->setName(qfilename.ascii());
				}

				notify_(new_message);

				// notify main window
				window_message.setStatusBar("");
				notify_(window_message);
			}
			if (filter.hasSubstring("HIN"))
			{
				bool has_periodic_boundary = false;
				Box3 bounding_box;

				// notify the main window
				WindowMessage window_message;
				window_message.setStatusBar("reading HIN file...");

				notify_(window_message);

				// reading HIN File
				System *system = new System();

				// memory allocation failed ?
				if (system == 0)
				{
					Log.info() << "> system memory allocation failed." << std::endl;
					return;
				}

				try
				{
					HINFile hin_file(fd->selectedFile().ascii());

					hin_file >> *system;

					has_periodic_boundary = hin_file.hasPeriodicBoundary();
					bounding_box = hin_file.getPeriodicBoundary();

					hin_file.close();
				}
				catch(...)
				{
					Log.info() << "> read HIN file failed." << std::endl;
					delete system;

					return;
				}

				// generating bounding box if exists
				if (has_periodic_boundary)
				{
					GLSimpleBox* simple_box = new GLSimpleBox;
					Vector3 first, second;

					bounding_box.get(first.x, first.y, first.z,
							second.x, second.y, second.z);
					Log.info() << "> creating bounding box (" << first << ", " << second << ")" << std::endl;

					simple_box->setVertex1(first);
					simple_box->setVertex2(second);
					simple_box->setColor("ff0000a0");
					simple_box->setName("BoundingBox");
					simple_box->setProperty(BALL::VIEW::GeometricObject::PROPERTY__OBJECT_TRANSPARENT);

					system->Composite::appendChild(*simple_box);
				}

				// writing info to log
				Log.info() << "> read " << system->countAtoms() << " atoms from HIN file \"" << fd->selectedFile() << "\"" << std::endl;


				QString filename = fd->selectedFile();

				// construct a name (the filename without the dir path)
				filename.remove(0, fd->dirPath().length() + 1);

				if (filename.find('.') != -1)
				{
					filename = filename.left(filename.find('.'));
				}

				// notify tree of a new composite
				NewCompositeMessage new_message;
				new_message.setComposite(system);
				new_message.setCompositeName(filename.ascii());

				if (system->getName() == "")
				{
					system->setName(filename.ascii());
				}

				notify_(new_message);

				// notify main window
				window_message.setStatusBar("");
				notify_(window_message);
			}
			if (filter.hasSubstring("MOL2"))
			{
				// notify the main window
				WindowMessage window_message;
				window_message.setStatusBar("reading MOL2 file...");

				notify_(window_message);

				// reading MOL2 File
				System *system = new System();

				// memory allocation failed ?
				if (system == 0)
				{
					Log.info() << "> system memory allocation failed." << std::endl;
					return;
				}

				try
				{
					MOL2File mol2_file(fd->selectedFile().ascii());
					mol2_file >> *system;
					mol2_file.close();
				}
				catch(...)
				{
					Log.info() << "> read MOL2 file failed." << std::endl;
					delete system;
					return;
				}

				// writing info to log
				Log.info() << "> read " << system->countAtoms() << " atoms from MOL2 file \"" << fd->selectedFile().ascii() << "\"" << std::endl;


				QString filename = fd->selectedFile();

				// construct a name (the filename without the dir path)
				filename.remove(0, fd->dirPath().length() + 1);

				if (filename.find('.') != -1)
				{
					filename = filename.left(filename.find('.'));
				}

				// notify tree of a new composite
				NewCompositeMessage new_message;
				new_message.setComposite(system);
				new_message.setCompositeName(filename.ascii());

				if (system->getName() == "")
				{
					system->setName(filename.ascii());
				}

				notify_(new_message);

				// notify main window
				window_message.setStatusBar("");
				notify_(window_message);
			}
	}

		bool MolecularFileDialog::writeFile()
			throw()
		{
		}

	}
}
