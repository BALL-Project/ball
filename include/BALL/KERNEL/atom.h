// $Id: atom.h,v 1.13 2000/03/17 11:25:53 oliver Exp $

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
#define BALL_ATOM_DEFAULT_FORCE   		0,0,0

namespace BALL 
{
	class Bond;
	class Element;
	class Fragment;
	class Molecule;

	/** Atom class.
		
			{\bf Framework:} BALL/KERNEL\\
			{\bf Definition:} \URL{BALL/KERNEL/atom.h}
			\\
			{\bf Category:} \Ref{Bond} container\\
			{\bf Concept:} composite design pattern\\
			
			An instance of Atom represents an instance of a chemical element ("atom").
			During each runtime instance of a program an atom is unique and identified by a \Ref{Object::Handle}.
			Atom equality is defined as atom identity.
			A linear ordering of atoms is defined as the linear order of the \Ref{Object::Handle}'s.
			Two atoms are connected by a \Ref{Bond} instance ("bond").
			There can exist only one bond between two atoms.
			For the sake of maintainability the first atom "owns" the bond thus the bond is existentially dependent on the first atom
			whilst the second atom "owes" the bond from the first atom.
			Bonds and atoms define an implict molecular graph.
			An atom may be inserted in a \Ref{Fragment} instance ("parent fragment").
			The "state" of an atom is defined by its attributes:
			\begin{itemize}
				\item "element"
				\item "charge"
				\item "name" 
				\item "type name" 
				\item "position"
				\item "radius"
				\item "type"
				\item "velocity"
				\item "force"
				\item "bonds"
			\end{itemize}
			
			@memo    Atom class (BALL kernel framework)
			@author  $Author: oliver $
			@version $Revision: 1.13 $
			@date    $Date: 2000/03/17 11:25:53 $
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

			BALL_CREATE(Atom)


			/** Atom type.
			*/
			typedef short Type;

		
			/** @name Enumerations */
			//@{
		
			/** Anonymous enumeration.
					Unnamed enumeration of all all non-categorized constants.
			*/
			enum 
			{
				/// Unknown atom type
				UNKNOWN_TYPE = -1,

				/// Any atom type 
				ANY_TYPE = 0,

				/// Maximal number of bonds of an atom
				MAX_NUMBER_OF_BONDS = 8
			};

			/** Predefined properties.
					Enumeration of all properties that are used by the BALL kernel.
			*/
			enum Property
			{
				NUMBER_OF_PROPERTIES
			};
				
			/**	The type of name used for getFullName
			*/
			enum FullNameType 
			{
				NO_VARIANT_EXTENSIONS,
				ADD_VARIANT_EXTENSIONS
			};
			//@}  

			/** @name Constructors */
			//@{

			/** Default constructor.
					Construct new atom.
					The state of {\em *this} atom is:
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

					@return      Atom - new constructed atom
					@see         Composite::Composite
					@see         PropertyManager::PropertyManager
			*/
			Atom();
		
			/** Copy constructor.
					Construct new atom by copying the atom {\em atom}.
					The copy is either deep or shallow (default).
					The state of {\em *this} bond is initialized to the state of the atom {\em atom}.\\
					{\bf Note:} Deep copying of atoms does not include bond cloning.

					@param       atom the atom to be copied (cloned)
					@param       deep make a deep (={\tt true}) or shallow (={\tt false}) copy of {\em stom}
					@return      Atom - new constructed atom cloned from {\em stom}
					@see         Composite::Composite
					@see         PropertyManager::PropertyManager
			*/
			Atom(const Atom& atom, bool deep = true);
		
