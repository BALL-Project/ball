// $Id: bond.h,v 1.17 2000/12/16 21:29:02 amoll Exp $

#ifndef BALL_KERNEL_BOND_H
#define BALL_KERNEL_BOND_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_CONCEPT_PROPERTY_H
#	include <BALL/CONCEPT/property.h>
#endif

#ifndef BALL_CONCEPT_COMPOSITE_H
#	include <BALL/CONCEPT/composite.h>
#endif

#ifndef BALL_DATATYPE_STRING_H
#	include <BALL/DATATYPE/string.h>
#endif


#define BALL_BOND_DEFAULT_FIRST_ATOM    0
#define BALL_BOND_DEFAULT_SECOND_ATOM   0
#define BALL_BOND_DEFAULT_NAME          ""
#define BALL_BOND_DEFAULT_ORDER         ORDER__UNKNOWN
#define BALL_BOND_DEFAULT_TYPE          TYPE__UNKNOWN


namespace BALL 
{

	class Atom;
	class AtomContainer;
	class Fragment;
	class System;

	/** The bond class of the kernel framework.
		
			{\bf Framework:} BALL/KERNEL\\
			{\bf Definition:} \URL{BALL/KERNEL/bond.h}
			\\
			{\bf Concept:} association\\
			
			An instance of Bond represents a chemical connectivity ("bond")
			between two \Ref{Atom} instances ("first atom" and "second atom").
			During each runtime instance of a program a bond is unique and identified by a \Ref{Object::Handle}.
			Bond equality is defined as bond identity.
			A linear ordering of bonds is defined as the linear order of the \Ref{Object::Handle}'s.
			There can exist only one bond between a first and a second atom that is shared by both of them.
			For the sake of maintainability the first atom "owns" the bond thus the bond is existentially dependent on the first atom
			whilst the second atom "owes" the bond from the first atom.
			Bonds and atoms define an implict molecular graph.
			The "state" of a bond is defined by its attributes:
			\begin{itemize}
				\item "first bonding (bond-owning) atom" (\Ref{Bond::first_})
				\item "second bonding (bond-owing) atom" (\Ref{Bond::second_})
				\item "bond name" (\Ref{Bond::name_})
				\item "bond order" (\Ref{Bond::bond_order_})
				\item "bond type" (\Ref{Bond::bond_type_})
			\end{itemize}
			
			@memo    Bond class (BALL kernel framework)
			@author  $Author: amoll $
			@version $Revision: 1.17 $
			@date    $Date: 2000/12/16 21:29:02 $
	*/
	class Bond
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
			
		/**	Too many bonds created.
				This exception may be thrown by \Ref{createBond} if the maximum number
				of bonds (8 bonds) is exceeded.
		*/
		class TooManyBonds
			:	public	Exception::GeneralException
		{
			public:
			TooManyBonds(const char* file, int line)
				throw();
		};

		/**	Not bounded to two atoms.
				This exception may be thrown by \Ref{getLength} if this instance is not bounded.
		*/
		class NotBounded
			:	public	Exception::GeneralException
		{
			public:
			NotBounded(const char* file, int line)
				throw();
		};

		//@}

		/** @name Type definitions */
		//@{

		/** Bond type.
		*/
		typedef short Order;
		typedef short Type;

		//@}  
	
		/** @name Enumerations */
		//@{
		
		/**	Predefined constants for the bond order */
		enum BondOrder
		{
			///
			ORDER__UNKNOWN        = 0,
			///
			ORDER__SINGLE         = 1,
			///
			ORDER__DOUBLE         = 2,
			///
			ORDER__TRIPLE         = 3,
			///
			ORDER__QUADRUPLE      = 4,
			///
			ORDER__AROMATIC       = 5,
			
			///
			NUMBER_OF_BOND_ORDERS
		};
			
		/**	Predefined constants for the bond type */
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
				Enumeration of all properties that are used by the BALL kernel.
		*/
		enum Property
		{
			NUMBER_OF_PROPERTIES
		};
		//@}  

		/** @name Constructors and Destructors */
		//@{

		/** Default constructor.
				Construct new bond.
				The state of {\em *this} bond is:
				\begin{itemize}
					\item bond has no connectivity with first atom (=0)
					\item bond has no connectivity with second atom (=0)
					\item bond name is the empty string (="")
					\item bond order is unknown (=\Ref{Bond::ORDER__UNKNOWN})
					\item bond type is unknown (=\Ref{Bond::TYPE__UNKNOWN})
				\end{itemize}

				@return    Bond - new constructed bond
		*/
		Bond()
			throw();

