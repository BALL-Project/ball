// $Id: protein.h,v 1.11 2000/08/30 19:58:10 oliver Exp $ 

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
			A protein may contain several \Ref{Chains}, \Ref{SecondaryStructure}s,
			and \Ref{Residue}s.\\
			{\bf Definition:}\URL{BALL/KERNEL/protein.h}
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

		/** @name	Constructors and Destructors */
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


		/**	@name	Persistence */
		//@{
		
		/**	Persistent writing.
				Writes a Protein object to a persistent stream.
				@param pm the persistence manager
		*/
		void persistentWrite(PersistenceManager& pm, const char* name = 0) const;

		/**	Persistent reading.
				Reads a Protein object from a persistent stream.
				@param pm the persistence manager
		*/
 		void persistentRead(PersistenceManager& pm);

		//@}


		/**	@name Assignment Methods */
		//@{

		/** Assignment with cloning facility.
				Assign {\em protein} to {\em *this}.
				The assignment is either deep or shallow (default).
				@param  protein the Protein to be copied (cloned)
				@param  deep make a deep (={\tt true}) or shallow (={\tt false}) copy of {\em protein}
		*/
		void set(const Protein& protein, bool deep = true);

		/** Assignment operator.
				Assign {\em protein} to {\em *this}.
				The assignment is either deep or shallow (default).
				@param   protein the Protein to be copied (cloned)
				@return  Protein& - {\em *this}
				@see     Protein::set
		*/
		Protein& operator = (const Protein& protein);

		/** Copying with cloning facility.
				Copy {\em *this} to {\em protein}.
				The assignment is either deep or shallow (default).
				@param  protein the Protein to be assigned to
				@see    Protein::set
		*/
		void get(Protein& protein, bool deep = true) const;
	
		/** Swapping of two proteins.
				Swap the states of {\em *this} with {\em protein}.
				@param  protein to swap with {\em *this}
		*/
		void swap(Protein& protein);
	
		//@}


		/**	@name Accessors */
		//@{

		/** Get a pointer to a child chain at a given position.
				The reference is 0 if {\em *this} does not have a chain at the given position.
				@param   position the position of the child chain
				@return  Chain* -
								 mutable reference to the child chain at {\em position} of {\em *this},
		*/
		Chain* getChain(Position position);

		/** Get a pointer to a child chain at a given position.
				The reference is 0 if {\em *this} does not have a chain at the given position.
				@param   position the position of the child chain
				@return  Chain* -
								 constant reference to the child chain at {\em position} of {\em *this},
		*/
		const Chain* getChain(Position position) const;

		/** Get a pointer to a child SecondaryStructure at a given position.
				The reference is 0 if {\em *this} does not have a SecondaryStructure at the given position.
				@param   position the position of the child SecondaryStructure
				@return  SecondaryStructure* - mutable reference to the child SecondaryStructure
																			 at {\em position} of {\em *this},
		*/
		SecondaryStructure* getSecondaryStructure(Position position);

		/** Get a pointer to a child SecondaryStructure at a given position.
				The reference is 0 if {\em *this} does not have a SecondaryStructure at the given position.
				@param   position the position of the child SecondaryStructure
				@return  SecondaryStructure* - constant reference to the child SecondaryStructure
																			 at {\em position} of {\em *this},
		*/
		const SecondaryStructure* getSecondaryStructure(Position position) const;

		/** Get a pointer to a child Residue at a given position.
				The reference is 0 if {\em *this} does not have a Residue at the given position.
				@param   position the position of the child Residue
				@return  Residue* - mutable reference to the child 
														Residue at {\em position} of {\em *this},
		*/
		Residue* getResidue(Position position);

		/** Get a pointer to a child Residue at a given position.
				The reference is 0 if {\em *this} does not have a Residue at the given position.
				@param   position the position of the child Residue
				@return  Residue* - constant reference to the child 
														Residue at {\em position} of {\em *this},
		*/
		const Residue* getResidue(Position position) const;

		/** Get a pointer to the N-terminal Residue.
				The reference is 0 if {\em *this} does not have a N-terminal Residue.
				@return  Residue* - mutable reference to the N-terminal	Residue
		*/
		Residue* getNTerminal();

		/** Get a pointer to the N-terminal Residue.
				The reference is 0 if {\em *this} does not have a N-terminal Residue.
				@return  Residue* - constant reference to the N-terminal Residue
		*/
		const Residue* getNTerminal() const;

		/** Get a pointer to the C-terminal Residue.
				The reference is 0 if {\em *this} does not have a C-terminal Residue.
				@return  Residue* - mutable reference to the C-terminal	Residue
		*/
		Residue* getCTerminal();

		/** Get a pointer to the C-terminal Residue.
				The reference is 0 if {\em *this} does not have a C-terminal Residue.
				@return  Residue* - constant reference to the C-terminal Residue
		*/
		const Residue* getCTerminal() const;

		/** Get a pointer to a child PDB-Atom at a given position.
				The reference is 0 if {\em *this} does not have a PDB-Atom at the given position.
				@param   position the position of the child PDB-Atom
				@return  Residue* - mutable reference to the child 
														PDB-Atom at {\em position} of {\em *this},
		*/
		PDBAtom* getPDBAtom(Position position);

		/** Get a pointer to a child PDB-Atom at a given position.
				The reference is 0 if {\em *this} does not have a PDB-Atom at the given position.
				@param   position the position of the child PDB-Atom
				@return  Residue* - constant reference to the child 
														PDB-Atom at {\em position} of {\em *this},
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
				Initiate self-validation of the internal state and data structure consistencies of {\em *this}.
				@return			bool -
										{\tt true} if the internal state of {\em *this} is correct (self-validated) and consistent,
										{\tt false} otherwise
		*/
		virtual bool isValid() const;

		/** Internal state dump.
				Dump the current internal state of {\em *this} to the output ostream {\em s} with dumping depth {\em depth}.
	
				@param	s output stream where to output the internal state of {\em *this}
				@param  depth the dumping depth
		*/
		virtual void dump(std::ostream& s = std::cout, Size depth = 0) const;
		//@}

		/**	@name	Storers
		*/
		//@{

		/* Persistent stream input and state restorage.
				Read persistent bond data from the input stream {\em s} and restore the state of {\em *this}.
				\\
				{\bf Note:} Not yet implemented.
				@param  s input stream from where to restore the internal state of {\em *this}
		*/
		virtual void read(std::istream& s);

		/* Persistent stream input and state restorage.
				Read persistent bond data from the input stream {\em s} and restore the state of {\em *this}.
				\\
				{\bf Note:} Not yet implemented.
				@param  s input stream from where to restore the internal state of {\em *this}
		*/
		virtual void write(std::ostream& s) const;
		//@}

		// --- EXTERNAL ITERATORS

		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(Chain)
		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(SecondaryStructure)
		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(Residue)
		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(PDBAtom)

		
		private:

		void clear_();

		// --- ATTRIBUTES

		String id_;
	};

} // namespace BALL

#endif // BALL_KERNEL_PROTEIN_H