			/** Detailed state initializing constructor.
					Construct new atom.
					The state of {\em *this} atom is:
					\begin{itemize}
						\item element type is {\em element}
						\item charge is {\em charge}
						\item name is {\em name}
						\item tpye name is {\em type_name}
						\item position is {\em position}
						\item radius is {\em radius}
						\item type {\em atom_type}
						\item velocity is {\em velocity}
						\item force is {\em force}
						\item bond table is empty (atom has no bonds)
					\end{itemize}

					@param       element element type of the constructed atom
					@param       name name of the constructed atom
					@param       type_name type name name of the constructed atom
					@param       atom_type type of the constructed atom
					@param       position position of the constructed atom
					@param       velocity velocity of the constructed atom
					@param       force force acting upon the constructed atom
					@param       charge charge of the constructed atom
					@param       radius radius of the constructed atom
					@param       auto_deletable the constructed atom is auto-deletable (={\tt true}) or not (={\tt false})
					@return      Atom - new constructed atom
					@see         Composite::Composite
					@see         PropertyManager::PropertyManager
			*/
			Atom
				(Element& element,
				 const String& name, const String& type_name = BALL_ATOM_DEFAULT_TYPE_NAME,
				 Type atom_type = BALL_ATOM_DEFAULT_TYPE,
				 const Vector3& position = Vector3(BALL_ATOM_DEFAULT_POSITION),
				 const Vector3& velocity = Vector3(BALL_ATOM_DEFAULT_VELOCITY),
				 const Vector3& force = Vector3(BALL_ATOM_DEFAULT_FORCE),
				 float charge = BALL_ATOM_DEFAULT_CHARGE,
				 float radius = BALL_ATOM_DEFAULT_RADIUS);

			//@}

			/** @name Destructors */
			//@{

			/** Destructor.
					Default destruction of {\em *this} atom.
					If the atom has bonds in common with another atom that atom is disconnected and
					the associated \Ref{Bond} instance is destroyed.
					Calls \Ref{Atom::destroy}.

					@see         Atom::destroy
			*/
			virtual ~Atom();

			/** Explicit default initialization.
					Set the state of {\em *this} atom to the default values.
					Calls {Composite::clear}.
					The state of {\em *this} atom is:
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

						@see	Composite::clear
			*/
			virtual void clear();
		
			/** Explicit destructor.
					Destroy {\em *this} atom explicitly without releasing its heap memory thus {\em *this} atom may exist further.
					Calls \Ref{Composite::destroy}.
					Set the state of {\em *this} atom to the default values. 
					The state of {\em *this} atom is:
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

					@see         Composite::destroy
			*/
			virtual void destroy();

			//@}
			
			/**	@name	Persistence */
			//@{
	
			///			
			virtual void persistentWrite(PersistenceManager& pm, const char* name = 0) const;

			/// 
			virtual void persistentRead(PersistenceManager& pm);
			//@}

			/** @name Assignment methods */
			//@{

			/** Assignment with cloning facility.
					Assign the atom {\em atom} to {\em *this} atom.
					The assignment is either deep or shallow (default).
					The state of {\em *this} atom is initialized to the state of the atom {\em atom}.\\
					\\
					{\bf Note:} Deep copying of bonds is not supported.

					@param       atom the atom to be copied (cloned)
					@param       deep make a deep (={\tt true}) or shallow (={\tt false}) copy of {\em atom}
			*/
			void set(const Atom& atom, bool deep = true);

			/** Assignment operator.
					Assign the atom {\em atom} to {\em *this} atom.
					The assignment is either deep or shallow (default).
					Calls \Ref{Atom::set}.
					The state of {\em *this} atom is initialized to the state of the atom {\em atom}.\\
					\\
					{\bf Note:} Deep copying of bonds is not supported.

					@param       atom the atom to be copied (cloned)
					@return      Atom& - {\em *this} atom
					@see         Atom::set
			*/
			Atom& operator = (const Atom& atom);

			/** Copying with cloning facility.
					Copy {\em *this} atom to the atom {\em atom}.
					The assignment is either deep or shallow (default).
					Calls \Ref{Atom::set}.
					The state of the atom {\em atom} is initialized to the state of {\em *this} atom.\\
					\\
					{\bf Note:} Deep copying of bonds is not supported.

					@param  atom the atom to be assigned to
					@see         Atom::set
			*/
			void get(Atom& atom, bool deep = true) const;

			/** Swapping of atoms.
					Swap the states of {\em *this} atom with the atom {\em atom}.

					@param       atom the atom {\em *this} atom is being swapped with
					@see         Atom::Atom
			*/
			void swap(Atom& atom);

			//@}

			/** @name Accessors: inspectors and mutators */
			//@{ 

			/** Change of the atom element type.
					Change the element type of {\em *this} atom to {\em element}.

					@param       element the new element type of {\em *this} atom
					@see         Atom::getElement
			*/
			void setElement(Element& element);
				
		
			/** Mutable inspection of the atom element type.
					Access a mutable reference to the element type of {\em *this} atom.

					@return      String& - mutable reference to the element type of {\em *this} atom
					@see         Atom::setElement
			*/
			Element& getElement();

