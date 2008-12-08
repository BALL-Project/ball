// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: message.h,v 1.69.16.1 2007/03/25 21:26:00 oliver Exp $
//

#ifndef BALL_VIEW_KERNEL_MESSAGE_H
#define BALL_VIEW_KERNEL_MESSAGE_H

#ifndef BALL_VIEW_KERNEL_STAGE_H
#	include <BALL/VIEW/KERNEL/stage.h>
#endif

#ifndef BALL_VIEW_KERNEL_COMMON_H
# include <BALL/VIEW/KERNEL/common.h>
#endif

class QWidget;

namespace BALL
{
	class Composite;
	class ConformationSet;

	namespace VIEW
	{
		class Dataset;
		class ConnectionObject;
		class GeometricObject;

/** @addtogroup ViewKernelConnectivityMessages */
//@{

/** General Message class.
		Message is the base class of all message classes and provides a base interface.
		Message classes will be sent through the ConnectionObject tree for
		the purpose of communication between several ConnectionObject classes.
		Each ConnectionObject handels in its <b>onNotify()</b> method messages
		that are relevant for that class by means of RTTI. 
		This class can be given a sender object that is a pointer to the 
		ConnectionObject that has initially sent the message. This methods
		concerning the sender object are internally used and no user interaction
		is necessary. A message can be deletable or not. If a message is deletable
		it will be deleted after all ConnectionObject objects has been informed
		of this message. Therefore a deletable message must be created with the <b> new</b>
		command.
		All messages should be created with the <b> new</b> command, since they will be
		automatically deleted after all ConnectionObjects have been notified.
		See ConnectionObject for further information concerning message handling.
		and message posting. \par
*/
class BALL_VIEW_EXPORT Message
{
	public:

	/**	@name	Constructors and Destructor
	*/	
	//@{

	/** Default Constructor.
			The state of this message is:
				-  sender    - set to 0
				-  deletable - set to <tt> false</tt> 
	*/
	Message()
		throw();

	/** Copy constructor.
	*/
	Message(const Message& message)
		throw();

	/** Destructor.
	*/
	virtual ~Message()
		throw();

	//@}
	/**	@name	Accessors: inspectors and mutators 
	*/
	//@{

	/** Change the sender.
			This method will be used internally
			by ConnectionObject::notify_.
			\see         ConnectionObject::notify_
	*/
	void setSender(const ConnectionObject* sender)
		throw();

	/** Non-mutable inspection of the sender.
			This method will be used internally
			by ConnectionObject::onNotify.
			\see         ConnectionObject::onNotify
	*/
	const ConnectionObject* getSender() const
		throw();

	/** Change the deletable flag.
			If the parameter <b> flag</b> is set to <tt> true</tt> the message is set to deletable
			and will be automatically deleted after all ConnectionObject objects
			are notified of this message. It is important that the message
			is created by the <b> new</b> command to avoid segmentation faults.\par
			If the parameter <b> flag</b> is set to <tt> false</tt> the message will not be 
			automatically deleted.\par
			Avoid creating non-deletable messages because it can result in
			segmentation faults.
			\param       flag the new deletable state of this message
	*/
	void setDeletable(bool flag = true)
		throw();

	/** Test if this message is deletable.
	*/
	bool isDeletable() const
		throw();
	//@}

	private:

	const ConnectionObject* connection_object_;

