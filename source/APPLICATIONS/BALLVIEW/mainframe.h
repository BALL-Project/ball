// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: mainframe.h,v 1.12 2005/01/20 23:30:01 amoll Exp $
//

#ifndef BALL_APPLICATIONS_BALLVIEW_MAINFRAME_H
#define BALL_APPLICATIONS_BALLVIEW_MAINFRAME_H

#ifndef BALL_VIEW_KERNEL_MAINCONTROL_H
# include <BALL/VIEW/KERNEL/mainControl.h>
#endif

#ifndef BALL_VIEW_WIDGETS_SCENE_H
# include <BALL/VIEW/WIDGETS/scene.h>
#endif

#ifndef BALL_VIEW_DIALOGS_DISPLAYPROPERTIES_H
# include <BALL/VIEW/DIALOGS/displayProperties.h>
#endif

#ifndef BALL_VIEW_WIDGETS_DATASETCONTROL_H
# include <BALL/VIEW/WIDGETS/datasetControl.h>
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

		void printScene();

		/// Catch key events
		void keyPressEvent(QKeyEvent* e);
			
		void toggleFullScreen();

		// Help menu
		void about();

		/** Open a file.
				Calls MolecularFileDialog::openFile
		*/
		virtual void openFile(const String& file) throw();

		///
 		void saveBALLViewProjectFile() throw();

		///
		void loadBALLViewProjectFile() throw();
		
		protected:

		Scene*										scene_;
		DatasetControl* 					dataset_control_;
		DisplayProperties*    		display_properties_;
		MolecularFileDialog*  		file_dialog_;
		bool 											fullscreen_;
		QRect 										last_size_;
	};

} // namespace BALL

#endif // BALL_APPLICATIONS_BALLVIEW_MAINFRAME_H
