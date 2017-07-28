class Composite
	: PersistentObject,
	  Object,
	  Selectable
{
%TypeHeaderCode
	#include <BALL/CONCEPT/composite.h>
%End

	public:
	Composite();
	Composite(const Composite&, bool deep = true);
	~Composite() throw();
	virtual void clear() throw();
	virtual void destroy() throw();
	void destroy(bool);
	void set(const Composite&, bool deep = true);
	void get(Composite&, bool deep = true) const;
	Size getDegree() const;
	Size countDescendants() const;
	Size getPathLength(const Composite&);
	Size getDepth() const;
	Size getHeight() const;
	Composite& getRoot();
	Composite* getLowestCommonAncestor(Composite&);
	Composite* getParent();
	Composite* getChild(Index);
	Composite* getSibling(Index);
	Composite* getFirstChild();
	Composite* getLastChild();

	const PreciseTime& getModificationTime() const throw();
	const PreciseTime& getSelectionTime() const throw();
	void prependChild(Composite& /Transfer/);
	void appendChild(Composite& /Transfer/);
	static bool insertParent
		(Composite&, Composite&,
		Composite&, bool destroy_parent = true);
	void insertBefore(Composite& /Transfer/);
	void insertAfter(Composite& /Transfer/);
	void spliceBefore(Composite&);
	void spliceAfter(Composite&);
	void splice(Composite&);
	bool removeChild(Composite&);
	bool removeSelected();
	void replace(Composite&);
	void swap(Composite&);
	virtual void select() throw();
	virtual void deselect() throw();
	bool isEmpty() const;
	bool isRoot() const;
	bool isRootOf(const Composite&) const;
	bool isInterior() const;
	bool hasChild() const;
	bool isChildOf(const Composite&) const;
	bool isFirstChild() const;
	bool isFirstChildOf(const Composite&) const;
	bool isLastChild() const;
	bool isLastChildOf(const Composite&) const;
	bool hasParent() const;
	bool isParentOf(const Composite&) const;
	bool hasSibling() const;
	bool isSiblingOf(const Composite&) const;
	bool hasPreviousSibling() const;
	bool isPreviousSiblingOf(const Composite&) const;
	bool hasNextSibling() const;
	bool isNextSiblingOf(const Composite&) const;
	bool isDescendantOf(const Composite&) const;
	bool isAncestorOf(const Composite&) const;
	bool isRelatedWith(const Composite&) const;
	bool isHomomorph(const Composite&) const;
	bool containsSelection() const;
	virtual bool isValid() const throw();
};
