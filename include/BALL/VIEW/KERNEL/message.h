// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: message.h,v 1.23 2003/08/29 10:36:40 amoll Exp $
//

#ifndef BALL_VIEW_KERNEL_MESSAGE_H
#define BALL_VIEW_KERNEL_MESSAGE_H

#ifndef BALL_VIEW_KERNEL_STAGE_H
#	include <BALL/VIEW/KERNEL/stage.h>
#endif

namespace BALL
{
	class Composite;

	namespace VIEW
	{
		// class forward
		class GeometricObject;

		/** General Message class.
				Message is the base class of all message classes and provides a base interface.
				Message classes will be sent through the ConnectionObject tree for
				the purpose of communication between several connectionObject classes.
				Each ConnectionObject handels in its <b>onNotify() method messages
				that are relevant for that class by means of RTTI. 
				This class can be given a sender object that is a pointer to the 
				ConnectionObject that has initially sent the message. This methods
				concerning the sender object are internally used and no user interaction
				is necessary. A message can be deletable or not. If a message is deletable
				it will be deleted after all ConnectionObject objects has been informed
				of this message. Therefore a deletable message must be created with the <b> new</b>
				command.
				All messages should be created with the <b> new</b> command and set to deletable
				for avoiding segmentation faults.
				See ConnectionObject for further information concerning message handling
				and message posting. \par
		*/
		class Message
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
					Change the sender of this message to the value
					represented by the void parameter <b> object</b>. This parameter always represents
					a void pointer to a ConnectionObject. This method will be used internally
					by the ConnectionObject class it its <b> notify_ method.
					\param       object the new sender of this message
					\see         ConnectionObject::notify_
			*/
			void setSender(const void* object)
				throw();

			/** Non-mutable inspection of the sender.
					The return type always represents
					a void pointer to a ConnectionObject. This method will be used internally
					by the ConnectionObject class it its <b> onNotify method.
					\return      const void* constant pointer to the sender of this message
					\see         ConnectionObject::onNotify
			*/
			const void* getSender() const
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

			void *connection_object_;

			bool deletable_;
		};

		
		/** CompositeMessage is the base class of all messages concerning the change of one Composite.
				With it ConnectionObject can notify and react to Composite changes.
		*/
		class CompositeMessage: public Message
		{
			public:

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

			/** Change the composite.
			*/
			void setComposite(const Composite* composite)
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
			//@}

			protected:

			Composite* 	composite_;
			String 			composite_name_;
		};


		/** NewCompositeMessage class.
				NewCompositeMessage is responsible for
				making known a new Composite object inside the ConnectionObject tree.\par
				Sent by: MolecularFileDialog, Server.\par
				Catched by: MolecularProperties, Control, MainControl.
		*/
		class NewCompositeMessage: public CompositeMessage
		{
			public:

			/**	@name	Constructors and Destructor
			*/	
			//@{

			/** Default Constructor.
			*/
			NewCompositeMessage()
				throw();

			/** Copy constructor.
			*/
			NewCompositeMessage(const CompositeMessage& message)
				throw();

			/** Destructor.
			*/
			virtual ~NewCompositeMessage()
				throw();
			//@}
		};


		/** RemovedCompositeMessage class.
		 		Send by Server, Control.\par
				Received by Control, MainControl.
		*/
		class RemovedCompositeMessage: public CompositeMessage
		{
			public:

			/**	@name	Constructors and Destructors
			*/	
			//@{

			/** Default Constructor.
			*/
			RemovedCompositeMessage()
				throw();

			/** Copy constructor.
			*/
			RemovedCompositeMessage(const CompositeMessage& message)
				throw();

			/** Destructor.
			*/
			virtual ~RemovedCompositeMessage()
				throw();
			//@}
		};


		/** ChangedCompositeMessage class.
				ChangedCompositeMessage is responsible for
				marking a Composite object as changed inside the ConnectionObject tree.
				It will be sent by existing ConnectionObject objects that change 
				Composites objects. 
		*/
		class ChangedCompositeMessage: public CompositeMessage
		{
			public:

			/**	@name	Constructors and Destructors
			*/	
			//@{

			/** Default Constructor.
			*/
			ChangedCompositeMessage()
				throw();

			/** Copy constructor.
			*/
			ChangedCompositeMessage(const ChangedCompositeMessage& message)
				throw();

			/** Destructor.
			*/
			virtual ~ChangedCompositeMessage()
				throw();

			//@}
			/**	@name	Accessors
			*/	
			//@{

			/// Update the listview in the Control 
			bool isUpdateControl() const
				throw() { return update_control_;}

			/// Update the listview in the Control 
			void setUpdateControl(bool state)
				throw() { update_control_ = state;}
			
			protected:
			
			bool update_control_;
		};


		/** SceneMessage is the message class that is responsible for
				changing the content of the Scene.
				ConnectionObject objects that would like to have the Scene focus on another
				Composite or would like to update the contents of the Scene
				(because of some changes in the composite structure) will sent this message.
				There are methods available that will tell the Scene to update its contents
				or change the camera positions.\par
				Received by Scene
		*/
		class SceneMessage: public Message
		{
			public:

