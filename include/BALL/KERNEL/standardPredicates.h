// $Id: standardPredicates.h,v 1.12 2001/02/04 10:29:05 amoll Exp $

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

#ifndef BALL_KERNEL_BOND_H
#	include <BALL/KERNEL/bond.h>
#endif

#ifndef BALL_KERNEL_EXPRESSION_H
#	include <BALL/KERNEL/expression.h>
#endif


namespace BALL 
{

	/**	@name	Predefined Predicates
			@see ExpressionPredicate
			\\
			{\bf Definition:}\URL{BALL/KERNEL/standardPredicates.h}
			\\
	*/
	//@{

	/** Default predicate.
			This predicate may be used to implement
			default rules. It is always {\bf true}.
	 */
	class TruePredicate
		:	public ExpressionPredicate
	{
		public:
			BALL_CREATE(TruePredicate)

			/** Evaluate the predicate for the atom {\tt atom}
					@param atom the atom to test
					@return true - always
			*/
			virtual bool operator () (const Atom& atom) const
				throw();
	};

	/** Selection predicate.
			This predicate is {\bf true}, if the atom is selected, {\bf false} otherwise.
	 */
	class SelectedPredicate
		:	public ExpressionPredicate
	{
		public:
			BALL_CREATE(SelectedPredicate)

			/** Evaluate the predicate for the atom {\tt atom}.
					@param atom the atom to test
					@return true - always
			*/
			virtual bool operator () (const Atom& atom) const
				throw();
	};

	/** Predicate for atom names
	 */
	class AtomNamePredicate
		:	public	ExpressionPredicate
	{
		public:
			BALL_CREATE(AtomNamePredicate)

			/** Evaluate the predicate for the atom {\tt atom}.
					@param atom the atom to test
					@return true, if the predicate is true, false otherwise
			*/
			virtual bool operator () (const Atom& atom) const
				throw();
	};

	/**
	 */
	class AtomTypePredicate
		:	public	ExpressionPredicate
	{
		public:
			BALL_CREATE(AtomTypePredicate)

			/** Evaluate the predicate for the atom {\tt atom}.
					@param atom the atom to test
					@return true, if the predicate is true, false otherwise
			*/
			virtual bool operator () (const Atom& atom) const
				throw();
	};

	/**
	 */
	class ElementPredicate
		:	public	ExpressionPredicate
	{
		public:
			BALL_CREATE(ElementPredicate)

			/** Evaluate the predicate for the atom {\tt atom}.
					@param atom the atom to test
					@return true, if the predicate is true, false otherwise
			*/
			virtual bool operator () (const Atom& atom) const
				throw();
	};

	/**
	 */
	class ResiduePredicate
		:	public	ExpressionPredicate
	{
		public:
			BALL_CREATE(ResiduePredicate)

			/** Evaluate the predicate for the atom {\tt atom}.
					@param atom the atom to test
					@return true, if the predicate is true, false otherwise
			*/
			virtual bool operator () (const Atom& atom) const
				throw();
	};

	/**
	 */
	class ResidueIDPredicate
		:	public	ExpressionPredicate
	{
		public:
			BALL_CREATE(ResidueIDPredicate)

			/** Evaluate the predicate for the atom {\tt atom}.
					@param atom the atom to test
					@return true, if the predicate is true, false otherwise
			*/
			virtual bool operator () (const Atom& atom) const
				throw();
	};

	/**
	 */
	class ProteinPredicate
		:	public	ExpressionPredicate
	{
		public:
			BALL_CREATE(ProteinPredicate)

			/** Evaluate the predicate for the atom {\tt atom}.
					@param atom the atom to test
					@return true, if the predicate is true, false otherwise
			*/
			virtual bool operator () (const Atom& atom) const
				throw();
	};

	/**
	 */
	class ChainPredicate
		:	public	ExpressionPredicate
	{
		public:
			BALL_CREATE(ChainPredicate)

			/** Evaluate the predicate for the atom {\tt atom}.
					@param atom the atom to test
					@return true, if the predicate is true, false otherwise
			*/
			virtual bool operator () (const Atom& atom) const
				throw();
	};