			/** Constant inspection of the atom element type.
					Access a constant reference to the element type of {\em *this} atom.

					@return      String& - constant reference to the element type of {\em *this} atom
					@see         Atom::setElement
			*/
			const Element& getElement() const;

			/** Change of the atom's charge.
					Change the charge of {\em *this} atom to {\em charge}.

					@param       charge the new charge of {\em *this} atom
					@see         Atom::getCharge
			*/
			void setCharge(float charge);

			/** Constant inspection of the atom's charge.
					Access the charge of {\em *this} atom.

					@return      float - copy of the the charge of {\em *this} atom
					@see         Atom::setCharge
			*/
			float getCharge() const;

			/** Mutable inspection of the atom's parent molecule.
					Access a mutable reference to the parent molecule of {\em *this} atom.
					The reference is 0 if {\em *this} atom does not have a parent molecule.\\
					\\
					{\bf Note:} No corresponding mutator Atom::setMolecule exists to
					consider design of contract - an atom may not insert into into a molecule,
					it must be inserted via the molecule.

					@return      Molecule* -
											 mutable reference to the parent molecule of {\em *this} atom,
											 0 if {\em *this} atom does not have a parent molecule
			*/
			Molecule *getMolecule();

			/** Mutable inspection of the atom's parent molecule.
					Access a constant reference to the parent molecule of {\em *this} atom.
					The reference is 0 if {\em *this} atom does not have a parent molecule.\\
					\\
					{\bf Note:} No corresponding mutator Atom::setMolecule exists to
					consider design of contract - an atom may not insert into into a molecule,
					it must be inserted via the molecule.

					@return      Molecule* -
											 constant reference to the parent molecule of {\em *this} atom,
											 0 if {\em *this} atom does not have a parent molecule
			*/
			const Molecule *getMolecule() const;

			/** Mutable inspection of the atom's parent fragment.
					Access a mutable reference to the parent fragment of {\em *this} atom.
					The reference is 0 if {\em *this} atom does not have a parent fragment.\\
					\\
					{\bf Note:} No corresponding mutator Atom::setFragment exists to
					consider design of contract - an atom may not insert into into a fragment,
					it must be inserted via the fragment.

					@return      Fragment* -
											 mutable reference to the parent fragment of {\em *this} atom,
											 0 if {\em *this} atom does not have a parent fragment
			*/
			Fragment* getFragment();

			/** Mutable inspection of the atom's parent fragment.
					Access a constant reference to the parent fragment of {\em *this} atom.
					The reference is 0 if {\em *this} atom does not have a parent fragment.\\
					\\
					{\bf Note:} No corresponding mutator Atom::setFragment exists to
					consider design of contract - an atom may not insert into into a fragment,
					it must be inserted via the fragment.

					@return      Fragment* -
											 constant reference to the parent fragment of {\em *this} atom,
											 0 if {\em *this} atom does not have a parent fragment
			*/
			const Fragment* getFragment() const;

			/** Change of the atom's name.
					Change the name of {\em *this} atom to {\em name}.

					@param       name the new name of {\em *this} atom
					@see         Atom::getName
			*/
			void setName(const String& name);

			/** Mutable inspection of the atom's name.
					Access a mutable reference to the name of {\em *this} atom.

					@return      String& - mutable reference to the name of {\em *this} atom
					@see         Atom::setName
			*/
			String& getName();

			/** Constant inspection of the atom's name.
					Access a constant reference to the name of {\em *this} atom.

					@return      String& - constant reference to the name of {\em *this} atom
					@see         Atom::setName
			*/
			const String& getName() const;

			/** Assemble a fully specified atom name.
					This method returns at fully specified atom name as used for charge and type assignments.
					The name consists of the name of the residue the atom is conatined in, a colon, and the atom name.
					Blanks are removed from both names. For example, for the alpha carbon atom of isoleucine {\tt getFullName} 
					will return the name {\tt ILE:CA}. 
					For N terminal residues, {\tt -N} is appended to the residue name, for C terminal residues {\tt -C}.
					If the residue is a CYS an involved in a disulphide bridge, an additional {\tt -S} or {\tt S} (for terminal residue)
					is appended. Single amino acids (C and N terminal) {\tt -M} is added.\\
					If the atom is not contained in a residue, the name if the parent fragment 
					is taken instead of	the residue name. If there is no parent fragment, name of the parent molecule ist taken.
					If the atom is not contained in any superstructure, getFullname returns getName.
					@param	type if type is set to {\tt Atom::NO_VARIANT_EXTENSIONS}, the variant extension ({\tt -XX}) is omitted
					@return	String&
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
			*/
			String getFullName(FullNameType type = ADD_VARIANT_EXTENSIONS) const;

