// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: standardPredicates.h,v 1.23 2002/12/19 13:33:17 anker Exp $

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

	/**	@name	Predefined Predicates. 
			{\bf Note:} These predicates do not provide the OCI.
			@see ExpressionPredicate
			{\bf Definition:}\URL{BALL/KERNEL/standardPredicates.h}
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

	/** False predicate.
			This predicate may be used to implement
			default rules. It is always {\bf false}.
	 */
	class FalsePredicate
		:	public ExpressionPredicate
	{
		public:
		BALL_CREATE(FalsePredicate)

		/** Evaluate the predicate for the atom {\tt atom}
				@param atom the atom to test
				@return false - always
		*/
		virtual bool operator () (const Atom& atom) const
			throw();
	};

	/** Selection predicate.
			This predicate is {\bf true}, if the atom is selected, {\bf false}
			otherwise.
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

	/** Predicate for matching atom names. Returns {\tt true}, if the name of
			the atom matches the argument of this predicate. This is an exact
			match.
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

	/** Predicate for matching atom types. Returns {\tt true}, if the type name
			of the atom matches exactly the argument of this predicate. 
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

	/** Predicate for matching elements. Returns {\tt true}, if the element
			symbol of the atom matches exactly the argument  of this predicate.
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

	/** Predicate for matching residues. Returns {\tt true}, if the name of the 
			residue that the atom belongs to matches exactly the argument of this
			predicate.
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

	/** Predicate for matching residues IDs. Returns {\tt true}, if the ID of
			the residue that the atom belongs to matches exactly the argument of
			this predicate.
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

	/** Predicate for matching proteins. Returns {\tt true}, if the name of the
			protein that the atom belongs to matches exactly the argument of this
			predicate.
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

	/** Predicate for matching chains. Returns {\tt true}, if the name of the
			chain that the atom belongs to matches exactly the argument of this
			expression.
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

	/** Predicate for matching secondary structures. Returns {\tt true}, if the
			name of the secondary structure that the atom belongs to matches
			exactly the argument of this predicate.
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

	/** Predicate indicating solvent atoms. 
			Returns {\tt true}, if the atom
			belongs to a solvent molecule. This predicate is {\bf true} if the 
			atom is contained in a molecule with the property \Ref{Molecule::IS_SOLVENT}
			set. This is usually the case for solvents added automatically by a
			force field.
			@see PeriodicBoundary::addSolvent
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

	/** Predicate for matching molecules. Returns {\tt true}, if the atom
			belongs to the molecule defined by the expression of this predicate.
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

	/** Predicate indicating backbone atoms. Returns {\tt true}, if the atom
			belongs to the backbone of a protein, i. e. is one of the members of
			the peptide bond.
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

	/** Predicate indicating nucleotide atoms. Returns {\tt true}, if the
			atom belongs to a nucleotide.
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

			/** Evaluate the predicate for the atom {\tt atom}.
					@param atom the atom to test
					@return true, if the predicate is true, false otherwise
			*/
			virtual bool operator () (const Atom& atom) const
				throw();

		private:
			bool dfs_(const Atom& atom, const Atom& first_atom, 
					const Size limit, HashSet<const Bond*>& visited) const
				throw();
				
	};


	/** Predicate for atoms bearing double bonds. 
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
			bool testPredicate_(const Atom& atom, Bond::Order order) const
				throw();
	};


	/** Predicate class for atoms bearing single bonds.
			Arguments of this class are	\emph{required} to consist 
			of an relational operator and a number between 1 and 8.
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
			Arguments of this class are	\emph{required} to consist 
			of an relational operator and a number between 1 and 8.
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
			Arguments of this class are	\emph{required} to consist 
			of an relational operator and a number between 1 and 8.
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
			Arguments of this class are	\emph{required} to consist 
			of an relational operator and a number between 1 and 8.
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
        {\tt -}            & single \\   
        {\tt =}            & double \\   
        {\tt \#}           & triple \\     
        {\tt \symbol{126}} & aromatic \\     
        {\tt .}            & any \\   
        Elements           & \\
        {\tt symbol}       & the desired element \\   
        {\tt *}            & any element \\   
        Groups             & \\
        {\tt ( )}          & symbols enlosed by brackets denote groups \\   
      \end{tabular}
	*/
	class ConnectedToPredicate
		:	public	ExpressionPredicate
	{

		private:

			class CTPNode
			{
				public:

					enum BondType
					{
						BONDTYPE__UNINITIALISED = 0,
						BONDTYPE__ANY           = 1,
						BONDTYPE__SINGLE        = 2,
						BONDTYPE__DOUBLE        = 3,
						BONDTYPE__TRIPLE        = 4,
						BONDTYPE__QUADRUPLE     = 5,
						BONDTYPE__AROMATIC      = 6
					};

					typedef std::list<CTPNode*>::iterator Iterator;
					typedef std::list<CTPNode*>::const_iterator ConstIterator;

					/**
					 */
					CTPNode()
						throw();

					/**
					 */
					CTPNode(const CTPNode& node)
						throw();

					/**
					 */
					virtual ~CTPNode()
						throw();

					/**
					*/
					virtual void destroy()
						throw();

					/**
					*/
					virtual void clear()
						throw();

					/**
					 */
					void setParent(CTPNode* parent)
						throw();

					/**
					 */
					CTPNode* getParent() const
						throw();

					/**
					 */
					void addChild(CTPNode* child)
						throw();

					/**
					 */
					void removeChild(CTPNode* child)
						throw();

					Iterator begin()
						throw();

					ConstIterator begin() const
						throw();

					Iterator end()
						throw();

					ConstIterator end() const
						throw();

					/**
					 */
					Size getNumberOfChildren() const
						throw();

					/**
					 */
					void setBondType(Size type)
						throw();

					/**
					 */
					void setBondType(char type)
						throw();

					/**
					 */
					Size getBondType() const
						throw();

					/**
					 */
					char getBondTypeChar() const
						throw();

					/**
					 */
					String getSymbol() const
						throw();

					/**
					 */
					void setSymbol(const String& symbol)
						throw();

					/**
					 */
					void setFinished()
						throw();

					/**
					 */
					void unsetFinished()
						throw();

					/**
					 */
					bool isFinished() const
						throw();

					/**
					 */
					void setLinked()
						throw();

					/**
					 */
					void unsetLinked()
						throw();

					/**
					 */
					bool isLinked() const
						throw();

					/**
					 */
					void linkWith(const CTPNode* partner)
						throw();

					/**
					 */
					const std::list<const CTPNode*>& getLinkList() const
						throw();


				private: 

					/*_
					 */
					String element_symbol_;

					/*_ Bond type means the bond connecting *to* this node.
					 */
					Size bond_type_;

					/*_
					 */
					std::list<CTPNode*> children_;

					/*_
					 */
					CTPNode* parent_;

					/*_
					 */
					bool finished_;

					/*_
					 */
					bool linked_;

					/*_
					 */
					list<const CTPNode*> link_list_;
			};

		public:
			BALL_CREATE(ConnectedToPredicate)

			ConnectedToPredicate()
				throw();

			virtual ~ConnectedToPredicate()
				throw();

			/** Evaluate the predicate for the atom {\tt atom}.
					@param atom the atom to test
					@return true, if the predicate is true, false otherwise
			*/
			virtual bool operator () (const Atom& atom) const
				throw();
		
			void dump() const
				throw();

			void dump(const CTPNode* current) const
				throw();

			virtual void setArgument(const String& argument)
				throw();

		private:
			/*_ The syntax tree
			*/
			CTPNode* tree_;

			/*_ Needed for realising parsing of loops (or links).
			*/
			HashMap<char, pair<CTPNode*, CTPNode*> > link_map_;

			/*_ Needed for realising parsing of loops (or links).
			*/
			char link_mark_;

			/*_ Keep it consistent
			*/
			CTPNode* createNewNode_(CTPNode* node)
				throw();
		
			/*_
			*/
			CTPNode* parse_()
				throw();

			/*_
			*/
			CTPNode* parse_(const String& input)
				throw();

			bool bondOrderMatch_(const Bond& bond, const CTPNode& node) const
				throw();

			bool find_(const Atom& atom, const CTPNode* current,
					HashSet<const Bond*>& visited) const
				throw();

	};

	/** Predicate indicating sp hybridized atoms.
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

	/** Predicate indicating sp2 hybridized atoms.
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


	/** Predicate indicating sp3 hybridized atoms.
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

	/** Charge predicate
	 */
	class ChargePredicate
		:	public	ExpressionPredicate
	{
		public:
		BALL_CREATE(ChargePredicate)

		/** Return true if the charge is in the indicated range.
				The argument of this predicate is either a number,
				in which case the charge of the atom has to be equal to 
				that number (down to the specified accuracy for floating 
				point comparisons, \Ref{EPSILON}), or an operator followed	
				by a number. Possible operators are: {\tt <}, {\tt >}, {\tt >=}, {\tt <=}, {\tt =}.
				@param atom the atom to test
				@return true, if the predicate is true, false otherwise
		*/
		virtual bool operator () (const Atom& atom) const
			throw();
	};

	//@}	

} // namespace BALL

#endif // BALL_KERNEL_STANDARDPREDICATES_H
