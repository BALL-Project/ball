// $Id: atom.h,v 1.39 2001/06/27 18:03:29 amoll Exp $

#ifndef BALL_KERNEL_ATOM_H
#define BALL_KERNEL_ATOM_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_CONCEPT_COMPOSITE_H
#	include <BALL/CONCEPT/composite.h>
#endif

#ifndef BALL_CONCEPT_PROPERTY_H
#	include <BALL/CONCEPT/property.h>
#endif

#ifndef BALL_CONCEPT_REVERSERANDOMACCESSITERATOR_H
#	include <BALL/CONCEPT/reverseRandomAccessIterator.h>
#endif

#ifndef BALL_DATATYPE_STRING_H
#	include <BALL/DATATYPE/string.h>
#endif

#ifndef BALL_CONCEPT_PROCESSOR_H
#	include <BALL/CONCEPT/processor.h>
#endif

#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector3.h>
#endif

// Defines for default values for an atom
#define BALL_ATOM_DEFAULT_ELEMENT    &Element::UNKNOWN
#define BALL_ATOM_DEFAULT_CHARGE     0
#define BALL_ATOM_DEFAULT_NAME       ""
#define BALL_ATOM_DEFAULT_TYPE_NAME  "?"
#define BALL_ATOM_DEFAULT_POSITION   0,0,0
#define BALL_ATOM_DEFAULT_RADIUS     0
#define BALL_ATOM_DEFAULT_TYPE       UNKNOWN_TYPE
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
			identified by a \Ref{Object::Handle}.
			Atom equality is defined as atom identity, so there cannot be any two identical atoms.
			A linear ordering of atoms is defined as the linear order of the
			\Ref{Object::Handle}'s.\\
			Two atoms can be connected via a \Ref{Bond}. There can be only one bond
			between any two atoms (double bonds etc. are expressed via the bond order
			attribute of the bond) and the total number of bonds of an atom is limited to 
			eight (can be changed at compile time, see \Ref{MAX_NUMBER_OF_BONDS}).\\
			Since \Ref{Atom} is derived from \Ref{ProperyManager}, it may contain
			arbitrary, user-defined properties.

			An atom may be inserted in a \Ref{Fragment} instance ("parent fragment").
			The "state" of an atom is defined by its attributes:
			\begin{itemize}
				\item "element" - an instance of \Ref{Element}
				\item "charge" - the charge in multiples of the the proton charge
				\item "name" - a string identifier
				\item "type name" - a string identifier, meaningful in the the context of a forcefield only
				\item "position" - the absolute position in cartesian coordinates (Angstrom)
				\item "radius" - the radius (Angstrom)
				\item "type" - an integer type, meaningful only in the context of a forcefield 
				\item "velocity" - velocity the velocity of the atom (Angstrom/ps)
				\item "force" - the force experienced by the atom (for forcefield calculations, in units of Newton)
				\item "bonds" - up to \Ref{MAX_NUMBER_OF_BONDS} bonds to other atoms
			\end{itemize}
			\\
			{\bf Definition:} \URL{BALL/KERNEL/atom.h}\\
			@see Bond
			@see Molecule
			@see AtomContainer
	*/
	class Atom
		: public Composite,
			public PropertyManager
	{
		public:

			/** @name Class friends
					\begin{itemize}
						\item class Bond
					\end{itemize}
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

				/// Maximaum number of bonds of an atom
				MAX_NUMBER_OF_BONDS = 8
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
				ADD_VARIANT_EXTENSIONS
			};

			//@}  
			/** @name Constructors 
			*/
			//@{

			/** Default constructor.
					The state of this instance is:
					\begin{itemize}
						\item element type is unknown (Element::UNKNOWN)
						\item charge is 0
						\item name is empty string
						\item type name is "?"
						\item position is \Ref{Vector3}(0,0,0)
						\item radius is 0
						\item type \Ref{INVALID_TYPE}
						\item velocity is \Ref{Vector3}(0,0,0)
						\item force is \Ref{Vector3}(0,0,0)
						\item bond table is empty (atom has no bonds)
					\end{itemize}
					@return  Atom - new atom
					@see     Composite::Composite
					@see     PropertyManager::PropertyManager
			*/
			Atom()
				throw();
		
			/** Copy constructor.
					The copy is either deep or shallow (default).
					\\
					{\bf Note:} Deep copying of atoms does not include bond cloning.
					@param   atom the atom to be copied (cloned)
					@param   deep make a deep (={\tt true}) or shallow (={\tt false}) copy of {\em atom}
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
					disconnected and the associated \Ref{Bond} instance is destroyed.
					Calls \Ref{Atom::destroy}.
					@see  Atom::destroy
			*/
			virtual ~Atom()
				throw();

			/** Explicit default initialization.
					Calls \Ref{Composite::clear} and resets the 
					attributes to the default values. In contrast
					to \Ref{destroy}, the atom is not removed from 
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
				throw();

			/**	Read an Atom from a persistent stream.
					@param pm the persistence manager
			*/
			virtual void persistentRead(PersistenceManager& pm)
				throw();

			//@}

			/** @name Assignment methods 
			*/
			//@{

      /** Deep/shallow assignment.
          The assignment is either deep or shallow (default is deep).
					In the case of a deep assignment, all composites contained in
					{\tt atom} are copied as well. 
					\\
          {\bf Caveat:} Bonds are not copied!
          @param   atom the atom to be copied
          @param   deep make a deep (={\bf true}) or shallow (={\bf false}) copy of {\tt atom}
      */
      void set(const Atom& atom, bool deep = true)
        throw();

      /** Deep/shallow assignment.
					The inverse operation to \Ref{set}, behaves identically.
          @param  atom the atom to be assigned to
          @see    Atom::set
      */
      void get(Atom& atom, bool deep = true) const
        throw();

			/** Assignment operator.
					The assignment is always deep.	Calls \Ref{Atom::set}.
					\\
					{\bf Note:} Bonds are not copied
					@param   atom the atom to be copied
					@return  Atom& - this instance
					@see     Atom::set
			*/
			const Atom& operator = (const Atom& atom)
				throw();

			/** Swap the contents of two atoms
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
					\\
					Use \Ref{Molecule::insert} to insert an atom into a molecule and
					\Ref{Molecule::remove} to remove it.
					@return  Molecule* - mutable pointer to the parent molecule
			*/
			Molecule *getMolecule()
				throw();

			/** Constant inspection of the atom's parent molecule.
					A NULL pointer is returned if this atom is not part of a molecule.
					\\
					Use \Ref{Molecule::insert} to insert an atom into a molecule and
					\Ref{Molecule::remove} to remove it.
					@return  Molecule* - constant pointer to the parent molecule
			*/
			const Molecule* getMolecule() const
				throw();

			/** Mutable inspection of the atom's parent fragment.
					A NULL pointer is returned if this atom is not part of a fragment.
					\\
					Use \Ref{Fragment::insert} to insert an atom into a fragment and
					\Ref{Fragment::remove} to remove it.
					@return   Fragment* - mutable pointer to the parent fragment
			*/
			Fragment* getFragment()
				throw();

			/** Constant inspection of the atom's parent fragment.
					A NULL pointer is returned if this atom is not part of a fragment.
					\\
					Use \Ref{Fragment::insert} to insert an atom into a fragment and
					\Ref{Fragment::remove} to remove it.					
					@return   Fragment* -	constant pointer to the parent fragment
			*/
			const Fragment* getFragment() const
				throw();

			/** Constant inspection of the atom's parent residue.
					A NULL pointer is returned if this atom is not part of a residue.
					\\
					Use \Ref{Residue::insert} to insert an atom into a residue and
					\Ref{Residue::remove} to remove it.					
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
					For example, for the alpha carbon atom of isoleucine {\tt getFullName} 
					will return the name {\tt ILE:CA}. 
					For N terminal residues, {\tt -N} is appended to the residue name, for C 
					terminal residues {\tt -C}.	If the residue is a CYS involved in a disulphide
					bridge, an additional {\tt -S} or {\tt S} (for terminal residue)
					is appended. For single amino acids (C and N terminal) {\tt -M} is added.\\
					If the atom is not contained in a residue, the name of the parent fragment 
					is taken instead of	the residue name. If there is no parent fragment, the name 
					of the parent molecule is taken.
					If the atom is not contained in any superstructure, getFullname returns getName.\\
					Overview of the returned strings:
					\begin{itemize}
						\item <residue>:<atom>  -- if contained in a residue
						\item <residue>-C:<atom>	-- for C terminal residues
						\item <residue>-N:<atom>	-- for N terminal residues
						\item CYS-S:<atom> -- for CYS residues involved in a SS bond
						\item CYS-NS:<atom> -- for N terminal CYS residues involved in a SS bond
						\item CYS-CS:<atom> -- for C terminal CYS residues involved in a SS bond
						\item <fragment>:atom -- for atoms contained in a fragment, but not in a residue
						\item <molecule>:atom -- for atoms contained in a molecule, but not in a fragment
					\end{itemize}				
					@param	type if type is set to {\tt Atom::NO_VARIANT_EXTENSIONS}, 
									the variant extension ({\tt -XX}) is omitted
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
					The reference is 0 if this instance does not have a bond with index {\em index}.\\
					{\bf Note:} No corresponding mutator Atom::setBond exists to
					consider design of contract - an atom may not insert a bond in its bond table at a given index.
					The atom's bond table is an implementation detail that is not relevant to and should not be relied
					on by the client programmer. A bond must always be created via \Ref{Bond::Bond} or
					\Ref{Atom::createBond}.
					@param   index the index of the bond to be accessed to
					@return  Bond* - mutable pointer to the bond that is indexed in this instance's bond table,
									 0 if this instance does not have a bond with index {\em index}
					@exception   IndexOverflow if {\tt index > MAX_NUMBER_OF_BONDS}
			*/
			Bond* getBond(Position index)
				throw(Exception::IndexOverflow);

			/** Constant inspection of an atom's indexed bond.
					The reference is 0 if this instance does not have a bond with index {\em index}.\\
					{\bf Note:} No corresponding mutator Atom::setBond exists to
					consider design of contract - an atom may not insert a bond in its bond table at a given index.
					The atom's bond table is an implementation detail that is not relevant to and should not be relied
					on by the client programmer. A bond must always be created via \Ref{Bond::Bond} or
					\Ref{Atom::createBond}.
					@param   index the index of the bond to be accessed to
					@return  Bond* - constant pointer to the bond that is indexed in this instance's bond table,
									 0 if this instance does not have a bond with index {\em index}
					@exception   IndexOverflow if {\tt index > MAX_NUMBER_OF_BONDS}
			*/
			const Bond* getBond(Position index) const
				throw(Exception::IndexOverflow);

			/** Mutable inspection of an atom's bond with another atom.
					The reference is 0 if this instance does not have a bond with {\em atom}.
					@param   atom the atom that is considered to have a bond with this instance
					@return  Bond* - mutable pointer to the bond that connects {\tt atom}  with this instance,
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
					Create a new instance of \Ref{Bond} connecting this instance to {\em atom}.
					Calls \Ref{Bond::createBond}.
					The state of the bond is initialized to the default values.
					@return  Bond* - default initialized Bond instance that connects this instance to {\em atom}
					@see     Bond::createBond
			*/
			Bond* createBond(Atom& atom)
				throw();

			/** Extended bond creation.
					Initialize the bond {\em bond} to connect this instance to {\em atom}.
					Calls \Ref{Bond::createBond}.
					The state of the bond is initialzed to the default values.\\
					{\bf Note:} This method is recommended for use if a subclass of the \Ref{Bond}
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
					otherwise \Ref{Bond::destroy}.\\
					{\bf Note:} This method is recommended to destroy a bond of an atom explicitly
					instead of using the keyword {\tt delete}.
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
					otherwise \Ref{Bond::destroy}.\\
					{\bf Note:} This method is recommended to destroy all bonds of an atom explicitly
					instead of using the keyword {\tt delete}.
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
					@return  bool {\tt true} if the bond {\em bond} connects this instance with another atom,
										    {\tt false} otherwise
					@see     Atom::hasBond
			*/
			bool hasBond(const Bond& bond) const
				throw();

			/** Determine whether the atom is bound to another.
					Calls \Ref{Atom::getBond}.
					@param   atom the atom in question
					@return  bool - {\tt true} if bond connects {\em atom} with {\em *this atom},
													{\tt false} otherwise
					@see     Atom::getBond
			*/
			bool isBoundTo(const Atom& atom) const
				throw();

			/** Determine whether the atom has any bond.
					@return  bool - {\tt true} if an atom is bound to this instance,
													{\tt false} otherwise
					@see     Atom::hasBond
			*/
			bool isBound() const
				throw();

			/**	True if the two atoms are geminal.
					Two atoms are geminal if they do not share a common bond but both have a
					bond to a third atom. For example the two hydrogen atoms in water are geminal. 
					@param	atom the second atom
					@return bool - {\bf true} if {\tt atom} is geminal to this instance
			*/
			bool isGeminal(const Atom& atom) const
				throw();

			/**	True if the two atoms are vicinal.
					Two atoms are vicinal if they are separated by three bonds (1-4 position).
					@param	atom the second atom
					@return bool - {\bf true} if {\tt atom} is vicinal to this instance
			*/
			bool isVicinal(const Atom& atom) const
				throw();

			//@}
			/** @name Debuggers and diagnostics 
			*/
			//@{ 

			/** Internal state and consistency self-validation.
					@return	bool - {\tt true} if the internal state of this 
									instance is correct (self-validated) and consistent, {\tt false} otherwise
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
					@param  processor a typed unary processor for \Ref{Bond} instances
					@return  bool - {\tt true} if application has been terminated successfully,
													{\tt false} otherwise
			*/
			bool applyBonds(UnaryProcessor<Bond>& processor)
				throw();

			//@}

			/** @name External iteration
			*/
			//@{

			typedef Index BondIteratorPosition_;

			class BondIteratorTraits_
			{
				public:

				BALL_CREATE_DEEP(BondIteratorTraits_)

				BondIteratorTraits_()
					throw()
					:	bound_(0),
						position_(0)
				{
				}
				
				BondIteratorTraits_(const Atom& atom)
					throw()
					:	bound_((Atom*)&atom),
						position_(0)
				{
				}
				
				BondIteratorTraits_(const BondIteratorTraits_& traits, bool /* deep */ = true)
					throw()
					:	bound_(traits.bound_),
						position_(traits.position_)
				{
				}
				
				const BondIteratorTraits_& operator = (const BondIteratorTraits_& traits)
					throw()
				{
					bound_ = traits.bound_;
					position_ = traits.position_;
					return *this;
				}

				Atom* getContainer()
					throw()
				{
					return bound_;
				}

				const Atom* getContainer() const
					throw()
				{
					return bound_;
				}

				bool isSingular() const
					throw()
				{
					return (bound_ == 0);
				}

				BondIteratorPosition_& getPosition()
					throw()
				{
					return position_;
				}

				const BondIteratorPosition_& getPosition() const
					throw()
				{
					return position_;
				}

				bool operator == (const BondIteratorTraits_& traits) const
					throw()
				{
					return (position_ == traits.position_);
				}

				bool operator != (const BondIteratorTraits_& traits) const
					throw()
				{
					return (position_ != traits.position_);
				}
				
				bool operator < (const BondIteratorTraits_& traits) const
					throw()
				{
					return (position_ < traits.position_);
				}

				Distance getDistance(const BondIteratorTraits_& traits) const
					throw()
				{
					return (Distance)(position_ - traits.position_);
				}

				bool isValid() const
					throw()
				{
					return (bound_ != 0 && position_ >= 0 && position_ < bound_->number_of_bonds_);
				}

				void invalidate()
					throw()
				{
					bound_ = 0;
					position_ = 0;
				}

				void toBegin()
					throw()
				{
					position_ = 0;
				}

				bool isBegin() const
					throw()
				{
					return (position_ == 0);
				}

				void toEnd()
					throw()
				{
					position_ = bound_->number_of_bonds_;
				}

				bool isEnd() const
					throw()
				{
					return (position_ >= bound_->number_of_bonds_);
				}

				Bond& getData()
					throw()
				{
					return *(bound_->bond_[position_]);
				}

				const Bond& getData() const
					throw()
				{
					return *(bound_->bond_[position_]);
				}

				void forward()
					throw()
				{
					++position_;
				}

				friend std::ostream& operator << (std::ostream& s, const BondIteratorTraits_& traits)
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
				BondIteratorPosition_ position_;

			};

			friend class BondIteratorTraits_;

			/** Random access iterator for bonds.
			*/
			typedef RandomAccessIterator
				<Atom, Bond, BondIteratorPosition_, BondIteratorTraits_>
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
				<Atom, Bond, BondIteratorPosition_, BondIteratorTraits_>
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
			typedef ReverseRandomAccessIterator
				<Atom, Bond, BondIteratorPosition_, BondIteratorTraits_>
				BondReverseIterator;

			/// Return a reverse bond iterator pointing to the last bond.
			BondReverseIterator rbeginBond()
				throw()
			{
				return BondReverseIterator::begin(*this);
			}

			/// Return a past-the-end bond iterator for reverse traversal.
			BondReverseIterator rendBond()
				throw()
			{
				return BondReverseIterator::end(*this);
			}

			/// Constant reverse random access iterator for bonds.
			typedef ConstReverseRandomAccessIterator
				<Atom, Bond, BondIteratorPosition_, BondIteratorTraits_>
				BondConstReverseIterator;

			/// Return a constant reverse bond iterator pointing to the first atom
			BondConstReverseIterator rbeginBond() const
				throw()
			{
				return BondConstReverseIterator::begin(*this);
			}

			/// Return a constant past-the-end bond iterator for reverse traversal
			BondConstReverseIterator rendBond() const
				throw()
			{
				return BondConstReverseIterator::end(*this);
			}

		//@}

		protected:

		/**	@name Attributes
		*/
		//@{
		///
		const Element* 	element_;
		///
		float 					charge_;
		///
		String 					name_;
		///
		String 					type_name_;
		///
		Vector3 				position_;
		///
		float 					radius_;
		///
		Type 						type_;
		///
		Vector3 				velocity_;
		///
		Vector3 				force_;
		///
		unsigned char		number_of_bonds_;
		///
		Bond*						bond_[MAX_NUMBER_OF_BONDS];
		//@}

		private:

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
