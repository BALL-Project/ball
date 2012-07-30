// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_KERNEL_PROTEIN_H
#define BALL_KERNEL_PROTEIN_H

#ifndef BALL_KERNEL_CHAINITERATOR_H
#	include <BALL/KERNEL/chainIterator.h>
#endif

#ifndef BALL_KERNEL_MOLECULE_H
#	include <BALL/KERNEL/molecule.h>
#endif

#define BALL_PROTEIN_DEFAULT_ID   ""

namespace BALL 
{
	/**	Protein Class.
			This class is used to represent proteins.
			A protein may contain several  \link Chain Chain \endlink ,  \link SecondaryStructure SecondaryStructure \endlink ,
			and  \link Residue Residue \endlink .
			 \par
			
    	\ingroup KernelContainers 
	*/
	class BALL_EXPORT Protein
		: public Molecule
	{
		public:

		BALL_CREATE_DEEP(Protein)

		/**	@name	Enums
		*/
		//@{
			
		/**
		*/
		enum Property
		{
			NUMBER_OF_PROPERTIES = Molecule::NUMBER_OF_PROPERTIES
		};

		//@}
		/** @name	Constructors and Destructors 
		*/
		//@{

		/// Default constructor
		Protein();
	
		/// Copy constructor
		Protein(const Protein& protein, bool deep = true);
	
		/// Detailled constructor 
		Protein(const String& name, const String& id = BALL_PROTEIN_DEFAULT_ID);

		///	Destructor
		virtual ~Protein();
	
		/// Clears the contents of the protein
		virtual void clear();
	
		/* Clears the contents of the protein and removes it from all composite structures.
		*/
		virtual void destroy();

		//@}
		/**	@name	Persistence 
		*/
		//@{
		
		/**	Writes a Protein object to a persistent stream.
		 *	@param pm the persistence manager
		 */
		void persistentWrite(PersistenceManager& pm, const char* name = 0) const;

		/**	Reads a Protein object from a persistent stream.
		 *	@param pm the persistence manager
		 */
 		void persistentRead(PersistenceManager& pm);

		//@}
		/**	@name Assignment Methods 
		*/
		//@{

		/** Assignment with cloning facility.
				The assignment is either deep or shallow (default).
				@param  protein the Protein to be copied (cloned)
				@param  deep make a deep (=<tt>true</tt>) or shallow (=<tt>false</tt>) copy
		*/
		void set(const Protein& protein, bool deep = true);

		/** Assignment operator.
				@param   protein the Protein to be copied (cloned)
				@return  Protein& - this instance
		*/
		Protein& operator = (const Protein& protein);

		/** Copy this instance to <b>  protein </b>.
				The assignment is either deep or shallow (default).
				@param  protein the Protein to be assigned to
		*/
		void get(Protein& protein, bool deep = true) const;
	
		/** Swapping of two proteins.
				@param  protein to swap with this instance
		*/
		void swap(Protein& protein);
	
		//@}

		/**	Equality operator.
				Two proteins are equal if they have the same handle.
				@see Object::operator ==.
		*/
		bool operator == (const Protein& protein) const;

		/**	Inequality operator
				@see operator ==
		*/
		bool operator != (const Protein& protein) const;

		/**	@name Accessors 
		*/
		//@{

		/** Get a pointer to a child chain at a given position.
				The pointer is 0 if this instance does not have a chain at the given position.
				@param   position the position of the child chain
				@return  Chain* - mutable pointer to the child
		*/
		Chain* getChain(Position position);

		/** Get a pointer to a child chain at a given position.
				The pointer is 0 if this instance does not have a chain at the given position.
				@param   position the position of the child chain
				@return  Chain* - constant pointer to the child
		*/
		const Chain* getChain(Position position) const;

		/** Get a pointer to a child SecondaryStructure at a given position.
				The pointer is 0 if this instance does not have a SecondaryStructure at the given position.
				@param   position the position of the child SecondaryStructure
				@return  SecondaryStructure* - mutable pointer to the child
		*/
		SecondaryStructure* getSecondaryStructure(Position position);

		/** Get a pointer to a child SecondaryStructure at a given position.
				The pointer is 0 if this instance does not have a SecondaryStructure at the given position.
				@param   position the position of the child SecondaryStructure
				@return  SecondaryStructure* - constant pointer to the child
		*/
		const SecondaryStructure* getSecondaryStructure(Position position) const;

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

		/** Get a pointer to a child Residue with the given ID.
		The pointer is 0 if this instance does not have a Residue with the given ID.
		@param   residue_ID the ID of the residue
		@return  Residue* - pointer to the child
		*/
		Residue* getResidueByID(String residue_ID);

		/** Get a pointer to a child Residue with the given ID.
		The pointer is 0 if this instance does not have a Residue with the given ID.
		@param   residue_ID the ID of the residue
		@return  Residue* - constant pointer to the child
		*/
		const Residue* getResidueByID(String residue_ID) const;

		/** Get a pointer to the N-terminal Residue.
				The pointer is 0 if this instance does not have a Residue with
				the property "PROPERTY__AMINO_ACID".
				@return  Residue* - mutable pointer to the N-terminal	Residue
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
				@return  Residue* - mutable pointer to the C-terminal	Residue
		*/
		Residue* getCTerminal();

		/** Get a pointer to the C-terminal Residue.
				The pointer is 0 if this instance does not have a Residue with
				the property "PROPERTY__AMINO_ACID".
				@return  Residue* - constant pointer to the C-terminal Residue
		*/
		const Residue* getCTerminal() const;

		/** Get a pointer to a child PDB-Atom at a given position.
				The pointer is 0 if this instance does not have a PDB-Atom at the given position.
				@param   position the position of the child PDB-Atom
				@return  Residue* - mutable pointer to the child
		*/
		PDBAtom* getPDBAtom(Position position);

		/** Get a pointer to a child PDB-Atom at a given position.
				The pointer is 0 if this instance does not have a PDB-Atom at the given position.
				@param   position the position of the child PDB-Atom
				@return  Residue* - constant pointer to the child
		*/
		const PDBAtom* getPDBAtom(Position position) const;

		/**	Set the ID of the NucleicAcid.
				@param id the new ID
		*/
		void setID(const String& id);

		/**	Retrieve the ID of the NucleicAcid.
				@return String the ID
		*/
		const String& getID() const;

		/** Count the child chains.
				@return Size the number of chains
		*/
		Size countChains() const;

		/** Count the child SecondaryStructures.
				@return Size the number of SecondaryStructures
		*/
		Size countSecondaryStructures() const;

		/** Count the child Residues.
				@return Size the number of Residues
		*/
		Size countResidues() const;

		/** Count the child PDB-Atoms.
				@return Size the number of PDB-Atoms
		*/
		Size countPDBAtoms() const;

		//@}

		using Molecule::prepend;
		using Molecule::append;
		using Molecule::insert;
		using Molecule::insertBefore;
		using Molecule::insertAfter;
		using Molecule::remove;
		using Molecule::splice;
		using Molecule::spliceBefore;
		using Molecule::spliceAfter;

		/** @name Type predicates */
		//@{
		/// @copydoc Composite::isProtein
		virtual bool isProtein() const { return true; }
		//@}

		/**	@name	Debugging and Diagnostics
		*/
		//@{

		/** Internal state and consistency self-validation.
				@return	 bool -<tt>true</tt> if the internal state of this instance is 
								 correct (self-validated) and consistent, <tt>false</tt> otherwise
		*/
		virtual bool isValid() const;

		/** Internal state dump.
				Dump the current internal state  to the output 
				ostream <b>  s </b> with dumping depth <b>  depth </b>.
				@param	s output stream where to output the internal state 
				@param  depth the dumping depth
		*/
		virtual void dump(std::ostream& s = std::cout, Size depth = 0) const;

		//@}

		// --- EXTERNAL ITERATORS

		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(Chain)
		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(SecondaryStructure)
		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(Residue)
		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(PDBAtom)

		BALL_DECLARE_STD_ITERATOR_WRAPPER(Protein, Chain, chains)
		BALL_DECLARE_STD_ITERATOR_WRAPPER(Protein, SecondaryStructure, secondaryStructures)
		BALL_DECLARE_STD_ITERATOR_WRAPPER(Protein, Residue, residues)
		BALL_DECLARE_STD_ITERATOR_WRAPPER(Protein, PDBAtom, pdbAtoms)

		private:

		// --- ATTRIBUTES

		String id_;
	};
} // namespace BALL

#endif // BALL_KERNEL_PROTEIN_H
