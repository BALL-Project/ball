// $Id: connectionObject.h,v 1.6 2001/02/11 13:04:40 hekl Exp $

#ifndef BALL_VIEW_KERNEL_CONNECTIONOBJECT_H
#define BALL_VIEW_KERNEL_CONNECTIONOBJECT_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_CONCEPT_COMPOSITE_H
#	include <BALL/CONCEPT/composite.h>
#endif

#ifndef BALL_DATATYPE_STRING_H
#	include <BALL/DATATYPE/string.h>
#endif

#ifndef BALL_DATATYPE_LIST_H
# include <BALL/DATATYPE/list.h>
#endif

#ifndef BALL_DATATYPE_OPTIONS_H
# include <BALL/DATATYPE/options.h>
#endif

#ifndef BALL_VIEW_KERNEL_MESSAGE_H
# include <BALL/VIEW/KERNEL/message.h>
#endif

#include <qobject.h>
#include <string>
#include <typeinfo>

using std::string;
using namespace BALL;

namespace BALL
{

	namespace VIEW
	{
		/** ConnectionObject class.		
				{\bf Framework:} BALL/VIEW/KERNEL\\
				{\bf Definition:} \URL{BALL/VIEW/KERNEL/connectionObject.h}\\
				{\bf Category:} \Ref{ConnectionObject} container\\
				{\bf Category:} \Ref{Message} container\\ 	\\
				The class ConnectionObject is a base class for all widgets and dialogs that
				handles communication between these classes through message posting.
				There are methods that handle the registering process between the classes that
				should be connected as well as notifying methods that are responsible for posting
				messages. The registering process will be handled by the interface of every
				dialog and widget. In this process a tree will be created that connects all 
				connectionObject objects.	See \Ref{ModularWidget} for further information concerning
				widget and dialog interface definition.
				The widgets communicate with the help of the method notify\_. With this method
				a message can be sent through the connectionObject tree. One connectionObject
				will be the root of this tree and therefore responsible for handling and sending
				all	incoming messages. A Message will first be sent to the root of the connectionObject
				tree and then will travers its way down to every leaf excluding the one that
				had sent the original message. If an incoming message triggers a reply message (in the
				onNotify method) that new message will be stored in a message queue in the root 
				of the tree and executed
				if the first message has traversed all leafs of the tree. Therefore a message should
				always be created with the new command and the flag deletable must be set to {\em true}.
				If a message if statically created in the {\em onNotify} method there is the possibility
				that the message is already destroyed when it should be processed.
				@memo    ConnectionObject class (BALL VIEW kernel framework)
				@author  $Author: hekl $
				@version $Revision: 1.6 $
				@date    $Date: 2001/02/11 13:04:40 $
		*/
		class ConnectionObject
		{
			public:
			
			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new connectionObject.
					Initialize the message queue and resets all connection to zero.
					@return  ConnectionObject - new constructed connectionObject
			*/
			ConnectionObject();
				
			//@}
			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} connectionObject.
					Calls \Ref{ConnectionObject::destroy}.
					@see   ConnectionObject::destroy
			*/
			virtual ~ConnectionObject();

			/** Explicit default initialization.
					Clears the message queue and deletes all deletable messages in it.
					Further all connections to the children and the parent will be cut.
			*/
			virtual void clear();

			/** Explicit destructor.
					Calls \Ref{ConnectionObject::clear}
					@see   ConnectionObject::clear
			*/
			virtual void destroy();

