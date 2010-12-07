// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_CONCEPT_EMBEDDABLE_H
#define BALL_CONCEPT_EMBEDDABLE_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
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

	/** Embeddable macro.
			This macro defines all methods required for clases
			derived from Embeddable. It should be included in the class definition
			of each of these classes, even for classes further down in the inheritance.
			The argument ist just the class type, e.g. BALL::ModularWidget.
	*/
	#define BALL_EMBEDDABLE(TYPE,BASE)\
		virtual void registerThis()  \
		{ \
			if (typeid(*this) != typeid(TYPE))\
			{\
				Log.error() << "Warning: derived class " << typeid(*this).name() << " was derived from BALL::Embeddable, but the macro\n"\
                  << "BALL_EMBEDDABLE(...) was not specified in the class declaration!" << std::endl;\
			}\
			Embeddable::registerInstance_(typeid(TYPE), this);\
			Embeddable::registerInstance_(typeid(BASE), this);\
		}\
		\
		static TYPE* getInstance(Position index)  \
		{ \
			Embeddable* ptr = Embeddable::getInstance_(typeid(TYPE), index);\
			if (ptr != 0)\
			{\
				return dynamic_cast<TYPE*>(ptr); \
			}\
			else\
			{\
				return 0;\
			}\
		}\
		static TYPE* getInstance(const String& identifier) \
		{\
			Embeddable* ptr = Embeddable::getInstance_(typeid(TYPE), identifier);\
			if (ptr != 0)\
			{\
				return dynamic_cast<TYPE*>(ptr); \
			}\
			else\
			{\
				return 0;\
			}\
		}\
		static Size countInstances()  { return (Embeddable::countInstances_(typeid(TYPE))); };
	
	/**	Python Embedding Base Class.
			This class defines a common interface for all classes that
			have to be accessible from an embedded Python interpreter.
			Each instance of a class derived from embeddable can be registered by calling
			registerThis() of the instance and is then accessible through the static methods
			of the class (e.g. getInstance).
		 \par
			@see PyInterpreter
			@see MainControl
			@see ModularWidget
	 	 \ingroup ConceptsMiscellaneous
	*/
	class BALL_EXPORT Embeddable
	{
		public:

		/**	@name Type definitions
		*/
		//@{
		typedef std::vector<Embeddable*> EmbeddableVector;
		//@}

		/**	@name Constructors and Destructors
		*/
		//@{

		/**	Default constructor
		*/
		Embeddable(const String& identifier = "<Embeddable>")
			;
		
		/**	Copy constructor
		*/
		Embeddable(const Embeddable& embeddable)
			;
		
		/**
		*/
		virtual ~Embeddable()
			;
		//@}

		/**	@name	Accessors
		*/
		//@{

		/**	Return the instance identifier
		*/
		void setIdentifier(const String& identifier)
			;

		/**	Assign a new identifier
		*/
		const String& getIdentifier() const
			;

		/**	Unregister the instance.
		*/
		void unregisterThis()
			;

		/**	Register the instance.
				DO NOT IMPLEMENT THIS METHOD! It is automatically implemented
				correctly when putting the  \link BALL_EMBEDDABLE BALL_EMBEDDABLE \endlink  macro in a class
				definition.
				@see getInstanceVector
		*/
		virtual void registerThis()
			;	

		//@}
		
		/** Internal value dump.
				Dump the current state to 
				the output ostream <b> s</b> with dumping depth <b> depth</b>.
				\param   s output stream where to output the state 
				\param   depth the dumping depth
		*/
		virtual void dump(std::ostream& s = std::cout, Size depth = 0) const
			;

		protected:
		/**
		*/
		static void registerInstance_(const std::type_info& type, const Embeddable* instance)
			;
		/**
		*/
		static void unregisterInstance_(const Embeddable* instance)
			;
		
		/**	Return the number of instances of a certain type
		*/
		static Size countInstances_(const std::type_info& type)
			;

		/**	Return an instance of a registered type by its index.
				If the index is out of bounds or the position is
				invalid, a null pointer is returned 
		*/
		static Embeddable* getInstance_(const std::type_info& type, Position index)
			;

		/**	Return an instance of a registered type by its identifier.
				If the identifier does not exist, a null pointer is returned 
		*/
		static Embeddable* getInstance_(const std::type_info& type, const String& identifier)
			;


		private:
		
		/**	An identifier for the class
		*/
		String	identifier_;

		/**	The instance vectors
		*/
		static StringHashMap<EmbeddableVector>	instance_vectors_;

		/**	A hash map to retrieve the class ID for each instance.
		*/
		static HashMap<Embeddable*, string>		instance_to_type_map_;
	};

} // namespace BALL

#endif // BALL_CONCEPT_EMBEDDABLE_H
