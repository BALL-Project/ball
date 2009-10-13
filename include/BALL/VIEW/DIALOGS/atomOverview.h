// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_DIALOGS_ATOMOVERVIEW_H
#define BALL_VIEW_DIALOGS_ATOMOVERVIEW_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#ifndef BALL_CONCEPT_COMPOSITE_H
#	include <BALL/CONCEPT/composite.h>
#endif

#include <BALL/VIEW/UIC/ui_atomOverview.h>

namespace BALL
{
	class AtomContainer;

	namespace VIEW
	{
		class MainControl;

		/**	Dialog for creating labels for a selection of molecular objects.
				The class  MolecularControl is responsible for creating such a selection.
 				The text, color and font of the labels can be changed.
				If this dialog is used, it should be created with MainControl as parent.
    		\ingroup  ViewDialogs
		*/
		class BALL_VIEW_EXPORT AtomOverview 
			: public QDialog,
				public Ui_AtomOverviewData
		{
			Q_OBJECT
				
			public:

			///
			class BALL_VIEW_EXPORT OverviewProcessor
				: public UnaryProcessor<Composite>
			{
				public:

					BALL_CREATE(OverviewProcessor)
				
					///
					OverviewProcessor();

					///
					OverviewProcessor(const OverviewProcessor& model_processor);

					///
					virtual ~OverviewProcessor();

					///
					Processor::Result operator() (Composite& composite);

					///
					void setTable(QTableWidget* widget) { table_ = widget;}

					///
					void showOnlySelection(bool state) {only_selection_ = state;}

				protected:

					QTableWidget* table_;
					bool 					only_selection_;
			};
	
			///
			class BALL_VIEW_EXPORT ApplyProcessor
				: public UnaryProcessor<Composite>
			{
				public:

					BALL_CREATE(ApplyProcessor)
				
					///
					ApplyProcessor();

					///
					ApplyProcessor(const ApplyProcessor& model_processor);

					///
					virtual ~ApplyProcessor();

					///
					Processor::Result operator() (Composite& composite);

					///
					void setTable(QTableWidget* widget) { table_ = widget; row_ = 0;}

					///
					void setMainControl(MainControl* main_control) {main_control_ = main_control;}

					///
					void showOnlySelection(bool state) {only_selection_ = state;}

				protected:

					QTableWidget* table_;
					Position row_;
					bool 					only_selection_;
					MainControl*  main_control_;
			};
			
		
			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					@param      parent the parent widget 
					@param 			name
			*/
			AtomOverview(QWidget *parent = NULL, const char *name = "AtomOverview");

			//@}
			/** @name Destructors 
			*/
			//@{

			/** Destructor.
			*/
			virtual ~AtomOverview();
							
			//@}
			/**	@name	Accessors: inspectors and mutators 
			 */
			//@{
			
			///
			void setParent(AtomContainer* ac);

			///
			void showOnlySelection(bool state) {only_selection_ = state;}

			//@}
			/** @name Protected slots
			*/
			//@{
				
			protected slots:
							
			/** Indicates the apply button was pressed.
			 		Creates a new Representation with the Label and notifies the widgets.
					@see  GeometricCenterProcessor
					@see  RepresentationMessage
			*/
			virtual void accept();

			void itemChanged(QTableWidgetItem* item);
			void itemActivated(QTableWidgetItem* item);

			protected:

			void restoreItem_(QTableWidgetItem* item);

			AtomContainer* parent_;
			OverviewProcessor processor_;
			ApplyProcessor 	  apply_processor_;
			String 						item_backup_;
			bool 							ignore_;
			bool 							only_selection_;
		};

} } // namespaces

#endif // BALL_VIEW_DIALOGS_AtomOverview_H
