// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: displayProperties.h,v 1.11.2.1 2003/01/07 13:18:27 anker Exp $

#ifndef BALL_MOLVIEW_GUI_DIALOGS_DISPLAYPROPERTIES_H
#define BALL_MOLVIEW_GUI_DIALOGS_DISPLAYPROPERTIES_H

#ifndef BALL_STRUCTURE_FRAGMENTDB_H
# include <BALL/STRUCTURE/fragmentDB.h>
#endif

#ifndef BALL_FORMAT_INIFILE_H
#	include <BALL/FORMAT/INIFile.h>
#endif

#ifndef BALL_MOLVIEW_GUI_FUNCTOR_GLATOMBONDMODELCONNECTOR_H
# include <BALL/MOLVIEW/GUI/FUNCTOR/glAtomBondModelConnector.h>
#endif

#ifndef BALL_MOLVIEW_FUNCTOR_STANDARDCOLORCALCULATOR_H
#	include <BALL/MOLVIEW/FUNCTOR/standardColorCalculator.h>
#endif

#ifndef BALL_VIEW_GUI_WIDGETS_MODULARWIDGET_H
# include <BALL/VIEW/GUI/WIDGETS/modularWidget.h>
#endif

#ifndef BALL_MOLVIEW_GUI_DIALOGS_DISPLAYPROPERTIESDATA_H
# include <BALL/MOLVIEW/GUI/DIALOGS/displayPropertiesData.h>
#endif

#ifndef BALL_MOLVIEW_GUI_FUNCTOR_GLBACKBONEMODEL_H
# include <BALL/MOLVIEW/GUI/FUNCTOR/glBackboneModel.h>
#endif

#ifndef BALL_MOLVIEW_GUI_FUNCTOR_GLBALLANDSTICKMODEL_H
# include <BALL/MOLVIEW/GUI/FUNCTOR/glBallAndStickModel.h>
#endif

#ifndef BALL_MOLVIEW_GUI_FUNCTOR_GLLINEMODEL_H
# include <BALL/MOLVIEW/GUI/FUNCTOR/glLineModel.h>
#endif

#ifndef BALL_MOLVIEW_GUI_FUNCTOR_GLSURFACEMODEL_H
# include <BALL/MOLVIEW/GUI/FUNCTOR/glSurfaceModel.h>
#endif

#ifndef BALL_MOLVIEW_GUI_FUNCTOR_GLVANDERWAALSMODEL_H
# include <BALL/MOLVIEW/GUI/FUNCTOR/glVanDerWaalsModel.h>
#endif

#ifndef BALL_MOLVIEW_GUI_FUNCTOR_REMOVEMODEL_H
# include <BALL/MOLVIEW/FUNCTOR/removeModel.h>
#endif

namespace BALL
{
	namespace MOLVIEW
	{

			/// Exception to be thrown if invalid option is given for a modelprocessor
 			class InvalidOption: public Exception::GeneralException
			{
  			public:
	   			InvalidOption(const char* file, int line, int option)
					throw();
			};

		/**	The DisplayProperties class.
				The class DisplayProperties is a dialog used for changing the graphical
				representation of a selection of molecular objects.
				With the help of various combo boxes it is possible to customize the look of
				the graphical visualization (the model, the drawing precision, the drawing mode,
				the coloring method and the custom color).
				The follwing properties are available:\\ \\
				The models:
				\begin{itemize}
				  \item  \Ref{AddGLBallAndStickModel} the ball and stick model
				  \item  \Ref{AddGLBackboneModel} the backbone model
				  \item  \Ref{AddGLLineModel} the line model
				  \item  \Ref{AddGLSurfaceModel} the surface model
				  \item  \Ref{AddGLVanDerWaalsModel} the van der waals model
				\end{itemize} 
				The coloring methods:
				\begin{itemize}
				  \item  \Ref{ElementColorCalculator} colors the molecular objects according to their atom elements
				  \item  \Ref{ResidueNameColorCalculator} colors the molecular objects according to their residue names
				  \item  \Ref{AtomChargeColorCalculator} colors the molecular objects according to their atom charge
				  \item  \Ref{AtomDistanceColorCalculator} colors the molecular objects according to their atom distance
				  \item  \Ref{CustomColorCalculator} colors the molecular objects according to selected color
				\end{itemize} 
				For information about the drawing precision see \Ref{GeometricObject}.\\
				This dialog is also responsible for selecting and deselecting,
				building bonds and adding hydrogens to molecular objects. Further it is possible
				to center the camera of the \Ref{Scene} to the geometric center of the selection of molecular objects.
				The class \Ref{MolecularControl} is responsible for creating such a selection.
				If this dialog is used, it should be created with \Ref{MainControl} as parent.
				The class DisplayPropertiesData contains the definition of the layout of
				this dialog and is therefore not necessary for understanding. \\
				{\bf Definition:} \URL{BALL/MOLVIEW/GUI/DIALOGS/displayProperties.h}
		*/
		class DisplayProperties 
			: public DisplayPropertiesData,
			  public ModularWidget
		{
			Q_OBJECT
			BALL_EMBEDDABLE(DisplayProperties)
				
			public:
			
			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Constructs new displayProperties.
					Sets the combo box of the coloring method to the \Ref{ElementColorCalculator}.
					This combo box as well as the others can be set differently according to saved
					preferences. See \Ref{fetchPreferences}.
					Calls \Ref{registerWidget}.
					@param      parent the parent widget of {\em *this} displayProperties 
					@param      name the name of {\em *this} displayProperties 
					@return     DisplayProperties new constructed displayProperties
					@see        fetchPreferences
					@see        ElementColorCalculator
					@see        QDialog
					@see        ModularWidget
			*/
			DisplayProperties(QWidget *parent = NULL, const char* name = NULL)
					throw();

			//@}
			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} displayProperties.
			*/
			virtual ~DisplayProperties()
				throw();

