// $Id: expression.C,v 1.29 2001/07/17 00:52:48 oliver Exp $

#include <BALL/KERNEL/expression.h>
#include <BALL/KERNEL/standardPredicates.h>

#include <BALL/CONCEPT/selectable.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/residue.h>
#include <BALL/KERNEL/nucleotide.h>
#include <BALL/KERNEL/bond.h>

using namespace std;

namespace BALL 
{

	// Expression class, frontend to ExpressionTree

	Expression::Expression() 
		throw()
		: create_methods_(),
			expression_tree_(0),
			expression_string_("<not initialized>")
	{
		registerStandardPredicates_();
	}


	Expression::Expression(const Expression& expression) 
		throw()
		:	create_methods_(expression.create_methods_),
		  expression_tree_(new ExpressionTree(*expression.expression_tree_)),
			expression_string_(expression.expression_string_)
	{
	}


	Expression::Expression(const String& expression_string) 
		throw()
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
		throw()
	{
		clear();
	}


	void Expression::clear()
		throw()
	{
		create_methods_.clear();
		// clear() has to make a default constructed instance out of this, so
		// the standard predicates have to be rebuilt.
		registerStandardPredicates_();

		delete expression_tree_;
		expression_tree_ = 0;
		expression_string_ = "<not initialized>";
	}


	const Expression& Expression::operator = (const Expression& expression)
		throw()
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
		throw()
	{
		if ((expression_tree_ == 0) && (expression.expression_tree_ == 0))
		{
			// both pointers are null pointers. Expressions should have null
			// pointers only if they are default constructed, so a consistency
			// check might be useful (the string should be empty; maybe later)

			return ((create_methods_ == expression.create_methods_)
					&& (expression_string_ == expression.expression_string_));
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

			// compare everything.

			return ((create_methods_ == expression.create_methods_)
					&& (*expression_tree_ == *expression.expression_tree_)
					&& (expression_string_ == expression.expression_string_));
			}
		}
	}


	bool Expression::hasPredicate(const String& name) const 
		throw()
	{
		return create_methods_.has(name);
	}

	bool Expression::operator () (const Atom& atom) const 
		throw()
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


	ExpressionPredicate* Expression::getPredicate
		(const String& name, const String& args) const 
		throw()
  {
    CreationMethod create_method = create_methods_[name];
    ExpressionPredicate* predicate = (ExpressionPredicate*)(create_method)();

    predicate->setArgument(args);

    return predicate;
	}


	void Expression::registerPredicate
		(const String& name, CreationMethod creation_method)
		throw()
	{
		create_methods_.insert(name, creation_method);
	}


	void Expression::setExpression(const String& expression_string)
		throw(Exception::ParseError)
	{
		// don't use clear() here, because it also would delete create_methods_
		delete expression_tree_;
		expression_tree_ = 0;

		expression_string_ = expression_string;

		// create a temporary tree from which the expression_tree_ can be built
		SyntaxTree tree(expression_string);
		tree.parse();
		// DEBUG
		Log.info() << endl << endl;
		tree.dump();
		Log.info() << endl << endl;

		// construct the tree
		expression_tree_ = constructExpressionTree_(tree);
	}


	const String& Expression::getExpressionString() const 
		throw()
	{
		return expression_string_;
	}


	const ExpressionTree* Expression::getExpressionTree() const
		throw()
	{
		return expression_tree_;
	}


	const StringHashMap<Expression::CreationMethod>& Expression::getCreationMethods() const
		throw()
	{
		return create_methods_;
	}


  ExpressionTree* Expression::constructExpressionTree_(const SyntaxTree& t)
		throw()
  {
    ExpressionTree* root = new ExpressionTree();
    root->setType(t.type);
    root->setNegate(t.negate);

    if (t.type == ExpressionTree::LEAF)
    {
      if (hasPredicate(t.expression))
      {
        root->setPredicate(getPredicate(t.expression, t.argument));
			}
			else
			{
        Log.error() << "Expression::constructExpressionTree_: "
					<< "could not find predicate for expression " << t.expression 
					<< "(" << t.argument << ")" << endl;
        root->setType(ExpressionTree::INVALID);
			}

		}
		else
		{

      for (SyntaxTree::ConstIterator it = t.begin(); it != t.end(); ++it)
      {
        root->appendChild(constructExpressionTree_(**it));
			}
		}

    return root;
	}


	void Expression::registerStandardPredicates_()
		throw()
	{
		using namespace RTTI;
		create_methods_.insert("true", TruePredicate::createDefault);
		create_methods_.insert("false", FalsePredicate::createDefault);
		create_methods_.insert("selected", SelectedPredicate::createDefault);
		create_methods_.insert("name", AtomNamePredicate::createDefault);
		create_methods_.insert("type", AtomTypePredicate::createDefault);
		create_methods_.insert("element", ElementPredicate::createDefault);
		create_methods_.insert("residue", ResiduePredicate::createDefault);
		create_methods_.insert("residueID", ResidueIDPredicate::createDefault);
		create_methods_.insert("protein", ProteinPredicate::createDefault);
		create_methods_.insert("secondaryStruct", SecondaryStructurePredicate::createDefault);
		create_methods_.insert("solvent", SolventPredicate::createDefault);
		create_methods_.insert("backbone", BackBonePredicate::createDefault);
		create_methods_.insert("chain", ChainPredicate::createDefault);
		create_methods_.insert("nucleotide", NucleotidePredicate::createDefault);
		create_methods_.insert("inRing", InRingPredicate::createDefault);
		create_methods_.insert("doubleBonds", DoubleBondsPredicate::createDefault);
		create_methods_.insert("tripleBonds", TripleBondsPredicate::createDefault);
		create_methods_.insert("aromaticBonds", AromaticBondsPredicate::createDefault);
		create_methods_.insert("numberOfBonds", NumberOfBondsPredicate::createDefault);
		create_methods_.insert("connectedTo", ConnectedToPredicate::createDefault);
		create_methods_.insert("sp3Hybridized", Sp3HybridizedPredicate::createDefault);
		create_methods_.insert("sp2Hybridized", Sp2HybridizedPredicate::createDefault);
		create_methods_.insert("spHybridized", SpHybridizedPredicate::createDefault);
	}

}
