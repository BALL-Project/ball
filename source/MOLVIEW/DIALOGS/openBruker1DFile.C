// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: openBruker1DFile.C,v 1.2 2003/08/26 09:17:57 oliver Exp $

#include <BALL/MOLVIEW/DIALOGS/openBruker1DFile.h>
#include <BALL/FORMAT/bruker1DFile.h>
#include <BALL/VIEW/GUI/KERNEL/mainControl.h>

namespace BALL
{
	using namespace VIEW;

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
      Log.info() << "Destructing object " << (void *)this << " of class " << RTTI::getName<OpenBruker1DFile>() << std::endl;
      #endif
    }

    void OpenBruker1DFile::initializeWidget(MainControl& main_control)
  		throw()
    {
      main_control.insertMenuEntry(MainControl::FILE_IMPORT, "Bruker&1D File", this, SLOT(exec()), ALT+Key_1);
    }

    void OpenBruker1DFile::finalizeWidget(MainControl& main_control)
		  throw()
    {
      main_control.removeMenuEntry (MainControl::FILE_IMPORT, "Bruker&1D File", this, SLOT(exec()), ALT+Key_1);
		}

	  void OpenBruker1DFile::openFile_()
		  throw()
    {
      setStatusbarText("reading Bruker1D file...");

      // reading the file
      Bruker1D *myfile = new Bruker1D();
      
      String mydir = dirPath().latin1();

      try
      {
			  myfile->read(mydir);
      }
      catch(...)
      {
				Log.info() << "> read Bruker1D file failed." << std::endl;
			  delete myfile;
			  return;
      }

      // writing to log
			Log.info() << "> Bruker file " << mydir << " succesfully read." << std::endl;

      // notify main window
      NewRegularData1DMessage new_message;
      new_message.setComposite((Composite *)myfile->getData());
      new_message.setCompositeName(mydir);

      notify_(new_message);

      setStatusbarText("");
    }
  }
}
