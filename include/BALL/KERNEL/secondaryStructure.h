// $Id: secondaryStructure.h,v 1.2 1999/12/04 18:34:19 oliver Exp $

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

	/**
	*/
	class SecondaryStructure
		: public BaseFragment
	{
		public:

		BALL_CREATE(SecondaryStructure)


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

		/**	@name	Constructors and Destructors */
		//@{

		/// Default constructor
		SecondaryStructure();
	
		/// Copy constructor
		SecondaryStructure(const SecondaryStructure& secondary_structure,bool deep = true);
	
		/// Detailled constructor 
		SecondaryStructure(const String& name);

		///	Destructor
		virtual ~SecondaryStructure();

		/// Clears the contents of the secondary structure
		virtual void clear();
	
		/// Clears the contents of the secondary structure and removes it from all composite structures
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
		void set(const SecondaryStructure& secondary_structure, bool deep = true);

		///
		SecondaryStructure &operator =(const SecondaryStructure& secondary_structure);

		///
		void get(SecondaryStructure& secondary_structure, bool deep = true) const;

		///
		void swap(SecondaryStructure& secondary_structure);

		//@}

	
		/**	@name Accessors */	
		//@{

		///
		Protein* getProtein();

		///
		const Protein* getProtein() const;

		///
		Chain* getChain();

		///
		const Chain* getChain() const;

		///
		Residue* getResidue(Index index);
	
		///
		const Residue* getResidue(Index index) const;

		///
		Residue* getNTerminal();
	
		///
		const Residue* getNTerminal() const;

		///
		Residue* getCTerminal();
	
		///
		const Residue* getCTerminal() const;

		///
		PDBAtom* getPDBAtom(Index index);
	
		///
		const PDBAtom* getPDBAtom(Index index) const;
	
		///
		Size countResidues() const;

		///
		Size countPDBAtoms() const;

		///
		void prepend(Residue& residue);

		///
		void append(Residue& residue);

		///
		void insert(Residue& residue);

		///
		void insertBefore(Residue& residue, Composite& before);

		///
		void insertAfter(Residue& residue, Composite& after);

		///
		bool remove(Residue& residue);

		///
		void spliceBefore(SecondaryStructure& secondary_structure);

		///
		void spliceAfter(SecondaryStructure& secondary_structure);

		///
		void splice(SecondaryStructure& secondary_structure);
		//@}

		/**	@name	Debugging and Diagnostics */
		//@{
	
		///
		virtual bool isValid() const;

		///
		virtual void dump(ostream& s = cout, unsigned long depth = 0) const;
		//@}

		// --- STORERS

		virtual void read(istream& s);

		virtual void write(ostream& s) const;

			
		// --- EXTERNAL ITERATORS

		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(Residue)
		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(PDBAtom)

	protected:

	private:

		Fragment* getFragment(Index index);
	
		const Fragment* getFragment(Index index) const;
	
		Atom* getAtom(Index index);
	
		const Atom* getAtom(Index index) const;

		void prepend(Atom &atom);

		void append(Atom &atom);

		void insert(Atom &atom);

		void insertBefore(Atom &atom, Composite &before);

		void insertAfter(Atom &atom, Composite &after);

		bool remove(Atom &atom);

		void prepend(BaseFragment &base_fragment);

		void append(BaseFragment &base_fragment);

		void insert(BaseFragment &base_fragment);
			

		void insertBefore(BaseFragment &base_fragment, Composite &before);

		void insertAfter(BaseFragment &base_fragment, Composite &after);

		void spliceBefore(BaseFragment &base_fragment);

		void spliceAfter(BaseFragment &base_fragment);

		void splice(BaseFragment &base_fragment);

		bool remove(BaseFragment &base_fragment);

		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(BaseFragment)

		void clear_();
	};

} // namespace BALL

#endif // BALL_KERNEL_SECONDARYSTRUCTURE_H
