// $Id: message.h,v 1.4 2001/05/13 13:47:47 hekl Exp $

#ifndef BALL_VIEW_KERNEL_MESSAGE_H
#define BALL_VIEW_KERNEL_MESSAGE_H

#ifndef BALL_COMMON_H
#include <BALL/common.h>
#endif

#ifndef BALL_CONCEPT_AUTODELETABLE_H
#	include <BALL/CONCEPT/autoDeletable.h>
#endif

#ifndef BALL_CONCEPT_COMPOSITE_H
#	include <BALL/CONCEPT/composite.h>
#endif

#ifndef BALL_DATATYPE_LIST_H
#	include <BALL/DATATYPE/list.h>
#endif

#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector3.h>
#endif


namespace BALL
{

	namespace VIEW
	{
		// class forward
		class ConnectionObject;

		/** General Message class.
				{\bf Framework:} BALL/VIEW/KERNEL\\
				{\bf Definition:} \URL{BALL/VIEW/KERNEL/message.h}\\
				{\bf Category:} \Ref{ConnectionObject} container\\ \\
				The class Message is the base class of all message classes. It provides all
				derived classes with the base interface each message class must have.
				Message classes will be sent through the \Ref{ConnectionObject} tree for
				the purpose of communication between several connectionObject classes.
				Each \Ref{ConnectionObject} handels in its {\em onNotify} method messages
				that are relevant for that class by means of \Ref{RTTI}. 
				This class can be given a sender object that is a pointer to the 
				\Ref{ConnectionObject} that has initially sent the message. This methods
				concerning the sender object are internally used and no user interaction
				is necessary. A message can be deletable or not. If a message is deletable
				it will be deleted after all \Ref{ConnectionObject} objects has been informed
				of this message. Therefore a deletable message must be created with the {\em new}
				command.
				All messages should be created with the {\em new} command and set to deletable
				for avoiding segmentation faults.
				See \Ref{ConnectionObject} for further information concerning message handling
				and message posting.
				@memo    Message class (BALL VIEW kernel framework)
				@author  $Author: hekl $
				@version $Revision: 1.4 $
				@date    $Date: 2001/05/13 13:47:47 $
		*/
		class Message
		{
			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new message.
					The state of {\em *this} message is:
					\begin{itemize}
					  \item  sender    - set to 0
						\item  deletable - set to {\tt false} 
					\end{itemize}
					@return      Message new constructed message
			*/
			Message()
				throw();

			/** Copy constructor.
					Construct new message by copying the message {\em message}.
					The state of {\em *this} message is set to the set of {\em message} message.
					@param       message the message to be copied
					@return      Message new constructed message copied from {\em message}
			*/
			Message(const Message& message)
				throw();

