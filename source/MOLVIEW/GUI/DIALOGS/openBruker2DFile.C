// $Id: openBruker2DFile.C,v 1.5 2001/01/29 17:19:29 anhi Exp $

#include <BALL/MOLVIEW/GUI/DIALOGS/openBruker2DFile.h>

using std::istream;
using std::ostream;

namespace BALL
{
  namespace MOLVIEW
  {
    OpenBruker2DFile::OpenBruker2DFile(QWidget* parent, const char* name)
      :     FileDialog("Import Bruker2D File", QFileDialog::ExistingFile, parent, name)
    {
      QStringList string_list;
      string_list = "Bruker2D-files (2rr)";

      setFilters(string_list);
    }

    OpenBruker2DFile::~OpenBruker2DFile()
			throw()
    {
      #ifdef BALL_VIEW_DEBUG
      cout << "Destructing object " << (void *)this
	   << " of class " << RTTI::getName<OpenBruker2DFile>() << endl;
      #endif
    }

    void OpenBruker2DFile::initializeWidget(MainControl& main_control)
    {
      main_control.insertMenuEntry(MainControl::FILE_IMPORT, "Bruker&2D File", this,
				   SLOT(exec()),
				   CTRL+Key_2);
    }

    void OpenBruker2DFile::finalizeWidget(MainControl& main_control)
    {
      main_control.removeMenuEntry
	(MainControl::FILE_IMPORT, "Bruker&2D File", this,
	 SLOT(exec()),
	 CTRL+Key_2);
    }

    void OpenBruker2DFile::openFile_()
    {
      // notify the main window
      WindowMessage window_message;
      window_message.setStatusBar("reading Bruker2D file...");
      notify_(window_message);

      // reading the file
      Bruker2D *myfile = new Bruker2D();
      
      String mydir = dirPath().latin1();

      try
	{
	  myfile->read(mydir);
	  // TEST!!!
	  //	  myfile->save("/home/andreas/nmrtest", 0, 90, 500, 400);
	}
      catch(...)
	{
	  Log.info() << "> read Bruker2D file failed." << endl;
	  delete myfile;
	  
	  return;
	}

      // writing info to log
      Log.info() << "> Bruker file " << mydir << " succesfully read." << endl;
      
      // notify main window
      NewRegularData2DMessage new_message;
      new_message.setComposite((Composite *)myfile->GetData());
      new_message.setCompositeName(mydir);

      notify_(new_message);

      // notify main window
      window_message.setStatusBar("");
      notify_(window_message);
    }
  }
}
