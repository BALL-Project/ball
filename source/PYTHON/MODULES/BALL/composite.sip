	class Composite
		: PersistentObject,
		 	Object,
			Selectable
	{
%HeaderCode
#include <BALL/CONCEPT/composite.h>
using namespace BALL;
%End
 	

		public:

		//typedef	UnaryPredicate<Composite>	KernelPredicateType;
		//static UnaryProcessor<Composite> DEFAULT_PROCESSOR;
		//static UnaryPredicate<Composite> DEFAULT_UNARY_PREDICATE;
		Composite();

		Composite(const Composite&, bool = true);

		//virtual ~Composite();

		virtual void clear();
	
		virtual void destroy();
		void destroy(bool);
		// void* clone(Composite&, UnaryPredicate<Composite>&) const;
		// virtual void persistentWrite(PersistenceManager&, const char* = 0) const;
		// virtual void persistentRead(PersistenceManager&);
		// void set(const Composite&, UnaryPredicate<Composite>&);

		void set(const Composite&, bool = true);

		//Composite& operator = (const Composite&);
		//void get(Composite& composite, UnaryPredicate<Composite>& predicate) const;

		void get(Composite&, bool = true) const;
		unsigned long getDegree() const;
		// unsigned long count(const KernelPredicateType&) const;
		unsigned long countDescendants() const;
		static unsigned long getPathLength(const Composite&, const Composite&);
		// static unsigned long getDepth(const Composite&);
		unsigned long getDepth() const;
		unsigned long getHeight() const;
		Composite& getRoot();
		const Composite& getRoot() const;
		Composite* getLowestCommonAncestor(Composite&);
		const Composite* getLowestCommonAncestor(const Composite&) const;
		Composite* getParent();
		const Composite* getParent() const;
		Composite* getChild(int);
		const Composite* getChild(int) const;
		Composite* getSibling(int);
		const Composite* getSibling(int) const;
		Composite* getFirstChild();
		const Composite* getFirstChild() const;
		Composite* getLastChild();
		const Composite* getLastChild() const;
		void expand();
		void collapse();
		void prependChild(Composite&);
		void appendChild(Composite&);
		static bool insertParent
			(Composite&, Composite& , 
			 Composite&, bool = true);

		void insertBefore(Composite&);
		void insertAfter(Composite&);
		void spliceBefore(Composite&);
		void spliceAfter(Composite&);
		void splice(Composite&);
		bool removeChild(Composite&);
		void replace(Composite&);
		void swap(Composite&);
		virtual void select();
		virtual void deselect();
		bool isExpanded() const;
		bool isCollapsed() const;
		bool isEmpty() const;
		bool isRoot() const;
		bool isRootOf(const Composite&) const;
		bool isInterior() const;
		bool hasChild() const;
		bool isChild() const;
		bool isChildOf(const Composite&) const;
		bool isFirstChild() const;
		bool isFirstChildOf(const Composite&) const;
		bool isLastChild() const;
		bool isLastChildOf(const Composite&) const;
		bool hasParent() const;
		bool isParent() const;
		bool isParentOf(const Composite&) const;
		bool hasSibling() const;
		bool isSiblingOf(const Composite&) const;
		bool hasPreviousSibling() const;
		bool isPreviousSiblingOf(const Composite&) const;
		bool hasNextSibling() const;
		bool isNextSiblingOf(const Composite&) const;
		bool hasDescendant() const;
		bool isDescendant() const;
		bool isDescendantOf(const Composite&) const;
		bool hasAnyAncestor() const;
		bool isAncestor() const;
		bool isAncestorOf(const Composite&) const;
		bool isRelatedWith(const Composite&) const;
		bool isHomomorph(const Composite&) const;
		bool containsSelection() const;
		virtual bool isValid() const;
		//virtual void dump(ostream& = cout, unsigned long = 0) const;
		
//		void host(Visitor<Composite>&);
//		bool applyAncestor(UnaryProcessor<Composite>&);
//		bool applyChild(UnaryProcessor<Composite>&);
//		bool applyDescendantPreorder(UnaryProcessor<Composite>&);
//		bool applyDescendantPostorder(UnaryProcessor<Composite>&);
//		bool applyDescendant(UnaryProcessor<Composite>&);
//		bool applyPreorder(UnaryProcessor<Composite>&);
//		bool applyPostorder(UnaryProcessor<Composite>&);
//		bool apply(UnaryProcessor<Composite>&);
//		bool applyLevel(UnaryProcessor<Composite>&, long);

	};
