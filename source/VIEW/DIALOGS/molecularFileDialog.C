// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: molecularFileDialog.C,v 1.12 2003/12/10 15:10:14 amoll Exp $

#include <BALL/VIEW/DIALOGS/molecularFileDialog.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/message.h>

#include <BALL/FORMAT/INIFile.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/FORMAT/HINFile.h>
#include <BALL/FORMAT/MOLFile.h>
#include <BALL/FORMAT/MOL2File.h>
#include <BALL/MATHS/simpleBox3.h>
#include <BALL/KERNEL/system.h>

#include <qmenubar.h>

namespace BALL
{
	namespace VIEW
	{
		MolecularFileDialog::MolecularFileDialog(QWidget *parent, const char* name)
			throw()
			:	QWidget(parent),
				ModularWidget(name),
				file_format_(0),
				x_(100),
				y_(100),
				width_(500),
				height_(500)
		{
#ifdef BALL_VIEW_DEBUG
	Log.error() << "new MolecularFileDialog " << this << std::endl;
#endif
			// register the widget with the MainControl
			registerWidget(this);
			hide();
		}

		MolecularFileDialog::~MolecularFileDialog()
			throw()
		{	
			#ifdef BALL_VIEW_DEBUG
			Log.info() << "Destructing object " << (void *)this 
								 << " of class MolecularFileDialog" << std::endl; 
			#endif 
		}

		void MolecularFileDialog::initializeWidget(MainControl& main_control)
			throw()
		{
			String hint("Open a PDB, HIN, MOL or MOL2 file");
			main_control.insertMenuEntry(MainControl::FILE_OPEN, "&Structure", (QObject *)this, 
																	 SLOT(readFile()), CTRL+Key_O, -1, hint);
			hint = "Save a system as PDB, HIN, MOL or MOL2 file (1 System has to be selected)";
			save_id_ = main_control.insertMenuEntry(MainControl::FILE, "&Save Structure", (QObject *)this, 
																	 SLOT(writeFile()), CTRL+Key_S, -1, hint);

			connect(main_control.initPopupMenu(MainControl::FILE), SIGNAL(aboutToShow()), this, SLOT(checkMenuEntries()));
		}
		
		void MolecularFileDialog::finalizeWidget(MainControl& main_control)
			throw()
		{
			main_control.removeMenuEntry(MainControl::FILE_OPEN, "&Open Structure", (QObject *)this, 
																	 SLOT(readFile()), CTRL+Key_R);
			main_control.removeMenuEntry(MainControl::FILE, "&Save Structure", (QObject *)this, 
																		SLOT(writeFile()), CTRL+Key_S);
		}

		void MolecularFileDialog::readFile()
		{
			QFileDialog *fd = new QFileDialog(getMainControl(), "Molecular File Dialog", true);

			fd->setMode(QFileDialog::ExistingFile);
			fd->setFilter("PDB Files (*.pdb *.brk *.ent)");
			fd->addFilter("HIN Files (*.hin)");
			fd->addFilter("MOL Files (*.mol)");
			fd->addFilter("MOL2 Files (*.mol2)");

			fd->setSelectedFilter(file_format_);

			fd->setCaption("Select a molecular file");
			fd->setViewMode(QFileDialog::Detail);
			fd->setGeometry(x_, y_, width_, height_);

			if (working_dir_ != "") fd->setDir(working_dir_.c_str());

			int result_dialog = fd->exec();
			x_ = fd->x();
			y_ = fd->y();
			width_ = fd->width();
			height_ = fd->height();
			working_dir_ = fd->dirPath();
			if (!result_dialog == QDialog::Accepted) return;


			String filename(fd->selectedFile().ascii());
			String filter(fd->selectedFilter().ascii());

			// construct a name for the system(the filename without the dir path)
			QString qfilename = fd->selectedFile();
			qfilename.remove(0, fd->dirPath().length() + 1);

			if (qfilename.find('.') != -1)
			{
				qfilename = qfilename.left(qfilename.find('.'));
			}

			openFile(String(fd->selectedFile().ascii()), String(filter), String(qfilename.ascii()));
		}


		void MolecularFileDialog::openFile(const String& file)
			throw()
		{
			vector<String> fields;
			String seperator = FileSystem::PATH_SEPARATOR;
			String filename = fields[file.split(fields, 
															 String(FileSystem::PATH_SEPARATOR).c_str()) -1];
			String extension = fields[filename.split(fields, ".") -1];
			filename = filename.getSubstring(0, filename.size() - (extension.size() + 1));
			openFile(file, extension, filename);
		}
		