			//@}
			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{
			
			/** Register connectionObject.
					Register connectionObject {\em object}. The connectionObject {\em *this} will
					be the new parent of {\em object} connectionObject. Each connectionObject can only
					be inserted once to a parent.
					@param    object the connectionObject that is the new child of {\em *this} connectionObject
					@see      ConnectionObject::unregisterConnectionObject
					@see      ConnectionObject::isConnectionObjectRegistered
			*/
			void registerConnectionObject(ConnectionObject &object);

			/** Unregister connectionObject.
					Unregister connectionObject {\em object}. The connectionObject {\em *this} deletes
					the {\em object} connectionObject from its children list and the parent of
					{\em object} connectionObject will be set to 0. 
					@param    object the connectionObject that will be removed from {\em *this} connectionObject
					@see      ConnectionObject::registerConnectionObject
					@see      ConnectionObject::isConnectionObjectRegistered
			*/
			void unregisterConnectionObject(ConnectionObject &object);

			/** Test if connectionObject is registered.
					Test if {\em object} connectionObject is a child of {\em *this} connectionObject.

					@param   object the connectionObject to be tested
					@return  bool - {\tt true} if {\em object} is a child of {\em *this} connectionObject, {\tt false} otherwise
					@see     ConnectionObject::registerConnectionObject
					@see     ConnectionObject::unregisterConnectionObject
			*/
			bool isConnectionObjectRegistered(const ConnectionObject &object);

			/** Inspection of parent connectionObject.
					Access the parent connectionObject of {\em *this} connectionObject.
					If {\em *this} connectionObject is root {\tt 0} will be returned.
					@return  ConnectionObject* - the pointer to the parent connectionObject, {\tt 0} if {\em *this} is root.
			*/
			ConnectionObject *getParent() const;

			/** Inspection of the root connectionObject.
					Access the root of {\em *this} connectionObject. Return the connectionObject that
					parent is {\tt 0}.
					@return   ConnectionObject* - the connectionObject that is the root
			*/
			ConnectionObject *getRoot();

			/** Message handling method.
					This method handles messages sent to {\em *this} connectionObject.
					If a certain message should be processed for {\em *this} connectionObject
					this method must be overridden. With the help of the RTTI methods one can easily
					determine if an incoming message is relevant for {\em *this} connectionObject.
					See \Ref{Message} for further information concerning messages.
					@param   message a pointer to a message. Do not delete the message in this method.
			*/
			virtual void onNotify(Message *message);

			//@}
			/**	@name	debuggers and diagnostics
			*/
			//@{

			/** Internal state and consistency self-validation.
					Initiate self-validation of the data structure consistencies
					of {\em *this} connectionObject.
					If the internal data structure of {\em *this} connectionObject is correct
					(self-validated) and consistent {\tt true} is returned, {\tt false} otherwise. 
					Checks the tree structure if all connections (in both ways: up and down) are 
					without error.

					@return			bool - {\tt true} if the internal data structure of {\em *this}
                      connectionObject is correct (self-validated) and consistent,
					 						{\tt false} otherwise
			*/
			virtual bool isValid() const;

			/** Internal value dump.
					Dump the current state of {\em *this} connectionObject to 
					the output ostream {\em s} with dumping depth {\em depth}.
					@param   s output stream where to output the state of {\em *this} connectionObject
					@param   depth the dumping depth
			*/
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const;

			//@}
			/**	@name	Storers
			*/
			//@{

			/** Persistent stream output and state restorage.
					Read persistent connectionObject data from the input stream {\em s} and 
					restore the state of {\em *this}.	\\
					{\bf Note:} Not yet implemented.
			  	@param       s input stream from where to restore the internal state of {\em *this} connectionObject
					@exception   NotImplemented - always
			*/
			virtual void read(std::istream& s);

			/** Persistent stream output and state storage.
  				Write persistent connectionObject data to the output stream {\em s} and 
					store the state of {\em *this}. \\
					{\bf Note:} Not yet implemented.
		 			@param       s output stream to where to store the internal state of {\em *this} connectionObject
					@exception   NotImplemented - always
			*/
			virtual void write(std::ostream& s) const;
	
			//@}
			/** @name Protected notification methods
			*/
			//@{
	
			protected:
			
			/** Notification method with pointer.
					Call this method if a message should be sent through the connectionObject tree.
					If this method is not called inside the {\em onNotify} method the message will
					immediately begin traversing the tree and trigger reactions in the other
					connectionObjects. After all connectionObjects have reacted to this message the
					method will return. If this method is called inside the {\em onNotify} method
					the message will be stored in the message queue of the root connectionObject and
					the method will immediately return.
					This new message will be executed after the previously message is completely executed.
					An often made error is to statically create a message in an {\em onNotify} method.
					In that case the message queue of the root connectionObject stores a pointer to
					the statically created message for later processing but if the message is processed
					later the {\em onNotify} method that has created that message is already finished.
					Therefore the message is no longer available what leads to a segmentation fault.
					To prevent that error always create messages with the {\tt new} command and uses
					this method for the notification.
					See \Ref{Message} For further information concerning messages.
					@param   message a pointer to a message that will be sent through the connectionObject tree.
					@see     Message::Message
			*/
			void notify_(Message *message);

			/** Notification method with reference.
					See {\em notify_} for detailed information on the notification process.
					Only use this method if you are absolutely sure what you are doing!
					@param    message a reference to a message that will be sent through the connectionObject tree.
					@see      Message::Message
			*/
			void notify_(Message &message);

			//@}

			private:

			void setParent_(ConnectionObject &object);
			void clearParent_();

			void onNotify_(Message* message);


			bool processing_message_queue_;
			List<Message *> message_queue_;

			ConnectionObject *parent_;
			List<ConnectionObject *> children_connection_objects_;
		};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/KERNEL/connectionObject.iC>
#		endif
		
	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_KERNEL_CONNECTIONOBJECT_H
