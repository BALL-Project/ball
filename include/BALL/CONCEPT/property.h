// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: property.h,v 1.40 2005/12/23 17:01:41 amoll Exp $
//

#ifndef BALL_CONCEPT_PROPERTY_H
#define BALL_CONCEPT_PROPERTY_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_DATATYPE_BITVECTOR_H
#	include <BALL/DATATYPE/bitVector.h>
#endif

#ifndef BALL_CONCEPT_PERSISTENTOBJECT_H
#	include <BALL/CONCEPT/persistentObject.h>
#endif

#include <iostream>

#include <boost/shared_ptr.hpp>
#include <boost/any.hpp>

namespace BALL 
{

	/**	@name	Properties
	 	 \ingroup ConceptsMiscellaneous
	*/
	//@{
	
	/** Named Property Class.
			
	*/
	class BALL_EXPORT NamedProperty
		:	public PersistentObject
	{
		public:

		BALL_CREATE(NamedProperty)

		/**	@name Enums
		*/
		//@{
		/**	The different types of data a NamedProperty may contain.
				
		*/
		enum Type
		{
			/**	Bool-type properties can have the value <b>true</b> or 
					<b>false</b>.
			*/
			BOOL,

			/** Int-type properties contain a variable of type <b>int</b>
			*/
			INT,

			/** Unsigned-int-type properties contain a variable of type 
					<b>unsigned int</b>
			*/
			UNSIGNED_INT,

			/**	Float-type properties contain a variable of type <b>double</b>
			*/
			FLOAT,

			/**	Double-type properties contain a variable of type <b>double</b>
			*/
			DOUBLE,

			/** String-type properties contain a pointer to a string.
					When destructing the property, the string is destructed, too.
			*/
			STRING,

			/**	Object-type properties contain a pointer to a PersistentObject.
					Remember to destruct the objects if they are not needed any more!
					the Destructor of NamedProperty will not delete this object!
			*/
			OBJECT,

			/** Properties of this type do not contain any data.
					Use this type to indicate that an object has a certain property.
					The type can be seen as an extension of the bit properties 
					used in PropertyManager except for the user-defined name.
			*/
			NONE,

			/**	Object-type properties contain a smart pointer to a PersistentObject.
			    Here, destruction will autmatically be handled by the smart pointer
					implementation.
			 */
			SMART_OBJECT
		};
		
		//@}
		/**	@name	Constructors and Destructors 
		*/
		//@{

		/*	The default constructor
		*/
		NamedProperty();

		/**	Standard constructor.
				Creates an object of type NONE. Use this constructor to create
				it properties with user defined names.
				The <tt>Type</tt> is set to  \link OBJECT OBJECT \endlink .
				@param	name the propertie's name
		*/
		NamedProperty(const std::string& name);

		/**	Constructor for bool-type properties.
				Creates a NamedProperty object containing a boolean value.
				The <tt>Type</tt> is set to  \link BOOL BOOL \endlink .
				@param	name the property's name
				@param	value the boolean value stored in the property
		*/	
		NamedProperty(const std::string& name, bool value);

		/**	Constructor for int-type properties.
				Creates a NamedProperty object containing a signed int value.
				The <tt>Type</tt> is set to  \link INT INT \endlink .
				@param	name the property's name
				@param	value the int value stored in the property
		*/
		NamedProperty(const std::string& name, int value);

		/** Constructor for unsigned int-type properties.
				Creates a NamedProperty object containing an unsigned int value.
				The <tt>Type</tt> is set to  \link UNSIGNED_INT UNSIGNED_INT \endlink .
				@param	name the property's name
				@param	value the int value stored in the property
		*/
		NamedProperty(const std::string& name, unsigned int value);

		/** Constructor for float-type properties.
				Creates a NamedProperty object containing a float value.
				The <tt>Type</tt> is set to  \link FLOAT FLOAT \endlink .
				@param	name the property's name
				@param	value the float value stored in the property
		*/
		NamedProperty(const std::string& name, float value);

		/** Constructor for double-type properties.
				Creates a NamedProperty object containing a double value.
				The <tt>Type</tt> is set to  \link DOUBLE DOUBLE \endlink .
				@param	name the property's name
				@param	value the double value stored in the property
		*/
		NamedProperty(const std::string& name, double value);

		/** Constructor for string-type properties.
				Objects of type STRING contain a pointer to a string
				object. This object is constructed as a copy of <tt>str</tt>
				The <tt>Type</tt> is set to  \link STRING STRING \endlink .
				@param	name the property's name
				@param	str the string stored in the property
		*/
		NamedProperty(const std::string& name, const std::string& str);

		/**	Constructor for object-type properties.
				Creates a NamedProperty object containing a 
				pointer to a persistent object.
				The <tt>Type</tt> is set to  \link OBJECT OBJECT \endlink .
				@param	name the property's name
				@param	po a reference to the persistent object stored in the property
		*/
		NamedProperty(const std::string& name, PersistentObject& po);

		/**	Constructor for smart-pointer handled object-type properties.
				Creates a NamedProperty object containing a 
				smart pointer to a persistent object.
				The <tt>Type</tt> is set to  \link SMART_OBJECT SMART_OBJECT \endlink .
				@param	name the property's name
				@param	ptr a smart pointer to the persistent object
		*/
		NamedProperty(const std::string& name, boost::shared_ptr<PersistentObject>& po);

		/**	Copy constructor
		*/
		NamedProperty(const NamedProperty&);

		/**
		 * Copy constructor with renaming
		 */
		NamedProperty(const NamedProperty&, const std::string& name);

		/**	Destructor .
				The destructor destructs the contained data in the case of STRING-type 
				properties only.
		*/
		~NamedProperty();

		/** Clear method
		*/
		virtual void clear();
		
		//@}	
		/**	@name Persistence
		*/
		//@{
			
		/**	Write a persistent copy of the object.
				\throws Exception::GeneralException
		*/
    virtual void persistentWrite(PersistenceManager& pm, const char* name = "") const;

		/**	Retrieve a persistent copy of the object
			  \throws Exception::GeneralException
		*/
    virtual void persistentRead(PersistenceManager& pm);
		
		//@}
		/**	@name	 Accessors 
		*/
		//@{

		/// Return the type of the data stored in the property object
		Type getType() const;
		
		/// Return the name of the property
		const std::string& getName() const;

		/** Return the data of the property object as bool.
				If the property object is not of BOOL type, <b>false</b>
				is returned.
		*/
		bool getBool() const;

		/** Return the data of the property object as int.
				If the property object is not of INT type, <b>0</b>
				is returned.
		*/
		int getInt() const;
			
		/** Return the data of the property object as float.
				If the property object is not of FLOAT type, <b>0.0</b>
				is returned.
		*/
		float getFloat() const;
			
		/** Return the data of the property object as double.
				If the property object is not of DOUBLE type, <b>0.0</b>
				is returned.
		*/
		double getDouble() const;
			
		/** Return the data of the property object as unsigned int.
				If the property object is not of UNSIGNED_INT type, <b>0</b>
				is returned.
		*/
		unsigned int getUnsignedInt() const;

		/** Return the data of the property object as a pointer to a PersistentObject.
				If the property object is not of OBJECT type, <b>0</b>
				is returned.
		*/
		PersistentObject* getObject() const;

		/** Return the data of the property object as a smart pointer to a PersistentObject.
				If the property object is not of SMART_OBJECT type, <b>0</b>
				is returned.
		*/
		boost::shared_ptr<PersistentObject> getSmartObject() const;

		/** Return the data of the property object as a string.
				If the property object is not of STRING type, <b>""</b>
				is returned.
		*/
		String getString() const;
		
		/** Converts the data contained in this NamedProperty to a string and returns it. \n
		Note that this function is intended as a convenience and does NOT depend on the type of the data (in constrast to getString()). */
		String toString() const;

		//@}

		/**	@name	Predicates */
		//@{

		/** Equality operator
				Two instances are equal if they have the same name and the same value.
		*/
		bool operator == (const NamedProperty& np) const;

		/** Inequality operator
				@see operator ==
		*/
		bool operator != (const NamedProperty& np) const;

		/** copy operator */
		void operator = (const NamedProperty& np);

		//@}

		private:

		/*_	The kind of information contained in the NamedProperty
		*/
		Type type_;

		/*_	The name of the property object 
		*/
		std::string name_;
		
		/*_	The data
		*/
		boost::any data_;

	};

