// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/DIALOGS/molecularFileDialog.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/message.h>

#include <BALL/FORMAT/INIFile.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/FORMAT/HINFile.h>
#include <BALL/FORMAT/MOLFile.h>
#include <BALL/FORMAT/MOL2File.h>
#include <BALL/FORMAT/SDFile.h>
#include <BALL/FORMAT/antechamberFile.h>
#include <BALL/FORMAT/XYZFile.h>
#include <BALL/MATHS/simpleBox3.h>
#include <BALL/KERNEL/system.h>

#include <QtWidgets/QMenuBar>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QToolBar>

namespace BALL
{
	namespace VIEW
	{

MolecularFileDialog::MolecularFileDialog(QWidget *parent, const char* name)
	:	QWidget(parent),
		ModularWidget(name),
		save_id_(0),
		read_all_pdb_models_(false)
{
#ifdef BALL_VIEW_DEBUG
Log.error() << "new MolecularFileDialog " << this << std::endl;
#endif
	// register the widget with the MainControl
	registerWidget(this);
	setObjectName(name);
	hide();
}

MolecularFileDialog::~MolecularFileDialog()
{	
	#ifdef BALL_VIEW_DEBUG
	Log.info() << "Destructing object " << (void *)this 
						 << " of class MolecularFileDialog" << std::endl; 
	#endif 
}

void MolecularFileDialog::initializeWidget(MainControl&)
{
	String description = "Shortcut|File|Open_Structure";
	open_id_ = insertMenuEntry(MainControl::FILE_OPEN, tr("&Structure"), this, 
	                           SLOT(readFiles()), description, QKeySequence::Open,
														 tr("Open a molecular file (e.g. PDB,MOL2,SDF)"),
														 UIOperationMode::MODE_ADVANCED);
	setIcon(open_id_, "actions/document-open", true);

	description = "Shortcut|File|Save_Structure";
	save_id_ = insertMenuEntry(MainControl::FILE, tr("&Save Structure"), this, 
														 SLOT(writeFile()), description, QKeySequence::Save,
														 tr("Save the highlighted System (e.g. as PDB,MOL2,SDF file)"),
														 UIOperationMode::MODE_ADVANCED);
	setIcon(save_id_, "actions/document-save", true);
}

void MolecularFileDialog::readFiles()
{
	QStringList files = QFileDialog::getOpenFileNames(
											0,
											tr("Choose a molecular file to open"),
											getWorkingDir().c_str(),
											getSupportedFileFormatsList().c_str());

	for (QStringList::Iterator it = files.begin(); it != files.end(); ++it) 
	{
		// construct a name for the system(the filename without the dir path)
		openMolecularFile(ascii(*it));
	}
}

String MolecularFileDialog::getSupportedFileFormats() const
{
	return String("*.pdb *.brk *.ent *.hin *.mol *.mol2 *.sdf *.ac *.xyz");
}

String MolecularFileDialog::getSupportedFileFormatsList() const
{
	QString sl;
	sl += tr("Molecular files") + " (*.pdb *.brk *.ent *.hin *.mol *.mol2 *.sdf *.ac *.xyz);;";
	sl += "PDB " + tr("files")  + " (*.pdb *.brk *.ent);;";
	sl += "HIN " + tr("files")  + " (*.hin);;";
	sl += "MOL " + tr("files")  + " (*.mol);;";
	sl += "MOL2 "+ tr("files")  + " (*.mol2);;";
	sl += "SD "  + tr("files")  + " (*.sdf);;";
	sl += "AC "  + tr("files")  + " (*.ac);;";
	sl += "XYZ " + tr("files")  + " (*.xyz)";
	return (String)sl;
}

System* MolecularFileDialog::openMolecularFile(const String& file)
{
	vector<String> fields;
	String seperators(FileSystem::PATH_SEPARATOR);
	// workaround on windows: QT returns the filename in linux style
	// but I am not sure, if this will stay this way.
#ifdef BALL_OS_WINDOWS
	 seperators += "/";
#endif
	Position p = file.split(fields, seperators.c_str()) -1;
	String filename = fields[p];				
	setWorkingDirFromFilename_(file);
	String extension = fields[filename.split(fields, ".") -1];
	filename = filename.getSubstring(0, filename.size() - (extension.size() + 1));
	return openMolecularFile(file, extension, filename);
}


System* MolecularFileDialog::openMolecularFile(const String& filename, 
																	 const String& filetype, 
																	 const String& system_name)
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
		setStatusbarText((String)tr("Could not open file") + " " + filename, true);
		return 0;
	}

