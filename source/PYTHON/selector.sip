// $Id: selector.sip,v 1.2 2000/07/03 11:11:13 oliver Exp $

class ExpressionPredicate
//  : public UnaryPredicate<Composite>
{
  public:
  ExpressionPredicate();
  ExpressionPredicate(const String& argument);
  virtual ~ExpressionPredicate();
  virtual bool operator () (const Composite& composite) const = 0;
  virtual void setArgument(const String& argument);
  protected:
  String argument_;
};

class Selector
  : public UnaryProcessor<Composite>
{
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
   ExpressionNode();
   ExpressionNode(ExpressionPredicate* predicate, bool negate = false);
   ExpressionNode(Type type, list<ExpressionNode*> children, bool negate = false);
   virtual ~ExpressionNode();
   virtual bool operator () (const Composite& composite) const;
   void setType(Type type);
   void setNegate(bool negate);
   void setPredicate(ExpressionPredicate* predicate);
   void appendChild(ExpressionNode* child);
   protected:
   Type         type_;
   bool         negate_;
   ExpressionPredicate* predicate_;
   list<ExpressionNode*> children_;
  };
  class SyntaxTree
  {
   public:
   typedef list<SyntaxTree*>::iterator    Iterator;
   typedef list<SyntaxTree*>::const_iterator ConstIterator;
   SyntaxTree(const String& expression);
   ~SyntaxTree();
   Iterator begin();
   Iterator end();
   ConstIterator begin() const;
   ConstIterator end() const; 
   void mergeLeft(SyntaxTree* tree);
   void mergeRight(SyntaxTree* tree);
   void parse();
   String        expression;
   String        argument;
   bool         evaluated;
   bool         negate;
   ExpressionNode::Type type;
   list<SyntaxTree*>   children;
   protected:
   void expandBrackets_();
   void collapseANDs_();
   void collapseORs_();
  };
  public:
  typedef void * (*CreationMethod) ();
  Selector();
  Selector(const String& expression);
  virtual ~Selector();
  bool hasPredicate(const String& name);
  virtual Processor::Result operator () (Composite& composite);
  ExpressionPredicate* getPredicate(const String& name, const String& args = "") const;
  void registerPredicate(const String& name, CreationMethod creation_method);
  void setExpression(const String& expression);
  protected:
  ExpressionNode* constructExpressionTree_(const SyntaxTree& tree);
  void registerStandardPredicates_();
  ExpressionNode*        expression_tree_;
  StringHashMap<CreationMethod> create_methods_;
 };
 class AtomNamePredicate
  : public ExpressionPredicate
 {
  public:
  virtual bool operator () (const Composite& composite) const;
 };
 class AtomTypePredicate
  : public ExpressionPredicate
 {
  public:
  virtual bool operator () (const Composite& composite) const;
 };
 class ElementPredicate
  : public ExpressionPredicate
 {
  public:
  virtual bool operator () (const Composite& composite) const;
 };
 class ResiduePredicate
  : public ExpressionPredicate
 {
  public:
  virtual bool operator () (const Composite& composite) const;
 };
 class ResidueIDPredicate
  : public ExpressionPredicate
 {
  public:
  virtual bool operator () (const Composite& composite) const;
 };
 class ProteinPredicate
  : public ExpressionPredicate
 {
  public:
  virtual bool operator () (const Composite& composite) const;
 };
 class ChainPredicate
  : public ExpressionPredicate
 {
  public:
  virtual bool operator () (const Composite& composite) const;
 };
 class SecondaryStructurePredicate
  : public ExpressionPredicate
 {
  public:
  virtual bool operator () (const Composite& composite) const;
 };
 class SolventPredicate
  : public ExpressionPredicate
 {
  public:
  virtual bool operator () (const Composite& composite) const;
 };
 class MoleculePredicate
  : public ExpressionPredicate
 {
  public:
  virtual bool operator () (const Composite& composite) const;
 };
 class BackBonePredicate
  : public ExpressionPredicate
 {
  public:
  virtual bool operator () (const Composite& composite) const;
 };
 class NucleicAcidPredicate
  : public ExpressionPredicate
 {
  public:
  virtual bool operator () (const Composite& composite) const;
 };
 class NucleotidePredicate
  : public ExpressionPredicate
 {
  public:
  virtual bool operator () (const Composite& composite) const;
 };
}  
