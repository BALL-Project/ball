// $Id: openMOLFile.C,v 1.1.2.3 2002/10/21 15:40:10 amoll Exp $

#include <BALL/MOLVIEW/GUI/DIALOGS/openMOLFile.h>
#include <BALL/KERNEL/system.h>
#include <BALL/FORMAT/MOLFile.h>

using std::endl;

namespace BALL
{
	using namespace BALL::VIEW;

	namespace MOLVIEW
	{
	  OpenMOLFile::OpenMOLFile(QWidget* parent, const char* name)
			throw()
			:	FileDialog("Import MOL file", QFileDialog::ExistingFile, parent, name)
		{
			QStringList string_list;
			string_list = "MOL files (*.mol)";
			string_list += "All files (*.*)";
			
			setFilters(string_list);
		}
			
		OpenMOLFile::~OpenMOLFile()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this << " of class " << RTTI::getName<OpenMOLFile>() << endl;
			#endif 
		}

		void OpenMOLFile::initializeWidget(MainControl& main_control)
			throw()
		{
			main_control.insertMenuEntry (MainControl::FILE_IMPORT, "&MOL File", this, SLOT(exec()), CTRL+Key_H);   
		}
		
		void OpenMOLFile::finalizeWidget(MainControl& main_control)
			throw()
		{
			main_control.removeMenuEntry (MainControl::FILE_IMPORT, "&MOL File", this, SLOT(exec()), CTRL+Key_H);   
		}

		void OpenMOLFile::openFile_()
			throw()
    {
			// notify the main window
			WindowMessage window_message;
			window_message.setStatusBar("reading MOL file...");

			notify_(window_message);

			// reading MOL File
			System *system = new System();
			
			// memory allocation failed ?
			if (system == 0)
			{
				Log.info() << "> system memory allocation failed." << endl;
				return;
			}

			try
			{
				MOLFile mol_file(getFileName());
				mol_file >> *system;
				mol_file.close();
			}
			catch(...)
			{
				Log.info() << "> read MOL file failed." << endl;
				delete system;
				return;
			}

			// writing info to log
			Log.info() << "> read " << system->countAtoms() << " atoms from MOL file \"" << getFileName() << "\"" << endl;

			QString filename = getFileName().c_str();

			// construct a name (the filename without the dir path)
			filename.remove(0, getPathName().length() + 1);

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

	} // namespace MOLVIEW

} // namespace BALL
