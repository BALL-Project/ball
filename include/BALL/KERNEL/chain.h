// $Id: chain.h,v 1.1 1999/08/26 07:53:19 oliver Exp $

#ifndef BALL_KERNEL_CHAIN_H
#define BALL_KERNEL_CHAIN_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_KERNEL_RESIDUE_H
#	include <BALL/KERNEL/residue.h>
#endif

#ifndef BALL_KERNEL_SECONDARYSTRUCTURE_H
#	include <BALL/KERNEL/secondaryStructure.h>
#endif

#ifndef BALL_KERNEL_SECONDARYSTRUCTUREITERATOR_H
#	include <BALL/KERNEL/secondaryStructureIterator.h>
#endif


#define BALL_CHAIN_DEFAULT_NAME   ' '

namespace BALL {
	
	class Protein;

	/**
	*/
	class Chain
		: public BaseFragment
	{
		public:

		BALL_CREATE(Chain)
		
		/**	@name	Enums
		*/
		//@{

		/**
		*/
		enum Property
		{
			NUMBER_OF_PROPERTIES = Fragment::NUMBER_OF_PROPERTIES
		};
		//@}

		/**	@name	Constructors and Destructors */
		//@{

		/// Default constrcutor
		Chain(void);
	
		/// Copy constructor
		Chain(const Chain& chain, bool deep = true);
	
		/// Detailled constructor
		Chain(const String& name);

		/// Destructor
		virtual ~Chain(void);

		/// Clears the chains contents
		virtual void clear(void);
	
		/// Clears the chains contents and removes the cahin from all composite structures
		virtual void destroy(void);
			
		//@}

		/** @name Persistence */
		//@{

		/// Persistent writing
		void persistentWrite(PersistenceManager& pm, const char* name = 0) const;

		/// Persistent reading
		void persistentRead(PersistenceManager& pm);

		//@}
		
		/**	@name Assignment */
		//@{

		///
		void set(const Chain& chain, bool deep = true);

		///
		Chain& operator = (const Chain& chain);

		///
		void get(Chain& chain, bool deep = true) const;

		///
		void swap(Chain& chain);

		//@}
	
		/**	@name	Accessors */
		//@{

		///
		Protein* getProtein(void);

		///
		const Protein* getProtein(void) const;

		///
		SecondaryStructure* getSecondaryStructure(Index index);
	
		///
		const SecondaryStructure* getSecondaryStructure(Index index) const;
	
		///
		Residue* getResidue(Index index);
	
		///
		const Residue* getResidue(Index index) const;
	
		///
		Residue* getNTerminal(void);
	
		///
		const Residue* getNTerminal(void) const;

		///
		Residue* getCTerminal(void);
	
		///
		const Residue* getCTerminal(void) const;

		///
		PDBAtom* getPDBAtom(Index index);
	
		///
		const PDBAtom* getPDBAtom(Index index) const;
	
		///
		Size countSecondaryStructures(void) const;

		///
		Size countResidues(void) const;

		///
		Size countPDBAtoms(void) const;

		///
		void prepend(SecondaryStructure& secondary_structure);

		///
		void append(SecondaryStructure& secondary_structure);

		///
		void insert(SecondaryStructure& secondary_structure);

		///
		void insertBefore(SecondaryStructure& secondary_structure, Composite& before);

		///
		void insertAfter(SecondaryStructure& secondary_structure, Composite& after);

		///
		bool remove(SecondaryStructure& secondary_structure);

		///
		void prepend(Residue& residue);

		///
		void append(Residue& residue);

		///
		void insert(Residue& residue);

		///
		void insertBefore(Residue& residue,Composite& before);

		///
		void insertAfter(Residue& residue, Composite& after);

		///
		bool remove(Residue& residue);

		///
		void spliceBefore(Chain& chain);

		///
		void spliceAfter(Chain &chain);

		///
		void splice(Chain &chain);
		
		//@}

		/**	@name	Debugging and Diagnostics */
		//@{

		///
		virtual bool isValid(void) const;
		
		///
		virtual void dump(ostream& s = cout, unsigned long depth = 0) const;

		//@}

		// --- STORERS

		///
		virtual void read(istream& s);

		///
		virtual void write(ostream& s) const;

		// --- EXTERNAL ITERATORS

		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(SecondaryStructure)
		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(Residue)
		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(PDBAtom)

	protected:

	private:

		BaseFragment* getBaseFragment(Index index);
	
		const BaseFragment* getBaseFragment(Index index) const;
	
		Atom* getAtom(Index index);
	
		const Atom* getAtom(Index index) const;

		void prepend(Atom& atom);

		void append(Atom& atom);

		void insert(Atom& atom);

		void insertBefore(Atom& atom, Composite& before);

		void insertAfter(Atom& atom, Composite& after);

		bool remove(Atom& atom);

		void prepend(BaseFragment& base_fragment);

		void append(BaseFragment& base_fragment);

		void insert(BaseFragment& base_fragment);

		void insertBefore(BaseFragment& base_fragment, Composite& before);

		void insertAfter(BaseFragment& base_fragment, Composite& after);

		void spliceBefore(BaseFragment& base_fragment);

		void spliceAfter(BaseFragment& base_fragment);

		void splice(BaseFragment& base_fragment);

		bool remove(BaseFragment& base_fragment);

		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(BaseFragment)

		void clear_(void);
	};

} // namespace BALL

#endif // BALL_KERNEL_CHAIN_H