			/** Change of the atom's position vector.
					Change the position vector of {\em *this} atom to {\em position}.

					@param       position the new position vector of {\em *this} atom
					@see         Atom::getPosition
			*/
			void setPosition(const Vector3& position);

			/** Mutable inspection of the atom's position vector.
					Access a mutable reference to the position vector of {\em *this} atom.

					@return      Vector3& - mutable reference to the position vector of {\em *this} atom
					@see         Atom::setPosition
			*/
			Vector3& getPosition();

			/** Constant inspection of the atom's position vector.
					Access a constant reference to the position vector of {\em *this} atom.

					@return      Vector3& - constant reference to the position vector of {\em *this} atom
					@see         Atom::setPosition
			*/
			const Vector3& getPosition() const;

			/** Change of the atom's radius.
					Change the radius of {\em *this} atom to {\em radius}.

					@param       radius the new radius of {\em *this} atom
					@see         Atom::getRadius
			*/
			void setRadius(float radius);
				

			/** Constant inspection of the atom's radius.
					Access the radius of {\em *this} atom.

					@return      float - copy of the the radius of {\em *this} atom
					@see         Atom::setRadius
			*/
			float getRadius() const;

			/** Change of the atom's type.
					Change the type of {\em *this} atom to the type {\em atom_type}.

					@param       atom_type the new type of {\em *this} atom
					@see         Atom::getType
			*/
			void setType(Type atom_type);
		
			/** Constant inspection of the atom's type.
					Access the type of {\em *this} atom.

					@return      Type - copy of the the type of {\em *this} atom
					@see         Atom::setType
			*/
			Type getType() const;
		

			/**	Set the type name of the atom.
			*/
			String getTypeName() const;

			/**	Set the type name of the atom.
			*/
			void setTypeName(const String& name);

			/** Change of the atom's velocity vector.
					Change the velocity vector of {\em *this} atom to {\em velocity}.
					

					@param       velocity the new velocity vector of {\em *this} atom
					@see         Atom::getVelocity
			*/
			void setVelocity(const Vector3& velocity);

			/** Mutable inspection of the atom's velocity vector.
					Access a mutable reference to the velocity vector of {\em *this} atom.
					BALL uses units of \TeX{\AA}/ps for the velocity.
					@return      Vector3& - mutable reference to the velocity vector of {\em *this} atom
					@see         Atom::setVelocity
			*/
			Vector3& getVelocity();

			/** Constant inspection of the atom's velocity vector.
					Access a constant reference to the velocity vector of {\em *this} atom.
					BALL uses units of \TeX{\AA}/ps for the velocity.

					@return      Vector3& - constant reference to the velocity vector of {\em *this} atom
					@see         Atom::setVelocity
			*/
			const Vector3& getVelocity() const;

			/** Change of the atom's force vector.
					Change the force vector of {\em *this} atom to {\em force}.
					BALL uses units of {\em Newton} (1 N = 1 J/m) as the unit of force.

					@param       force the new force vector of {\em *this} atom
					@see         Atom::getForce
			*/
			void setForce(const Vector3& force);

			/** Mutable inspection of the atom's force vector.
					Access a mutable reference to the force vector of {\em *this} atom.
					BALL uses units of {\em Newton} (1 N = 1 J/m) as the unit of force.

					@return      Vector3& - mutable reference to the force vector of {\em *this} atom
					@see         Atom::setForce
			*/
			Vector3& getForce();

			/** Constant inspection of the atom's force vector.
					Access a constant reference to the force vector of {\em *this} atom.
					BALL uses units of {\em Newton} (1 N = 1 J/m) as the unit of force.

					@return      Vector3& - constant reference to the force vector of {\em *this} atom
					@see         Atom::setForce
			*/
			const Vector3& getForce() const;

