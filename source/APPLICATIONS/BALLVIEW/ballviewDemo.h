// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id:

#ifndef BALLVIEWDEMO_H
#define BALLVIEWDEMO_H

#ifndef BALL_VIEW_KERNEL_MODULARWIDGET_H
# include <BALL/VIEW/KERNEL/modularWidget.h>
#endif

#ifndef BALL_VIEW_KERNEL_MESSAGE_H
# include <BALL/VIEW/KERNEL/message.h>
#endif

#include "ballviewDemoData.h"

namespace BALL
{
	namespace VIEW
	{
		/**	Dialog for creating labels for a selection of molecular objects.
				The class  MolecularControl is responsible for creating such a selection.
 				The text and color of the labels can be changed.
				If this dialog is used, it should be created with MainControl as parent.
    		\ingroup  ViewDialogs
		*/
		class BALLViewDemo : 
			public BALLViewDemoData,
			public ModularWidget
		{
			Q_OBJECT
			BALL_EMBEDDABLE(BALLViewDemo,ModularWidget)
				
			public:
			
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
			BALLViewDemo(QWidget *parent = NULL, const char *name = NULL )
				throw();

			//@}
			/** @name Destructors 
			*/
			//@{

			/** Destructor.
			*/
			virtual ~BALLViewDemo()
				throw();
							
			//@}
			/**	@name	Accessors: inspectors and mutators 
			 */
			//@{

			/** Message handling method.
					Catches only ControlSelectionMessage from MolecularControl.
					If such a message is catched the apply button will be enabled and labels
					can be appended onto the selection.
					@param message the pointer to the message that should be processed
		  */
			virtual void onNotify(Message *message)
				throw();
							
			//@}
			
			public slots:
				
  		/** @name Public slots
	  	*/
			//@{
				
			/** Show and raise the dialog
			*/
			void show();
						
			//@}
			/** @name Protected slots
			*/
			//@{
				
			protected slots:
							
			///
			virtual void next();
						
			//@}
			
		private:
			
			void enableNextStep_();
			bool first_selection_;

			Position step_;
		};

} } // namespaces

#endif // BALL_VIEW_DIALOGS_BALLViewDemo_H
