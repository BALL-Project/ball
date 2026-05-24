// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_CONCEPT_PERSISTENTOBJECT_H
#define BALL_CONCEPT_PERSISTENTOBJECT_H

#ifndef BALL_CONCEPT_OBJECT_H
#	include <BALL/CONCEPT/object.h>
#endif

namespace BALL
{

	/**	The persistent object.
			v2.2 (PR-removal phase 6): the persistence STREAM framework
			(PersistenceManager + the persistentRead/Write virtuals + operator>>)
			has been removed. PersistentObject is RETAINED as a near-empty base
			TYPE for object-valued NamedProperty values, VIEW Stage::Material, and
			PDB/crystal smart-properties (its base-erasure is deferred to H6a/D49 +
			VIEW). The only remaining hook is finalize().

	 	 \ingroup  Persistence
	*/
	class BALL_EXPORT PersistentObject
		:	public Object
	{	
		public:

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
				The default constructor has no functionality.
		*/
		PersistentObject() 
			:	Object()
		{
		}
		
		/**	Destructor.
				The destructor has no functionality.
		*/
		virtual ~PersistentObject() 
		{
		}
		//@}

		/**	@name	Finalization
		*/
		//@{

		/**	Finalize the deserialization.
				This method is called for all objects after their pointers have
				been demangled. It is intended for the adaptation of internal data
				structures. For example, the bond class has to swap <tt>first_</tt>
				and <tt>second_</tt> depending on the order of the atoms. It is
				usually left unimplemented.
				\throws Exception::GeneralException
		*/
		virtual void finalize()
		{
		}
		//@}

	};
	
} // namespace BALL

#endif // BALL_CONCEPT_PERSISTENTOBJECT_H
