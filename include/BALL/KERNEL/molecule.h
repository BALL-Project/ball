// $Id: molecule.h,v 1.4 2000/04/11 15:26:04 amoll Exp $

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

	/**	Kernel Molecule Class.
			Used to represent general molecules without specific properties.\\
			{\bf Definition:}\URL{BALL/KERNEL/molecule.h}
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

		/**	Default constructor */
		Molecule();
	
		/** Copy constructor */
		Molecule(const Molecule& molecule, bool deep = true);
	
		/** Detailled constructor */
		Molecule(const String& name);

		/** Destructor */
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

		/**	Persistent writing.
				Writes a Molecule object to a persistent stream.
				@param pm the persistence manager
		*/
		void persistentWrite(PersistenceManager& pm, const char* name = 0) const;

		/**	Persistent reading.
				Reads a Molecule object from a persistent stream.
				@param pm the persistence manager
		*/
		void persistentRead(PersistenceManager& pm);

		//@}


		/**	@name	Assignment */
		//@{

		/**	Assign from another Molecule.
				@param molecule	the Molecule object to assign from
				@param deep
		*/
		void set(const Molecule& molecule, bool deep = true);

		/**	Assignment operator.
				Assign the components from another Molecule.
				@param molecule the Molecule to assign from
		**/
		Molecule& operator = (const Molecule& molecule);

		/**	Assign to another Molecule.
				Assigns the components to another Molecule.
				@param molecule	the Molecule to be assigned to
				@param deep
		*/
		void get(Molecule& molecule, bool deep = true) const;

		/**	Swap the contents of two Molecules.
				@param	molecule the Molecule to swap contents with
		*/
		void swap(Molecule& molecule);
	
		//@}

		
		/**	@name	Accessors */
		//@{

		/**	Access the superior System.
				@return	System* pointer to the superior System
		*/
		System* getSystem();

		/**	Get a const pointer to the superior System.
				@return	System* pointer to the superior System
		*/
		const System* getSystem() const;

		/** Insert an atom as the first child of the instance
				@param atom the atom to add
		*/
		void prepend(Atom& atom);

		/** Insert an atom as the last child of the instance
				@param atom the atom to add
		*/
		void append(Atom& atom);

		/** Insert an atom as the last child of the instance
				@param atom the atom to add
		*/
		void insert(Atom& atom);

		///
		void insertBefore(Atom& atom, Composite& before);

		///
		void insertAfter(Atom& atom, Composite& after);

		/** Remove an Atom
				@param atom the Atom to remove
		*/
		bool remove(Atom& atom);

		/** Insert a BaseFragment as the first child of the instance
				@param base_fragment the BaseFragment to add
		*/
		void prepend(BaseFragment& base_fragment);

		/** Insert a BaseFragment as the last child of the instance
				@param base_fragment the BaseFragment to add
		*/
		void append(BaseFragment& base_fragment);

		/** Insert a BaseFragment as the last child of the instance
				@param base_fragment the BaseFragment to add
		*/
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

		/** Remove a BaseFragment
				@param base_fragment the BaseFragment to remove
		*/
		bool remove(BaseFragment& base_fragment);

		//@}

	
		/**	@name Debugging and Diagnostics */
		//@{

		/**	Test if instance is valid.
				@return bool
		*/
		virtual bool isValid() const;

		///
		virtual void dump(std::ostream& s = std::cout, Size depth = 0) const;

		//@}


		/**	@name	Storers */
		//@{

		/**	Read the components of a molecule from an instream.
				@param s the instream
		*/
		virtual void read(std::istream& s);

		/**	Write the components of a molecule to an outstream.
				@param s the outstream
		*/
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
