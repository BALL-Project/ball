// $Id: standardPredicates.h,v 1.1 2000/05/19 12:23:12 anker Exp $

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
	 */
	//@{

	/**
	 */
	class AtomNamePredicate
		:	public	ExpressionPredicate
		{
			public:
				BALL_CREATE_NODEEP(AtomNamePredicate)
					virtual bool operator () (const Atom& atom) const;
		};

	/**
	 */
	class AtomTypePredicate
		:	public	ExpressionPredicate
		{
			public:
				BALL_CREATE_NODEEP(AtomTypePredicate)
					virtual bool operator () (const Atom& atom) const;
		};

	/**
	 */
	class ElementPredicate
		:	public	ExpressionPredicate
		{
			public:
				BALL_CREATE_NODEEP(ElementPredicate)
					virtual bool operator () (const Atom& atom) const;
		};

	/**
	 */
	class ResiduePredicate
		:	public	ExpressionPredicate
		{
			public:
				BALL_CREATE_NODEEP(ResiduePredicate)
					virtual bool operator () (const Atom& atom) const;
		};

	/**
	 */
	class ResidueIDPredicate
		:	public	ExpressionPredicate
		{
			public:
				BALL_CREATE_NODEEP(ResidueIDPredicate)
					virtual bool operator () (const Atom& atom) const;
		};

	/**
	 */
	class ProteinPredicate
		:	public	ExpressionPredicate
		{
			public:
				BALL_CREATE_NODEEP(ProteinPredicate)
					virtual bool operator () (const Atom& atom) const;
		};

	/**
	 */
	class ChainPredicate
		:	public	ExpressionPredicate
		{
			public:
				BALL_CREATE_NODEEP(ChainPredicate)
					virtual bool operator () (const Atom& atom) const;
		};

	/**
	 */
	class SecondaryStructurePredicate
		:	public	ExpressionPredicate
		{
			public:
				BALL_CREATE_NODEEP(SecondaryStructurePredicate)
					virtual bool operator () (const Atom& atom) const;
		};

	/**
	 */
	class SolventPredicate
		:	public	ExpressionPredicate
		{
			public:
				BALL_CREATE_NODEEP(SolventPredicate)
					virtual bool operator () (const Atom& atom) const;
		};

	/**
	 */
	class MoleculePredicate
		:	public	ExpressionPredicate
		{
			public:
				BALL_CREATE_NODEEP(MoleculePredicate)
					virtual bool operator () (const Atom& atom) const;
		};

	/**
	 */
	class BackBonePredicate
		:	public	ExpressionPredicate
		{
			public:
				BALL_CREATE_NODEEP(BackBonePredicate)
					virtual bool operator () (const Atom& atom) const;
		};


	/**
	 */
	class NucleicAcidPredicate
		:	public	ExpressionPredicate
		{
			public:
				BALL_CREATE_NODEEP(NucleicAcidPredicate)
					virtual bool operator () (const Atom& atom) const;
		};

	/**
	 */
	class NucleotidePredicate
		:	public	ExpressionPredicate
		{
			public:
				BALL_CREATE_NODEEP(NucleotidePredicate)
					virtual bool operator () (const Atom& atom) const;
		};

	/** Predicate for atoms being included in rings of a certain number.
	 */
	class inRingPredicate
		:	public	ExpressionPredicate
		{
			public:
				BALL_CREATE_NODEEP(inRingPredicate)
					virtual bool operator () (const Atom& atom) const;
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
					virtual bool operator () (const Atom& atom) const;
		};

	/** Predicate class for atoms bearing triple bonds.
	 */
	class tripleBondsPredicate
		:	public	ExpressionPredicate
		{
			public:
				BALL_CREATE_NODEEP(tripleBondsPredicate)
					virtual bool operator () (const Atom& atom) const;
		};

	/** Predicate class for atoms bearing aromatic bonds.
	 */
	class aromaticBondsPredicate
		:	public	ExpressionPredicate
		{
			public:
				BALL_CREATE_NODEEP(aromaticBondsPredicate)
					virtual bool operator () (const Atom& atom) const;
		};

	/** Predicate class for atoms bearing a certain number of bonds.
	 */
	class numberOfBondsPredicate
		:	public	ExpressionPredicate
		{
			public:
				BALL_CREATE_NODEEP(numberOfBondsPredicate)
					virtual bool operator () (const Atom& atom) const;
		};

	/** Predicate class for atoms being connected to a constellation defined
			by an expression
	 */
	class connectedToPredicate
		:	public	ExpressionPredicate
		{
			public:
				BALL_CREATE_NODEEP(connectedToPredicate)
					virtual bool operator () (const Atom& atom) const;
		};

	/** Predicate class for atoms being sp hybridized ...
	 */
	class spHybridizedPredicate
		:	public	ExpressionPredicate
		{
			public:
				BALL_CREATE_NODEEP(spHybridizedPredicate)
					virtual bool operator () (const Atom& atom) const;
		};

	/** Predicate class for atoms being sp2 hybridized
	 */
	class sp2HybridizedPredicate
		:	public	ExpressionPredicate
		{
			public:
				BALL_CREATE_NODEEP(sp2HybridizedPredicate)
					virtual bool operator () (const Atom& atom) const;
		};

	/** Predicate class for atoms being sp3 hybridized
	 */
	class sp3HybridizedPredicate
		:	public	ExpressionPredicate
		{
			public:
				BALL_CREATE_NODEEP(sp3HybridizedPredicate)
					virtual bool operator () (const Atom& atom) const;
		};

	//@}	


} // namespace BALL

#endif // BALL_KERNEL_STANDARDPREDICATES_H
