// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: displayProperties.h,v 1.38 2004/10/01 14:20:22 amoll Exp $
//

#ifndef BALL_VIEW_DIALOGS_DISPLAYPROPERTIES_H
#define BALL_VIEW_DIALOGS_DISPLAYPROPERTIES_H

#ifndef BALL_VIEW_KERNEL_MODULARWIDGET_H
# include <BALL/VIEW/KERNEL/modularWidget.h>
#endif

#ifndef BALL_VIEW_DATATYPE_COLORRGBA_H
# include <BALL/VIEW/DATATYPE/colorRGBA.h>
#endif

#ifndef BALL_VIEW_KERNEL_PREFERENCESENTRY
# include <BALL/VIEW/KERNEL/preferencesEntry.h>
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

		/**	Dialog for creating and changing representations for a selection of molecular objects.
				It can create a new Representation for a selection of Composite 's from the 
				MolecularControl. If a Representation is selected in the GeometricControl, it
				can be modified with this dialog.
				With the help of various combo boxes it is possible to customize the look of
				the graphical visualization (the model, the drawing precision, the drawing mode,
				the coloring method and the custom color).
				For information about the drawing precision see Representation. <br>
				If this dialog is used, it should be created with MainControl as parent. <br>
				If you want to add a new coloring method or type of Representation, have a look at
				VIEW/KERNEL/common.h.
				\ingroup ViewDialogs
		*/
		class BALL_EXPORT DisplayProperties 
			: public DisplayPropertiesData,
				public ModularWidget,
				public PreferencesEntry
		{
			Q_OBJECT
				
			public:
			
			BALL_EMBEDDABLE(DisplayProperties,ModularWidget)

			/**	@name	Constructors and Destructors
			*/	
			//@{

			/** Default Constructor.
					Calls ModularWidget::registerWidget.
			*/
			DisplayProperties(QWidget *parent = NULL, const char* name = NULL)
				throw();

			/// Copy constructor just implemented for Python Interface, dont use it! 
			DisplayProperties(const DisplayProperties& dp)
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
					If a CompositeMessage with type NEW_MOLECULE is catched,
					the chosen graphical visualization
					will be applied to the Composite object and the follwing Message
					objects will be sent through the ConnectionObject tree:
						- CompositeMessage with type CENTER_CAMERA
						- RepresentationMessage with type NEW
					\par
					\param message the pointer to the message that should be processed
			*/
			virtual void onNotify(Message *message)
					throw();
			
			//@} 
			/**	ModularWidget methods 
			*/ 
			//@{
			
			/** Fetches the preferences (the position, custom color, model, drawing 
					precision, drawing mode and the coloring method from the INIFile.
					This method will be called inside MainControl::show().
					\param  inifile the INIFile that contains the needed information 
					\see    writePreferences
			*/
			virtual void fetchPreferences(INIFile &inifile)
					throw();
				
			/** Writes the preferences (the position, custom color, model, drawing 
					precision, drawing mode and the coloring method to the INIFile.
					This method will be called inside MainControl::aboutToExit()
					\param  inifile the INIFile to be written into
					\see    fetchPreferences
			*/
			virtual void writePreferences(INIFile &inifile)
					throw();
				
			/**	Initialize the popup menu <b>Display</b> with the entry
					<b>Display Properties</b>, which opens the dialog.
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

			/// Insert the ModelSettingsDialog and the ColoringSettingsDialog into the Preferences
			virtual void initializePreferencesTab(Preferences &preferences)
				throw();

			/// Remove the ModelSettingsDialog and the ColoringSettingsDialog from the Preferences
			virtual void finalizePreferencesTab(Preferences &preferences)
				throw();

			///
			void applyPreferences()
				throw();

			/**	Menu checking method.
					This method is called by MainControl::checkMenus before a popup menu is shown.
					The menu entry <b>Display Properties</b> will be checked if this dialog is visible. 
					\param main_control the MainControl object whose menus should be checked
			*/
			virtual void checkMenu(MainControl& main_control)
					throw();

			/// Set the selected Representation, which should be modified
			void setRepresentation(Representation* rep)
				throw() {rep_ = rep;}

			/// Settings from String
			bool getSettingsFromString(const String& data)
				throw();

			/// Set if Representations are automaticaly created for new Molecules
			void enableCreationForNewMolecules(bool state) 
				throw() { create_representations_for_new_molecules_ = state;}
				
			public slots:
					
			//@} 
			/** @name Public slots 
			*/ 
			//@{

			/** Starts the dialog.
					Calls QDialog::raise().
			*/
			void show();

			/// Switch to the mode, that a new Representation will be created
			void createRepresentationMode();

			/// Switch to the mode, that an existing Representation will be modified
			void modifyRepresentationMode();
				
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

			///
			virtual void setSurfaceDrawingPrecision(float value);
			
			///
			virtual void setDrawingPrecision(int value);

			///
			virtual void setTransparency(int value);

			virtual void setCustomColor(const ColorRGBA& color);
			
			/** Indicates the apply button was pressed.
					Applies the selected model with its selected properties to 
					the selected Composite objects or modifies an existing Representation.
					A SceneMessage will be sent to inform the Scene.
					A RepresentationMessage notifies the GeometricControl.
			*/
			virtual void apply();
			
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
			
			//_ Set buttons and slider according to the values
			void checkDrawingPrecision_()
				throw();

			/*_ Create the new representation for the selection in the MolecularControl or for a given List of Composites.
					Called by onNotify() after receiving CompositeMessage::NEW_MOLECULE and by apply().
					To insert a new type of model, this is the only method in DisplayProperties you have to
					change (See also VIEW/KERNEL/common.h).
			*/
			virtual Representation* createRepresentation_(const List<Composite*>& composites)
				throw(Exception::InvalidOption);

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
			bool 						create_representations_for_new_molecules_;
		};

} } // namespaces

#endif // BALL_VIEW_DIALOGS_DISPLAYPROPERTIES_H
