// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: objectCreator.h,v 1.13 2003/03/26 13:08:54 sturm Exp $

#ifndef BALL_VIEW_GUI_KERNEL_OBJECTCREATOR_H
#define BALL_VIEW_GUI_KERNEL_OBJECTCREATOR_H

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
				The class ObjectCreator is a base class for converting  \link PersistentObject PersistentObject \endlink 
				objects into  \link Composite Composite \endlink  objects. It defines methods for initialization
				and conversion that must be overriden by the derived class. 
				This class is used by the class  \link Server Server \endlink  to convert incoming
				 \link PersistentObject PersistentObject \endlink  objects to  \link Composite Composite \endlink  objects.
		\ingroup ViewGuiKernelServer
		*/
		class ObjectCreator
		{
			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new objectCreator.
					The state of {\em *this} server is:  \link TextPersistenceManager TextPersistenceManager \endlink 
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
					Calls  \link destroy destroy \endlink .
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
					Initialize the  \link TextPersistenceManager TextPersistenceManager \endlink  of {\em *this} objectCreator.
					Override this method to initialize the persistence manager for specified
					objects. See  \link TextPersistenceManager TextPersistenceManager \endlink  for information on how and why
					to register  \link PersistentObject PersistentObject \endlink  classes.
					@param  pm the  \link TextPersistenceManager TextPersistenceManager \endlink  to be initialized
					@see    TextPersistenceManager
			*/
			virtual void initPersistenceManager(TextPersistenceManager &pm)
				throw();

			/** Convert persistent objects.
					Override this method to convert  \link PersistentObject PersistentObject \endlink  objects to
					 \link Composite Composite \endlink  objects. See  \link TextPersistenceManager TextPersistenceManager \endlink  for information
					on how to convert  \link PersistentObject PersistentObject \endlink  objects into other objects.				
					@param  po a reference to a  \link PersistentObject PersistentObject \endlink  to be converted
					@return Composite* a converted  \link Composite Composite \endlink  object if {\em po} could successfully be converted 
										into a  \link Composite Composite \endlink  object
			*/
			virtual Composite *convertObject(PersistentObject &po)
				throw();
							
			//@}
			/**	@name	Operators
			*/
			//@{

			/** Read from a socket stream.
					This method will be called by the  \link Server Server \endlink  to convert objects
					that are available at the  \link IOSocketStream IOSocketStream \endlink  using the 
					 \link convertObject convertObject \endlink  method.
					It initializes the  \link TextPersistenceManager TextPersistenceManager \endlink  by calling the method
					 \link initPersistenceManager initPersistenceManager \endlink  if it is not already
					initialized.
					Calls  \link initPersistenceManager initPersistenceManager \endlink .
					Calls  \link convertObject convertObject \endlink .
					@param iostream_socket a reference to a  \link IOSocketStream IOSocketStream \endlink  object
					@return Composite* a pointer to a received  \link Composite Composite \endlink  object (<tt>0</tt> if no composite object was received)
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

#endif // BALL_VIEW_GUI_KERNEL_OBJECTCREATOR_H
