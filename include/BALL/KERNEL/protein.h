// $Id: protein.h,v 1.18 2001/07/15 16:14:22 oliver Exp $ 

#ifndef BALL_KERNEL_PROTEIN_H
#define BALL_KERNEL_PROTEIN_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_KERNEL_CHAIN_H
#	include <BALL/KERNEL/chain.h>
#endif

#ifndef BALL_KERNEL_CHAINITERATOR_H
#	include <BALL/KERNEL/chainIterator.h>
#endif

#ifndef BALL_KERNEL_MOLECULE_H
#	include <BALL/KERNEL/molecule.h>
#endif

#ifndef BALL_KERNEL_PDBATOM_H
#	include <BALL/KERNEL/PDBAtom.h>
#endif

#ifndef BALL_KERNEL_RESIDUE_H
#	include <BALL/KERNEL/residue.h>
#endif

#ifndef BALL_KERNEL_SECONDARYSTRUCTURE_H
#	include <BALL/KERNEL/secondaryStructure.h>
#endif


#define BALL_PROTEIN_DEFAULT_ID   ""

namespace BALL 
{

	/**	Kernel Protein Class.
			This class is used to represent proteins.
			A protein may contain several \Ref{Chain}, \Ref{SecondaryStructure},
			and \Ref{Residue}.\\
			{\bf Definition:} \URL{BALL/KERNEL/protein.h}
	*/
	class Protein
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
		Protein()
			throw();
	
		/// Copy constructor
		Protein(const Protein& protein, bool deep = true)
			throw();
	
		/// Detailled constructor 
		Protein(const String& name, const String& id = BALL_PROTEIN_DEFAULT_ID)
			throw();

		///	Destructor
		virtual ~Protein()
			throw();
	
		/// Clears the contents of the protein
		virtual void clear()
			throw();
	
		/* Clears the contents of the protein and removes it from all composite structures.
		*/
		virtual void destroy()
			throw();

		//@}
		/**	@name	Persistence 
		*/
		//@{
		
		/**	Writes a Protein object to a persistent stream.
				@param pm the persistence manager
		*/
		void persistentWrite(PersistenceManager& pm, const char* name = 0) const
			throw(Exception::GeneralException);

		/**	Reads a Protein object from a persistent stream.
				@param pm the persistence manager
		*/
 		void persistentRead(PersistenceManager& pm)
			throw(Exception::GeneralException);

		//@}
		/**	@name Assignment Methods 
		*/
		//@{

		/** Assignment with cloning facility.
				The assignment is either deep or shallow (default).
				@param  protein the Protein to be copied (cloned)
				@param  deep make a deep (={\tt true}) or shallow (={\tt false}) copy
		*/
		void set(const Protein& protein, bool deep = true)
			throw();

		/** Assignment operator.
				@param   protein the Protein to be copied (cloned)
				@return  Protein& - this instance
		*/
		const Protein& operator = (const Protein& protein)
			throw();

		/** Copy this instance to {\em protein}.
				The assignment is either deep or shallow (default).
				@param  protein the Protein to be assigned to
		*/
		void get(Protein& protein, bool deep = true) const
			throw();
	
		/** Swapping of two proteins.
				@param  protein to swap with this instance
		*/
		void swap(Protein& protein)
			throw();
	
		//@}

		/**	Equality operator.
				Two proteins are equal if they have the same handle.
				@see Object::operator ==.
		*/
		bool operator == (const Protein& protein) const
			throw();

		/**	Inequality operator
				@see operator ==
		*/
		bool operator != (const Protein& protein) const
			throw();

		/**	@name Accessors 
		*/
		//@{

		/** Get a pointer to a child chain at a given position.
				The pointer is 0 if this instance does not have a chain at the given position.
				@param   position the position of the child chain
				@return  Chain* - mutable pointer to the child
		*/
		Chain* getChain(Position position)
			throw();

		/** Get a pointer to a child chain at a given position.
				The pointer is 0 if this instance does not have a chain at the given position.
				@param   position the position of the child chain
				@return  Chain* - constant pointer to the child
		*/
		const Chain* getChain(Position position) const
			throw();

