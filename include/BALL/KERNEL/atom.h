// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: atom.h,v 1.61 2003/12/01 07:35:10 oliver Exp $
//

#ifndef BALL_KERNEL_ATOM_H
#define BALL_KERNEL_ATOM_H

#ifndef BALL_CONCEPT_COMPOSITE_H
#	include <BALL/CONCEPT/composite.h>
#endif

#ifndef BALL_CONCEPT_PROPERTY_H
#	include <BALL/CONCEPT/property.h>
#endif

#ifndef BALL_CONCEPT_RANDOMACCESSITERATOR_H
#	include <BALL/CONCEPT/randomAccessIterator.h>
#endif

#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector3.h>
#endif

// Defines for default values for an atom
#define BALL_ATOM_DEFAULT_ELEMENT &Element::UNKNOWN
#define BALL_ATOM_DEFAULT_CHARGE     0
#define BALL_ATOM_DEFAULT_NAME       ""
#define BALL_ATOM_DEFAULT_TYPE_NAME  "?"
#define BALL_ATOM_DEFAULT_POSITION   0,0,0
#define BALL_ATOM_DEFAULT_RADIUS     0
#define BALL_ATOM_DEFAULT_TYPE       Atom::UNKNOWN_TYPE
#define BALL_ATOM_DEFAULT_VELOCITY   0,0,0
#define BALL_ATOM_DEFAULT_FORCE   	 0,0,0

namespace BALL 
{
	class Bond;
	class Element;
	class Fragment;
	class Residue;
	class Molecule;