		/** Copy constructor.
				Construct new bond by copying the bond {\em bond}.
				Calls \Ref{Bond::createBond}.
				The state of {\em *this} bond is initialized to the state of the bond {\em bond}.\\
				\\
				{\bf Note:} Deep copying of bonds make no sense, the parameter {\bf deep} is therefore
				ignored.
				The use of this method is not recommended because it may result in inconcistencies
				of the whole system. It is used for backup only.

				@param  bond the bond to be copied (cloned)
				@param       deep make a deep (={\tt true}) or shallow (={\tt false}) copy of {\em bond}
				@return      Bond - new constructed bond cloned from {\em bond}
				@see         createBond
		*/
		Bond(const Bond& bond, bool deep = true)
			throw();
	
		/** Detailed state initializing constructor.
				Construct new bond.
				The state of {\em *this} bond is:
				\begin{itemize}
					\item bond has connectivity with first atom (={\em first})
					\item bond has connectivity with second atom (={\em second})
					\item bond name is {\em name}
					\item bond order is {\em order}
					\item bond type is {\em type}
				\end{itemize}

				@param       name name of the constructed bond
				@param       first first atom of the constructed bond
				@param       second second atom of the constructed bond
				@param       order order of the constructed bond
				@param       type type of the constructed bond
				@return      Bond - new constructed bond
		*/
		 Bond(const String& name, Atom& first, Atom& second, Order order = BALL_BOND_DEFAULT_ORDER,
				 Type type = BALL_BOND_DEFAULT_TYPE)
			throw();

		/** Global bond creation.
				Connect the first atom {\em first} to the second atom {\em second} via the bond {\em bond}.
				The state of the bond {\em bond} is:
				\begin{itemize}
					\item bond has connectivity with first atom (={\em first})
					\item bond has connectivity with second atom (={\em second})
				\end{itemize}

				@param 	bond the instantiated bond that connects the first atom {\em first}
								to the second atom {\em second}
				@param 	first the first atom of the bond
				@param 	second the second atom of the bond
				@return Bond* - bond that connects the first atom {\em first} with the second atom {\em second}
		*/
		static Bond* createBond(Bond& bond, Atom& first, Atom& second)
			throw(TooManyBonds);

		/** Destructor.
				Default destruction of {\em *this} bond.
				If the bond is connecting two atoms, they are disconnected.
		*/
		virtual ~Bond()
			throw();

		/** Explicit default initialization.
				Set the state of {\em *this} bond to the default values.
				The state of {\em *this} bond is:
				\begin{itemize}
					\item bond has no connectivity with first atom (=0)
					\item bond has no connectivity with second atom (=0)
					\item bond name is the empty string (="")
					\item bond order is unknown (=\Ref{Bond::ORDER__UNKNOWN})
					\item bond type is unknown (=\Ref{Bond::TYPE__UNKNOWN})
				\end{itemize}
		*/
		virtual void clear()
			throw();

		/** Explicit destructor.
				Destroy {\em *this} bond explicitly without releasing its heap memory thus
				{\em *this} bond may exist further.	Calls \Ref{Bond::clear}.
				Set the state of {\em *this} bond to the default values. 
				The state of {\em *this} bond is:
				\begin{itemize}
					\item bond has no connectivity with first atom (=0)
					\item bond has no connectivity with second atom (=0)
					\item bond name is the empty string (="")
					\item bond order is unknown (=\Ref{Bond::ORDER__UNKNOWN})
					\item bond type is unknown (=\Ref{Bond::TYPE__UNKNOWN})
				\end{itemize}
				{\bf Note:} Destroy is equivalent to \Ref{Bond::clear}.

				@see Bond::clear
		*/
		virtual void destroy()
			throw();
		//@}

		/**	@name Persistence */
		//@{
	
		/**	Persistent writing.
				Writes a Bond object to a persistent stream.
				@param pm the persistence manager
		*/
		void persistentWrite(PersistenceManager& pm, const char* name = 0) const
			throw();

		/**	Persistent reading.
				Reads a Bond object from a persistent stream.
				@param pm the persistence manager
		*/
		void persistentRead(PersistenceManager& pm)
			throw();

		/**	Finalize the deserialization.
				Bond might have to swap {\tt first_} and {\tt second_}.
		*/
		void finalize()
			throw();

		//@}


		/** @name Assignment methods */
		//@{

