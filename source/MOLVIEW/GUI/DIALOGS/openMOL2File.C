// $Id: openMOL2File.C,v 1.1 2001/08/16 00:50:13 oliver Exp $

#include <BALL/MOLVIEW/GUI/DIALOGS/openMOL2File.h>

#include <BALL/VIEW/GUI/PRIMITIV/glsimpleBox.h>
#include <BALL/VIEW/GUI/PRIMITIV/gllabel.h>
#include <BALL/MATHS/box3.h>
#include <BALL/KERNEL/system.h>

using std::istream;
using std::ostream;
using std::endl;
using std::cerr;

namespace BALL
{

	using namespace BALL::VIEW;

	namespace MOLVIEW
	{
	  OpenMOL2File::OpenMOL2File(QWidget* parent, const char* name)
			throw()
			:	FileDialog("Import MOL2 file", QFileDialog::ExistingFile, parent, name)
		{
			QStringList string_list;
			string_list = "MOL2 files (*.mol2)";
			string_list += "All files (*.*)";
			
			setFilters(string_list);
		}
			
		OpenMOL2File::~OpenMOL2File()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<OpenMOL2File>() << endl;
			#endif 
		}

		void OpenMOL2File::initializeWidget(MainControl& main_control)
			throw()
		{
			main_control.insertMenuEntry
				(MainControl::FILE_IMPORT, "&MOL2 File", this,
				 SLOT(exec()), 
				 CTRL+Key_H);   
		}
		
		void OpenMOL2File::finalizeWidget(MainControl& main_control)
			throw()
		{
			main_control.removeMenuEntry
				(MainControl::FILE_IMPORT, "&MOL2 File", this,
				 SLOT(exec()), 
				 CTRL+Key_H);   
		}

		void OpenMOL2File::openFile_()
			throw()
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
				Log.info() << "> system memory allocation failed." << endl;
				return;
			}

			try
			{
				MOL2File mol2_file(getFileName());
				
				mol2_file >> *system;
				
				mol2_file.close();
			}
			catch(...)
			{
				Log.info() << "> read MOL2 file failed." << endl;
				delete system;

				return;
			}

			// writing info to log
			Log.info() << "> read " << system->countAtoms() << " atoms from MOL2 file \"" 
								 << getFileName() << "\"" << endl;


			QString filename = getFileName().c_str();

			cerr << filename << endl;
			cerr << getPathName() << endl;

			// construct a name (the filename without the dir path)
			filename.remove(0, getPathName().length() + 1);

			cerr << filename << endl;

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

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/MOLVIEW/GUI/DIALOGS/openMOL2File.iC>
#		endif

	} // namespace MOLVIEW

} // namespace BALL
