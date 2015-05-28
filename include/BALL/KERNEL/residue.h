// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_KERNEL_RESIDUE_H
#define BALL_KERNEL_RESIDUE_H

#ifndef BALL_KERNEL_FRAGMENT_H
#	include <BALL/KERNEL/fragment.h>
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
	class SecondaryStructure;

	/** Residue class.
			This class is used to represent residues in proteins.
			 \par
			
    	\ingroup KernelContainers 
	*/
	class BALL_EXPORT Residue
		: public Fragment
	{
		public:

		BALL_CREATE_DEEP(Residue)
	
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
			/** Property for cyclic peptides.
					This property is set by the build bonds processor, if 
					an atom of the first and an atom of the last residue in a chain
					share a bond.
			*/
			PROPERTY__CYCLIC,

			///
			NUMBER_OF_PROPERTIES
		};

		/**	Enum used to describe the kind of
				name constructed by  \link getFullName getFullName \endlink .
		*/
		enum FullNameType
		{
			/// This returns the unmodified residue name only
			NO_VARIANT_EXTENSIONS,
			/// Add the residue variant extensions (e.g. '-C' for C-terminal residues)
			ADD_VARIANT_EXTENSIONS,
			// Add the residue ID separated by a colon
      ADD_RESIDUE_ID,
      // Add the residue ID and the residue extension
      ADD_VARIANT_EXTENSIONS_AND_ID
		};

		//@}


		/**	@name	Constructors and Destructors 
		*/
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

		/// Clears the contents of this instance.
		virtual void clear();
	
		/// Clears the contents of this instance and removes it from all composite structures.
		virtual void destroy();
	
		//@}
		/** @name Persistence 
		*/
		//@{

		/**	Writes a Residue object to a persistent stream.
		 *	@param pm the persistence manager
		 */
		void persistentWrite(PersistenceManager& pm, const char* name = 0) const;

		/**	Reads a Residue object from a persistent stream.
		 *	@param pm the persistence manager
		 */
		void persistentRead(PersistenceManager& pm);

		//@}
		/**	@name	Assignment 
    */
		//@{

		/** Assignment with cloning facility.
				The assignment is either deep or shallow (default).
				@param  residue the Residue to be copied (cloned)
				@param  deep make a deep (=<tt>true</tt>) or shallow (=<tt>false</tt>) copy
		*/
		void set(const Residue& residue, bool deep = true);

		/**	Assignment operator	
		*/
		Residue& operator = (const Residue& residue);

		/**	Assign a Residue object from another instance.
		*/
		void get(Residue& residue, bool deep = true) const;

		/**	Swap the contents of two Residue objects. 
		*/
		void swap(Residue& residue);
	
		//@}

		/**	Equality operator.
				Two residues are equal if they have the same handle.
				@see Object::operator ==
		*/
		bool operator == (const Residue& residue) const;

		/**	Inequality operator
				@see operator ==
		*/
		bool operator != (const Residue& residue) const;


		/**	@name	Accessors 
		*/
		//@{

		/**	Return the fully qualified name.
				Residue names are modified as follows:

					- <tt>-N</tt> is added for N terminal residues
					- <tt>-C</tt> is added for C terminal residue
					- <tt>[-]S</tt> is added for CYS involved in a disulphide bridge
					- <tt>-M</tt> is added for individual amino acids (C and N terminal)
				
				@param	type if type is set to <tt>NO_VARIANT_EXTENSIONS</tt>,
								the variant extension (<tt>-XX</tt>) is omitted.
				@return String the full fragment name
		*/
		String getFullName(FullNameType type = ADD_VARIANT_EXTENSIONS) const;
		
		/**	Check whether the torsion angle phi is defined.
				@return bool
		*/
		bool hasTorsionPhi() const;
	
		/**	Calculate the torsion angle phi.
				@return Angle - the torsion angle phi
		*/
		Angle getTorsionPhi() const;
		
		/**	Check whether the torsion angle psi is defined.
				@return bool
		*/
		bool hasTorsionPsi() const;
	
		/**	Calculate the torsion angle psi.
				@return Angle - the torsion angle phi
		*/
		Angle getTorsionPsi() const;

		/**	Check whether the torsion angle omega is defined.
				@return bool
		*/
		bool hasTorsionOmega() const;

		/**	Calculate the torsion angle psi.
				@return Angle - the torsion angle phi
		*/
		Angle getTorsionOmega() const;

		/** Get a pointer to the parent protein.
				The pointer is 0 if this instance does not have a parent protein.
				@return  Protein* - mutable pointer to the parent protein
		*/
		Protein* getProtein();

		/** Get a constant pointer to the parent protein.
				The pointer is 0 if this instance does not have a parent protein.
				@return  Protein* - constant pointer to the parent protein
		*/
		const Protein* getProtein() const;

		/** Get a pointer to the parent chain.
				The pointer is 0 if this instance does not have a parent chain.
				@return  Chain* - mutable pointer to the parent chain
		*/
		Chain* getChain();

		/** Get a pointer to the parent chain.
				The pointer is 0 if this instance does not have a parent chain.
				@return  Chain* - constant pointer to the parent chain
		*/
		const Chain* getChain() const;

		/** Get a pointer to the parent secondary structure.
				The pointer is 0 if this instance does not have a parent secondary structure.
				@return  SecondaryStructure* - mutable pointer to the parent chain
		*/
		SecondaryStructure* getSecondaryStructure();

		/** Get a pointer to the parent secondary structure.
				The pointer is 0 if this instance does not have a parent secondary structure.
				@return  SecondaryStructure* - mutable pointer to the parent chain
		*/
		const SecondaryStructure* getSecondaryStructure() const;

		/** Get a pointer to a child PDBAtom at a given position.
				The pointer is 0 if this instance does not have a PDBAtom at this position.
				@param   position the position of the child PDBAtom
				@return  PDBAtom* - mutable pointer to the child
		*/
		PDBAtom* getPDBAtom(Position position);
	
		/** Get a pointer to a child PDBAtom at a given position.
				The pointer is 0 if this instance does not have a PDBAtom at this position.
				@param   position the position of the child PDBAtom
				@return  PDBAtom* - constant pointer to the child
		*/
		const PDBAtom* getPDBAtom(Position position) const;
	
		/**	Set the ID.
				@param id the new ID
		*/
		void setID(const String& id);

		/**	Retrieve the ID.
				@return String the ID
		*/
		const String& getID() const;

		/**
		 * Set the insertion code.
		 *
		 * An insertion code is a visible ASCII character as well as the space
		 * character ' '.
		 *
		 * @param insertion_code the new insertion code
		 * @throws InvalidArgument if an unsupported insertion code has been specified.
		 */
		void setInsertionCode(char insertion_code);

		/**
		 * Unset the current insertion code.
		 *
		 * This is equivalent to calling setInsertionCode(' ').
		 */
		void unsetInsertionCode();

		/**	Get the insertion code.
				@return  char the insertion code
		*/
		char getInsertionCode() const;

		/**	Count the child PDBAtoms.
				@return  Size the number of PDBAtoms
		*/
		Size countPDBAtoms() const;

		/** Prepend a PDBAtom at position 0.
				@param atom the PDBAtom to prepend
		*/
		void prepend(PDBAtom& atom);

		/** Append a PDBAtom at the last position.
				@param atom the PDBAtom to append
		*/
		void append(PDBAtom& atom);

		/** Insert a PDBAtom at the last position.
				@param atom the PDBAtom to append
		*/
		void insert(PDBAtom& atom);

		/** Insert a PDBAtom before a given Composite object.
				@param atom the PDBAtom to insert
				@param before the Composite object to insert before
		*/
		void insertBefore(PDBAtom& atom, Composite& before);

		/** Insert a PDBAtom after a given Composite object.
				@param atom the PDBAtom to insert
				@param after the Composite object to insert after
		*/
		void insertAfter(PDBAtom& atom, Composite& after);

		/** Remove a PDBAtom.
				@param atom the PDBAtom to remove
		*/
		bool remove(PDBAtom& atom);

		/**	Cut all children of <tt>residue</tt> and prepend them before the children of this instance.
				@param residue the residue to access
		*/
		void spliceBefore(Residue& residue);

		/**	Cut all children of <tt>residue</tt> and append them after the children of this instance.
				@param residue the residue to access
		*/
		void spliceAfter(Residue& residue);

		/**	Move the children of <tt>residue</tt> into this instance.
				The children are inserted using  \link spliceBefore spliceBefore \endlink .
		*/
		void splice(Residue& residue);

		//@}
		/**	@name	Predicates 
		*/
		//@{

		/** Test if this residue is an amino acid.
				Returns true, if this instance has the property "PROPERTY__AMINO_ACID".
				@return bool
		*/
		bool isAminoAcid() const;
	
		/** Test if this residue is terminal.
				Returns true, if this instance has the property "PROPERTY__AMINO_ACID"
				and is the first or last amino acid residue	in its parent chain.
				@return bool
		*/
		bool isTerminal() const;
	
		/** Test if this residue is N-terminal.
				Returns true, if this instance has the property "PROPERTY__AMINO_ACID"
				and is the first amino acid residue	in its parent chain.
				@return bool
		*/
		bool isNTerminal() const;
	
		/** Test if this residue is C-terminal.
				Returns true, if this instance has the property "PROPERTY__AMINO_ACID"
				and is the last amino acid residue in its parent chain.
				@return bool
		*/
		bool isCTerminal() const;

		//@}

		/** @name Type predicates */
		//@{
		/// @copydoc Composite::isResidue
		virtual bool isResidue() const { return true; }
		//@}

		/**	@name	Debugging and Diagnostics 
		*/
		//@{

		/** Internal state and consistency self-validation.
				@return	 bool - <tt>true</tt> if the internal state 
								 is correct (self-validated) and consistent, <tt>false</tt> otherwise
		*/
		virtual bool isValid() const;

		/** Internal state dump.
				Dump the current internal state to the output
				ostream <b>  s </b> with dumping depth <b>  depth </b>.
				@param	s output stream where to output the internal state
				@param  depth the dumping depth
		*/
		virtual void dump(std::ostream& s = std::cout, Size depth = 0) const;

		//@}

		// --- EXTERNAL ITERATORS	

		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(PDBAtom)
		BALL_DECLARE_STD_ITERATOR_WRAPPER(Residue, PDBAtom, pdbAtoms)


		private:

		AtomContainer* getAtomContainer(Position position);
	
		const AtomContainer* getAtomContainer(Position position) const;
	
		Size countAtomContainers() const;

		void prepend(AtomContainer& atom_container);

		void append(AtomContainer& atom_container);

		void insert(AtomContainer& atom_container);

		void insertBefore(AtomContainer& atom_container, Composite& composite);

		void insertAfter(AtomContainer& atom_container, Composite& composite);

		void spliceBefore(AtomContainer& atom_container);

		void spliceAfter(AtomContainer& base_ragment);

		void splice(AtomContainer& AtomContainer);

		bool remove(AtomContainer& AtomContainer);

		bool isSuperAtomContainerOf(const AtomContainer& atom_container) const;

		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(AtomContainer)
		BALL_DECLARE_STD_ITERATOR_WRAPPER(Residue, AtomContainer, atomContainers)

		// --- ATTRIBUTES

		String 	id_;

		char 		insertion_code_;
	};

  template <class ResidueContainerType>
  const Residue* getNTerminal(const ResidueContainerType& residue_container)
  {
		ResidueConstIterator res_it(residue_container.beginResidue());
    for (; res_it != residue_container.endResidue(); ++res_it)
		{
      if (res_it->isAminoAcid() == true)
			{
        return &(*res_it);
			}
		}

    return 0;
  }

  template <class ResidueContainerType>
  const Residue* getCTerminal(const ResidueContainerType& residue_container)
  {
		ResidueConstReverseIterator res_it(residue_container.rbeginResidue());
    for (; res_it != residue_container.rendResidue(); ++res_it)
		{
			// Look for the last residue marked as amino acid
      if (res_it->isAminoAcid() == true)
			{
        return &(*res_it);
			}
		}

    return 0;
  }

} // namespace BALL

#endif // BALL_KERNEL_RESIDUE_H