		/** Assignment with cloning facility.
				Assign the bond {\em bond} to {\em *this}.
				The assignment is either deep or shallow (default).
				The state of {\em *this} bond is initialized to the state of the bond {\em bond}.\\
				\\
				{\bf Note:} Deep copying of bonds is not supported.
				The use of this method is not recommended because it may result in inconcistencies
				of the whole system. It is used for backup only.

				@param bond the bond to be copied (cloned)
				@param deep make a deep (={\tt true}) or shallow (={\tt false}) copy of {\em bond}
		*/
		void set(const Bond& bond, bool deep = true)
			throw();
		
		/** Assignment operator.
				Assign the bond {\em bond} to {\em *this}.
				The assignment is either deep or shallow (default).
				Calls \Ref{Bond::set}.
				The state of {\em *this} bond is initialized to the state of the bond {\em bond}.\\
				\\
				{\bf Note:} Deep copying of bonds is not supported.
				The use of this method is not recommended because it may result in inconcistencies
				of the whole system. It is used for backup only.

				@param  bond the bond to be copied (cloned)
				@return Bond - {\em *this} bond
				@see    Bond::set
		*/
		const Bond& operator = (const Bond& bond)
			throw();

		/** Copying with cloning facility.
				Copy {\em *this} to the bond {\em bond}.
				The assignment is either deep or shallow (default).
				Calls \Ref{Bond::set}.
				The state of the bond {\em bond} is initialized to the state of {\em *this}.\\
				\\
				{\bf Note:} Deep copying of bonds is not supported.
				The use of this method is not recommended because it may result in inconcistencies 
				of the whole system. It is used for backup only.

				@param bond the bond to be assigned to
				@see   Bond::set
		*/
		void get(Bond& bond, bool deep = true) const
			throw();

		/** Swapping of bonds.
				Swap the states of {\em *this} with the bond {\em bond}.

				@param bond the bond {\em *this} is being swapped with
				@see   Bond::Bond
		*/
		void swap(Bond& bond)
			throw();
	
		//@}
	
		/** @name Accessors: inspectors and mutators */
		//@{ 

		/** Set the first Atom.
				@param atom the atom to set
		*/
		void setFirstAtom(Atom* atom)
			throw();
	
		/** Mutable inspection of the first bond atom.
				Access a mutable reference to the first atom of {\em *this} bond.
				The reference is 0 if no first atom is connected to {\em *this} bond.\\
				\\
				{\bf Note:} No corresponding mutator Bond::setFirstAtom exists to provide consistency 
										of the bond tables in the atoms.
				@return      Atom* -
										 mutable reference to the first atom that is connected to {\em *this} bond,
										 0 if no first atom exists
				@see         Bond::getSecondAtom
		*/
		Atom* getFirstAtom()
			throw();
	 
		/** Constant inspection of the first bond atom.
				Access a constant reference to the first atom of {\em *this} bond.
				The reference is 0 if no first atom is connected to {\em *this} bond.\\
				\\
				{\bf Note:} No corresponding mutator Bond::setFirstAtom exists to provide consistency 
										of the bond tables in the atoms.
				@return      Atom* - 
										 constant reference to the first atom that is connected to {\em *this} bond,
											 0 if no first atom exists
				@see         Bond::getSecondAtom
		*/
		const Atom* getFirstAtom() const
			throw();
	 
		/**	Return the partner atom of an atom.
				If the given {\tt atom} is part of this bond, the other atom
				of the bond is returned. 0 is returned if {\tt atom} is not
				one of the bond's atoms.
				@param	atom one of the bond's atoms
				@return	the atom's bond partner
		*/
		Atom* getPartner(const Atom& atom) const
			throw();

		/** Set the second Atom.
				@param atom the atom to set
		*/
		void setSecondAtom(Atom* atom)
			throw();
	
		/** Mutable inspection of the second bond atom.
				Access a mutable reference to the second atom of {\em *this} bond.
				The reference is 0 if no second atom is connected to {\em *this} bond.\\
				\\
				{\bf Note:} No corresponding mutator Bond::setSecondAtom exists to provide consistency
										of the bond tables in the atoms.
				@return      Atom* - 
										 mutable reference to the second atom that is connected to {\em *this} bond,
										 0 if no second atom exists
				@see         Bond::getFirstAtom
		*/
		Atom* getSecondAtom()
			throw();
	 