	typedef std::vector<NamedProperty>::iterator NamedPropertyIterator;

	/**	Property Manager class.
			This class manages properties of a class.
			A property can be seen as easy way to extend the
			class BALL_EXPORT' attributes without changing its interface.
			There are different types of properties defined depending 
			on the kind of data stored. The simplest kind of property 
			is just stored as a boolean information: Either an object <b>  has </b>
			a certain property, or it does not. These properties are obviously coded 
			as a BitVector. However the meaning of the different bits has to be
			defined and the uniqueness of this meaning has to be guaranteed. In
			order to avoid this, all kernel classes derived from PropertyManager
			contain a public enum that defines a value NUMBER_OF_PROPERTIES. All
			user defined properties should	refer to this constant (e.g. by
			defining <tt>MY_PROPERTY = Molecule::NUMBER_OF_PROPERTIES + 1</tt>).
			 \par
			The second way of storing properties is much more flexible, but less
			efficient.  It uses the class NamedProperty that also allows to store
			additional information along with the property. In this case, each
			property is accessed via a unique name.
			 \par
			<b>Interface:</b> Storable
			 \par
			
	*/
	class BALL_EXPORT PropertyManager
	{
		public:
		
		BALL_CREATE(PropertyManager)

		/**	@name	Constructors and Destructors */
		//@{

