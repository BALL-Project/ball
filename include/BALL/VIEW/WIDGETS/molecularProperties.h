// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: molecularProperties.h,v 1.14 2004/01/20 15:08:41 amoll Exp $

#ifndef BALL_VIEW_WIDGETS_MOLECULARPROPERTIES_H
#define BALL_VIEW_WIDGETS_MOLECULARPROPERTIES_H

#ifndef BALL_VIEW_KERNEL_MODULARWIDGET_H
# include <BALL/VIEW/KERNEL/modularWidget.h>
#endif

#ifndef BALL_MATHS_VECTOR3_H
 #include <BALL/MATHS/vector3.h>
#endif

#ifndef BALL_VIEW_DIALOGS_AMBERCONFIGURATIONDIALOG_H
 #include <BALL/VIEW/DIALOGS/amberConfigurationDialog.h>
#endif

#include <qwidget.h>

namespace BALL
{
	class Composite;
	class AmberFF;
	class CharmmFF;

	namespace VIEW
	{

/**	MolecularProperties is a widget that reacts to Messages itself and converts some
		to other Message objects.
		The widget itself is invisible, but it has severeal menu entries:
		- checkResidue
		- centerCamera
		- buildBonds
		- addHydrogens
		- select
		- deselect
		- createGridFromDistance
		- calculateSecondaryStructure
		This class is also necessary to add certain properties to new Composite objects, which were opened with the 
		MolecularFileDialog.
		Further it converts the geometric selection sent by Scene to a molecular selection 
		whose objects can be given a new graphical representation by the DisplayProperties dialog.
		See onNotify for information concerning the conversion mechanism. \par
		\ingroup ViewWidgets
*/
class MolecularProperties
	: public QWidget, 
		public ModularWidget
{
	Q_OBJECT

	public:

	BALL_EMBEDDABLE(MolecularProperties, ModularWidget)

	
	/**	@name	Constructors
	*/	
	//@{

	/** Default Constructor.
			Calls registerWidget.
			\see        ModularWidget
	*/
	MolecularProperties(QWidget* parent = 0, const char* name = 0)
		throw();
	
	//@}
	/** @name Destructors 
	*/
	//@{

	/** Destructor.
	*/
	virtual ~MolecularProperties()
		throw();

	//@}
	/**	@name	Accessors: inspectors and mutators 
	 */
	//@{

	/** Handles messages sent by other registered ConnectionObject objects.
			Converts CompositeMessage if the
			retrieved Composite object is kind of AtomContainer and
			applies molecular properties to it (like normalize_names and
			build_bonds).\par
			\param message the pointer to the message that should be processed
			\see   CompositeMessage
			\see   GeometricObjectSelectionMessage
			\see   MolecularTaskMessage
	*/
	void onNotify(Message *message)
		throw();

	/**	Check the menu entries.
			The menus <b>Select</b>, <b>Deselect</b>, <b>Add Hydrogens</b> and <b>Build Bonds</b>
			will be enabled if the selection of molecular objects is not empty.
			The menu <b>Focus camera</b> will be enabled only if only one molecular object
			is in the selection list.
	*/
	void checkMenu(MainControl& main_control)
		throw();

	/** Get the instance of the AMBER forcefield.
	 		The forcefield will be created, when this function is called the first time.
	*/
	AmberFF& getAMBERFF() 
		throw();
	
	/** Get the instance of the CHARMM forcefield.
	 		The forcefield will be created, when this function is called the first time.
	*/
	CharmmFF& getCHARMMFF() 
		throw();

	///
	AmberConfigurationDialog& getAmberConfigurationDialog()
		throw();
	
	/// Print the results of the AMBER forcefield.
	void printAmberResults()
		throw();
	
	/** Fetch the widgets preferences from the INIfile.
			\param  inifile the INIFile that contains the needed values
	*/
	virtual void fetchPreferences(INIFile &inifile)
		throw();
			
	/** Writes the widgets preferences to the INIFile.
			\param  inifile the INIFile that contains the needed values
	*/
	virtual void writePreferences(INIFile &inifile)
		throw();
			
	public slots:

	/** Centers the camera of Scene to the geometric center of the molecular objects
			in the selection list.
			A SceneMessage will be sent to inform the Scene.
	*/
	void centerCamera(Composite* composite = 0);

	/** Creates bonds.
			If selected molecular objects are available Bond objects will be created
			for each object in the selection list using the build_bonds processor of the FragmentDB.
			A CompositeMessage will be sent for each object in the
			selection list.
			The number of bonds created will be written into the Log object.
	*/
	void buildBonds();
	
	/** Adds hydrogens.
			If selected molecular objects are available hydrogens will be created
			for each object in the selection list
			using the add_hydrogens processor of the FragmentDB.
			A CompositeMessage will be sent for each object in the selection list.
			The number of hydrogens created will be written into the Log object.
	*/
	void addHydrogens();
	
	/** Colors selected objects uniquely.
			If selected molecular objects are available they will be colored according to
			the BALL_SELECTED_COLOR.
			A CompositeMessage will be sent for each object in the selection list.
	*/
	void select();

	/** Colors deselected objects in their own color.
			If selected molecular objects are available they will be colored according to
			their own color as specified in the objects. This method reverses the process
			done in the select method.
			A CompositeMessage will be sent for each object in the selection list.
	*/
	void deselect();

	/** Check the residues
	 */
	virtual bool checkResidue();

	///
	virtual void createGridFromDistance();

	///
	virtual void calculateSecondaryStructure();

	///
	virtual void mapProteins();

	//@}

	
private:
	
	virtual void calculateCenter_(Composite& composite);

	virtual void addComposite_(Composite& composite, const String& name)
		throw();

	Index center_camera_id_, 
				build_bonds_id_, 
				add_hydrogens_id_, 
				check_structure_id_,
				select_id_, 
				deselect_id_, 
				create_distance_grid_id_,
				calculate_ss_,
				map_proteins_;

	
	Vector3 										view_center_vector_;
	float 											view_distance_;
	AmberFF* 										amber_;
	CharmmFF*										charmm_;
	AmberConfigurationDialog    amber_dialog_;
};

} } // namespaces

#endif // BALL_VIEW_WIDGETS_MOLECULARPROPERTIES_H
