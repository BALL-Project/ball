// $Id: persistentObject.h,v 1.3 2000/08/26 14:58:15 amoll Exp $

#ifndef BALL_CONCEPT_PERSISTENTOBJECT_H
#define BALL_CONCEPT_PERSISTENTOBJECT_H

namespace BALL 
{

	class PersistenceManager;

	/**	The persistent object.
			Each object that needs persistence, has to be derived from PersistentObject to
			define an interface to the \Ref{PersistenceManager}.\\
			The class itself has no functionality except for the {\tt operator >>}.
			{\bf Definition:} \URL{BALL/CONCEPT/persistentObject.h}
	*/
	class PersistentObject
	{	
		public:

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
				The default constructor has no functionality.
		*/
		PersistentObject() 
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
				The method first calls {\tt pm.startOutput()} to write the necessary
				headers. Then, {\tt persistentWrite(pm)} is called to serialize
				the object. Finally, a call to {\tt pm.stopOutput()} writes all
				dependend objects and the end marker to the persistent stream.
				@param	pm	the persistence manager
				@return the persistence manager (for chaining multiple output operations)
		*/
		PersistenceManager& operator >> (PersistenceManager& pm) const;

		/**	Serialize the object.
				This method serializes the object by calls to Layer 1 methods of \Ref{PersistenceManager}.
				The first method in the implementation of persistentRead should be a call
				to {\tt pm.writeObjectHeader(*this, name)}. Likewise the last method
				in this method should be {\tt pm.writeObjectTrailer(name)}.
				@param	pm the persistence manager
				@param	name the name of the object (0 for writing base classes)
		*/
		virtual void persistentWrite(PersistenceManager& pm, const char* name = "") const;

		/**	Deserialize the object.
				This method reads the contents of an persistent object into an (already existing!)
				object. It is implemented using Layer 1 commands of PersistenceManager.
				To each {\bf write} command used in persistentWrite, there should be exactly
				the same call to a read command in persistentRead. Only the header and trailer
				(writeObjectHeader/writeObjectTrailer calls) have to be omitted, as the header is
				read before this method is called (the object type has to be known in advance
				to dynamically create the object).
				@param pm the PersistenceManager
		*/
		virtual void persistentRead(PersistenceManager& pm);

		/**	Finalize the deserialization.
				This method is called for all objects after their pointers have been demangled.
				It is intended for the adaptation of internal data structures. For example, the bond class 
				has to swap {\tt first\_} and {\tt second\_} depending on the order of the
				atoms. It is usually left unimplemented.
		*/
		virtual void finalize()
		{
		}
		//@}

	};
	
} // namespace BALL

#endif // BALL_CONCEPT_PERSISTENTOBJECT_H
