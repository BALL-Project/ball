// $Id: residue.h,v 1.1 1999/08/26 07:53:19 oliver Exp $

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
		//@}

		/**	@name	Constructors and Destructors */
		//@{	
	
		/// Default constructor
		Residue(void);
	
		/// Copy constructor
		Residue(const Residue& residue, bool deep = true);
	
		/// Detailled constructor
		Residue
			(const String& name,
			 const String& id = BALL_RESIDUE_DEFAULT_ID,
			 char insertion_code = BALL_RESIDUE_DEFAULT_INSERTION_CODE);

		/// Destructor
		virtual ~Residue(void);

		/// Clears the residues contents
		virtual void clear(void);
	
		/// Clears the residues contents and removes it from all composite structures
		virtual void destroy(void);
	
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

		///
		Protein* getProtein(void);

		///
		const Protein* getProtein(void) const;

		///
		Chain* getChain(void);
		
		///
		const Chain* getChain(void) const;

		///
		PDBAtom* getPDBAtom(Index index);
	
		///
		const PDBAtom* getPDBAtom(Index index) const;
	
		///
		void setID(const String& id);

		///
		String& getID(void);

		///
		const String& getID(void) const;

		///
		void setInsertionCode(char insertion_code);

		///
		char getInsertionCode(void) const;

		///
		Size countPDBAtoms(void) const;

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
		bool isAminoAcid(void) const;
	
		///
		bool isTerminal(void) const;
	
		///
		bool isNTerminal(void) const;
	
		///
		bool isCTerminal(void) const;

		//@}

	
		/**	@name	Debugging and Diagnostics */
		//@{

		///
		virtual bool isValid(void) const;

		///
		virtual void dump(ostream& s = cout, unsigned long depth = 0) const;
		//@}

		// --- STORERS

		virtual void read(istream& s);

		virtual void write(ostream& s) const;

		// --- EXTERNAL ITERATORS	

		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(PDBAtom)



		private:

		BaseFragment* getBaseFragment(Index index);
	
		const BaseFragment* getBaseFragment(Index index) const;
	
		Size countBaseFragments(void) const;

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

		void clear_(void);

		// --- ATTRIBUTES

		String 	id_;

		char 		insertion_code_;
	};


  template <class ResidueContainerType>
  const Residue* getNTerminal(const ResidueContainerType& residue_container)
  {
		ResidueConstIterator res_it;
    for ( res_it = residue_container.beginResidue(); !res_it.isEnd(); ++res_it)
      if ((*res_it).isAminoAcid() == true)
        return &(*res_it);

    return 0;
  }

  template <class ResidueContainerType>
  const Residue* getCTerminal(const ResidueContainerType& residue_container)
  {
    for (ResidueConstIterator res_it = residue_container.rbeginResidue(); !res_it.isREnd(); ++res_it)
      if ((*res_it).isAminoAcid() == true)
        return &(*res_it);

    return 0;
  }
 

} // namespace BALL

#endif // BALL_KERNEL_RESIDUE_H
