// $Id: selector.h,v 1.10 2000/05/19 08:11:05 anker Exp $

#ifndef BALL_KERNEL_SELECTOR_H
#define BALL_KERNEL_SELECTOR_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_DATATYPE_STRINGHASHMAP_H
#	include <BALL/DATATYPE/stringHashMap.h>
#endif

#ifndef BALL_KERNEL_ATOM_H
#	include <BALL/KERNEL/atom.h>
#endif

namespace BALL 
{

	/**	@name	Kernel Object Selection.
			These classes allow the convenient selection of kernel objects.\\
			{\bf Definition:}\URL{BALL/KERNEL/selector.h}
	*/
	//@{

	/**
	*/
	class ExpressionPredicate
		:	public UnaryPredicate<Atom>
	{
		public:

		/**	@name	Constructors and Destructors.
		*/
		//@{

		/** Default Constructor
		*/
		ExpressionPredicate();
		
		/** Construct an ExpressionPredicate with a string.
				@param argument the argument to use for this predicate
		*/
		ExpressionPredicate(const String& argument);

		/** Destructor
		*/
		virtual ~ExpressionPredicate();
		//@}

		/** @name Accessors.
		*/
		//@{

		/** evaluate this ExpressionPredicate by comparing its argument with
				the selected predicate of atom.
				@param atom the atom to compare with
		*/
		virtual bool operator () (const Atom& atom) const = 0;

		/** set the Argument of this ExpressionPredicate
				@param argument the string to use for this predicate
		*/
		virtual void setArgument(const String& argument);
		//@}
		
		protected:
		
		String argument_;
	};

	class ExpressionTree
	{
		public:

		enum Type
		{	
			INVALID = 0,
			LEAF,
			OR,
			AND
		};

		/**	@name	Constructors and Destructors
		*/
		//@{
			
		/**	Default constructor.
				Create an empty expression node. The node's type is set to INVALID,
				{\tt negate_} is set to {\bf false}, the internal predicate is set
				to 0, and the list of children is empty.
		*/
		ExpressionTree();
			
		/**	Detailled construtor.
				Create an expression node representing a leaf, i.e., a 
				predicate.
				@param	predicate the node's predicate
				@param	negate set to {\bf true} if the node's predicate should be negated
		*/
		ExpressionTree(ExpressionPredicate* predicate, bool negate = false);

		/**
		*/
		ExpressionTree(Type type, list<ExpressionTree*> children, bool negate = false);

		/**	Destructor
		*/
		virtual ~ExpressionTree();
		//@}

		/**	@name	Predicates
		*/
		//@{
		
		/** Evaluate the (sub)expression.
		*/
		virtual bool operator () (const Atom& atom) const;
		//@}

		/**	@name	Accessors
		*/
		//@{

		/**	Set the expression node's type.
		*/
		void setType(Type type);
		
		/**	Set the expression node's negation mode.
		*/
		void setNegate(bool negate);
		
		/**	Set the predicate.
		*/
		void setPredicate(ExpressionPredicate* predicate);

		/**	Append a child to the tree.
		*/
		void appendChild(ExpressionTree* child);
		//@}

		
		protected:
		
		Type									type_;
		bool									negate_;
		ExpressionPredicate*	predicate_;
		list<ExpressionTree*>	children_;
	};

	/**
	*/
	class SyntaxTree
	{
		public:

		/**	@name	Type Definitions
		*/
		//@{

		///
		typedef	list<SyntaxTree*>::iterator				Iterator;

		///
		typedef	list<SyntaxTree*>::const_iterator	ConstIterator;
		//@}

		/**	@name	Constructors and Destructors	
		*/
		//@{

		/**
		*/
		SyntaxTree(const String& expression);

		/**
		*/
		~SyntaxTree();
		//@}

		/**	@name	Accessors
		*/
		//@{
		///
		Iterator begin();

		///
		Iterator end();

		///
		ConstIterator begin() const;

		///
		ConstIterator end() const;	

		///
		void mergeLeft(SyntaxTree* tree);

		///
		void mergeRight(SyntaxTree* tree);

		///
		void parse();
		//@}

		String								expression;
		String								argument;
		bool									evaluated;
		bool									negate;
		ExpressionTree::Type	type;
		list<SyntaxTree*>			children;
			
		protected:
		
		void expandBrackets_();
		void collapseANDs_();
		void collapseORs_();

	};
	/**
	*/

	class Expression
	{
		public:

		/**	@name	Type Definitions
		*/
		//@{

		/**	A creation method for predicates.
		*/
		typedef void * (*CreationMethod) ();
		//@}
 
		/**	@name	Constructors and Destructors
		*/
		//@{
		/** Default Constructor.
		*/
		Expression();

		/** Copy Constructor.
		*/
		Expression(const Expression& expression);

		/** Construct an Expression with a string
		*/
		Expression(const String& expression_string);

		/** Destructor.
		*/
		virtual ~Expression();
		//@}

		/**	@name	Predicates
		*/
		//@{

		/**
		*/
		bool hasPredicate(const String& name);
		//@}

		/**	@name	Accessors
		*/
		//@{

		/** Evaluate the expression of {\tt atom}
				@param atom
		*/
		virtual bool operator () (const Atom& atom);

		/**	Create a new predicate according to the name.
				If the predicate is not known, return 0.
		*/
		ExpressionPredicate* getPredicate(const String& name, const String& args = "") const;

		/**	Register a new predicate class.
		*/
		void registerPredicate(const String& name, CreationMethod creation_method);

		/**
		*/
		void setExpression(const String& expression);

		//@}

		protected:

		ExpressionTree*	constructExpressionTree_(const SyntaxTree& tree);
		void registerStandardPredicates_();

		StringHashMap<CreationMethod> create_methods_;
		ExpressionTree*								expression_tree_;

	};

	class Selector
		:	public UnaryProcessor<Composite>
	{
		
		public:
		
		/**	@name	Constructors and Destructors
		*/
		//@{
		BALL_CREATE_NODEEP(Selector)
		/**	Default constructor.
		*/
		Selector();

		/**	Copy constructor.
		*/
		Selector(const Selector& selector);

		/** Construct a Selector with a string.
		*/
		Selector(const String& expression_string);

		/** Destructor.
		*/
		virtual ~Selector();
		//@}

		/**	@name	Processor related methods
		*/
		//@{
		/**
		*/
		virtual Processor::Result operator () (Composite& composite);
		
		/**	Processor start method.
				This method is needed to reset the internal counter for the number of
				selected atoms.
		*/
		virtual bool start();
		//@}

		/**	@name	Accessors
		*/
		//@{

		/**	Return the number of atoms selected during the last application.
		*/
		Size getNumberOfSelectedAtoms() const;
				
		//@}
			
		protected:

		Size				number_of_selected_atoms_;
		Expression	expression_;
	};

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

#endif // BALL_KERNEL_SELECTOR_H
