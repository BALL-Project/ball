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
		/**	Dialog for creating labels.
				The class LabelDialog is a dialog for appending to a selection of 
				molecular objects a label which text and color can be changed.
				The class  \link MolecularControl MolecularControl \endlink  is responsible for creating such a selection.
				If this dialog is used, it should be created with  \link MainControl MainControl \endlink  as parent.
				The class LabelDialogData contains the definition of the layout of
				this dialog and is therefore not necessary for understanding.  \par
    		\ingroup  ViewDialogs
		*/
		class LabelDialog : 
			public LabelDialogData,
			public ModularWidget
		{
			Q_OBJECT
			BALL_EMBEDDABLE(LabelDialog)
				
			public:
			
			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new labelDialog.
					Calls  \link registerWidget registerWidget \endlink .
					@param      parent the parent widget of {\em *this} labelDialog 
					@param      name the name of {\em *this} labelDialog
					@return     LabelDialog new constructed labelDialog
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
					Default destruction of {\em *this} labelDialog.
			*/
			virtual ~LabelDialog()
				throw();
							
			//@}
			/**	@name	Accessors: inspectors and mutators 
			 */
			//@{

			/** Message handling method.
					Handles messages sent by other registered  \link ConnectionObject ConnectionObject \endlink  objects.
					Catches only  \link MolecularSelectionMessage MolecularSelectionMessage \endlink  objects.
					If such a message is catched the apply button will be enabled and labels
					can be appended onto the selection.
					@param message the pointer to the message that should be processed
					@see   MolecularSelectionMessage
					@see   ConnectionObject
					@see   Message
		  */
			virtual void onNotify(Message *message)
				throw();
							
			//@}
			/**	ModularWidget methods
			*/
			//@{
			
			/** Fetches the preferences.
					Fetches the preferences (the position and the selected color of {\em *this}
					labelDialog) from the	 \link INIFile INIFile \endlink  {\em inifile}.
					This method will be called inside the method  \link show show \endlink  from the class
					 \link MainControl MainControl \endlink .
					@param  inifile the  \link INIFile INIFile \endlink  that contains the needed information for {\em *this} labelDialog
					@see    writePreferences
			*/
			virtual void fetchPreferences(INIFile &inifile)
				throw();
			
			/** Writes the preferences.
					Writes the preferences (the position and the selected color of {\em *this}
					labelDialog) to the	 \link INIFile INIFile \endlink  {\em inifile}.
					This method will be called inside the method  \link aboutToExit aboutToExit \endlink  from the class
					 \link MainControl MainControl \endlink .
					@param  inifile the  \link INIFile INIFile \endlink  to be written into
					@see    fetchPreferences
			*/
			virtual void writePreferences(INIFile &inifile)
				throw();
				
			/**	Initializes the widget.
					Initializes the popup menu {\em Display} with its checkable submenu 
					{\em Label Dialog} and connects it to the method  \link openDialog openDialog \endlink .
					This method is called automatically	immediately before the main application 
					is started. 
					This method will be called by  \link show show \endlink  from the  \link MainControl MainControl \endlink  object.
					@param main_control the  \link MainControl MainControl \endlink  object to be initialized with {\em *this} labelDialog
					@see   openDialog
					@see   finalizeWidget
					@see   insertMenuEntry
					@see   checkMenu
					@see   show
			*/
			virtual void initializeWidget(MainControl& main_control)
				throw();
			
			/**	Removes the widget.
					Removes the checkable submenu {\em Label Dialog} from the popup menu
					{\em Display}	and cuts the connection to the method  \link openDialog openDialog \endlink .
					This method will be called by  \link aboutToExit aboutToExit \endlink  from the  \link MainControl MainControl \endlink 
					object.
					@param main_control the  \link MainControl MainControl \endlink  object to be finalized with {\em *this} labelDialog
					@see   initializeWidget
					@see   checkMenu
					@see   removeMenuEntry
					@see   aboutToExit
			*/
			virtual void finalizeWidget(MainControl& main_control)
				throw();
			
			/**	Menu checking method.
					This method is called by the method  \link checkMenus checkMenus \endlink  from the
					 \link MainControl MainControl \endlink  object before a popup menu is shown.
					The menus {\em Label Dialog} will be checked
					if {\em *this} labelDialog is visible. It will be enabled if a selection of
					objects to be labeled is available, disabled otherwise.
					@param main_control the  \link MainControl MainControl \endlink  object whose menus should be checked
					@see   initializeWidget
					@see   finalizeWidget
					@see   checkMenus
					@see   show
			*/
			virtual void checkMenu(MainControl& main_control)
				throw();
							
			//@}
			
			public slots:
				
  		/** @name Public slots
	  	*/
			//@{
				
			/** Starts the labelDialog dialog.
					Opens {\em *this} labelDialog dialog.
					Calls  \link show show \endlink  and  \link raise raise \endlink  from  \link QDialog QDialog \endlink  class.
					See documentation of QT-library for information concerning QDialog widgets.
			*/
			void openDialog();
						
			//@}
			/** @name Protected slots
			*/
			//@{
				
			protected slots:
							
			/** Indicates the apply button was pressed.
					Appends a  \link GLLabel GLLabel \endlink  with the specified text and color
					to the geometric center of the objects in the selection. 
					Sents the message  \link ChangedCompositeMessage ChangedCompositeMessage \endlink ,  \link SceneMessage SceneMessage \endlink 
					and  \link WindowMessage WindowMessage \endlink  to inform other  \link ConnectionObject ConnectionObject \endlink  about the change
					in the  \link Composite Composite \endlink  objects in the selection.
					@see  onNotify
					@see  Composite
					@see  GeometricCenterProcessor
					@see  GLLabel
					@see  ChangedCompositeMessage
					@see  SceneMessage
					@see  WindowMessage
			*/
			virtual void accept();
			
			/** Opens the edit color dialog.
					Opens the dialog for editing the color in which the text of the label
					should be drawn.
					Opens a  \link QColorDialog QColorDialog \endlink  from the QT-library.
					See documentation of QT-library for information concerning QColorDialog widgets.
			 */ 
			virtual void editColor();
						
			//@}
			
		private:
			
			int id_;
			
			ColorRGBA custom_color_;
			List<Composite*> selection_;
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_DIALOGS_LABELDIALOG_H