		void MolecularFileDialog::openFile(const String& filename, 
																	 		 const String& filetype, 
																	 		 const String& system_name)
			throw()
		{
			bool ok = false;
			try
			{
				if (File::isReadable(filename)) ok = true;
			}
			catch(...)
			{}

			if (!ok || filename == "/" || filename == "\\") 
			{
				Log.error() << "Could not open file " << filename << std::endl;
				return;
			}


			if (filetype.hasSubstring("pdb") ||
					filetype.hasSubstring("brk") ||
					filetype.hasSubstring("ent") )
			{
				readPDBFile(filename, system_name);
				file_format_ = PDB_FILE;
			}
			else if (filetype.hasSubstring("HIN") ||
							 filetype.hasSubstring("hin"))
			{
				readHINFile(filename, system_name);
				file_format_ = HIN_FILE;
			}
			else if (filetype.hasSubstring("MOL2") ||
							 filetype.hasSubstring("mol2"))
			{
				readMOL2File(filename, system_name);
				file_format_ = MOL_FILE;
			}
			else if (filetype.hasSubstring("MOL") ||
							 filetype.hasSubstring("mol"))
			{
				readMOLFile(filename, system_name);
				file_format_ = MOL2_FILE;
			}
			else
			{
				Log.error() << "Unknown filetype: " << filetype << std::endl;
			}
		}


		bool MolecularFileDialog::writeFile()
		{
			List<Composite*>& selection = 
				((MainControl*) ((ConnectionObject*)this)->getRoot())->getControlSelection();

			if (selection.size() == 0 || !RTTI::isKindOf<System> (**selection.begin()))
			{
				Log.error() << "Not a single system selected! Aborting writing..." << std::endl;
				return false;
			}
			

			setStatusbarText("writing PDB file...");

			QFileDialog *fd = new QFileDialog(
					(MainControl*) ((ConnectionObject*)this)->getRoot()
					, "Molecular File Dialog", true);
			fd->setMode(QFileDialog::AnyFile);
			fd->setFilter("PDB Files (*.pdb)");
			fd->addFilter("HIN Files (*.hin)");
			fd->addFilter("MOL Files (*.mol)");
			fd->addFilter("MOL2 Files (*.mol2)");

			fd->setSelectedFilter(file_format_);

			fd->setCaption("Select a filename for writing the system");
			fd->setViewMode(QFileDialog::Detail);
			fd->setGeometry(x_, y_, width_, height_);
			if (working_dir_ != "") fd->setDir(working_dir_.c_str());

			int result_dialog = fd->exec();
			x_ = fd->x();
			y_ = fd->y();
			width_ = fd->width();
			height_ = fd->height();
			working_dir_ = fd->dirPath();
			if (!result_dialog == QDialog::Accepted) return false;

			String filename(fd->selectedFile().ascii());
			String filter(fd->selectedFilter().ascii());
			
			if (filename == "/" || filename == "\\") 
			{
				return false;
			}

			const System& system = *(const System*) (*selection.begin());

			bool result = false;
			if (filter.hasSubstring("PDB"))
			{
				result = writePDBFile(filename, system);
				file_format_ = PDB_FILE;
			}
			else if (filter.hasSubstring("HIN"))
			{
				result = writeHINFile(filename, system);
				file_format_ = HIN_FILE;
			}
			else if (filter.hasSubstring("MOL"))
			{
				result = writeMOLFile(filename, system);
				file_format_ = MOL_FILE;
			}
			else if (filter.hasSubstring("MOL2"))
			{
				result = writeMOL2File(filename, system);
				file_format_ = MOL2_FILE;
			}

			if (!result) 
			{
				return false;
			}
		
			Log.info() << "> " << system.countAtoms() << " atoms written to file \"" << filename << "\"" << std::endl;

			return true;
		}

		bool MolecularFileDialog::writePDBFile(String filename, const System& system)
			throw()
		{
			try
			{
				PDBFile file(filename, std::ios::out);
				file << system;
				file.close();
			}
			catch(...)
			{
				Log.info() << "> write PDB file failed." << std::endl;
				return false;
			}

			return true;
		}


		bool MolecularFileDialog::writeHINFile(String filename, const System& system)
			throw()
		{
			try
			{
				HINFile file(filename, std::ios::out);
				file << system;
				file.close();
			}
			catch(...)
			{
				Log.info() << "> write HIN file failed." << std::endl;
				return false;
			}

			return true;
		}


		bool MolecularFileDialog::writeMOLFile(String filename, const System& system)
			throw()
		{
			try
			{
				MOLFile file(filename, std::ios::out);
				file << system;
				file.close();
			}
			catch(...)
			{
				Log.info() << "> write MOL file failed." << std::endl;
				return false;
			}

			return true;
		}


