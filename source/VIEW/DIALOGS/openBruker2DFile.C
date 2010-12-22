// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/DIALOGS/openBruker2DFile.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/message.h>

#include <BALL/FORMAT/bruker2DFile.h>

#include <QtGui/QFileDialog>

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
    {
      #ifdef BALL_VIEW_DEBUG
				Log.info() << "Destructing object of class OpenBruker2DFile" << endl;
      #endif
    }

    void OpenBruker2DFile::initializeWidget(MainControl&)
    {
      insertMenuEntry(MainControl::FILE_IMPORT, tr("Bruker&2D File"), this, SLOT(openFile()));
    }

	  void OpenBruker2DFile::openFile()
    {
      Q3FileDialog* fd = new Q3FileDialog(this, tr("Import Bruker2DFile"), true);
			fd->setMode(Q3FileDialog::ExistingFile);
      fd->setFilter("Bruker2DFile-files (*.2r)");
			if (!fd->exec()) return;
      setStatusbarText((String)tr("reading Bruker2DFile file..."));

      // reading the file
      Bruker2DFile myfile;
      
      try
      {
			  myfile.read(fd->selectedFile().ascii());
      }
      catch(...)
      {
				Log.info() << "> " << (String)tr("read Bruker2DFile file failed.") << std::endl;
			  return;
      }

      setStatusbarText(String(tr("Read 2D NMR spectrum from")) + " " + fd->selectedFile().ascii());

			RegularData2D* data = new RegularData2D(myfile.getData());

      RegularData2DMessage* msg = new RegularData2DMessage(RegularData2DMessage::NEW);
			msg->setData(*data);
      notify_(msg);
    }
  }
}
