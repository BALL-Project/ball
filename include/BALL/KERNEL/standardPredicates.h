// $Id: standardPredicates.h,v 1.2 2000/05/22 17:41:11 anker Exp $

#ifndef BALL_KERNEL_STANDARDPREDICATES_H
#define BALL_KERNEL_STANDARDPREDICATES_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_DATATYPE_STRINGHASHMAP_H
#	include <BALL/DATATYPE/stringHashMap.h>
#endif

#ifndef BALL_KERNEL_ATOM_H
#	include <BALL/KERNEL/atom.h>
#endif

#ifndef BALL_KERNEL_EXPRESSION_H
#	include <BALL/KERNEL/expression.h>
#endif


namespace BALL 
{

	/**	@name	Predefined Predicates
		@see ExpressionPredicate
	 */
	//@{

	/** Predicate for atom names
	 */
	class AtomNamePredicate
		:	public	ExpressionPredicate
		{
			public:
				BALL_CREATE_NODEEP(AtomNamePredicate)
				/** Evaluate the predicate for the atom {\tt atom}
					@param atom the atom to test
					@return true, if the predicate is true, false otherwise
					*/
				virtual bool operator () (const Atom& atom) const;
		};

	/**
	 */
	class AtomTypePredicate
		:	public	ExpressionPredicate
		{
			public:
				BALL_CREATE_NODEEP(AtomTypePredicate)
				/** Evaluate the predicate for the atom {\tt atom}
					@param atom the atom to test
					@return true, if the predicate is true, false otherwise
					*/
				virtual bool operator () (const Atom& atom) const;
		};

	/**
	 */
	class ElementPredicate
		:	public	ExpressionPredicate
		{
			public:
				BALL_CREATE_NODEEP(ElementPredicate)
				/** Evaluate the predicate for the atom {\tt atom}
					@param atom the atom to test
					@return true, if the predicate is true, false otherwise
					*/
				virtual bool operator () (const Atom& atom) const;
		};

	/**
	 */
	class ResiduePredicate
		:	public	ExpressionPredicate
		{
			public:
				BALL_CREATE_NODEEP(ResiduePredicate)
				/** Evaluate the predicate for the atom {\tt atom}
					@param atom the atom to test
					@return true, if the predicate is true, false otherwise
					*/
				virtual bool operator () (const Atom& atom) const;
		};

	/**
	 */
	class ResidueIDPredicate
		:	public	ExpressionPredicate
		{
			public:
				BALL_CREATE_NODEEP(ResidueIDPredicate)
				/** Evaluate the predicate for the atom {\tt atom}
					@param atom the atom to test
					@return true, if the predicate is true, false otherwise
					*/
				virtual bool operator () (const Atom& atom) const;
		};

	/**
	 */
	class ProteinPredicate
		:	public	ExpressionPredicate
		{
			public:
				BALL_CREATE_NODEEP(ProteinPredicate)
				/** Evaluate the predicate for the atom {\tt atom}
					@param atom the atom to test
					@return true, if the predicate is true, false otherwise
					*/
				virtual bool operator () (const Atom& atom) const;
		};

	/**
	 */
	class ChainPredicate
		:	public	ExpressionPredicate
		{
			public:
				BALL_CREATE_NODEEP(ChainPredicate)
				/** Evaluate the predicate for the atom {\tt atom}
					@param atom the atom to test
					@return true, if the predicate is true, false otherwise
					*/
				virtual bool operator () (const Atom& atom) const;
		};

	/**
	 */
	class SecondaryStructurePredicate
		:	public	ExpressionPredicate
		{
			public:
				BALL_CREATE_NODEEP(SecondaryStructurePredicate)
				/** Evaluate the predicate for the atom {\tt atom}
					@param atom the atom to test
					@return true, if the predicate is true, false otherwise
					*/
				virtual bool operator () (const Atom& atom) const;
		};

	/**
	 */
	class SolventPredicate
		:	public	ExpressionPredicate
		{
			public:
				BALL_CREATE_NODEEP(SolventPredicate)
				/** Evaluate the predicate for the atom {\tt atom}
					@param atom the atom to test
					@return true, if the predicate is true, false otherwise
					*/
				virtual bool operator () (const Atom& atom) const;
		};

	/**
	 */
	class MoleculePredicate
		:	public	ExpressionPredicate
		{
			public:
				BALL_CREATE_NODEEP(MoleculePredicate)
				/** Evaluate the predicate for the atom {\tt atom}
					@param atom the atom to test
					@return true, if the predicate is true, false otherwise
					*/
				virtual bool operator () (const Atom& atom) const;
		};

	/**
	 */
	class BackBonePredicate
		:	public	ExpressionPredicate
		{
			public:
				BALL_CREATE_NODEEP(BackBonePredicate)
				/** Evaluate the predicate for the atom {\tt atom}
					@param atom the atom to test
					@return true, if the predicate is true, false otherwise
					*/
				virtual bool operator () (const Atom& atom) const;
		};


	/**
	 */
	class NucleicAcidPredicate
		:	public	ExpressionPredicate
		{
			public:
				BALL_CREATE_NODEEP(NucleicAcidPredicate)
				/** Evaluate the predicate for the atom {\tt atom}
					@param atom the atom to test
					@return true, if the predicate is true, false otherwise
					*/
				virtual bool operator () (const Atom& atom) const;
		};