			/**	@name	Enumeration
			*/	
			//@{
			
			/// Types for SceneMessages
			enum Type
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
				REMOVE_COORDINATE_SYSTEM
			};

			//@}
			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					The state of this sceneMessage is set to:
					  -  type - UNDEFINED
						-  camera - set to defaults
					\par
			*/
			SceneMessage()
				throw();

			/** Copy constructor.
			*/
			SceneMessage(const SceneMessage& message)
				throw();

			//@}
			/** @name Destructors 
			*/
			//@{

			/** Destructor.
			*/
			virtual ~SceneMessage()
				throw();

			//@}
			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{
			
			/// Set the type of the Message
			void setType(Type type)
				throw();

			/// Get the type of the message
			Type getType() const
				throw() { return type_;}

			/** Set the Camera in this message.
			 		Only usefull with type UPDATE_CAMERA.
			*/
			void setCamera(const Camera& camera) 
				throw() { camera_ = camera; }

			/** Get the Camera in this message.
			 		Only usefull with type UPDATE_CAMERA.
			*/
			Camera& getCamera()
				throw() { return camera_;}

			/** Get the Camera in this message.
			 		Only usefull with type UPDATE_CAMERA.
			*/
			const Camera& getCamera() const
				throw() { return camera_;}

			//@}

			private:

			Type   type_;
			Camera camera_;
		};


		/** SelectionMessage class.
				SelectionMessage is a container for Composite objects that are somehow selected.
				It will be sent by existing ConnectionObject objects that collect
				Composites objects for a certain purpose.

		*/
		class SelectionMessage: public Message
		{
			public:

			/**	@name	Constructors and Destructors
			*/	
			//@{

			/** Default Constructor.
					The state of this selectionMessage is set to:
					  -  composite list - empty
			*/
			SelectionMessage()
				throw();

			/** Copy constructor.
			*/
			SelectionMessage(const SelectionMessage& message)
				throw();

			/** Destructor.
			*/
			virtual ~SelectionMessage()
				throw();
							
			//@}
			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{

			/** Change the selection of Composite objects.
					The selection list will be copied from the given list.
			*/
			void setSelection(const List<Composite*> &selection)
				throw();

			/** Non-mutable inspection of the selection of Composite objects.
			*/
			const List<Composite*>& getSelection() const
				throw();

			//@}

			private:

			List<Composite*> selection_;
		};


		/** Used to inform MainControl of selection in Control (not the one of the checkboxes!) \par
		 		Send by Control.\par
				Caught by MainControl.
		 */
		class ControlSelectionMessage: public SelectionMessage
		{
			public:
			ControlSelectionMessage()
				throw();
		};

		/** Used to inform MainControl and MolecularProperties of the selection of one composite in Control.
		 		MolecularProperties sends as answer a SceneMessage to inform the Scene.
		 */
		class CompositeSelectedMessage: public CompositeMessage
		{
			public:

			CompositeSelectedMessage(Composite* composite, bool selected) 
				throw();

			/// Query if the composite is to be selected or deselected
			bool isSelected() const
				throw() { return selected_;}
			
			/// Set if the composite is to be selected or deselected
			void setSelected(bool state)
				throw() { selected_ = state;}

			protected:

			bool selected_;
		};

		/** Send by MainControl to Control objects to sync selection
		 */
		class NewSelectionMessage: public Message
		{
			public:
			NewSelectionMessage()
				throw();
		};

		/** GeometricObjectSelectionMessage class.
		 		Send by Scene after picking GeometricObject.
		*/
		class GeometricObjectSelectionMessage: public Message
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
			void setSelection(const List<GeometricObject*> &selection)
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


		/** Message to center camera on a specific Composite.
		 		Send by MolecularControl to MolecularProperties.
		 \ingroup ViewKernelConnectivity
		 */
		class CenterCameraMessage: public CompositeMessage
		{
			public:

			///
			CenterCameraMessage()
				throw();
		};

		class Representation;

		/// Base class for all messages concerning a Representation
		class RepresentationMessage: public Message
		{
			public:

			/// Types of RepresentationMessages
			enum Type
			{
				/// Default Value
				UNKNOWN = -1,

				/// Add a Representation
				ADD = 0,

				/// Remove a Representation
				REMOVE,

				/// Update the Representation
				UPDATE
			};

			///
			RepresentationMessage()
				throw();

			///
			void setRepresentation(Representation* rep)
				throw() {representation_ = rep;}

			///
			Representation* getRepresentation() 
				throw() {return representation_;}

			///
			void setType(Type type)
				throw();

			///
			Type getType() const
				throw() { return type_;}
			
			private:

			Representation* representation_;
			Type 						type_;
		};
			