			/** Constant inspection of the atom's number of bonds.
					Access the number of bonds of {\em *this} atom.

					@return	Size - copy of number of bonds of {\em *this} atom
			*/
			Size countBonds() const;

			/** Mutable inspection of an atom's indexed bond.
					Access a mutable reference to the bond with index {\em index} in {\em *this} atom's bond table.
					The reference is 0 if {\em *this} atom does not have a bond with index {\em index}.\\
					\\
					{\bf Note:} No corresponding mutator Atom::setBond exists to
					consider design of contract - an atom may not insert a bond in its bond table at a given index.
					The atom's bond table is an implementation detail that is not relevant to and should not be relied
					on by the client programmer. A bond must always be created via \Ref{Bond::Bond} or
					\Ref{Atom::createBond}.

					@param       index the index of the bond to be accessed to
					@return      Bond* -
											 mutable reference to the bond that is indexed in {\em *this} atom's bond table,
											 0 if {\em *this} atom does not have a bond with index {\em index}
			*/
			Bond* getBond(Position index);

			/** Constant inspection of an atom's indexed bond.
					Access a constant reference to the bond with index {\em index} in {\em *this} atom's bond table.
					The reference is 0 if {\em *this} atom does not have a bond with index {\em index}.\\
					\\
					{\bf Note:} No corresponding mutator Atom::setBond exists to
					consider design of contract - an atom may not insert a bond in its bond table at a given index.
					The atom's bond table is an implementation detail that is not relevant to and should not be relied
					on by the client programmer. A bond must always be created via \Ref{Bond::Bond} or
					\Ref{Atom::createBond}.

					@param       index the index of the bond to be accessed to
					@return      Bond* -
											 constant reference to the bond that is indexed in {\em *this} atom's bond table,
											 0 if {\em *this} atom does not have a bond with index {\em index}
			*/
			const Bond* getBond(Position index) const;

			/** Mutable inspection of an atom's bond with another atom.
					Access a mutable reference to the bond that connects the atom {\em atom} with {\em *this} atom.
					The reference is 0 if {\em *this} atom does not have a bond with the atom {\em atom}.

					@param       bonded_to the atom that is considered to have a bond with {\em *this} atom
					@return      Bond* -
											 mutable reference to the bond that connects the atom {\em bonded_to} with {\em *this} atom,
											 0 if {\em *this} atom does not have a bond with the atom {\em atom}
					@see         Atom::createBond	     
			*/
			Bond* getBond(const Atom& atom);

			/** Constant inspection of an atom's bond with another atom.
					Access a constant reference to the bond that connects the atom {\em atom} with {\em *this} atom.
					The reference is 0 if {\em *this} atom does not have a bond with the atom {\em atom}.

					@param       atom the atom that is considered to have a bond with {\em *this} atom
					@return      Bond* -
											 mutable reference to the bond that connects the atom {\em atom} with {\em *this} atom,
											 0 if {\em *this} atom does not have a bond with the atom {\em atom}
					@see         Atom::createBond	     
			*/
			const Bond *getBond(const Atom& atom) const;

			//@}

			/** @name Miscellaneous */
			//@{ 

			/** Bond creation.
					Create a new instance of \Ref{Bond} connecting {\em *this} atom to the atom {\em atom}.
					Calls \Ref{Bond::createBond}.
					The state of the bond is initialzed to the default values.

					@return        Bond* - default initialized Bond instance that connects {\em *this atom} to the atom {\em atom}
					@see           Bond::createBond
			*/
			Bond *createBond(Atom& atom);

			/** Extended bond creation.
					Initialize the bond {\em bond} to connect {\em *this} atom to the atom {\em atom}.
					Calls \Ref{Bond::createBond}.
					The state of the bond is initialzed to the default values.\\
					\\
					{\bf Note:} This method is recommended for use if a subclass of the \Ref{Bond}
									is to be used as the new bond. This permits extensibility of bonds to the framework client.

					@return        Bond* - default initialized bond {\em bond} that connects {\em *this atom} to the atom {\em atom}
					@see           Bond::createBond
			*/
			Bond *createBond(Bond& bond, Atom& atom);

			// UNDOCUMENTED, yet
			Bond *cloneBond(Bond& bond, Atom& atom);

