// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: objectCreator.h,v 1.2 2003/08/26 15:26:42 amoll Exp $
//

#ifndef BALL_VIEW_KERNEL_OBJECTCREATOR_H
#define BALL_VIEW_KERNEL_OBJECTCREATOR_H

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

#ifndef BALL_VIEW_KERNEL_COMMON_H
#	include <BALL/VIEW/KERNEL/common.h>
#endif

#include <vector>

namespace BALL
{
	namespace VIEW
	{
		/** ObjectCreator class.
				The class ObjectCreator is a base class for converting PersistentObject
				objects into Composite objects. It defines methods for initialization
				and conversion that must be overriden by the derived class. 
				This class is used by the class Server to convert incoming
				PersistentObject objects to Composite objects.
		*/
		class ObjectCreator
		{
			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new objectCreator.
					The state of this server is: TextPersistenceManager
					not initialized.
					\return      ObjectCreator new constructed objectCreator
					\see         TextPersistenceManager
			*/
			ObjectCreator()
				throw();

			//@}
			/** @name Destructors 
			*/
			//@{

			/** Destructor.
			*/
			virtual ~ObjectCreator()
				throw();

			/** Explicit default initialization.
					Empty for further purpose.
			*/
			virtual void clear()
				throw();

			//@}
			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{		

			/** Initialize the persistence manager.
					Initialize the TextPersistenceManager of this objectCreator.
					Override this method to initialize the persistence manager for specified
					objects. See TextPersistenceManager for information on how and why
					to register PersistentObject classes.
					\param  pm the TextPersistenceManager to be initialized
					\see    TextPersistenceManager
			*/
			virtual void initPersistenceManager(TextPersistenceManager &pm)
				throw();

			/** Convert persistent objects.
					Override this method to convert PersistentObject objects to
					Composite objects. See TextPersistenceManager for information
					on how to convert PersistentObject objects into other objects.				
					\param  po a reference to a PersistentObject to be converted
					\return Composite* a converted Composite object if <b> po could successfully be converted 
										into a Composite object
			*/
			virtual Composite *convertObject(PersistentObject &po)
				throw();
							
			//@}
			/**	@name	Operators
			*/
			//@{

			/** Read from a socket stream.
					This method will be called by the Server to convert objects
					that are available at the IOSocketStream using the 
					convertObject method.
					It initializes the TextPersistenceManager by calling the method
					initPersistenceManager if it is not already
					initialized.
					Calls initPersistenceManager.
					Calls convertObject.
					\param iostream_socket a reference to a IOSocketStream object
					\return Composite* a pointer to a received Composite object (<tt> 0 if no composite object was received)
			*/
			Composite *operator() (IOStreamSocket &iostream_socket)
				throw();

			//@}

			private:
			
			bool init_;
			TextPersistenceManager pm_;
		};

  
	}// namespace VIEW
}// namespace BALL

#endif // BALL_VIEW_KERNEL_OBJECTCREATOR_H
