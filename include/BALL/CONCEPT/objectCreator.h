// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_CONCEPT_OBJECTCREATOR_H
#define BALL_CONCEPT_OBJECTCREATOR_H

#ifndef BALL_CONCEPT_COMPOSITE_H
#	include <BALL/CONCEPT/composite.h>
#endif

#ifndef BALL_CONCEPT_PERSISTENTOBJECT_H       
# include <BALL/CONCEPT/persistentObject.h>
#endif

#ifndef BALL_CONCET_TEXTPERSISTENCEMANAGER_H
# include <BALL/CONCEPT/textPersistenceManager.h>
#endif

#ifdef BALL_HAS_ASIO
#ifndef BALL_SYSTEM_NETWORKING_H
#	include <BALL/SYSTEM/networking.h>
#endif
#endif

#include <vector>

namespace BALL
{
	/** ObjectCreator class.
			The class ObjectCreator is a base class for converting PersistentObject
			objects into Composite objects. It defines methods for initialization
			and conversion that must be overriden by the derived class. 
			This class is used by the class Server to convert incoming
			PersistentObject objects to Composite objects.
	*/
	class BALL_EXPORT ObjectCreator
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
			;

		//@}
		/** @name Destructors 
		*/
		//@{

		/** Destructor.
		*/
		virtual ~ObjectCreator()
			;

		/** Explicit default initialization.
				Empty for further purpose.
		*/
		virtual void clear()
			;

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
			;

		/** Convert persistent objects.
				Override this method to convert PersistentObject objects to
				Composite objects. See TextPersistenceManager for information
				on how to convert PersistentObject objects into other objects.				
				\param  po a reference to a PersistentObject to be converted
				\return Composite* a converted Composite object if <b> po</b> could successfully be converted 
									into a Composite object
		*/
		virtual Composite *convertObject(PersistentObject &po)
			;
						
		//@}
		/**	@name	Operators
		*/
		//@{

#ifdef BALL_HAS_ASIO
		/** Read from a socket stream.
				This method will be called by the Server to convert objects
				that are available at the TCPIOStream using the 
				convertObject method.
				It initializes the TextPersistenceManager by calling the method
				initPersistenceManager if it is not already
				initialized.
				Calls initPersistenceManager.
				Calls convertObject.
				\param tcp_iostream a reference to a TCPIOStream object
				\return Composite* a pointer to a received Composite object (<tt> 0</tt> if no composite object was received)
		*/
		Composite *operator() (TCPIOStream &tcp_iostream)
			;
#endif

		//@}

		private:
		
		bool init_;
		TextPersistenceManager pm_;
	};
  
}// namespace BALL

#endif // BALL_CONCEPT_OBJECTCREATOR_H
