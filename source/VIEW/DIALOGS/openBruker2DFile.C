// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: openBruker2DFile.C,v 1.3 2003/08/28 15:13:26 amoll Exp $

#include <BALL/VIEW/DIALOGS/openBruker2DFile.h>
#include <BALL/FORMAT/bruker2DFile.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <qfiledialog.h>
#include <qkeysequence.h>

namespace BALL
{
  namespace VIEW
  {
    OpenBruker2DFile::OpenBruker2DFile(QWidget* parent, const char* name)
      : QWidget(parent),
				ModularWidget(name)
    {
			hide();
			registerWidget(this);
    }
  
    OpenBruker2DFile::~OpenBruker2DFile()
      throw()
    {
      #ifdef BALL_VIEW_DEBUG
      Log.info() << "Destructing object " << (void *)this << " of class " 
								 << RTTI::getName<OpenBruker2DFile>() << endl;
      #endif
    }

    void OpenBruker2DFile::initializeWidget(MainControl& main_control)
  		throw()
    {
      main_control.insertMenuEntry(MainControl::FILE_IMPORT, 
								"Bruker&2D File", this, SLOT(openFile_()));
    }

    void OpenBruker2DFile::finalizeWidget(MainControl& main_control)
		  throw()
    {
      main_control.removeMenuEntry (MainControl::FILE_IMPORT, "Bruker&2D File", this, SLOT(exec()));
		}

	  void OpenBruker2DFile::openFile_()
		  throw()
    {
      QFileDialog* fd = new QFileDialog(this,"Import Bruker2DFile", true);
			fd->setMode(QFileDialog::ExistingFile);
      fd->setFilter("Bruker2DFile-files (2r)");
			if (!fd->exec()) return;
      setStatusbarText("reading Bruker2DFile file...");

      // reading the file
      Bruker2DFile *myfile = new Bruker2DFile();
      
      try
      {
			  myfile->read(fd->selectedFile().ascii());
      }
      catch(...)
      {
				Log.info() << "> read Bruker2DFile file failed." << std::endl;
			  delete myfile;
			  return;
      }

      // writing to log
      setStatusbarText(String("Read 2D NMR spectrum from ") + fd->selectedFile().ascii());

			/*
      // notify main window
      NewRegularData2DMessage new_message;
      new_message.setComposite((Composite*)new RegularData2D(myfile.getData()));
      new_message.setCompositeName(mydir);
      notify_(new_message);
			*/
    }
  }
}