		/** Constant inspection of the second bond atom.
				Access a constant reference to the second atom of {\em *this} bond.
				The reference is 0 if no second atom is connected to {\em *this} bond.\\
				\\
				{\bf Note:} No corresponding mutator Bond::setSecondAtom exists to provide consistency
										of the bond tables in the atoms.
				@return      Atom* -
										 constant reference to the second atom that is connected to {\em *this} bond,
										 0 if no second atom exists
				@see         Bond::getFirstAtom
		*/
		const Atom* getSecondAtom() const
			throw();

		/** Change of the bond's name.
				Change the name of {\em *this} bond to {\em name}.
				@param name the new name of {\em *this} bond
				@see         Bond::getName
		*/
		void setName(const String& name)
			throw();

		/** Constant inspection of the bond's name.
				Access a constant reference to the name of {\em *this} bond.
				@return      String& - constant reference to the name of {\em *this} bond
				@see         Bond::setName
		*/
		const String& getName() const
			throw();

		/** Change of the bond's order.
				Change the order of {\em *this} bond to the order {\em bond_order}.
				@param       bond_order the new order of {\em *this} bond
				@see         Bond::getOrder
		*/
		void setOrder(Order bond_order)
			throw();
	
		/** Constant inspection of the bond's order.
				Access the order of {\em *this} bond.
				@return      Order - copy of the order of {\em *this} bond
				@see         Bond::setOrder
		*/
		Order getOrder() const
			throw();
	
		/** Change of the bond's type.
				Change the type of {\em *this} bond to the type {\em bond_type}.
				@param       bond_type the new type of {\em *this} bond
				@see         Bond::getType
		*/
		void setType(Type bond_type)
			throw();
	
		/** Constant inspection of the bond's type.
				Access the type of {\em *this} bond.
				@return      Type - copy of the type of {\em *this} bond
				@see         Bond::setType
		*/
		Type getType() const
			throw();
	
		/** Constant inspection of the bond's length.
				Access the length of {\em *this} bond.
				@exception NotBounded if the bond has not two atoms
				@return      Real - copy of the length of {\em *this} bond
		*/
		Real getLength() const
			throw(NotBounded);

		/** Global mutable inspection of a bond.
				Access the bond that might connect the atom {\em first} with the atom {\em second}.
				If no such bond exists 0 is returned. The order of atoms may be arbitrary. Calls \Ref{Atom::getBond}.
				@return      Bond* -
										 mutable reference to the bond connecting the atom {\em first} and {\em second},
										 0 if no such bond exists
				@see         Atom::getBond
		*/
		static Bond* getBond(Atom& first, Atom& second)
			throw();

		/** Mutable inspection of the bound atom
				It is tested, if {\em *this} bond connects the two atoms.
				if this is true a pointer to the bound atom is returned.
				@param			 atom , its partner is looked for
				@return      mutable reference to the bound atom
										 0 if the atom has no bound atom
		*/
		Atom* getBoundAtom(const Atom& atom)
			throw();

		/** Constant inspection of the bound atom
				It is tested, if {\em *this} bond connects the two atoms.
				if this is true a pointer to the bound atom is returned.
				@param			 atom , its partner is looked for
				@return      constant reference to the bound atom
										 0 if the atom has no bound atom
		*/
		const Atom* getBoundAtom(const Atom& atom) const
			throw();

		//@}

		/** @name Predicates */
		//@{ 

		/** Determine whether the bond belongs to an atom.
				Query, if {\em *this} bond connects the atom {\em atom} with another atom.
				If such a bond exists {\tt true} is returned, {\tt false} otherwise.
				Calls \Ref{Atom::hasBond}.
				@param atom the atom that is queried to connect {\em *this} bond to another atom
				@return  bool -
								 {\tt true} if bond connects the atom {\em atom} with another atom,
								 {\tt false} otherwise
				@see     Atom::hasBond
		*/
		bool isBondOf(const Atom& atom) const
			throw();

		/** Determine whether this bond contains any atom.
				Query, if {\em *this} bond connects an atom.
				If such a bond exists {\tt true} is returned, {\tt false} otherwise.
				@return      bool -
										 {\tt true} if bond connects the atom {\em atom} with another atom,
										 {\tt false} otherwise
				@see         Atom::hasBond
		*/
		bool isBound() const
			throw();

		/** Determine whether the bond connects two fragments.
				Query, if {\em *this} bond connects its two atoms within a common parent \Ref{Composite} instance.
				If {\em *this} bond is intermolecular {\tt true} is returned, {\tt false} otherwise.
				Calls \Ref{Composite::getRoot}.
				@return      bool -
										 {\tt true} if {\em *this} bond is intermolecular
										 {\tt false} otherwise
				@see         Composite::getRoot
		*/
		bool isInterBond() const
			throw();

