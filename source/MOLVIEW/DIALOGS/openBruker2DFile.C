// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: openBruker2DFile.C,v 1.2 2003/08/26 09:17:58 oliver Exp $

#include <BALL/MOLVIEW/DIALOGS/openBruker2DFile.h>
#include <BALL/FORMAT/bruker2DFile.h>
#include <BALL/VIEW/GUI/WIDGETS/regularData2DWidget.h>
#include <BALL/VIEW/GUI/KERNEL/mainControl.h>

namespace BALL
{
	using namespace VIEW;

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
      cout << "Destructing object " << (void *)this << " of class " << RTTI::getName<OpenBruker2DFile>() << std::endl;
      #endif
    }

    void OpenBruker2DFile::initializeWidget(MainControl& main_control)
			throw()
    {
      main_control.insertMenuEntry(MainControl::FILE_IMPORT, "Bruker&2D File", this, SLOT(exec()), ALT+Key_2);
    }

    void OpenBruker2DFile::finalizeWidget(MainControl& main_control)
			throw()
    {
      main_control.removeMenuEntry (MainControl::FILE_IMPORT, "Bruker&2D File", this, SLOT(exec()), ALT+Key_2);
    }

    void OpenBruker2DFile::openFile_()
			throw()
    {
      // notify the main window
			MainControl::getMainControl(this)->setStatusbarText("reading Bruker2D file...");

      // reading the file
      Bruker2D *myfile = new Bruker2D();
      
      String mydir = dirPath().latin1();

      try
			{
				 myfile->read(mydir);
			}
      catch(...)
			{
				 Log.info() << "> read Bruker2D file failed." << std::endl;
				 delete myfile;
				 return;
			}

      // writing info to log
      Log.info() << "> Bruker file " << mydir << " succesfully read." << std::endl;
      
      // notify main window
      NewRegularData2DMessage new_message;
      new_message.setComposite((Composite *)myfile->GetData());
      new_message.setCompositeName(mydir);

      notify_(new_message);

      // notify main window
			MainControl::getMainControl(this)->setStatusbarText("");
    }
  }
}