		/** NewMolecularMessage class.
				This message will be converted
				from the message NewCompositeMessage if the Composite object contains
				a molecular structure. The conversion will be done by the widget class
				 \link MolecularProperties MolecularProperties \endlink .
				There are widgets that will be react only to the class newMolecularMessage but
				not to the message class  \link NewCompositeMessage NewCompositeMessage \endlink  such as  \link DisplayProperties DisplayProperties \endlink .
				Because the RTTI mechanism of BALL is used to identify the different messages
				this class is not derived from  \link NewCompositeMessage NewCompositeMessage \endlink . Otherwise every method
				accepting  \link NewCompositeMessage NewCompositeMessage \endlink  object would also accept 
				 \link NewMolecularMessage NewMolecularMessage \endlink  objects and that is not allowed.  \par
				
    		\ingroup  ViewKernel
		*/
		class NewMolecularMessage: public CompositeMessage
		{
			public:

			/**	@name	Constructors and Destructor
			*/	
			//@{

			/** Default Constructor.
			*/
			NewMolecularMessage()
				throw();

			/** Copy constructor.
			*/
			NewMolecularMessage(const CompositeMessage& message)
				throw();

			/** Destructor.
			*/
			virtual ~NewMolecularMessage()
				throw();
			//@}
		};


		/** Message to perform specific tasks for molecular items.
		 		Send by MolecularControl to MolecularProperties.
		 */
		class MolecularTaskMessage
			: public Message
		{
			public:

			/// Enum for the different molecular tasks
			enum Type
			{
				///
				UNKNOWN = 0,
				///
				BUILD_BONDS,
				///
				CHECK_RESIDUE,
				///
				ADD_HYDROGENS
			};

			///
			MolecularTaskMessage()
				throw();

			///
			void setType(Type type)
				throw();

			///
			Index getType() const
				throw() {return type_;}

			protected:
			
			Index type_;
		};



		/** ChangedMolecularMessage class.
				The class ChangedCompositeMessage is the message class that is responsible for
				marking a  \link Composite Composite \endlink  object as changed inside the  \link ConnectionObject ConnectionObject \endlink  tree.
				It will be sent by existing  \link ConnectionObject ConnectionObject \endlink  objects that change (or would like to
				change)  \link Composites Composites \endlink  objects. One can use the  \link RTTI RTTI \endlink  mechanism to identify
				such messages in the {\em onNotify} method of user constructed  \link ConnectionObject ConnectionObject \endlink 
				objects.
				This class is only provided for convinience. So in VIEW applications one can
				use this class in the same way as the class  \link ChangedCompositeMessage ChangedCompositeMessage \endlink .  \par
				
		*/
		class ChangedMolecularMessage: public ChangedCompositeMessage
		{
			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new changedMolecularMessage.
					@return      ChangedMolecularMessage new constructed changedMolecularMessage
					@see         ChangedCompositeMessage
			*/
			ChangedMolecularMessage()
				throw();

			/** Copy constructor.
					Construct new changedMolecularMessage by copying the changedMolecularMessage {\em message}.
					The state of {\em *this} changedMolecularMessage is set to the set of {\em message} 
					changedMolecularMessage.
					@param       message the changedMolecularMessage to be copied
					@return      ChangedMolecularMessage new constructed changedMolecularMessage copied from {\em message}
					@see         CompositeMessage
					@see         ChangedCompositeMessage
			*/
			ChangedMolecularMessage(const CompositeMessage& message)
				throw();

			//@}

			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} changedMolecularMessage.
			*/
			virtual ~ChangedMolecularMessage()
				throw();
			//@}
		
			private:
		};


		/** MolecularSelectionMessage class.
				The class MolecularSelectionMessage is the message class that is a container for
				 \link Composite Composite \endlink  objects that are somehow selected.
				It will be sent by existing  \link ConnectionObject ConnectionObject \endlink  objects that collect
				 \link Composites Composites \endlink  objects for a certain purpose.
				One can use the  \link RTTI RTTI \endlink  mechanism to identify
				such messages in the {\em onNotify} method of user constructed  \link ConnectionObject ConnectionObject \endlink 
				objects.
				This class is only provided for convinience. So in VIEW applications one can
				use this class in the same way as the class  \link SelectionMessage SelectionMessage \endlink .  \par
				
		*/
		class MolecularSelectionMessage: public SelectionMessage
		{
			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new molecularSelectionMessage.
					@return      MolecularSelectionMessage new constructed molecularSelectionMessage
					@see         SelectionMessage
			*/
			MolecularSelectionMessage()
				throw();

			/** Copy constructor.
					Construct new molecularSelectionMessage by copying the molecularSelectionMessage
					{\em message}.
					The state of {\em *this} molecularSelectionMessage is set to the set of 
					{\em message} molecularSelectionMessage.
					@param       message the molecularSelectionMessage to be copied
					@return      MolecularSelectionMessage new constructed molecularSelectionMessage copied from {\em message}
					@see         SelectionMessage
			*/
			MolecularSelectionMessage(const SelectionMessage& message)
				throw();

			//@}

			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} molecularSelectionMessage.
			*/
			virtual ~MolecularSelectionMessage()
				throw();
			//@}
		
			private:
		};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/KERNEL/message.iC>
#		endif
  			
	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_KERNEL_MESSAGE_H