	// TODO substitue by GenericMolFile* file = MolFileFactory::open(filename);
	if (filetype.hasSubstring("pdb") || filetype.hasSubstring("PDB") ||
			filetype.hasSubstring("brk") || filetype.hasSubstring("BRK") ||
			filetype.hasSubstring("ent") || filetype.hasSubstring("ENT"))
	{
		return readPDBFile(filename, system_name);
	}
	else if (filetype.hasSubstring("HIN") ||
					 filetype.hasSubstring("hin"))
	{
		return readHINFile(filename, system_name);
	}
	else if (filetype.hasSubstring("MOL2") ||
					 filetype.hasSubstring("mol2"))
	{
		return readMOL2File(filename, system_name);
	}
	else if (filetype.hasSubstring("MOL") ||
					 filetype.hasSubstring("mol"))
	{
		return readMOLFile(filename, system_name);
	}
	else if (filetype.hasSubstring("SDF") ||
					 filetype.hasSubstring("sdf"))
	{
		return readSDFile(filename, system_name);
	}	
	else if (filetype.hasSubstring("AC") ||
					 filetype.hasSubstring("ac"))
	{
		return readACFile(filename, system_name);
	}
	else if (filetype.hasSubstring("XYZ") ||
					 filetype.hasSubstring("xyz"))
	{
		return readXYZFile(filename, system_name);
	}
	else
	{
		setStatusbarText(String(tr("Unknown filetype")) + ": " + filetype, true);
	}

	return 0;
}


bool MolecularFileDialog::writeFile()
{
	list<Composite*>& selection = getMainControl()->getMolecularControlSelection();

    if (selection.size() != 1 || !RTTI::isKindOf<System> (*selection.begin()))
	{
		setStatusbarText((String)tr("Not a single system selected! Aborting writing..."), true);
		return false;
	}

	const System& system = *(const System*) (*selection.begin());
	String file_name = getWorkingDir();
	file_name += FileSystem::PATH_SEPARATOR;
	file_name += system.getName();
	file_name += ".pdb";

	QString s = QFileDialog::getSaveFileName(
								0,
								tr("Choose a filename to save the selected system"),
								file_name.c_str(),
								getSupportedFileFormats().c_str());

	if (s == QString::null) return false;

	setStatusbarText((String)tr("writing file..."));

	String filename = ascii(s);
	setWorkingDirFromFilename_(filename);

	String filter(filename);
	while (filter.has(FileSystem::PATH_SEPARATOR))
	{
		filter = filter.after(FileSystem::PATH_SEPARATOR);
	}

	String filename_without_path = filter;

	while (filter.has('.'))
	{
		filter = filter.after(".");
	}

	bool result = false;
	if (filter == "PDB" || filter == "pdb" ||
			filter == "ent" || filter == "ENT" ||
			filter == "brk" || filter == "BRK")
	{
		result = writePDBFile(filename, system);
	}
	else if (filter.hasSubstring("HIN") || filter.hasSubstring("hin"))
	{
		result = writeHINFile(filename, system);
	}
	else if (filter.hasSubstring("MOL2") || filter.hasSubstring("mol2"))
	{
		result = writeMOL2File(filename, system);
	}
	else if (filter.hasSubstring("MOL") || filter.hasSubstring("mol"))
	{
		result = writeMOLFile(filename, system);
	}
	else if (filter.hasSubstring("SDF") || filter.hasSubstring("sdf"))
	{
		result = writeSDFile(filename, system);
	}	
	else if (filter.hasSubstring("AC") || filter.hasSubstring("ac"))
	{
		result = writeACFile(filename, system);
	}
	else if (filter.hasSubstring("XYZ") || filter.hasSubstring("xyz"))
	{
		result = writeXYZFile(filename, system);
	}
	else
	{
		if (filter == filename_without_path)
		{
			filename += ".pdb";
			result = writePDBFile(filename, system);
		}
		else
		{
			setStatusbarText((String)tr("Unknown file format, please set the file extension accordingly to type, aborting..."), true);
			return false;
		}
	}

	if (!result) 
	{
		return false;
	}

	setStatusbarText(String(system.countAtoms()) + " " + (String)tr("atoms written to file") + " \"" + filename + "\"", true);
	return true;
}

bool MolecularFileDialog::writePDBFile(String filename, const System& system)
{
	try
	{
		PDBFile file(filename, std::ios::out);
		file << system;
		file.close();
	}
	catch(Exception::GeneralException& e)
	{
		Log.error() << e << std::endl;
		setStatusbarText((String)tr("Writing of PDB file failed, see logs!"), true);
		return false;
	}

	return true;
}


bool MolecularFileDialog::writeHINFile(String filename, const System& system)
{
	try
	{
		HINFile file(filename, std::ios::out);
		file << system;
		file.close();
	}
	catch(Exception::GeneralException& e)
	{
		Log.error() << e << std::endl;
		setStatusbarText((String)tr("Writing of HIN file failed, see logs!"), true);
		return false;
	}

	return true;
}


