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
		: expression_tree_(0),
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
		:	expression_tree_(0)
	{
		registerStandardPredicates_();
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
		delete expression_tree_;
		expression_string_.clear();
	}


	const Expression& Expression::operator = (const Expression& expression)
		throw()
	{
		clear();

		create_methods_ = expression.create_methods_;
		expression_tree_ = new ExpressionTree(*expression.expression_tree_);
		expression_string_ = expression.expression_string_;

		return *this;
	}


	bool Expression::operator == (const Expression& expression) const 
		throw()
	{
		return ((create_methods_ == expression.create_methods_)
			&& (*expression_tree_ == *expression.expression_tree_)
			&& (expression_string_ == expression.expression_string_));
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


	void Expression::setExpression(const String& expression)
		throw()
	{
		delete expression_tree_;
		expression_tree_ = 0;
		expression_string_ = expression;

		SyntaxTree tree(expression);
		tree.parse();

		expression_tree_ = constructExpressionTree_(tree);
	}


	const String& Expression::getExpression() const 
		throw()
	{
		return expression_string_;
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
        Log.error() << "Expression::setExpression: could not find predicate for expression " << t.expression << "(" << t.argument << ")" << endl;
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


	// no more nested classes of Selector: ExpressionTree

	ExpressionTree::ExpressionTree()
		throw()
		:	type_(INVALID),
			negate_(false),
			predicate_(0),
			children_()
	{
	}


	ExpressionTree::ExpressionTree
		(Type type, list<ExpressionTree*>	children, bool negate)
		throw()
		:	type_(type),
			negate_(negate),
			predicate_(0),
			children_(children)
	{
	}
	

	// BAUSTELLE: 
	// should we clone predicate_ or should we only set the pointer?
	ExpressionTree::ExpressionTree(const ExpressionTree& tree)
		throw()
		:	type_(tree.type_),
			negate_(tree.negate_),
			predicate_(tree.predicate_),
			children_(tree.children_)
	{
	}
			

	ExpressionTree::~ExpressionTree()
		throw()
	{
		clear();
	}


	void ExpressionTree::clear()
		throw()
	{
		type_ = INVALID;
		negate_ = false;
		// BAUSTELLE:
		// memory leak if nobody cares for the predicate.
		predicate_ = 0;
		children_.clear();
	}


	const ExpressionTree& ExpressionTree::operator = 
		(const ExpressionTree& tree)
		throw()
	{
		clear();

		type_ = tree.type_;
		negate_ = tree.negate_;
		predicate_ = tree.predicate_;
		children_ = tree.children_;

		return *this;
	}


	bool ExpressionTree::operator () (const Atom& atom) const 
		throw()
	{
    bool result;
    if (type_ == LEAF)
    {

      if (predicate_ != 0)
      {
        result = (negate_ ^ (predicate_->operator () (atom)));
			} 
			else 
			{
        result = false;
			}

		} 
		else 
		{

      // the empty clause is always true
      if (children_.size() == 0)
      {
        return (!negate_);
			}

			// evaluated all children
      list<ExpressionTree*>::const_iterator list_it = children_.begin();
      bool abort = false;
      for (; !abort && list_it != children_.end(); ++list_it)
      {
        result = (*list_it)->operator () (atom);

				// OR expressions may be aborted, if the first subexpression yields true
        if (type_ == OR)
        {
          if (result == true)
          {
            abort = true;
					}
				// AND expressions may be aborted, if the first subexpression yields false
				}
				else
				{
          if (result == false)
          {
            abort = true;
					}
				}
			}
		}

    return result; 
	}


	bool ExpressionTree::operator == (const ExpressionTree& tree) const
		throw()
	{
		return ((type_ == tree.type_)
			&& (negate_ == tree.negate_)
			&& (*predicate_ == *tree.predicate_)
			&& (children_ == tree.children_));
	}


	void ExpressionTree::setType(Type type)
		throw()
	{
		type_ = type;
	}


	ExpressionTree::Type ExpressionTree::getType() const 
		throw()
	{
		return type_;
	}


	void ExpressionTree::setNegate(bool negate)
		throw()
	{
		negate_= negate;
	}


	bool ExpressionTree::getNegate() const 
		throw()
	{
		return negate_;
	}


	void ExpressionTree::setPredicate(ExpressionPredicate* predicate)
		throw()
	{
		predicate_= predicate;
	}


	ExpressionPredicate*  ExpressionTree::getPredicate() const 
		throw()
	{
		return predicate_;
	}


	void ExpressionTree::appendChild(ExpressionTree* child)
		throw()
	{
		children_.push_back(child);
	}


	SyntaxTree::SyntaxTree()
		throw()
	{
	}

	SyntaxTree::SyntaxTree(const String& s)
		throw()
		:	expression(s),
			argument(""),
			evaluated(false),
			negate(false),
			type(ExpressionTree::INVALID)			
	{
	}

	SyntaxTree::~SyntaxTree()
		throw()
	{
		for (Iterator it = begin(); it != end(); ++it)
		{
			delete *it;
		}
	}

	SyntaxTree::Iterator SyntaxTree::begin()
		throw()
	{
		return children.begin();
	}
	
	SyntaxTree::ConstIterator SyntaxTree::begin() const
		throw()
	{
		return children.begin();
	}
	
	SyntaxTree::Iterator SyntaxTree::end()
		throw()
	{
		return children.end();
	}
	
	SyntaxTree::ConstIterator SyntaxTree::end() const
		throw()
	{
		return children.end();
	}

	void SyntaxTree::mergeLeft(SyntaxTree* tree)
		throw()
	{
    if (tree->children.empty())
    {
      children.push_front(tree);
      return;
		}

    for (list<SyntaxTree*>::reverse_iterator it = tree->children.rbegin(); 
				 it != tree->children.rend(); ++it)
    {
      children.push_front(*it);
		}

    tree->children.erase(tree->begin(), tree->end());

    delete tree;
	}

	void SyntaxTree::mergeRight(SyntaxTree* tree)
		throw()
	{
    if (tree->children.empty())
    {
      children.push_back(tree);
      return;
		}

    for (Iterator it = tree->begin(); it != tree->end(); ++it)
    {
      children.push_back(*it);
		}

    tree->children.erase(tree->begin(), tree->end());

    delete tree;
	}

	void SyntaxTree::parse()
		throw()
	{
    if (!evaluated)
    {
      expandBrackets_();
      collapseANDs_();
      collapseORs_();
		}
	}
 
	void SyntaxTree::expandBrackets_()
		throw()
	{
    // we do not try to expand already processed nodes
    if (evaluated == true)
    {
      return;
		}

    if ((expression == "AND" || expression == "OR") && children.size() == 0)
    {
      return;
		}

    String s = expression;
    // brackets separate a string into three sections (at most)
    // sections are called x, y, and z
    // and sx, sy, sz are iterators to the start, ex, ey, ez to the end
    // of the substring
    String::size_type sx, ex, sy, ey, sz, ez;
    sx = 0;
    ez = s.size();
    ex = s.find_first_of('(');
    if (ex == string::npos)
    {
			Log.error() << "Expression::setExpression: need at least on opening bracket in expression {" << s << "}" << endl;
      type = ExpressionTree::INVALID;
      return;
		}
    sy = ex;
    sy++;
    Index bracket_count = 1;
    Size i;
    for (i = (Size)sy; (i < (Size)s.size()) && (bracket_count > 0); ++i)
    {
      switch (s[i])
      {
        case '(': bracket_count++; break;
        case ')': bracket_count--; break;
			}
		}

		if (bracket_count > 0)
		{
			type = ExpressionTree::INVALID;
			Log.error() << "Expression::setExpression: didn't find closing ')' in expression: {" << s << "}" << endl;
			return;
		}

		if (bracket_count < 0)
		{
			type = ExpressionTree::INVALID;
			Log.error() << "Expression::setExpression: found too many closing ')' in expression: {" << s << "}" << endl;
			return;
		}

    ey = i;
    if (ey >= s.size())
    {
      sz = s.size();
		} 
		else 
		{
      sz = ++i;
		}
    // we identified the first expresion in brackets.
    // now decide, whether it is a predicate or a bracket expresion
    // or something strange
    String left(s, (Index)sx, (Index)ex);
    Size number_of_fields = left.countFields();
    if (number_of_fields == 0)
    {
      // there is nothing left of the bracket: it is a bracketed expression

      // we don't have to consider an expression left of the bracket
      ex = sx;

      // expand the expression inside the brackets
      SyntaxTree* new_t = new SyntaxTree(String(s, (Index)sy, (Index)(ey - sy - 1)));
      children.push_front(new_t);
      new_t->expandBrackets_();

		} 
		else 
		{

      // get word directly to the left of the opening bracket
      String left_word = left.getField((Index)number_of_fields - 1);
      if (left_word == "AND" || left_word == "OR" || left_word == "!")
      {
        SyntaxTree* new_t = new SyntaxTree(String(s, (Index)sy, (Index)(ey - sy - 1)));
        new_t->expandBrackets_();
        children.push_front(new_t);

        if (left_word == "!")
        {
          new_t->negate = true;
				}
				else
				{
          new_t = new SyntaxTree(left_word);
          new_t->evaluated = false;
          children.push_front(new_t);
				}

        String rest_expression = "";
        for (i = 0; i < number_of_fields - 1; i++)
        {
          rest_expression += left.getField((Index)i) + " ";
				}
        rest_expression.trim();
        if (rest_expression != "")
        {
          new_t = new SyntaxTree(rest_expression);
          new_t->expandBrackets_();
          mergeLeft(new_t);
				}
        ex = sx;

			}
			else
			{

        SyntaxTree* new_t = new SyntaxTree(String(s, (Index)sy, (Index)(ey - sy - 1)));
        new_t->type = ExpressionTree::LEAF;
        new_t->argument = new_t->expression;
        new_t->expression = left_word;
        new_t->evaluated = true;
        if (left_word[0] == '!')
        {
          new_t->expression = String(left_word, 1);
          new_t->negate = true;
				}
        children.push_front(new_t);

        if (number_of_fields > 1)
        {
          left_word = left.getField((Index)number_of_fields - 2);

          if (left_word == "!")
          {
            new_t->negate = true;
            number_of_fields--;
					}
				}

        String rest_expression = "";
        for (Size i = 0; i < number_of_fields - 1; ++i)
        {
          rest_expression += left.getField((Index)i) + " ";
				}
        rest_expression.trim();

        if (rest_expression != "")
        {
          new_t = new SyntaxTree(rest_expression);
          new_t->expandBrackets_();
          mergeLeft(new_t);
				}
        ex = sx;
			}
		}

    if (ex != sx)
    {
      SyntaxTree* new_t = new SyntaxTree(String(s, (Index)sx, (Index)(ex - sx)));
      new_t->expandBrackets_();
      mergeLeft(new_t);
		}
    if (ez != sz)
    {
      SyntaxTree* new_t = new SyntaxTree(String(s, (Index)sz, (Index)(ez - sz)));
      new_t->expandBrackets_();
      mergeRight(new_t);
		}
	}
 
	void SyntaxTree::collapseANDs_()
		throw()
	{
    if (children.size() < 3)
    {
      return;
		}

    Iterator  it = begin();

    while (it != end())
    {
      for (; it != end() && (*it)->expression != "AND"; ++it);

      if (it == end())
      {
        break;
			}

      // remember the first node in the AND expression
      Iterator  start = it;
      start--;

      for (; it != end() && ((*it)->expression == "AND" || (*it)->evaluated == true); ++it);

      // remember the last node for the AND expression
      Iterator  end = it;

      SyntaxTree* t = new SyntaxTree("AND");
      it = start;
      children.insert(start, t);
      t->type = ExpressionTree::AND;
      t->evaluated = true;
      t->collapseANDs_();

      for (it = start; it != end; ++it)
      {
        if ((*it)->expression != "AND")
        {
          t->children.push_back(*it);
				}
			}
      children.erase(start, end);

      it = end;
		}

    for (it = begin(); it != end(); ++it)
    {
      (*it)->collapseANDs_();
		}
	}
 

  void SyntaxTree::collapseORs_()
		throw()
  {
    Iterator  it = begin();

    while (it != end())
    {
      for (; it != end() && (*it)->expression != "OR"; ++it);

      if (it == end())
      {
        break;
			}

      // remember the first node in the OR expression
      Iterator  start = it;
      start--;

      for (; it != end() && ((*it)->expression == "OR" || ((*it)->evaluated == true)); ++it);

      // remember the last node for the OR expression
      Iterator  end = it;

      SyntaxTree* t = new SyntaxTree("OR");
      it = start;
      children.insert(start, t);
      t->type = ExpressionTree::OR;
      t->evaluated = true;
      t->collapseORs_();

      for (it = start; it != end; ++it)
      {
        if ((*it)->expression != "OR")
        {
          t->children.push_back(*it);
				}
			}
      children.erase(start, end);

      it = end;
		}

    for (it = begin(); it != end(); ++it)
    {
      (*it)->collapseORs_();
		}
	}


	ExpressionPredicate::ExpressionPredicate() 
		throw()
		: UnaryPredicate<Atom>(),
			argument_("")
	{
	}


	ExpressionPredicate::ExpressionPredicate
		(const ExpressionPredicate& predicate) 
		throw()
		:	UnaryPredicate<Atom>(predicate),
			argument_(predicate.argument_)
	{
	}


	ExpressionPredicate::ExpressionPredicate(const String& argument) 
		throw()
		:	UnaryPredicate<Atom>(),
			argument_(argument)
	{
	}


	ExpressionPredicate::~ExpressionPredicate() 
		throw()
	{
		clear();
	}


	const ExpressionPredicate& ExpressionPredicate::operator =
		(const ExpressionPredicate& predicate) 
		throw()
	{
		UnaryPredicate<Atom>::operator = (predicate);
		argument_ = predicate.argument_;

		return *this;
	}


	void ExpressionPredicate::clear() 
		throw()
	{
		argument_ = "";
		// BAUSTELLE
		// UnaryPredicate does not implement the OCI
		// UnaryPredicate<Atom>::clear();
	}


	bool ExpressionPredicate::operator == (const ExpressionPredicate& predicate) const 
		throw()
	{
		// BAUSTELLE
		// UnaryPredicate does not implement the OCI
		// return (UnaryPredicate<Atom>::operator == (predicate)
		//	&& (argument_ == predicate.argument_));
		return (argument_ == predicate.argument_);
	}


	bool ExpressionPredicate::operator () (BALL::Atom const &) const 
		throw()
	{
		return true;
	}


	void ExpressionPredicate::setArgument(const String& args) 
		throw()
	{
		argument_ = args;
	}


	const String& ExpressionPredicate::getArgument() const 
		throw()
	{
		return argument_;
	}

}