		BALL_INLINE 

		/// Default constructor
		PropertyManager();

		BALL_INLINE
		/// Copy constructor
		PropertyManager(const PropertyManager& property_manager);

		/// Destructor
		virtual ~PropertyManager();

		/// Clears all properties
		virtual void clear();

		/// Clears all properties
		virtual void destroy();

		//@}
		/**	@name	Assignment 
		*/
		//@{

		/** Assign properties from another property manager
				@param property_manager the PropertyManager object to copy from
		*/
		void set(const PropertyManager& property_manager);

		/** Assignment operator.
				This operator calls <tt>set(property_manager)</tt> to assign
				a new object.
				@param property_manager the PropertyManager object to copy from
				@return PropertyManager <tt>*this</tt>
		*/
		const PropertyManager& operator = (const PropertyManager& property_manager);

		/** Assign properties to another property manager.
				This method copies the contents of the PropertyManager object
				to the given <tt>property_manager</tt>.
				@param	property_manager the target object
		*/
		void get(PropertyManager& property_manager) const;

		/** Swap the properties with another property manager.
				@param property_manager the PropertyManager object to swap the properties with
		*/
		void swap(PropertyManager& property_manager);

		//@}
		/**	@name	Accessors 
		*/
		//@{
			 
		/** Return a mutable reference to the bit vector.
				This method returns the bit vector containing the unnamed properties
				of the property manager.
				@return BitVector& a mutable reference to the (private) BitVector containing 
					the unnamed properties
		*/
		BitVector& getBitVector();

		/** Return a const reference to the bit vector containing the unnamed properties.
				This method returns the bit vector containing the unnamed properties
				of the property manager.
				@return BitVector& a const reference to the (private) BitVector containing 
					the unnamed properties
		*/
		const BitVector& getBitVector() const;
			
		//@}
		/**	@name Converters 
		*/
		//@{

		/** Converter to BitVector.	
				This operator converts a PropertyManager object to a BitVector
				object containing the unnamed properties.
				@return BitVector& a reference to the BitVector containing the
					unnamed properties of the property manager
		*/
		operator BitVector& ();

		//@}
		/**	@name	Handling unnamed bit properties 
		*/
		//@{

		/** Set a property.
				This method sets an unnamed property of a PropertyManager object by
				calling <tt>BitVector::setBit(property)</tt> for the private bit vector.
				@param property the number of the property to be set
		*/
		void setProperty(Property property);

		/** Clear a property.
				This method clears an unnamed property of a PropertyManager object by
				calling <tt>BitVector::clearBit(property)</tt> for the private bit vector.
				@param property the number of the property to be cleared
		*/
		void clearProperty(Property property);

		/** Toggle (invert) a property.
				This method clears an unnamed property of a PropertyManager object by
				calling <tt>BitVector::toggleBit(property)</tt> for the private bit vector.
				@param property the number of the property to be toggled
		*/
		void toggleProperty(Property property);

		/** Count all properties.
				This method returns the sum of unnamed and named properties.
				It counts the number of <b>  ones </b> in the bit vector
				and the number of properties stored in the named property vector.
		*/
		Size countProperties() const;
	
		//@}

		/**	@name	Handling named properties 
				PropertyManager contains an vector of  \link NamedProperty NamedProperty \endlink  objects.
				Each of the NamedProeprty objects has to have a <b>  unique </b> name.
				The setProperty methods ensure the uniqueness of this name.
				If a setProperty method is called for an existing name, the
				existing object is destructed prior to the insertion of the new object.
		*/
		//@{

		/**	Non-mutable access to a named property by its index.
				\throws Exception::IndexOverflow
		*/
		const NamedProperty& getNamedProperty(Position index) const;

		/**	Mutable access to a named property by its index.
				\throws Exception::IndexOverflow
		*/
		NamedProperty& getNamedProperty(Position index);

		/**	Set a named property.
				This method adds a new named property to the property manager's 
				array of named properties or substitutes an already existing
				named property by <tt>property</tt>. If a property with the same name as 
				<tt>property</tt> already exists, it is destructed.
				@param	property the new property to be set
		*/
		void setProperty(const NamedProperty& property);

		/**	Set a named property without any data.
				This method creates a new named property not containing any data
				(<tt>NamedProperty::Type == NONE</tt>).
				Already existing data using the same <tt>name</tt> is overwritten.
				@param	name the name to be used for the new property
		*/
		void setProperty(const std::string& name);

