// $Id: residue.h,v 1.14 2000/05/09 23:10:35 amoll Exp $

#ifndef BALL_KERNEL_RESIDUE_H
#define BALL_KERNEL_RESIDUE_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_KERNEL_FRAGMENT_H
#	include <BALL/KERNEL/fragment.h>
#endif

#ifndef BALL_KERNEL_PDBATOM_H
#	include <BALL/KERNEL/PDBAtom.h>
#endif

#ifndef BALL_KERNEL_PDBATOMITERATOR_H
#	include <BALL/KERNEL/PDBAtomIterator.h>
#endif

#ifndef BALL_KERNEL_RESIDUEITERATOR_H
#	include <BALL/KERNEL/residueIterator.h>
#endif

 
#define BALL_RESIDUE_DEFAULT_ID               ""
#define BALL_RESIDUE_DEFAULT_INSERTION_CODE   ' '

namespace BALL 
{

	class Chain;
	class Protein;

	/** Kernel Residue Class.
			This class is used to represent residues in proteins.\\
			{\bf Definition:}\URL{BALL/KERNEL/residue.h}
	*/
	class Residue
		: public Fragment
	{
		public:

		BALL_CREATE(Residue)
	
		/**	@name	Enums
		*/
		//@{

		///
		enum Property
		{
			///
			PROPERTY__NON_STANDARD = Fragment::NUMBER_OF_PROPERTIES + 1,
			///
			PROPERTY__AMINO_ACID,
			///
			PROPERTY__WATER,
			///
			PROPERTY__HAS_SSBOND,
			///
			PROPERTY__C_TERMINAL,
			///
			PROPERTY__N_TERMINAL,

			///
			NUMBER_OF_PROPERTIES
		};

		/**	Enum used to describe the kind of
				name constructed by \Ref{getFullName}.
		*/
		enum FullNameType
		{
			///
			NO_VARIANT_EXTENSIONS,

			///
			ADD_VARIANT_EXTENSIONS
		};
		//@}

		/**	@name	Constructors and Destructors */
		//@{	
	
		/// Default constructor
		Residue();
	
		/// Copy constructor
		Residue(const Residue& residue, bool deep = true);
	
		/// Detailled constructor
		Residue
			(const String& name,
			 const String& id = BALL_RESIDUE_DEFAULT_ID,
			 char insertion_code = BALL_RESIDUE_DEFAULT_INSERTION_CODE);

		/// Destructor
		virtual ~Residue();

		/// Clears the residues contents.
		virtual void clear();
	
		/// Clears the residues contents and removes it from all composite structures.
		virtual void destroy();
	
		//@}

		/** @name Persistence */
		//@{

		/// Persistent writing
		void persistentWrite(PersistenceManager& pm, const char* name = 0) const;

		/// Persistent reading
		void persistentRead(PersistenceManager& pm);

		//@}

		/**	@name	Assignment */
		//@{

		/** Assignment with cloning facility.
				The assignment is either deep or shallow (default).
				@param  residue the Residue to be copied (cloned)
				@param  deep make a deep (={\tt true}) or shallow (={\tt false}) copy of {\em residue}
		*/
		void set(const Residue& residue, bool deep = true);

		/**	Assignment operator	*/
		Residue &operator = (const Residue& residue);

		/**	Assign a Residue object from another instance.*/
		void get(Residue& residue, bool deep = true) const;

		/**	Swap the contents of two NucleicAcid objects. */
		void swap(Residue& residue);
	
		//@}

		/**	@name	Accessors */
		//@{

		/**	Return the fully qualified name.
				Residue names are modified as follows:
				\begin{itemize}
					\item {\tt -N} is added for N terminal residues
					\item {\tt -C} is added for C terminal residue
					\item {\tt [-]S} is added for CYS involved in a disulphide bridge
					\item {\tt -M} is added for individual amino acids (C and N terminal)
				\end{itemize}
			@param	type if type is set to {\tt NO_VARIANT_EXTENSIONS},
				the variant extension ({\tt -XX}) is omitted.
			@return String the full fragment name
		*/
		String getFullName(FullNameType type = ADD_VARIANT_EXTENSIONS) const;
		
		/**	Check whether the torsion angle phi is defined.
		*/
		bool hasTorsionPhi() const;
	
		/**	Calculate the torsion angle phi.
		*/
		Angle getTorsionPhi() const;
		
		/**	Check whether the torsion angle psi is defined.
		*/
		bool hasTorsionPsi() const;
	
		/**	Calculate the torsion angle psi.
		*/
		Angle getTorsionPsi() const;		

		/** Get a pointer to the parent protein.
				The reference is 0 if {\em *this} instance does not have a parent protein.
				@return  Protein* -
								 mutable reference to the parent protein of {\em *this}
		*/
		Protein* getProtein();

		/** Get a constant pointer to the parent protein.
				The reference is 0 if {\em *this} instance does not have a parent protein.
				@return  Protein* -
								 constant reference to the parent protein of {\em *this}
		*/
		const Protein* getProtein() const;

		/** Get a pointer to the parent chain.
				The reference is 0 if {\em *this} instance does not have a parent chain.
				@return  Chain* -
								 mutable reference to the parent chain of {\em *this}
		*/
		Chain* getChain();

		/** Get a pointer to the parent chain.
				The reference is 0 if {\em *this} instance does not have a parent chain.
				@return  Chain* -
								 constant reference to the parent chain of {\em *this}
		*/
		const Chain* getChain() const;

		/** Get a pointer to a child PDBAtom at a given position.
				The reference is 0 if {\em *this} instance does not have a PDBAtom at this position.
				@param   position the position of the child PDBAtom
				@return  PDBAtom* -
								 mutable reference to the child PDBAtom at {\em position} of {\em *this}
		*/
		PDBAtom* getPDBAtom(Position position);
	
		/** Get a pointer to a child PDBAtom at a given position.
				The reference is 0 if {\em *this} instance does not have a PDBAtom at this position.
				@param   position the position of the child PDBAtom
				@return  PDBAtom* -
								 constant reference to the child PDBAtom at {\em position} of {\em *this}
		*/
		const PDBAtom* getPDBAtom(Position position) const;
	
		/**	Set the ID of {\em *this} instance.
				@param id the new ID
		*/
		void setID(const String& id);

		/**	Retrieve the ID of {\em *this} instance.
				@return String the ID
		*/
		const String& getID() const;

		/**	Set the insertion code of {\em *this} instance.
				@param insertion_code the new insertion code
		*/
		void setInsertionCode(char insertion_code);

		/**	Get the insertion code of {\em *this} instance.
				@return  char the insertion code
		*/
		char getInsertionCode() const;

		/**	Count the child PDB-Atoms of {\em *this} instance.
				@return  Size the number of PDB-Atoms
		*/
		Size countPDBAtoms() const;

		/** Prepend a PDB-Atom at position 0.
				@param atom, the PDB-Atom to prepend
		*/
		void prepend(PDBAtom& atom);

		/** Append a PDB-Atom at the last position.
				@param atom, the PDB-Atom to append
		*/
		void append(PDBAtom& atom);

		/** Insert a PDB-Atom at the last position.
				@param atom, the PDB-Atom to append
		*/
		void insert(PDBAtom& atom);

		/** Insert a PDB-Atom before a given {\em Comosite} object.
				@param atom, the PDB-Atom to insert
				@param before, the {\em Comosite} object to insert before
		*/
		void insertBefore(PDBAtom& atom, Composite& before);

		/** Insert a PDB-Atom after a given {\em Comosite} object.
				@param atom, the PDB-Atom to insert
				@param after, the {\em Comosite} object to insert after
		*/
		void insertAfter(PDBAtom& atom, Composite& after);

		/** Remove a PDB-Atom.
				@param atom, the PDB-Atom to remove
		*/
		bool remove(PDBAtom& atom);

		/**	Cut all children of {\tt residue} and prepend them before the children of {\em *this}.
				@param residue the residue to access
		*/
		void spliceBefore(Residue& residue);

		/**	Cut all children of {\tt residue} and prepend them after the children of {\em *this}.
				@param residue the residue to access
		*/
		void spliceAfter(Residue& residue);

		/**	Move the children of {\tt residue} into {\em *this}.
				The children of {\tt residue} are inserted at its position if
				it is a child of {\tt this}.
				Otherwise the children are inserted using \Ref{spliceBefore}.
		*/
		void splice(Residue& residue);

		//@}

		/**	@name	Predicates */
		//@{

		/** Test if this residue is an amino acid.
				return bool
		*/
		bool isAminoAcid() const;
	
		/** Test if this residue is terminal.
				Returns true, if this instance has the property "PROPERTY__AMINO_ACID"
				and is the first or last Residue in a parent chain.
				return bool
		*/
		bool isTerminal() const;
	
		/** Test if this residue is N-terminal.
				Returns true, if this instance has the property "PROPERTY__AMINO_ACID"
				and is the first Residue in a parent chain.
				return bool
		*/
		bool isNTerminal() const;
	
		/** Test if this residue is C-terminal.
				Returns true, if this instance has the property "PROPERTY__AMINO_ACID"
				and is the last Residue in a parent chain.
				return bool
		*/
		bool isCTerminal() const;

		//@}

	
		/**	@name	Debugging and Diagnostics */
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

		// --- STORERS

		virtual void read(std::istream& s);

		virtual void write(std::ostream& s) const;

		// --- EXTERNAL ITERATORS	

		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(PDBAtom)



		private:

		BaseFragment* getBaseFragment(Position position);
	
		const BaseFragment* getBaseFragment(Position position) const;
	
		Size countBaseFragments() const;

		void prepend(BaseFragment& base_fragment);

		void append(BaseFragment& base_fragment);

		void insert(BaseFragment& base_fragment);

		void insertBefore(BaseFragment& base_fragment, Composite& composite);

		void insertAfter(BaseFragment& base_fragment, Composite& composite);

		void spliceBefore(BaseFragment& base_fragment);

		void spliceAfter(BaseFragment& base_ragment);

		void splice(BaseFragment& BaseFragment);

		bool remove(BaseFragment& BaseFragment);

		bool isSuperBaseFragmentOf(const BaseFragment& base_fragment) const;

		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(BaseFragment)

		void clear_();

		// --- ATTRIBUTES

		String 	id_;

		char 		insertion_code_;
	};


  template <class ResidueContainerType>
  const Residue* getNTerminal(const ResidueContainerType& residue_container)
  {
		ResidueConstIterator res_it;
    for ( res_it = residue_container.beginResidue(); !res_it.isEnd(); ++res_it)
		{
      if ((*res_it).isAminoAcid() == true)
			{
        return &(*res_it);
			}
		}

    return 0;
  }

  template <class ResidueContainerType>
  const Residue* getCTerminal(const ResidueContainerType& residue_container)
  {
    for (ResidueConstIterator res_it = residue_container.rbeginResidue(); !res_it.isREnd(); ++res_it)
		{
      if ((*res_it).isAminoAcid() == true)
			{
        return &(*res_it);
			}
		}

    return 0;
  }
 

} // namespace BALL

#endif // BALL_KERNEL_RESIDUE_H