bool MolecularFileDialog::writeMOLFile(String filename, const System& system)
{
	try
	{
		MOLFile file(filename, std::ios::out);
		file << system;
		file.close();
	}
	catch(Exception::GeneralException& e)
	{
		Log.error() << e << std::endl;
		setStatusbarText((String)tr("Writing of MOL file failed, see logs!"), true);
		return false;
	}

	return true;
}


bool MolecularFileDialog::writeMOL2File(String filename, const System& system)
{
	try
	{
		MOL2File file(filename, std::ios::out);
		file << system;
		file.close();
	}
	catch(Exception::GeneralException& e)
	{
		Log.error() << e << std::endl;
		setStatusbarText((String)tr("Writing of MOL2 file failed, see logs!"), true);
		return false;
	}

	return true;		
}

bool MolecularFileDialog::writeSDFile(String filename, const System& system)
{
	try
	{
		SDFile file(filename, std::ios::out);
		file << system;
		file.close();
	}
	catch(Exception::GeneralException& e)
	{
		Log.error() << e << std::endl;
		setStatusbarText((String)tr("Writing of SD file failed, see logs!"), true);
		return false;
	}

	return true;		
}

bool MolecularFileDialog::writeACFile(String filename, const System& system)
{
	try
	{
		AntechamberFile file(filename, std::ios::out);
		file << system;
		file.close();
	}
	catch(Exception::GeneralException& e)
	{
		Log.error() << e << std::endl;
		setStatusbarText((String)tr("Writing of AC file failed, see logs!"), true);
		return false;
	}

	return true;		
}

bool MolecularFileDialog::writeXYZFile(String filename, const System& system)
{
	try
	{
		XYZFile file(filename, std::ios::out);
		file << system;
		file.close();
	}
	catch(Exception::GeneralException& e)
	{
		Log.error() << e << std::endl;
		setStatusbarText((String)tr("Writing of XYZ file failed, see logs!"), true);
		return false;
	}

	return true;		
}


System* MolecularFileDialog::readPDBFile(String filename, String system_name)
{
	setStatusbarText((String)tr("reading PDB file..."), true);

	System* system = new System();

	try
	{
		PDBFile pdb_file(filename);
		if(read_all_pdb_models_) {
			pdb_file.selectAllModels();
		}
		pdb_file >> *system;
		pdb_file.close();
	}
	catch(Exception::GeneralException& e)
	{
		Log.error() << e << std::endl;
		setStatusbarText((String)tr("Reading of PDB file failed, see logs!"), true);
		delete system;
		return 0;
	}

	if (!finish_(filename, system_name, system)) return 0;
	return system;
}