	/**
	 */
	class NucleotidePredicate
		:	public	ExpressionPredicate
		{
			public:
				BALL_CREATE_NODEEP(NucleotidePredicate)
				/** Evaluate the predicate for the atom {\tt atom}
					@param atom the atom to test
					@return true, if the predicate is true, false otherwise
					*/
				virtual bool operator () (const Atom& atom) const;
		};

	/** Predicate for atoms being included in rings of a certain number.
	 */
	class InRingPredicate
		:	public	ExpressionPredicate
		{
			public:

				BALL_CREATE_NODEEP(InRingPredicate)

				InRingPredicate();
				/** Evaluate the predicate for the atom {\tt atom}
					@param atom the atom to test
					@return true, if the predicate is true, false otherwise
					*/
				virtual bool operator () (const Atom& atom) const;

			private:
				static Size limitcount;
				bool dfs(const Atom* atom, const Atom* first_atom, 
					const int limit) const;
		};

	/** Predicate for bearing double bonds. Arguments of this class are
			\emph{required} to consist of an relational operator and a number
			between 1 and 8.
	 */
	class doubleBondsPredicate
		:	public	ExpressionPredicate
		{
			public:
				BALL_CREATE_NODEEP(doubleBondsPredicate)
				/** Evaluate the predicate for the atom {\tt atom}
					@param atom the atom to test
					@return true, if the predicate is true, false otherwise
					*/
				virtual bool operator () (const Atom& atom) const;
		};

	/** Predicate class for atoms bearing triple bonds.
	 */
	class tripleBondsPredicate
		:	public	ExpressionPredicate
		{
			public:
				BALL_CREATE_NODEEP(tripleBondsPredicate)
				/** Evaluate the predicate for the atom {\tt atom}
					@param atom the atom to test
					@return true, if the predicate is true, false otherwise
					*/
				/** Evaluate the predicate for the atom {\tt atom}
					@param atom the atom to test
					@return true, if the predicate is true, false otherwise
					*/
				virtual bool operator () (const Atom& atom) const;
		};

	/** Predicate class for atoms bearing aromatic bonds.
	 */
	class aromaticBondsPredicate
		:	public	ExpressionPredicate
		{
			public:
				BALL_CREATE_NODEEP(aromaticBondsPredicate)
				/** Evaluate the predicate for the atom {\tt atom}
					@param atom the atom to test
					@return true, if the predicate is true, false otherwise
					*/
				virtual bool operator () (const Atom& atom) const;
		};

	/** Predicate class for atoms bearing a certain number of bonds.
	 */
	class numberOfBondsPredicate
		:	public	ExpressionPredicate
		{
			public:
				BALL_CREATE_NODEEP(numberOfBondsPredicate)
				/** Evaluate the predicate for the atom {\tt atom}
					@param atom the atom to test
					@return true, if the predicate is true, false otherwise
					*/
				virtual bool operator () (const Atom& atom) const;
		};

	/** Predicate class for atoms being connected to a constellation defined
			by an expression. Syntax:
			\begin{tabular}{cl}
				Bonds \\
				{\tt -} & single \\
				{\tt =} & double \\
				{\tt \#} & triple \\
				{\tt ~} & aromatic \\
				{\tt .} & any \\
				\hline
				Elements \\
				{\tt symbol} & the desired element \\
				{\tt *} & any element \\
				Groups \\
				{\tt ( )} & symbols enlosed by brackets denote groups \\
			\end{tabular}

	 */
	class connectedToPredicate
		:	public	ExpressionPredicate
		{
			public:
				BALL_CREATE_NODEEP(connectedToPredicate)
				/** Evaluate the predicate for the atom {\tt atom}
					@param atom the atom to test
					@return true, if the predicate is true, false otherwise
					*/
				virtual bool operator () (const Atom& atom) const;
			
			private:
				bool parse(const String& group, list<String>* subs) const;
				bool find(const String& group, HashSet<Bond*>* found) const;
		};

	/** Predicate class for atoms being sp hybridized ...
	 */
	class spHybridizedPredicate
		:	public	ExpressionPredicate
		{
			public:
				BALL_CREATE_NODEEP(spHybridizedPredicate)
				/** Evaluate the predicate for the atom {\tt atom}
					@param atom the atom to test
					@return true, if the predicate is true, false otherwise
					*/
				virtual bool operator () (const Atom& atom) const;
		};

	/** Predicate class for atoms being sp2 hybridized
	 */
	class sp2HybridizedPredicate
		:	public	ExpressionPredicate
		{
			public:
				BALL_CREATE_NODEEP(sp2HybridizedPredicate)
				/** Evaluate the predicate for the atom {\tt atom}
					@param atom the atom to test
					@return true, if the predicate is true, false otherwise
					*/
				virtual bool operator () (const Atom& atom) const;
		};

	/** Predicate class for atoms being sp3 hybridized
	 */
	class sp3HybridizedPredicate
		:	public	ExpressionPredicate
		{
			public:
				BALL_CREATE_NODEEP(sp3HybridizedPredicate)
				/** Evaluate the predicate for the atom {\tt atom}
					@param atom the atom to test
					@return true, if the predicate is true, false otherwise
					*/
				virtual bool operator () (const Atom& atom) const;
		};

	//@}	


} // namespace BALL

#endif // BALL_KERNEL_STANDARDPREDICATES_H