		/**	Set a named property containing boolean data.
				This method creates a new named property containing boolean
				data (<tt>NamedProperty::Type == BOOL</tt>).
				Already existing data using the same <tt>name</tt> is overwritten.
				@param	name the name to be used for the new property
				@param	value the boolean value 
		*/
		void setProperty(const std::string& name, bool value);

		/**	Set a named property containing a signed integer number.
				This method creates a new named property containing an 
				int (<tt>NamedProperty::Type == INT</tt>).
				Already existing data using the same <tt>name</tt> is overwritten.
				@param	name the name to be used for the new property
				@param	value the data 
		*/
		void setProperty(const std::string& name, int value);

		/**	Set a named property containing an unsigned integer number.
				This method creates a new named property containing an 
				unsigned int (<tt>NamedProperty::Type == UNSIGNED_INT</tt>).
				Already existing data using the same <tt>name</tt> is overwritten.
				@param	name the name to be used for the new property
				@param	value the data 
		*/
		void setProperty(const std::string& name, unsigned int value);

		/**	Set a named property containing a floating point number.
				This method creates a new named property containing a
				floating point number (<tt>NamedProperty::Type == FLOAT</tt>).
				Already existing data using the same <tt>name</tt> is overwritten.
				@param	name the name to be used for the new property
				@param	value the data 
		*/
		void setProperty(const std::string& name, float value);

		/**	Set a named property containing a double-precision floating point number.
				This method creates a new named property containing a
				floating point number (<tt>NamedProperty::Type == DOUBLE</tt>).
				Already existing data using the same <tt>name</tt> is overwritten.
				@param	name the name to be used for the new property
				@param	value the data 
		*/
		void setProperty(const std::string& name, double value);

		/**	Set a named property containing a string.
				This method creates a new named property containing a
				string (<tt>NamedProperty::Type == STRING</tt>).
				Already existing data using the same <tt>name</tt> is overwritten.
				@param	name the name to be used for the new property
				@param	value the data 
		*/
		void setProperty(const std::string& name, const std::string& value);

		/**	Set a named property containing a PersistentObject.
				This method creates a new named property containing a
				string (<tt>NamedProperty::Type == OBJECT</tt>).
				Already existing data using the same <tt>name</tt> is overwritten.
				@param	name the name to be used for the new property
				@param	value the data 
		*/
		void setProperty(const std::string& name, const PersistentObject& value);

		/**	Retrieve a named property.
				If the property manager contains a property named <tt>name</tt>
				this property is returned. An empty property is returned
				otherwise.
				@param	name the name of the proeprty to be retrieved
		*/
		const NamedProperty& getProperty(const std::string& name) const;

		/** Return an iterator to the first NamedProperty stored in this container. */
		NamedPropertyIterator beginNamedProperty();

		/** Return an iterator past the last NamedProperty stored in this container. */
		NamedPropertyIterator endNamedProperty();

		/**	Remove a named property.
				If the named property <tt>name</tt> does exist, it is remove from
				the array of properties.
				@param name the name of the property to be removed
		*/
		void clearProperty(const std::string& name);

		/**	Return the number of named properties.
				@return Size the number of named properties stored in the ProprtyManager object
		*/
		Size countNamedProperties() const;
		//@}

		/**	@name	Predicates 
		*/
		//@{
		/// Query for an unnamed property
		bool hasProperty(Property property) const;

		/// Query for a named property
		bool hasProperty(const std::string& name) const;

		/** Equality operator
				Two instances are equal if they have the same named and unnamed properties.
		*/
		bool operator == (const PropertyManager& pm) const;

		/// Inequality operator
		bool operator != (const PropertyManager& pm) const;

		//@}


		/**	@name Storable Interface
		*/
		//@{
		///	Persistent stream writing.
		void write(PersistenceManager& pm) const;

		///	Persistent stream reading.
		bool read(PersistenceManager& pm);
		//@}

		/**	@name	Debugging and Diagnostics 
		 */
		//@{

		/**	Test if instance is valid.
				Returns true if the bitvector is valid.
				@return bool <b>true</b>
		*/
		bool isValid() const;
	
		/** Internal state dump.
				Dump the current internal state of {\em *this} to 
				the output ostream <b>  s </b> with dumping depth <b>  depth </b>.
				@param   s - output stream where to output the internal state of {\em *this}
				@param   depth - the dumping depth
		*/
		void dump(std::ostream& s = std::cout, Size depth = 0) const;
		
		//@}

		private:

		BitVector bitvector_;
		std::vector<NamedProperty> named_properties_;
	};

	//@}

#	ifndef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/CONCEPT/property.iC>
#	endif

} // namespace BALL

#endif // BALL_CONCEPT_PROPERTY_H
