// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: moleculeObjectCreator.h,v 1.2 2003/08/26 08:04:42 oliver Exp $

#ifndef BALL_MOLVIEW_GUI_KERNEL_MOLECULEOBJECTCREATOR_H
#define BALL_MOLVIEW_GUI_KERNEL_MOLECULEOBJECTCREATOR_H

#ifndef BALL_VIEW_GUI_KERNEL_OBJECTCREATOR_H
#	include <BALL/VIEW/GUI/KERNEL/objectCreator.h>
#endif
  
namespace BALL
{
	namespace MOLVIEW
	{
		using VIEW::ObjectCreator;

		/** MoleculeObjectCreator class.
				The class MoleculeObjectCreator is a class for converting PersistentObject
				objects that represent molecular structures into Composite objects.
				Objects that do not represent molecular structures will not be converted.
				All necessary initialization and registration of molecular structure patterns
				will be done in the method initPersistenceManager.
				The conversion will be executed in the method convertObject.
				This class is used by the class Server to convert incoming
				PersistentObject objects to Composite objects. \par
		*/
		class MoleculeObjectCreator: public ObjectCreator
		{
			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
			*/
			MoleculeObjectCreator()
				throw();

			//@}
			/** @name Destructors 
			*/
			//@{

			/** Destructor.
			*/
			virtual ~MoleculeObjectCreator()
				throw();

			/** Explicit default initialization.
					Calls ObjectCreator::clear.
			*/
			virtual void clear()
				throw();

			//@}
			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{		

			/** Initializes the TextPersistenceManager of this moleculeObjectCreator.
					Registers all necessary molecular structures available in BALL.
					See TextPersistenceManager for information on how and why
					to register PersistentObject classes.
					\param  pm the TextPersistenceManager to be initialized
					\see    TextPersistenceManager
					\see    convertObject
			*/
			virtual void initPersistenceManager(TextPersistenceManager &pm)
				throw();

			/** Converts persistent objects.
					If <b> po is an object that represents a molecular structure (an Atom,
					Protein or System) it will be converted into a Composite
					object and returned. If it is none of the above <tt> 0</tt> will be returned.
					See TextPersistenceManager for information
					on how to convert PersistentObject objects into other objects.				
					\param  po a reference to a PersistentObject to be converted
					\return Composite* a converted Composite object if <b> po could successfully be converted into a Composite object
					\see    initPersistenceManager
			*/
			virtual Composite *convertObject(PersistentObject &po)
				throw();
			//@}
		};

	} // namespace MOLVIEW
} // namespace BALL

#endif // BALL_MOLVIEW_GUI_KERNEL_MOLECULEOBJECTCREATOR_H 
