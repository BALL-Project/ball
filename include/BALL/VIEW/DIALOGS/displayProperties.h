// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: displayProperties.h,v 1.22 2003/12/09 12:36:19 amoll Exp $
//

#ifndef BALL_VIEW_DIALOGS_DISPLAYPROPERTIES_H
#define BALL_VIEW_DIALOGS_DISPLAYPROPERTIES_H

#ifndef BALL_VIEW_KERNEL_MODULARWIDGET_H
# include <BALL/VIEW/KERNEL/modularWidget.h>
#endif

#ifndef BALL_VIEW_DATATYPE_COLORRGBA_H
# include <BALL/VIEW/DATATYPE/colorRGBA.h>
#endif

#include <BALL/VIEW/UIC/displayPropertiesData.h>

namespace BALL
{
	class Composite;

	namespace VIEW
	{
		class Representation;
		
		class ColoringSettingsDialog;
		class ModelSettingsDialog;
		class Preferences;

		/**	Dialog for creating and changing representations.
				for a selection of molecular objects.
				The class MolecularControl is responsible for creating such a selection.
				With the help of various combo boxes it is possible to customize the look of
				the graphical visualization (the model, the drawing precision, the drawing mode,
				the coloring method and the custom color).
				For information about the drawing precision see Represenation.\par
				If this dialog is used, it should be created with MainControl as parent.\par
				If you want to add a new coloring method or type of representation, have a look at
				VIEW/KERNEL/common.h.
				\ingroup ViewDialogs
		*/
		class DisplayProperties 
			: public DisplayPropertiesData,
				public ModularWidget
		{
			Q_OBJECT
				
			public:
			
			BALL_EMBEDDABLE(DisplayProperties)

			/// Exception to be thrown if an invalid option is given for a modelprocessor
			class InvalidOption: public Exception::GeneralException
			{
				public:

					InvalidOption(const char* file, int line, int option)
						throw();
			};

			/**	@name	Constructors and Destructors
			*/	
			//@{

			/** Default Constructor.
					Sets the combo box of the coloring method to the ElementColorProcessor.
					This combo box as well as the others can be set differently according to saved
					preferences. See fetchPreferences.
					Calls registerWidget.
					\see        fetchPreferences
					\see        ModularWidget
			*/
			DisplayProperties(QWidget *parent = NULL, const char* name = NULL)
				throw();

			/** Destructor
			*/
			virtual ~DisplayProperties()
				throw();

			//@} 
			/**	@name	Accessors: inspectors and mutators 
			*/ 
			//@{

			/** Message handling method.
					Handles messages sent by other registered ConnectionObject objects.
					Catches NewMolecularMessage and MolecularSelectionMessage.
					If NewMolecularMessage is catched the chosen graphical visualization
					will be applied to the Composite object and the follwing Message
					objects will be sent through the ConnectionObject tree:
						- CompositeMessage
						- ShowDisplayPropertiesMessage
						- RepresentationMessage 
						- ControlSelectionMessage
					\par
					\param message the pointer to the message that should be processed
			*/
			virtual void onNotify(Message *message)
					throw();
			
			//@} 
			/**	ModularWidget methods 
			*/ 
			//@{
			
			/** Fetches the preferences (the position, the custom color, the model, the drawing 
					precision, the drawing mode and the coloring method from the	INIFile <b>inifile</b>.
					This method will be called inside the method show() from the class MainControl.
					\param  inifile the INIFile that contains the needed information 
					\see    writePreferences
					\see    MainControl
			*/
			virtual void fetchPreferences(INIFile &inifile)
					throw();
				
			/** Writes the preferences (the position, the custom color, the model, the drawing 
					precision, the drawing mode and the coloring method to the	INIFile <b> inifile.
					This method will be called inside the method aboutToExit from the class
					MainControl.
					\param  inifile the INIFile to be written into
					\see    fetchPreferences
			*/
			virtual void writePreferences(INIFile &inifile)
					throw();
				
			/**	Initialize the popup menu <b>Display</b> with the submenu
					<b>Display Properties</b>.
					This menu entry opens the dialog.
					This method is called automatically	immediately before the main application is started
					by MainControl::show()
					\param main_control the MainControl object to be initialized 
			*/
			virtual void initializeWidget(MainControl& main_control)
					throw();
				
			/**	Removes the menu entry <b>Display Properties</b> from the 
					<b> Display</b> menu.
					This method will be called by MainControl::aboutToExit() 
					\param main_control the MainControl object to be finalized 
			*/
			virtual void finalizeWidget(MainControl& main_control)
					throw();

			///
			virtual void initializePreferencesTab(Preferences &preferences)
				throw();

			///
			virtual void finalizePreferencesTab(Preferences &preferences)
				throw();

			///
			void applyPreferences(Preferences& preferences)
				throw();

			///
			void defaultPreferences(Preferences& preferences)
				throw();

			/**	Menu checking method.
					This method is called by MainControl::checkMenus before a popup menu is shown.
					The menu <b> Display Properties</b> will be checked if this displayProperties is visible. 
					\param main_control the MainControl object whose menus should be checked
			*/
			virtual void checkMenu(MainControl& main_control)
					throw();

			/// Set the selected Representation, which should be modified
			void setRepresentation(Representation* rep)
				throw() {rep_ = rep;}
				
			public slots:
					
			//@} 
			/** @name Public slots 
			*/ 
			//@{

			/** Starts the dialog.
					Calls raise from QDialog class.
			*/
			void show();

			/// Switch to the mode, that a new Representation will be created
			void createRepresentationMode();

			/// Switch to the mode, that an existing Representation will be modified
			void modifyRepresentationMode();
				
			protected slots:
					
			//@} /** @name Protected slots */ //@{
					
			/** Changes the model.
					This slot is connected to the model combo box and will be automatically
					called if the content of this combo box is changed.
					\param  index the position of the entry in the combobox
			*/
			virtual void selectModel(int index);

			/** Changes the drawing mode.
					This slot is connected to the mode combo box and will be automatically
					called if the content of this combo box is changed.
					\param  index the position of the entry in the combobox
			*/
			virtual void selectMode(int index);

			/** Changes the coloring method.
					This slot is connected to the coloring method combo box and will be automatically
					called if the content of this combo box is changed.
					\param  index the position of the entry in the combobox
			*/
			virtual void selectColoringMethod(int index);
			
			/** Indicates the apply button was pressed.
					Applies the selected model with its selected properties to 
					the selected Composite objects or modifies an existing Representation.
					A SceneMessage will be sent to inform the Scene.
					A RepresentationMessage notifies the GeometricControl.
			*/
			virtual void applyButtonClicked();
			
			/** Opens the dialog for editing the custom color.
					Opens a QColorDialog from the QT-library.
			 */ 
			virtual void editColor();

			/** Opens the color dialog for the color of selected items.
					\see BALL_SELECTED_COLOR
			*/
			virtual void editSelectionColor();

			///
			virtual void coloringOptionsPressed();

			///
			virtual void modelOptionsPressed();

			///
			virtual void precisionBoxChanged(int index);

			///
			virtual void transparencySliderChanged();

			///
			virtual void precisionSliderChanged();

			//@}
				
			protected:
			
			//_ Set the selection of a given combobox to a given value 
			void setComboBoxIndex_(QComboBox* combo_box, QString& item_string);

			//_ Set buttons and slider according to the values
			void checkDrawingPrecision_()
				throw();

			/*_ Create the new representation for the selection in the MolecularControl or for a given List of Composites.
					Called by onNotify() after receiving CompositeMessage::NEW_MOLECULE and by applyButtonClicked().
					To insert a new type of model, this is the only method in DisplayProperties you have to
					change (See also VIEW/KERNEL/common.h).
			*/
			virtual void createRepresentation_(const List<Composite*>& composites)
				throw(InvalidOption);

			//_ Read a inifile entry and set the accoring attribut and ComboBox.
			virtual void getEntry_(INIFile& inifile, const String& key, QComboBox& box);

			//_
			virtual void getAdvancedModelOptions_()
				throw();

			//_
			virtual void getAdvancedColoringOptions_()
				throw();

			// --------------------------------------------------------------------------------
			// attributs
			// --------------------------------------------------------------------------------
			ModelSettingsDialog* 			model_settings_;
			ColoringSettingsDialog* 	coloring_settings_;
			Preferences* 							preferences_;
			
			// the menu entry id of the dialog
			int 						id_;
			
			// used by GeometricControl to modify an existing representation
			Representation* rep_;

			ColorRGBA 			custom_color_;
			bool 						advanced_options_modified_;
		};

} } // namespaces

#endif // BALL_VIEW_DIALOGS_DISPLAYPROPERTIES_H
