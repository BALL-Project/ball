// $Id: embeddable.h,v 1.2 2000/07/16 19:27:27 oliver Exp $

#ifndef BALL_CONCEPT_EMBEDDABLE_H
#define BALL_CONCEPT_EMBEDDABLE_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_DATATYPE_LIST_H
#	include <BALL/DATATYPE/list.h>
#endif

#ifndef BALL_DATATYPE_STRINGHASHMAP_H
#	include <BALL/DATATYPE/stringHashMap.h>
#endif

#ifndef BALL_DATATYPE_HASHMAP_H
#	include <BALL/DATATYPE/hashMap.h>
#endif

#include <typeinfo>

namespace BALL 
{

	///
	#define BALL_EMBEDDABLE(TYPE)\
		virtual void registerThis() throw() { Embeddable::registerInstance_(typeid(TYPE), this); };\
		static TYPE* getInstance(Position index) throw() { return dynamic_cast<TYPE*>(Embeddable::getInstance_(typeid(TYPE), index)); };\
		static Size countInstances() throw() { return (Embeddable::countInstances_(typeid(TYPE))); };
	
	/**	Pyhton Embedding Base Class.
			This class defines a common interface for all classes that
			have to be accessible from an embedded Python interpreter.
			\\
			{\bf Definition:} \URL{BALL/CONCEPT/embeddable.h}		
			\\
			@see PyInterpreter
			@see MainControl
			@see ModularWidget
	*/
	class Embeddable
	{
		public:

		/**	@name Type definitions
		*/
		//@{
		typedef List<Embeddable*> EmbeddableList;
		//@}

		/**	@name Constructors and Destructors
		*/
		//@{
		
		/**
		*/
		virtual ~Embeddable();
		//@}

		/**	@name	Accessors
		*/
		//@{

		/**	Unregister the instance.
		*/
		void unregisterThis() throw();

		/**	Register the instance.
				DO NOT IMPLEMENT THIS METHOD! It is automatically implemented
				correctly when putting the \Ref{BALL_EMBEDDABLE} macro in a class
				definition.
				@see getInstanceList
		*/
		virtual void registerThis() throw();	

		//@}
			
		protected:
		/**
		*/
		static void registerInstance_(const std::type_info& type, Embeddable* instance) throw();
		/**
		*/
		static void unregisterInstance_(Embeddable* instance) throw();
		
		/**	Return the number of instances of a certain type
		*/
		static Size countInstances_(const std::type_info& type) throw ();

		/**	Return a specified instance of a registerd type.
				If the index is out of bounds or the position is
				invalid, a null pointer is returned 
		*/
		static Embeddable* getInstance_(const std::type_info& type, Position index) throw();

		private:

		static StringHashMap<EmbeddableList>	instance_lists_;
		static HashMap<Embeddable*, string>		instance_to_type_map_;
	};

} // namespace BALL

#endif // BALL_CONCEPT_EMBEDDABLE_H
