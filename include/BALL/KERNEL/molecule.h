// $Id: molecule.h,v 1.15 2000/12/16 21:29:03 amoll Exp $

#ifndef BALL_KERNEL_MOLECULE_H
#define BALL_KERNEL_MOLECULE_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_KERNEL_ATOMCONTAINER_H
#	include <BALL/KERNEL/atomContainer.h>
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
		: public AtomContainer
	{
		public:

		BALL_CREATE_DEEP(Molecule)

		/**	@name	Enums
		*/
		//@{

		/**	Properties
		*/
		enum Property
		{
			IS_SOLVENT = AtomContainer::NUMBER_OF_PROPERTIES,
			NUMBER_OF_PROPERTIES
		};
		//@}

		/**	@name	Constructors and Destructors */
		//@{

		/**	Default constructor. */
		Molecule()
			throw();
	
		/** Copy constructor. */
		Molecule(const Molecule& molecule, bool deep = true)
			throw();
	
		/** Detailled constructor. */
		Molecule(const String& name)
			throw();

		/** Destructor. */
		virtual ~Molecule()
			throw();
		
		//@}

		/** @name Persistence */
		//@{

		/**	Persistent writing.
				Writes a Molecule object to a persistent stream.
				@param pm the persistence manager
		*/
		void persistentWrite(PersistenceManager& pm, const char* name = 0) const
			throw();

		/**	Persistent reading.
				Reads a Molecule object from a persistent stream.
				@param pm the persistence manager
		*/
		void persistentRead(PersistenceManager& pm)
			throw();

		//@}


		/**	@name	Assignment */
		//@{

		/**	Assign from another Molecule.
				@param molecule	the Molecule object to assign from
				@param deep
		*/
		void set(const Molecule& molecule, bool deep = true)
			throw();

		/**	Assignment operator.
				Assign the components from another Molecule.
				@param molecule the Molecule to assign from
		**/
		const Molecule& operator = (const Molecule& molecule)
			throw();

		/**	Assign to another Molecule.
				Assigns the components to another Molecule.
				@param molecule	the Molecule to be assigned to
				@param deep
		*/
		void get(Molecule& molecule, bool deep = true) const
			throw();

		/**	Swap the contents of two molecules.
				@param	molecule the Molecule to swap contents with
		*/
		void swap(Molecule& molecule)
			throw();
	
		//@}
		
		/**	@name	Accessors */
		//@{

		/**	Access the parent System.
				@return	System* pointer to the parent System
		*/
		System* getSystem()
			throw();

		/**	Get a const pointer to the parent System.
				@return	System* pointer to the parent System
		*/
		const System* getSystem() const
			throw();

		/** Insert an atom as the first child of {\em *this} instance.
				@param atom the atom to add
		*/
		void prepend(Atom& atom)
			throw();

		/** Insert an atom as the last child of {\em *this} instance.
				@param atom the atom to add
		*/
		void append(Atom& atom)
			throw();

		/** Insert an atom as the last child of {\em *this} instance.
				@param atom the atom to add
		*/
		void insert(Atom& atom)
			throw();

		/** Insert an atom before a {\em Comosite} object.
				@param atom, the atom to insert
				@param before, the {\em Comosite} object to insert before
		*/
		void insertBefore(Atom& atom, Composite& before)
			throw();

		/** Insert an atom after a {\em Comosite} object.
				@param atom, the atom to insert
				@param after, the {\em Comosite} object to insert after
		*/
		void insertAfter(Atom& atom, Composite& after)
			throw();

		/** Remove an atom.
				@param atom the atom to remove
		*/
		bool remove(Atom& atom)
			throw();

		/** Insert a AtomContainer as the first child of {\em *this} instance.
				@param atom_container the AtomContainer to add
		*/
		void prepend(AtomContainer& atom_container)
			throw();

		/** Append a AtomContainer as the last child of {\em *this} instance.
				@param atom_container the AtomContainer to add
		*/
		void append(AtomContainer& atom_container)
			throw();

		/** Insert a AtomContainer as the last child of {\em *this} instance.
				@param atom_container the AtomContainer to add
		*/
		void insert(AtomContainer& atom_container)
			throw();

		/** Insert a AtomContainer before a given {\em Comosite} object.
				@param atom_container, the AtomContainer to insert
				@param before, the {\em Comosite} object to insert before
		*/
		void insertBefore(AtomContainer& atom_container, Composite& before)
			throw();

		/** Insert a AtomContainer after a given {\em Comosite} object.
				@param atom_container, the AtomContainer to insert
				@param after, the {\em Comosite} object to insert after
		*/
		void insertAfter(AtomContainer& atom_container, Composite& after)
			throw();

		/**	Cut all children of {\tt atom_container} and prepend them before the children of this molecule.
				@param atom_container the AtomContainer to access
		*/
		void spliceBefore(AtomContainer& atom_container)
			throw();

		/**	Cut all children of {\tt atom_container} and append them after the children of this molecule.
				@param atom_container the AtomContainer to access
		*/
		void spliceAfter(AtomContainer& atom_container)
			throw();

		/**	Move the children of atom_container into this molecule.
				The children of {\tt atom_container} are inserted at the position of 
				{\tt atom_container} if {\tt atom_container} is a child of {\tt this}.
				Otherwise the children are inserted using \Ref{spliceBefore}.
		*/
		void splice(AtomContainer& atom_container)
			throw();

		/** Remove a AtomContainer.
				@param atom_container the AtomContainer to remove
		*/
		bool remove(AtomContainer& atom_container)
			throw();

		//@}

	
		/**	@name Debugging and Diagnostics */
		//@{

		/** Internal state dump.
				Dump the current internal state of {\em *this} molecule to the 
				output ostream {\em s} with dumping depth {\em depth}.
				@param	s output stream where to output the internal state of {\em *this} molecule
				@param  depth the dumping depth
		*/
		virtual void dump(std::ostream& s = std::cout, Size depth = 0) const
			throw();

		//@}


		/**	@name	Storers */
		//@{

		/**	Read the components of a molecule from an instream.
				\\
				{\bf Note:} Not yet implemented.	
				@param s the instream
		*/
		virtual void read(std::istream& s)
			throw();

		/**	Write the components of a molecule to an outstream.
				\\
				{\bf Note:} Not yet implemented.	
				@param s the outstream
		*/
		virtual void write(std::ostream& s) const
			throw();
		
		//@}

		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(Fragment)


		protected:

		Molecule* getMolecule()
			throw();

		const Molecule* getMolecule() const
			throw();

		AtomContainer* getSuperAtomContainer()
			throw();

		const AtomContainer* getSuperAtomContainer() const
			throw();

		void prepend(Molecule& molecule)
			throw();

		void append(Molecule& molecule)
			throw();

		void insert(Molecule& molecule)
			throw();

		void insertBefore(Molecule& molecule, Composite& composite)
			throw();

		void insertAfter(Molecule& molecule, Composite& composite)
			throw();

		bool remove(Molecule& molecule)
			throw();

		bool isSubAtomContainerOf(const AtomContainer& atom_container) const
			throw();
	};

} // namespace BALL 

#endif // BALL_KERNEL_MOLECULE_H
