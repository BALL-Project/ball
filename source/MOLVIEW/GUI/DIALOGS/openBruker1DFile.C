// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: openBruker1DFile.C,v 1.9 2003/06/02 14:34:28 oliver Exp $

#include <BALL/MOLVIEW/GUI/DIALOGS/openBruker1DFile.h>
#include <BALL/FORMAT/bruker1DFile.h>

namespace BALL
{
	using namespace VIEW;

  namespace MOLVIEW
  {
    OpenBruker1DFile::OpenBruker1DFile(QWidget* parent, const char* name)
      : FileDialog("Import Bruker1DFile File", QFileDialog::ExistingFile, parent, name)
    {
      QStringList string_list;
      string_list = "Bruker1DFile-files (1r)";

      setFilters(string_list);
    }
  
    OpenBruker1DFile::~OpenBruker1DFile()
      throw()
    {
      #ifdef BALL_VIEW_DEBUG
      Log.info() << "Destructing object " << (void *)this << " of class " << RTTI::getName<OpenBruker1DFile>() << endl;
      #endif
    }

    void OpenBruker1DFile::initializeWidget(MainControl& main_control)
  		throw()
    {
      main_control.insertMenuEntry(MainControl::FILE_IMPORT, "Bruker&1D File", this, SLOT(exec()), CTRL+Key_1);
    }

    void OpenBruker1DFile::finalizeWidget(MainControl& main_control)
		  throw()
    {
      main_control.removeMenuEntry (MainControl::FILE_IMPORT, "Bruker&1D File", this, SLOT(exec()), CTRL+Key_1);
		}

	  void OpenBruker1DFile::openFile_()
		  throw()
    {
      setStatusbarText("reading Bruker1DFile file...");

      // reading the file
      Bruker1DFile *myfile = new Bruker1DFile();
      
      String mydir = dirPath().latin1();

      try
      {
			  myfile->read(mydir);
      }
      catch(...)
      {
				Log.info() << "> read Bruker1DFile file failed." << std::endl;
			  delete myfile;
			  return;
      }

      // writing to log
			Log.info() << "> Bruker file " << mydir << " succesfully read." << std::endl;
      setStatusbarText(String("Read 1D NMR spectrum from ") + mydir);
    }
  }
}