			//@}

			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} message.
			*/
			virtual ~Message()
				throw();
			//@}
		
			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{
			/** Change the sender.
					Change the sender of {\em *this} message to the value
					represented by the void parameter {\em object}. This parameter always represents
					a void pointer to a \Ref{ConnectionObject}. This method will be used internally
					by the \Ref{ConnectionObject} class it its {\em notify_} method.
					@param       object the new sender of {\em *this} message
					@see         getSender
					@see         ConnectionObject
					@see         ConnectionObject::notify_
			*/
			void setSender(const void* object)
				throw();

			/** Non-mutable inspection of the sender.
					Access the constant pointer of the sender of {\em *this} message.
					This return type always represents
					a void pointer to a \Ref{ConnectionObject}. This method will be used internally
					by the \Ref{ConnectionObject} class it its {\em onNotify} method.
					@return      const void* constant pointer to the sender of {\em *this} message
					@see         setSender
					@see         ConnectionObject
					@see         ConnectionObject::onNotify
			*/
			const void* getSender() const
				throw();

			/** Change the deletable flag.
					Change the deletable flag of {\em *this} message to the value
					represented by the parameter {\em flag}. \\
					If the parameter {\em flag} is set to {\tt true} the message is set to deletable
					and will be automatically deleted after all \Ref{ConnectionObject} objects
					are notified of {\em *this} message. It is important that {\em *this} message
					is created by the {\em new} command to avoid segmentation faults.\\
					If the parameter {\em flag} is set to {\tt false} the message will not be 
					automatically deleted.\\
					Avoid creating non-deletable messages because it can result in
					segmentation faults.
					@param       flag the new deletable state of {\em *this} message
					@see         isDeletable
			*/
			void setDeletable(bool flag = true)
				throw();

			/** Test if {\em *this} message is deletable.
					@return  bool -	{\tt true} if {\em *this} message is deletable, {\tt false} otherwise
					@see     setDeletable
			*/
		  bool isDeletable() const
				throw();
			//@}

			private:

			void *connection_object_;

			bool deletable_;
		};


		
		/** CompositeMessage class.
				{\bf Framework:} BALL/VIEW/KERNEL\\
				{\bf Definition:} \URL{BALL/VIEW/KERNEL/message.h}\\
				{\bf Category:} \Ref{Composite} container\\	\\
				The class CompositeMessage is the base class of all composite message classes.
				It provides all	derived classes with the base interface each composite message
				class must have.
				The CompositeMessage class is a base class for messages that are relevant for
				\Ref{ConnectionObject} objects that must react to \Ref{Composite} changes.
				@memo    CompositeMessage class (BALL VIEW kernel framework)
				@author  $Author: hekl $
				@version $Revision: 1.4 $
				@date    $Date: 2001/05/13 13:47:47 $
		*/
		class CompositeMessage: public Message
		{
			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new compositeMessage.
					The state of {\em *this} compositeMessage is:
					\begin{itemize}
					  \item  composite       - set to 0
						\item  composite name  - set to "" 
					\end{itemize}
					@return      CompositeMessage new constructed compositeMessage
					@see         Message
			*/
			CompositeMessage()
				throw();

			/** Copy constructor.
					Construct new compositeMessage by copying the compositeMessage {\em message}.
					The state of {\em *this} compositeMessage is set to the set of {\em message} 
					compositeMessage.
					@param       message the compositeMessage to be copied
					@return      CompositeMessage new constructed compositeMessage copied from {\em message}
					@see         CompositeMessage
			*/
			CompositeMessage(const CompositeMessage& message)
				throw();

			//@}

			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} compositeMessage.
			*/
			virtual ~CompositeMessage()
				throw();
			//@}
		
			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{
			/** Change the composite.
					Change the composite of {\em *this} compositeMessage to the value
					represented by the parameter {\em composite}.
					@param       composite a reference to the new composite of {\em *this} compositeMessage
					@see         getComposite
			*/
			void setComposite(const Composite& composite)
				throw();

			/** Change the composite.
					Change the composite of {\em *this} compositeMessage to the value
					represented by the parameter {\em composite}.
					@param       composite a pointer to the new composite of {\em *this} compositeMessage
					@see         getComposite
			*/
			void setComposite(const Composite* composite)
				throw();

			/** Inspection of the composite.
					Access the pointer of the composite of {\em *this} compositeMessage.
					@return      Composite* pointer to the composite of {\em *this} compositeMessage
					@see         setComposite
			*/
			Composite* getComposite() const
				throw();

			/** Change the name of the composite.
					Change the name of the composite of {\em *this} compositeMessage to the value
					represented by the parameter {\em name}.
					@param       name the new name of the composite of {\em *this} compositeMessage
					@see         getCompositeName
			*/
			void setCompositeName(string name)
				throw();

			/** Inspection of the name of the composite.
					Access the name of the composite of {\em *this} compositeMessage.
					@return      string the name of the composite of {\em *this} compositeMessage
					@see         setCompositeName
			*/
			string getCompositeName() const
				throw();
			//@}

			private:

			Composite *composite_;

			string composite_name_;
		};


		/** NewCompositeMessage class.
				{\bf Framework:} BALL/VIEW/KERNEL\\
				{\bf Definition:} \URL{BALL/VIEW/KERNEL/message.h}\\ \\
				The class NewCompositeMessage is the message class that is responsible for
				making known a new \Ref{Composite} object inside the \Ref{ConnectionObject} tree.
				It will be sent by existing \Ref{ConnectionObject} objects that create new
				\Ref{Composites} objects. One can use the \Ref{RTTI} mechanism to identify
				such messages in the {\em onNotify} method of user constructed \Ref{ConnectionObject}
				objects.
				@memo    NewCompositeMessage class (BALL VIEW kernel framework)
				@author  $Author: hekl $
				@version $Revision: 1.4 $
				@date    $Date: 2001/05/13 13:47:47 $
		*/
		class NewCompositeMessage: public CompositeMessage
		{
			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new newCompositeMessage.
					@return      NewCompositeMessage new constructed newCompositeMessage
					@see         CompositeMessage
			*/
			NewCompositeMessage()
				throw();

