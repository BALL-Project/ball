// $Id: openBruker1DFile.C,v 1.1 2001/01/29 17:07:30 anhi Exp $

#include <BALL/MOLVIEW/GUI/DIALOGS/openBruker1DFile.h>

using std::istream;
using std::ostream;

namespace BALL
{
  namespace MOLVIEW
  {
    OpenBruker1DFile::OpenBruker1DFile(QWidget* parent, const char* name)
      : FileDialog("Import Bruker1D File", QFileDialog::ExistingFile, parent, name)
    {
      QStringList string_list;
      string_list = "Bruker1D-files (1r)";

      setFilters(string_list);
    }
  
    OpenBruker1DFile::~OpenBruker1DFile()
      throw()
    {
      #ifdef BALL_VIEW_DEBUG
      cout << "Destructing object " << (void *)this
	   << " of class " << RTTI::getName<OpenBruker1DFile>() << endl;
      #endif
    }

    void OpenBruker1DFile::initializeWidget(MainControl& main_control)
    {
      main_control.insertMenuEntry(MainControl::FILE_IMPORT, "Bruker&1D File", this,
				   SLOT(exec()),
				   CTRL+Key_1);
    }

    void OpenBruker1DFile::finalizeWidget(MainControl& main_control)
    {
      main_control.removeMenuEntry
	(MainControl::FILE_IMPORT, "Bruker&1D File", this,
	 SLOT(exec()),
	 CTRL+Key_1);
    }

    void OpenBruker1DFile::openFile_()
    {
      // notify the main window
      WindowMessage window_message;
      window_message.setStatusBar("reading Bruker1D file...");
      notify_(window_message);

      // reading the file
      Bruker1D *myfile = new Bruker1D();
      
      String mydir = dirPath().latin1();

      try
      {
	myfile->read(mydir);
      }
      catch(...)
      {
	Log.info() << "> read Bruker1D file failed." << endl;
	delete myfile;

	return;
      }

      // writing to log
      Log.info() << "> Bruker file " << mydir << " succesfully read." << endl;

      // notify main window
      NewRegularData1DMessage new_message;
      new_message.setComposite((Composite *)myfile->GetData());
      new_message.setCompositeName(mydir);

      notify_(new_message);

      // notify main window
      window_message.setStatusBar("");
      notify_(window_message);
    }
  }
}
