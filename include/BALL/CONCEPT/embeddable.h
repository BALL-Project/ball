// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: embeddable.h,v 1.16 2003/09/07 17:23:59 oliver Exp $
//

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

	/** Embeddable macro.
			This macro defines all methods required for clases
			derived from Embeddable. It should be included in the class definition
			of each of these classes, even for classes further down in the inheritance.
			The argument ist just the class type, e.g. BALL::ModularWidget.
	*/
	#define BALL_EMBEDDABLE(TYPE)\
		virtual void registerThis() throw() \
		{ \
			if (typeid(*this) != typeid(TYPE))\
			{\
				Log.error() << "Warning: derived class " << typeid(*this).name() << " was derived from BALL::Embeddable, but the macro " << std::endl\
                  << "BALL_EMBEDDABLE(...) was not specified in the class declaration!" << std::endl;\
			}\
			Embeddable::registerInstance_(typeid(TYPE), this);\
		}\
		\
		static TYPE* getInstance(Position index) throw() { return dynamic_cast<TYPE*>(Embeddable::getInstance_(typeid(TYPE), index)); };\
		static TYPE* getInstance(const String& identifier) throw() { return dynamic_cast<TYPE*>(Embeddable::getInstance_(typeid(TYPE), identifier)); };\
		static Size countInstances() throw() { return (Embeddable::countInstances_(typeid(TYPE))); };
	
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

		/**	Default constructor
		*/
		Embeddable(const String& identifier = "<Embeddable>")
			throw();
		
		/**	Copy constructor
		*/
		Embeddable(const Embeddable& embeddable)
			throw();
		
		/**
		*/
		virtual ~Embeddable()
			throw();
		//@}

		/**	@name	Accessors
		*/
		//@{

		/**	Return the instance identifier
		*/
		void setIdentifier(const String& identifier)
			throw();

		/**	Assign a new identifier
		*/
		const String& getIdentifier() const
			throw();

		/**	Unregister the instance.
		*/
		void unregisterThis()
			throw();

		/**	Register the instance.
				DO NOT IMPLEMENT THIS METHOD! It is automatically implemented
				correctly when putting the  \link BALL_EMBEDDABLE BALL_EMBEDDABLE \endlink  macro in a class
				definition.
				@see getInstanceList
		*/
		virtual void registerThis()
			throw();	

		//@}
			
		protected:
		/**
		*/
		static void registerInstance_(const std::type_info& type, const Embeddable* instance)
			throw();
		/**
		*/
		static void unregisterInstance_(const Embeddable* instance)
			throw();
		
		/**	Return the number of instances of a certain type
		*/
		static Size countInstances_(const std::type_info& type)
			throw();

		/**	Return an instance of a registered type by its index.
				If the index is out of bounds or the position is
				invalid, a null pointer is returned 
		*/
		static Embeddable* getInstance_(const std::type_info& type, Position index)
			throw();

		/**	Return an instance of a registered type by its identifier.
				If the identifier does not exist, a null pointer is returned 
		*/
		static Embeddable* getInstance_(const std::type_info& type, const String& identifier)
			throw();


		private:
		
		/**	An identifier for the class
		*/
		String	identifier_;

		/**	The instance lists
		*/
		static StringHashMap<EmbeddableList>	instance_lists_;

		/**	A hash map to retrieve the class ID for each instance.
		*/
		static HashMap<Embeddable*, string>		instance_to_type_map_;
	};

} // namespace BALL

#endif // BALL_CONCEPT_EMBEDDABLE_H
