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

	class PersistenceManager;

	/**	The persistent object.
			Each object that needs persistence, has to be derived from
			PersistentObject to define an interface to the
			 \link PersistenceManager PersistenceManager \endlink .
			 \par
			The class itself has no functionality except for the <tt>operator >></tt>.
			 \par
			
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

		/**	@name	Persistent Writing and Reading 
		*/
		//@{
			
		/**	Stream operator.
				This operator is used to serialize an instance of PersistentObject.
				The method first calls <tt>pm.startOutput()</tt> to write the necessary
				headers. Then, <tt>persistentWrite(pm)</tt> is called to serialize
				the object. Finally, a call to <tt>pm.stopOutput()</tt> writes all
				dependend objects and the end marker to the persistent stream.
				@param	pm	the persistence manager
				\throws Exception::GeneralException
				@return the persistence manager (for chaining multiple output 
								operations)
		*/
		PersistenceManager& operator >> (PersistenceManager& pm) const;

		/**	Serialize the object.
				This method serializes the object by calls to Layer 1 methods of 
				 \link PersistenceManager PersistenceManager \endlink .  The first method in the implementation
				of persistentRead should be a call to 
				<tt>pm.writeObjectHeader(*this, name)</tt>. Likewise the last method in
				this method should be <tt>pm.writeObjectTrailer(name)</tt>.
				@param	pm the persistence manager
				@param	name the name of the object (0 for writing base classes)
				\throws Exception::GeneralException
		*/
		virtual void persistentWrite(PersistenceManager& pm, const char* name = "") const;

		/**	Deserialize the object.
				This method reads the contents of an persistent object into an
				(already existing!) object. It is implemented using Layer 1
				commands of PersistenceManager.  To each <b>write</b> command used
				in persistentWrite, there should be exactly the same call to a read
				command in persistentRead. Only the header and trailer
				(writeObjectHeader/writeObjectTrailer calls) have to be omitted, as
				the header is read before this method is called (the object type
				has to be known in advance to dynamically create the object).
				@param pm the PersistenceManager
				\throws Exception::GeneralException
		*/
		virtual void persistentRead(PersistenceManager& pm);

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
