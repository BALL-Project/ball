// $Id: molecule.h,v 1.7 2000/04/14 13:09:54 amoll Exp $

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

		/** Insert an atom before an given {\em Comosite} object.
				@param atom, the atom to insert
				@param before, the {\em Comosite} object to insert before
		*/
		void insertBefore(Atom& atom, Composite& before);

		/** Insert an atom after an given {\em Comosite} object.
				@param atom, the atom to insert
				@param after, the {\em Comosite} object to insert after
		*/
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

		/** Insert a BaseFragment before a given {\em Comosite} object.
				@param base_fragment, the BaseFragment to insert
				@param before, the {\em Comosite} object to insert before
		*/
		void insertBefore(BaseFragment& base_fragment, Composite& before);

		/** Insert a BaseFragment after a given {\em Comosite} object.
				@param base_fragment, the BaseFragment to insert
				@param after, the {\em Comosite} object to insert after
		*/
		void insertAfter(BaseFragment& base_fragment, Composite& after);

		/**	Cut all children of {\tt base_fragment} and prepend them before the children of this molecule.
				@param base_fragment the BaseFragment to access
		*/
		void spliceBefore(BaseFragment& base_fragment);

		/**	Cut all children of {\tt base_fragment} and append them after the children of this molecule.
				@param base_fragment the BaseFragment to access
		*/
		void spliceAfter(BaseFragment& base_fragment);

		/**	Move the children of base_fragment into this molecule.
				The children of {\tt base_fragment} are inserted at the position of 
				{\tt base_fragment} if {\tt base_fragment} is a child of {\tt this}.
				Otherwise the children are inserted using \Ref{spliceBefore}.
		*/
		void splice(BaseFragment& base_fragment);

		/** Remove a BaseFragment
				@param base_fragment the BaseFragment to remove
		*/
		bool remove(BaseFragment& base_fragment);

		//@}

	
		/**	@name Debugging and Diagnostics */
		//@{

		/** Internal state and consistency self-validation.
				Initiate self-validation of the internal state and data structure consistencies of {\em *this} molecule.
				If the internal state of {\em *this} molecule is correct (self-validated) and consistent {\tt true} is returned,
				{\tt false} otherwise. 
				@return			bool -
										{\tt true} if the internal state of {\em *this} molecule is correct (self-validated) and consistent,
										{\tt false} otherwise
		*/
		virtual bool isValid() const;

		/** Internal state dump.
				Dump the current internal state of {\em *this} molecule to the output ostream {\em s} with dumping depth {\em depth}.
	
				@param	s output stream where to output the internal state of {\em *this} molecule
				@param  depth the dumping depth
		*/
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
