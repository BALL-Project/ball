// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: openBruker1DFile.C,v 1.3 2003/08/28 15:13:26 amoll Exp $

#include <BALL/VIEW/DIALOGS/openBruker1DFile.h>
#include <BALL/FORMAT/bruker1DFile.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <qfiledialog.h>
#include <qkeysequence.h>

namespace BALL
{
  namespace VIEW
  {
    OpenBruker1DFile::OpenBruker1DFile(QWidget* parent, const char* name)
      : QWidget(parent),
				ModularWidget(name)
    {
			hide();
			registerWidget(this);
    }
  
    OpenBruker1DFile::~OpenBruker1DFile()
      throw()
    {
      #ifdef BALL_VIEW_DEBUG
      Log.info() << "Destructing object " << (void *)this << " of class " 
								 << RTTI::getName<OpenBruker1DFile>() << endl;
      #endif
    }

    void OpenBruker1DFile::initializeWidget(MainControl& main_control)
  		throw()
    {
      main_control.insertMenuEntry(MainControl::FILE_IMPORT, 
								"Bruker&1D File", this, SLOT(openFile_()));
    }

    void OpenBruker1DFile::finalizeWidget(MainControl& main_control)
		  throw()
    {
      main_control.removeMenuEntry (MainControl::FILE_IMPORT, "Bruker&1D File", this, SLOT(exec()));
		}

	  void OpenBruker1DFile::openFile_()
		  throw()
    {
      QFileDialog* fd = new QFileDialog(this,"Import Bruker1DFile", true);
			fd->setMode(QFileDialog::ExistingFile);
      fd->setFilter("Bruker1DFile-files (1r)");
			if (!fd->exec()) return;
      setStatusbarText("reading Bruker1DFile file...");

      // reading the file
      Bruker1DFile *myfile = new Bruker1DFile();
      
      try
      {
			  myfile->read(fd->selectedFile().ascii());
      }
      catch(...)
      {
				Log.info() << "> read Bruker1DFile file failed." << std::endl;
			  delete myfile;
			  return;
      }

      // writing to log
      setStatusbarText(String("Read 1D NMR spectrum from ") + fd->selectedFile().ascii());
    }
  }
}
