// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: mainframe.h,v 1.35 2003/06/05 10:29:14 amoll Exp $

#ifndef BALL_APPLICATIONS_MOLVIEW_MAINFRAME_H
#define BALL_APPLICATIONS_MOLVIEW_MAINFRAME_H

#ifndef BALL_VIEW_KERNEL_LOGVIEW_H
#	include <BALL/VIEW/KERNEL/logView.h>
#endif

#ifndef BALL_VIEW_GUI_KERNEL_MAINCONTROL_H
# include <BALL/VIEW/GUI/KERNEL/mainControl.h>
#endif

#ifndef BALL_VIEW_GUI_KERNEL_SERVER_H
# include <BALL/VIEW/GUI/KERNEL/server.h>
#endif

#ifndef BALL_VIEW_GUI_WIDGETS_SCENE_H
# include <BALL/VIEW/GUI/WIDGETS/scene.h>
#endif

#ifndef BALL_MOLVIEW_GUI_DIALOGS_DISPLAYPROPERTIES_H
# include <BALL/MOLVIEW/GUI/DIALOGS/displayProperties.h>
#endif

#ifndef BALL_MOLVIEW_GUI_DIALOGS_LABELPROPERTIES_H
# include <BALL/MOLVIEW/GUI/DIALOGS/labelProperties.h>
#endif

#ifndef BALL_MOLVIEW_GUI_WIDGETS_MOLECULARCONTROL_H
# include <BALL/MOLVIEW/GUI/WIDGETS/molecularControl.h>
#endif

#ifndef BALL_MOLVIEW_GUI_WIDGETS_MOLECULARPROPERTIES_H
# include <BALL/MOLVIEW/GUI/WIDGETS/molecularProperties.h>
#endif

#ifndef BALL_MOLVIEW_GUI_FUNCTOR_MOLECULEGLOBJECTCOLLECTOR_H
# include <BALL/MOLVIEW/GUI/FUNCTOR/moleculeGLObjectCollector.h>
#endif  

#ifndef BALL_VIEW_GUI_FUNCTOR_POVRENDERER_H
# include <BALL/VIEW/GUI/FUNCTOR/POVRenderer.h>
#endif

#ifndef BALL_MOLVIEW_GUI_DIALOGS_CONTOURSURFACEDIALOG_H
# include <BALL/MOLVIEW/GUI/DIALOGS/contourSurfaceDialog.h>
#endif

#ifndef BALL_VIEW_GUI_DIALOGS_MOLECULARFILEDIALOG_H
# include <BALL/MOLVIEW/GUI/DIALOGS/molecularFileDialog.h>
#endif

#ifndef BALL_VIEW_GUI_DIALOGS_FDPBDIALOG_H
# include <BALL/VIEW/GUI/DIALOGS/FDPBDialog.h>
#endif

#include "DIALOGS/DlgAmberMinimization.h"

class QWidget;
class QSplitter;
class QVBoxLayout;
class QPopupMenu;
class QLabel;

using namespace BALL;
using namespace BALL::VIEW;
using namespace BALL::MOLVIEW;


class Mainframe	
	: public BALL::VIEW::MainControl
{
	Q_OBJECT

	public:

	enum MenuKey
	{
		MENU__FILE_EXPORT_POVRAYFILE,
		
		MENU__EDIT_CUT,
		MENU__EDIT_COPY,
		MENU__EDIT_PASTE,
		MENU__EDIT_DELETE,
		MENU__EDIT_CLEAR_CLIPBOARD,

		MENU__INSERT_LABEL,

		MENU__BUILD_ASSIGN_CHARGES,
		MENU__BUILD_AMBER_ENERGY,
		MENU__BUILD_AMBER_MINIMIZATION,
		MENU__BUILD_AMBER_MDSIMULATION,

		MENU__DISPLAY_OPEN_DISPLAY_PROPERTIES_DIALOG,
    MENU__DISPLAY_OPEN_SURFACE_DIALOG,
		MENU__DISPLAY_OPEN_PREFERENCES_DIALOG,
		MENU__DISPLAY_CENTER_CAMERA,

		MENU__CONTROL_ROTATE_MODE,
		MENU__CONTROL_PICKING_MODE,

		MENU__HELP_ABOUT
	};


	Mainframe(QWidget* parent = 0, const char* name = 0);

	virtual ~Mainframe()
		throw();

	virtual void fetchPreferences(INIFile& inifile)
	throw();

	virtual void writePreferences(INIFile& inifile)
	throw();

	virtual void onNotify(Message *message)
	throw();
	

	public slots:
	// active the menu entries
	// (connected to aboutToShow())
	void checkMenuEntries();

	void exportPOVRay();

	// Build menu
	void assignCharges();
	void calculateAmberEnergy();
	void amberMinimization();
	void amberMDSimulation();
  void computeSurface();

	// Help menu
	void about();

  private:

	Scene*								scene_;
	MolecularControl*			control_;
	DisplayProperties*    display_properties_;
	DlgAmberMinimization*	minimization_dialog_;
  ContourSurfaceDialog* surface_dialog_;
	LabelProperties*	    label_properties_;
	MolecularProperties*  molecular_properties_;
	MolecularFileDialog*  file_dialog_;
	Server*   						server_;

	MoleculeGLObjectCollector		GL_object_collector_;

	FragmentDB		fragment_db_;
				
	QSplitter*		hor_splitter_;
	QSplitter*		vert_splitter_;
	LogView*			logview_;
	QVBoxLayout*	vboxlayout_;
	
	FDPBDialog*  					FDPB_dialog_;
	List<QPopupMenu*> popup_menus_;

	QLabel*						tool_box_;
};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include "mainframe.iC"
#		endif

#endif // BALL_APPLICATIONS_MOLVIEW_MAINFRAME_H