	bool deletable_;
};


/** Message to notify about changes in a Dataset.
 		This message can be e.g. used to notify all ModularWidget about a new 
		DockResult, Trajectory or any other kind of dataset. The Dataset contains the
		information on the kind of data it contains. The receiving widget thus has to
		query the type of the message and the the type of the Dataset.
		@see Dataset
		@see DatasetController
		@see DatasetControl
*/
class DatasetMessage
	: public Message
{
	public:

		/// Message type enumeration
		enum Type
		{
			/// Default Value
			UNDEFINED = -1,

			/// To add 
			ADD = 0,

			/// To be removed 
			REMOVE,

			/// Update 
			UPDATE,

			/// Was selected in Control
			SELECTED,

			///
			VISUALIZE,

			/// Allow >90 different kinds of visualization
			VISUALIZE_END = 100
		};

		///
		DatasetMessage(Dataset* set = 0, Type type = UNDEFINED);

		///
		DatasetMessage(const DatasetMessage& msg);

		///
		Type getType() const { return type_;}

		///
		void setType(Type type) { type_ = type;}

		///
		Dataset* getDataset() const { return dataset_;}

		///
		void setDataset(Dataset* set) { dataset_ = set;}

		///
		bool isValid() const;

		protected:

		Dataset* dataset_;
		Type 		 type_;
		String 	 dataset_type_;
};

				

/** CompositeMessage is the base class of all messages concerning the change of one Composite.
		With it ConnectionObject can notify and react to Composite changes.
*/
class BALL_VIEW_EXPORT CompositeMessage
	: public Message
{
	public:

	///
	enum CompositeMessageType
	{
		/// Undefinded (default) type
		UNDEFINED = -1,

		/** A new composite to be added 
		 		MolecularStructure will add bonds, normalize names and send a msg with type NEW_MOLECULE afterwards.
		*/
		NEW_COMPOSITE,
		
		/// A Composite to be removed
		REMOVED_COMPOSITE,
		
		/// Update all datas for a Composite (but not in the MolecularControl)
		CHANGED_COMPOSITE,
		
		/// Update all datas for a composite (also in MolecularControl)
		CHANGED_COMPOSITE_HIERARCHY,
		
		/// selected a composite (e.g. per checkboxes in MolecularControl)
		SELECTED_COMPOSITE,
		
		/// deselected a composite (e.g. per checkboxes in MolecularControl)
		DESELECTED_COMPOSITE,
		
		/// center the camera on a composite
		CENTER_CAMERA,

		/** MolecularControl will add the Composite.
		 * 	DisplayProperties will create a Representation
		*/
		NEW_MOLECULE
	};

	/**	@name	Constructors and Destructors
	*/	
	//@{

	/** Default Constructor.
			The state is set to:
				-  composite       - set to 0
				-  composite name  - set to "" 
	*/
	CompositeMessage()
		throw();

	///
	CompositeMessage(const Composite& composite, CompositeMessageType type, bool update_representations = true)
		throw();

	/// Copy constructor.
	CompositeMessage(const CompositeMessage& message)
		throw();

	/// Destructor.
	virtual ~CompositeMessage()
		throw();

	//@}
	/**	@name	Accessors: inspectors and mutators 
	*/
	//@{
	
	/** Change the composite.
	*/
	void setComposite(const Composite& composite)
		throw();

	/** Inspection of the composite.
	*/
	Composite* getComposite() const
		throw();

	/** Change the name of the composite.
			\param       name the new name of the composite of this compositeMessage
	*/
	void setCompositeName(const String& name)
		throw();

	/** Inspection of the name of the composite.
	*/
	const String& getCompositeName() const
		throw();

	///
	void setType(CompositeMessageType type)
		throw() { type_ = type;}

	///
	CompositeMessageType getType() const
		throw() { return type_;}

	///
	void setUpdateRepresentations(bool state)
		throw() { update_representations_ = state;}

	///
	bool updateRepresentations() const
		throw() { return update_representations_;}

	///
	void setShowSelectionInfos(bool state)
		throw() { show_selection_infos_ = state;}

	///
	bool showSelectionInfos()
		throw() { return show_selection_infos_;}

	//@}

	protected:

	CompositeMessageType 	type_;
	Composite* 						composite_;
	String 								composite_name_;
	bool 									update_representations_;
	bool 									show_selection_infos_;
};


/** SceneMessage is the message class that is responsible for
		changing the content of the Scene.
		ConnectionObject objects that would like to have the Scene focus on another
		Composite or would like to update the contents of the Scene
		(because of some changes in the composite structure) will sent this message.
		There are methods available that will tell the Scene to update its contents
		or change the camera positions.\par
		Send by MainControl, GeometricControl and several dialogs. \par
		Received by Scene
*/
class BALL_VIEW_EXPORT SceneMessage: public Message
{
	public:

	/**	@name	Enumeration
	*/	
	//@{
	
	/// Types for SceneMessages
	enum SceneMessageType
	{
		///
		UNDEFINED = 0,

		/// Rebuild the GLDisplayList objects in the GLRenderer
		REBUILD_DISPLAY_LISTS,
		
		/// Redraw from the GLDisplayList objects
		REDRAW,

		/// Move the Camera in the Scene to the value in this message
		UPDATE_CAMERA,
		
		/// Remove the coordinate system in the Scene.
		REMOVE_COORDINATE_SYSTEM,

		/// Export a PNG
		EXPORT_PNG,

		/// Export a POVRay
		EXPORT_POVRAY,

		/// 
		ENTER_ROTATE_MODE,

		/// 
		ENTER_PICKING_MODE,

		/// Move Composites or a Clipping Plane
		ENTER_MOVE_MODE
	};

	//@}
	/**	@name	Constructors and Destructors
	*/	
	//@{

	/** Default Constructor.
			The state of this sceneMessage is set to:
				-  type - UNDEFINED
				-  camera - set to defaults
			\par
	*/
	SceneMessage(SceneMessageType type = UNDEFINED)
		throw();

	/** Copy constructor.
	*/
	SceneMessage(const SceneMessage& message)
		throw();

	/** Destructor.
	*/
	virtual ~SceneMessage()
		throw();

	//@}
	/**	@name	Accessors: inspectors and mutators 
	*/
	//@{
	
	/// Set the type of the Message
	void setType(SceneMessageType type)
		throw();

	/// Get the type of the message
	SceneMessageType getType() const
		throw() { return type_;}

	/** Set the Stage in this message.
	*/
	void setStage(Stage stage) 
		throw() { stage_ = stage;}
	
	/// Get the Stage in this message.
	Stage& getStage() 
		throw() { return stage_;}

	/// Get the Stage in this message.
	const Stage& getStage() const
		throw() { return stage_;}

	//@}

	private:

	SceneMessageType  type_;
	Stage 			 			stage_;
};


/** GenericSelectionMessage class.
		GenericSelectionMessage is a container for Composite objects that are somehow selected.
		It will be sent by existing ConnectionObject objects that collect
		Composites objects for a certain purpose.
*/
class BALL_VIEW_EXPORT GenericSelectionMessage: public Message
{
	public:

	/**	@name	Constructors and Destructors
	*/	
	//@{

	/** Default Constructor.
			The state is set to:
				-  composite list - empty
	*/
	GenericSelectionMessage()
		throw();

	/** Copy constructor.
	*/
	GenericSelectionMessage(const GenericSelectionMessage& message)
		throw();

	/** Destructor.
	*/
	virtual ~GenericSelectionMessage()
		throw();
					
	//@}
	/**	@name	Accessors: inspectors and mutators 
	*/
	//@{

	/** Change the selection of Composite objects.
			The selection list will be copied from the given list.
	*/
	void setSelection(const List<Composite*>& selection)
		throw();

	/** Non-mutable inspection of the selection of Composite objects.
	*/
	const List<Composite*>& getSelection() const
		throw();

	/** mutable inspection of the selection of Composite objects.
	*/
	List<Composite*>& getSelection()
		throw();

	//@}

	private:

	List<Composite*> selection_;
};


/** Used to inform MainControl of selection in MolecularControl (not the one of the checkboxes!) 
 		and the other way round. \par
	  MolecularControl -> MainControl and \par
		MainControl -> MolecularControl
 */
class BALL_VIEW_EXPORT ControlSelectionMessage: public GenericSelectionMessage
{
	public:
	ControlSelectionMessage()
		throw();
};


/** Send by MainControl to Control objects to sync selection
 */
class BALL_VIEW_EXPORT NewSelectionMessage: public Message
{
	public:
	NewSelectionMessage()
		throw();