			/** Copy constructor.
					Construct new newCompositeMessage by copying the newCompositeMessage {\em message}.
					The state of {\em *this} newCompositeMessage is set to the set of {\em message} 
					newCompositeMessage.
					@param       message the newCompositeMessage to be copied
					@return      NewCompositeMessage new constructed newCompositeMessage copied from {\em message}
					@see         NewCompositeMessage
			*/
			NewCompositeMessage(const CompositeMessage& message)
				throw();

			//@}

			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} newCompositeMessage.
			*/
			virtual ~NewCompositeMessage()
				throw();
			//@}
		
			private:
		};


		/** RemovedCompositeMessage class.
				{\bf Framework:} BALL/VIEW/KERNEL\\
				{\bf Definition:} \URL{BALL/VIEW/KERNEL/message.h}\\ \\
				The class RemovedCompositeMessage is the message class that is responsible for
				marking a \Ref{Composite} object as removed inside the \Ref{ConnectionObject} tree.
				It will be sent by existing \Ref{ConnectionObject} objects that remove (or would like to
				remove) \Ref{Composites} objects. One can use the \Ref{RTTI} mechanism to identify
				such messages in the {\em onNotify} method of user constructed \Ref{ConnectionObject}
				objects.
				@memo    RemovedCompositeMessage class (BALL VIEW kernel framework)
				@author  $Author: hekl $
				@version $Revision: 1.4 $
				@date    $Date: 2001/05/13 13:47:47 $
		*/
		class RemovedCompositeMessage: public CompositeMessage
		{
			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new removedCompositeMessage.
					@return      RemovedCompositeMessage new constructed removedCompositeMessage
					@see         CompositeMessage
			*/
			RemovedCompositeMessage()
				throw();

			/** Copy constructor.
					Construct new removedCompositeMessage by copying the removedCompositeMessage {\em message}.
					The state of {\em *this} removedCompositeMessage is set to the set of {\em message} 
					removedCompositeMessage.
					@param       message the removedCompositeMessage to be copied
					@return      RemovedCompositeMessage new constructed removedCompositeMessage copied from {\em message}
					@see         RemovedCompositeMessage
			*/
			RemovedCompositeMessage(const CompositeMessage& message)
				throw();

			//@}

			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} removedCompositeMessage.
			*/
			virtual ~RemovedCompositeMessage()
				throw();
			//@}
		
