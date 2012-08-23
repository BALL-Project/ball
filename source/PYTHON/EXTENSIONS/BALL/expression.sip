// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class ExpressionPredicate
//  : public UnaryPredicate<Atom>
	: UnaryAtomPredicate
{
%TypeHeaderCode
	#include <BALL/KERNEL/expression.h>
	using namespace BALL;
	typedef UnaryPredicate<Atom> UnaryAtomPredicate;
%End
  public:
  // BALL_CREATE_NODEEP(ExpressionPredicate)
  ExpressionPredicate();
  ExpressionPredicate(const String&);
	~ExpressionPredicate() throw();
  bool operator () (const Atom&) const throw();
%MethodCode
	sipRes = sipCpp->operator () (*a0);
%End

  virtual void setArgument(const String&) throw();
};

//class ExpressionTree
//{
//  public:
//  enum Type
//  { 
//   INVALID = 0,
//   LEAF,
//   OR,
//   AND
//  };
//  ExpressionTree();
//  ExpressionTree(ExpressionPredicate* predicate, bool negate = false);
//  ExpressionTree(Type type, list<ExpressionTree*> children, bool negate = false);
//  virtual ~ExpressionTree();
//  virtual bool operator () (const Atom& atom) const;
//  void setType(Type type);
//  void setNegate(bool negate);
//  void setPredicate(ExpressionPredicate* predicate);
//  void appendChild(ExpressionTree* child);
//  protected:
//  Type         type_;
//  bool         negate_;
//  ExpressionPredicate* predicate_;
//  list<ExpressionTree*> children_;
//};
//class SyntaxTree
//{
//  public:
//  typedef list<SyntaxTree*>::iterator    Iterator;
//  typedef list<SyntaxTree*>::const_iterator ConstIterator;
//  SyntaxTree();
//  SyntaxTree(const String& expression);
//  virtual ~SyntaxTree();
//  Iterator begin();
//  Iterator end();
//  ConstIterator begin() const;
//  ConstIterator end() const; 
//  void mergeLeft(SyntaxTree* tree);
//  void mergeRight(SyntaxTree* tree);
//  void parse();
//  String        expression;
//  String        argument;
//  bool         evaluated;
//  bool         negate;
//  ExpressionTree::Type type;
//  list<SyntaxTree*>   children;
//  protected:
//  void expandBrackets_();
//  void collapseANDs_();
//  void collapseORs_();
//};
class Expression
{
%TypeHeaderCode
	#include <BALL/KERNEL/expression.h>
	using namespace BALL;
%End
  public:
  // typedef void * (*CreationMethod) ();
  Expression();
  Expression(const Expression&);
  Expression(const String&);
  ~Expression() throw();
  bool hasPredicate(const String&);
  bool operator () (const Atom&) const throw();
%MethodCode
	sipRes = sipCpp->operator () (*a0);
%End

  ExpressionPredicate* getPredicate(const String&, const String& args = "") const;
  // void registerPredicate(const String& name, CreationMethod creation_method);
  void setExpression(const String&);
  const String& getExpressionString() const;
};
