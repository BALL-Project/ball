// $Id: secondaryStructure.h,v 1.20 2001/02/24 22:52:33 amoll Exp $

#ifndef BALL_KERNEL_SECONDARYSTRUCTURE_H
#define BALL_KERNEL_SECONDARYSTRUCTURE_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_KERNEL_PDBATOM_H
#	include <BALL/KERNEL/PDBAtom.h>
#endif

#ifndef BALL_KERNEL_RESIDUE_H
#	include <BALL/KERNEL/residue.h>
#endif

#ifndef BALL_KERNEL_RESIDUEITERATOR_H
#	include <BALL/KERNEL/residueIterator.h>
#endif

namespace BALL 
{
	 
	class Protein;

	/**	Kernel Secondary Structure Class.
			This class is used to represent secondary structure elements
			of protein chains.\\
			{\bf Definition:}\URL{BALL/KERNEL/secondaryStructure.h}
	*/
	class SecondaryStructure
		: public AtomContainer
	{
		public:

		BALL_CREATE_DEEP(SecondaryStructure)


		/**	@name	Enums
		*/
		//@{

		/**	
		*/
		enum Property
		{
			PROPERTY__HELIX       = Fragment::NUMBER_OF_PROPERTIES + 1,
			PROPERTY__RANDOM_COIL = Fragment::NUMBER_OF_PROPERTIES,
			PROPERTY__STRAND      = Fragment::NUMBER_OF_PROPERTIES,
			PROPERTY__TURN        = Fragment::NUMBER_OF_PROPERTIES,

			NUMBER_OF_PROPERTIES
		};

		//@}
		/**	@name	Constructors and Destructors 
		*/
		//@{

		/// Default constructor
		SecondaryStructure()
			throw();
	
		/// Copy constructor
		SecondaryStructure(const SecondaryStructure& secondary_structure, bool deep = true)
			throw();
	
		/// Detailled constructor 
		SecondaryStructure(const String& name)
			throw();

		///	Destructor
		virtual ~SecondaryStructure()
			throw();

		/// Clears the contents of the secondary structure.
		virtual void clear()
			throw();
	
		/// Clears the contents of the secondary structure and removes it from all composite structures.
		virtual void destroy()
			throw();
	
		//@}

		/**	Equality operator.
				@see Object::operator ==
		*/
		bool operator == (const SecondaryStructure& secondary_structure) const
			throw();

		/**	Inequality operator
				@see operator ==
		*/
		bool operator != (const SecondaryStructure& secondary_structure) const
			throw();

		/** @name Persistence */
		//@{

		/**	Writes a SecondaryStructure object to a persistent stream.
				@param pm the persistence manager
		*/
		void persistentWrite(PersistenceManager& pm, const char* name = 0) const
			throw();

		/**	Reads a SecondaryStructure object from a persistent stream.
				@param pm the persistence manager
		*/
		void persistentRead(PersistenceManager& pm)
			throw();

		//@}
		/**	@name	Assignment 
		*/
		//@{

		/** Assignment with cloning facility.
				The assignment is either deep or shallow (default).
				@param  secondary_structure the SecondaryStructure to be copied (cloned)
				@param  deep make a deep (={\tt true}) or shallow (={\tt false}) copy of {\em secondary_structure}
		*/
		void set(const SecondaryStructure& secondary_structure, bool deep = true)
			throw();

		/** Assignment operator.
				@param   secondary_structure the SecondaryStructure to be copied (cloned)
				@return  Secondary_structure& - this instance
				@see     SecondaryStructure::set
		*/
		const SecondaryStructure& operator =(const SecondaryStructure& secondary_structure)
			throw();

		/** Copy this instance to {\em secondary_structure}.
				The assignment is either deep or shallow (default).
				@param  secondary_structure the SecondaryStructure to be assigned to
				@see    secondary_structure::set
		*/
		void get(SecondaryStructure& secondary_structure, bool deep = true) const
			throw();

		/** Swapping of two secondary structures.
				@param  secondary_structure to swap with this instance
		*/
		void swap(SecondaryStructure& secondary_structure)
			throw();

		//@}
		/**	@name Accessors 
		*/	
		//@{

		/** Get a pointer to the parent protein.
				The pointer is 0 if this instance does not have a parent protein.
				@return  Protein* - mutable pointer to the parent protein
		*/
		Protein* getProtein()
			throw();

		/** Get a pointer to the parent protein.
				The pointer is 0 if this instance does not have a parent protein.
				@return  Protein* - constant pointer to the parent protein
		*/
		const Protein* getProtein() const
			throw();

		/** Get a pointer to the parent chain.
				The pointer is 0 if this instance does not have a parent chain.
				@return  Protein* - mutable pointer to the parent chain
		*/
		Chain* getChain()
			throw();

		/** Get a pointer to the parent chain.
				The pointer is 0 if this instance does not have a parent chain.
				@return  Protein* - constant pointer to the parent chain
		*/
		const Chain* getChain() const
			throw();

		/** Get a pointer to a child Residue at a given position.
				The pointer is 0 if this instance does not have a Residue at the given position.
				@param   position the position of the child Residue
				@return  Residue* - mutable pointer to the child Residue at {\em position}
		*/
		Residue* getResidue(Position position)
			throw();
	
		/** Get a pointer to a child Residue at a given position.
				The pointer is 0 if this instance does not have a Residue at the given position.
				@param   position the position of the child Residue
				@return  Residue* - constant pointer to the child Residue at {\em position}
		*/
		const Residue* getResidue(Position position) const
			throw();

		/** Get a pointer to the N-terminal Residue.
				The pointer is 0 if this instance does not have a Residue with
				the property "PROPERTY__AMINO_ACID".
				@return  Residue* - mutable pointer to the N-terminal Residue
		*/
		Residue* getNTerminal()
			throw();
	
		/** Get a pointer to the N-terminal Residue.
				The pointer is 0 if this instance does not have a Residue with
				the property "PROPERTY__AMINO_ACID".
				@return  Residue* - constant pointer to the N-terminal Residue
		*/
		const Residue* getNTerminal() const
			throw();

		/** Get a pointer to the C-terminal Residue.
				The pointer is 0 if this instance does not have a Residue with
				the property "PROPERTY__AMINO_ACID".
				@return  Residue* - mutable pointer to the C-terminal Residue
		*/
		Residue* getCTerminal()
			throw();
	
		/** Get a pointer to the C-terminal Residue.
				The pointer is 0 if this instance does not have a Residue with
				the property "PROPERTY__AMINO_ACID".
				@return  Residue* - constant pointer to the C-terminal Residue
		*/
		const Residue* getCTerminal() const
			throw();

		/** Get a pointer to a child PDBAtom at a given position.
				The pointer is 0 if this instance residue does not have a PDBAtom at this position.
				@param   position the position of the child PDBAtom
				@return  PDBAtom* - constant pointer to the child PDBAtom at {\em position}
		*/
		PDBAtom* getPDBAtom(Position position)
			throw();
	
		/** Get a pointer to a child PDBAtom at a given position.
				The pointer is 0 if this instance residue does not have a PDBAtom at this position.
				@param   position the position of the child PDBAtom
				@return  PDBAtom* - mutable pointer to the child PDBAtom at {\em position}
		*/
		const PDBAtom* getPDBAtom(Position position) const
			throw();
	
		/** Count the Residues.
				@return  Size the number of residues
		*/
		Size countResidues() const
			throw();

		/** Count the PDBAtoms.
				@return  Size the number of PDBAtoms
		*/
		Size countPDBAtoms() const
			throw();

		/** Prepend a Residue at position 0.
				@param residue, the Residue to prepend
		*/
		void prepend(Residue& residue)
			throw();

		/** Append a Residue after the last position.
				@param residue, the Residue to append
		*/
		void append(Residue& residue)
			throw();

		/** Insert a Residue after the last position.
				@param residue, the Residue to insert
		*/
		void insert(Residue& residue)
			throw();

		/** Insert a Residue before a given Composite object.
				@param residue, the Residue to insert
				@param before, the Composite object to insert before
		*/
		void insertBefore(Residue& residue, Composite& before)
			throw();

		/** Insert a Residue after a given Composite object.
				@param residue, the Residue to insert
				@param after, the Composite object to insert before
		*/
		void insertAfter(Residue& residue, Composite& after)
			throw();

		/** Remove a Residue.
				@param residue the Residue to remove
		*/
		bool remove(Residue& residue)
			throw();

		/**	Move the children of {\tt secondary_structure} into this instance.
				Cut all children of {\tt secondary_structure} and prepend them before the children of this instance.
				@param secondary_structure the SecondaryStructure to access
		*/
		void spliceBefore(SecondaryStructure& secondary_structure)
			throw();

		/**	Move the children of {\tt secondary_structure} into this instance.
				Cut all children of {\tt secondary_structure} and append them after the children of this instance.
				@param secondary_structure the SecondaryStructure to access
		*/
		void spliceAfter(SecondaryStructure& secondary_structure)
			throw();

		/**	Move the children of {\tt secondary_structure} into this instance.
				The children are inserted using \Ref{spliceBefore}.
		*/
		void splice(SecondaryStructure& secondary_structure)
			throw();

		//@}
		/**	@name	Debugging and Diagnostics 
		*/
		//@{
	
		/** Internal state and consistency self-validation.
				@return	 bool - {\tt true} if the internal state is correct (self-validated) and consistent,
												{\tt false} otherwise
		*/
		virtual bool isValid() const
			throw();

		/** Internal state dump.
				Dump the current internal state of this instance to the output ostream {\em s} 
				with dumping depth {\em depth}.
				@param	s output stream where to output the internal state of this instance
				@param  depth the dumping depth
		*/
		virtual void dump(std::ostream& s = std::cout, Size depth = 0) const
			throw();

		//@}
		/** @name STORERS 
		*/
		//@{

		/* Persistent stream input and state restorage.
			 Read persistent data from the input stream {\em s} and 
			 restore the state of this instance. \\
			 {\bf Note:} Not yet implemented.
			 @param  s input stream from where to restore the internal state of this instance
		*/
		virtual void read(std::istream& s)
			throw();

		/* Persistent stream output and state storage.
			 Write persistent data to the output stream {\em s} and 
			 store the state of this instance. \\
			 {\bf Note:} Not yet implemented.	
			 @param  s input stream from where to restore the internal state of this instance
		*/
		virtual void write(std::ostream& s) const
			throw();

		//@}
			
		// --- EXTERNAL ITERATORS

		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(Residue)
		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(PDBAtom)


	private:

		Fragment* getFragment(Position position)
			throw();
	
		const Fragment* getFragment(Position position) const
			throw();
	
		Atom* getAtom(Position position)
			throw();
	
		const Atom* getAtom(Position position) const
			throw();

		void prepend(Atom &atom)
			throw();

		void append(Atom &atom)
			throw();

		void insert(Atom &atom)
			throw();

		void insertBefore(Atom &atom, Composite &before)
			throw();

		void insertAfter(Atom &atom, Composite &after)
			throw();

		bool remove(Atom &atom)
			throw();

		void prepend(AtomContainer &atom_container)
			throw();

		void append(AtomContainer &atom_container)
			throw();

		void insert(AtomContainer &atom_container)
			throw();
			
		void insertBefore(AtomContainer &atom_container, Composite &before)
			throw();

		void insertAfter(AtomContainer &atom_container, Composite &after)
			throw();

		void spliceBefore(AtomContainer &atom_container)
			throw();

		void spliceAfter(AtomContainer &atom_container)
			throw();

		void splice(AtomContainer &atom_container)
			throw();

		bool remove(AtomContainer &atom_container)
			throw();

		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(AtomContainer)

	};

} // namespace BALL

#endif // BALL_KERNEL_SECONDARYSTRUCTURE_H