			/** Explicit bond destruction.
					Destroy the bond connecting {\em *this atom} and the atom {\em atom} explicitly.
					If the bond is auto-deletable the default destructor is called otherwise \Ref{Bond::destroy}.\\
					\\
					{\bf Note:} This method is recommended to destroy a bond of an atom explicitly
					instead of using the keyword {\tt delete}. This is due to erroneous explicit destruction of statically allocated bonds.

					@param         atom the atom that should be disconnected from {\em *this} atom
					@see           AutoDeletable
					@see           Bond::destroy
			*/
			bool destroyBond(const Atom& atom);

			/** Explicit bond table destruction.
					Destroy all the bonds connecting {\em *this atom} with another atom explicitly.
					If the bonds are auto-deletable the default destructors are called otherwise \Ref{Bond::destroy}.\\
					\\
					{\bf Note:} This method is recommended to destroy all bonds of an atom explicitly
					instead of using the keyword {\tt delete}. This is due to erroneous explicit destruction of statically allocated bonds.

					@param         atom the atom that should be disconnected from {\em *this} atom
					@see           AutoDeletable
					@see           Bond::destroy
			*/
			void destroyBonds();

			//@}

			/** @name Predicates */
			//@{ 

			/** Request for the bonding with an atom.
					Query, if {\em *this} atom is bonded to another atom.
					If such a bond exists {\tt true} is returned, {\tt false} otherwise. 
		
					@param       bond the bond that is queried to connect {\em *this} atom with another atom
					@return      bool -
											 {\tt true} if the bond {\em bond} connects {\em *this} atom with another atom,
											 {\tt false} otherwise
					@see         Atom::hasBond
			*/
			bool hasBond(const Bond& bond) const;

			/** Request for the bonding with an atom.
					Query, if {\em *this} atom is bonded to the atom {\em atom}.
					If such a bond exists {\tt true} is returned, {\tt false} otherwise.
					Calls \Ref{Atom::getBond}.
		
					@param       atom the atom that is queried to be bonded to {\em *this} atom
					@return      bool -
											 {\tt true} if bond connects the atom {\em atom} with {\em *this atom},
											 {\tt false} otherwise
					@see         Atom::getBond
			*/
			bool isBondedTo(const Atom& atom) const;

			/** Request for the bonding with an atom.
					Query, if {\em *this} atom has any atom that it is connected to.
					If such an atom exists {\tt true} is returned, {\tt false} otherwise. 
		
					@return      bool -
											 {\tt true} if an atom is bonded to {\em *this} atom,
											 {\tt false} otherwise
					@see         Atom::hasBond
			*/
			bool isBonded() const;

			/**	True if the two atoms are geminal.
					Two atoms are geminal if they do not share common bond but
					both have a bond to a third atom. For example the two hydrogen atoms
					in water are geminal. 
					@param	atom the second atom
					@return bool - {\bf true} if {\tt atom} is geminal to {\tt this} atom
			*/
			bool isGeminal(const Atom& atom) const;

			/**	True if the two atoms are vicinal.
					Two atoms are vicinal if they are separated by three bonds (1-4 position).
					@param	atom the second atom
					@return bool - {\bf true} if {\tt atom} is vicinal to {\tt this} atom
			*/
			bool isVicinal(const Atom& atom) const;

			//@}

			/** @name Debuggers and diagnostics */
			//@{ 

			/** Internal state and consistency self-validation.
					Initiate self-validation of the internal state and data structure consistencies of {\em *this} atom.
					If the internal state of {\em *this} atom is correct (self-validated) and consistent {\tt true} is returned, {\tt false} otherwise. 
		
					@return			bool -
											{\tt true} if the internal state of {\em *this} atom is correct (self-validated) and consistent,
					 						{\tt false} otherwise
			*/
			virtual bool isValid() const;

			/** Internal state dump.
					Dump the current internal state of {\em *this} atom to the output ostream {\em s} with dumping depth {\em depth}.
		
					@param   s - output stream where to output the internal state of {\em *this} atom
					@param   depth - the dumping depth
			*/
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const;

		
			//@}

			/** @name Storers */
			//@{ 

			/* Persistent stream input and state restorage.
					Read persistent bond data from the input stream {\em s} and restore the state of {\em *this} atom.
					Virtually called by \Ref{Object::operator >>}.\\
					\\
					{\bf Note:} Not yet implemented.
		
					@param  s input stream from where to restore the internal state of {\em *this} atom
					@see    Object::operator >>
			*/
			virtual void read(std::istream& s);

