// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id:

#ifndef BALL_MOLVIEW_GUI_DIALOGS_LABELPROPERTIES_H
#define BALL_MOLVIEW_GUI_DIALOGS_LABELPROPERTIES_H

#ifndef BALL_FORMAT_INIFILE_H
#	include <BALL/FORMAT/INIFile.h>
#endif

#ifndef BALL_VIEW_GUI_WIDGETS_MODULARWIDGET_H
# include <BALL/VIEW/GUI/WIDGETS/modularWidget.h>
#endif

#ifndef BALL_MOLVIEW_KERNEL_MOLECULARMESSAGE_H
# include <BALL/MOLVIEW/KERNEL/molecularMessage.h>
#endif

#ifndef BALL_MOLVIEW_GUI_DIALOGS_LABELPROPERTIESDATA_H
# include <BALL/MOLVIEW/GUI/DIALOGS/labelPropertiesData.h>
#endif

namespace BALL
{
	using VIEW::ModularWidget;
	using VIEW::MainControl;
	using VIEW::Message;
	using VIEW::ColorRGBA;

	namespace MOLVIEW
	{
    /**  \addtogroup  MolviewDialogs
     *  @{
     */
		/**	The LabelProperties class.
				The class LabelProperties is a dialog for appending to a selection of 
				molecular objects a label which text and color can be changed.
				The class  \link MolecularControl MolecularControl \endlink  is responsible for creating such a selection.
				If this dialog is used, it should be created with  \link MainControl MainControl \endlink  as parent.
				The class LabelPropertiesData contains the definition of the layout of
				this dialog and is therefore not necessary for understanding.  \par
				
		*/
		class LabelProperties : 
			public LabelPropertiesData,
			public ModularWidget
		{
			Q_OBJECT
			BALL_EMBEDDABLE(LabelProperties)
				
			public:
			
			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new labelProperties.
					Calls  \link registerWidget registerWidget \endlink .
					@param      parent the parent widget of {\em *this} labelProperties 
					@param      name the name of {\em *this} labelProperties
					@return     LabelProperties new constructed labelProperties
					@see        QDialog
					@see        ModularWidget
			*/
			LabelProperties(QWidget *parent = NULL, const char *name = NULL )
				throw();

			//@}
			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} labelProperties.
			*/
			virtual ~LabelProperties()
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
					@see   applyButtonClicked
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
					labelProperties) from the	 \link INIFile INIFile \endlink  {\em inifile}.
					This method will be called inside the method  \link show show \endlink  from the class
					 \link MainControl MainControl \endlink .
					@param  inifile the  \link INIFile INIFile \endlink  that contains the needed information for {\em *this} labelProperties
					@see    writePreferences
			*/
			virtual void fetchPreferences(INIFile &inifile)
				throw();
			
			/** Writes the preferences.
					Writes the preferences (the position and the selected color of {\em *this}
					labelProperties) to the	 \link INIFile INIFile \endlink  {\em inifile}.
					This method will be called inside the method  \link aboutToExit aboutToExit \endlink  from the class
					 \link MainControl MainControl \endlink .
					@param  inifile the  \link INIFile INIFile \endlink  to be written into
					@see    fetchPreferences
			*/
			virtual void writePreferences(INIFile &inifile)
				throw();
				
			/**	Initializes the widget.
					Initializes the popup menu {\em Display} with its checkable submenu 
					{\em Label Properties} and connects it to the method  \link openDialog openDialog \endlink .
					This method is called automatically	immediately before the main application 
					is started. 
					This method will be called by  \link show show \endlink  from the  \link MainControl MainControl \endlink  object.
					@param main_control the  \link MainControl MainControl \endlink  object to be initialized with {\em *this} labelProperties
					@see   openDialog
					@see   finalizeWidget
					@see   insertMenuEntry
					@see   checkMenu
					@see   show
			*/
			virtual void initializeWidget(MainControl& main_control)
				throw();
			
			/**	Removes the widget.
					Removes the checkable submenu {\em Label Properties} from the popup menu
					{\em Display}	and cuts the connection to the method  \link openDialog openDialog \endlink .
					This method will be called by  \link aboutToExit aboutToExit \endlink  from the  \link MainControl MainControl \endlink 
					object.
					@param main_control the  \link MainControl MainControl \endlink  object to be finalized with {\em *this} labelProperties
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
					The menus {\em Label Properties} will be checked
					if {\em *this} labelProperties is visible. It will be enabled if a selection of
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
				
			/** Starts the labelProperties dialog.
					Opens {\em *this} labelProperties dialog.
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
			virtual void applyButtonClicked();
			
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
     /** @} */
	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_GUI_DIALOGS_LABELPROPERTIES_H
