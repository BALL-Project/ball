// $Id: selector.C,v 1.11 2000/02/12 19:28:49 oliver Exp $

#include <BALL/KERNEL/selector.h>

#include <BALL/CONCEPT/selectable.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/PSE.h>
#include <BALL/KERNEL/residue.h>
#include <BALL/KERNEL/nucleotide.h>

using namespace std;

namespace BALL 
{
	
	// nested classes of Selector: ExpressionNode

	Selector::ExpressionNode::ExpressionNode()
		:	type_(INVALID),
			negate_(false),
			predicate_(0)
	{
	}

	Selector::ExpressionNode::~ExpressionNode()
	{
	}

	Selector::ExpressionNode::ExpressionNode
		(Type type, list<ExpressionNode*>	children, bool negate)
		:	type_(type),
			negate_(negate),
			predicate_(0),
			children_(children)
	{
	}
	
	bool Selector::ExpressionNode::operator () (const Atom& atom) const
	{
    bool result;
    if (type_ == LEAF)
    {

      if (predicate_ != 0)
      {
        result = (negate_ ^ (predicate_->operator () (atom)));
			} else {
        result = false;
			}

		} else {

      // the empty clause is always true
      if (children_.size() == 0)
      {
        return (!negate_);
			}

			// evaluated all children
      list<ExpressionNode*>::const_iterator list_it = children_.begin();
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
				} else {
          if (result == false)
          {
            abort = true;
					}
				}
			}
		}

    return result; 
	}

	void Selector::ExpressionNode::setType(Type type)
	{
		type_= type;
	}

	void Selector::ExpressionNode::setNegate(bool negate)
	{
		negate_= negate;
	}

	void Selector::ExpressionNode::setPredicate(ExpressionPredicate* predicate)
	{
		predicate_= predicate;
	}

	void Selector::ExpressionNode::appendChild(ExpressionNode* child)
	{
		children_.push_back(child);
	}

	Selector::SyntaxTree::SyntaxTree(const String& s)
		:	expression(s),
			argument(""),
			evaluated(false),
			negate(false),
			type(ExpressionNode::INVALID)			
	{
	}

	Selector::SyntaxTree::~SyntaxTree()
	{
		for (Iterator it = begin(); it != end(); ++it)
		{
			delete *it;
		}
	}

	Selector::SyntaxTree::Iterator Selector::SyntaxTree::begin()
	{
		return children.begin();
	}
	
	Selector::SyntaxTree::ConstIterator Selector::SyntaxTree::begin() const
	{
		return children.begin();
	}
	
	Selector::SyntaxTree::Iterator Selector::SyntaxTree::end()
	{
		return children.end();
	}
	
	Selector::SyntaxTree::ConstIterator Selector::SyntaxTree::end() const
	{
		return children.end();
	}

	void Selector::SyntaxTree::mergeLeft(SyntaxTree* tree)
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

	void Selector::SyntaxTree::mergeRight(SyntaxTree* tree)
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

	void Selector::SyntaxTree::parse()
	{
    if (!evaluated)
    {
      expandBrackets_();
      collapseANDs_();
      collapseORs_();
		}
	}
 
	void Selector::SyntaxTree::expandBrackets_()
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
      type = ExpressionNode::INVALID;
      return;
		}
    sy = ex;
    sy++;
    Size bracket_count = 1;
    Size i;
    for (i = sy; i < s.size() && bracket_count > 0; ++i)
    {
      switch (s[i])
      {
        case '(': bracket_count++; break;
        case ')': bracket_count--; break;
			}
		}

    if (bracket_count != 0)
    {
      type = ExpressionNode::INVALID;
      Log.error() << "Didn't find closing ')' in expression: {" << s << "}" << endl;
      return;
		}

    ey = i;
    if (ey >= s.size())
    {
      sz = s.size();
		} else {
      sz = ++i;
		}
     // we identified the first expresion in brackets.
    // now decide, whether it is a predicate or a bracket expresion
    // or something strange
    String left(s, (Index)sx, ex);
    Size number_of_fields = left.countFields();
    if (number_of_fields == 0)
    {
      // there is nothing left of the bracket: it is a bracketed expression

      // we don't have to consider an expression left of the bracket
      ex = sx;

      // expand the expression inside the brackets
      SyntaxTree* new_t = new SyntaxTree(String(s, (Index)sy, ey - sy - 1));
      children.push_front(new_t);
      new_t->expandBrackets_();

		} else {

      // get word directly to the left of the opening bracket
      String left_word = left.getField((Index)number_of_fields - 1);
      if (left_word == "AND" || left_word == "OR" || left_word == "!")
      {
        SyntaxTree* new_t = new SyntaxTree(String(s, (Index)sy, ey - sy - 1));
        new_t->expandBrackets_();
        children.push_front(new_t);

        if (left_word == "!")
        {
          new_t->negate = true;
				} else {
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

			} else {

        SyntaxTree* new_t = new SyntaxTree(String(s, (Index)sy, ey - sy - 1));
        new_t->type = ExpressionNode::LEAF;
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
      SyntaxTree* new_t = new SyntaxTree(String(s, (Index)sx, ex - sx));
      new_t->expandBrackets_();
      mergeLeft(new_t);
		}
    if (ez != sz)
    {
      SyntaxTree* new_t = new SyntaxTree(String(s, (Index)sz, ez - sz));
      new_t->expandBrackets_();
      mergeRight(new_t);
		}
	}
 
	void Selector::SyntaxTree::collapseANDs_()
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
      t->type = ExpressionNode::AND;
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
 

  void Selector::SyntaxTree::collapseORs_()
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
      t->type = ExpressionNode::OR;
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
 

	Selector::Selector()
		:	expression_tree_(0),
			number_of_selected_atoms_(0)
	{
		registerStandardPredicates_();
	}
	
	Selector::Selector(const String& expression)
		:	expression_tree_(0),
			number_of_selected_atoms_(0)
	{
		registerStandardPredicates_();
		setExpression(expression);
	}

	Selector::~Selector()
	{
		delete expression_tree_;
	}

	void Selector::registerStandardPredicates_()
	{
		using namespace RTTI;
		create_methods_.insert("name", (CreationMethod)getNew<AtomNamePredicate>);
		create_methods_.insert("type", (CreationMethod)getNew<AtomTypePredicate>);
		create_methods_.insert("element", (CreationMethod)getNew<ElementPredicate>);
		create_methods_.insert("residue", (CreationMethod)getNew<ResiduePredicate>);
		create_methods_.insert("residueID", (CreationMethod)getNew<ResidueIDPredicate>);
		create_methods_.insert("protein", (CreationMethod)getNew<ProteinPredicate>);
		create_methods_.insert("secondarystruct", (CreationMethod)getNew<SecondaryStructurePredicate>);
		create_methods_.insert("solvent", (CreationMethod)getNew<SolventPredicate>);
		create_methods_.insert("backbone", (CreationMethod)getNew<BackBonePredicate>);
		create_methods_.insert("chain", (CreationMethod)getNew<ChainPredicate>);
		create_methods_.insert("nucleotide", (CreationMethod)getNew<NucleotidePredicate>);
	}

	void Selector::setExpression(const String& expression)
	{
		delete expression_tree_;

		SyntaxTree	tree(expression);
		tree.parse();

		expression_tree_ = constructExpressionTree_(tree);
	}

	bool Selector::hasPredicate(const String& name)
	{
		return create_methods_.has(name);
	}

	Size Selector::getNumberOfSelectedAtoms() const
	{
		return number_of_selected_atoms_;
	}

	ExpressionPredicate* Selector::getPredicate
		(const String& name, const String& args) const
  {
    CreationMethod create_method = create_methods_[name];
    ExpressionPredicate* predicate = (ExpressionPredicate*)(create_method)();

    predicate->setArgument(args);

    return predicate;
	}

  Selector::ExpressionNode* Selector::constructExpressionTree_(const SyntaxTree& t)
  {
    ExpressionNode* root = new ExpressionNode();
    root->setType(t.type);
    root->setNegate(t.negate);

    if (t.type == ExpressionNode::LEAF)
    {
      if (hasPredicate(t.expression))
      {
        root->setPredicate(getPredicate(t.expression, t.argument));
			} else {
        Log.error() << "could not find predicate for expression " << t.expression << "(" << t.argument << ")" << endl;
        root->setType(ExpressionNode::INVALID);
			}

		} else {

      for (SyntaxTree::ConstIterator it = t.begin(); it != t.end(); ++it)
      {
        root->appendChild(constructExpressionTree_(**it));
			}
		}

    return root;
	}

	bool Selector::start() 
	{
		// reset the number of selected atoms
		number_of_selected_atoms_ = 0;

		// and continue
		return true;
	}

  Processor::Result Selector::operator () (Composite& composite)
  {
		// if the composite is an atom, we apply the expression tree...
		if (RTTI::isKindOf<Atom>(composite))
		{
			Atom& atom = dynamic_cast<Atom&>(composite);
			if (expression_tree_->operator () (atom))
			{
				// select the atoms and increase the atom counter
				atom.select();
				number_of_selected_atoms_++;
			}
		}

    return Processor::CONTINUE;
	}

	void Selector::registerPredicate
		(const String& name, CreationMethod creation_method)
	{
		create_methods_.insert(name, creation_method);
	}

	////////////////////////////////////////////////////
	// expression predicate: the predicate base class //
	////////////////////////////////////////////////////

	ExpressionPredicate::ExpressionPredicate()
	{
	}

	ExpressionPredicate::~ExpressionPredicate()
	{
	}

	void ExpressionPredicate::setArgument(const String& args)
	{
		argument_ = args;
	}

	// Atom name predicate

	bool AtomNamePredicate::operator () (const Atom& atom) const
	{
    return (atom.getName() == argument_);
 	}

	// Atom type predicate

	bool AtomTypePredicate::operator () (const Atom& atom) const
	{
		return (atom.getTypeName() == argument_);
 	}


	// element predicate

	bool ElementPredicate::operator () (const Atom& atom) const
	{
		return (atom.getElement().getSymbol() == argument_);
	}

	// residue predicate

	bool ResiduePredicate::operator () (const Atom& atom) const
	{
		const Residue*	res = atom.getAncestor(RTTI::getDefault<Residue>());
		if (res != 0)
		{
			return (res->getName() == argument_);
		}

		return false;
	}
	
	// residue ID predicate

	bool ResidueIDPredicate::operator () (const Atom& atom) const
	{
    const Residue*	res = atom.getAncestor(RTTI::getDefault<Residue>());
		if (res != 0)
		{
			return (res->getID() == argument_);
		}
		
		return false;
	}
	
	// protein predicate

	bool ProteinPredicate::operator () (const Atom& /* atom */) const
	{
		//BAUSTELLE
		return false;
	}
	
	// chain predicate

	bool ChainPredicate::operator () (const Atom& /* atom */) const
	{
		//BAUSTELLE
		return false;
	}
	
	// secondary structure predicate

	bool SecondaryStructurePredicate::operator () (const Atom& /* atom */) const
	{
		//BAUSTELLE
		return false;
	}
	
	// solvent predicate

	bool SolventPredicate::operator () (const Atom& /* atom */) const
	{
		//BAUSTELLE
		return false;
	}
	
	// backbone predicate

	bool BackBonePredicate::operator () (const Atom& atom) const
	{
		if (atom.hasAncestor(RTTI::getDefault<Residue>()))
		{
			String name = RTTI::castTo<Atom>(atom)->getName();
			if ((name == "C") || (name == "N") || (name == "CA") || (name == "O"))
			{
				return true;
			}
		}

		return false;
	}
	
	// nucleotide predicate

	bool NucleotidePredicate::operator () (const Atom& atom) const
	{
		return RTTI::isKindOf<Nucleotide>(atom);
	}
	
} // namespace BALL