	/**
	 */
	class SecondaryStructurePredicate
		:	public	ExpressionPredicate
	{
		public:
			BALL_CREATE(SecondaryStructurePredicate)

			/** Evaluate the predicate for the atom {\tt atom}.
					@param atom the atom to test
					@return true, if the predicate is true, false otherwise
			*/
			virtual bool operator () (const Atom& atom) const
				throw();
	};

	/**
	 */
	class SolventPredicate
		:	public	ExpressionPredicate
	{
		public:
			BALL_CREATE(SolventPredicate)

			/** Evaluate the predicate for the atom {\tt atom}.
					@param atom the atom to test
					@return true, if the predicate is true, false otherwise
			*/
			virtual bool operator () (const Atom& atom) const
				throw();
	};

	/**
	 */
	class MoleculePredicate
		:	public	ExpressionPredicate
	{
		public:
			BALL_CREATE(MoleculePredicate)

			/** Evaluate the predicate for the atom {\tt atom}.
					@param atom the atom to test
					@return true, if the predicate is true, false otherwise
			*/
			virtual bool operator () (const Atom& atom) const
				throw();
	};

	/**
	 */
	class BackBonePredicate
		:	public	ExpressionPredicate
	{
		public:
			BALL_CREATE(BackBonePredicate)

			/** Evaluate the predicate for the atom {\tt atom}.
					@param atom the atom to test
					@return true, if the predicate is true, false otherwise
			*/
			virtual bool operator () (const Atom& atom) const
				throw();
	};

	/**
	 */
	class NucleicAcidPredicate
		:	public	ExpressionPredicate
	{
		public:
			BALL_CREATE(NucleicAcidPredicate)

			/** Evaluate the predicate for the atom {\tt atom}.
					@param atom the atom to test
					@return true, if the predicate is true, false otherwise
			*/
			virtual bool operator () (const Atom& atom) const
				throw();
	};

	/**
	 */
	class NucleotidePredicate
		:	public	ExpressionPredicate
	{
		public:
			BALL_CREATE(NucleotidePredicate)

			/** Evaluate the predicate for the atom {\tt atom}.
					@param atom the atom to test
					@return true, if the predicate is true, false otherwise
			*/
			virtual bool operator () (const Atom& atom) const
				throw();
	};

	/** Predicate for atoms being included in rings of a certain number.
	 */
	class InRingPredicate
		:	public	ExpressionPredicate
	{
		public:

			BALL_CREATE(InRingPredicate)

			InRingPredicate();
			
			/** Evaluate the predicate for the atom {\tt atom}.
					@param atom the atom to test
					@return true, if the predicate is true, false otherwise
			*/
			virtual bool operator () (const Atom& atom) const
				throw();

		private:
			bool dfs(const Atom& atom, const Atom& first_atom, 
				const Size limit, HashSet<const Bond*>& visited) const;
	};

	/** Predicate for bearing double bonds. 
			Arguments of this class are	\emph{required} to consist 
			of an relational operator and a number between 1 and 8.
	 */
	class DoubleBondsPredicate
		:	public	ExpressionPredicate
	{
		public:
			BALL_CREATE(DoubleBondsPredicate)

			/** Evaluate the predicate for the atom {\tt atom}.
					@param atom the atom to test
					@return true, if the predicate is true, false otherwise
			*/
			virtual bool operator () (const Atom& atom) const
				throw();

		protected:
			bool testPredicate_(const Atom& atom, Bond::Order order) const;
	};

	/** Predicate class for atoms bearing single bonds.
	 */
	class SingleBondsPredicate
		:	public DoubleBondsPredicate
	{
		public:
			BALL_CREATE(SingleBondsPredicate)

			/** Evaluate the predicate for the atom {\tt atom}.
					@param atom the atom to test
					@return true, if the predicate is true, false otherwise
			*/
			virtual bool operator () (const Atom& atom) const
				throw();
	};

