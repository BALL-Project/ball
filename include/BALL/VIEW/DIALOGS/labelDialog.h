// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id:

#ifndef BALL_VIEW_DIALOGS_LABELDIALOG_H
#define BALL_VIEW_DIALOGS_LABELDIALOG_H

#ifndef BALL_FORMAT_INIFILE_H
#	include <BALL/FORMAT/INIFile.h>
#endif

#ifndef BALL_VIEW_KERNEL_MODULARWIDGET_H
# include <BALL/VIEW/KERNEL/modularWidget.h>
#endif

#ifndef BALL_VIEW_KERNEL_MESSAGE_H
# include <BALL/VIEW/KERNEL/message.h>
#endif

#include <BALL/VIEW/UIC/labelDialogData.h>

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
		class BALL_EXPORT LabelDialog : 
			public LabelDialogData,
			public ModularWidget
		{
			Q_OBJECT
			BALL_EMBEDDABLE(LabelDialog,ModularWidget)
				
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
			LabelDialog(QWidget *parent = NULL, const char *name = NULL )
				throw();

			//@}
			/** @name Destructors 
			*/
			//@{

			/** Destructor.
			*/
			virtual ~LabelDialog()
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
			/**	ModularWidget methods
			*/
			//@{
			
			/** Fetches the preferences (the position and the selected color of {\em *this}
					labelDialog) from the	 INIFile.
					@see    writePreferences
			*/
			virtual void fetchPreferences(INIFile &inifile)
				throw();
			
			/** Writes the preferences (the position and the selected color) to the	INIFile.
					This method will be called inside the method  MainControl::aboutToExit 
					@see    fetchPreferences
			*/
			virtual void writePreferences(INIFile &inifile)
				throw();
				
			/**	Initializes the popup menu <b>  Display </b> with its checkable submenu <b>  Add Label </b>; 
					This method is called automatically	immediately before the main application is started. 
					@param main_control the  MainControl object to be initialized 
					@see   openDialog
					@see   finalizeWidget
					@see   insertMenuEntry
			*/
			virtual void initializeWidget(MainControl& main_control)
				throw();
			
			/**	Removes the checkable submenu <b>  Label Dialog </b> from the popup menu <b>  Display </b>.
					This method will be called by  MainControl::aboutToExit.
					@param main_control the  MainControl to be finalized 
					@see   initializeWidget
					@see   removeMenuEntry
					@see   aboutToExit
			*/
			virtual void finalizeWidget(MainControl& main_control)
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
							
			/** Indicates the apply button was pressed.
			 		Creates a new Representation with the Label and notifies the widgets.
					@see  GeometricCenterProcessor
					@see  RepresentationMessage
			*/
			virtual void accept();
			
			/** Opens the dialog to select the color for the label text.
					See documentation of QT-library for information concerning QColorDialog widgets.
			 */ 
			virtual void editColor();
						
			//@}
			
		private:
			
			int id_;
			
			ColorRGBA custom_color_;
			List<Composite*> selection_;
		};

} } // namespaces

#endif // BALL_VIEW_DIALOGS_LABELDIALOG_H
