// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/KERNEL/expression.h>
#include <BALL/KERNEL/standardPredicates.h>
#include <BALL/KERNEL/compiledExpression.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/moleculeStore.h>
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
		// K0.5.3: prefer the compiled fast path. CompiledExpression's
		// evaluate_one runs the variant-dispatched leaf evaluator with no
		// virtual calls; the cache makes the per-evaluation lookup O(1)
		// after the first compile.
		MoleculeStore* store = atom.getStore();
		if (store != nullptr && expression_string_.size() > 0
		    && expression_string_ != String("<not initialized>"))
		{
			try
			{
				// K0.5.4: use the Expression-aware overload so any
				// registered predicate (ring/SMARTS/user-registered/
				// ancestor-walk) lowers to an OwnedPred instead of
				// throwing.
				auto compiled = CompiledExpressionCache::instance()
					.get_or_compile(*store, *this);
				try
				{
					return compiled->evaluate_one(atom);
				}
				catch (Exception::InvalidArgument&)
				{
					// 2026-05-18 (R14.1 catch + recompile): generation
					// staleness check fired. Invalidate this store's
					// cached entries (typically because compact() bumped
					// generation between get_or_compile and evaluate_one
					// on a hot path) and recompile.
					CompiledExpressionCache::instance().invalidate_store(store);
					auto compiled2 = CompiledExpressionCache::instance()
						.get_or_compile(*store, *this);
					return compiled2->evaluate_one(atom);
				}
			}
			catch (Exception::ParseError&)
			{
				// Should only happen now on genuine syntax errors or a
				// predicate that isn't even registered. Fall through to
				// the v1.x ExpressionTree path which has the same try/throw.
			}
		}
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

	// K0.5.3: public accessor for the compiled fast-path AST. Returns
	// nullptr if the source can't be lowered (so callers can choose to
	// take the legacy ExpressionTree path explicitly).
	std::shared_ptr<const CompiledExpression>
	Expression::getCompiled(MoleculeStore& store) const
	{
		if (expression_string_.size() == 0
		    || expression_string_ == String("<not initialized>"))
		{
			return nullptr;
		}
		try
		{
			// K0.5.4: Expression-aware overload.
			return CompiledExpressionCache::instance()
				.get_or_compile(store, *this);
		}
		catch (Exception::ParseError&)
		{
			return nullptr;
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
		create_methods_.insert("true", (CreationMethod)Factory<TruePredicate>::createVoid);
		create_methods_.insert("false", (CreationMethod)Factory<FalsePredicate>::createVoid);
		create_methods_.insert("selected", (CreationMethod)Factory<SelectedPredicate>::createVoid);
		create_methods_.insert("name", (CreationMethod)Factory<AtomNamePredicate>::createVoid);
		create_methods_.insert("type", (CreationMethod)Factory<AtomTypePredicate>::createVoid);
		create_methods_.insert("element", (CreationMethod)Factory<ElementPredicate>::createVoid);
		create_methods_.insert("residue", (CreationMethod)Factory<ResiduePredicate>::createVoid);
		create_methods_.insert("residueID", (CreationMethod)Factory<ResidueIDPredicate>::createVoid);
		create_methods_.insert("protein", (CreationMethod)Factory<ProteinPredicate>::createVoid);
		create_methods_.insert("secondaryStruct", (CreationMethod)Factory<SecondaryStructurePredicate>::createVoid);
		create_methods_.insert("solvent", (CreationMethod)Factory<SolventPredicate>::createVoid);
		create_methods_.insert("backbone", (CreationMethod)Factory<BackBonePredicate>::createVoid);
		create_methods_.insert("chain", (CreationMethod)Factory<ChainPredicate>::createVoid);
		create_methods_.insert("nucleotide", (CreationMethod)Factory<NucleotidePredicate>::createVoid);
		create_methods_.insert("inRing", (CreationMethod)Factory<InRingPredicate>::createVoid);
		create_methods_.insert("doubleBonds", (CreationMethod)Factory<DoubleBondsPredicate>::createVoid);
		create_methods_.insert("tripleBonds", (CreationMethod)Factory<TripleBondsPredicate>::createVoid);
		create_methods_.insert("aromaticBonds", (CreationMethod)Factory<AromaticBondsPredicate>::createVoid);
		create_methods_.insert("numberOfBonds", (CreationMethod)Factory<NumberOfBondsPredicate>::createVoid);
		create_methods_.insert("connectedTo", (CreationMethod)Factory<ConnectedToPredicate>::createVoid);
		create_methods_.insert("sp3Hybridized", (CreationMethod)Factory<Sp3HybridizedPredicate>::createVoid);
		create_methods_.insert("sp2Hybridized", (CreationMethod)Factory<Sp2HybridizedPredicate>::createVoid);
		create_methods_.insert("spHybridized", (CreationMethod)Factory<SpHybridizedPredicate>::createVoid);
		create_methods_.insert("charge", (CreationMethod)Factory<ChargePredicate>::createVoid);
		create_methods_.insert("isAxial", (CreationMethod)Factory<AxialPredicate>::createVoid);
		create_methods_.insert("is4C1", (CreationMethod)Factory<Conformation4C1Predicate>::createVoid);
		// SMARTS predicate registration: gated under BALL_COLLAPSE_KERNEL_ONLY (its link
		// dependency STRUCTURE/SmartsMatcher → QSAR/RingPerception is excluded
		// in the KERNEL-only build). v2.2 HCP-1 task 0 -- restored from
		// Track-B's lifted gate; returns at HCP-3 module re-open.
#ifndef BALL_COLLAPSE_KERNEL_ONLY
		create_methods_.insert("SMARTS", (CreationMethod)Factory<SMARTSPredicate>::createVoid);
#endif
	}

}
