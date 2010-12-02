// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_KERNEL_CONNECTIONOBJECT_H
#define BALL_VIEW_KERNEL_CONNECTIONOBJECT_H

#ifndef BALL_COMMON_MACROS_H
# include <BALL/COMMON/macros.h>
#endif

namespace BALL
{
	namespace VIEW
	{
		class Message;
	
		/** Base class for all widgets and dialogs that handles communication through message posting.
				There are methods that handle the registering process between the classes that
				should be connected as well as notifying methods that are responsible for posting
				messages. The registering process will be handled by the interface of every
				dialog and widget. In this process a tree will be created that connects all 
				connectionObject objects.	See ModularWidget for further information concerning
				widget and dialog interface definition.
				The widgets communicate with the help of the method notify_. With this method
				a message can be sent through the connectionObject tree. One connectionObject
				will be the root of this tree and therefore responsible for handling and sending
				all	incoming messages. A Message will first be sent to the root of the connectionObject
				tree and then will travers its way down to every leaf excluding the one that
				had sent the original message. If an incoming message triggers a reply message (in the
				onNotify method) that new message will be stored in a message queue in the root 
				of the tree and executed
				if the first message has traversed all leafs of the tree. Therefore a message should
				always be created with the new command and the flag deletable must be set to <b> true</b>.
				If a message if statically created in the <b> onNotify</b> method there is the possibility
				that the message is already destroyed when it should be processed.
				\ingroup ViewKernelConnectivity
		*/
		class BALL_VIEW_EXPORT ConnectionObject
		{
			public:
			
			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Initialize the message queue and resets all connection to zero.
			*/
			ConnectionObject();
				
			//@}
			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Calls destroy.
					\see   destroy
			*/
			virtual ~ConnectionObject();

			/** Explicit default initialization.
					Clears the message queue and deletes all deletable messages in it.
					Further all connections to the children and the parent will be cut.
			*/
			virtual void clear();

			/** Explicit destructor.
					Calls clear
					\see   clear
			*/
			virtual void destroy();

			//@}
			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{
			
			/** Register connectionObject.
					The connectionObject this will be the new parent of <b> object</b> connectionObject. 
					Each connectionObject can only be inserted once to a parent.
					\param    object the connectionObject that is the new child of this connectionObject
					\see      unregisterConnectionObject
					\see      isConnectionObjectRegistered
			*/
			void registerConnectionObject(ConnectionObject &object);

			/** Unregister connectionObject.
					The connectionObject this deletes
					the <b> object</b> connectionObject from its children list and the parent of
					<b> object</b> connectionObject will be set to 0. 
					\param    object the connectionObject that will be removed from this connectionObject
					\see      registerConnectionObject
					\see      isConnectionObjectRegistered
			*/
			void unregisterConnectionObject(ConnectionObject &object);

			/** Test if connectionObject is registered.
					\param   object the connectionObject to be tested
					\return  bool <tt> true</tt> if <b> object</b> is a child of this connectionObject, <tt> false</tt> otherwise
					\see     registerConnectionObject
					\see     unregisterConnectionObject
			*/
			bool isConnectionObjectRegistered(const ConnectionObject &object);

			/** Access the parent connectionObject of this connectionObject.
					If this connectionObject is root <tt> 0</tt> will be returned.
					\return  ConnectionObject* the pointer to the parent connectionObject, <tt> 0</tt> if this is root.
			*/
			ConnectionObject* getParent() const;

			/** Access the root of this ConnectionObject. 
			 		Returns the ConnectionObject that parent is <tt> 0</tt>.
			*/
			ConnectionObject* getRoot();

			/** Message handling method.
					This method handles messages sent to this connectionObject.
					If a certain message should be processed for this connectionObject
					this method must be overridden. With the help of the RTTI methods one can easily
					determine if an incoming message is relevant for this connectionObject.
					See Message for further information concerning messages.
					\param   message a pointer to a message. Do not delete the message in this method.
			*/
			virtual void onNotify(Message *message);

			//@}
			/**	@name	debuggers and diagnostics
			*/
			//@{

			/** Internal state and consistency self-validation.
					Initiate self-validation of the data structure consistencies
					of this connectionObject.
					If the internal data structure of this connectionObject is correct
					(self-validated) and consistent <tt> true</tt> is returned, <tt> false</tt> otherwise. 
					Checks the tree structure if all connections (in both ways: up and down) are 
					without error.

					\return			bool <tt> true</tt> if the internal data structure of this
                      connectionObject is correct (self-validated) and consistent,
					 						<tt> false</tt> otherwise
			*/
			virtual bool isValid() const;

			/** Internal value dump.
					Dump the current state to 
					the output ostream <b> s</b> with dumping depth <b> depth</b>.
					\param   s output stream where to output the state 
					\param   depth the dumping depth
			*/
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const;

			//@}
			
			/** @name Protected notification methods
			*/
			//@{
	
			protected:
			
			/** Notification method with pointer.
					Call this method if a message should be sent through the connectionObject tree.
					If this method is not called inside the <b> onNotify</b> method the message will
					immediately begin traversing the tree and trigger reactions in the other
					connectionObjects. After all connectionObjects have reacted to this message the
					method will return. If this method is called inside the <b> onNotify</b> method
					the message will be stored in the message queue of the root connectionObject and
					the method will immediately return.
					This new message will be executed after the previously message is completely executed.
					An often made error is to statically create a message in an <b> onNotify</b> method.
					In that case the message queue of the root connectionObject stores a pointer to
					the statically created message for later processing but if the message is processed
					later the <b> onNotify</b> method that has created that message is already finished.
					Therefore the message is no longer available what leads to a segmentation fault.
					To prevent that error always create messages with the <tt> new</tt> command and uses
					this method for the notification.
					See Message For further information concerning messages.
					\param   message a pointer to a message that will be sent through the connectionObject tree.
					\see     Message
			*/
			void notify_(Message *message);

			/** Notification method with reference.
					See <b> notify_</b> for detailed information on the notification process.
					Only use this method if you are absolutely sure what you are doing!
					\param    message a reference to a message that will be sent through the connectionObject tree.
					\see      Message
			*/
			void notify_(Message &message);

			//@}

			protected:

			void onNotify_(Message* message);

			private:

			void setParent_(ConnectionObject &object);

			void clearParent_();

	
			bool processing_message_queue_;
			std::list<Message *> message_queue_;

			ConnectionObject *parent_;
			std::list<ConnectionObject *> children_connection_objects_;
		};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/KERNEL/connectionObject.iC>
#		endif
  		
} } // namespace
#endif // BALL_VIEW_KERNEL_CONNECTIONOBJECT_H
