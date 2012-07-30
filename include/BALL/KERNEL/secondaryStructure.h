// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_KERNEL_SECONDARYSTRUCTURE_H
#define BALL_KERNEL_SECONDARYSTRUCTURE_H

#ifndef BALL_KERNEL_RESIDUE_H
#	include <BALL/KERNEL/residue.h>
#endif

#ifndef BALL_KERNEL_RESIDUEITERATOR_H
#	include <BALL/KERNEL/residueIterator.h>
#endif

#ifndef BALL_KERNEL_ATOMCONTAINER_H
# include <BALL/KERNEL/atomContainer.h>
#endif

namespace BALL 
{
	/**	Secondary structure class.
			This class is used to represent secondary structure elements
			of protein chains.
			 \par
			
	 		\ingroup KernelMiscellaneous 
	*/
	class BALL_EXPORT SecondaryStructure
		: public AtomContainer
	{
		public:

		BALL_CREATE_DEEP(SecondaryStructure)

		/**	@name	Enums
		*/
		//@{

		/**	Secondary structure type
		*/
		enum Type
		{
			///
			HELIX,
			///
			COIL,
			///
			STRAND,
			///
			TURN,
			///
			UNKNOWN,
			///
			NUMBER_OF_TYPES
		};

		//@}


		/**	@name	Constructors and Destructors 
		*/
		//@{

		/// Default constructor
		SecondaryStructure();
	
		/// Copy constructor
		SecondaryStructure(const SecondaryStructure& secondary_structure, bool deep = true);
	
		///
		SecondaryStructure(const String& name);

		///	Destructor
		virtual ~SecondaryStructure();

		/// Clear the contents of the secondary structure
		virtual void clear();
	
		/// Clear the contents of the secondary structure and remove it from all composite structures.
		virtual void destroy();
		//@}

		/**	@name Predicates
		*/
		//@{
		/**	Equality operator.
				Two instance of SecondaryStructure are equal if they have the same handle.
				@see Object::operator ==
		*/
		bool operator == (const SecondaryStructure& secondary_structure) const;

		/**	Inequality operator
				@see operator ==
		*/
		bool operator != (const SecondaryStructure& secondary_structure) const;
		//@}

		/** @name Persistence 
		*/
		//@{

		/**	Write a SecondaryStructure object to a persistent stream
		 *	@param pm the persistence manager
		 */
		void persistentWrite(PersistenceManager& pm, const char* name = 0) const;

		/**	Read a SecondaryStructure object from a persistent stream
		 *	@param pm the persistence manager
		 */
		void persistentRead(PersistenceManager& pm);
		//@}

		/**	@name	Assignment 
		*/
		//@{

		/** Assignment.
				The assignment is either deep or shallow (default).
				@param  secondary_structure the SecondaryStructure to be copied (cloned)
				@param  deep make a deep (=<tt>true</tt>) or shallow (=<tt>false</tt>) copy
		*/
		void set(const SecondaryStructure& secondary_structure, bool deep = true);

		/** Assignment operator.
				@param   secondary_structure the SecondaryStructure to be copied (cloned)
				@return  Secondary_structure& - this instance
				@see     SecondaryStructure::set
		*/
		SecondaryStructure& operator = (const SecondaryStructure& secondary_structure);

		/** Assignment.
				The assignment is either deep or shallow (default).
				@param  secondary_structure the SecondaryStructure to be assigned to
				@see    secondary_structure::set
		*/
		void get(SecondaryStructure& secondary_structure, bool deep = true) const;

		/** Swapping of two secondary structures.
				@param  secondary_structure to swap with this instance
		*/
		void swap(SecondaryStructure& secondary_structure);
		//@}

		/**	@name Accessors 
		*/	
		//@{

		/// Return the type of secondary structure
		Type getType() const { return type_; }

		/// Set the type of secondary structure
		void setType(Type type) { type_ = type; }
			
		/** Get a pointer to the parent protein.
				The pointer is 0 if this instance does not have a parent protein.
				@return  Protein* - mutable pointer to the parent protein
		*/
		Protein* getProtein();

		/** Get a pointer to the parent protein.
				The pointer is 0 if this instance does not have a parent protein.
				@return  Protein* - constant pointer to the parent protein
		*/
		const Protein* getProtein() const;

		/** Get a pointer to the parent chain.
				The pointer is 0 if this instance does not have a parent chain.
				@return  Protein* - mutable pointer to the parent chain
		*/
		Chain* getChain();

		/** Get a pointer to the parent chain.
				The pointer is 0 if this instance does not have a parent chain.
				@return  Protein* - constant pointer to the parent chain
		*/
		const Chain* getChain() const;

		/** Get a pointer to a child Residue at a given position.
				The pointer is 0 if this instance does not have a Residue at the given position.
				@param   position the position of the child Residue
				@return  Residue* - mutable pointer to the child
		*/
		Residue* getResidue(Position position);
	
		/** Get a pointer to a child Residue at a given position.
				The pointer is 0 if this instance does not have a Residue at the given position.
				@param   position the position of the child Residue
				@return  Residue* - constant pointer to the child
		*/
		const Residue* getResidue(Position position) const;

		/** Get a pointer to the N-terminal Residue.
				The pointer is 0 if this instance does not have a Residue with
				the property "PROPERTY__AMINO_ACID".
				@return  Residue* - mutable pointer to the N-terminal Residue
		*/
		Residue* getNTerminal();
	
		/** Get a pointer to the N-terminal Residue.
				The pointer is 0 if this instance does not have a Residue with
				the property "PROPERTY__AMINO_ACID".
				@return  Residue* - constant pointer to the N-terminal Residue
		*/
		const Residue* getNTerminal() const;

		/** Get a pointer to the C-terminal Residue.
				The pointer is 0 if this instance does not have a Residue with
				the property "PROPERTY__AMINO_ACID".
				@return  Residue* - mutable pointer to the C-terminal Residue
		*/
		Residue* getCTerminal();
	
		/** Get a pointer to the C-terminal Residue.
				The pointer is 0 if this instance does not have a Residue with
				the property "PROPERTY__AMINO_ACID".
				@return  Residue* - constant pointer to the C-terminal Residue
		*/
		const Residue* getCTerminal() const;

		/** Get a pointer to a child PDBAtom at a given position.
				The pointer is 0 if this instance residue does not have a PDBAtom at this position.
				@param   position the position of the child PDBAtom
				@return  PDBAtom* - constant pointer to the child
		*/
		PDBAtom* getPDBAtom(Position position);
	
		/** Get a pointer to a child PDBAtom at a given position.
				The pointer is 0 if this instance residue does not have a PDBAtom at this position.
				@param   position the position of the child PDBAtom
				@return  PDBAtom* - mutable pointer to the child
		*/
		const PDBAtom* getPDBAtom(Position position) const;
	
		/** Count the Residues.
				@return  Size the number of residues
		*/
		Size countResidues() const;

		/** Count the PDBAtoms.
				@return  Size the number of PDBAtoms
		*/
		Size countPDBAtoms() const;

		/** Prepend a Residue at position 0.
				@param residue the Residue to prepend
		*/
		void prepend(Residue& residue);

		/** Append a Residue after the last position.
				@param residue the Residue to append
		*/
		void append(Residue& residue);

		/** Insert a Residue after the last position.
				@param residue the Residue to insert
		*/
		void insert(Residue& residue);

		/** Insert a Residue before a given Composite object.
				@param residue the Residue to insert
				@param before the Composite object to insert before
		*/
		void insertBefore(Residue& residue, Composite& before);

		/** Insert a Residue after a given Composite object.
				@param residue the Residue to insert
				@param after the Composite object to insert before
		*/
		void insertAfter(Residue& residue, Composite& after);

		/** Remove a Residue.
				@param residue the Residue to remove
		*/
		bool remove(Residue& residue);

		/**	Move the children of <tt>secondary_structure</tt> into this instance.
				Cut all children of <tt>secondary_structure</tt> and prepend them before the children of this instance.
				@param secondary_structure the SecondaryStructure to access
		*/
		void spliceBefore(SecondaryStructure& secondary_structure);

		/**	Move the children of <tt>secondary_structure</tt> into this instance.
				Cut all children of <tt>secondary_structure</tt> and append them after the children of this instance.
				@param secondary_structure the SecondaryStructure to access
		*/
		void spliceAfter(SecondaryStructure& secondary_structure);

		/**	Move the children of <tt>secondary_structure</tt> into this instance.
				The children are inserted using  \link spliceBefore spliceBefore \endlink .
		*/
		void splice(SecondaryStructure& secondary_structure);
		//@}

		/**	@name	Debugging and Diagnostics 
		*/
		//@{
	
		/** Internal state and consistency self-validation.
				@return	 bool - <tt>true</tt> if the internal state is correct (self-validated) and consistent, 
												<tt>false</tt> otherwise
		*/
		virtual bool isValid() const;

		/** Internal state dump.
				Dump the current internal state of this instance to the output ostream <b>  s </b> 
				with dumping depth <b>  depth </b>.
				@param	s output stream where to output the internal state of this instance
				@param  depth the dumping depth
		*/
		virtual void dump(std::ostream& s = std::cout, Size depth = 0) const;

		//@}
			
		// --- EXTERNAL ITERATORS

		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(Residue)
		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(PDBAtom)

		BALL_DECLARE_STD_ITERATOR_WRAPPER(SecondaryStructure, Residue, residues)
		BALL_DECLARE_STD_ITERATOR_WRAPPER(SecondaryStructure, PDBAtom, pdbAtoms)

		protected:
		
		/// The secondary structure type (helix, strand, coil, turn)
		Type type_;

		private:

		Fragment* getFragment(Position position);
	
		const Fragment* getFragment(Position position) const;
	
		Atom* getAtom(Position position);
	
		const Atom* getAtom(Position position) const;

		void prepend(Atom& atom);

		void append(Atom& atom);

		void insert(Atom& atom);

		void insertBefore(Atom& atom, Composite& before);

		void insertAfter(Atom& atom, Composite& after);

		bool remove(Atom& atom);

		void prepend(AtomContainer& atom_container);

		void append(AtomContainer& atom_container);

		void insert(AtomContainer& atom_container);
			
		void insertBefore(AtomContainer& atom_container, Composite& before);

		void insertAfter(AtomContainer& atom_container, Composite& after);

		void spliceBefore(AtomContainer& atom_container);

		void spliceAfter(AtomContainer& atom_container);

		void splice(AtomContainer& atom_container);

		bool remove(AtomContainer& atom_container);

		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(AtomContainer)
		BALL_DECLARE_STD_ITERATOR_WRAPPER(SecondaryStructure, AtomContainer, atomContainers)
	};

} // namespace BALL

#endif // BALL_KERNEL_SECONDARYSTRUCTURE_H
