// $Id: system.h,v 1.5 2000/01/15 18:54:17 oliver Exp $

#ifndef BALL_KERNEL_SYSTEM_H
#define BALL_KERNEL_SYSTEM_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_KERNEL_ATOM_H
#	include <BALL/KERNEL/atomIterator.h>
#endif

#ifndef BALL_KERNEL_MOLECULE_H
#	include <BALL/KERNEL/molecule.h>
#endif

#ifndef BALL_KERNEL_MOLECULEITERATOR_H
#	include <BALL/KERNEL/moleculeIterator.h>
#endif

#ifndef BALL_KERNEL_PROTEIN_H
#	include <BALL/KERNEL/protein.h>
#endif

#ifndef BALL_KERNEL_PROTEINITERATOR_H
#	include <BALL/KERNEL/proteinIterator.h>
#endif

#define BALL_SYSTEM_DEFAULT_NAME   ""

namespace BALL 
{

	class Molecule;

	/**
	*/
	class System
		: public Composite,
			public PropertyManager
	{
		public:

		BALL_CREATE(System)

		/**	@name	Enums
		*/
		//@{
		
		/**
		*/
		enum Property
		{
			NUMBER_OF_PROPERTIES
		};
		//@}

		/**	@name	Constructors and Destructors*/
		//@{

		/// Default constructor
		System();
	
		/// Copy constructor
		System(const System& system, bool deep = true);
	
		/// Detailled constructor
		System(const String& name);

		///	Destructor
		virtual ~System();

		/// Clears the contents of the system
		virtual void clear();
	
		/// Clears the contents of the system and removes all its composite structures
		virtual void destroy();
		//@}
	
		/** @name Persistence */
		//@{

		/// Persistent writing
		void persistentWrite(PersistenceManager& pm, const char* name = 0) const;

		/// Persistent reading
		void persistentRead(PersistenceManager& pm);

		//@}

	
		/**	@name	Assignemnt */
		//@{

		///
		void set(const System& system, bool deep = true);

		///
		System& operator = (const System& system);

		///
		void get(System& system, bool deep = true) const;

		///
		void swap(System& system);
	
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
		Size countMolecules() const;

		///
		Size countFragments() const;

		///
		Size countAtoms() const;

		///
		void prepend(Molecule& molecule);

		///
		void append(Molecule& molecule);

		///
		void insert(Molecule& molecule);

		///
		void insertBefore(Molecule& molecule, Composite& before);

		///
		void insertAfter(Molecule& molecule, Composite& after);

		///
		bool remove(Molecule& molecule);

		///
		void spliceBefore(System& system);

		///
		void spliceAfter(System& system);

		///
		void splice(System& system);		
		//@}

		/**	@name 	Miscellaneous */
		//@{

		/// Removes all bonds contained in the system	
		void destroyBonds();

		//@}

		/**	@name	Debugging and Diagnostics */
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
		
		// --- EXTERNAL ITERATORS ---

		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(Atom)
		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(BaseFragment)
		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(Fragment)
		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(Molecule)
		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(Protein)
		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(Residue)
		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(Chain)
		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(SecondaryStructure)


		private:

		void clear_();

		// --- ATTRIBUTES

		String name_;
	};

} // namespace BALL

#endif // BALL_KERNEL_SYSTEM_H
