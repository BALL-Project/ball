// $Id: protein.h,v 1.2 1999/12/04 18:34:18 oliver Exp $ 

#ifndef BALL_KERNEL_PROTEIN_H
#define BALL_KERNEL_PROTEIN_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_KERNEL_CHAIN_H
#	include <BALL/KERNEL/chain.h>
#endif

#ifndef BALL_KERNEL_CHAINITERATOR_H
#	include <BALL/KERNEL/chainIterator.h>
#endif

#ifndef BALL_KERNEL_MOLECULE_H
#	include <BALL/KERNEL/molecule.h>
#endif

#ifndef BALL_KERNEL_PDBATOM_H
#	include <BALL/KERNEL/PDBAtom.h>
#endif

#ifndef BALL_KERNEL_RESIDUE_H
#	include <BALL/KERNEL/residue.h>
#endif

#ifndef BALL_KERNEL_SECONDARYSTRUCTURE_H
#	include <BALL/KERNEL/secondaryStructure.h>
#endif


#define BALL_PROTEIN_DEFAULT_ID   ""

namespace BALL 
{

	/**
	*/
	class Protein
		: public Molecule
	{
		public:

		BALL_CREATE(Protein)

		/**	@name	Enums
		*/
		//@{
			
		/**
		*/
		enum Property
		{
			NUMBER_OF_PROPERTIES = Molecule::NUMBER_OF_PROPERTIES
		};
		//@}

		/** @name	Constructors and Destructors */
		//@{

		///
		Protein();
	
		///
		Protein(const Protein& protein, bool deep = true);
	
		///
		Protein(const String& name, const String& id = BALL_PROTEIN_DEFAULT_ID);

		///
		virtual ~Protein();
	
		///
		virtual void clear();
	
		///
		virtual void destroy();

		//@}


		/**	@name	Persistence */
		//@{
		
		///
		void persistentWrite(PersistenceManager& pm, const char* name = 0) const;

		///
		void persistentRead(PersistenceManager& pm);

		//@}


		/**	@name Assignment Methods */
		//@{

		/**
		*/
		void set(const Protein& protein, bool deep = true);

		/**
		*/
		Protein& operator = (const Protein& protein);

		/**
		*/
		void get(Protein& protein, bool deep = true) const;
	
		/**
		*/
		void swap(Protein& protein);
	
		//@}


		/**	@name Accessors */
		//@{

		///
		Chain* getChain(Index index);

		///		
		const Chain* getChain(Index index) const;

		///		
		SecondaryStructure* getSecondaryStructure(Index index);

		///		
		const SecondaryStructure* getSecondaryStructure(Index index) const;

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
		void setID(const String& id);

		///
		String& getID();

		///
		const String& getID() const;

		///
		Size countChains() const;

		///
		Size countSecondaryStructures() const;

		///
		Size countResidues() const;

		///
		Size countPDBAtoms() const;
		//@}

#ifdef	BALL_CFG_USING_METHOD_DIRECTIVE
		using Molecule::prepend;
		using Molecule::append;
		using Molecule::insert;
		using Molecule::insertBefore;
		using Molecule::insertAfter;
		using Molecule::remove;
		using Molecule::splice;
		using Molecule::spliceBefore;
		using Molecule::spliceAfter;
#else
		Molecule::prepend;
		Molecule::append;
		Molecule::insert;
		Molecule::insertBefore;
		Molecule::insertAfter;
		Molecule::remove;
		Molecule::splice;
		Molecule::spliceBefore;
		Molecule::spliceAfter;
#endif

		/**	@name	Debugging and Diagnostics
		*/
		//@{

		/**
		*/
		virtual bool isValid() const;

		/**
		*/
		virtual void dump(ostream& s = cout, unsigned long depth = 0) const;
		//@}

		/**	@name	Storers
		*/
		//@{

		/**
		*/
		virtual void read(istream& s);

		/**
		*/
		virtual void write(ostream& s) const;
		//@}

		// --- EXTERNAL ITERATORS

		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(Chain)
		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(SecondaryStructure)
		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(Residue)
		BALL_KERNEL_DEFINE_ITERATOR_CREATORS(PDBAtom)

		
		private:

		void clear_();

		// --- ATTRIBUTES

		String id_;
	};

} // namespace BALL

#endif // BALL_KERNEL_PROTEIN_H
