// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: mainframe.h,v 1.56 2003/11/15 12:53:57 oliver Exp $
//

#ifndef BALL_APPLICATIONS_MOLVIEW_MAINFRAME_H
#define BALL_APPLICATIONS_MOLVIEW_MAINFRAME_H

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

#ifndef BALL_VIEW_WIDGETS_MOLECULARPROPERTIES_H
# include <BALL/VIEW/WIDGETS/molecularProperties.h>
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

#ifndef BALL_VIEW_DIALOGS_AMBERMINIMIZATIONDIALOG_H
# include <BALL/VIEW/DIALOGS/amberMinimizationDialog.h>
#endif

#ifndef BALL_VIEW_DIALOGS_MOLECULARDYNAMICSDIALOG_H
# include <BALL/VIEW/DIALOGS/molecularDynamicsDialog.h>
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

		/** This class is only intended for usage with multithreading.
				It notifies the Mainframe, that the thread for simulations has finished and can be deleted.
				This should only be used internaly.
		*/
		class SimulationThreadFinished
			: public QCustomEvent
		{
			public:
				SimulationThreadFinished()
					: QCustomEvent( SIMULATION_THREAD_FINISHED_EVENT ){}
		};

		///
		class SimulationOutput
			: public QCustomEvent
		{
			public:
				///
				SimulationOutput()
					: QCustomEvent( SIMULATION_OUTPUT_EVENT ){}

				///
				void setMessage(const String& msg) {message_ = msg;}

				///
				String getMessage() {return message_;}

			protected:
				String message_;
		};

		///
		class UpdateCompositeEvent
			: public QCustomEvent
		{
			public:
				///
				UpdateCompositeEvent()
					:QCustomEvent(UPDATE_COMPOSITE_EVENT),
					 composite_(0){}

				///
				void setComposite(const Composite* composite) { composite_ = composite;}
				
				///
				const Composite* getComposite() const { return composite_;}

			protected:
				const Composite* composite_;
		};


		enum MenuKey
		{
			MENU_EXPORT_POVRAYFILE = 20000,
			MENU_EXPORT_VRMLFILE,
			
			MENU_ASSIGN_CHARGES,
			MENU_AMBER_ENERGY,
			MENU_AMBER_MINIMIZATION,
			MENU_AMBER_MDSIMULATION,
			MENU_STOPSIMULATION,
			MENU_PEPTIDE,
			MENU_CALCULATE_HBONDS,
			MENU_FDPB,

			MENU_FULLSCREEN,
			MENU_CONTOUR_SURFACE,

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
		
		bool stopedSimulation() { return stop_simulation_;}

		void printAmberResults(const AmberFF& amber)
			throw();
				
		public slots:
		// active the menu entries
		// (connected to aboutToShow())
		void checkMenuEntries();

		void exportPOVRay();

		void exportVRML();

		// Build menu
		void assignCharges();
		void calculateAmberEnergy();
		void amberMinimization();
		void amberMDSimulation();
		void computeIsoContourSurface();
		void buildPeptide();
		void calculateHBonds();
		void stopSimulation();

		void toggleFullScreen();
		// Help menu
		void about();

		virtual void customEvent( QCustomEvent * e );

		/** Open a file.
				Calls MolecularFileDialog::openFile
		*/
		void openFile(const String& file)
			throw();

		private:

		Scene*										scene_;
		MolecularControl*					control_;
		DatasetControl* 					dataset_control_;
		GeometricControl*					geometric_control_;
		DisplayProperties*    		display_properties_;
		AmberMinimizationDialog*	minimization_dialog_;
		MolecularDynamicsDialog*	md_dialog_;
		ContourSurfaceDialog* 		surface_dialog_;
		LabelDialog*	    				label_dialog_;
		MolecularProperties*  		molecular_properties_;
		MolecularFileDialog*  		file_dialog_;
		FDPBDialog*  							FDPB_dialog_;
		Server*   								server_;

		LogView*									logview_;
		SimulationThread* 				simulation_thread_;
		
		bool 											fullscreen_;
		bool 											stop_simulation_;
	};

} // namespace BALL

#endif // BALL_APPLICATIONS_MOLVIEW_MAINFRAME_H
