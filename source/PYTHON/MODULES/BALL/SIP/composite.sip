// $Id: composite.sip,v 1.2 2000/08/28 15:48:08 oliver Exp $

class Composite
  :  PersistentObject,
   	 Object,
   	 Selectable
{
%HeaderCode	
	#include <BALL/CONCEPT/composite.h>
	using namespace BALL;
%End
  public:
  // typedef UnaryPredicate<Composite> KernelPredicateType;
  // BALL_CREATE(Composite)
  // static UnaryProcessor<Composite> DEFAULT_PROCESSOR;
  // static KernelPredicateType DEFAULT_UNARY_PREDICATE;
  Composite();
  Composite(const Composite& /composite/, bool /deep/ = true);
  ~Composite();
  virtual void clear();
  virtual void destroy();
  void destroy(bool /virtual_destroy/);
  // void* clone(Composite& /root/, KernelPredicateType& /predicate/) const;
  // virtual void persistentWrite(PersistenceManager& /pm/, const char* /name/ = 0) const;
  // virtual void persistentRead(PersistenceManager& /pm/);
  // void set(const Composite& /composite/, KernelPredicateType& /predicate/);
  void set(const Composite& /composite/, bool /deep/ = true);
  // Composite& operator = (const Composite& /composite/);
  // void get(Composite& /composite/, KernelPredicateType& predicate) const;
  void get(Composite& /composite/, bool /deep/ = true) const;
  Size getDegree() const;
  // Size count(const KernelPredicateType& /predicate/) const;
  Size countDescendants() const;
  static Size getPathLength(const Composite& /a/, const Composite& /b/);
  // static Size getDepth(const Composite& /composite/);
  Size getDepth() const;
  Size getHeight() const;
  Composite& getRoot();
  const Composite& getRoot() const;
  Composite* getLowestCommonAncestor(Composite& /composite/);
  const Composite* getLowestCommonAncestor(const Composite& /composite/) const;
  // template <typename T> T* getAncestor(const T&  );
  // template <class T> const T* getAncestor(const T&  ) const;
  // template <typename T> T* getPrevious(const T&  );
  // template <class T> const T* getPrevious(const T& dummy) const;
  // template <typename T> T* getNext(const T&  );
  // template <class T> const T* getNext(const T& dummy) const;
  Composite* getParent();
  const Composite* getParent() const;
  Composite* getChild(Index /index/);
  const Composite* getChild(Index /index/) const;
  Composite* getSibling(Index /index/);
  const Composite* getSibling(Index /index/) const;
  Composite* getFirstChild();
  const Composite* getFirstChild() const;
  Composite* getLastChild();
  const Composite* getLastChild() const;
  const Time& getModificationTime() const throw();
  const Time& getSelectionTime() const throw();
  void expand();
  void collapse();
  void prependChild(Composite& /composite/);
  void appendChild(Composite& /composite/);
  static bool insertParent
   (Composite& /parent/, Composite& /first/, 
    Composite& /last/, bool /destroy_parent/ = true);
  void insertBefore(Composite& /composite/);
  void insertAfter(Composite& /composite/);
  void spliceBefore(Composite& /composite/);
  void spliceAfter(Composite& /composite/);
  void splice(Composite& /composite/);
  bool removeChild(Composite& /child/);
  void replace(Composite& /composite/);
  void swap(Composite& /composite/);
  virtual void select();
  virtual void deselect();
  bool isExpanded() const;
  bool isCollapsed() const;
  bool isEmpty() const;
  bool isRoot() const;
  bool isRootOf(const Composite& /composite/) const;
  bool isInterior() const;
  bool hasChild() const;
  bool isChildOf(const Composite& /composite/) const;
  bool isFirstChild() const;
  bool isFirstChildOf(const Composite& /composite/) const;
  bool isLastChild() const;
  bool isLastChildOf(const Composite& /composite/) const;
  bool hasParent() const;
  bool isParentOf(const Composite& /composite/) const;
  bool hasSibling() const;
  bool isSiblingOf(const Composite& /composite/) const;
  bool hasPreviousSibling() const;
  bool isPreviousSiblingOf(const Composite& /composite/) const;
  bool hasNextSibling() const;
  bool isNextSiblingOf(const Composite& /composite/) const;
  bool isDescendantOf(const Composite& /composite/) const;
  // template <typename T> bool hasAncestor(const T& dummy ) const; 
  bool isAncestorOf(const Composite& /composite/) const;
  bool isRelatedWith(const Composite& /composite/) const;
  bool isHomomorph(const Composite& /composite/) const;
  bool containsSelection() const;
  virtual bool isValid() const;
  // virtual void dump(std::ostream& s = std::cout, Size depth = 0) const;
  // void host(Visitor<Composite>& visitor);
  // template <typename T> bool applyAncestor(UnaryProcessor<T>& processor);
  // template <typename T> bool applyChild(UnaryProcessor<T>& processor);
  // template <typename T> bool applyDescendantPreorder(UnaryProcessor<T>& processor);
  // template <typename T> bool applyDescendantPostorder(UnaryProcessor<T>& processor);
  // template <typename T> bool applyDescendant(UnaryProcessor<T>& processor);
  // template <typename T> bool applyPreorder(UnaryProcessor<T>& processor);
  // template <typename T> bool applyPostorder(UnaryProcessor<T>& processor);
  // template <typename T> bool apply(UnaryProcessor<T>& processor);
  // template <typename T> bool applyLevel(UnaryProcessor<T>& processor, long level);
};
