// $Id: mainframe.h,v 1.6 2000/01/12 17:41:55 oliver Exp $

#ifndef BALL_APPLICATIONS_MOLVIEW_MAINFRAME_H
#define BALL_APPLICATIONS_MOLVIEW_MAINFRAME_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#include <strstream>

#include <qmetaobject.h>
#include <qwidget.h>
#include <qmenubar.h>
#include <qkeycode.h>
#include <qapplication.h>
#include <qmessagebox.h>
#include <qpopupmenu.h>
#include <qlayout.h>
#include <qstring.h>
#include <qsplitter.h>
#include <qstatusbar.h>

#ifndef BALL_CONCEPT_NOTIFICATION_H
#	include <BALL/CONCEPT/notification.h>
#endif

#ifndef BALL_FORMAT_HINFILE_H
#	include <BALL/FORMAT/HINFile.h>
#endif

#ifndef BALL_FORMAT_INIFILE_H
#	include <BALL/FORMAT/INIFile.h>
#endif

#ifndef BALL_FORMAT_PDBFILE_H
#	include <BALL/FORMAT/PDBFile.h>
#endif

#ifndef BALL_KERNEL_BASEFRAGMENT_H
#	include <BALL/KERNEL/baseFragment.h>
#endif

#ifndef BALL_VIEW_KERNEL_LOGVIEW_H
#	include <BALL/VIEW/KERNEL/logView.h>
#endif

#ifndef BALL_VIEW_OPENGL_KERNEL_SCENE_H
# include <BALL/VIEW/OPENGL/KERNEL/scene.h>
#endif

#ifndef BALL_APPLICATIONS_MOLVIEW_CONTROL_H
# include "control.h"
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


class Mainframe	
	: public QWidget
{
	Q_OBJECT

  public:

	  enum
		{
			MENU__CHECK_RESIDUE   = 1,
			MENU__CUT             = 2,
			MENU__COPY            = 3,
			MENU__PASTE           = 4,
			MENU__BUILD_BONDS     = 5,
			MENU__REMOVE_BONDS    = 6,
			MENU__SELECT          = 7,
			MENU__DESELECT        = 8,
			MENU__CENTER_CAMERA   = 9,
			MENU__OPEN_DISPLAY    = 10,
			MENU__CLEAR_CLIPBOARD = 11
		};
	

	  Mainframe
			(QWidget *parent__pQWidget = 0,
			 const char *name__pc = 0);

  	~Mainframe
			(void);

		Scene &getScene
			(void);

		MoleculeObjectProcessor &getObjectProcessor
			(void);
		
		void setPreferences(INIFile& inifile) const;
		void getPreferences(const INIFile& inifile);
		

  public slots:

	  void importPDB();
	  void importHIN();

		void exportPovray();

		void about();

		void updateEditMenuFromSelection(bool selected__bool);
		void updateEditMenuFromCutOrCopy(bool copied__bool);
    
  protected:
	  
	  void resizeEvent(QResizeEvent *__pQResizeEvent);


  private:
	
	  Scene*				scene_;

    MoleculeObjectProcessor __mMoleculeObjectProcessor_;
		MoleculeGLObjectCollector __mMoleculeGLObjectCollector_;

		QMenuBar*			menubar_;
		QPopupMenu*		edit__mpQPopupMenu_;
		QSplitter*		hor_splitter_;
		QSplitter*		vert_splitter_;
		Control*			control_;
		LogView*			logview_;
		QStatusBar*		statusbar_;
		QVBoxLayout*	vboxlayout_;
	

		List<QPopupMenu*> popup_menus__mList_;

		INIFile				preferences_;
};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include "mainframe.iC"
#		endif

#endif // BALL_APPLICATIONS_MOLVIEW_MAINFRAME_H
