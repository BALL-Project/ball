// $Id: expression.sip,v 1.1 2000/07/18 17:02:23 oliver Exp $

class ExpressionPredicate
//  : public UnaryPredicate<Atom>
	: UnaryAtomPredicate
{
%HeaderCode
	#include <BALL/KERNEL/expression.h>
	using namespace BALL;
%End
  public:
  // BALL_CREATE_NODEEP(ExpressionPredicate)
  ExpressionPredicate();
  ExpressionPredicate(const String& /argument/);
	~ExpressionPredicate();
  virtual bool operator () (const Atom& /atom/) const;
  virtual void setArgument(const String& /argument/);
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
%HeaderCode
	#include <BALL/KERNEL/expression.h>
	using namespace BALL;
%End
  public:
  // typedef void * (*CreationMethod) ();
  Expression();
  Expression(const Expression& /expression/);
  Expression(const String& /expression_string/);
  ~Expression();
  bool hasPredicate(const String& /name/);
  virtual bool operator () (const Atom& /atom/) const;
  ExpressionPredicate* getPredicate(const String& /name/, const String& /args/ = "") const;
  // void registerPredicate(const String& name, CreationMethod creation_method);
  void setExpression(const String& /expression/);
  const String& getExpression() const;
};
