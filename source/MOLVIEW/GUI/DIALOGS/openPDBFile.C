// $Id: openPDBFile.C,v 1.5 2001/05/13 16:28:12 hekl Exp $

#include <BALL/MOLVIEW/GUI/DIALOGS/openPDBFile.h>

using std::istream;
using std::ostream;
using std::cerr;
using std::endl;

namespace BALL
{

	namespace MOLVIEW
	{
	  OpenPDBFile::OpenPDBFile(QWidget* parent, const char* name)
			throw()
			:	FileDialog("Import PDB file", QFileDialog::ExistingFile, parent, name)
		{
			QStringList string_list;
			string_list = "PDB files (*.pdb)";
			string_list += "PDB files (*.brk)";
			string_list += "PDB files (*.ent)";
			string_list += "All files (*.*)";
			
			setFilters(string_list);
		}
			
		OpenPDBFile::~OpenPDBFile()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<OpenPDBFile>() << endl;
			#endif 
		}

		void OpenPDBFile::initializeWidget(MainControl& main_control)
			throw()
		{
			main_control.insertMenuEntry
				(MainControl::FILE_IMPORT, "&PDB File", this,
				 SLOT(exec()), 
				 CTRL+Key_P);   
		}
		
		void OpenPDBFile::finalizeWidget(MainControl& main_control)
			throw()
		{
			main_control.removeMenuEntry
				(MainControl::FILE_IMPORT, "&PDB File", this,
				 SLOT(exec()), 
				 CTRL+Key_P);   
		}

		void OpenPDBFile::openFile_()
			throw()
    {
			// notify the main window
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
				PDBFile pdb_file(getFileName());
				
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
			Log.info() << "> read " << system->countAtoms() << " atoms from PDB file \"" 
								 << getFileName() << "\"" << endl;


			QString filename = getFileName().c_str();

			// construct a name (the filename without the dir path)
			filename.remove(0, getPathName().length() + 1);

			if (filename.find('.') != -1)
			{
				filename = filename.left(filename.find('.'));
			}

			// notify main window
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

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/MOLVIEW/GUI/DIALOGS/openPDBFile.iC>
#		endif

	} // namespace MOLVIEW

} // namespace BALL