			//@}
			/**	@name	Accessors: inspectors and mutators 
			 */
			//@{

			/** Message handling method.
					Handles messages sent by other registered \Ref{ConnectionObject} objects.
					Catches \Ref{NewMolecularMessage} and \Ref{MolecularSelectionMessage}.
					If \Ref{NewMolecularMessage} is catched the chosen graphical visualization
					will be applied to the \Ref{Composite} object and the follwing \Ref{Message}
					objects will be sent through the \Ref{ConnectionObject} tree:
					\begin{itemize}
						\item  \Ref{ChangedMolecularMessage} to indicate that the \Ref{Composite} object has changed.
						\item  \Ref{SceneMessage} to force an update of the \Ref{Scene} (and set the camera to new object)
					\end{itemize}
					If \Ref{MolecularSelectionMessage} is catched the selection of this object will 
					be used for changing the visualization.
					If such a message is catched the apply button will be enabled and the graphical
					representation of the selection can be changed.
					@param message the pointer to the message that should be processed
					@see   applyButtonClicked
					@see   NewMolecularMessage
					@see   ChangedMolecularMessage
					@see   MolecularSelectionMessage
					@see   SceneMessage
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
					Fetches the preferences (the position, the custom color, the model, the drawing 
					precision, the drawing mode and the coloring method of {\em *this}
					displayProperties) from the	\Ref{INIFile} {\em inifile}.
					This method will be called inside the method \Ref{show} from the class
					\Ref{MainControl}.
					@param  inifile the \Ref{INIFile} that contains the needed information for {\em *this} displayProperties
					@see    writePreferences
					@see    MainControl
			*/
			virtual void fetchPreferences(INIFile &inifile)
					throw();
				
			/** Writes the preferences.
					Writes the preferences (the position, the custom color, the model, the drawing 
					precision, the drawing mode and the coloring method of {\em *this}
					displayProperties) to the	\Ref{INIFile} {\em inifile}.
					This method will be called inside the method \Ref{aboutToExit} from the class
					\Ref{MainControl}.
					@param  inifile the \Ref{INIFile} to be written into
					@see    fetchPreferences
					@see    MainControl
			*/
			virtual void writePreferences(INIFile &inifile)
					throw();
				
			/**	Initializes the widget.
					Initializes the popup menu {\em Display} with its checkable submenus
					\begin{itemize}
						\item  {\em Display Properties} - opens the dialog (indicates if open)
						\item  {\em Select} - marks the selected molecular objects in the selected color (See \Ref{GeometricObject})
						\item  {\em Deselect} - uses the previously set color of the selected molecular objects
						\item  {\em Focus Camera} - centers the camera of \Ref{Scene} to the geometric center 
										of the molecular objects in the selection
						\item  {\em Build Bonds} - generates the \Ref{Bond} object to the molecular objects in the selection
						\item  {\em Add Hydrogens} - adds hydrogens to the molecular objects in the selection
					\end{itemize}
					and adds them to the appropriate slots.
					This method is called automatically	immediately before the main application is started. 
					This method will be called by \Ref{show} from the \Ref{MainControl} object.
					@param main_control the \Ref{MainControl} object to be initialized with {\em *this} displayProperties
					@see   openDialog
					@see   select
					@see   deselect
					@see   centerCamera
					@see   buildBonds
					@see   addHydrogens
					@see   GeometricObject
					@see   finalizeWidget
					@see   insertMenuEntry
					@see   checkMenu
					@see   show
					@see   MainControl
			*/
			virtual void initializeWidget(MainControl& main_control)
					throw();
				
