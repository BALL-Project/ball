// $Id: selector.h,v 1.8 2000/03/14 19:38:06 oliver Exp $

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

	/**@name	Kernel Object Selection
	*/
	//@{

	/**
	*/
	class ExpressionPredicate
		:	public UnaryPredicate<Atom>
	{
		public:

		/**	@name	Constructors and Destructors	
		*/
		//@{

		/**
		*/
		ExpressionPredicate();
		
		/**
		*/
		ExpressionPredicate(const String& argument);

		/**
		*/
		virtual ~ExpressionPredicate();
		//@}

		/**
		*/
		virtual bool operator () (const Atom& atom) const = 0;

		/**
		*/
		virtual void setArgument(const String& argument);

		
		protected:
		
		String argument_;
	};


	/**
	*/
	class Selector
		:	public UnaryProcessor<Composite>
	{
		
		public:
		/**	@name	Nested Classes
		*/
		//@{
		
		/**
		*/
		class ExpressionNode
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
					{\tt negate_} is set to {\bf false}, the internal predicate is set to 0, and 
					the list of children is empty.
			*/
			ExpressionNode();
				
			/**	Detailled construtor.
					Create an expression node representing a leaf, i.e., a 
					predicate.
					@param	predicate the node's predicate
					@param	negate set to {\bf true} if the node's predicate should be negated
			*/
			ExpressionNode(ExpressionPredicate* predicate, bool negate = false);

			/**
			*/
			ExpressionNode(Type type, list<ExpressionNode*> children, bool negate = false);

			/**	Destructor
			*/
			virtual ~ExpressionNode();
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
			void appendChild(ExpressionNode* child);
			//@}

			
			protected:
			
			Type									type_;
			bool									negate_;
			ExpressionPredicate*	predicate_;
			list<ExpressionNode*>	children_;
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
			ExpressionNode::Type	type;
			list<SyntaxTree*>			children;
				
			protected:
			
			void expandBrackets_();
			void collapseANDs_();
			void collapseORs_();

		};

		//@}

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
		BALL_CREATE_NODEEP(Selector)
		/**	Default constructor.
		*/
		Selector();

		/**	Copy constructor.
		*/
		Selector(const Selector& selector);

		/**
		*/
		Selector(const String& expression);

		/**
		*/
		virtual ~Selector();
		//@}

		/**	@name	Predicates
		*/
		//@{

		/**
		*/
		bool hasPredicate(const String& name);
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

		/**	Return the number of atoms selected during the last application.
		*/
		Size getNumberOfSelectedAtoms() const;
				
		//@}
			
		protected:

		ExpressionNode*	constructExpressionTree_(const SyntaxTree& tree);
		void registerStandardPredicates_();

		
		ExpressionNode*								expression_tree_;
		StringHashMap<CreationMethod> create_methods_;
		Size													number_of_selected_atoms_;
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

	
	//@}	
		
	//@}

} // namespace BALL

#endif // BALL_KERNEL_SELECTOR_H
