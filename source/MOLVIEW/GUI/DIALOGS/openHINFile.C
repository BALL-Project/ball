// $Id: openHINFile.C,v 1.1 2000/09/23 15:39:12 hekl Exp $

#include <BALL/MOLVIEW/GUI/DIALOGS/openHINFile.h>

#include <BALL/VIEW/GUI/PRIMITIV/glsimpleBox.h>
#include <BALL/VIEW/GUI/PRIMITIV/gllabel.h>
#include <BALL/MATHS/box3.h>
#include <BALL/KERNEL/system.h>

using std::istream;
using std::ostream;

namespace BALL
{

	using namespace BALL::VIEW;

	namespace MOLVIEW
	{
	  OpenHINFile::OpenHINFile(QWidget* parent, const char* name)
			:	FileDialog("Import HIN file", QFileDialog::ExistingFile, parent, name)
		{
			QStringList string_list;
			string_list = "HIN files (*.hin)";
			string_list += "All files (*.*)";
			
			setFilters(string_list);
		}
			
		OpenHINFile::~OpenHINFile()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<OpenHINFile>() << endl;
			#endif 
		}

		void OpenHINFile::openFile_()
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
				Log.info() << "> system memory allocation failed." << endl;
				return;
			}

			try
			{
				HINFile hin_file(getFileName());
				
				hin_file >> *system;
				
				has_periodic_boundary = hin_file.hasPeriodicBoundary();
				bounding_box = hin_file.getPeriodicBoundary();

				hin_file.close();
			}
			catch(...)
			{
				Log.info() << "> read HIN file failed." << endl;
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
				Log.info() << "> creating bounding box (" 
									 << first << ", " << second << ")" 
									 << endl;

				simple_box->setVertex1(first);
				simple_box->setVertex2(second);
				simple_box->setColor("ff0000a0");
				simple_box->setName("BoundingBox");
		 		simple_box->setProperty(BALL::VIEW::GeometricObject::PROPERTY__OBJECT_TRANSPARENT);

				system->Composite::appendChild(*simple_box);
			}

			// writing info to log
			Log.info() << "> read " << system->countAtoms() << " atoms from HIN file \"" 
								 << getFileName() << "\"" << endl;


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

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/MOLVIEW/GUI/DIALOGS/openHINFile.iC>
#		endif

	} // namespace MOLVIEW

} // namespace BALL
