// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id:

#ifndef BALL_MOLVIEW_GUI_DIALOGS_LABELPROPERTIES_H
#define BALL_MOLVIEW_GUI_DIALOGS_LABELPROPERTIES_H

#ifndef BALL_VIEW_GUI_WIDGETS_MODULARWIDGET_H
# include <BALL/VIEW/GUI/WIDGETS/modularWidget.h>
#endif

#ifndef BALL_MOLVIEW_KERNEL_MOLECULARMESSAGE_H
# include <BALL/MOLVIEW/KERNEL/molecularMessage.h>
#endif

#ifndef BALL_MOLVIEW_DIALOGS_LABELPROPERTIESDATA_H
# include <BALL/MOLVIEW/DIALOGS/labelPropertiesData.h>
#endif

#ifndef BALL_VIEW_DATATYPE_COLORRGBA_H
# include <BALL/VIEW/DATATYPE/colorRGBA.h>
#endif

namespace BALL
{
	class INIFile;

	using VIEW::ModularWidget;
	using VIEW::MainControl;
	using VIEW::Message;
	using VIEW::ColorRGBA;

	namespace MOLVIEW
	{
		/**	The LabelProperties class.
				The class LabelProperties is a dialog for appending to a selection of 
				molecular objects a label which text and color can be changed.
				The class MolecularControl is responsible for creating such a selection.
				If this dialog is used, it should be created with MainControl as parent.
				The class LabelPropertiesData contains the definition of the layout of
				this dialog and is therefore not necessary for understanding. \par
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
					Calls registerWidget.
					\param      parent the parent widget of this labelProperties 
					\param      name the name of this labelProperties
					\return     LabelProperties new constructed labelProperties
					\see        QDialog
					\see        ModularWidget
			*/
			LabelProperties(QWidget *parent = NULL, const char *name = NULL )
				throw();

			//@}
			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of this labelProperties.
			*/
			virtual ~LabelProperties()
				throw();
							
			//@}
			/**	@name	Accessors: inspectors and mutators 
			 */
			//@{

			/** Message handling method.
					Handles messages sent by other registered ConnectionObject objects.
					Catches only MolecularSelectionMessage objects.
					If such a message is catched the apply button will be enabled and labels
					can be appended onto the selection.
					\param message the pointer to the message that should be processed
					\see   applyButtonClicked
					\see   MolecularSelectionMessage
					\see   ConnectionObject
					\see   Message
		  */
			virtual void onNotify(Message *message)
				throw();
							
			//@}
			/**	ModularWidget methods
			*/
			//@{
			
			/** Fetches the preferences.
					Fetches the preferences (the position and the selected color of this
					labelProperties) from the	INIFile <b> inifile.
					This method will be called inside the method show from the class
					MainControl.
					\param  inifile the INIFile that contains the needed information for this labelProperties
					\see    writePreferences
			*/
			virtual void fetchPreferences(INIFile &inifile)
				throw();
			
			/** Writes the preferences.
					Writes the preferences (the position and the selected color of this
					labelProperties) to the	INIFile <b> inifile.
					This method will be called inside the method aboutToExit from the class
					MainControl.
					\param  inifile the INIFile to be written into
					\see    fetchPreferences
			*/
			virtual void writePreferences(INIFile &inifile)
				throw();
				
			/**	Initializes the widget.
					Initializes the popup menu <b> Display</b> with its checkable submenu 
					<b> Label Properties and connects it to the method openDialog.
					This method is called automatically	immediately before the main application 
					is started. 
					This method will be called by show from the MainControl object.
					\param main_control the MainControl object to be initialized with this labelProperties
					\see   openDialog
					\see   finalizeWidget
					\see   insertMenuEntry
					\see   checkMenu
					\see   show
			*/
			virtual void initializeWidget(MainControl& main_control)
				throw();
			
			/**	Removes the widget.
					Removes the checkable submenu <b> Label Properties</b> from the popup menu
					<b> Display	and cuts the connection to the method openDialog.
					This method will be called by aboutToExit from the MainControl
					object.
					\param main_control the MainControl object to be finalized with this labelProperties
					\see   initializeWidget
					\see   checkMenu
					\see   removeMenuEntry
					\see   aboutToExit
			*/
			virtual void finalizeWidget(MainControl& main_control)
				throw();
			
			/**	Menu checking method.
					This method is called by the method checkMenus from the
					MainControl object before a popup menu is shown.
					The menus <b> Label Properties</b> will be checked
					if this labelProperties is visible. It will be enabled if a selection of
					objects to be labeled is available, disabled otherwise.
					\param main_control the MainControl object whose menus should be checked
					\see   initializeWidget
					\see   finalizeWidget
					\see   checkMenus
					\see   show
			*/
			virtual void checkMenu(MainControl& main_control)
				throw();
							
			//@}
			
			public slots:
				
  		/** @name Public slots
	  	*/
			//@{
				
			/** Starts the labelProperties dialog.
					Opens this labelProperties dialog.
					Calls show and raise from QDialog class.
					See documentation of QT-library for information concerning QDialog widgets.
			*/
			void openDialog();
						
			//@}
			/** @name Protected slots
			*/
			//@{
				
			protected slots:
							
			/** Indicates the apply button was pressed.
					Appends a GLLabel with the specified text and color
					to the geometric center of the objects in the selection. 
					Sents the message ChangedCompositeMessage, SceneMessage
					and WindowMessage to inform other ConnectionObject about the change
					in the Composite objects in the selection.
					\see  onNotify
					\see  Composite
					\see  GeometricCenterProcessor
					\see  GLLabel
					\see  ChangedCompositeMessage
					\see  SceneMessage
					\see  WindowMessage
			*/
			virtual void applyButtonClicked();
			
			/** Opens the edit color dialog.
					Opens the dialog for editing the color in which the text of the label
					should be drawn.
					Opens a QColorDialog from the QT-library.
					See documentation of QT-library for information concerning QColorDialog widgets.
			 */ 
			virtual void editColor();
						
			//@}
			
		private:
			
			int id_;
			
			ColorRGBA custom_color_;
			List<Composite*> selection_;
		};

	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_GUI_DIALOGS_LABELPROPERTIES_H
