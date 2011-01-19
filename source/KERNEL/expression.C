// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/KERNEL/expression.h>
#include <BALL/KERNEL/standardPredicates.h>
#include <BALL/CONCEPT/factory.h>

using namespace::std;
namespace BALL
{
	// Expression class, frontend to ExpressionTree

	Expression::Expression()
		: create_methods_(),
			expression_tree_(0),
			expression_string_("<not initialized>")
	{
		registerStandardPredicates_();
	}


	Expression::Expression(const Expression& expression)
		:	create_methods_(expression.create_methods_),
		  expression_tree_(new ExpressionTree(*expression.expression_tree_)),
			expression_string_(expression.expression_string_)
	{
	}


	Expression::Expression(const String& expression_string)
		:	create_methods_(),
			expression_tree_(0),
			expression_string_("")
	{
		registerStandardPredicates_();
		// Use this method instead of ctor initialization because it builds a
		// whole expression tree.
		setExpression(expression_string);
	}


	Expression::~Expression()
	{
		clear();
	}


	void Expression::clear()
	{
		delete expression_tree_;
		expression_tree_ = 0;
		expression_string_ = "<not initialized>";
	}


	Expression& Expression::operator = (const Expression& expression)
	{

		// don't use clear() here (for performance reasons, create_methods_ is
		// rebuilt by clear() and we had to clear() it again.
		create_methods_.clear();
		delete expression_tree_;
		expression_string_.clear();

		create_methods_ = expression.create_methods_;
		expression_tree_ = new ExpressionTree(*expression.expression_tree_);
		expression_string_ = expression.expression_string_;

		return *this;
	}


	bool Expression::operator == (const Expression& expression) const
	{
		if ((expression_tree_ == 0) && (expression.expression_tree_ == 0))
		{
			// both pointers are null pointers. Expressions should have null
			// pointers only if they are default constructed, so a consistency
			// check might be useful (the string should be empty; maybe later)

			return (expression_string_ == expression.expression_string_);
		}
		else
		{
			if ((expression_tree_ == 0) || (expression.expression_tree_ == 0))
			{
				// one of the pointers is NULL. The instances cannot be equal.
				return false;
			}
			else
			{
				return ((*expression_tree_ == *expression.expression_tree_)
								&& (expression_string_ == expression.expression_string_));
			}
		}
	}


	bool Expression::hasPredicate(const String& name) const
	{
		return create_methods_.has(name);
	}

	bool Expression::operator () (const Atom& atom) const
	{
		if (expression_tree_ != 0)
		{
			return expression_tree_->operator () (atom);
		}
		else
		{
			Log.error() << "Expression::operator (): no expression set" << endl;
			return false;
		}
	}


	ExpressionPredicate* Expression::getPredicate (const String& name, const String& args) const
  {
    CreationMethod create_method = create_methods_[name];
    ExpressionPredicate* predicate = (ExpressionPredicate*)(create_method)();

    predicate->setArgument(args);

    return predicate;
	}


	void Expression::registerPredicate(const String& name, CreationMethod creation_method)
	{
		create_methods_.insert(name, creation_method);
	}


	void Expression::setExpression(const String& expression_string)
	{
		// don't use clear() here, because it also would delete create_methods_
		if (expression_tree_ != 0)
		{
			delete expression_tree_;
			expression_tree_ = 0;
		}

		// remember the expression
		expression_string_ = expression_string;

		// create a temporary tree from which the expression_tree_ can be built
		ExpressionParser parser;
		parser.parse(expression_string);

		// construct the tree
		expression_tree_ = constructExpressionTree_(parser.getSyntaxTree());
	}


	const String& Expression::getExpressionString() const
	{
		return expression_string_;
	}


	const ExpressionTree* Expression::getExpressionTree() const
	{
		return expression_tree_;
	}


	const StringHashMap<Expression::CreationMethod>& Expression::getCreationMethods() const
	{
		return create_methods_;
	}