	/** Atom class.
			A class representing atoms.
			During each runtime instance of a program an atom is unique and
			identified by a  \link Object::Handle Object::Handle \endlink . Atom equality is defined as
			atom identity, so there cannot be any two identical atoms. A linear
			ordering of atoms is defined as the linear order of the
			 \link Object::Handle Object::Handle \endlink s.
			 \par
			Two atoms can be connected via a  \link Bond Bond \endlink . There can be only one
			bond between any two atoms (double bonds etc. are expressed via the
			bond order attribute of the bond) and the total number of bonds of an
			atom is limited to eight (can be changed at compile time, see
			 \link MAX_NUMBER_OF_BONDS MAX_NUMBER_OF_BONDS \endlink ).
			 \par
			Since  \link Atom Atom \endlink  is derived from  \link ProperyManager ProperyManager \endlink , it may contain
			arbitrary, user-defined properties.
			An atom may be inserted in a  \link Fragment Fragment \endlink  instance ("parent fragment").
			The "state" of an atom is defined by its attributes:

				- "element" - an instance of  \link Element Element \endlink 
				- "charge" - the charge in multiples of the the proton charge
				- "name" - a string identifier
				- "type name" - a string identifier, meaningful in the the
							context of a forcefield only
				- "position" - the absolute position in cartesian coordinates
							(Angstrom)
				- "radius" - the radius (Angstrom)
				- "type" - an integer type, meaningful only in the context of a
							forcefield 
				- "velocity" - velocity the velocity of the atom (Angstrom/ps)
				- "force" - the force experienced by the atom (for forcefield
							calculations, in units of Newton)
				- "bonds" - up to  \link MAX_NUMBER_OF_BONDS MAX_NUMBER_OF_BONDS \endlink  bonds to other atoms
			
			
			 \par
			@see Bond
			@see Molecule
			@see AtomContainer
    
			\ingroup KernelContainers 
	*/
	class Atom
		: public Composite,
			public PropertyManager
	{
		public:

			/** @name Class friends
			
						- class Bond
					
			*/
			friend class Bond;

			BALL_CREATE_DEEP(Atom)

			/** Atom type.
			*/
			typedef short Type;
	
			/** @name Enumerations 
			*/
			//@{
		
			/** Anonymous enumeration.
					Unnamed enumeration of all non-categorized constants.
			*/
			enum 
			{
				/** Unknown atom type.
						The type assigned for default-constructed atoms.
				*/
				UNKNOWN_TYPE = -1,

				/** Any atom type.
						Used as a wild card in the context of forcefields mainly
				*/
				ANY_TYPE = 0,

				/// Maximum number of bonds of an atom
				MAX_NUMBER_OF_BONDS = 12
			};

			/** Predefined properties.
					Enumeration of all properties that are used by the BALL kernel.
			*/
			enum Property
			{
				NUMBER_OF_PROPERTIES = 0
			};
				
			/**	The type of name used for getFullName.
					@see getFullName
			*/
			enum FullNameType 
			{
				// Do not add extensions
				NO_VARIANT_EXTENSIONS,
				// Add the residue extensions
				ADD_VARIANT_EXTENSIONS,
				// Add the residue ID
				ADD_RESIDUE_ID,
				// Add the residue ID and the residue extension
				ADD_VARIANT_EXTENSIONS_AND_ID
			};

			//@}  

			/** @name Constructors 
			*/
			//@{

			/** Default constructor.
					The state of this instance is:

						- element type is unknown (Element::UNKNOWN)
						- charge is 0
						- name is empty string
						- type name is "?"
						- position is  \link Vector3 Vector3 \endlink (0,0,0)
						- radius is 0
						- type  \link INVALID_TYPE INVALID_TYPE \endlink 
						- velocity is  \link Vector3 Vector3 \endlink (0,0,0)
						- force is  \link Vector3 Vector3 \endlink (0,0,0)
						- bond table is empty (atom has no bonds)
					
					@return  Atom - new atom
					@see     Composite::Composite
					@see     PropertyManager::PropertyManager
			*/
			Atom()
				throw();
		
			/** Copy constructor.
					The copy is either deep or shallow (default).
					 \par
					<b>Note:</b> Deep copying of atoms does not include bond cloning.
					@param   atom the atom to be copied (cloned)
					@param   deep make a deep (=<tt>true</tt>) or shallow (=<tt>false</tt>) copy of {\em atom}
					@return  Atom - new constructed atom cloned from {\em atom}
					@see     Composite::Composite
					@see     PropertyManager::PropertyManager
			*/
			Atom(const Atom& atom, bool deep = true)
				throw();
		
			/** Detailed state initializing constructor.
					The item bond table is empty (atom has no bonds).
					@param   element element type of the constructed atom
					@param   name name of the constructed atom
					@param   type_name type name name of the constructed atom
					@param   atom_type type of the constructed atom
					@param   position position of the constructed atom
					@param   velocity velocity of the constructed atom
					@param   force force acting upon the constructed atom
					@param   charge charge of the constructed atom
					@param   radius radius of the constructed atom
					@return  Atom - new constructed atom
					@see     Composite::Composite
					@see     PropertyManager::PropertyManager
			*/
			Atom(Element& element,
					 const String& name, const String& type_name = BALL_ATOM_DEFAULT_TYPE_NAME,
					 Type atom_type = BALL_ATOM_DEFAULT_TYPE,
					 const Vector3& position = Vector3(BALL_ATOM_DEFAULT_POSITION),
					 const Vector3& velocity = Vector3(BALL_ATOM_DEFAULT_VELOCITY),
					 const Vector3& force = Vector3(BALL_ATOM_DEFAULT_FORCE),
					 float charge = BALL_ATOM_DEFAULT_CHARGE,
					 float radius = BALL_ATOM_DEFAULT_RADIUS)	
				throw();

			//@}

			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					If the atom has bonds in common with an other atom that atom is
					disconnected and the associated  \link Bond Bond \endlink  instance is destroyed.
					Calls  \link Atom::destroy Atom::destroy \endlink .
					@see  Atom::destroy
			*/
			virtual ~Atom()
				throw();

			/** Explicit default initialization.
					Calls  \link Composite::clear Composite::clear \endlink  and resets the 
					attributes to the default values. In contrast
					to  \link destroy destroy \endlink , the atom is not removed from 
					any composite structure, i.e. its parent fragment
					pointer remains unchanged.
					@see	Composite::clear
					@see	destroy
			*/
			virtual void clear()
				throw();
		
			/** Explicit destructor.
					Destroy this instance explicitly and 
					reset its attributes to the default values. 
					@see  Composite::clear
			*/
			virtual void destroy()
				throw();

			//@}

			/**	@name	Persistence 
			*/
			//@{
	
			/**	Write an Atom to a persistent stream.
					@param pm the persistence manager
			*/
			virtual void persistentWrite(PersistenceManager& pm, const char* name = 0) const
				throw(Exception::GeneralException);

			/**	Read an Atom from a persistent stream.
					@param pm the persistence manager
			*/
			virtual void persistentRead(PersistenceManager& pm)
				throw(Exception::GeneralException);

			//@}

			/** @name Assignment methods 
			*/
			//@{

      /** Deep/shallow assignment.
          The assignment is either deep or shallow (default is deep).
					In the case of a deep assignment, all composites contained in
					<tt>atom</tt> are copied as well. 
					 \par
          <b>Caveat:</b> Bonds are not copied!
          @param   atom the atom to be copied
          @param   deep make a deep (=<b>true</b>) or shallow (=<b>false</b>) copy of <tt>atom</tt>
      */
      void set(const Atom& atom, bool deep = true)
        throw();

      /** Deep/shallow assignment.
					The inverse operation to  \link set set \endlink , behaves identically.
          @param  atom the atom to be assigned to
          @see    Atom::set
      */
      void get(Atom& atom, bool deep = true) const
        throw();

			/** Assignment operator.
					The assignment is always deep.	Calls  \link Atom::set Atom::set \endlink .
					 \par
					<b>Note:</b> Bonds are not copied
					@param   atom the atom to be copied
					@return  Atom& - this instance
					@see     Atom::set
			*/
			Atom& operator = (const Atom& atom)	throw();

			/** Swap the contents of two atoms.
					The static attributes are swapped by exchanging the indices of the two atoms!
					@param   atom the atom being swapped with this instance 
			*/
			void swap(Atom& atom)
				throw();

			//@}

			/**	Predicates
			*/
			//@{
			/**	Equality operator.
					Two atoms are equal if they have the same handle.
					@see Object::operator ==
			*/
			bool operator == (const Atom& atom) const
				throw();

			/**	Inequality operator
					@see operator ==
			*/
			bool operator != (const Atom& atom) const
				throw();
			//@}
				
			/** @name Accessors: inspectors and mutators 
			*/
			//@{ 

			/** Change the atom element type.
					@param   element the new element
			*/
			void setElement(const Element& element)
				throw();
				
			/** Constant inspection of the atom element type.
					@return  Element& - constant reference to the element type
			*/
			const Element& getElement() const
				throw();

			/** Set the charge of an atom.
					Charges should be assigned in multiples of the proton charge
					(elementary charge).
					@param   charge the new charge
			*/
			void setCharge(float charge)
				throw();

			/** Retrieve the charge of an atom
					Charges should be assigned in multiples of the proton charge
					(elementary charge).
					@return  float - copy of the charge
			*/
			float getCharge() const
				throw();

			/** Mutable inspection of the atom's parent molecule.
					A NULL pointer is returned if this atom is not part of a molecule.
					 \par
					Use  \link Molecule::insert Molecule::insert \endlink  to insert an atom into a molecule and
					 \link Molecule::remove Molecule::remove \endlink  to remove it.
					@return  Molecule* - mutable pointer to the parent molecule
			*/
			Molecule *getMolecule()
				throw();

			/** Constant inspection of the atom's parent molecule.
					A NULL pointer is returned if this atom is not part of a molecule.
					 \par
					Use  \link Molecule::insert Molecule::insert \endlink  to insert an atom into a molecule and
					 \link Molecule::remove Molecule::remove \endlink  to remove it.
					@return  Molecule* - constant pointer to the parent molecule
			*/
			const Molecule* getMolecule() const
				throw();

			/** Mutable inspection of the atom's parent fragment.
					A NULL pointer is returned if this atom is not part of a fragment.
					 \par
					Use  \link Fragment::insert Fragment::insert \endlink  to insert an atom into a fragment and
					 \link Fragment::remove Fragment::remove \endlink  to remove it.
					@return   Fragment* - mutable pointer to the parent fragment
			*/
			Fragment* getFragment()
				throw();

			/** Constant inspection of the atom's parent fragment.
					A NULL pointer is returned if this atom is not part of a fragment.
					 \par
					Use  \link Fragment::insert Fragment::insert \endlink  to insert an atom into a fragment and
					 \link Fragment::remove Fragment::remove \endlink  to remove it.					
					@return   Fragment* -	constant pointer to the parent fragment
			*/
			const Fragment* getFragment() const
				throw();

			/** Constant inspection of the atom's parent residue.
					A NULL pointer is returned if this atom is not part of a residue.
					 \par
					Use  \link Residue::insert Residue::insert \endlink  to insert an atom into a residue and
					 \link Residue::remove Residue::remove \endlink  to remove it.					
					@return   Residue* -	constant pointer to the parent residue
			*/
			const Residue* getResidue() const
				throw();

			/** Set the atom name.
					@param   name the new name
			*/
			void setName(const String& name)
				throw();

			/** Return the atom name
					@return  String& - constant reference to the name
			*/
			const String& getName() const
				throw();

			/** Assemble a fully specified atom name.
					This method returns at fully specified atom name as used for charge and 
					type assignments.	The name consists of the name of the residue the atom is 
					contained in, a colon, and the atom name.	Blanks are removed from both names. 
					For example, for the alpha carbon atom of isoleucine <tt>getFullName</tt> 
					will return the name <tt>ILE:CA</tt>. 
					For N terminal residues, <tt>-N</tt> is appended to the residue name, for C 
					terminal residues <tt>-C</tt>.	If the residue is a CYS involved in a disulphide
					bridge, an additional <tt>-S</tt> or <tt>S</tt> (for terminal residue)
					is appended. For single amino acids (C and N terminal) <tt>-M</tt> is added. \par
					If the atom is not contained in a residue, the name of the parent fragment 
					is taken instead of	the residue name. If there is no parent fragment, the name 
					of the parent molecule is taken.
					If the atom is not contained in any superstructure, getFullname returns getName. \par
					Overview of the returned strings:

						- <residue>:<atom>  -- if contained in a residue
						- <residue>-C:<atom>	-- for C terminal residues
						- <residue>-N:<atom>	-- for N terminal residues
						- CYS-S:<atom> -- for CYS residues involved in a SS bond
						- CYS-NS:<atom> -- for N terminal CYS residues involved in a SS bond
						- CYS-CS:<atom> -- for C terminal CYS residues involved in a SS bond
						- <fragment>:atom -- for atoms contained in a fragment, but not in a residue
						- <molecule>:atom -- for atoms contained in a molecule, but not in a fragment
									
					@param	type if type is set to <tt>Atom::NO_VARIANT_EXTENSIONS</tt>, 
									the variant extension (<tt>-XX</tt>) is omitted
					@return	String the full name
			*/
			String getFullName(FullNameType type = ADD_VARIANT_EXTENSIONS) const
				throw();

			/** Change the atom's position vector.
					@param   position the new position vector
			*/
			void setPosition(const Vector3& position)
				throw();

			/** Mutable inspection of the atom's position vector.
					@return  Vector3& - mutable reference to the position vector
			*/
			Vector3& getPosition()
				throw();

			/** Constant inspection of the atom's position vector.
					@return  Vector3& - constant reference to the position vector
			*/
			const Vector3& getPosition() const
				throw();

			/** Set the atom radius.
					BALL uses units of Angstrom for the atom radii.
					@param   radius the new radius
			*/
			void setRadius(float radius)
				throw();
				
			/** Return the atom radius.
					BALL uses units of Angstrom for the atom radii.
					@return  float - copy of the radius
			*/
			float getRadius() const
				throw();

			/** Set the atom type.
					@param   atom_type the new type
			*/
			void setType(Type atom_type)
				throw();
		
			/** Return the atom type.
					@return  Type - copy of the type
			*/
			Type getType() const
				throw();
		
			/**	Get the type name of the atom.
					@return the type name 
			*/
			String getTypeName() const
				throw();

			/**	Set the type name of the atom.
					@param the new type name
			*/
			void setTypeName(const String& name)
				throw();

			/** Set the atom velocity
					BALL uses units of {\AA}/ps for the velocity.
					@param   velocity the new velocity vector
			*/
			void setVelocity(const Vector3& velocity)
				throw();

			/** 
					BALL uses units of {\AA}/ps for the velocity.
					@return  Vector3& - constant reference to the velocity vector
			*/
			const Vector3& getVelocity() const
				throw();

			/** Change the atom's force vector.
					BALL uses units of {\em Newton} (1 N = 1 J/m) as the unit of force.
					@param   force the new force vector
			*/
			void setForce(const Vector3& force)
				throw();

			/** Mutable inspection of the atom's force vector.
					BALL uses units of {\em Newton} (1 N = 1 J/m) as the unit of force.
					@return  Vector3& - mutable reference to the force vector
			*/
			Vector3& getForce()
				throw();

			/** Constant inspection of the atom's force vector.
					BALL uses units of {\em Newton} (1 N = 1 J/m) as the unit of force.
					@return  Vector3& - constant reference to the force vector
			*/
			const Vector3& getForce() const
				throw();

			/** Constant inspection of the atom's number of bonds.
					@return	Size - copy of number of bonds
			*/
			Size countBonds() const
				throw();

			/** Mutable inspection of an atom's indexed bond.
					The reference is 0 if this instance does not have a bond with index {\em index}. \par
					<b>Note:</b> No corresponding mutator Atom::setBond exists to
					consider design of contract - an atom may not insert a bond in its bond table at a given index.
					The atom's bond table is an implementation detail that is not relevant to and should not be relied
					on by the client programmer. A bond must always be created via  \link Bond::Bond Bond::Bond \endlink  or
					 \link Atom::createBond Atom::createBond \endlink .
					@param   index the index of the bond to be accessed to
					@return  Bond* - mutable pointer to the bond that is indexed in this instance's bond table,
									 0 if this instance does not have a bond with index {\em index}
					@exception   IndexOverflow if <tt>index > MAX_NUMBER_OF_BONDS</tt>
			*/
			Bond* getBond(Position index)
				throw(Exception::IndexOverflow);

			/** Constant inspection of an atom's indexed bond.
					The reference is 0 if this instance does not have a bond with index {\em index}. \par
					<b>Note:</b> No corresponding mutator Atom::setBond exists to
					consider design of contract - an atom may not insert a bond in its bond table at a given index.
					The atom's bond table is an implementation detail that is not relevant to and should not be relied
					on by the client programmer. A bond must always be created via  \link Bond::Bond Bond::Bond \endlink  or
					 \link Atom::createBond Atom::createBond \endlink .
					@param   index the index of the bond to be accessed to
					@return  Bond* - constant pointer to the bond that is indexed in this instance's bond table,
									 0 if this instance does not have a bond with index {\em index}
					@exception   IndexOverflow if <tt>index > MAX_NUMBER_OF_BONDS</tt>
			*/
			const Bond* getBond(Position index) const
				throw(Exception::IndexOverflow);

			/** Mutable inspection of an atom's bond with another atom.
					The reference is 0 if this instance does not have a bond with {\em atom}.
					@param   atom the atom that is considered to have a bond with this instance
					@return  Bond* - mutable pointer to the bond that connects <tt>atom</tt>  with this instance,
									 0 if this instance does not have a bond with {\em atom}
					@see     Atom::createBond	     
			*/
			Bond* getBond(const Atom& atom)
				throw();

			/** Constant inspection of an atom's bond with another atom.
					The reference is 0 if this instance does not have a bond with {\em atom}.
					@param   atom the atom that is considered to have a bond with this instance
					@return  Bond* - constant pointer to the bond that connects {\em atom} with 
									 this instance, 0 if this instance does not have a bond with {\em atom}
					@see     Atom::createBond	     
			*/
			const Bond* getBond(const Atom& atom) const
				throw();

			//@}
			/** @name Miscellaneous 
			*/
			//@{ 

			/** Bond creation.
					Create a new instance of  \link Bond Bond \endlink  connecting this instance to {\em atom}.
					Calls  \link Bond::createBond Bond::createBond \endlink .
					The state of the bond is initialized to the default values.
					@return  Bond* - default initialized Bond instance that connects this instance to {\em atom}
					@see     Bond::createBond
			*/
			Bond* createBond(Atom& atom)
				throw();

			/** Extended bond creation.
					Initialize the bond {\em bond} to connect this instance to {\em atom}.
					Calls  \link Bond::createBond Bond::createBond \endlink .
					The state of the bond is initialzed to the default values. \par
					<b>Note:</b> This method is recommended for use if a subclass of the  \link Bond Bond \endlink 
									 is to be used as the new bond. This permits extensibility of bonds to the framework client.
					@return  Bond* - default initialized bond {\em bond} that connects this instance to {\em atom}
					@see     Bond::createBond
			*/
			Bond* createBond(Bond& bond, Atom& atom)
				throw();

			/**	Create a copy of a bond.
			*/
			Bond* cloneBond(Bond& bond, Atom& atom)
				throw();

			/** Explicit bond destruction.
					Destroy the bond connecting {\em *this atom} and {\em atom} explicitly.
					If the bond is auto-deletable the default destructor is called 
					otherwise  \link Bond::destroy Bond::destroy \endlink . \par
					<b>Note:</b> This method is recommended to destroy a bond of an atom explicitly
					instead of using the keyword <tt>delete</tt>.
					This is due to erroneous explicit destruction of statically allocated bonds.
					@param   atom the atom that should be disconnected from this instance
					@see     AutoDeletable
					@see     Bond::destroy
			*/
			bool destroyBond(const Atom& atom)
				throw();

			/** Explicit bond table destruction.
					Destroy all the bonds connecting {\em *this atom} with another atom explicitly.
					If the bonds are auto-deletable the default destructors are called 
					otherwise  \link Bond::destroy Bond::destroy \endlink . \par
					<b>Note:</b> This method is recommended to destroy all bonds of an atom explicitly
					instead of using the keyword <tt>delete</tt>.
					This is due to erroneous explicit destruction of statically allocated bonds.
					@param     atom the atom that should be disconnected from this instance
					@see       AutoDeletable
					@see       Bond::destroy
			*/
			void destroyBonds()
				throw();

			//@}
			/** @name Predicates 
			*/
			//@{ 

			/** Determine whether the atom takes part in a certain bond.
					@param   bond the bond in question
					@return  bool <tt>true</tt> if the bond {\em bond} connects this instance with another atom,
										    <tt>false</tt> otherwise
					@see     Atom::hasBond
			*/
			bool hasBond(const Bond& bond) const
				throw();

			/** Determine whether the atom is bound to another.
					Calls  \link Atom::getBond Atom::getBond \endlink .
					@param   atom the atom in question
					@return  bool - <tt>true</tt> if bond connects {\em atom} with {\em *this atom},
													<tt>false</tt> otherwise
					@see     Atom::getBond
			*/
			bool isBoundTo(const Atom& atom) const
				throw();

			/** Determine whether the atom has any bond.
					@return  bool - <tt>true</tt> if an atom is bound to this instance,
													<tt>false</tt> otherwise
					@see     Atom::hasBond
			*/
			bool isBound() const
				throw();

			/**	True if the two atoms are geminal.
					Two atoms are geminal if they do not share a common bond but both have a
					bond to a third atom. For example the two hydrogen atoms in water are geminal. 
					@param	atom the second atom
					@return bool - <b>true</b> if <tt>atom</tt> is geminal to this instance
			*/
			bool isGeminal(const Atom& atom) const
				throw();

			/**	True if the two atoms are vicinal.
					Two atoms are vicinal if they are separated by three bonds (1-4 position).
					@param	atom the second atom
					@return bool - <b>true</b> if <tt>atom</tt> is vicinal to this instance
			*/
			bool isVicinal(const Atom& atom) const
				throw();

			//@}
			/** @name Debuggers and diagnostics 
			*/
			//@{ 

			/** Internal state and consistency self-validation.
					@return	bool - <tt>true</tt> if the internal state of this 
									instance is correct (self-validated) and consistent, <tt>false</tt> otherwise
			*/
			virtual bool isValid() const
				throw();

			/** Internal state dump.
					Dump the current internal state of this instance to 
					the output ostream {\em s} with dumping depth {\em depth}.
					@param   s - output stream where to output the internal state
					@param   depth - the dumping depth
			*/
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const
				throw();

			//@}

			/** @name Internal iteration
			*/
			//@{

			/** Application of an unary processor on every contained bond.
					@param  processor a typed unary processor for  \link Bond Bond \endlink  instances
					@return  bool - <tt>true</tt> if application has been terminated successfully,
													<tt>false</tt> otherwise
			*/
			bool applyBonds(UnaryProcessor<Bond>& processor)
				throw();

			//@}

			/** @name External iteration
			*/
			//@{

			typedef Index BondIteratorPosition;

			class BondIteratorTraits
			{
				public:

				BALL_CREATE_DEEP(BondIteratorTraits)

				BondIteratorTraits()
					throw()
					:	bound_(0),
						position_(0)
				{
				}
				
				BondIteratorTraits(const Atom& atom)
					throw()
					:	bound_((Atom*)&atom),
						position_(0)
				{
				}
				
				BondIteratorTraits(const BondIteratorTraits& traits, bool /* deep */ = true)
					throw()
					:	bound_(traits.bound_),
						position_(traits.position_)
				{
				}
				
				BondIteratorTraits& operator = (const BondIteratorTraits& traits)
					throw()
				{
					bound_ = traits.bound_;
					position_ = traits.position_;
					return *this;
				}

				Atom* getContainer() throw() { return bound_;	}

				const Atom* getContainer() const throw() { return bound_;	}

				bool isSingular() const throw()	{	return (bound_ == 0);	}

				BondIteratorPosition& getPosition()	throw()	{	return position_;	}

				const BondIteratorPosition& getPosition() const	throw()	{ return position_;	}

				bool operator == (const BondIteratorTraits& traits) const	throw()
				{
					return (position_ == traits.position_);
				}

				bool operator != (const BondIteratorTraits& traits) const	throw()
				{
					return !(position_ == traits.position_);
				}
				
				bool operator < (const BondIteratorTraits& traits) const throw()
				{
					return (position_ < traits.position_);
				}

				Distance getDistance(const BondIteratorTraits& traits) const throw()
				{
					return (Distance)(position_ - traits.position_);
				}

				bool isValid() const throw()
				{
					return (bound_ != 0 && position_ >= 0 && position_ < bound_->number_of_bonds_);
				}

				void invalidate()	throw()	
				{	
					bound_ = 0;
					position_ = 0;
				}

				void toBegin() throw() { position_ = 0;	}

				bool isBegin() const throw() { return (position_ == 0); }

				void toEnd() throw() { position_ = bound_->number_of_bonds_; }

				bool isEnd() const throw() { return (position_ >= bound_->number_of_bonds_);}

				Bond& getData()	throw() {	return *(bound_->bond_[position_]); }

				const Bond& getData() const	throw()	{	return *(bound_->bond_[position_]);	}

				void forward() throw() { ++position_; }

				friend std::ostream& operator << (std::ostream& s, const BondIteratorTraits& traits)
					throw()
				{
					return (s << traits.position_ << ' ');
				}

				void dump(std::ostream& s) const
					throw()
				{
					s << position_ << std::endl;
				}

				void toRBegin()
					throw()
				{
					position_ = bound_->number_of_bonds_ - 1;
				}

				bool isRBegin() const
					throw()
				{
					return (position_ == bound_->number_of_bonds_ - 1);
				}
				
				void toREnd()
					throw()
				{
					position_ = -1;
				}

				bool isREnd() const
					throw()
				{
					return (position_ <= -1);
				}
				
				void backward()
					throw()
				{
					--position_;
				}

				void backward(Distance distance)
					throw()
				{
					position_ -= distance;
				}

				void forward(Distance distance)
					throw()
				{
					position_ += distance;
				}

				Bond& getData(Index index)
					throw()
				{
					return *(bound_->bond_[index]);
				}

				const Bond &getData(Index index) const
					throw()
				{
					return *(bound_->bond_[index]);
				}

				private:

				Atom* 								bound_;
				BondIteratorPosition position_;

			};

			friend class BondIteratorTraits;

			/** Random access iterator for bonds.
			*/
			typedef RandomAccessIterator
				<Atom, Bond, BondIteratorPosition, BondIteratorTraits>
				BondIterator;
			
			/// Return a bond iterator pointing to the first bond of the atom
			BondIterator beginBond()
				throw()
			{
				return BondIterator::begin(*this);
			}

			/// Return a past-the-end bond iterator
			BondIterator endBond()
				throw()
			{
				return BondIterator::end(*this);
			}

			/// Constant random access iterator for bonds
			typedef ConstRandomAccessIterator
				<Atom, Bond, BondIteratorPosition, BondIteratorTraits>
				BondConstIterator;

			/// Return a constant bond iterator pointing to the first bond
			BondConstIterator beginBond() const
				throw()
			{
				return BondConstIterator::begin(*this);
			}

			/// Return a constant past-the-end bond iterator.
			BondConstIterator endBond() const
				throw()
			{
				return BondConstIterator::end(*this);
			}
		
			/// Reverse random access iterator for bonds.
			typedef std::reverse_iterator<BondIterator>	BondReverseIterator;

			/// Return a reverse bond iterator pointing to the last bond.
			BondReverseIterator rbeginBond()
				throw()
			{
				return BondReverseIterator(endBond());
			}

			/// Return a past-the-end bond iterator for reverse traversal.
			BondReverseIterator rendBond()
				throw()
			{
				return BondReverseIterator(beginBond());
			}

			/// Constant reverse random access iterator for bonds.
			typedef std::reverse_iterator<BondConstIterator> BondConstReverseIterator;

			/// Return a constant reverse bond iterator pointing to the first atom
			BondConstReverseIterator rbeginBond() const
				throw()
			{
				return BondConstReverseIterator(endBond());
			}

			/// Return a constant past-the-end bond iterator for reverse traversal
			BondConstReverseIterator rendBond() const
				throw()
			{
				return BondConstReverseIterator(beginBond());
			}

		//@}

		/**	@name Efficient handling of atom attributes
		*/
		//@{
		///
		class StaticAtomAttributes
		{
			public:
			///
			float 					charge;
			///
			Vector3 				position;
			///
			Type 						type;
			///
			Vector3 				velocity;
			///
			Vector3 				force;
			///
			Atom*						ptr;

			/// Set the attributes to their default values
			void clear();

			/** Swap the contents of the two attributes.
					Adjusts the <tt>ptr</tt> and <tt>index_</tt> members of
					 \link StaticAtomAttributes StaticAtomAttributes \endlink  and  \link Atom Atom \endlink .
			*/
			void swap(StaticAtomAttributes& attr);

			/** Assign the contents from a different atom attribute.
			*/
			void set(StaticAtomAttributes& attr);

			/** Assign the contents from a different atom attribute.
			*/
			StaticAtomAttributes& operator = (const StaticAtomAttributes& attr);
		};

		
		///
		class AttributeVector
			:	public std::vector<StaticAtomAttributes>
		{
			public:
			~AttributeVector() throw();
		};

		///
		typedef std::list<Atom*> AtomPtrList;

		///
		typedef std::list<Position> AtomIndexList;
			
		/**	Compact memory for a list of atoms.
				This method packs the static attributes of the atom in the given 
				range into a contiguous memory segment in order to increase 
				locality.
		*/
		static Position compact(const AtomIndexList& indices)
			throw(Exception::OutOfRange);

		/** Access to the static attribute array
		*/
		static AttributeVector& getAttributes();

		/** Return the index in the static attribute array
		*/
		Position getIndex() const;

		StaticAtomAttributes* getAttributePtr();
		const StaticAtomAttributes* getAttributePtr() const;
		//@}


		protected:

		/**	@name Attributes
		*/
		//@{

		///
		static AttributeVector	static_attributes_;

		///
		static AtomIndexList		free_list_;

		///
		Position				index_;
		///
		const Element* 	element_;
		///
		String 					name_;
		///
		String 					type_name_;
		///
		float 					radius_;
		///
		unsigned char		number_of_bonds_;
		///
		Bond*						bond_[MAX_NUMBER_OF_BONDS];
		//@}

		private:

		/// Return the next unallocated index in the static attribute array
		static Position nextIndex_();

		/// Free an index in the static attribute array
		static void freeIndex_(Position index);

		///
		void clear_()
			throw();

		///
		void swapLastBond_(const Atom* atom)
			throw();

	};

# ifndef BALL_NO_INLINE_FUNCTIONS
#   include <BALL/KERNEL/atom.iC>
# endif
} // namespace BALL

#ifndef BALL_KERNEL_BONDITERATOR_H
#	include <BALL/KERNEL/bondIterator.h>
#endif


#endif // BALL_KERNEL_ATOM_H