		bool MolecularFileDialog::writeMOL2File(String filename, const System& system)
			throw()
		{
			try
			{
				MOL2File file(filename, std::ios::out);
				file << system;
				file.close();
			}
			catch(...)
			{
				Log.info() << "> write MOL2 file failed." << std::endl;
				return false;
			}

			return true;		
		}


		bool MolecularFileDialog::readPDBFile(String filename, String system_name)
			throw()
		{
			setStatusbarText("reading PDB file...");

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

			return finish_(filename, system_name, system);
		}


		bool MolecularFileDialog::readHINFile(String filename, String system_name)
			throw()
		{
			bool has_periodic_boundary = false;
			SimpleBox3 bounding_box;

			setStatusbarText("reading HIN file...");

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
				/*
				Box3* simple_box = new Box3;
				Vector3 first, second;

				bounding_box.get(first.x, first.y, first.z, second.x, second.y, second.z);
				Log.info() << "> creating bounding box (" << first << ", " << second << ")" << std::endl;

				simple_box->setVertex1(first);
				simple_box->setVertex2(second);
				simple_box->setColor("ff0000a0");
				simple_box->setName("BoundingBox");
				simple_box->setProperty(BALL::VIEW::GeometricObject::PROPERTY__OBJECT_TRANSPARENT);

				*/
			}

			return finish_(filename, system_name, system);
		}


		bool MolecularFileDialog::readMOLFile(String filename, String system_name)
			throw()
		{
			setStatusbarText("reading MOL file...");

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

			return finish_(filename, system_name, system);
		}


		bool MolecularFileDialog::readMOL2File(String filename, String system_name)
			throw()
		{
			setStatusbarText("reading MOL2 file...");

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

			return finish_(filename, system_name, system);
		}
		

		bool MolecularFileDialog::finish_(const String& filename, const String& system_name, System* system)
			throw()
		{
			// writing info to log
			Log.info() << "> read " << system->countAtoms() << " atoms from file \"" << filename<< "\"" << std::endl;

			if (system->getName() == "")
			{
				system->setName(system_name);
			}

			if (!filename[0] == FileSystem::PATH_SEPARATOR)
			{
				system->setProperty("FROM_FILE", working_dir_ + FileSystem::PATH_SEPARATOR + filename);
			}
			else
			{
				system->setProperty("FROM_FILE", filename);
			}

			// notify tree of a new composite
			CompositeMessage* new_message = new CompositeMessage;
			new_message->setComposite(*system);
			new_message->setCompositeName(system_name);
			new_message->setType(CompositeMessage::NEW_COMPOSITE);
			notify_(new_message);

			return true;
		}


		void MolecularFileDialog::writePreferences(INIFile& inifile)
			throw()
		{
			inifile.insertValue("WINDOWS", "File::format", String(file_format_));
			inifile.insertValue("WINDOWS", "File::dialog_x", String(x_));
			inifile.insertValue("WINDOWS", "File::dialog_y", String(y_));
			inifile.insertValue("WINDOWS", "File::dialog_width", String(width_));
			inifile.insertValue("WINDOWS", "File::dialog_height", String(height_));
			inifile.insertValue("WINDOWS", "File::working_dir", working_dir_);
		}
		

		void MolecularFileDialog::fetchPreferences(INIFile& inifile)
			throw()
		{
			if (inifile.hasEntry("WINDOWS", "File::format"))
			{
				file_format_ = inifile.getValue("WINDOWS", "File::format").toInt();
			}

			if (inifile.hasEntry("WINDOWS", "File::dialog_x") &&
					inifile.hasEntry("WINDOWS", "File::dialog_y"))
			{
				x_ = inifile.getValue("WINDOWS", "File::dialog_x").toInt(),
				y_ = inifile.getValue("WINDOWS", "File::dialog_y").toInt();
			}

			if (inifile.hasEntry("WINDOWS", "File::dialog_width") &&
					inifile.hasEntry("WINDOWS", "File::dialog_height"))
			{
				width_ = inifile.getValue("WINDOWS", "File::dialog_width").toInt(),
				height_ = inifile.getValue("WINDOWS", "File::dialog_height").toInt();
			}

			if (inifile.hasEntry("WINDOWS", "File::working_dir"))
			{
				working_dir_ = inifile.getValue("WINDOWS", "File::working_dir");
			}
		}


		void MolecularFileDialog::checkMenuEntries()
			throw()
		{
			getMainControl()->menuBar()->setItemEnabled(save_id_, getMainControl()->getSelectedSystem());
		}

} } //namespaces
