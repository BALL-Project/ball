// $Id: objectCreator.h,v 1.5 2001/07/16 14:49:30 amoll Exp $

#ifndef BALL_VIEW_GUI_KERNEL_OBJECTCREATOR_H
#define BALL_VIEW_GUI_KERNEL_OBJECTCREATOR_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_CONCEPT_COMPOSITE_H
#	include <BALL/CONCEPT/composite.h>
#endif

#ifndef BALL_CONCEPT_PERSISTENTOBJECT_H       
# include <BALL/CONCEPT/persistentObject.h>
#endif

#ifndef BALL_CONCET_TEXTPERSISTENCEMANAGER_H
# include <BALL/CONCEPT/textPersistenceManager.h>
#endif

#ifndef BALL_SYSTEM_SOCKET_H
#	include <BALL/SYSTEM/socket.h>
#endif

#ifndef BALL_VIEW_COMMON_GLOBAL_H
#	include <BALL/VIEW/COMMON/global.h>
#endif

#include <vector>


namespace BALL
{
	
	namespace VIEW
	{

		/** ObjectCreator class.
				The class ObjectCreator is a base class for converting \Ref{PersistentObject}
				objects into \Ref{Composite} objects. It defines methods for initialization
				and conversion that must be overriden by the derived class. 
				This class is used by the class \Ref{Server} to convert incoming
				\Ref{PersistentObject} objects to \Ref{Composite} objects.
				{\bf Definition:} \URL{BALL/VIEW/GUI/KERNEL/objectCreator.h}
		*/
		class ObjectCreator
		{
			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new objectCreator.
					The state of {\em *this} server is: \Ref{TextPersistenceManager}
					not initialized.
					@return      ObjectCreator new constructed objectCreator
					@see         TextPersistenceManager
			*/
			ObjectCreator()
				throw();

			//@}

			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} objectCreator.
					Calls \Ref{destroy}.
					@see         destroy
			*/
			virtual ~ObjectCreator()
				throw();

			/** Explicit default initialization.
					Empty for further purpose.
			*/
			virtual void clear()
				throw();

			/** Explicit destructor.
					Empty for further purpose.
			*/
			virtual void destroy()
				throw();
			//@}

			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{		
			/** Initialize the persistence manager.
					Initialize the \Ref{TextPersistenceManager} of {\em *this} objectCreator.
					Override this method to initialize the persistence manager for specified
					objects. See \Ref{TextPersistenceManager} for information on how and why
					to register \Ref{PersistentObject} classes.
					@param  pm the \Ref{TextPersistenceManager} to be initialized
					@see    TextPersistenceManager
			*/
			virtual void initPersistenceManager(TextPersistenceManager &pm)
				throw();

			/** Convert persistent objects.
					Override this method to convert \Ref{PersistentObject} objects to
					\Ref{Composite} objects. See \Ref{TextPersistenceManager} for information
					on how to convert \Ref{PersistentObject} objects into other objects.				
					@param  po a reference to a \Ref{PersistentObject} to be converted
					@return Composite* a converted \Ref{Composite} object if {\em po} could successfully be converted into a \Ref{Composite} object
			*/
			virtual Composite *convertObject(PersistentObject &po)
				throw();
			//@}

			/**	@name	Operators
			*/
			//@{
			/** Read from a socket stream.
					This method will be called by the \Ref{Server} to convert objects
					that are available at the \Ref{IOSocketStream} using the 
					\Ref{convertObject} method.
					It initializes the \Ref{TextPersistenceManager} by calling the method
					\Ref{initPersistenceManager} if it is not already
					initialized.
					Calls \Ref{initPersistenceManager}.
					Calls \Ref{convertObject}.
					@param iostream_socket a reference to a \Ref{IOSocketStream} object
					@return Composite* a pointer to a received \Ref{Composite} object ({\tt 0} if no composite object was received)
			*/
			Composite *operator() (IOStreamSocket &iostream_socket)
				throw();
			//@}

			/**	@name	Storers
			*/	
			//@{

			/** Persistent stream output and state restorage.
  			  Read persistent server data from the input stream {\em s} and 
				  restore the state of {\em *this} objectCreator.\\
				  {\bf Note:} Not yet implemented.
				  @param s input stream from where to restore the internal state of {\em *this} objectCreator
			*/
			virtual void read(std::istream& s)
				throw();

			/** Persistent stream output and state storage.
  			  Write persistent server data to the output stream {\em s} and 
				  store the state of {\em *this}.\\
				  {\bf Note:} Not yet implemented.
				  @param s output stream to where to store the internal state of {\em *this} objectCreator
			*/
			virtual void write(std::ostream& s) const
				throw();
			//@}

			
			protected:

			private:
			
			bool init_;
			TextPersistenceManager pm_;
		};



#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/GUI/KERNEL/objectCreator.iC>
#		endif

	}// namespace VIEW
		
}// namespace BALL

#endif // BALL_VIEW_GUI_KERNEL_OBJECTCREATOR_H