	/** Predicate class for atoms bearing triple bonds.
	 */
	class TripleBondsPredicate
		:	public DoubleBondsPredicate
	{
		public:
			BALL_CREATE(TripleBondsPredicate)

			/** Evaluate the predicate for the atom {\tt atom}.
					@param atom the atom to test
					@return true, if the predicate is true, false otherwise
				*/
			virtual bool operator () (const Atom& atom) const
				throw();
	};

	/** Predicate class for atoms bearing aromatic bonds.
	 */
	class AromaticBondsPredicate
		:	public DoubleBondsPredicate
	{
		public:
			BALL_CREATE(AromaticBondsPredicate)

			/** Evaluate the predicate for the atom {\tt atom}.
					@param atom the atom to test
					@return true, if the predicate is true, false otherwise
			*/
			virtual bool operator () (const Atom& atom) const
				throw();
	};

	/** Predicate class for atoms bearing a certain number of bonds.
	 */
	class NumberOfBondsPredicate
		:	public	ExpressionPredicate
	{
		public:
			BALL_CREATE(NumberOfBondsPredicate)

			/** Evaluate the predicate for the atom {\tt atom}.
					@param atom the atom to test
					@return true, if the predicate is true, false otherwise
			*/
			virtual bool operator () (const Atom& atom) const
				throw();
	};

	/** Predicate class for atoms being connected to a constellation defined
			by an expression. Syntax:
			\begin{tabular}{cl}
				Bonds \\
				{\tt -}  & single \\
				{\tt =}  & double \\
				{\tt \#} & triple \\
				{\tt \~} & aromatic \\
				{\tt .}  & any \\
				Elements \\
				{\tt symbol} & the desired element \\
				{\tt *} & any element \\
				Groups \\
				{\tt ( )} & symbols enlosed by brackets denote groups \\
			\end{tabular}
	*/
	class ConnectedToPredicate
		:	public	ExpressionPredicate
	{
		public:
			BALL_CREATE(ConnectedToPredicate)

			/** Evaluate the predicate for the atom {\tt atom}.
					@param atom the atom to test
					@return true, if the predicate is true, false otherwise
			*/
			virtual bool operator () (const Atom& atom) const
				throw();
		
		private:
			bool parse(const String& group, 
					std::list< std::pair<String, String> >& subs) const;
			bool bondOrderMatch(const String& bond_description, 
					const Bond::Order order) const;
			bool find(const String& group, const Atom& atom, 
					const Bond* source) const;
			bool findAndTest(const String& group, const Atom& atom,
					const Bond* source) const;
	};

	/** Predicate class for atoms being sp hybridized ...
	 */
	class SpHybridizedPredicate
		:	public	ExpressionPredicate
	{
		public:
			BALL_CREATE(SpHybridizedPredicate)

			/** Evaluate the predicate for the atom {\tt atom}.
					@param atom the atom to test
					@return true, if the predicate is true, false otherwise
			*/
			virtual bool operator () (const Atom& atom) const
				throw();
	};

	/** Predicate class for atoms being sp2 hybridized.
	 */
	class Sp2HybridizedPredicate
		:	public	ExpressionPredicate
	{
		public:
			BALL_CREATE(Sp2HybridizedPredicate)

			/** Evaluate the predicate for the atom {\tt atom}.
					@param atom the atom to test
					@return true, if the predicate is true, false otherwise
			*/
			virtual bool operator () (const Atom& atom) const
				throw();
	};

	/** Predicate class for atoms being sp3 hybridized.
	 */
	class Sp3HybridizedPredicate
		:	public	ExpressionPredicate
	{
		public:
			BALL_CREATE(Sp3HybridizedPredicate)

			/** Evaluate the predicate for the atom {\tt atom}.
					@param atom the atom to test
					@return true, if the predicate is true, false otherwise
			*/
			virtual bool operator () (const Atom& atom) const
				throw();
	};

	//@}	


} // namespace BALL

#endif // BALL_KERNEL_STANDARDPREDICATES_H
