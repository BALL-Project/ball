// $Id: mainframe.h,v 1.7 2000/01/14 20:48:39 oliver Exp $

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
#include <qbutton.h>
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

#ifndef BALL_VIEW_OPENGL_KERNEL_SERVER_H
# include <BALL/VIEW/OPENGL/KERNEL/server.h>
#endif

#ifndef BALL_MOLVIEW_OPENGL_FUNCTOR_MOLECULEOBJECTPROCESSOR_H
# include <BALL/MOLVIEW/OPENGL/FUNCTOR/moleculeObjectProcessor.h>
#endif

#ifndef BALL_MOLVIEW_OPENGL_FUNCTOR_MOLECULEGLOBJECTCOLLECTOR_H
# include <BALL/MOLVIEW/OPENGL/FUNCTOR/moleculeGLObjectCollector.h>
#endif


#include "control.h"
#include "DlgDisplayProperties.h"
#include "DlgPreferences.h"

using namespace BALL;
using namespace BALL::VIEW;
using namespace BALL::MOLVIEW;


class Mainframe	
: public QWidget,
	public NotificationTarget<Server>
{
	Q_OBJECT

  public:

	  enum MenuKey
		{
			MENU__OPEN_FILE_PDB,
			MENU__OPEN_FILE_HIN,
			MENU__OPEN_FILE_MOL2,

			MENU__EXPORT_POVRAY,
			
			MENU__EDIT_CUT,
			MENU__EDIT_COPY,
			MENU__EDIT_PASTE,
			MENU__EDIT_DELETE,
			MENU__EDIT_SELECT,
			MENU__EDIT_DESELECT,
			MENU__EDIT_CLEAR_CLIPBOARD,

			MENU__BUILD_CHECK_RESIDUE,
			MENU__BUILD_BUILD_BONDS,
			MENU__BUILD_ADD_HYDROGENS,

			MENU__DISPLAY_OPEN_DISPLAY_PROPERTIES_DIALOG,
			MENU__DISPLAY_OPEN_PREFERENCES_DIALOG,
			MENU__DISPLAY_CENTER_CAMERA,

			MENU__HELP_ABOUT
		};
	

	  Mainframe
			(QWidget* parent = 0, const char* name = 0);

  	~Mainframe();

		Scene& getScene();

		MoleculeObjectProcessor& getObjectProcessor();
		
		void setPreferences(INIFile& inifile) const;

		void getPreferences(const INIFile& inifile);

		void addComposite(Composite*, QString*);

		void removeComposite(Composite*, bool delete_composite = true);


 protected:

		virtual bool onNotify(Server& server);


  public slots:
		
		// active the menu entries
		// (connected to aboutToShow())
		void checkMenuEntries();
		
		// File menu
	  void importPDB();
	  void importHIN();
		void exportPovray();

		// Edit menu
		void cut();
		void copy();
		void paste();
		void erase();
		void clearClipboard();
		void select();
		void deselect();

		// Build menu
		void checkResidue();
		void buildBonds();
		void addHydrogens();
		
    // Display menu
		void openDisplayPropertiesDialog();
		void openPreferencesDialog();
		void centerCamera();

		// Help menu
		void about();


		void applyDisplayPropertiesDialog();
		void applyPreferencesDialog();

		void startServer();
		void stopServer();
		void toggleServer();

		
  protected:
	  
	  void resizeEvent(QResizeEvent* event);

  private:
	
	  Scene*				scene_;

    MoleculeObjectProcessor		object_processor_;
		MoleculeGLObjectCollector GL_object_collector_;

		QMenuBar*			menubar_;
		QPopupMenu*		edit_menu_;
		QSplitter*		hor_splitter_;
		QSplitter*		vert_splitter_;
		Control*			control_;
		LogView*			logview_;
		QStatusBar*		statusbar_;
		QVBoxLayout*	vboxlayout_;
		
		DlgDisplayProperties	display_properties_dialog_;
		DlgPreferences				preferences_dialog_;

		List<QPopupMenu*> popup_menus_;

		INIFile				preferences_;

		FragmentDB		fragment_db_;
		
		List<Composite*>	selection_;
		List<Composite*>  copy_list_;
		List<Composite*>  cut_list_;

		QLabel*						server_icon_;
		QLabel*						tool_box_;
};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include "mainframe.iC"
#		endif

#endif // BALL_APPLICATIONS_MOLVIEW_MAINFRAME_H