			/* Persistent stream output and state storage.
					Write persistent bond data to the output stream {\em s} and store the state of {\em *this} atom.
					Virtually called by \Ref{Object::operator <<}.\\
					\\
					{\bf Note:} Not yet implemented.
		
					@param  s input stream from where to restore the internal state of {\em *this} atom
					@see         Object::operator <<
			*/
			virtual void write(std::ostream& s) const;

			//@}  

			/** @name Internal iterators */
			//@{

			/** Application of an unary processor on every contained bond.
					Apply the typed unary processor {\em processor} on all incident bonds of {\em *this} atom.

					@param  processor a typed unary processor for \Ref{Bond} instances
					@return      bool -
											 {\tt true} if application has been terminated successfully,
											 {\tt false} otherwise
			*/
			bool applyBonds(UnaryProcessor<Bond>& processor);
			//@}

			/** @name External iterators */
			//@{

			typedef Index BondIteratorPosition_;

			class BondIteratorTraits_
			{
				public:

				BALL_CREATE(BondIteratorTraits_)

				BondIteratorTraits_()
					:	bound_(0),
						position_(0)
				{
				}
				
				BondIteratorTraits_(const Atom& atom)
					:	bound_((Atom*)&atom),
						position_(0)
				{
				}
				
				BondIteratorTraits_(const BondIteratorTraits_& traits, bool /* deep */ = true)
					:	bound_(traits.bound_),
						position_(traits.position_)
				{
				}
				
				BondIteratorTraits_& operator = (const BondIteratorTraits_& traits)
				{
					bound_ = traits.bound_;
					position_ = traits.position_;
					return *this;
				}

				Atom* getContainer()
				{
					return bound_;
				}

				const Atom* getContainer() const
				{
					return bound_;
				}

				bool isSingular() const
				{
					return (bool)(bound_ == 0);
				}

				BondIteratorPosition_& getPosition()
				{
					return position_;
				}

				const BondIteratorPosition_& getPosition() const
				{
					return position_;
				}

				bool operator == (const BondIteratorTraits_& traits) const
				{
					return (bool)(position_ == traits.position_);
				}

				bool operator != (const BondIteratorTraits_& traits) const
				{
					return (bool)(position_ != traits.position_);
				}
				
				bool operator < (const BondIteratorTraits_& traits) const
				{
					return (bool)(position_ < traits.position_);
				}

				Distance getDistance(const BondIteratorTraits_& traits) const
				{
					return (Distance)(position_ - traits.position_);
				}

				bool isValid() const
				{
					return (bool)(bound_ != 0 && position_ >= 0 && position_ < bound_->number_of_bonds_);
				}

				void invalidate()
				{
					bound_ = 0;
					position_ = 0;
				}

				void toBegin()
				{
					position_ = 0;
				}

				bool isBegin() const
				{
					return (bool)(position_ == 0);
				}

				void toEnd()
				{
					position_ = bound_->number_of_bonds_;
				}

				bool isEnd() const
				{
					return (bool)(position_ >= bound_->number_of_bonds_);
				}

				Bond& getData()
				{
					return *(bound_->bond_[position_]);
				}

				const Bond& getData() const
				{
					return *(bound_->bond_[position_]);
				}

				void forward()
				{
					++position_;
				}

				friend std::ostream& operator << (std::ostream& s, const BondIteratorTraits_& traits)
				{
					return (s << traits.position_ << ' ');
				}

				void dump(std::ostream& s) const
				{
					s << position_ << std::endl;
				}

				void toRBegin()
				{
					position_ = bound_->number_of_bonds_ - 1;
				}

				bool isRBegin() const
				{
					return (bool)(position_ == bound_->number_of_bonds_ - 1);
				}
				
				void toREnd()
				{
					position_ = -1;
				}

				bool isREnd() const
				{
					return (bool)(position_ <= -1);
				}
				
				void backward()
				{
					--position_;
				}

				void backward(Distance distance)
				{
					position_ -= distance;
				}

				void forward(Distance distance)
				{
					position_ += distance;
				}

				Bond& getData(Index index)
				{
					return *(bound_->bond_[index]);
				}

				const Bond &getData(Index index) const
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
			
