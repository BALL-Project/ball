// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_KERNEL_BOND_H
#define BALL_KERNEL_BOND_H

#ifndef BALL_CONCEPT_PROPERTY_H
#	include <BALL/CONCEPT/property.h>
#endif

#ifndef BALL_CONCEPT_COMPOSITE_H
#	include <BALL/CONCEPT/composite.h>
#endif

#ifndef BALL_CONCEPT_ATOMCONTAINER_H
#	include <BALL/KERNEL/atomContainer.h>
#endif

#define BALL_BOND_DEFAULT_FIRST_ATOM    0
#define BALL_BOND_DEFAULT_SECOND_ATOM   0
#define BALL_BOND_DEFAULT_NAME          ""
#define BALL_BOND_DEFAULT_ORDER         ORDER__UNKNOWN
#define BALL_BOND_DEFAULT_TYPE          TYPE__UNKNOWN


namespace BALL
{
	class Atom;
	class Fragment;
	class System;

	/** Bond class.
			An instance of Bond represents a bond between two atoms.
			There can be only one bond between two atoms (double, triple, etc. bonds are represented
			using just one bond and an appropriate value for the bond order attribute, see  \link setBondOrder setBondOrder \endlink ).
			 \par
			The "first" and "second" atom are assigned in a unique way: the first atom is always
			the atom with the lower  \link Handle Handle \endlink  than the second atom.
			 \par
			The "state" of a bond is defined by its attributes:
			
				- "first atom" ( \link Bond::first_ Bond::first_ \endlink )
				- "second atom" ( \link Bond::second_ Bond::second_ \endlink )
				- "bond name" ( \link Bond::name_ Bond::name_ \endlink ): an arbitrary name
				- "bond order" ( \link Bond::bond_order_ Bond::bond_order_ \endlink ): the order, i.e. single, double, triple,...
				- "bond type" ( \link Bond::bond_type_ Bond::bond_type_ \endlink ): covalent, hydrogen bond, etc. to identify non-standard bond types
			
			Bond equality is defined as bond identity.
			A linear ordering of bonds is defined as the linear order of the  \link Object::Handle Object::Handle \endlink s.
			
    	\ingroup KernelMiscellaneous 
	*/
	class BALL_EXPORT Bond
		: public Composite,
			public PropertyManager
	{
		public:

		///
		friend class Atom;

		BALL_CREATE_DEEP(Bond)

		/**	@name	Exceptions
		*/
		//@{

		/**	Not bound to two atoms.
		 *	This exception may be thrown by  \link getLength getLength \endlink  if this instance is not bound.
		 */
		class BALL_EXPORT NotBound
			:	public Exception::GeneralException
		{
			public:
				NotBound(const char* file, int line);
		};
		//@}

		/** @name Type definitions 
		*/
		//@{

		/** Bond type.
		*/
		typedef short Order;
		typedef short Type;

		//@}  
		/** @name Enumerations 
		*/
		//@{

		/**	Predefined constants for the bond order 
		*/
		enum BondOrder
		{
			/// Default value.
			ORDER__UNKNOWN        = 0,
			/// Single bond.
			ORDER__SINGLE         = 1,
			/// Double bond.
			ORDER__DOUBLE         = 2,
			/// Triple Bond.
			ORDER__TRIPLE         = 3,
			/// Quadruple bond.
			ORDER__QUADRUPLE      = 4,
			/// Aromatic bonds.
			ORDER__AROMATIC       = 5,
			/// Any bond orders (useful for predicates).
			ORDER__ANY            = 6,
			/// The number of bond orders.
			NUMBER_OF_BOND_ORDERS
		};

		/**	Predefined constants for the bond type.
				Those are just defined for convenience but they are not
				used right now.
		*/
		enum BondType
		{
			/// 
			TYPE__UNKNOWN           = 0,
			///
			TYPE__COVALENT          = 1,
			///
			TYPE__HYDROGEN          = 2,
			///
			TYPE__DISULPHIDE_BRIDGE = 3,
			///
			TYPE__SALT_BRIDGE       = 4,
			///
			TYPE__PEPTIDE           = 5,
			///
			NUMBER_OF_BOND_TYPES
		};

		/** Predefined properties.
				Enumeration of all properties that are used by Bond.
		*/
		enum Property
		{
			/// This property can be used to store aromaticity information without overwriting bond orders.
			IS_AROMATIC = 0,
			///
			NUMBER_OF_PROPERTIES
		};
		//@}  

		/** @name Constructors and Destructors 
		*/
		//@{

		/** Default constructor.
		 *	The state of this bond is:
		 *		- bond has no first atom (=0)
		 *		- bond has no second atom (=0)
		 *		- bond name is the empty string (="")
		 *		- bond order is unknown (= \link Bond::ORDER__UNKNOWN Bond::ORDER__UNKNOWN \endlink )
		 *		- bond type is unknown (= \link Bond::TYPE__UNKNOWN Bond::TYPE__UNKNOWN \endlink )
		 */
		Bond();

		/** Copy constructor.
		 *	Calls  \link Bond::createBond Bond::createBond \endlink .
		 *	The state of this bond is initialized to the state of bond.
		 *	 \par
		 *	<b>Note:</b> Deep copying of bonds makes no sense, the parameter <tt>deep</tt> is therefore
		 *	ignored. The use of this method is not recommended because it may result in inconcistencies
		 *	of the whole the kernel data structure. This if for internal use only!
		 *	@param			 bond the bond to be copied
		 *	@param       deep ignored
		 *	@see         createBond
		 */
		Bond(const Bond& bond, bool deep = true);

		/** Detailed constructor.
		 *	Calls  \link createBond createBond \endlink  to create a new bond between the two atoms
		 *	@param       name name of the constructed bond
		 *	@param       first first atom of the constructed bond
		 *	@param       second second atom of the constructed bond
		 *	@param       order order of the constructed bond
		 *	@param       type type of the constructed bond
		 *	@exception TooManyBonds if one of the atom already possesses  \link Atom::MAX_NUMBER_OF_BONDS Atom::MAX_NUMBER_OF_BONDS \endlink  bonds.
		 */
		Bond(const String& name, Atom& first, Atom& second, Order order = BALL_BOND_DEFAULT_ORDER,
				 Type type = BALL_BOND_DEFAULT_TYPE);

		/** Create a bond.
		 *	Connect the two atoms <tt>first</tt> and <tt>second</tt> via a bond.
		 *	@param 	bond the instantiated bond that connects the first atom <tt>first</tt> 
		 *					to the second atom <tt>second</tt>
		 *	@param 	first the first atom of the bond
		 *	@param 	second the second atom of the bond
		 *	@return Bond* <tt>this</tt>
		 *	@exception TooManyBonds if one of the atom already possesses  \link Atom::MAX_NUMBER_OF_BONDS Atom::MAX_NUMBER_OF_BONDS \endlink  bonds.
		 */
		static Bond* createBond(Bond& bond, Atom& first, Atom& second);

		/** Destructor.
		 *	If the bond is connecting two atoms, they are disconnected.
		 */
		virtual ~Bond();

		/** Disconnect and reset to default state.
		 *	Reset the bond attributes to their default values.
		 *	The state of this bond is:
		 *		- bond has no connectivity with first atom (=0)
		 *		- bond has no connectivity with second atom (=0)
		 *		- bond name is the empty string (="")
		 *		- bond order is unknown (= \link Bond::ORDER__UNKNOWN Bond::ORDER__UNKNOWN \endlink )
		 *		- bond type is unknown (= \link Bond::TYPE__UNKNOWN Bond::TYPE__UNKNOWN \endlink )
		 */
		virtual void clear();

		/** Explicit destructor.
		 *	Destroy this bond explicitly without releasing its heap memory thus
		 *	this bond may exist further.	Calls  \link Bond::clear Bond::clear \endlink .
		 *	Set the state of this bond to the default values. 
		 *	<b>Note:</b> Destroy is equivalent to  \link Bond::clear Bond::clear \endlink .
		 *	@see Bond::clear
		 */
		virtual void destroy();
		//@}

		/**	@name Persistence 
		*/
		//@{

		/**	Write a bond to a persistent stream.
		 *	@param pm the persistence manager
		 */
		void persistentWrite(PersistenceManager& pm, const char* name = 0) const;

		/**	Read a bond from a persistent stream.
		 *	@param pm the persistence manager
		 */
		void persistentRead(PersistenceManager& pm);

		/**	Finalize the deserialization.
		 *	Bond might have to swap <tt>first_</tt> and <tt>second_</tt> to ensure
		 *	the correct order (see  \link Bond Bond \endlink ).
		 */
		void finalize();
		//@}

		/**	@name Predicates
		*/
		//@{
		///	Equality operator. Two bonds are equal, if they are identical instances.
		bool operator == (const Bond& bond) const;

		///	Inequality operator. Two bonds are not equal, if they are different instances.
		bool operator != (const Bond& bond) const;
		//@}

		/** @name Assignment methods 
		*/
		//@{

		/** Assignment operator.
				Assign a deep copy (on the level of  \link Composite Composite \endlink ).
				
				<b>Note:</b> The use of this method is not recommended because it may result in inconcistencies
				of the whole system. This is for internal use only.
				@param  bond the bond to be copied
				@return Bond - this bond
				@see    Bond::set
		*/
		Bond& operator = (const Bond& bond);

		/** Swap the contents of two bonds
				@param bond the bond <tt>this</tt> is being swapped with
				@see   Bond::Bond
		*/
		void swap(Bond& bond);
		//@}

		/** @name Accessors
		*/
		//@{ 

		/** Set the first atom.
				This method does not ensure the correct order of atoms 
				(see  \link Bond Bond \endlink ), so its use is recommended for internal purposes only.
		*/
		void setFirstAtom(Atom* atom);
		/** Set the second atom.
				This method does not ensure the correct order of atoms 
				(see  \link Bond Bond \endlink ), so its use is recommended for internal purposes only.
		*/
		void setSecondAtom(Atom* atom);

		/// Return a pointer to the first atom
		const Atom* getFirstAtom() const;
		/// Return a pointer to the first atom
		Atom* getFirstAtom();

		/// Return a pointer to the second atom.
		const Atom* getSecondAtom() const;
		/// Return a pointer to the second atom.
		Atom* getSecondAtom();

		/**	Return the partner atom of an atom.
				If the given <tt>atom</tt> is part of this bond, the other atom
				of the bond is returned. 0 is returned if <tt>atom</tt> is not
				one of the bond's atoms.
				@param	atom one of the bond's atoms
				@return	the atom's bond partner
		*/
		Atom* getPartner(const Atom& atom) const;

		/// Assign the atom name
		void setName(const String& name);

		/// Return the atom name
		const String& getName() const;

		/// Assign the bond order
		void setOrder(Order bond_order);

		/// Return the bond order
		Order getOrder() const;

		/** Returns true if this bond is aromatic.
		 *
		 * 	A bond is defined as aromatic, if it has the property Bond::IS_AROMATIC or if
		 * 	it has the order Bond::ORDER__AROMATIC
		 */
		bool isAromatic() const;

		/// Set the bond type
		void setType(Type bond_type);

		/// Return the bond type
		Type getType() const;

		/** Return the bond length
		 *	@return    float - the distance between the two atoms
		 *	@exception NotBound if the bond is not bound to two atoms
		 */
		float getLength() const;

		/** Return the partner of the atom in this bond.
				@param			 atom an atom
				@return      Atom a constant pointer to the atom,
										 0 if the atom is not part of the bond
		*/
		const Atom* getBoundAtom(const Atom& atom) const;

		/** Return the partner of the atom in this bond.
				@param			 atom an atom
				@return      Atom a pointer to the atom,
										 0 if the atom is not part of the bond
		*/
		Atom* getBoundAtom(const Atom& atom);
		//@}

		/** @name Predicates 
		*/
		//@{ 

		/** Determine whether the bond belongs to an atom.
				Calls  \link Atom::hasBond Atom::hasBond \endlink .
				@param atom the atom that is queried to connect this bond to another atom
				@return  bool - <tt>true</tt> if bond connects the atom <b>  atom </b> with another atom, 
												<tt>false</tt> otherwise
				@see     Atom::hasBond
		*/
		bool isBondOf(const Atom& atom) const;

		/** Determine whether this bond contains any atom.
				@return   bool - <tt>true</tt> if bond connects the atom <b>  atom </b> with another atom, 
												 <tt>false</tt> otherwise
				@see      Atom::hasBond
		*/
		bool isBound() const;

		/** Determine whether the bond connects two fragments.
				If both atoms have no roots, the result is false.
				Calls  \link Composite::getRoot Composite::getRoot \endlink .
				@return   bool - <tt>true</tt> if this bond is intermolecular, 
												 <tt>false</tt> otherwise
				@see      Composite::getRoot
		*/
		bool isInterBond() const;

		/** Request for the intermolecular bonding of this bond within {\em atom_container}.
				Query, if this bond connects a atom within {\em atom_container} instance with an atom outside.
				Calls  \link Composite::isDescendantOf Composite::isDescendantOf \endlink .
				@param  	atom_container the queried parent fragment.
				@return  	bool -	<tt>true</tt> if this bond is intermolecular, 
													<tt>false</tt> otherwise
				@see      Composite::isDescendantOf
		*/
		bool isInterBondOf(const AtomContainer& atom_container) const;

		/**	Request for the intramolecular bonding of this bond.
				Query, if this bond connects its two atoms within a common parent  \link Composite Composite \endlink  instance.
				If both atoms have no roots, the result is true.
				Calls  \link Composite::getRoot Composite::getRoot \endlink .
				@return bool - <tt>true</tt> if this bond is intramolecular, 
											 <tt>false</tt> otherwise
				@see    Composite::getRoot
		*/
		bool isIntraBond() const;

		/** Request for the intramolecular bonding of this bond within {\em atom_container}.
				Query, if this bond connects its two atoms within the common parent {\em atom_container} instance.
				Calls  \link Composite::isDescendantOf Composite::isDescendantOf \endlink .
				@param  atom_container the queried parent fragment.
				@return bool - <tt>true</tt> if this bond is intramolecular, 
											 <tt>false</tt> otherwise
				@see    Composite::isDescendantOf
		*/
		bool isIntraBondOf(const AtomContainer& atom_container) const;

		//@}

		/** @name Debugging and diagnostics 
		*/
		//@{ 

		/** Internal state and consistency self-validation.
				@return   bool - <tt>true</tt> if the internal state of this bond is correct (self-validated)
									and consistent, <tt>false</tt> otherwise
		*/
		virtual bool isValid() const;

		/** Internal state dump.
				Dump the current internal state of this bond to the output ostream <b>  s </b>
				with dumping depth <b>  depth </b>.
				@param	s output stream where to output the internal state of this bond
				@param  depth the dumping depth
		*/
		virtual void dump(std::ostream& s = std::cout, Size depth = 0) const;
		//@}

		protected:

		/**	@name Atributes
		*/
		//@{

		// First atom of the bond (bond owner)
		Atom*		first_;

		// Second atom
		Atom*		second_;

		// Bond name
		String	name_;

		// Bond order
		Order		bond_order_;

		// Bond type
		Type		bond_type_;
		//@}

		private:

		void arrangeBonds_();
		void clear_();
	};

# ifndef BALL_NO_INLINE_FUNCTIONS
#   include <BALL/KERNEL/bond.iC>
# endif
} // namespace BALL

#endif // BALL_KERNEL_BOND_H

