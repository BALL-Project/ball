// $Id: system.h,v 1.9 2000/05/09 16:21:34 amoll Exp $

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

	/** Kernel System Class.
			This class is used to represent a system, i.e., a collection
			of molecules.\\
			{\bf Definition:}\URL{BALL/KERNEL/system.h}
	*/
	class System
		: public BaseFragment
	{
		public:

		BALL_CREATE(System)

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

		/** Assignment with cloning facility.
				Assign {\em system} to {\em *this}.
				The assignment is either deep or shallow (default).
				@param  system the System to be copied (cloned)
				@param  deep make a deep (={\tt true}) or shallow (={\tt false}) copy of {\em system}
		*/
		void set(const System& system, bool deep = true);

		/** Assignment operator.
				Assign {\em system} to {\em *this}.
				@param   system the System to be copied (cloned)
				@return  System& - {\em *this}
				@see     System::set
		*/
		System& operator = (const System& system);

		/** Copying with cloning facility.
				Copy {\em *this} to {\em system}.
				The assignment is either deep or shallow (default).
				@param  System the System to be assigned to
				@see    System::set
		*/
		void get(System& system, bool deep = true) const;
	
		//@}


		/**	@name	Accessors */
		//@{

		/** Get a pointer to a child Molecule at a given position.
				The reference is 0 if {\em *this} does not have a Molecule at this position.
				@param   position the position of the child molecule
				@return  Molecule* -
								 mutable reference to the child molecule at {\em position} of {\em *this},
		*/
		Molecule* getMolecule(Position position);

		/** Get a pointer to a child Molecule at a given position.
				The reference is 0 if {\em *this} does not have a Molecule at this position.
				@param   position the position of the child molecule
				@return  Molecule* -
								 constant reference to the child molecule at {\em position} of {\em *this},
		*/
		const Molecule* getMolecule(Position position) const;

		/** Count the molecules in this system
				@return Size the number of molecules
		*/
		Size countMolecules() const;

		/** Count the fragments in this system
				@return Size the number of fragments
		*/
		Size countFragments() const;

		/** Count the atoms in this system
				@return Size the number of atoms
		*/
		Size countAtoms() const;

		/** Prepend a molecule at position 0.
				@param molecule, the molecule to prepend
		*/
		void prepend(Molecule& molecule);

		/** Append a molecule after the last position.
				@param molecule, the molecule to append
		*/
		void append(Molecule& molecule);

		/** Insert a molecule after the last position.
				@param molecule, the molecule to insert
		*/
		void insert(Molecule& molecule);

		/** Insert a molecule before a given {\em Comosite} object.
				@param molecule, the molecule to insert
				@param before, the {\em Comosite} object to insert before
		*/
		void insertBefore(Molecule& molecule, Composite& before);

		/** Insert a molecule after a given {\em Comosite} object.
				@param molecule, the molecule to insert
				@param after, the {\em Comosite} object to insert before
		*/
		void insertAfter(Molecule& molecule, Composite& after);

		/** Remove a molecule.
				@param molecule the molecule to remove
		*/
		bool remove(Molecule& molecule);

		/**	Move the children of {\tt system} into {\em *this}.
				Cut all children of {\tt system} and prepend them before the children of {\em *this}.
				@param system the system to access
		*/
		void spliceBefore(System& system);

		/**	Move the children of {\tt system} into {\em *this}.
				Cut all children of {\tt system} and append them after the children of {\em *this}.
				@param system the system to access
		*/
		void spliceAfter(System& system);

		/**	Move the children of {\tt system} into {\em *this}.
				The children are inserted using \Ref{spliceBefore}.
		*/
		void splice(System& system);		
		//@}

		/**	@name 	Miscellaneous */
		//@{

		/// Removes all bonds contained in the system	
		void destroyBonds();

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
	};

} // namespace BALL

#endif // BALL_KERNEL_SYSTEM_H