			/**	Removes the widget.
					Removes the checkable submenus from the popup menu
					{\em Display}	and cut the all previously registered connections.
					This method will be called by \Ref{aboutToExit} from the \Ref{MainControl} object.
					@param main_control the \Ref{MainControl} object to be finalized with {\em *this} displayProperties
					@see   initializeWidget
					@see   checkMenu
					@see   removeMenuEntry
					@see   aboutToExit
					@see   MainControl
			*/
			virtual void finalizeWidget(MainControl& main_control)
					throw();
				
			/**	Menu checking method.
					This method is called by the method \Ref{checkMenus} from the
					\Ref{MainControl} object before a popup menu is shown.
					The menu {\em Display Properties} will be checked
					if {\em *this} displayProperties is visible. 
					The menus {\em Select}, {\em Deselect}, {\em Add Hydrogens} and {\em Build Bonds}
					will be enabled if the selection of molecular objects is not empty.
					The menu {\em Focus camera} will be enabled only if only one molecular object
					is in the selection list.
					@param main_control the \Ref{MainControl} object whose menus should be checked
					@see   onNotify
					@see   initializeWidget
					@see   finalizeWidget
					@see   checkMenus
					@see   show
					@see   MainControl
			*/
			virtual void checkMenu(MainControl& main_control)
					throw();
				
			public slots:
					
			//@}
			/** @name Public slots
			*/
			//@{

			/** Starts the displayProperties dialog.
					Opens {\em *this} displayProperties dialog.
					Calls \Ref{show} and \Ref{raise} from \Ref{QDialog} class.
					See documentation of QT-library for information concerning QDialog widgets.
			*/
			void openDialog();
				
			protected slots:
					
			//@}
			/** @name Protected slots
			*/
			//@{
					
			/** Changes the drawing precision for the static modell.
					This slot is connected to the drawing precision combo box and will be automatically
					called if the contents of this combo box is changed.
					This method changes the drawing precision of the selected model.
					@param   string the string containing the new drawing precision
					@see     selectColoringMethod
					@see     applyButtonClicked
					@see     selectModel
			*/
			virtual void selectPrecisionStatic(const QString& string);

			/** Changes the drawing precision for the dynamic modell.
			 */
			virtual void selectPrecisionDynamic(const QString& string);
				
			/** Changes the static model.
					This slot is connected to the static model combo box and will be automatically
					called if the content of this combo box is changed.
					@param   string the string containing the new model
					@see     selectColoringMethod
					@see     applyButtonClicked
					@see     selectPrecision
			*/
			virtual void selectModelStatic(const QString& string);

			/** Changes the dynamic model.
			 */
			virtual void selectModelDynamic(const QString& string);
			
			/** Changes the coloring method.
					This slot is connected to the coloring method combo box and will be automatically
					called if the content of this combo box is changed.
					This method changes the coloring method of the selected model.
					@param   string the string containing the new coloring method
					@see     selectPrecision
					@see     applyButtonClicked
					@see     selectModel
			*/
			virtual void selectColoringMethod(const QString& string);
			
			/** Indicates the apply button was pressed.
					Applies the selected model with its selected properties to each \Ref{Composite} object
					available in the molecular selection list.
					The message \Ref{ChangedCompositeMessage} will be sent for each \Ref{Composite}
					object in the	selection list.
					The messages \Ref{WindowMessage} and \Ref{SceneMessage} will
					be sent to inform the \Ref{MainControl} and the \Ref{Scene} about the change.
					@see  selectPrecision
					@see  selectColoringMethod
					@see  selectModel
					@see  onNotify
					@see  Composite
					@see  ChangedCompositeMessage
					@see  SceneMessage
					@see  WindowMessage
			*/
			virtual void applyButtonClicked();
			
			/** Opens the edit color dialog.
					Opens the dialog for editing the color in which the model
					should be drawn (if coloring method: custom was chosen).
					Opens a \Ref{QColorDialog} from the QT-library.
					See documentation of QT-library for information concerning QColorDialog widgets.
					@see   selectColoringMethod
			 */ 
			virtual void editColor();

			/** Opens the color dialog for the color of selected items.
			 		@see BALL_SELECTED_COLOR
			*/
			virtual void editSelectionColor();
			//@}
				
			private:
			
			/*_ Set the selection of a given combobox to a given value */
			void setComboBoxIndex_(QComboBox* combo_box, QString& item_string);

			// --------------------------------------------------------------------------------
			// methods for the model processors
			// --------------------------------------------------------------------------------

