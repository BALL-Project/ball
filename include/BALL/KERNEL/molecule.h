// $Id: molecule.h,v 1.2 1999/12/28 18:38:45 oliver Exp $

#ifndef BALL_KERNEL_MOLECULE_H
#define BALL_KERNEL_MOLECULE_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_KERNEL_BASEFRAGMENT_H
#	include <BALL/KERNEL/baseFragment.h>
#endif

#ifndef BALL_KERNEL_FRAGMENT_H
#	include <BALL/KERNEL/fragment.h>
#endif

namespace BALL 
{

	class System;

	/**	
	*/
	class Molecule
		: public BaseFragment
	{
		public:

		BALL_CREATE(Molecule)

		/**	@name	Enums
		*/
		//@{

		/**	Properties
		*/
		enum Property
		{
			IS_SOLVENT = BaseFragment::NUMBER_OF_PROPERTIES,
			NUMBER_OF_PROPERTIES
		};
		//@}

		/**	@name	Constructors and Destructors */
		//@{

		///	Default constructor
		Molecule();
	
		/// Copy constructor
		Molecule(const Molecule& molecule, bool deep = true);
	
		/// Detailled constructor
		Molecule(const String& name);

		/// Destructor
		virtual ~Molecule();

		/** Clears the contents of the molecule and removes its comopsite children.
				The molecule is not removed from its parent composite (if any).
		*/
		virtual void clear();
	
		/** Clears the contents of the molecule and removes its comopsite children.
				The molecule is also removed from its parent composite (if any).
		*/
		virtual void destroy();
		
		//@}

		/** @name Persistence */
		//@{

		///
		void persistentWrite(PersistenceManager& pm, const char* name = 0) const;

		///
		void persistentRead(PersistenceManager& pm);

		//@}


		/**	@name	Assignment */
		//@{

		///
		void set(const Molecule& molecule, bool deep = true);

		///
		Molecule& operator = (const Molecule& molecule);

		///
		void get(Molecule& molecule, bool deep = true) const;

		///
		void swap(Molecule& molecule);
	
		//@}

		
		/**	@name	Accessors */
		//@{

		///
		System* getSystem();

		///
		const System* getSystem() const;

		///
		void prepend(Atom& atom);

		///
		void append(Atom& atom);

		///
		void insert(Atom& atom);

		///
		void insertBefore(Atom& atom, Composite& before);

		///
		void insertAfter(Atom& atom, Composite& after);

		///
		bool remove(Atom& atom);

		///
		void prepend(BaseFragment& base_fragment);

		///
		void append(BaseFragment& base_fragment);

		///
		void insert(BaseFragment& base_fragment);

		///
		void insertBefore(BaseFragment& base_fragment, Composite& before);

		///
		void insertAfter(BaseFragment& base_fragment, Composite& after);

		///
		void spliceBefore(BaseFragment& base_fragment);

		///
		void spliceAfter(BaseFragment& base_fragment);

		///
		void splice(BaseFragment& base_fragment);

		///
		bool remove(BaseFragment& base_fragment);

		//@}

	
		/**	@name Debugging and Diagnostics */
		//@{

		///
		virtual bool isValid() const;

		///
		virtual void dump(std::ostream& s = std::cout, Size depth = 0) const;

		//@}


		/**	@name	Storers */
		//@{

		///
		virtual void read(std::istream& s);

		///
		virtual void write(std::ostream& s) const;
		
		//@}

		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(Fragment)



		protected:

		Molecule* getMolecule();

		const Molecule* getMolecule() const;

		BaseFragment* getSuperBaseFragment();

		const BaseFragment* getSuperBaseFragment() const;

		void prepend(Molecule& molecule);

		void append(Molecule& molecule);

		void insert(Molecule& molecule);

		void insertBefore(Molecule& molecule, Composite& composite);

		void insertAfter(Molecule& molecule, Composite& composite);

		bool remove(Molecule& molecule);

		bool isSubBaseFragmentOf(const BaseFragment& base_fragment) const;
	};

} // namespace BALL 

#endif // BALL_KERNEL_MOLECULE_H
