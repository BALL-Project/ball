// $Id: property.h,v 1.13 2000/08/28 15:53:10 oliver Exp $

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
/*
#define BALL_PROPERTY_DEFAULT_VALUE                1
#define BALL_PROPERTY_DEFAULT_INVALID_NAME_VALUE   0
*/
namespace BALL 
{

	/**	@name	Properties
	*/
	//@{
	
	/** Named Property Class
	*/
	class NamedProperty
		:	public PersistentObject
	{
		public:

		BALL_CREATE_NODEEP(NamedProperty)

		/**	@name Enums
		*/
		//@{
		/**	The different types of data a NamedProperty may contain.
				
		*/
		enum Type
		{
			/**	@doc Bool-type properties can have the value {\bf true} or {\bf false}.
			*/
			BOOL,

			/** @doc Int-type properties contain a variable of type {\bf int}
			*/
			INT,

			/** @doc Unsigned-int-type properties contain a variable of type {\bf unsigned int}
			*/
			UNSIGNED_INT,

			/**	@doc Float-type properties contain a variable of type {\bf double}
			*/
			FLOAT,

			/**	@doc Double-type properties contain a variable of type {\bf double}
			*/
			DOUBLE,

			/** @doc String-type properties contain a pointer to a string.
					When destructing the property, the string is destructed, too. ???
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
			NONE
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
				The {\tt Type} is set to \Ref{OBJECT}.
				@param	name the propertie's name
		*/
		NamedProperty(const string& name);

		/**	Constructor for bool-type properties.
				Creates a NamedProperty object containing a boolean value.
				The {\tt Type} is set to \Ref{BOOL}.
				@param	name the property's name
				@param	value the boolean value stored in the property
		*/	
		NamedProperty(const string& name, bool value);

		/**	Constructor for int-type properties.
				Creates a NamedProperty object containing a signed int value.
				The {\tt Type} is set to \Ref{INT}.
				@param	name the property's name
				@param	value the int value stored in the property
		*/
		NamedProperty(const string& name, int value);

		/** Constructor for unsigned int-type properties.
				Creates a NamedProperty object containing an unsigned int value.
				The {\tt Type} is set to \Ref{UNSIGNED_INT}.
				@param	name the property's name
				@param	value the int value stored in the property
		*/
		NamedProperty(const string& name, unsigned int value);

		/** Constructor for float-type properties.
				Creates a NamedProperty object containing a float value.
				The {\tt Type} is set to \Ref{FLOAT}.
				@param	name the property's name
				@param	value the float value stored in the property
		*/
		NamedProperty(const string& name, float value);

		/** Constructor for double-type properties.
				Creates a NamedProperty object containing a double value.
				The {\tt Type} is set to \Ref{DOUBLE}.
				@param	name the property's name
				@param	value the double value stored in the property
		*/
		NamedProperty(const string& name, double value);

		/** Constructor for string-type properties.
				Objects of type STRING contain a pointer to a string
				object. This object is constructed as a copy of {\tt str}
				The {\tt Type} is set to \Ref{STRING}.
				@param	name the property's name
				@param	str the string stored in the property
		*/
		NamedProperty(const string& name, string& str);

		/**	Constructor for object-type properties.
				Creates a NamedProperty object containing a 
				pointer to a persistent object.
				The {\tt Type} is set to \Ref{OBJECT}.
				@param	name the property's name
				@param	po a reference to the persistent object stored in the property
		*/
		NamedProperty(const string& name, PersistentObject& po);

		/**	Copy constructor
		*/
		NamedProperty(const NamedProperty&);

		/**	Destructor .
				The destructor destructs the contained data in the case of STRING-type 
				properties only.
		*/
		~NamedProperty();
		//@}	

		/**	@name Persistence
		*/
		//@{
			
		/**	Write a persistent copy of the object.
		*/
    virtual void persistentWrite(PersistenceManager& pm, const char* name = "") const;

		/**	Retrieve a persistent copy of the object
		*/
    virtual void persistentRead(PersistenceManager& pm);
		//@}
 
		
		/**	@name	 Accessors 
		*/
		//@{

		/// Return the type of the data stored in the property object
		Type getType() const;
		
		/// Return the name of the property
		string getName() const;

		/** Return the data of the property object as bool.
				If the property object is not of BOOL type, {\bf false}
				is returned.
		*/
		bool getBool() const;

		/** Return the data of the property object as int.
				If the property object is not of INT type, {\bf 0}
				is returned.
		*/
		int getInt() const;
			
		/** Return the data of the property object as float.
				If the property object is not of FLOAT type, {\bf 0.0}
				is returned.
		*/
		float getFloat() const;
			
		/** Return the data of the property object as double.
				If the property object is not of DOUBLE type, {\bf 0.0}
				is returned.
		*/
		double getDouble() const;
			
		/** Return the data of the property object as unsigned int.
				If the property object is not of UNSIGNED_INT type, {\bf 0}
				is returned.
		*/
		unsigned int getUnsignedInt() const;

		/** Return the data of the property object as a pointer to a PersistentObject.
				If the property object is not of OBJECT type, {\bf 0}
				is returned.
		*/
		PersistentObject* getObject() const;

		/** Return the data of the property object as a string.
				If the property object is not of STRING type, {\bf ""}
				is returned.
		*/
		string getString() const;
		//@}

		/**	@name	Storers */
		//@{

		/// Output operator
		friend std::ostream& operator << (std::ostream& s, const NamedProperty& property);

		/// Input operator
		friend std::istream& operator >> (std::istream& s, NamedProperty& property);
		//@}

		private:

		/**	The kind of information contained in the NamedProperty
		*/
		Type type_;

		/**	The name of the property object 
		*/
		string name_;
		
		/*_	The data
		*/
		union 
		{
			bool							b;
			int								i;
			unsigned int			ui;
			float							f;
			double						d;
			string*						s;
			PersistentObject*	object;
		} data_;

	};

	/**	Property Manager class.
			This class manages properties of a class.
			A property can be seen as easy way to extend the
			class' attributes without changing its interface.
			There are different types of properties defined depending 
			on the kind of data stored. The simplest kind of property 
			is just stored as a boolean information: Either an object {\em has}
			a certain property, or it does not. These properties are obviously coded 
			as a BitVector. However the meaning of the different bits has to be defined
			and the uniqueness of this meaning has to be guaranteed. In order to avoid
			this, all kernel classes derived from PropertyManager contain a public enum
			that defines a value NUMBER_OF_PROPERTIES. All user defined properties should	
			refer to this constant (e.g. by defining {\tt MY_PROPERTY = Molecule::NUMBER_OF_PROPERTIES + 1}).\\
			The second way of storing properties is much more flexible, but less efficient. 
			It uses the class NamedProperty that also allows to store additional information 
			along with the property. In this case, each property is accessed via a 
			unique name.\\
			
			{\bf Interface:} Storable
			\\
			{\bf Definition:} \URL{BALL/CONCEPT/property.h}
			\\
	*/
	class PropertyManager
	{
		public:

		/**	@name	Constructors and Destructors */
		//@{

		/// Default constructor
		PropertyManager();

		/// Copy constructor
		PropertyManager(const PropertyManager& property_manager, bool deep = true);

		/// Destructor
		virtual ~PropertyManager();

		/// Clears all properties
		void clear();

		/// Clears all properties
		void destroy();
		//@}

		/**	@name	Assignment 
		*/
		//@{

		/** Assign properties from another property manager
				@param property_manager the PropertyManager object to copy from
				@param deep ignored (just for interface compatibility)
		*/
		void set(const PropertyManager& property_manager, bool deep = true);

		/** Assignment operator.
				This operator calls {\tt set(property_manager)} to assign
				a new object.
				@param property_manager the PropertyManager object to copy from
				@return PropertyManager {\tt *this}
		*/
		PropertyManager& operator = (const PropertyManager& property_manager);

		/** Assign properties to another property manager.
				This method copies the contents of the PropertyManager object
				to the given {\tt property_manager}.
				@param	property_manager the target object
				@param	deep ignored (just for interface compatibility)
		*/
		void get(PropertyManager& property_manager, bool deep = true) const;

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
				@return BitVector\& a mutable reference to the (private) BitVector containing 
					the unnamed properties
		*/
		BitVector& getBitVector();

		/** Return a const reference to the bit vector containing the unnamed properties.
				This method returns the bit vector containing the unnamed properties
				of the property manager.
				@return BitVector\& a const reference to the (private) BitVector containing 
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
				@return BitVector\& a reference to the BitVector containing the
					unnamed properties of the property manager
		*/
		operator BitVector& ();
		//@}


		/**	@name	Handling unnamed bit properties 
		*/
		//@{

		/** Set a property.
				This method sets an unnamed property of a PropertyManager object by
				calling {\tt BitVector::setBit(property)} for the private bit vector.
				@param property the number of the property to be set
		*/
		void setProperty(Property property);

		/** Clear a property.
				This method clears an unnamed property of a PropertyManager object by
				calling {\tt BitVector::clearBit(property)} for the private bit vector.
				@param property the number of the property to be cleared
		*/
		void clearProperty(Property property);

		/** Toggle (invert) a property.
				This method clears an unnamed property of a PropertyManager object by
				calling {\tt BitVector::toggleBit(property)} for the private bit vector.
				@param property the number of the property to be toggled
		*/
		void toggleProperty(Property property);

		/** Count all properties.
				This method returns the sum of unnamed and named properties.
				It counts the number of {\em ones} in the bit vector
				and the number of properties stored in the named property vector.
		*/
		Size countProperties() const;
		//@}
	
		/**	@name	Handling named properties 
				PropertyManager contains an vector of \Ref{NamedProperty} objects.
				Each of the NamedProeprty objects has to have a {\em unique} name.
				The setProperty methods ensure the uniqueness of this name.
				If a setProperty method is called for an existing name, the
				existing object is destructed prior to the insertion of the new object.
		*/
		//@{
		/**	Set a named property.
				This method adds a new named property to the property manager's 
				array of named properties or substitutes an already existing
				named property by {\tt property}. If a property with the same name as 
				{\tt property} already exists, it is destructed.
				@param	property the new property to be set
		*/
		void setProperty(const NamedProperty& property);

		/**	Set a named property without any data.
				This method creates a new named property not containing any data
				({\tt NamedProperty::Type == NONE}).
				Already existing data using the same {\tt name} is overwritten.
				@param	name the name to be used for the new property
		*/
		void setProperty(const string& name);

		/**	Set a named property containing boolean data.
				This method creates a new named property containing boolean
				data ({\tt NamedProperty::Type == BOOL}).
				Already existing data using the same {\tt name} is overwritten.
				@param	name the name to be used for the new property
				@param	value the boolean value 
		*/
		void setProperty(const string& name, bool value);

		/**	Set a named property containing a signed integer number.
				This method creates a new named property containing an 
				int ({\tt NamedProperty::Type == INT}).
				Already existing data using the same {\tt name} is overwritten.
				@param	name the name to be used for the new property
				@param	value the data 
		*/
		void setProperty(const string& name, int value);

		/**	Set a named property containing an unsigned integer number.
				This method creates a new named property containing an 
				unsigned int ({\tt NamedProperty::Type == UNSIGNED_INT}).
				Already existing data using the same {\tt name} is overwritten.
				@param	name the name to be used for the new property
				@param	value the data 
		*/
		void setProperty(const string& name, unsigned int value);

		/**	Set a named property containing a floating point number.
				This method creates a new named property containing a
				floating point number ({\tt NamedProperty::Type == FLOAT}).
				Already existing data using the same {\tt name} is overwritten.
				@param	name the name to be used for the new property
				@param	value the data 
		*/
		void setProperty(const string& name, float value);

		/**	Set a named property containing a double-precision floating point number.
				This method creates a new named property containing a
				floating point number ({\tt NamedProperty::Type == DOUBLE}).
				Already existing data using the same {\tt name} is overwritten.
				@param	name the name to be used for the new property
				@param	value the data 
		*/
		void setProperty(const string& name, double value);

		/**	Set a named property containing a string.
				This method creates a new named property containing a
				string ({\tt NamedProperty::Type == STRING}).
				Already existing data using the same {\tt name} is overwritten.
				@param	name the name to be used for the new property
				@param	value the data 
		*/
		void setProperty(const string& name, const string& value);

		/**	Set a named property containing a PersistentObject.
				This method creates a new named property containing a
				string ({\tt NamedProperty::Type == OBJECT}).
				Already existing data using the same {\tt name} is overwritten.
				@param	name the name to be used for the new property
				@param	value the data 
		*/
		void setProperty(const string& name, const PersistentObject& value);

		/**	Retrieve a named property.
				If the property manager contains a property named {\tt name}
				this property is returned. An empty property is returned
				otherwise.
				@param	name the name of the proeprty to be retrieved
		*/
		const NamedProperty& getProperty(const string& name) const;

		/**	Remove a named property.
				If the named property {\tt name} does exist, it is remove from
				the array of properties.
				@param name the name of the property to be removed
		*/
		void clearProperty(const string& name);

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
		bool hasProperty(const string& name) const;

		//@}

		/**	@name	Storers */
		//@{

		/// Output operator
		friend std::ostream& operator << (std::ostream& s, const PropertyManager& property_manager);

		/// Input operator
		friend std::istream& operator >> (std::istream& s, PropertyManager& property_manager);
		//@}


		/**	@name Storable Interface
		*/
		//@{
		
		/**	Persistent stream writing.
		*/
		void write(PersistenceManager& pm) const;

		/**	Persistent stream reading.
		*/
		bool read(PersistenceManager& pm);

		//@}


		/**	@name	Debugging and Diagnostics */
		//@{

		/**	Test if instance is valid.
				Returns true if the bitvector is valid.
				@return bool {\bf true}
		*/
		bool isValid() const;
	
		/** Internal state dump.
				Dump the current internal state of {\em *this} to 
				the output ostream {\em s} with dumping depth {\em depth}.
				@param   s - output stream where to output the internal state of {\em *this}
				@param   depth - the dumping depth
		*/
		void dump(std::ostream& s = std::cout, Size depth = 0) const;
		//@}



		private:

		BitVector							bitvector_;

		vector<NamedProperty>	named_properties_;
	};

	//@}

#	ifndef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/CONCEPT/property.iC>
#	endif

} // namespace BALL

#endif // BALL_CONCEPT_PROPERTY_H