			enum Address
			{
				ADDRESS__UNKOWN                    = 0,
				ADDRESS__STATIC_MODEL,
				ADDRESS__DYNAMIC_MODEL,
				ADDRESS__ALWAYS_FRONT_MODEL,
				ADDRESS__TRANSPARENT_MODEL,
				ADDRESS__STATIC_DRAWING_PRECISION,
				ADDRESS__STATIC_DRAWING_MODE,
				ADDRESS__DYNAMIC_DRAWING_PRECISION,
				ADDRESS__DYNAMIC_DRAWING_MODE,
				ADDRESS__CAMERA_DISTANCE
			};		
			
			enum Value
			{
				VALUE__MODEL_UNKOWN         = 0,
				VALUE__MODEL_LINES          = 1,
				VALUE__MODEL_STICK          = 2,
				VALUE__MODEL_BALL_AND_STICK = 3,
				VALUE__MODEL_SURFACE        = 4,
				VALUE__MODEL_VAN_DER_WAALS  = 5,
				VALUE__MODEL_REMOVE         = 6,
				VALUE__MODEL_BACKBONE       = 7,
				VALUE__SELECT               = 20,
				VALUE__DESELECT             = 21,
				
				VALUE__DRAWING_PRECISION_LOW    = 0,
				VALUE__DRAWING_PRECISION_MEDIUM = 1,
				VALUE__DRAWING_PRECISION_HIGH   = 2,
				
				VALUE__DRAWING_MODE_DOTS      = 0,
				VALUE__DRAWING_MODE_WIREFRAME = 1,
				VALUE__DRAWING_MODE_SOLID     = 2
			};
			
			enum ColorCalculatorValues
			{
				COLORCALCULATOR_VALUES__ELEMENT          = 0,
				COLORCALCULATOR_VALUES__RESIDUE_NAME     = 1,
				COLORCALCULATOR_VALUES__ATOM_CHARGE      = 2,
				COLORCALCULATOR_VALUES__CUSTOM           = 3
			};

			int getValue_(int address);
			void setValue_(int address, int value);
			bool hasValue_(int address, int value);

			virtual void applyOn_(Composite& composite);
			virtual void setColorCalculator_(ColorCalculatorValues values, const ColorRGBA &first_color = ColorRGBA(),
																			 const ColorRGBA &second_color = ColorRGBA(), const ColorRGBA &third_color = ColorRGBA())
																			 throw(InvalidOption);

			virtual void setColorCalculator_(ColorCalculator& color_calculator);

			virtual void setupStaticProcessor_()
				throw(InvalidOption);

			virtual void setupDynamicProcessor_()
				throw(InvalidOption);

			// --------------------------------------------------------------------------------
			// attributs
			// --------------------------------------------------------------------------------
			
			int id_;
			
			QString  					model_string_static_;
			QString  					model_string_dynamic_;
			QString   				precision_string_static_;
			QString   				precision_string_dynamic_;
			QString   				coloring_method_string_;
			ColorRGBA 				custom_color_;
			
			bool 							distance_coloring_;
			
			// --------------------------------------------------------------------------------
			// Attributs for the model processors
			// --------------------------------------------------------------------------------
		
			// general
			vector<int> 								address_array_;

			// model specific
			FragmentDB 									fragmentdb_;
			ColorCalculator*						color_calculator_;
			GLAtomBondModelConnector 		model_connector_;

			AddGLBallAndStickModel 			ball_and_stick_model_static_;
			AddGLBallAndStickModel 			ball_and_stick_model_dynamic_;
			AddGLSurfaceModel 					surface_model_static_;
			AddGLSurfaceModel 					surface_model_dynamic_;
			AddGLVanDerWaalsModel 			van_der_waals_model_static_;
			AddGLVanDerWaalsModel 			van_der_waals_model_dynamic_;
			AddGLBackboneModel 					backbone_model_static_;
			AddGLBackboneModel 					backbone_model_dynamic_;
			RemoveModel 								remove_model_static_;
			RemoveModel 								remove_model_dynamic_;
			AddGLLineModel 							line_model_static_;
			AddGLLineModel 							line_model_dynamic_;

			BaseModelProcessor* 				static_base_model_pointer_;
			BaseModelProcessor* 				dynamic_base_model_pointer_;

			ElementColorCalculator      element_color_calculator_;
			ResidueNameColorCalculator  residue_name_color_calculator_;
			AtomChargeColorCalculator   atom_charge_color_calculator_;
			AtomDistanceColorCalculator distance_color_calculator_;
			CustomColorCalculator       custom_color_calculator_;
		};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/MOLVIEW/GUI/DIALOGS/displayProperties.iC>
#		endif 

	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_GUI_DIALOGS_DISPLAYPROPERTIES_H
