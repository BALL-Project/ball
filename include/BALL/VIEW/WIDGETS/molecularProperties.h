// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: molecularProperties.h,v 1.7 2003/10/15 14:29:03 amoll Exp $

#ifndef BALL_VIEW_WIDGETS_MOLECULARPROPERTIES_H
#define BALL_VIEW_WIDGETS_MOLECULARPROPERTIES_H

#ifndef BALL_VIEW_KERNEL_MODULARWIDGET_H
# include <BALL/VIEW/KERNEL/modularWidget.h>
#endif

#ifndef BALL_MATHS_VECTOR3_H
 #include <BALL/MATHS/vector3.h>
#endif

#include <qwidget.h>

namespace BALL
{
	class Composite;

	namespace VIEW
	{

/**	MolecularProperties is a widget that reacts to Messages itself and converts some
		to other Message objects.
		This class is necessary to convert new Composite objects opened with either
		openPDBFile or openHINFile to Composite objects
		that have certain molecular properties. Further it converts the geometric selection
		sent by Scene to a molecular selection whose objects can be given a new
		graphical representation by the DisplayProperties dialog.
		See onNotify for information concerning the conversion mechanism. \par
		\ingroup ViewWidgets
*/
class MolecularProperties
	: public QWidget, 
		public ModularWidget
{
	Q_OBJECT
	BALL_EMBEDDABLE(MolecularProperties)

	public:
	
	/**	@name	Constructors
	*/	
	//@{

	/** Default Constructor.
			Calls registerWidget.
			\param      parent the parent widget of this molecularProperties 
			\param      name the name of this molecularProperties 
			\return     MolecularProperties new constructed molecularProperties
			\see        QWidget
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
			Converts NewCompositeMessage to NewMolecularMessage if the
			retrieved Composite object is kind of AtomContainer and
			applies molecular properties to it (like normalize_names and
			build_bonds).\par
			\param message the pointer to the message that should be processed
			\see   Message
			\see   NewCompositeMessage
			\see   NewMolecularMessage
			\see   GeometricObjectSelectionMessage
			\see   ConnectionObject
	*/
	void onNotify(Message *message)
		throw();


	/**	Check the menu entries.
			The menus <b> Select</b>, <b> Deselect</b>, <b> Add Hydrogens</b> and <b> Build Bonds</b>
			will be enabled if the selection of molecular objects is not empty.
			The menu <b> Focus camera</b> will be enabled only if only one molecular object
			is in the selection list.
	*/
	void checkMenu(MainControl& main_control)
		throw();

	public slots:

	/** Centers the camera of Scene to the geometric center of the molecular objects
			in the selection list.
			The messages WindowMessage and SceneMessage will
			be sent to inform the MainControl and the Scene about the change.
	*/
	void centerCamera(Composite* composite = 0);

	/** Creates bonds.
			If selected molecular objects are available Bond objects will be created
			for each object in the selection list
			using the build_bonds processor of the FragmentDB
			The message ChangedCompositeMessage will be sent for each object in the
			selection list. The messages WindowMessage and SceneMessage will
			be sent to inform the MainControl and the Scene about the change.
			The number of bonds created will be written into the Log object.
	*/
	void buildBonds();
	
	/** Adds hydrogens.
			If selected molecular objects are available hydrogens will be created
			for each object in the selection list
			using the add_hydrogens processor of the FragmentDB
			The message ChangedCompositeMessage will be sent for each object in the
			selection list. The messages WindowMessage and SceneMessage will
			be sent to inform the MainControl and the Scene about the change.
			The number of hydrogens created will be written into the Log object.
	*/
	void addHydrogens();
	
	/** Colors selected objects uniquely.
			If selected molecular objects are available they will be colored according to
			the selected color as specified in GeometricObject.
			The message ChangedCompositeMessage will be sent for each object in the
			selection list. The messages WindowMessage and SceneMessage will
			be sent to inform the MainControl and the Scene about the change.
	*/
	void select();

	/** Colors deselected objects in their own color.
			If selected molecular objects are available they will be colored according to
			their own color as specified in the objects. This method reverses the process
			done in the select method.
			The message ChangedCompositeMessage will be sent for each object in the
			selection list. The messages WindowMessage and SceneMessage will
			be sent to inform the MainControl and the Scene about the change.
	*/
	void deselect();

	/** Check the residues
	 */
	virtual bool checkResidue();

	///
	virtual void createGridFromDistance();

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
				create_distance_grid_id_;

	
	Vector3 										view_center_vector_;
	Real 												view_distance_;

};

} } // namespaces

#endif // BALL_VIEW_WIDGETS_MOLECULARPROPERTIES_H