		/** Request for the intermolecular bonding of {\em *this} bond within {\em atom_container}.
				Query, if {\em *this} bond connects its two atoms within the common parent {\em atom_container} instance.
				If {\em *this} bond is intermolecular {\tt true} is returned, {\tt false} otherwise.
				Calls \Ref{Composite::isDescendantOf}.
				@param  	atom_container the queried parent fragment.
				@return  	bool -
									{\tt true} if {\em *this} bond is intermolecular
									{\tt false} otherwise
				@see      Composite::isDescendantOf
		*/
		bool isInterBondOf(const AtomContainer& atom_container) const
			throw();

		/** Request for the intermolecular bonding of {\em *this} bond within {\em system}.
				Query, if {\em *this} bond connects its two atoms within the common parent {\em system} instance.
				If {\em *this} bond is intermolecular {\tt true} is returned, {\tt false} otherwise.
				Calls \Ref{Composite::isDescendantOf}.
	
				@param 	system the queried parent system.
				@return	bool -
								{\tt true} if {\em *this} bond is intermolecular
								{\tt false} otherwise
				@see    Composite::isDescendantOf
		*/
		bool isInterBondOf(const System& system) const
			throw();

		/**	Request for the intramolecular bonding of {\em *this} bond.
				Query, if {\em *this} bond connects its two atoms within a common parent \Ref{Composite} instance.
				If {\em *this} bond is intramolecular {\tt true} is returned, {\tt false} otherwise.
				Calls \Ref{Composite::getRoot}.
	
				@return      bool -
										 {\tt true} if {\em *this} bond is intramolecular
										 {\tt false} otherwise
				@see         Composite::getRoot
		*/
		bool isIntraBond() const
			throw();

		/** Request for the intramolecular bonding of {\em *this} bond within {\em atom_container}.
				Query, if {\em *this} bond connects its two atoms within the common parent {\em atom_container} instance.
				If {\em *this} bond is intramolecular {\tt true} is returned, {\tt false} otherwise.
				Calls \Ref{Composite::isDescendantOf}.
	
				@param       atom_container the queried parent fragment.
				@return      bool -
										 {\tt true} if {\em *this} bond is intramolecular
										 {\tt false} otherwise
				@see         Composite::isDescendantOf
		*/
		bool isIntraBondOf(const AtomContainer& atom_container) const
			throw();

		/** Request for the intramolecular bonding of {\em *this} bond within {\em system}.
				Query, if {\em *this} bond connects its two atoms within the common parent {\em system} instance.
				If {\em *this} bond is intramolecular {\tt true} is returned, {\tt false} otherwise.
				Calls \Ref{Composite::isDescendantOf}.
	
				@param 		system the queried parent system.
				@return   bool -
									{\tt true} if {\em *this} bond is intramolecular
									{\tt false} otherwise
				@see      Composite::isDescendantOf
		*/
		bool isIntraBondOf(const System& system) const
			throw();

		//@}

		/** @name Debuggers and diagnostics */
		//@{ 

		/** Internal state and consistency self-validation.
				Initiate self-validation of the internal state and data structure consistencies of {\em *this} bond.
				If the internal state of {\em *this} bond is correct (self-validated) and consistent {\tt true} 
				is returned,{\tt false} otherwise. 
	
				@return      bool -
										 {\tt true} if the internal state of {\em *this} bond is correct (self-validated)
										 and consistent, {\tt false} otherwise
		*/
		virtual bool isValid() const
			throw();

		/** Internal state dump.
				Dump the current internal state of {\em *this} bond to the output ostream {\em s}
				with dumping depth {\em depth}.
	
				@param	s output stream where to output the internal state of {\em *this} bond
				@param  depth the dumping depth
		*/
		virtual void dump(std::ostream& s = std::cout, Size depth = 0) const
			throw();
	
		//@}

		protected:
		
		private:

			void arrangeBonds_()
				throw();

			void clear_()
				throw();

			//_ first atom of the bond (bond owner)
			Atom*		first_;

			//_ second atom
			Atom*		second_;

			//_ bond name
			String	name_;

			//_ bond order
			Order		bond_order_;

			//_ bond type
			Type		bond_type_;
	};

} // namespace BALL

#endif // BALL_KERNEL_BOND_H
