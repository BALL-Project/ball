// $Id: molecularFileDialog.C,v 1.4 2002/12/12 18:16:23 amoll Exp $

#include <BALL/MOLVIEW/GUI/DIALOGS/molecularFileDialog.h>

#include <BALL/FORMAT/PDBFile.h>
#include <BALL/FORMAT/HINFile.h>
#include <BALL/FORMAT/MOLFile.h>
#include <BALL/FORMAT/MOL2File.h>
#include <BALL/VIEW/GUI/PRIMITIV/glsimpleBox.h>
#include <BALL/MATHS/box3.h>
#include <BALL/KERNEL/system.h>

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
			registerWidget(this);
		}

		MolecularFileDialog::~MolecularFileDialog()
			throw()
		{	
			#ifdef BALL_MOLVIEW_DEBUG
			Log.error() << "Destructing object " << (void *)this 
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
			main_control.insertMenuEntry(MainControl::FILE, "&Open...", (QObject *)this, 
																	 SLOT(readFile()), QFileDialog::CTRL+QFileDialog::Key_R);
			main_control.insertMenuEntry(MainControl::FILE, "&Save As...", (QObject *)this, 
																	 SLOT(writeFile()), QFileDialog::CTRL+QFileDialog::Key_W);
		}
		
		void MolecularFileDialog::finalizeWidget(MainControl& main_control)
			throw()
		{
			main_control.removeMenuEntry(MainControl::FILE, "&Read System", (QObject *)this, 
																	 SLOT(readFile()), QFileDialog::CTRL+QFileDialog::Key_R);
			main_control.removeMenuEntry(MainControl::FILE, "&Write System", (QObject *)this, 
																		SLOT(writeFile()), QFileDialog::CTRL+QFileDialog::Key_W);
		}

		void MolecularFileDialog::readFile()
		{
			// no throw specifier because of that #$%@* moc
			if (MainControl::getMainControl(this) == 0) throw VIEW::MainControlMissing(__FILE__, __LINE__);
			
			QFileDialog *fd = new QFileDialog(MainControl::getMainControl((QObject *)this), "Molecular File Dialog", true);
			fd->setMode(QFileDialog::ExistingFile);
			fd->addFilter("PDB Files (*.pdb *.brk *.ent)");
			fd->addFilter("HIN Files (*.hin)");
			fd->addFilter("MOL Files (*.mol)");
			fd->addFilter("MOL2 Files (*.mol2)");

			fd->setSelectedFilter(1);

			fd->setCaption("Select a molecular file");
			fd->setViewMode(QFileDialog::Detail);

			fd->exec();

			String filename(fd->selectedFile());
			String filter(fd->selectedFilter());

			bool ok = false;
			try
			{
				if (File::isReadable(filename)) ok = true;
			}
			catch(...)
			{}

			if (!ok || filename == "/" || filename == "\\") return;

			// construct a name for the system(the filename without the dir path)
			QString qfilename = fd->selectedFile();
			qfilename.remove(0, fd->dirPath().length() + 1);

			if (qfilename.find('.') != -1)
			{
				qfilename = qfilename.left(qfilename.find('.'));
			}

			if (filter.hasSubstring("PDB"))
			{
				readPDBFile(filename, String(qfilename));
			}
			else if (filter.hasSubstring("HIN"))
			{
				readHINFile(filename, String(qfilename));
			}
			else if (filter.hasSubstring("MOL"))
			{
				readMOLFile(filename, String(qfilename));
			}
			else if (filter.hasSubstring("MOL2"))
			{
				readMOL2File(filename, String(qfilename));
			}
		}


		bool MolecularFileDialog::writeFile()
		{
			// no throw specifier because of that #$%@* moc
			if (MainControl::getMainControl(this) == 0) throw VIEW::MainControlMissing(__FILE__, __LINE__);

			List<Composite*>& selection = MainControl::getMainControl(this)->getControlSelection();

			if (selection.size() == 0 || !RTTI::isKindOf<System> (**selection.begin()))
			{
				Log.error() << "Not a single system selected! Aborting writing..." << std::endl;
				return false;
			}

			setStatusbarText("writing PDB file...");

			QFileDialog *fd = new QFileDialog(MainControl::getMainControl((QObject *)this), "Molecular File Dialog", true);
			fd->setMode(QFileDialog::AnyFile);
			fd->addFilter("PDB Files (*.pdb)");

			fd->setSelectedFilter(1);

			fd->setCaption("Select a filename for writing the system");
			fd->setViewMode(QFileDialog::Detail);

			fd->exec();

			String filename(fd->selectedFile());

			const System& system = *(const System*) (*selection.begin());

			try
			{
				PDBFile pdb_file(filename, std::ios::out);
				pdb_file << system;
				pdb_file.close();
			}
			catch(...)
			{
				Log.info() << "> write PDB file failed." << std::endl;
				return false;
			}
			
			Log.info() << "> written " << system.countAtoms() << " atoms to PDB file \"" << filename << "\"" << std::endl;

			setStatusbarText("");

			return true;
		}


		bool MolecularFileDialog::readPDBFile(String filename, String system_name)
			throw()
		{

			// open a PDB File
			setStatusbarText("reading PDB file...");

			// reading PDB File
			System* system = new System();

			try
			{
				PDBFile pdb_file(filename);
				pdb_file >> *system;
				pdb_file.close();
			}
			catch(...)
			{
				Log.info() << "> read PDB file failed." << std::endl;
				delete system;
				return false;
			}

			// writing info to log
			Log.info() << "> read " << system->countAtoms() << " atoms from PDB file \"" << filename << "\"" << std::endl;

			if (system->getName() == "")
			{
				system->setName(system_name);
			}

			// notify main window
			NewCompositeMessage new_message;
			new_message.setComposite(system);
			new_message.setCompositeName(system_name);
			notify_(new_message);

			// notify main window
			setStatusbarText("");

			return true;
		}


		bool MolecularFileDialog::readHINFile(String filename, String system_name)
			throw()
		{
			bool has_periodic_boundary = false;
			Box3 bounding_box;

			// notify the main window
			setStatusbarText("reading HIN file...");

			// reading HIN File
			System* system = new System();

			try
			{
				HINFile hin_file(filename);

				hin_file >> *system;

				has_periodic_boundary = hin_file.hasPeriodicBoundary();
				bounding_box = hin_file.getPeriodicBoundary();

				hin_file.close();
			}
			catch(...)
			{
				Log.info() << "> read HIN file failed." << std::endl;
				delete system;

				return false;
			}

			// generating bounding box if exists
			if (has_periodic_boundary)
			{
				GLSimpleBox* simple_box = new GLSimpleBox;
				Vector3 first, second;

				bounding_box.get(first.x, first.y, first.z, second.x, second.y, second.z);
				Log.info() << "> creating bounding box (" << first << ", " << second << ")" << std::endl;

				simple_box->setVertex1(first);
				simple_box->setVertex2(second);
				simple_box->setColor("ff0000a0");
				simple_box->setName("BoundingBox");
				simple_box->setProperty(BALL::VIEW::GeometricObject::PROPERTY__OBJECT_TRANSPARENT);

				system->Composite::appendChild(*simple_box);
			}

			// writing info to log
			Log.info() << "> read " << system->countAtoms() << " atoms from HIN file \"" << filename << "\"" << std::endl;


			if (system->getName() == "")
			{
				system->setName(system_name);
			}

			// notify tree of a new composite
			NewCompositeMessage new_message;
			new_message.setComposite(system);
			new_message.setCompositeName(system_name);
			notify_(new_message);

			setStatusbarText("");
			return true;
		}


		bool MolecularFileDialog::readMOLFile(String filename, String system_name)
			throw()
		{
			// notify the main window
			setStatusbarText("reading MOL file...");

			// reading MOL2 File
			System* system = new System();

			try
			{
				MOLFile mol_file(filename);
				mol_file >> *system;
				mol_file.close();
			}
			catch(...)
			{
				Log.info() << "> read MOL file failed." << std::endl;
				delete system;
				return false;
			}

			// writing info to log
			Log.info() << "> read " << system->countAtoms() << " atoms from MOL file \"" << filename<< "\"" << std::endl;

			if (system->getName() == "")
			{
				system->setName(system_name);
			}

			// notify tree of a new composite
			NewCompositeMessage new_message;
			new_message.setComposite(system);
			new_message.setCompositeName(system_name);
			notify_(new_message);

			setStatusbarText("");
			return true;
		}


		bool MolecularFileDialog::readMOL2File(String filename, String system_name)
			throw()
		{
			// notify the main window
			setStatusbarText("reading MOL2 file...");

			// reading MOL2 File
			System* system = new System();

			try
			{
				MOL2File mol2_file(filename);
				mol2_file >> *system;
				mol2_file.close();
			}
			catch(...)
			{
				Log.info() << "> read MOL2 file failed." << std::endl;
				delete system;
				return false;
			}

			// writing info to log
			Log.info() << "> read " << system->countAtoms() << " atoms from MOL2 file \"" << filename<< "\"" << std::endl;

			if (system->getName() == "")
			{
				system->setName(system_name);
			}

			// notify tree of a new composite
			NewCompositeMessage new_message;
			new_message.setComposite(system);
			new_message.setCompositeName(system_name);
			notify_(new_message);

			setStatusbarText("");
			return true;
		}

	}
}
