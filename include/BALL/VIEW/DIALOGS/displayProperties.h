// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: displayProperties.h,v 1.6 2003/09/01 10:28:53 amoll Exp $

#ifndef BALL_VIEW_DIALOGS_DISPLAYPROPERTIES_H
#define BALL_VIEW_DIALOGS_DISPLAYPROPERTIES_H

#ifndef BALL_VIEW_MODELS_STANDARDCOLORPROCESSOR_H
#	include <BALL/VIEW/MODELS/standardColorProcessor.h>
#endif

#ifndef BALL_VIEW_KERNEL_MODULARWIDGET_H
# include <BALL/VIEW/KERNEL/modularWidget.h>
#endif

#include <BALL/VIEW/UIC/displayPropertiesData.h>

namespace BALL
{
	namespace VIEW
	{

class Representation;

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
		The follwing properties are available:\par \par
		The models:
			-  AddGLBallAndStickModel the ball and stick model
			-  AddGLBackboneModel the backbone model
			-  AddGLLineModel the line model
			-  AddGLSurfaceModel the surface model
			-  AddGLVanDerWaalsModel the van der waals model
		\par
		The coloring methods:
			-  ElementColorProcessor 
						 		colors the molecular objects according to their atom elements
			-  ResidueNameColorProcessor 
						 		colors the molecular objects according to their residue names
			-  AtomChargeColorProcessor 
								colors the molecular objects according to their atom charge
			-  AtomDistanceColorProcessor 
								colors the molecular objects according to their atom distance
			-  CustomColorProcessor 
							 colors the molecular objects according to selected color
		\par
		For information about the drawing precision see GeometricObject.\par
		This dialog is also responsible for selecting and deselecting,
		building bonds and adding hydrogens to molecular objects. Further it is possible
		to center the camera of the Scene to the geometric center of the selection 
		of molecular objects.
		The class MolecularControl is responsible for creating such a selection.
		If this dialog is used, it should be created with MainControl as parent.
		The class DisplayPropertiesData contains the definition of the layout of
		this dialog and is therefore not necessary for understanding. \par
*/
class DisplayProperties 
	: public DisplayPropertiesData,
		public ModularWidget
{
	Q_OBJECT
		
	public:
	
	BALL_EMBEDDABLE(DisplayProperties)

	/**	@name	Constructors and Destructors
	*/	
	//@{

	/** Default Constructor.
			Constructs new displayProperties.
			Sets the combo box of the coloring method to the ElementColorProcessor.
			This combo box as well as the others can be set differently according to saved
			preferences. See fetchPreferences.
			Calls registerWidget.
			\param      parent the parent widget of this displayProperties 
			\param      name the name of this displayProperties 
			\return     DisplayProperties new constructed displayProperties
			\see        fetchPreferences
			\see        ElementColorProcessor
			\see        QDialog
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
				-  ChangedMolecularMessage to indicate that the Composite object has changed.
				-  SceneMessage to force an update of the Scene (and set the camera to new object)
			\par
			If MolecularSelectionMessage is catched the selection of this object will 
			be used for changing the visualization.
			If such a message is catched the apply button will be enabled and the graphical
			representation of the selection can be changed.
			\param message the pointer to the message that should be processed
			\see   applyButtonClicked
			\see   NewMolecularMessage
			\see   ChangedMolecularMessage
			\see   MolecularSelectionMessage
			\see   SceneMessage
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
			Fetches the preferences (the position, the custom color, the model, the drawing 
			precision, the drawing mode and the coloring method of this
			displayProperties) from the	INIFile <b>inifile</b>.
			This method will be called inside the method show from the class
			MainControl.
			\param  inifile the INIFile that contains the needed information for 
							this displayProperties
			\see    writePreferences
			\see    MainControl
	*/
	virtual void fetchPreferences(INIFile &inifile)
			throw();
		
	/** Writes the preferences.
			Writes the preferences (the position, the custom color, the model, the drawing 
			precision, the drawing mode and the coloring method of this
			displayProperties) to the	INIFile <b> inifile.
			This method will be called inside the method aboutToExit from the class
			MainControl.
			\param  inifile the INIFile to be written into
			\see    fetchPreferences
	*/
	virtual void writePreferences(INIFile &inifile)
			throw();
		
	/**	Initializes the widget.
			Initializes the popup menu <b> Display</b> with its checkable submenus
				-  <b> Display Properties</b> - opens the dialog (indicates if open)
				-  <b> Select</b> - 
							 marks the selected molecular objects in the selected color 
				-  <b> Deselect</b> - 
							 uses the previously set color of the selected molecular objects
				-  <b> Focus Camera</b> - centers the camera of Scene to the geometric center 
								of the molecular objects in the selection
				-  <b> Build Bonds</b> - 
							 generates the Bond object to the molecular objects in the selection
				-  <b> Add Hydrogens</b> - adds hydrogens to the molecular objects in the selection
			\par
			and adds them to the appropriate slots.
			This method is called automatically	immediately before the main application is started. 
			This method will be called by show from the MainControl object.
			\param main_control the MainControl object to be initialized 
						 with this displayProperties
	*/
	virtual void initializeWidget(MainControl& main_control)
			throw();
		
	/**	Removes the widget.
			Removes the checkable submenus from the popup menu
			<b> Display</b>	and cut the all previously registered connections.
			This method will be called by aboutToExit from the MainControl object.
			\param main_control the MainControl object to be finalized with 
						 this displayProperties
			\see   checkMenu
			\see   removeMenuEntry
	*/
	virtual void finalizeWidget(MainControl& main_control)
			throw();
		
	/**	Menu checking method.
			This method is called by the method checkMenus from the
			MainControl object before a popup menu is shown.
			The menu <b> Display Properties</b> will be checked
			if this displayProperties is visible. 
			The menus <b> Select</b>, <b> Deselect</b>, <b> Add Hydrogens</b> and <b> Build Bonds</b>
			will be enabled if the selection of molecular objects is not empty.
			The menu <b> Focus camera</b> will be enabled only if only one molecular object
			is in the selection list.
			\param main_control the MainControl object whose menus should be checked
	*/
	virtual void checkMenu(MainControl& main_control)
			throw();

	void setRepresentation(Representation* rep)
		throw() {rep_ = rep;}
		
	public slots:
			
	//@} 
	/** @name Public slots 
	*/ 
	//@{

	/** Starts the displayProperties dialog.
			Opens this displayProperties dialog.
			Calls show and raise from QDialog class.
			See documentation of QT-library for information concerning QDialog widgets.
	*/
	void showDialog();

	///
	void createRepresentationMode();

	///
	void modifyRepresentationMode();
		
	protected slots:
			
	//@} /** @name Protected slots */ //@{
			
	/** Changes the drawing precision.
			This slot is connected to the drawing precision combo box and will be automatically
			called if the contents of this combo box is changed.
			This method changes the drawing precision of the selected model.
			\param   string the string containing the new drawing precision
	*/
	virtual void selectPrecision(int index);

	/** Changes the model.
			This slot is connected to the model combo box and will be automatically
			called if the content of this combo box is changed.
			\param   string the string containing the new model
	*/
	virtual void selectModel(int index);

	/** Changes the drawing mode.
			This slot is connected to the mode combo box and will be automatically
			called if the content of this combo box is changed.
			\param   string the string containing the new mode
	*/
	virtual void selectMode(int index);

	/** Changes the coloring method.
			This slot is connected to the coloring method combo box and will be automatically
			called if the content of this combo box is changed.
			This method changes the coloring method of the selected model.
			\param   string the string containing the new coloring method
	*/
	virtual void selectColoringMethod(int index);
	
	/** Indicates the apply button was pressed.
			Applies the selected model with its selected properties to each Composite object
			available in the molecular selection list.
			The message ChangedCompositeMessage will be sent for each Composite
			object in the	selection list.
			The messages WindowMessage and SceneMessage will
			be sent to inform the MainControl and the Scene about the change.
	*/
	virtual void applyButtonClicked();
	
	/** Opens the edit color dialog.
			Opens the dialog for editing the color in which the model
			should be drawn (if coloring method: custom was chosen).
			Opens a QColorDialog from the QT-library.
			See documentation of QT-library for information concerning QColorDialog widgets.
			\see   selectColoringMethod
	 */ 
	virtual void editColor();

	/** Opens the color dialog for the color of selected items.
			\see BALL_SELECTED_COLOR
	*/
	virtual void editSelectionColor();

	//@}
		
	protected:
	
	/*_ Set the selection of a given combobox to a given value */
	void setComboBoxIndex_(QComboBox* combo_box, QString& item_string);

	// --------------------------------------------------------------------------------
	// methods for the model processors

	/*_ Create the new representation, either for one composite, (if one is given) or for
	 		all in the Control selected composites.
			Called by onNotify() after receiving NewMolecularMessage and by applyButtonClicked().
	*/
	virtual void createRepresentation_(const Composite* composite = 0)
		throw(InvalidOption);

	//_ Read a inifile entry and set the accoring attribut and ComboBox.
	virtual void getEntry_(INIFile& inifile, const String& key, Index& assign_to, QComboBox& box);

	// --------------------------------------------------------------------------------
	// attributs
	// --------------------------------------------------------------------------------
	
	int 				id_;
	
	Index 			precision_;
	Index 			mode_;
	Index 			coloring_method_;
	Index 			model_type_;

	// used by GeometricControl to modify existing representation
	Representation* rep_;

	ColorRGBA 	custom_color_;
};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_DIALOGS_DISPLAYPROPERTIES_H