	/// Open all SelectableListViewItems, if they have a selected child
	bool openItems()
		throw() { return open_;}

	///
	void setOpenItems(bool state)
		throw() { open_ = state;}

	protected:
	 bool open_;
};

/** GeometricObjectSelectionMessage class. \par
		Sent by Scene after picking GeometricObject. \par
		Caught by MainControl.
*/
class BALL_VIEW_EXPORT GeometricObjectSelectionMessage: public Message
{
	public:

	/**	@name	Constructors and Destructors
	*/	
	//@{

	/** Default Constructor.
	*/
	GeometricObjectSelectionMessage()
		throw();

	/** Destructor.
	*/
	virtual ~GeometricObjectSelectionMessage()
		throw();

	//@}
	/**	@name	Accessors: inspectors and mutators 
	*/
	//@{
	
	/** Change the selection of Composite objects.
			The selection list will be copied from the given list.
	*/
	void setSelection(const List<GeometricObject*>& selection)
		throw() { selection_ = selection;}

	/** Non-mutable inspection of the selection of Composite objects.
	*/
	const List<GeometricObject*>& getSelection() const
		throw() { return selection_;}

	/// Set the GeometricObject's to be selected or deselected
	void setSelected(bool state)
		throw() { state_ = state;}

	/// Query if the GeometricObject are selected or deselected
	bool isSelected() const
		throw() { return state_;}

	//@}

	private:

	List<GeometricObject*> selection_;

	bool state_;
};


class Representation;

/// Base class for all messages concerning a Representation
class BALL_VIEW_EXPORT RepresentationMessage: public Message
{
	public:

	/// Types of RepresentationMessage
	enum RepresentationMessageType
	{
		/// Default Value
		UNDEFINED = -1,

		/// Add a Representation
		ADD = 0,

		/// Remove a Representation
		REMOVE,

		/// Update the Representation
		UPDATE,

		/// Representation was selected in GeometriControl
		SELECTED,

		/// Started the update of a Representation, no changes allowed in GeometricControl
		STARTED_UPDATE,

		/// Finished the update of a Representation
		FINISHED_UPDATE,

		/// Add a Representation to GeometricControl, but dont update in Scene
		ADD_TO_GEOMETRIC_CONTROL
	};

	///
	RepresentationMessage()
		throw();
	
	///
	virtual ~RepresentationMessage()
		throw();

	///
	RepresentationMessage(Representation& rep, RepresentationMessageType type)
		throw();

	///
	void setRepresentation(Representation& rep)
		throw() {representation_ = &rep;}

	///
	Representation* getRepresentation() 
		throw() {return representation_;}

	///
	void setType(RepresentationMessageType type)
		throw();

	///
	RepresentationMessageType getType() const
		throw() { return type_;}
	
	private:

