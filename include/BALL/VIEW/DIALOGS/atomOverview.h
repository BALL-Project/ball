// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id:

#ifndef BALL_VIEW_DIALOGS_ATOMOVERVIEW_H
#define BALL_VIEW_DIALOGS_ATOMOVERVIEW_H

#include <BALL/VIEW/UIC/atomOverviewData.h>

#ifndef BALL_CONCEPT_COMPOSITE_H
#	include <BALL/CONCEPT/composite.h>
#endif

namespace BALL
{
	class AtomContainer;

	namespace VIEW
	{
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
					OverviewProcessor()
						throw();

					///
					OverviewProcessor(const OverviewProcessor& model_processor)
						throw();

					///
					virtual ~OverviewProcessor()
						throw();

					///
					Processor::Result operator() (Composite& composite);

					///
					void setTable(QTableWidget* widget) { table_ = widget;}

				protected:

					QTableWidget* table_;
			};
	
			///
			class BALL_VIEW_EXPORT ApplyProcessor
				: public UnaryProcessor<Composite>
			{
				public:

					BALL_CREATE(ApplyProcessor)
				
					///
					ApplyProcessor()
						throw();

					///
					ApplyProcessor(const ApplyProcessor& model_processor)
						throw();

					///
					virtual ~ApplyProcessor()
						throw();

					///
					Processor::Result operator() (Composite& composite);

					///
					void setTable(QTableWidget* widget) { table_ = widget; row_ = 0;}

				protected:

					QTableWidget* table_;
					Position row_;
			};
			
		
			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Calls  \link ModularWidget::registerWidget registerWidget \endlink .
					@param      parent the parent widget 
					@param 			name
					@see        QDialog
					@see        ModularWidget
			*/
			AtomOverview(QWidget *parent = NULL, const char *name = NULL )
				throw();

			//@}
			/** @name Destructors 
			*/
			//@{

			/** Destructor.
			*/
			virtual ~AtomOverview()
				throw();
							
			//@}
			/**	@name	Accessors: inspectors and mutators 
			 */
			//@{
			
			///
			void setParent(AtomContainer* ac);

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
		};

} } // namespaces

#endif // BALL_VIEW_DIALOGS_AtomOverview_H