			private:
		};


		/** ChangedCompositeMessage class.
				{\bf Framework:} BALL/VIEW/KERNEL\\
				{\bf Definition:} \URL{BALL/VIEW/KERNEL/message.h}\\ \\
				The class ChangedCompositeMessage is the message class that is responsible for
				marking a \Ref{Composite} object as changed inside the \Ref{ConnectionObject} tree.
				It will be sent by existing \Ref{ConnectionObject} objects that change (or would like to
				change) \Ref{Composites} objects. One can use the \Ref{RTTI} mechanism to identify
				such messages in the {\em onNotify} method of user constructed \Ref{ConnectionObject}
				objects.
				@memo    ChangedCompositeMessage class (BALL VIEW kernel framework)
				@author  $Author: hekl $
				@version $Revision: 1.4 $
				@date    $Date: 2001/05/13 13:47:47 $
		*/
		class ChangedCompositeMessage: public CompositeMessage
		{
			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new changedCompositeMessage.
					@return      RemovedCompositeMessage new constructed changedCompositeMessage
					@see         CompositeMessage
			*/
			ChangedCompositeMessage()
				throw();

			/** Copy constructor.
					Construct new changedCompositeMessage by copying the changedCompositeMessage {\em message}.
					The state of {\em *this} changedCompositeMessage is set to the set of {\em message} 
					changedCompositeMessage.
					@param       message the changedCompositeMessage to be copied
					@return      ChangedCompositeMessage new constructed changedCompositeMessage copied from {\em message}
					@see         ChangedCompositeMessage
			*/
			ChangedCompositeMessage(const CompositeMessage& message)
				throw();

			//@}

			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} changedCompositeMessage.
			*/
			virtual ~ChangedCompositeMessage()
				throw();
			//@}
		
			private:
		};


		/*
		class DisplayMessage: public CompositeMessage
		{
			public:

			//@{
			DisplayMessage()
				throw();

			DisplayMessage(const DisplayMessage& message)
				throw();

			virtual ~DisplayMessage()
				throw();
			//@}
 			private:
		};
		*/

		/** SceneMessage class.
				
				{\bf Framework:} BALL/VIEW/KERNEL\\
				{\bf Definition:} \URL{BALL/VIEW/KERNEL/message.h}\\ \\
				The class SceneMessage is the message class that is responsible for
				changing the visual properties of the \Ref{Scene}.
				\Ref{ConnectionObject} objects that would like to have the \Ref{Scene} focus on another
				\Ref{Composite} or would like to update the contents of the \Ref{Scene}
				(because of some changes in the composite structure) will sent this message.
				This message will only be catched by the {\em onNotify} method of the \Ref{Scene}.
				There are methods available that will tell the \Ref{Scene} to update its contents
				or change the camera positions.
				@memo    SceneMessage class (BALL VIEW kernel framework)
				@author  $Author: hekl $
				@version $Revision: 1.4 $
				@date    $Date: 2001/05/13 13:47:47 $
		*/
		class SceneMessage: public Message
		{
			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new sceneMessage.
					The state of {\em *this} sceneMessage is set to:
					\begin{itemize}
					  \item  update flag     - set to {\tt false}
						\item  camera look at  - set to {\tt (0,0,0)} 
						\item  camera position - set to {\tt (0,0,0)} 
					\end{itemize}
					@return      SceneMessage new constructed sceneMessage
					@see         Message
			*/
			SceneMessage()
				throw();

			/** Copy constructor.
					Construct new sceneMessage by copying the sceneMessage {\em message}.
					The state of {\em *this} sceneMessage is set to the set of {\em message} 
					sceneMessage.
					@param       message the sceneMessage to be copied
					@return      SceneMessage new constructed sceneMessage copied from {\em message}
					@see         SceneMessage
			*/
			SceneMessage(const SceneMessage& message)
				throw();

			//@}

			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} sceneMessage.
			*/
			virtual ~SceneMessage()
				throw();
			//@}
		
			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{
			/** Change the update flag of {\em *this} sceneMessage.
					Change the update flag of {\em *this} sceneMessage to the value
					represented by the parameter {\em value}. \\
					If the parameter {\em value} is set to {\tt true} the sceneMessage will evoke an update
					of the \Ref{Scene} contents (the visualization will be drawn anew).
					Changes of the camera position will be ignored.\\
					If the parameter {\em value} is set to {\tt false} the sceneMessage will not evoke an update
					of the \Ref{Scene} contents. Changes of the camera position will be used.
					@param       value the new update state of {\em *this} sceneMessage
					@see         isUpdateOnly
			*/
			void updateOnly(bool value = true)
				throw();

			/** Test if {\em *this} sceneMessage evokes only a \Ref{Scene} update.
					@return  bool -	{\tt true} if {\em *this} sceneMessage evokes only a \Ref{Scene} update, {\tt false} otherwise (camera position changeable)
					@see     updateOnly
			*/
			bool isUpdateOnly()
				throw();

			/** Change the {\em look at} component of the camera position.
					Change the {\em look at} component of the camera position of {\em *this} sceneMessage
					to the value represented by the parameter {\em vector3}.
					@param       vector3 the new {\em look at} vector of the camera position of {\em *this} sceneMessage
					@see         getCameraLookAt
					@see         Vector3
			*/
			void setCameraLookAt(Vector3 vector3)
				throw();

			/** Mutable inspection of the {\em look at} component of the camera position.
					Access the mutual reference of the {\em look at} component of the
					camera position of {\em *this} sceneMessage.
					@return      Vector3& mutable reference to the {\em look at} component of the camera position of {\em *this} sceneMessage
					@see         setCameraLookAt
					@see         Vector3
			*/
			Vector3& getCameraLookAt()
				throw();

			/** Non-mutable inspection of the {\em look at} component of the camera position.
					Access the constant reference of the {\em look at} component of the camera position
					of {\em *this} sceneMessage.
					@return      const Vector3& constant reference to the {\em look at} component of the camera position of {\em *this} sceneMessage
					@see         setCameraLookAt
					@see         Vector3
			*/
			const Vector3& getCameraLookAt() const
				throw();

			/** Change the {\em view point} component of the camera position.
					Change the {\em view point} component of the camera position of {\em *this} sceneMessage
					to the value represented by the parameter {\em vector3}.
					@param       vector3 the new {\em view point} vector of the camera position of {\em *this} sceneMessage
					@see         getCameraViewPoint
					@see         Vector3
			*/
			void setCameraViewPoint(Vector3 vector3)
				throw();

			/** Mutable inspection of the {\em view point} component of the camera position.
					Access the mutual reference of the {\em view point} component of the
					camera position of {\em *this} sceneMessage.
					@return      Vector3& mutable reference to the {\em view point} component of the camera position of {\em *this} sceneMessage
					@see         setCameraViewPoint
					@see         Vector3
			*/
			Vector3& getCameraViewPoint()
				throw();

			/** Non-mutable inspection of the {\em view point} component of the camera position.
					Access the constant reference of the {\em view point} component of the camera position
					of {\em *this} sceneMessage.
					@return      const Vector3& constant reference to the {\em view point} component of the camera position of {\em *this} sceneMessage
					@see         setCameraViewPoint
					@see         Vector3
			*/
			const Vector3& getCameraViewPoint() const
				throw();
			//@}

			private:

			bool update_only_;

			Vector3 camera_look_at_;
			Vector3 camera_view_point_;
		};


		/** WindowMessage class.
				{\bf Framework:} BALL/VIEW/KERNEL\\
				{\bf Definition:} \URL{BALL/VIEW/KERNEL/message.h}\\ \\
				The class WindowMessage is the message class that is responsible for
				changing the status bar of the main Application. The main Application
				can in its {\em onNotify} method, by means of the \Ref{RTTI} mechanism, filter
				this message and extract the new contents of status bar that any other
				\Ref{ConnectionObject} object has sent through the \Ref{ConnectionObject} tree.
				@memo    WindowMessage class (BALL VIEW kernel framework)
				@author  $Author: hekl $
				@version $Revision: 1.4 $
				@date    $Date: 2001/05/13 13:47:47 $
		*/
		class WindowMessage: public Message
		{
			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new windowMessage.
					The state of {\em *this} windowMessage is set to:
					\begin{itemize}
					  \item  status bar - set to ""
					\end{itemize}
					@return      WindowMessage new constructed windowMessage
					@see         Message
			*/
			WindowMessage()
				throw();

			/** Copy constructor.
					Construct new windowMessage by copying the windowMessage {\em message}.
					The state of {\em *this} windowMessage is set to the set of {\em message} 
					windowMessage.
					@param       message the windowMessage to be copied
					@return      WindowMessage new constructed windowMessage copied from {\em message}
					@see         WindowMessage
			*/
			WindowMessage(const WindowMessage& message)
				throw();

			//@}

			/** @name Destructors 
      */
			//@{

			/** Destructor.
					Default destruction of {\em *this} windowMessage.
			*/
			virtual ~WindowMessage()
				throw();
			//@}
		
			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{
			/** Change the contents of the status bar string.
					Change the contents of the status bar string of {\em *this} windowMessage to the value
					represented by the parameter {\em message}.
					@param       message the new status bar message of {\em *this} windowMessage
					@see         getStatusBar
			*/
			void setStatusBar(const string &message)
				throw();
			
			/** Inspection of the contents of the status bar string.
					Access the the contents of the status bar string of {\em *this} windowMessage.
					@return      string the contents of the status bar string of {\em *this} windowMessage
					@see         setStatusBar
			*/
			string getStatusBar() const
				throw();
			//@}

			private:

			string status_bar_;
		};
		

		/** SelectionMessage class.
				{\bf Framework:} BALL/VIEW/KERNEL\\
				{\bf Definition:} \URL{BALL/VIEW/KERNEL/message.h}\\
				{\bf Category:} \Ref{Composite} container\\	\\
				The class SelectionMessage is the message class that is a container for
				\Ref{Composite} objects that are somehow selected.
				It will be sent by existing \Ref{ConnectionObject} objects that collect
				\Ref{Composites} objects for a certain purpose.
				One can use the \Ref{RTTI} mechanism to identify
				such messages in the {\em onNotify} method of user constructed \Ref{ConnectionObject}
				objects.
				@memo    SelectionMessage class (BALL VIEW kernel framework)
				@author  $Author: hekl $
				@version $Revision: 1.4 $
				@date    $Date: 2001/05/13 13:47:47 $
		*/
		class SelectionMessage: public Message
		{
			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new selectionMessage.
					The state of {\em *this} selectionMessage is set to:
					\begin{itemize}
					  \item  composite list - empty
					\end{itemize}
					@return      SelectionMessage new constructed selectionMessage
					@see         Message
			*/
			SelectionMessage()
				throw();

			/** Copy constructor.
					Construct new selectionMessage by copying the selectionMessage {\em message}.
					The state of {\em *this} selectionMessage is set to the set of {\em message} 
					selectionMessage.
					@param       message the selectionMessage to be copied
					@return      SelectionMessage new constructed selectionMessage copied from {\em message}
					@see         SelectionMessage
			*/
			SelectionMessage(const SelectionMessage& message)
				throw();

			//@}

			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} selectionMessage.
			*/
			virtual ~SelectionMessage()
				throw();
			//@}
		
			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{
			/** Change the selection of \Ref{Composite} objects.
					Change the selection of \Ref{Composite} objects of {\em *this} selectionMessage
					to the value represented by the parameter {\em selection}. The selection list will
					be copied from the given list.
					@param       selection the new selection of \Ref{Composite} objects of {\em *this} selectionMessage
					@see         getSelection
			*/
			void setSelection(const List<Composite*> &selection)
				throw();

			/** Non-mutable inspection of the selection of \Ref{Composite} objects.
					Access the constant reference of the selection of \Ref{Composite} objects
					of {\em *this} selectionMessage.
					@return      const List<Composite*>& constant reference to the selection of \Ref{Composite} objects	of {\em *this} selectionMessage
					@see         setSelection
					@see         List
					@see         Composite
			*/
			const List<Composite*>& getSelection() const
				throw();
			//@}

			private:

			List<Composite*> selection_;
		};



		/** GeometricObjectSelectionMessage class.
				{\bf Framework:} BALL/VIEW/KERNEL\\
				{\bf Definition:} \URL{BALL/VIEW/KERNEL/message.h}\\ \\
				The class GeometricObjectSelectionMessage is the message class that is a container for
				\Ref{GeometricObject} objects that are somehow selected.
				It will be sent by existing \Ref{ConnectionObject} objects that collect
				\Ref{GeometricObject} objects for a certain purpose.
				One can use the \Ref{RTTI} mechanism to identify
				such messages in the {\em onNotify} method of user constructed \Ref{ConnectionObject}
				objects.
				@memo    GeometricObjectSelectionMessage class (BALL VIEW kernel framework)
				@author  $Author: hekl $
				@version $Revision: 1.4 $
				@date    $Date: 2001/05/13 13:47:47 $
		*/
		class GeometricObjectSelectionMessage: public SelectionMessage
		{
			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new geometricObjectSelectionMessage.
					@return      GeometricObjectSelectionMessage new constructed geometricObjectSelectionMessage
					@see         SelectionMessage
			*/
			GeometricObjectSelectionMessage()
				throw();

			/** Copy constructor.
					Construct new geometricObjectSelectionMessage by copying the
					geometricObjectSelectionMessage {\em message}.
					The state of {\em *this} geometricObjectSelectionMessage is set to the set of {\em message} 
					geometricObjectSelectionMessage.
					@param       message the geometricObjectSelectionMessage to be copied
					@return      GeometricObjectSelectionMessage new constructed geometricObjectSelectionMessage copied from {\em message}
					@see         GeometricObjectSelectionMessage
			*/
			GeometricObjectSelectionMessage(const SelectionMessage& message)
				throw();

			//@}

			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} geometricObjectSelectionMessage.
			*/
			virtual ~GeometricObjectSelectionMessage()
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
