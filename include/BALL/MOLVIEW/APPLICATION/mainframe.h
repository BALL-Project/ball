// $Id: mainframe.h,v 1.1 1999/12/22 19:15:29 oliver Exp $

#ifndef BALL_MOLVIEW_APPLICATION_MAINFRAME_H
#define BALL_MOLVIEW_APPLICATION_MAINFRAME_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#include <qmetaobject.h>
#include <qwidget.h>
#include <qmenubar.h>
#include <qkeycode.h>
#include <qapplication.h>
#include <qmessagebox.h>
#include <qpopupmenu.h>
#include <qlayout.h>
#include <qtextview.h>
#include <qstring.h>
#include <qfiledialog.h>

#ifndef BALL_COMMON_LOGSTREAM_H
#	include <BALL/COMMON/logStream.h>
#endif

#ifndef BALL_CONCEPT_NOTIFICATION_H
#	include <BALL/CONCEPT/notification.h>
#endif

#ifndef BALL_FORMAT_HINFILE_H
#	include <BALL/FORMAT/HINFile.h>
#endif

#ifndef BALL_FORMAT_PDBFILE_H
#	include <BALL/FORMAT/PDBFile.h>
#endif

#ifndef BALL_VIEW_OPENGL_KERNEL_SCENE_H
# include <BALL/VIEW/OPENGL/KERNEL/scene.h>
#endif

#ifndef BALL_MOLVIEW_APPLICATION_CONTROL_H
# include <BALL/MOLVIEW/APPLICATION/control.h>
#endif

#ifndef BALL_MOLVIEW_OPENGL_FUNCTOR_MOLECULEOBJECTPROCESSOR_H
# include <BALL/MOLVIEW/OPENGL/FUNCTOR/moleculeObjectProcessor.h>
#endif

#ifndef BALL_MOLVIEW_OPENGL_FUNCTOR_MOLECULEGLOBJECTCOLLECTOR_H
# include <BALL/MOLVIEW/OPENGL/FUNCTOR/moleculeGLObjectCollector.h>
#endif


using namespace BALL;
using namespace BALL::VIEW;
using namespace BALL::MOLVIEW;


class Mainframe: public QWidget
{
	Q_OBJECT

  public:

	  Mainframe
			(QWidget *parent__pQWidget = 0,
			 const char *name__pc = 0);

  	~Mainframe
			(void);

		Scene &getScene
			(void);

		MoleculeObjectProcessor &getObjectProcessor
			(void);


  public slots:

	  void importPDB();
	  void importHIN();
		void about();

    
  protected:
	  
	  void resizeEvent(QResizeEvent *__pQResizeEvent);


  private:
	
		LogStream __mLogStream_;

	  Scene __mScene_;
    MoleculeObjectProcessor __mMoleculeObjectProcessor_;
		MoleculeGLObjectCollector __mMoleculeGLObjectCollector_;

		QMenuBar __mQMenuBar_;
		QVBoxLayout *__mpQVBoxLayout_;
		QHBoxLayout *__mpQHBoxLayout_;

		Control __mControl_;
		QTextView *__mpQTextView_;

		List<QPopupMenu *> popup_menus__mList_;
};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/MOLVIEW/APPLICATION/mainframe.iC>
#		endif

#endif // BALL_MOLVIEW_APPLICATION_MAINFRAME_H