	Representation* 					representation_;
	RepresentationMessageType type_;
};
	

/** Message to perform specific tasks for molecular items.\par
		Send by MolecularControl to MolecularProperties.
 */
class BALL_VIEW_EXPORT MolecularTaskMessage
	: public Message
{
	public:

	/// Enum for the different molecular tasks
	enum MolecularTaskMessageType
	{
		///
		UNDEFINED = -1,
		///
		BUILD_BONDS,
		///
		CHECK_RESIDUE,
		///
		ADD_HYDROGENS,
		///
		CREATE_DISTANCE_GRID
	};

	///
	MolecularTaskMessage(MolecularTaskMessageType type = UNDEFINED)
		throw();

	///
	void setType(MolecularTaskMessageType type)
		throw();

	///
	MolecularTaskMessageType getType() const
		throw() {return type_;}

	protected:
	
	MolecularTaskMessageType type_;
};


/** Notify the DisplayProperties dialog to show itself.\par
 		Send by the GeometriControl and MolecularControl.
*/
class BALL_VIEW_EXPORT ShowDisplayPropertiesMessage
	:public Message
{
	public:
		///
		ShowDisplayPropertiesMessage()
			: Message(){};
};

/** Notify the DisplayProperties dialog so that it creates a new Representation. \par
 		Send by the MolecularControl.
*/
class BALL_VIEW_EXPORT CreateRepresentationMessage
	:public Message
{
	public:
		///
		CreateRepresentationMessage()
			throw();

		///
		CreateRepresentationMessage(const List<Composite*>& composites, 
																ModelType model_type, 
																ColoringMethod coloring_method)
			throw();   

		///
		const List<Composite*>& getComposites() const
			throw() { return composites_;}

		///
		ModelType getModelType() const
 			throw() { return model_type_;}
		
		///
		ColoringMethod getColoringMethod() const
			throw() { return coloring_method_;} 
	
 		private: 
		List<Composite*> 	composites_;
		ModelType 				model_type_;
		ColoringMethod 		coloring_method_;
};
	

/** Message send by one GenericControl to notify all other GenericControl instances to
 		deselect their QListView.
*/
class BALL_VIEW_EXPORT DeselectControlsMessage
	: public Message
{
	public:
		DeselectControlsMessage()
			: Message() {};
};


///
class BALL_VIEW_EXPORT TransformationMessage
	: public Message
{
	public:

	///
	TransformationMessage()
		throw();

	///
	TransformationMessage(const Matrix4x4& m)
		throw();

	///
	void setMatrix(const Matrix4x4& m)
		throw() { matrix_ = m;}

	///
	const Matrix4x4& getMatrix() const
		throw() { return matrix_;}

	private:

	Matrix4x4 matrix_;
};


///
class BALL_VIEW_EXPORT FinishedSimulationMessage
	: public Message
{
	public:

	///
	FinishedSimulationMessage()
		throw();
};
	

///
class BALL_VIEW_EXPORT SyncClippingPlanesMessage
	: public Message
{
	public:

	///
	SyncClippingPlanesMessage()
		throw() {};
};


///
class BALL_VIEW_EXPORT ShowHelpMessage
	: public Message
{
	public:

	///
	ShowHelpMessage(String url = "", String project = "BALLView", String entry = "")
		throw();

	String getURL() const { return url_;}

	String getProject() const { return project_;}

	String getEntry() const { return entry_;}

	protected:

	String url_;
	String project_;
	String entry_;
};


/** Register a QObject to a URL in the documentation
 		This message is send by ModularWidget::registerForHelpSystem and should
		probably not be used otherwise.
*/
class BALL_VIEW_EXPORT RegisterHelpSystemMessage
	: public Message
{
	public:

	///
	RegisterHelpSystemMessage()
		throw();

	///
	void setObject(const QObject* object) { object_ = object;}

	///
	void setURL(const String& url) { url_ = url;}

	///
	void setRegisterMode(bool state) { register_ = state;}

	///
	const QObject* getObject() const { return object_;}

	///
	const String& getURL() const { return url_;}

	///
	bool isRegister() const { return register_;}

	protected:

	const QObject* object_;
	Index 	 menu_entry_;
	String 	 url_;
	bool  	 register_;
};



/// Message to notify docking has finished
class BALL_VIEW_EXPORT DockingFinishedMessage
	:public Message
{
	public:
		///
		DockingFinishedMessage()
			throw();

		///
		DockingFinishedMessage(bool abort)
			throw();
			 
		///
		virtual ~DockingFinishedMessage()
			throw();
			
		///
		void setConformationSet(const ConformationSet* conformation_set)
		{
			conformation_set_ = conformation_set;
		}
		
		//
		const ConformationSet* getConformationSet() const { return conformation_set_; }
		
		///
		bool wasAborted() { return abort_; }

	protected:

		/// this conformation set is deleted in DockResult
		const ConformationSet* conformation_set_;
		bool abort_;
};

//@}

#	ifndef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/VIEW/KERNEL/message.iC>
#	endif
  			
} } // namespaces

#endif // BALL_VIEW_KERNEL_MESSAGE_H
