// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: mainframe.h,v 1.4 2004/07/25 20:44:50 amoll Exp $
//

#ifndef BALL_APPLICATIONS_BALLVIEW_MAINFRAME_H
#define BALL_APPLICATIONS_BALLVIEW_MAINFRAME_H

#ifndef BALL_VIEW_WIDGETS_LOGVIEW_H
#	include <BALL/VIEW/WIDGETS/logView.h>
#endif

#ifndef BALL_VIEW_KERNEL_MAINCONTROL_H
# include <BALL/VIEW/KERNEL/mainControl.h>
#endif

#ifndef BALL_VIEW_KERNEL_SERVER_H
# include <BALL/VIEW/KERNEL/server.h>
#endif

#ifndef BALL_VIEW_WIDGETS_SCENE_H
# include <BALL/VIEW/WIDGETS/scene.h>
#endif

#ifndef BALL_VIEW_DIALOGS_DOWNLOADPDBFILE_H
# include <BALL/VIEW/DIALOGS/downloadPDBFile.h>
#endif

#ifndef BALL_VIEW_DIALOGS_DISPLAYPROPERTIES_H
# include <BALL/VIEW/DIALOGS/displayProperties.h>
#endif

#ifndef BALL_VIEW_DIALOGS_LABELDIALOG_H
# include <BALL/VIEW/DIALOGS/labelDialog.h>
#endif

#ifndef BALL_VIEW_WIDGETS_MOLECULARCONTROL_H
# include <BALL/VIEW/WIDGETS/molecularControl.h>
#endif

#ifndef BALL_VIEW_WIDGETS_DATASETCONTROL_H
# include <BALL/VIEW/WIDGETS/datasetControl.h>
#endif

#ifndef BALL_VIEW_WIDGETS_GEOMETRICCONTROL_H
# include <BALL/VIEW/WIDGETS/geometricControl.h>
#endif

#ifndef BALL_VIEW_WIDGETS_MOLECULARSTRUCTURE_H
# include <BALL/VIEW/WIDGETS/molecularStructure.h>
#endif

#ifndef BALL_VIEW_DIALOGS_FDPBDIALOG_H
# include <BALL/VIEW/DIALOGS/FDPBDialog.h>
#endif

#ifndef BALL_VIEW_DIALOGS_CONTOURSURFACEDIALOG_H
# include <BALL/VIEW/DIALOGS/contourSurfaceDialog.h>
#endif

#ifndef BALL_VIEW_DIALOGS_MOLECULARFILEDIALOG_H
# include <BALL/VIEW/DIALOGS/molecularFileDialog.h>
#endif

#include <qwidget.h>

namespace BALL
{
	using namespace BALL::VIEW;
	class AmberFF;
	class SimulationThread;

	class Mainframe	
		: public BALL::VIEW::MainControl
	{
		Q_OBJECT

		public:

		BALL_EMBEDDABLE(Mainframe, MainControl)

		Mainframe(QWidget* parent = 0, const char* name = 0);

		virtual ~Mainframe() throw();


		public slots:

		void exportPOVRay();

		void exportVRML();

		void printScene();

		/// Catch key events
		void keyPressEvent(QKeyEvent* e);
			
		// Build menu
		void computeIsoContourSurface();
		void toggleFullScreen();

		// Help menu
		void about();

		///
		virtual void checkMenus();

		/** Open a file.
				Calls MolecularFileDialog::openFile
		*/
		void openFile(const String& file) throw();

		///
		void saveBALLViewProjectFile() throw();

		///
		void loadBALLViewProjectFile() throw();
		
		///
		void loadBALLViewProjectFile(const String& filename) throw();

		protected:

		void setSelection_(Composite* c, HashSet<Position>& hash_set, Position& current)
			throw();

		Scene*										scene_;
		MolecularControl*					control_;
		DatasetControl* 					dataset_control_;
		GeometricControl*					geometric_control_;
		DisplayProperties*    		display_properties_;
		ContourSurfaceDialog* 		surface_dialog_;
		LabelDialog*	    				label_dialog_;
		MolecularStructure*  			molecular_structure_;
		MolecularFileDialog*  		file_dialog_;
		DownloadPDBFile*					download_pdb_dialog_;
		FDPBDialog*  							FDPB_dialog_;
		Server*   								server_;
		LogView*									logview_;
		bool 											fullscreen_;

		Index menu_cs_, menu_FPDB_;
	};

} // namespace BALL

#endif // BALL_APPLICATIONS_BALLVIEW_MAINFRAME_H
