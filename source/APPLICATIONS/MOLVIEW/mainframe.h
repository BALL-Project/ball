// $Id: mainframe.h,v 1.20 2000/07/17 21:27:38 oliver Exp $

#ifndef BALL_APPLICATIONS_MOLVIEW_MAINFRAME_H
#define BALL_APPLICATIONS_MOLVIEW_MAINFRAME_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#include <strstream>

#include <qmetaobject.h>
#include <qmainwindow.h>
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
#include <qlabel.h>
#include <qtooltip.h>

#ifndef BALL_FORMAT_INIFILE_H
#	include <BALL/FORMAT/INIFile.h>
#endif

#ifndef BALL_VIEW_KERNEL_LOGVIEW_H
#	include <BALL/VIEW/KERNEL/logView.h>
#endif

#ifndef BALL_VIEW_KERNEL_CONNECTIONOBJECT_H
#	include <BALL/VIEW/KERNEL/connectionObject.h>
#endif

#ifndef BALL_VIEW_OPENGL_KERNEL_MAINCONTROL_H
# include <BALL/VIEW/OPENGL/KERNEL/mainControl.h>
#endif

#ifndef BALL_VIEW_OPENGL_KERNEL_SERVER_H
# include <BALL/VIEW/OPENGL/KERNEL/server.h>
#endif

#ifndef BALL_VIEW_OPENGL_WIDGETS_SCENE_H
# include <BALL/VIEW/OPENGL/WIDGETS/scene.h>
#endif

#ifndef BALL_MOLVIEW_KERNEL_GEOMETRICCONVERTOR_H
# include <BALL/MOLVIEW/KERNEL/geometricConvertor.h>
#endif

#ifndef BALL_MOLVIEW_OPENGL_WIDGETS_MOLECULARCONTROL_H
# include <BALL/MOLVIEW/OPENGL/WIDGETS/molecularControl.h>
#endif

#ifndef BALL_MOLVIEW_OPENGL_WIDGETS_MOLECULARPROPERTIES_H
# include <BALL/MOLVIEW/OPENGL/WIDGETS/molecularProperties.h>
#endif

#ifndef BALL_MOLVIEW_OPENGL_WIDGETS_OPENHINFILE_H
# include <BALL/MOLVIEW/OPENGL/WIDGETS/openHINFile.h>
#endif

#ifndef BALL_MOLVIEW_OPENGL_WIDGETS_OPENPDBFILE_H
# include <BALL/MOLVIEW/OPENGL/WIDGETS/openPDBFile.h>
#endif

#ifndef BALL_MOLVIEW_OPENGL_FUNCTOR_MOLECULEGLOBJECTCOLLECTOR_H
# include <BALL/MOLVIEW/OPENGL/FUNCTOR/moleculeGLObjectCollector.h>
#endif  

#ifndef BALL_MOLVIEW_OPENGL_FUNCTOR_MOLECULEOBJECTPROCESSOR_H
# include <BALL/MOLVIEW/OPENGL/FUNCTOR/moleculeObjectProcessor.h>
#endif  

#include "DIALOGS/DlgDisplayProperties.h"
#include "DIALOGS/DlgPreferences.h"
#include "DIALOGS/DlgAmberMinimization.h"
#include "DIALOGS/DlgLabelProperties.h"

using namespace BALL;
using namespace BALL::VIEW;
using namespace BALL::MOLVIEW;


class Mainframe	
	: public BALL::VIEW::MainControl
{
	Q_OBJECT


  public:

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

		MENU__INSERT_LABEL,

		MENU__BUILD_CHECK_RESIDUE,
		MENU__BUILD_BUILD_BONDS,
		MENU__BUILD_ADD_HYDROGENS,
		MENU__BUILD_ASSIGN_CHARGES,
		MENU__BUILD_AMBER_ENERGY,
		MENU__BUILD_AMBER_MINIMIZATION,

		MENU__DISPLAY_OPEN_DISPLAY_PROPERTIES_DIALOG,
		MENU__DISPLAY_OPEN_PREFERENCES_DIALOG,
		MENU__DISPLAY_CENTER_CAMERA,

		MENU__CONTROL_ROTATE_MODE,
		MENU__CONTROL_PICKING_MODE,

		MENU__HELP_ABOUT
	};


	Mainframe
		(QWidget* parent = 0, const char* name = 0);

	~Mainframe();


	void setPreferences(INIFile& inifile) const;

	void getPreferences(const INIFile& inifile);

	virtual void onNotify(Message *message);

	

	public slots:
	// active the menu entries
	// (connected to aboutToShow())
	void checkMenuEntries();

	// active the menu entries
	// File menu
	void importHIN();
	void importPDB();

	// Edit menu
	void cut();
	void copy();
	void paste();
	void clearClipboard();
	void select();
	void deselect();

	// insert menu
	void insertLabel();

	// Control menu
	void rotateMode();
	void pickingMode();

	// Build menu
	void checkResidue();
	void buildBonds();
	void addHydrogens();
	void assignCharges();
	void calculateAmberEnergy();
	void amberMinimization();

	// Display menu
	void openDisplayPropertiesDialog();
	void openPreferencesDialog();
	void centerCamera();

	// Help menu
	void about();

	// old style, will be replaced by connectionObject -Notification		
	void applyPreferencesDialog();

	void startServer();
	void stopServer();
	void checkServer();


  private:

	Scene*								scene_;
	MolecularControl*			control_;
	DlgDisplayProperties* display_properties_;
	DlgPreferences*				preferences_dialog_;
	DlgAmberMinimization*	minimization_dialog_;
	DlgLabelProperties*	  label_properties_;
	OpenHINFile*					open_hin_file_;
	OpenPDBFile*					open_pdb_file_;
	MolecularProperties*  molecular_properties_;
	Server*   						server_;
	GeometricConvertor*   geometric_convertor_;

	MoleculeGLObjectCollector		GL_object_collector_;
	MoleculeObjectProcessor			object_processor_;

	INIFile				preferences_;

	FragmentDB		fragment_db_;
				
	QSplitter*		hor_splitter_;
	QSplitter*		vert_splitter_;
	LogView*			logview_;
	QVBoxLayout*	vboxlayout_;
	
	List<QPopupMenu*> popup_menus_;
	List<Composite*>  selection_;
	List<Composite*>  copy_list_;

	QLabel*						server_icon_;
	QLabel*						tool_box_;

	bool rotate_mode_;
};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include "mainframe.iC"
#		endif

#endif // BALL_APPLICATIONS_MOLVIEW_MAINFRAME_H
