// $Id: residue.h,v 1.8 2000/03/23 20:04:39 oliver Exp $

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

	/**
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
				name constructed by \Ref{getFullName}
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

		/// Clears the residues contents
		virtual void clear();
	
		/// Clears the residues contents and removes it from all composite structures
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

		///	
		void set(const Residue& residue, bool deep = true);

		///	
		Residue &operator = (const Residue& residue);

		///	
		void get(Residue& residue, bool deep = true) const;

		///	
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

		///
		Protein* getProtein();

		///
		const Protein* getProtein() const;

		///
		Chain* getChain();
		
		///
		const Chain* getChain() const;

		///
		PDBAtom* getPDBAtom(Index index);
	
		///
		const PDBAtom* getPDBAtom(Index index) const;
	
		///
		void setID(const String& id);

		///
		String& getID();

		///
		const String& getID() const;

		///
		void setInsertionCode(char insertion_code);

		///
		char getInsertionCode() const;

		///
		Size countPDBAtoms() const;

		///
		void prepend(PDBAtom& atom);

		///
		void append(PDBAtom& atom);

		///
		void insert(PDBAtom& atom);

		///
		void insertBefore(PDBAtom& atom, Composite& before);

		///
		void insertAfter(PDBAtom& atom, Composite& after);

		///
		bool remove(PDBAtom& atom);

		///
		void spliceBefore(Residue& residue);

		///
		void spliceAfter(Residue& residue);

		///
		void splice(Residue& residue);

		//@}


	
		/**	@name	Predicates */
		//@{

		///
		bool isAminoAcid() const;
	
		///
		bool isTerminal() const;
	
		///
		bool isNTerminal() const;
	
		///
		bool isCTerminal() const;

		//@}

	
		/**	@name	Debugging and Diagnostics */
		//@{

		///
		virtual bool isValid() const;

		///
		virtual void dump(std::ostream& s = std::cout, Size depth = 0) const;
		//@}

		// --- STORERS

		virtual void read(std::istream& s);

		virtual void write(std::ostream& s) const;

		// --- EXTERNAL ITERATORS	

		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(PDBAtom)



		private:

		BaseFragment* getBaseFragment(Index index);
	
		const BaseFragment* getBaseFragment(Index index) const;
	
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