		/** Get a pointer to a child SecondaryStructure at a given position.
				The pointer is 0 if this instance does not have a SecondaryStructure at the given position.
				@param   position the position of the child SecondaryStructure
				@return  SecondaryStructure* - mutable pointer to the child
		*/
		SecondaryStructure* getSecondaryStructure(Position position)
			throw();

		/** Get a pointer to a child SecondaryStructure at a given position.
				The pointer is 0 if this instance does not have a SecondaryStructure at the given position.
				@param   position the position of the child SecondaryStructure
				@return  SecondaryStructure* - constant pointer to the child
		*/
		const SecondaryStructure* getSecondaryStructure(Position position) const
			throw();

		/** Get a pointer to a child Residue at a given position.
				The pointer is 0 if this instance does not have a Residue at the given position.
				@param   position the position of the child Residue
				@return  Residue* - mutable pointer to the child
		*/
		Residue* getResidue(Position position)
			throw();

		/** Get a pointer to a child Residue at a given position.
				The pointer is 0 if this instance does not have a Residue at the given position.
				@param   position the position of the child Residue
				@return  Residue* - constant pointer to the child
		*/
		const Residue* getResidue(Position position) const
			throw();

		/** Get a pointer to the N-terminal Residue.
				The pointer is 0 if this instance does not have a Residue with
				the property "PROPERTY__AMINO_ACID".
				@return  Residue* - mutable pointer to the N-terminal	Residue
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
				@return  Residue* - mutable pointer to the C-terminal	Residue
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

		/** Get a pointer to a child PDB-Atom at a given position.
				The pointer is 0 if this instance does not have a PDB-Atom at the given position.
				@param   position the position of the child PDB-Atom
				@return  Residue* - mutable pointer to the child
		*/
		PDBAtom* getPDBAtom(Position position)
			throw();

		/** Get a pointer to a child PDB-Atom at a given position.
				The pointer is 0 if this instance does not have a PDB-Atom at the given position.
				@param   position the position of the child PDB-Atom
				@return  Residue* - constant pointer to the child
		*/
		const PDBAtom* getPDBAtom(Position position) const
			throw();

		/**	Set the ID of the NucleicAcid.
				@param id the new ID
		*/
		void setID(const String& id)
			throw();

		/**	Retrieve the ID of the NucleicAcid.
				@return String the ID
		*/
		const String& getID() const
			throw();

		/** Count the child chains.
				@return Size the number of chains
		*/
		Size countChains() const
			throw();

		/** Count the child SecondaryStructures.
				@return Size the number of SecondaryStructures
		*/
		Size countSecondaryStructures() const
			throw();

		/** Count the child Residues.
				@return Size the number of Residues
		*/
		Size countResidues() const
			throw();

		/** Count the child PDB-Atoms.
				@return Size the number of PDB-Atoms
		*/
		Size countPDBAtoms() const
			throw();

		//@}

#ifdef	BALL_CFG_USING_METHOD_DIRECTIVE
		using Molecule::prepend;
		using Molecule::append;
		using Molecule::insert;
		using Molecule::insertBefore;
		using Molecule::insertAfter;
		using Molecule::remove;
		using Molecule::splice;
		using Molecule::spliceBefore;
		using Molecule::spliceAfter;
#else
		Molecule::prepend;
		Molecule::append;
		Molecule::insert;
		Molecule::insertBefore;
		Molecule::insertAfter;
		Molecule::remove;
		Molecule::splice;
		Molecule::spliceBefore;
		Molecule::spliceAfter;
#endif

		/**	@name	Debugging and Diagnostics
		*/
		//@{

		/** Internal state and consistency self-validation.
				@return	 bool -{\tt true} if the internal state of this instance is 
								 correct (self-validated) and consistent, {\tt false} otherwise
		*/
		virtual bool isValid() const
			throw();

		/** Internal state dump.
				Dump the current internal state  to the output 
				ostream {\em s} with dumping depth {\em depth}.
				@param	s output stream where to output the internal state 
				@param  depth the dumping depth
		*/
		virtual void dump(std::ostream& s = std::cout, Size depth = 0) const
			throw();

		//@}

		// --- EXTERNAL ITERATORS

		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(Chain)
		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(SecondaryStructure)
		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(Residue)
		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(PDBAtom)

		
		private:

		// --- ATTRIBUTES

		String id_;
	};

} // namespace BALL

#endif // BALL_KERNEL_PROTEIN_H