System* MolecularFileDialog::readHINFile(String filename, String system_name)
{
	bool has_periodic_boundary = false;
	SimpleBox3 bounding_box;

	setStatusbarText((String)tr("reading HIN file..."), true);

	System* system = new System();

	try
	{
		HINFile hin_file(filename);

		hin_file >> *system;

		has_periodic_boundary = hin_file.hasPeriodicBoundary();
		bounding_box = hin_file.getPeriodicBoundary();

		hin_file.close();
	}
	catch(Exception::GeneralException& e)
	{
		Log.error() << e << std::endl;
		setStatusbarText((String)tr("Reading of HIN file failed, see logs!"), true);
		delete system;
		return 0;
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

	if (!finish_(filename, system_name, system)) return 0;
	return system;
}


System* MolecularFileDialog::readMOLFile(String filename, String system_name)
{
	setStatusbarText((String)tr("reading MOL file..."), true);

	System* system = new System();

	try
	{
		MOLFile mol_file(filename);
		mol_file >> *system;
		mol_file.close();
	}
	catch(Exception::GeneralException& e)
	{
		Log.error() << e << std::endl;
		setStatusbarText((String)tr("Reading of MOL file failed, see logs!"), true);
		delete system;
		return 0;
	}

	if (!finish_(filename, system_name, system)) return 0;
	return system;
}


System* MolecularFileDialog::readMOL2File(String filename, String system_name)
{
	setStatusbarText((String)tr("reading MOL2 file..."), true);

	System* system = new System();

	try
	{
		MOL2File mol2_file(filename);
		mol2_file >> *system;
		mol2_file.close();
	}
	catch(Exception::GeneralException& e)
	{
		setStatusbarText(tr("Reading of MOL2 file failed, see logs!"), true);
		Log.error() << e << std::endl;
		delete system;
		return 0;
	}

	if (!finish_(filename, system_name, system)) return 0;
	return system;
}


System* MolecularFileDialog::readSDFile(String filename, String system_name)
{
	setStatusbarText((String)tr("reading SD file..."), true);

	System* system = new System();

	try
	{
		SDFile sd_file(filename);
		sd_file >> *system;
		sd_file.close();
	}
	catch(Exception::GeneralException& e)
	{
		setStatusbarText((String)tr("Reading of SD file failed, see logs!"), true);
		delete system;
		return 0;
	}

	if (!finish_(filename, system_name, system)) return 0;
	return system;
}


System* MolecularFileDialog::readACFile(String filename, String system_name)
{
	setStatusbarText((String)tr("reading AC file..."), true);

	System* system = new System();

	try
	{
		AntechamberFile ac_file(filename);
		ac_file >> *system;
		ac_file.close();
	}
	catch(Exception::GeneralException& e)
	{
		setStatusbarText((String)tr("Reading of AC file failed, see logs!"), true);
		delete system;
		return 0;
	}

	if (!finish_(filename, system_name, system)) return 0;
	return system;
}

System* MolecularFileDialog::readXYZFile(String filename, String system_name)
{
	setStatusbarText((String)tr("reading XYZ file..."), true);

	System* system = new System();

	try
	{
		XYZFile sd_file(filename);
		sd_file >> *system;
		sd_file.close();
	}
	catch(Exception::GeneralException& e)
	{
		setStatusbarText((String)tr("Reading of XYZ file failed, see logs!"), true);
		delete system;
		return 0;
	}

	if (!finish_(filename, system_name, system)) return 0;
	return system;
}


bool MolecularFileDialog::finish_(const String& filename, const String& system_name, System* system)
{
	// writing info to log
	setStatusbarText(String(tr("Read")) + " " + String(system->countAtoms()) + " " +
			            (String)tr("atoms from file") + " \"" + filename + "\"", true);

	if (system->getName() == "")
	{
		system->setName(system_name);
	}

	if (filename[0] != FileSystem::PATH_SEPARATOR)
	{
		system->setProperty("FROM_FILE", getWorkingDir() + FileSystem::PATH_SEPARATOR + filename);
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


void MolecularFileDialog::checkMenu(MainControl& mc)
{
	bool busy = mc.compositesAreLocked();
	if (save_id_)
		save_id_->setEnabled(!busy && mc.getSelectedSystem());
	if (open_id_)
		open_id_->setEnabled(!busy);
}


MolecularFileDialog::MolecularFileDialog(const MolecularFileDialog& mfd)
	: QWidget(),
		ModularWidget(mfd)
{}

System* MolecularFileDialog::openFile_(String type)
{
	QStringList files = QFileDialog::getOpenFileNames(
											0,
											tr("Choose a molecular file to open"),
											getWorkingDir().c_str(),
											"*.*");

	System* system = 0;
	for (QStringList::Iterator it = files.begin(); it != files.end(); ++it) 
	{
		vector<String> fields;
		String seperators(FileSystem::PATH_SEPARATOR);
		// workaround on windows: QT returns the filename in linux style
		// but I am not sure, if this will stay this way.
#ifdef BALL_OS_WINDOWS
		 seperators += "/";
#endif
		String file = ascii(*it);
		Position p = file.split(fields, seperators.c_str()) -1;
		String filename = fields[p];				
		setWorkingDirFromFilename_(file);

		// construct a name for the system(the filename without the dir path)
		system = openMolecularFile(file, type, filename);
	}

	return system;
}

System* MolecularFileDialog::openPDBFile()
{
	return openFile_("PDB");
} 

System* MolecularFileDialog::openHINFile()
{
	return openFile_("HIN");
}

System* MolecularFileDialog::openMOLFile()
{
	return openFile_("MOL");
}

System* MolecularFileDialog::openMOL2File()
{
	return openFile_("MOL2");
}

System* MolecularFileDialog::openSDFile()
{
	return openFile_("SDF");
}

System* MolecularFileDialog::openACFile()
{
	return openFile_("AC");
}

System* MolecularFileDialog::openXYZFile()
{
	return openFile_("XYZ");
}

void MolecularFileDialog::onNotify(Message *message)
{
    if (RTTI::isKindOf<ControlSelectionMessage>(message))
	{
		checkMenu(*getMainControl());
	}
}

bool MolecularFileDialog::canHandle(const String& fileformat) const
{
	String formats = getSupportedFileFormats();
	vector<String> fields;
	formats.split(fields, "*. ");

	for (Position p = 0; p < fields.size(); p++)
	{
		String s = fields[p];
		if (s == fileformat) return true;
		s.toUpper();
		if (s == fileformat) return true;
	}

	return false;
}

bool MolecularFileDialog::openFile(const String& filename)
{
	return openMolecularFile(filename) != 0;
}


} } //namespaces
