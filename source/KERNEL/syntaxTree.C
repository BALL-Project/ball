// $Id: syntaxTree.C,v 1.4 2001/07/17 09:31:05 anker Exp $

#include <BALL/KERNEL/syntaxTree.h>

namespace BALL
{

	SyntaxTree::SyntaxTree()
		throw()
		:	expression(""),
			argument(""),
			evaluated(false),
			negate(false),
			type(ExpressionTree::INVALID),
			children()
	{
	}

	SyntaxTree::SyntaxTree(const String& s)
		throw()
		:	expression(s),
			argument(""),
			evaluated(false),
			negate(false),
			type(ExpressionTree::INVALID),
			children()
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

	void SyntaxTree::clear()
		throw()
	{
		expression = "";
		argument = "";
		evaluated = false;
		negate = false;
		type = ExpressionTree::INVALID;
		children.clear();
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
		// if the tree does not bear any children, save the pointer as the
		// first of our own childs and return
    if (tree->children.empty())
    {
      children.push_front(tree);
      return;
		}

		// else go through the children of the tree (from right to left) and
		// save them in our own list of children.
    for (list<SyntaxTree*>::reverse_iterator it = tree->children.rbegin(); 
				 it != tree->children.rend(); ++it)
    {
      children.push_front(*it);
		}

		// now erase all the children of the tree...
    tree->children.erase(tree->begin(), tree->end());

		// and the tree itself.
    delete tree;
	}

	void SyntaxTree::mergeRight(SyntaxTree* tree)
		throw()
	{
		// if the tree does not bear any children, save the pointer as the
		// first of our own childs and return
    if (tree->children.empty())
    {
      children.push_back(tree);
      return;
		}

		// else go through the children of the tree (from left to right) and
		// save them in our own list of children.
    for (Iterator it = tree->begin(); it != tree->end(); ++it)
    {
      children.push_back(*it);
		}

		// now erase all the children of the tree...
    tree->children.erase(tree->begin(), tree->end());

		// ... and the tree itself.
    delete tree;
	}

	void SyntaxTree::parse()
		throw(Exception::ParseError)
	{
    if (!evaluated)
    {
      expandBrackets_();
			Log.info() << endl << "before collapseANDs_()" << endl;
			dump();
      collapseANDs_();
			Log.info() << endl << "after collapseANDs_()" << endl;
			dump();
      collapseORs_();
			Log.info() << endl << "after collapseORs_()" << endl;
			dump();
		}
	}
 
	void SyntaxTree::expandBrackets_()
		throw(Exception::ParseError)
	{
    // we do not try to expand already processed nodes
    if (evaluated == true)
    {
      return;
		}

		// if this node represents a conjunction or a disjunction but doesn't
		// bear any children, the expression is not parseable.
    if ((expression == "AND" || expression == "OR") && children.size() == 0)
    {
			// BAUSTELLE
			// Why is it correct, if we arrive here?
			// throw Exception::ParseError(__FILE__, __LINE__, expression.c_str(),
			//		"conjunction without children");
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
			// we didn't find an opening bracket
			throw Exception::ParseError(__FILE__, __LINE__, s.c_str(),
					"can't find opening '('");
		}

		// the subexpression y has to start one position after the bracket.
    sy = ex;
    sy++;

		// we found an opening bracket, so increase the counter
    Index bracket_count = 1;

		// go through the expression and count opening and closing brackets.
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
			// we found too many opening brackets
			throw Exception::ParseError(__FILE__, __LINE__, s.c_str(),
					"didn't find closing ')'");
		}

		if (bracket_count < 0)
		{
			// we found too many closing brackets.
			throw Exception::ParseError(__FILE__, __LINE__, s.c_str(),
					"found too many closing ')'");
		}

		// the subexpression y ends at position i which points to the last
		// closing bracket (see for() {} above)
    ey = i;

		// BAUSTELLE: I think ey >= s.size() cannot happen, because the for()
		// statement above runs at most to s.size() - 1.
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
      SyntaxTree* new_t = new SyntaxTree(String(s, (Index)sy, 
						(Index)(ey - sy - 1)));
      children.push_front(new_t);
      new_t->expandBrackets_();

		} 
		else 
		{

      // get word directly to the left of the opening bracket
      String left_word = left.getField((Index)number_of_fields - 1);
      if (left_word == "AND" || left_word == "OR" || left_word == "!")
      {
        SyntaxTree* new_t = new SyntaxTree(String(s, (Index)sy,
							(Index)(ey - sy - 1)));
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

        SyntaxTree* new_t = new SyntaxTree(String(s, (Index)sy,
							(Index)(ey - sy - 1)));
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
      SyntaxTree* new_t = new SyntaxTree(String(s, (Index)sx,
						(Index)(ex - sx)));
      new_t->expandBrackets_();
      mergeLeft(new_t);
		}
    if (ez != sz)
    {
      SyntaxTree* new_t = new SyntaxTree(String(s, (Index)sz,
						(Index)(ez - sz)));
      new_t->expandBrackets_();
      mergeRight(new_t);
		}
	}
 
	void SyntaxTree::collapse_(ExpressionTree::Type type, const String& string)
		throw(Exception::ParseError)
	{

		// if we have less than 3 children, we cannot collapse anything.
    if (children.size() < 3)
    {
      return;
		}

    Iterator  it = begin();
    while (it != end())
    {
      for (; it != end() && (*it)->expression != string; ++it);

      if (it == end())
      {
        break;
			}

      // remember the first node in the AND expression
      Iterator  start = it;
      start--;

      for (; it != end() 
					&& ((*it)->expression == string || (*it)->evaluated == true); ++it);

      // remember the last node for the AND expression
      Iterator  end = it;

      SyntaxTree* t = new SyntaxTree(string);
      it = start;
      children.insert(start, t);
      t->type = type;
      t->evaluated = true;
      t->collapse_(type, string);

      for (it = start; it != end; ++it)
      {
        if ((*it)->expression != string)
        {
          t->children.push_back(*it);
				}
			}
      children.erase(start, end);

      it = end;
		}

    for (it = begin(); it != end(); ++it)
    {
      (*it)->collapse_(type, string);
		}

		// mark as successfully updated
		evaluated = true;
	}
 

  void SyntaxTree::collapseANDs_()
		throw(Exception::ParseError)
	{
		collapse_(ExpressionTree::AND, "AND");
	}


  void SyntaxTree::collapseORs_()
		throw(Exception::ParseError)
	{
		collapse_(ExpressionTree::OR, "OR");
	}


	void SyntaxTree::dump(std::ostream& os, Size depth) const
		throw()
	{
		BALL_DUMP_STREAM_PREFIX(os);
		BALL_DUMP_HEADER(os, this, this);
		BALL_DUMP_DEPTH(os, depth);
		os << "[expression = " << expression 
			<< "  arg = " << argument
			<< "  evaluated = " << evaluated 
			<< "  negate = " << negate 
			<< "  type = " << type
			<< "]" << ::std::endl;
		list<SyntaxTree*>::const_iterator it = children.begin();
		for (; it != children.end(); ++it)
		{
			(*it)->dump(os, depth + 2);
		}
		BALL_DUMP_STREAM_SUFFIX(os);
	}
}
