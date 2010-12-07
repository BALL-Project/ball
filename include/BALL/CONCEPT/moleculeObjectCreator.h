// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_CONCEPT_MOLECULEOBJECTCREATOR_H
#define BALL_CONCEPT_MOLECULEOBJECTCREATOR_H

#ifndef BALL_CONCEPT_OBJECTCREATOR_H
#	include <BALL/CONCEPT/objectCreator.h>
#endif
  
namespace BALL
{
	/** MoleculeObjectCreator class.
			The class MoleculeObjectCreator is a class for converting PersistentObject
			objects that represent molecular structures into Composite objects.
			Objects that do not represent molecular structures will not be converted.
			All necessary initialization and registration of molecular structure patterns
			will be done in the method initPersistenceManager.
			The conversion will be executed in the method convertObject.
			This class is used by the class Server to convert incoming
			PersistentObject objects to Composite objects. \par
		 \ingroup  Persistence
	*/
	class BALL_EXPORT MoleculeObjectCreator: public ObjectCreator
	{
		public:

		/**	@name	Constructors
		*/	
		//@{

		/** Default Constructor.
		*/
		MoleculeObjectCreator()
			;

		//@}
		/** @name Destructors 
		*/
		//@{

		/** Destructor.
		*/
		virtual ~MoleculeObjectCreator()
			;

		/** Explicit default initialization.
				Calls ObjectCreator::clear.
		*/
		virtual void clear()
			;

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
			;

		/** Converts persistent objects.
				If <b> po </b> is an object that represents a molecular structure (an Atom,
				Protein or System) it will be converted into a Composite
				object and returned. If it is none of the above <tt> 0</tt> will be returned.
				See TextPersistenceManager for information
				on how to convert PersistentObject objects into other objects.				
				\param  po a reference to a PersistentObject to be converted
				\return Composite* a converted Composite object if <b> po</b> could successfully be converted into a Composite object
				\see    initPersistenceManager
		*/
		virtual Composite *convertObject(PersistentObject &po)
			;
		//@}
	};

} // namespace BALL

#endif // BALL_CONCEPT_MOLECULEOBJECTCREATOR_H