  ExpressionTree* Expression::constructExpressionTree_(const ExpressionParser::SyntaxTree& t)
  {
    ExpressionTree* root = new ExpressionTree;
    root->setType(t.type);
    root->setNegate(t.negate);

    if (t.type == ExpressionTree::LEAF)
    {
      if (hasPredicate(t.predicate))
      {
        root->setPredicate(getPredicate(t.predicate, t.argument));
			}
			else
			{
				delete root;

				throw Exception::ParseError(__FILE__, 0,
																		String(t.predicate  + "(" + t.argument + ")"),
																		"Predicate could not be found");
			}

		}
		else
		{
      for (ExpressionParser::SyntaxTree::ConstIterator it = t.begin(); it != t.end(); ++it)
      {
        root->appendChild(constructExpressionTree_(**it));
			}
		}

    return root;
	}


	void Expression::registerStandardPredicates_()
	{
		create_methods_.insert("true", (PersistenceManager::CreateMethod)Factory<TruePredicate>::createVoid);
		create_methods_.insert("false", (PersistenceManager::CreateMethod)Factory<FalsePredicate>::createVoid);
		create_methods_.insert("selected", (PersistenceManager::CreateMethod)Factory<SelectedPredicate>::createVoid);
		create_methods_.insert("name", (PersistenceManager::CreateMethod)Factory<AtomNamePredicate>::createVoid);
		create_methods_.insert("type", (PersistenceManager::CreateMethod)Factory<AtomTypePredicate>::createVoid);
		create_methods_.insert("element", (PersistenceManager::CreateMethod)Factory<ElementPredicate>::createVoid);
		create_methods_.insert("residue", (PersistenceManager::CreateMethod)Factory<ResiduePredicate>::createVoid);
		create_methods_.insert("residueID", (PersistenceManager::CreateMethod)Factory<ResidueIDPredicate>::createVoid);
		create_methods_.insert("protein", (PersistenceManager::CreateMethod)Factory<ProteinPredicate>::createVoid);
		create_methods_.insert("secondaryStruct", (PersistenceManager::CreateMethod)Factory<SecondaryStructurePredicate>::createVoid);
		create_methods_.insert("solvent", (PersistenceManager::CreateMethod)Factory<SolventPredicate>::createVoid);
		create_methods_.insert("backbone", (PersistenceManager::CreateMethod)Factory<BackBonePredicate>::createVoid);
		create_methods_.insert("chain", (PersistenceManager::CreateMethod)Factory<ChainPredicate>::createVoid);
		create_methods_.insert("nucleotide", (PersistenceManager::CreateMethod)Factory<NucleotidePredicate>::createVoid);
		create_methods_.insert("inRing", (PersistenceManager::CreateMethod)Factory<InRingPredicate>::createVoid);
		create_methods_.insert("doubleBonds", (PersistenceManager::CreateMethod)Factory<DoubleBondsPredicate>::createVoid);
		create_methods_.insert("tripleBonds", (PersistenceManager::CreateMethod)Factory<TripleBondsPredicate>::createVoid);
		create_methods_.insert("aromaticBonds", (PersistenceManager::CreateMethod)Factory<AromaticBondsPredicate>::createVoid);
		create_methods_.insert("numberOfBonds", (PersistenceManager::CreateMethod)Factory<NumberOfBondsPredicate>::createVoid);
		create_methods_.insert("connectedTo", (PersistenceManager::CreateMethod)Factory<ConnectedToPredicate>::createVoid);
		create_methods_.insert("sp3Hybridized", (PersistenceManager::CreateMethod)Factory<Sp3HybridizedPredicate>::createVoid);
		create_methods_.insert("sp2Hybridized", (PersistenceManager::CreateMethod)Factory<Sp2HybridizedPredicate>::createVoid);
		create_methods_.insert("spHybridized", (PersistenceManager::CreateMethod)Factory<SpHybridizedPredicate>::createVoid);
		create_methods_.insert("charge", (PersistenceManager::CreateMethod)Factory<ChargePredicate>::createVoid);
		create_methods_.insert("isAxial", (PersistenceManager::CreateMethod)Factory<AxialPredicate>::createVoid);
		create_methods_.insert("is4C1", (PersistenceManager::CreateMethod)Factory<Conformation4C1Predicate>::createVoid);
		create_methods_.insert("SMARTS", (PersistenceManager::CreateMethod)Factory<SMARTSPredicate>::createVoid);
	}

}
