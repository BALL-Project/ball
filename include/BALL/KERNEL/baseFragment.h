// $Id: baseFragment.h,v 1.6 2000/03/30 10:16:05 oliver Exp $

#ifndef BALL_KERNEL_BASEFRAGMENT_H
#define BALL_KERNEL_BASEFRAGMENT_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_CONCEPT_COMPOSITE_H
#	include <BALL/CONCEPT/composite.h>
#endif

#ifndef BALL_CONCEPT_PROPERTY_H
#	include <BALL/CONCEPT/property.h>
#endif

#ifndef BALL_DATATYPE_STRING_H
#	include <BALL/DATATYPE/string.h>
#endif

#ifndef BALL_CONCEPT_PROCESSOR_H
#	include <BALL/CONCEPT/processor.h>
#endif

#ifndef BALL_KERNEL_ATOMITERATOR_H
#	include <BALL/KERNEL/atomIterator.h>
#endif

#ifndef BALL_KERNEL_BASEFRAGMENTITERATOR_H
#	include <BALL/KERNEL/baseFragmentIterator.h>
#endif


#define BALL_BASEFRAGMENT_DEFAULT_NAME   ""

namespace BALL 
{

	class Atom;
	class Molecule;

	/**	Atom Container Base Class.
			The {\tt BaseFragment} class is the base class
			of all kernel classes containing atoms.\\
			{\bf Definition:}\URL{BALL/KERNEL/baseFragment.h}
	*/
	class BaseFragment
		: public Composite,
			public PropertyManager
	{
		public:

		BALL_CREATE(BaseFragment)

		/**	@name	Enums 
		*/
		//@{

		///
		enum Property
		{	
			///
			NUMBER_OF_PROPERTIES = 0
		};
		//@}

		/**	@name Constructors and Destructors */
		//@{

		/// Default constructor
		BaseFragment();
	
		/// Copy constructor			
		BaseFragment(const BaseFragment& base_fragment, bool deep = true);
	
		/// Constructor
		BaseFragment(const String& name);

		/// Destructor
		virtual ~BaseFragment();

		/** Clears the contents of the BaseFragment.
				This methods clears the base fragment's name, destroys all its children
				(as in \Ref{Composite::clear}), but does not remove it from its parent composite structures.
		*/
		virtual void clear();
	
		/** Clears the contents of the BaseFragment and removes it from its parent composite structures.
				This methods clears the base fragment's name, destroys all its children
				(as in \Ref{Composite::destroy}), and removes it from its parent composite structures.
		*/
		virtual void destroy();

		//@}

		/**	@name	Persistence */
		//@{

		/// Writes a persistent copy of the object
		virtual void persistentWrite(PersistenceManager& pm, const char* name = 0) const;

		/// Reads a persistent object
		virtual void persistentRead(PersistenceManager& pm);

		//@}			
	
		/**	@name	Assignment */
		//@{

		///
		void set(const BaseFragment& base_fragment, bool deep = true);

		///
		BaseFragment& operator = (const BaseFragment& base_fragment);

		///
		void get(BaseFragment& base_fragment, bool deep = true) const;

		///
		void swap(BaseFragment& base_fragment);

		//@}
	
		/**	@name	Accessors */
		//@{

		///
		void setName(const String& name);

		///
		String& getName();

		///
		const String& getName() const;

		///
		Molecule* getMolecule();

		///
		const Molecule* getMolecule() const;

		///
		BaseFragment* getSuperBaseFragment();

		///
		const BaseFragment* getSuperBaseFragment() const;

		///
		BaseFragment* getBaseFragment(Index index);
	
		///
		const BaseFragment* getBaseFragment(Index index) const;
	
		///
		Atom* getAtom(Index index);
	
		///
		const Atom* getAtom(Index index) const;
	
		///
		Atom* getAtom(const String& name);
	
		///
		const Atom* getAtom(const String& name) const;
	
		///
		Size countBaseFragments() const;

		///
		Size countAtoms() const;

		///
		Size countBonds() const;

		///
		Size countInterBonds() const;

		///
		Size countIntraBonds() const;

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


		/**	@name	Miscellaneous */
		//@{

		///
		void destroyBonds();

		//@}
	

		/**	@name	Predicates */
		//@{

		///
		bool isSubBaseFragmentOf(const BaseFragment& base_fragment) const;

		///
		bool isSuperBaseFragmentOf(const BaseFragment& base_fragment) const;
		//@}

		/**	Debugging and Diagnostics */
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

		// --- INTERNAL ITERATION ---
		/**	@name	Internal Iteration
		*/
		//@{
		///	Apply to all bonds inside this BaseFragment
		bool applyIntraBond(UnaryProcessor<Bond>& processor);

		/// Apply to all bonds to atoms outside this BaseFragment
		bool applyInterBond(UnaryProcessor<Bond>& processor);
		//@}

		// --- EXTERNAL ITERATORS
		
		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(Atom)
		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(BaseFragment)


		private:

		void clear_();

		// --- ATTRIBUTES

		String 							name_;
	};

} // namespace BALL

#endif // BALL_KERNEL_BASEFRAGMENT_H