			/** \Ref{Atom::BondIterator} builder.
					Build a \Ref{Atom::BondIterator} instance for traversing
					{\em *this} atom and iterating over its bonds.
					The built \Ref{Atom::BondIterator} instance is positioned at
					{\em *this} atom's first bond.

					@return      Atom::BondIterator - a copy of a \Ref{Atom::BondIterator} instance 
												of {\em *this} atom positioned at its first bond
			*/
			BondIterator beginBond()
			{
				return BondIterator::begin(*this);
			}

			/** Past-the-end \Ref{Atom::BondIterator} builder.
					Build a past-the-end \Ref{Atom::BondIterator} instance for {\em *this} atom.
					The built \Ref{Atom::BondIterator} instance is positioned past the last valid forward reachable bond of {\em *this} atom.\\
					\\
					{\bf Note:} A past-the-end \Ref{Atom::BondIterator} instance is used as a sentinel for the finish of a forward iteration over {\em *this} atom.

					@return      Atom::BondIterator - a copy of a past-the-end \Ref{Atom::BondIterator} instance of {\em *this} atom positioned past its last forward reachable bond
			*/
			BondIterator endBond()
			{
				return BondIterator::end(*this);
			}



			/** Constant random access iterator for bonds.
			*/
			typedef ConstRandomAccessIterator
				<Atom, Bond, BondIteratorPosition_, BondIteratorTraits_>
				BondConstIterator;

			/** \Ref{Atom::BondConstIterator} builder.
					Build a \Ref{Atom::BondConstIterator} instance for traversing
					{\em *this} constant atom and iterating over its constant bonds.
					The built \Ref{Atom::BondConstIterator} instance is positioned at
					{\em *this} atom's first constant bond.

					@return      Atom::BondConstIterator - a copy of a \Ref{Atom::BondIterator} instance of {\em *this} constant atom positioned at its first constant bond
			*/
			BondConstIterator beginBond() const
			{
				return BondConstIterator::begin(*this);
			}

			/** Past-the-end \Ref{Atom::BondConstIterator} builder.
					Build a past-the-end \Ref{Atom::BondConstIterator} instance for {\em *this} constant atom.
					The built \Ref{Atom::BondConstIterator} instance is positioned past the last valid forward reachable constant bond of {\em *this} atom.\\
					\\
					{\bf Note:} A past-the-end \Ref{Atom::BondConstIterator} instance is used as a sentinel for the finish of a forward iteration over {\em *this} constant atom.

					@return      Atom::BondConstIterator - a copy of a past-the-end \Ref{Atom::BondIterator} instance of {\em *this} atom positioned past its last forward reachable constant bond
			*/
			BondConstIterator endBond() const
			{
				return BondConstIterator::end(*this);
			}


		
			/** Reverse random access iterator for bonds.
			*/
			typedef ReverseRandomAccessIterator
				<Atom, Bond, BondIteratorPosition_, BondIteratorTraits_>
				BondReverseIterator;

			BondReverseIterator rbeginBond()
			{
				return BondReverseIterator::begin(*this);
			}

			BondReverseIterator rendBond()
			{
				return BondReverseIterator::end(*this);
			}



			/** Constant reverse random access iterator for bonds.
			*/
			typedef ConstReverseRandomAccessIterator
				<Atom, Bond, BondIteratorPosition_, BondIteratorTraits_>
				BondConstReverseIterator;

			BondConstReverseIterator rbeginBond() const
			{
				return BondConstReverseIterator::begin(*this);
			}

			BondConstReverseIterator rendBond() const
			{
				return BondConstReverseIterator::end(*this);
			}

		//@}

		protected:

		private:

			void clear_();

			void swapLastBond_(const Atom* atom);

			// Attributes - names are self-explaining...
			Element* 	element_;
			float 		charge_;
			String 		name_;
			String 		type_name_;
			Vector3 	position_;
			float 		radius_;
			Type 			type_;
			Vector3 	velocity_;
			Vector3 	force_;

			unsigned char number_of_bonds_;
			Bond* bond_[MAX_NUMBER_OF_BONDS];
	};

# ifndef BALL_NO_INLINE_FUNCTIONS
#   include <BALL/KERNEL/atom.iC>
# endif

} // namespace BALL

#	ifndef BALL_KERNEL_BONDITERATOR_H
#		include <BALL/KERNEL/bondIterator.h>
#	endif


#endif // BALL_KERNEL_ATOM_H
