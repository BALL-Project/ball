// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: composite.h,v 1.47 2003/06/11 16:09:22 oliver Exp $
//

#ifndef BALL_CONCEPT_COMPOSITE_H
#define BALL_CONCEPT_COMPOSITE_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_CONCEPT_PERSISTENTOBJECT_H
#	include <BALL/CONCEPT/persistentObject.h>
#endif

#ifndef BALL_CONCEPT_COMPARATOR_H
#	include <BALL/CONCEPT/comparator.h>
#endif

#ifndef BALL_CONCEPT_BIDIRECTIONALITERATOR_H
#	include <BALL/CONCEPT/bidirectionalIterator.h>
#endif

#ifndef BALL_CONCEPT_OBJECT_H
#	include <BALL/CONCEPT/object.h>
#endif

#ifndef BALL_CONCEPT_SELECTABLE_H
#	include <BALL/CONCEPT/selectable.h>
#endif

#ifndef BALL_CONCEPT_VISITOR_H
#	include <BALL/CONCEPT/visitor.h>
#endif

#ifndef BALL_CONCEPT_PROCESSOR_H
#	include <BALL/CONCEPT/processor.h>
#endif

#ifndef BALL_CONCEPT_TIMESTAMP_H
#	include <BALL/CONCEPT/timeStamp.h>
#endif

///
namespace BALL 
{
	/**	Composite Class.
			This class implements a variant of the composite design pattern. A
			Composite may contain an arbitrary number of other composites, thus
			forming a tree. All BALL kernel classes are derived from Composite.
			This provides a unique interface for all kernel classes.
			 \par
			The composite class provides a selection mechanism that allows
			hierarchical selection and deselection of arbitrary subtrees. The
			time of the last selection/deselection operation is stored as well as
			the time of the last modification operation in time stamps that can
			be accessed via  \link getModificationTime getModificationTime \endlink  and  \link getSelectionTime getSelectionTime \endlink .
			Selecting or deselecting a Composite automatically selects or
			deselects all its children (recursively!).  Selecting or deselecting
			all children of a node deselects their parent as well.	Selection
			information is propagated upwards in the tree.
			 \par
			Composites are persistent objects. 
			 \par
			
			\ingroup ConceptsMiscellaneous 
	*/
	class Composite
		: public PersistentObject,
			public Selectable
	{
		public:

		/**	@name	Type Definitions and Enums
		*/
		//@{

#ifndef BALL_KERNEL_PREDICATE_TYPE
#define BALL_KERNEL_PREDICATE_TYPE
		/**	Composite predicate type.
				This type declares a predicate operating on composites.
				As it is used as a predicate for all kernel classes,
				it is named KernelPredicateType.
		*/
		typedef	UnaryPredicate<Composite>	KernelPredicateType;
#endif

		/**	Time stamp type.
		*/
		enum StampType
		{
			/**	
			*/
			MODIFICATION = 1,
			/**
			*/
			SELECTION = 2,
			/**	
			*/
			BOTH = 3
		};
		//@}
				
		BALL_CREATE_DEEP(Composite)

		static UnaryProcessor<Composite> DEFAULT_PROCESSOR;
		static KernelPredicateType DEFAULT_UNARY_PREDICATE;
		
		/**	@name	Construction and Destruction 
		*/
		//@{
		
		/**	Default constructor.
				This constructor creates an empty composite object.
		*/
		Composite()
			throw();

		/**	Copy constructor.
				Creates a copy of a composite. {\em Deep} copies include the whole
				composite tree, {\em shallow} copies contain anly a single composite.
				@param	composite the composite to be cloned (the root of the tree in 
								the case of a deep copy)
				@param	deep make a deep copy (<b>true</b>) or shallow copy 
								(<b>false</b>)
		*/
		Composite(const Composite& composite, bool deep = true)
			throw();

		/**	Destructor.	
				The destructor calls  \link destroy destroy \endlink  to remove the composite from 
				potential tree structures. It also recursively destructs all
				children of the composite.
		*/
		virtual ~Composite()
			throw();

		/**	Clear the composite properties.	
				This method removes the composite's children and destructs them if
				they are auto-deletable.
				 \par
				It does not remove the composite from any parental structure.
				 \par
				This  method updates the modification time stamp of <tt>this</tt>.
				@see	stamp
				@see	AutoDeletable
				@see	destroy
		*/
		virtual void clear()
			throw();
	
		/**	Destroy the composite.
				This method removes the composite from potential parental
				structures and then calls  \link clear clear \endlink  to destruct all children.
				 \par
				This  method updates the modification time stamp of <tt>this</tt>.
				@see stamp
				@see	~Composite
				@see	clear
		*/
		virtual void destroy()
			throw();

		/**	Non-virtual destroy method.
				This method behaves exactly like destroy except for a small
				difference: when called with <b>true</b>, it calls the {\em virtual}
				clear function. If called with <b>false</b> it calls the original
				clear function of Composite. This is useful when implementing the
				behaviour of derived classes.
				 \par
				This  method updates the modification time stamp of <tt>this</tt>.
				@see stamp
				@param	virtual_destroy call the virtual clear method (<b>true</b>) or
								<tt>Composite::clear()</tt> (<b>false</b>)
		*/		
		void destroy(bool virtual_destroy)
			throw();

		/**	Clone with a predicate.
				This method copies the attributes of <tt>this</tt> composite to root
				(shallow copy) and then adds recursively each of its children.
				@param	root the cloning target root is <tt>destroy</tt>ed prior to
								any copying 
				@return  a pointer to the root composite (<tt>&root</tt>)
		*/
		void* clone(Composite& root) const
			throw();

		//@}		

		/**	@name	Persistence 
		*/
		//@{
		
		/** Write a persistent copy of the object.
				@param	pm the persistence manager
				@param	name the object name
		*/
		virtual void persistentWrite(PersistenceManager& pm,
				const char* name = 0) const
			throw(Exception::GeneralException);

		/** Read a persistent object.
				@param	pm the persistence manager
		*/
		virtual void persistentRead(PersistenceManager& pm)
			throw(Exception::GeneralException);

		//@}

		/**	@name	Modifying and Accessing the Tree 
		*/
		//@{

		/**	Assignment.
				@param	composite the Composite tree to assign from
				@param	deep a <tt>bool</tt> deciding whether the assignment will be
								deep or shallow.
		*/
		void set(const Composite& composite, bool deep = true)
			throw();

		/**	Assignment operator.
				@param	composite the Composite tree to assign from
				@return	a const reference to <b>this</b>
		*/
		const Composite& operator = (const Composite& composite)
			throw();

		/**	Assignment of a tree to another.
				Create a deep (<tt>deep</tt> = <b>true</b>) or shallow copy of a composite
				and assign it to <tt>composite</tt>. <tt>composite</tt> is destroyed first.
				@param	composite the composite to assign the copy to
				@param	deep <b>true</b> for a deep copy
		*/
		void get(Composite& composite, bool deep = true) const
			throw();

		/**	Return the degree of the node.
				This method returns the number of children of a composite object.
				@return Size the number of children
		*/
		Size getDegree() const
			throw();

		/**	Count the number of nodes fulfilling a predicate in this subtree.
				@param	predicate the predicate
				@return Size the number of nodes in the subtree satisfying the predicate
		*/
		Size count(const KernelPredicateType& predicate) const
			throw();

		/**	Count the number of descendants.
				@return Size the number of descendants of this node
		*/
		Size countDescendants() const
			throw();

		/** Get the length of the path between two composite objects.
				If no path exists <tt>INVALID_SIZE</tt> is returned.
				@param composite the second object
				@return Size the size of the path
		*/
		Size getPathLength(const Composite& composite) const
			throw();

		/** Get the depth of this item in its tree.
				The depth of a root item is 0.
				@return Size the depth
		*/
		Size getDepth() const
			throw();

		/** Get the height of this item in its tree.
				The hight of a leaf is 0.
				@return Size the height
		*/
		Size getHeight() const
			throw();

		/** Get the root of this item.
				@return Composite& the root
		*/
		Composite& getRoot()
			throw();

		/** Get a const reference to the root of this item.
				@return Composite& the root
		*/
		const Composite& getRoot() const
			throw();

		/** Get the lowest common ancestor of this item with an other.
				If no comman ancestor exists 0 is returned.
				@return Composite& the lowest common ancestor
		*/
		Composite* getLowestCommonAncestor(Composite& composite)
			throw();

		/** Get a const reference to the lowest common ancestor of this item
				with an other. If no common ancestor exists, 0 is returned.
				@return Composite& the lowest common ancestor
		*/
		const Composite* getLowestCommonAncestor(const Composite& composite) const
			throw();

		/**	Find the first ancestor of type T.
				This method walks up the tree from parent to parent and
				checks whether the composite object is a kind of <tt>T</tt>.
				This method is useful to identify special container classes.
				@return a pointer to the first composite found that is a kind of T
								or 0 if no matching composite was found up to the root of
								the tree
		*/
		template <typename T>
		T* getAncestor(const T& /* dummy */)
			throw();

		/**	Find the first ancestor of type T (const method).
				This method operates also on constant trees.
				@return a pointer to the first composite found that is a kind of T
								or 0 if no matching composite was found to the root of the
								tree
		*/
		template <class T>
		const T* getAncestor(const T& /* dummy */) const
			throw();

		/**	Find the nearest previous composite of type T.
				This method walks backward in the tree from composite to composite and
				checks whether the composite object is a kind of <tt>T</tt>.
				@return a pointer to the first composite found that is a kind of T
								or 0 if no matching composite was found up to the root of
								the tree
		*/
		template <typename T>
		T* getPrevious(const T& /* dummy */)
			throw();

		/**	Find the nearest previous composite of type T (const method).
				This method walks backward in the tree from composite to composite and
				checks whether the composite object is a kind of <tt>T</tt>.
				@return a pointer to the first composite found that is a kind of T
								or 0 if no matching composite was found up to the root of
								the tree
		*/
		template <class T>
		const T* getPrevious(const T& dummy) const
			throw();

		/**	Find the next composite of type T.
				This method walks backward in the tree from composite to composite and
				checks whether the composite object is a kind of <tt>T</tt>.
				@return a pointer to the first composite found that is a kind of T
								or 0 if no matching composite was found up to the root of
								the tree
		*/
		template <typename T>
		T* getNext(const T& /* dummy */)
			throw();

		/**	Find the next composite of type T (const method).
				This method walks backward in the tree from composite to composite and
				checks whether the composite object is a kind of <tt>T</tt>.
				@return a pointer to the first composite found that is a kind of T
								or 0 if no matching composite was found up to the root of
								the tree
		*/
		template <class T>
		const T* getNext(const T& dummy) const
			throw();

		/**	Return the composite's parent.
				@return a pointer to the parent composite or 0 if no parent exists
		*/
		Composite* getParent()
			throw();

		/**	Return the composite's parent (const method).
				@return a pointer to the parent composite or 0 if no parent exists
		*/
		const Composite* getParent() const
			throw();

		/**	Return the {\em index}th child of this composite.
				If no such child exists, 0 is returned.
				The index of the first child is <b>0</b>.
				@param	index the index of the child to return
				@return	a pointer to the child or 0 if there is no such child.
		*/
		Composite* getChild(Index index)
			throw();
	
		/**	Return a const pointer to the {\em index}th child of this composite.
				If no such child exists, 0 is returned.
				The index of the first child is <b>0</b>.
				@param	index the index of the child to return
				@return	a const pointer to the child or 0 if there is no such child.
		*/
		const Composite* getChild(Index index) const
			throw();
	
		/**	Return a pointer to the sibling index positions from this composite.
				A pointer to the sibling <tt>index</tt> positions to the right (for
				positive values of <tt>index</tt>) or <tt>-index</tt> positions to the left 
				(for negative values of <tt>index</tt>) is returned.
				For Index = 0 the this-pointer is returned.
				@param index the index of the sibling
				@return	a pointer to the child or 0 if there is no such sibling.
		*/
		Composite* getSibling(Index index)
			throw();

		/**	Return a const pointer to the sibling index positions from this composite.
				A pointer to the sibling <tt>index</tt> positions to the right (for
				positive values of <tt>index</tt>) or <tt>-index</tt> positions to the left 
				(for negative values of <tt>index</tt>) is returned.
				For Index = 0 the this-pointer is returned.
				@param index the index of the sibling
				@return	a const pointer to the child or 0 if there is no such sibling.
		*/
		const Composite* getSibling(Index index) const
			throw();

		/**	Return a pointer to the first child.
				@return a pointer to the first child or 0 if there is no child.
		*/
		Composite* getFirstChild()
			throw();

		/**	Return a const pointer to the first child.
				@return a const pointer to the first child or 0 if there is no child.
		*/
		const Composite* getFirstChild() const
			throw();

		/**	Return a pointer to the last child.
				@return a  pointer to the last child or 0 if there is no child.
		*/
		Composite* getLastChild()
			throw();

		/**	Return a const pointer to the last child.
				@return a const pointer to the last child or 0 if there is no child.
		*/
		const Composite* getLastChild() const
			throw();
			
		/**	Return the time of last modification
				@return the last modification time
		*/
		const PreciseTime& getModificationTime() const 
			throw();

		/**	Return the time of last change of selection.
				@return the last time of change of selection
		*/
		const PreciseTime& getSelectionTime() const 
			throw();

		/**	Modify a time stamp.
				Update one or both of the two time stamps with the
				current time. The time stamp is then propagated up to the
				root of the composite tree. Each composite contains two stamps. 
				the \emph{modification stamp} is updated each time the tree structure
				changes, while the \emph{selection stamp} is updated each time the
				selection status changes.
				@param stamp the time stamp type 
		*/
		void stamp(StampType stamp = BOTH) 
			throw();
			
		/**	Expand a collapsed composite.
				Only expanded composite subtrees are iterated.
				@see collapse
		*/
		void expand()
			throw();

		/**	Collapse a composite.
				Collapsed composite subtrees are ignored by iterators.
				This is usefull for accelerating an iteration process.
				@see expand
		*/
		void collapse()
			throw();

		/**	Insert a composite as the first child of this composite.
				Updates the modification time stamp.
				@see stamp
				@param	composite the composite to be inserted
		*/
		void prependChild(Composite& composite)
			throw();

		/**	Insert a composite as the last child of this composite.
				Updates the modification time stamp. <b>Note</b> that this method
				alters the composite tree from which <tt>composite</tt> is taken,
				if there is such a tree.
				@see stamp
				@param	composite the composite to be inserted
		*/
		void appendChild(Composite& composite)
			throw();

		/**	Insert a new parent node.
				This method is used to combine a range of nodes into a single
				parent. First, the <tt>parent</tt> composite is <tt>destroy</tt>ed.
				Then, all nodes from <tt>first</tt> through <tt>last</tt> are inserted
				into <tt>parent</tt> and <tt>parent</tt> is inserted in the former
				position of <tt>first</tt>. The method returns <b>false</b>, if {\tt
				first} or <tt>last</tt> have differing parents, if <tt>parent</tt> is
				identical with either <tt>first</tt> or <tt>last</tt>, or if <tt>first</tt>
				is already a descendant of <tt>parent</tt>.  
				 \par
				This method updates the modification time stamp.
				@see stamp
				@param	parent the new parent of the nodes from <tt>first</tt> through
								<tt>last</tt>
				@param	first the first of the nodes to be inserted into <tt>parent</tt>
				@param	last the last of the nodes to be inserted into <tt>parent</tt>
				@param	destroy_parent keeps the current contents of <tt>parent</tt>
								if set to <tt>true</tt>
		*/
		static bool insertParent(Composite& parent, Composite& first,  
														 Composite& last, bool destroy_parent = true)
			throw();

		/**	Insert a node before this node.
				This method inserts <tt>composite</tt> before <tt>this</tt> node, if {\tt
				this} node has a parent and is not a descendant of <tt>composite</tt>.
				Self-insertion is recognized and ignored (nothing is done).
				 \par
				This method updates the modification time stamp.
				@see stamp
				@param	composite the node to be inserted in the tree before <tt>this</tt>
		*/
		void insertBefore(Composite& composite)
			throw();

		/**	Insert a node after this node.
				This method inserts <tt>composite</tt> after <tt>this</tt> node, if {\tt
				this} node has a parent and is not a descendant of <tt>composite</tt>.
				Self-insertion is recognized and ignored (nothing is done).
				 \par
				This method updates the modification time stamp.
				@see stamp
				@param	composite the node to be inserted in the tree after of <tt>this</tt>
		*/
		void insertAfter(Composite& composite)
			throw();

		/**	Prepend all children of <tt>composite</tt> to the children of this
				composite.  The method does nothing, if <tt>composite</tt> is
				identical to <tt>this</tt> or is a descendent of <tt>this</tt>.
				 \par
				This method updates the modification time stamp.
				@see stamp
				@param the composite to be spliced
		*/
		void spliceBefore(Composite& composite)
			throw();

		/**	Append all children of <tt>composite</tt> to the children of this
				composite.  The method does nothing, if <tt>composite</tt> is
				identical to <tt>this</tt> or is a descendent of <tt>this</tt>.
				 \par
				This method updates the modification time stamp.
				@see stamp
				@param composite the composite to be spliced
		*/
		void spliceAfter(Composite& composite)
			throw();

		/**	Insert the children of composite into this composite.
				The children of <tt>composite</tt> are inserted at the position of 
				<tt>composite</tt> if <tt>composite</tt> is a child of <tt>this</tt>.
				Otherwise the children are inserted using  \link spliceBefore spliceBefore \endlink .
				 \par
				This method updates the modification time stamp.
				@see stamp
				@param composite the composite to be spliced
		*/
		void splice(Composite& composite)
			throw();

		/**	Remove a child from its parent.
				<tt>child</tt> is only removed, if it is a true child of <tt>this</tt>.
				 \par
				This method updates the modification time stamp of <tt>this</tt>.
				@see stamp
				@param child the child to remove
				@return false if child could not be removed
		*/
		bool removeChild(Composite& child)
			throw();

		/** This instance and its subtree is removed form its tree and 
				replaced by <tt>composite</tt> and its subtree.
				 \par
				This method updates the modification time stamp of 
				<tt>this</tt> and <tt>composite</tt>.
				@see stamp
				@param	composite the composite which will be inserted
		*/
		void replace(Composite& composite)
			throw();

		/**	Swap the contents of two composites.
				 \par
				This  method updates the modification time stamp of <tt>this</tt> and
				<tt>composite</tt>.
				@see stamp
				@param	composite the composite with which the contents will be
								swapped
		*/
		void swap(Composite& composite)
			throw();

		/**	Select a composite.
				This method selects the composite and all the composites therein.
				 \par
				If the state of this composite is modified, its selection time
				stamp is updated and that of its ancestors (up to and including the
				root composite) as well. The time stamps of descendants that
				changed their selection state are update, too.
		*/	
		virtual void select()
			throw();

		/**	Deselect a composite.
				This method deselects the composite and all the composites therein.
				 \par
				If the state of this composite is modified, its selection time
				stamp is updated and that of its ancestors (up to and including the
				root composite) as well. The time stamps of descendants that
				changed their selection state are update, too.
		*/	
		virtual void deselect()
			throw();

		//@}
		/**	@name	Predicates */
		//@{

		/**	Equality operator.
				Compares the handles of two Composite objects, therefore two
				Composite objects can never be equal.
				@see Object::operator ==
				@param	composite the composite against which equality will be tested
		*/
		bool operator == (const Composite& composite) const
			throw();

		/**	Inequality operator.
				@see operator ==
		*/
		bool operator != (const Composite& composite) const
			throw();

		/** Test if instance is expanded.
				@see expand
		*/
		bool isExpanded() const
			throw();
		
		/** Test if instance is collapsed.
				@see collapse
		*/
		bool isCollapsed() const
			throw();
	
		/**	Return true if the node does not contain children.
				@return bool <b>true</b> if <tt>number_of_children_ == 0</tt>
		*/
		bool isEmpty() const
			throw();

		/**	Return true if the node has no parent.
				@return bool <b>true</b> if <tt>parent_ == 0</tt>
		*/
		bool isRoot() const
			throw();
	
		/** Return true if the node is root of composite.
		*/
		bool isRootOf(const Composite& composite) const
			throw();
	
		/** Return true if the node is not the root or a leaf.
		*/
		bool isInterior() const
			throw();
	
		/** Return true if the node has a child.
		*/
		bool hasChild() const
			throw();
	
		/** Return true if the node has the parent <tt>composite</tt>.
		*/
		bool isChildOf(const Composite& composite) const
			throw();
	
		/** Return true if the node is the first child of its parent.
		*/
		bool isFirstChild() const
			throw();
	
		/** Return true if the node is the first child of <tt>composite</tt>.
		*/
		bool isFirstChildOf(const Composite& composite) const
			throw();
	
		/** Return true if the node is the last child of its parent.
		*/
		bool isLastChild() const
			throw();
	
		/** Return true if the node is the last child of <tt>composite</tt>.
		*/
		bool isLastChildOf(const Composite& composite) const
			throw();
	
		/** Return true if the node has a parent.
		*/
		bool hasParent() const
			throw();

		/** Return true if the node is the parent of <tt>composite</tt>.
		*/
		bool isParentOf(const Composite& composite) const
			throw();

		/** Return true if the node has a sibling.
				(Its parent has other childs.)
		*/
		bool hasSibling() const
			throw();
			
		/** Return true if the node is a sibling of <tt>composite</tt>.
		*/
		bool isSiblingOf(const Composite& composite) const
			throw();
			
		/** Return true if the node has a previous sibling.
				(Its parent has a child before this.)
		*/
		bool hasPreviousSibling() const
			throw();
	
		/** Return true if the node is a previous sibling of <tt>composite</tt>.
		*/
		bool isPreviousSiblingOf(const Composite& composite) const
			throw();
	
		/** Return true if the node has a previous sibling.
				(Its parent has a child after this.)
		*/
		bool hasNextSibling() const
			throw();

		/** Return true if the node is a next sibling of <tt>composite</tt>.
		*/
		bool isNextSiblingOf(const Composite& composite) const
			throw();
		
		/** Return true if the node is a descendent of <tt>composite</tt>.
		*/
		bool isDescendantOf(const Composite& composite) const
			throw();

		/** Return true if the node has a ancestor of the same type as dummy.
		*/
		template <typename T>
		bool hasAncestor(const T& dummy ) const
			throw();

		/** Return true if the node has composite as descendent.
		*/
		bool isAncestorOf(const Composite& composite) const
			throw();

		/** Return true if the node has composite as ancestor or
				composite is ancestor of this node.
		*/
		bool isRelatedWith(const Composite& composite) const
			throw();
	
		/** Return true if composite is homomorph to this node.
				(The subtrees of the two instances have to be of the same form.)
		*/
		bool isHomomorph(const Composite& composite) const
			throw();

		/**	Return true if any descendant is selected.
				This method does not check all nodes recursively. Instead, on each
				modification of the tree, internal flags are updated and the
				information is propagated upwards in the tree.
				 \par
				Complexity: O(1)
				 \par
				@return bool <b>true</b> if any node in the subtree is selected
		*/
		bool containsSelection() const
			throw();
		//@}

		/**	@name	Debugging and Diagnostics */
		//@{
		/** Test if the subtree with this node as root is valid.
				(The structure of the subtree has to be valid.)
		*/
		virtual bool isValid() const
			throw();

		/** Dump the constent of this instance to an ostream.
				@param	s the stream to which we will dump
				@param	depth the indentation depth of the output
		*/
		virtual void dump(std::ostream& s = std::cout, Size depth = 0) const
			throw();

		//@}
		/**	@name	Application and Hosting */
		//@{

		/**	Visitor host method.
				Composites may be visited.
				For an example look into Composite\_test.C
				@param	visitor	the visitor
		*/
		void host(Visitor<Composite>& visitor)
			throw(Exception::GeneralException);

		/** Apply a processor to all ancestors of this node.
				@return true if the processor could be applied.
		*/
		template <typename T>
		bool applyAncestor(UnaryProcessor<T>& processor)
			throw(Exception::GeneralException);

		/** Apply a processor to all children of this node.
				@return true if the processor could be applied.
		*/
		template <typename T>
		bool applyChild(UnaryProcessor<T>& processor)
			throw(Exception::GeneralException);
		
		/** Apply a processor to all descendents of this node.
				The node itself is not processed.
				The root of a subtree is accessed before the nodes in its left 
				and right subtree.
				@return true if the processor could be applied.
		*/
		template <typename T>
		bool applyDescendantPreorder(UnaryProcessor<T>& processor)
			throw(Exception::GeneralException);

		/** Apply a processor to all descendents of this node.
				The node itself is not processed.
				The root of a subtree is accessed after the nodes in its left 
				and right subtree.
				@return true if the processor could be applied.
		*/
		template <typename T>
		bool applyDescendantPostorder(UnaryProcessor<T>& processor)
			throw(Exception::GeneralException);
	
		/** Apply a processor to all descendents of this node.
				The node itself is not processed.
				applyDescendantPreorder is used.
				@see applyDescendantPreorder
				@return true if the processor could be applied.
		*/
		template <typename T>
		bool applyDescendant(UnaryProcessor<T>& processor)
			throw(Exception::GeneralException);
		
		/** Apply a processor to the node and its subtree.
				The root of a subtree is accessed before the nodes in its left 
				and right subtree.
				@return true if the processor could be applied.
		*/
		template <typename T>
		bool applyPreorder(UnaryProcessor<T>& processor)
			throw(Exception::GeneralException);
		
		/** Apply a processor to the node and its subtree.
				The root of a subtree is accessed after the nodes in its left 
				and right subtree.
				@return true if the processor could be applied.
		*/
		template <typename T>
		bool applyPostorder(UnaryProcessor<T>& processor)
			throw(Exception::GeneralException);

		/** Apply a processor to the node and its subtree.
				applyPreorder is used.
				@see applyPreorder
				@return true if the processor could be applied.
		*/
		template <typename T>
		bool apply(UnaryProcessor<T>& processor)
			throw(Exception::GeneralException);
		
		/** Apply a processor to the node and its siblings.
				@return true if the processor could be applied.
		*/
		template <typename T>
		bool applyLevel(UnaryProcessor<T>& processor, long level)
			throw(Exception::GeneralException);
		//@}			


	
		class AncestorIteratorTraits
		{
			public:

			BALL_CREATE_DEEP(AncestorIteratorTraits)

			BALL_INLINE
			AncestorIteratorTraits()
				throw()
				:	bound_(0),
					ancestor_(0)
			{
			}
		
			BALL_INLINE
			AncestorIteratorTraits(const Composite& composite)
				throw()
				:	bound_((Composite *)&composite),
					ancestor_(0)
			{
			}
		
			BALL_INLINE
			AncestorIteratorTraits(const AncestorIteratorTraits& traits, bool /* deep */ = true)
				throw()
				:	bound_(traits.bound_),
					ancestor_(traits.ancestor_)
			{
			}
		
			BALL_INLINE
			const AncestorIteratorTraits& operator = (const AncestorIteratorTraits& traits)
				throw()
			{
				bound_ = traits.bound_;
				ancestor_ = traits.ancestor_;
				return *this;
			}

			BALL_INLINE	Composite* getContainer()	throw() { throw bound_; }

			BALL_INLINE	const Composite* getContainer() const	throw() { return bound_; }

			BALL_INLINE	bool isSingular() const	throw()	{	return (bound_ == 0);	}

			BALL_INLINE	Composite*& getPosition()	throw()	{	return ancestor_;	}

			BALL_INLINE	Composite* const& getPosition() const	throw() {	return ancestor_;	}

			BALL_INLINE	bool operator == (const AncestorIteratorTraits& traits) const	throw()	{	return (ancestor_ == traits.ancestor_);	}
		
			BALL_INLINE	bool operator != (const AncestorIteratorTraits& traits) const	throw()	{	return !(ancestor_ == traits.ancestor_); }

			BALL_INLINE	bool isValid() const	throw()	{	return (bound_ != 0 && ancestor_ != 0);	}

			BALL_INLINE	void invalidate()	throw()	{	bound_ 	= ancestor_ = 0; }
			
			BALL_INLINE	void toBegin()	throw()	{	ancestor_ = bound_->parent_; }

			BALL_INLINE	bool isBegin() const throw() { return (ancestor_ == bound_->parent_); }

			BALL_INLINE void toEnd() throw() { ancestor_ = 0;	}

			BALL_INLINE	bool isEnd() const throw() { return (ancestor_ == 0); }

			BALL_INLINE Composite& getData() throw() { return *ancestor_;	}

			BALL_INLINE	const Composite& getData() const throw() { return *ancestor_; }

			BALL_INLINE void forward() throw() { ancestor_ = ancestor_->parent_; }

			private:

			Composite* bound_;
			Composite* ancestor_;
		};

		friend class AncestorIteratorTraits;

		typedef ForwardIterator <Composite, Composite, Composite *, AncestorIteratorTraits>
			AncestorIterator;

		AncestorIterator beginAncestor() throw()
		{
			return AncestorIterator::begin(*this);
		}

		AncestorIterator endAncestor() throw()
		{
			return AncestorIterator::end(*this);
		}

		typedef ConstForwardIterator<Composite, Composite, Composite *, AncestorIteratorTraits>
			AncestorConstIterator;

		AncestorConstIterator beginAncestor() const
			throw()
		{
			return AncestorConstIterator::begin(*this);
		}

		AncestorConstIterator endAncestor() const
			throw()
		{
			return AncestorConstIterator::end(*this);
		}



		class ChildCompositeIteratorTraits
		{
			public:

			ChildCompositeIteratorTraits()
				throw()
				:	bound_(0),
					child_(0)
			{
			}
			
			ChildCompositeIteratorTraits(const Composite& composite)
				throw()
				:	bound_((Composite *)&composite),
					child_(0)
			{
			}
		
			ChildCompositeIteratorTraits(const ChildCompositeIteratorTraits& traits)
				throw()
				:	bound_(traits.bound_),
					child_(traits.child_)
			{
			}
		
			const ChildCompositeIteratorTraits& operator = (const ChildCompositeIteratorTraits& traits)
				throw()
			{
				bound_ = traits.bound_;
				child_ = traits.child_;
				return *this;
			}

			BALL_INLINE Composite* getContainer()	throw() {	return bound_; }

			BALL_INLINE const Composite* getContainer() const throw()	{	return bound_; }

			BALL_INLINE bool isSingular() const throw()	{ return (bound_ == 0);	}

			BALL_INLINE Composite*& getPosition() throw()	{ return child_; }

			BALL_INLINE Composite* const& getPosition() const	throw()	{ return child_; }

			BALL_INLINE bool operator == (const ChildCompositeIteratorTraits& traits) const throw() { return (child_ == traits.child_); }
		
			BALL_INLINE bool operator != (const ChildCompositeIteratorTraits& traits) const throw() { return !(child_ == traits.child_); }
		
			BALL_INLINE bool isValid() const throw() { return (bound_ != 0 && child_ != 0); }

			BALL_INLINE void invalidate() throw() { bound_ = child_ = 0; }

			BALL_INLINE void toBegin() throw() { child_ = bound_->first_child_; }

			BALL_INLINE bool isBegin() const throw() { return (child_ == bound_->first_child_); }

			BALL_INLINE void toEnd() throw() { child_ = 0; }

			BALL_INLINE bool isEnd() const throw() { return (child_ == 0); }

			BALL_INLINE void toRBegin() throw() { child_ = bound_->last_child_; }

			BALL_INLINE bool isRBegin() const throw() { return (child_ == bound_->last_child_); }

			BALL_INLINE void toREnd() throw() { child_ = 0; }

			BALL_INLINE bool isREnd() const throw() { return (child_ == 0); }

			BALL_INLINE Composite& getData() throw() { return *child_; }

			BALL_INLINE const Composite& getData() const throw() { return *child_; }

			BALL_INLINE void forward() throw() {	child_ = child_->next_;	}

			BALL_INLINE void backward()	throw()	{	child_ = child_->previous_;	}

			private:

			Composite* bound_;
			Composite* child_;
		};

		friend class ChildCompositeIteratorTraits;

		typedef BidirectionalIterator<Composite, Composite, Composite *, ChildCompositeIteratorTraits>
			ChildCompositeIterator;

		ChildCompositeIterator beginChildComposite()
			throw()
		{
			return ChildCompositeIterator::begin(*this);
		}

		ChildCompositeIterator endChildComposite()
			throw()
		{
			return ChildCompositeIterator::end(*this);
		}



		typedef ConstBidirectionalIterator<Composite, Composite, Composite *, ChildCompositeIteratorTraits>
			ChildCompositeConstIterator;

		ChildCompositeConstIterator beginChildComposite() const
			throw()
		{
			return ChildCompositeConstIterator::begin(*this);
		}

		ChildCompositeConstIterator endChildComposite() const
			throw()
		{
			return ChildCompositeConstIterator::end(*this);
		}



		typedef std::reverse_iterator<ChildCompositeIterator> ChildCompositeReverseIterator;

		ChildCompositeReverseIterator rbeginChildComposite() throw()
		{
			return ChildCompositeReverseIterator(endChildComposite());
		}

		ChildCompositeReverseIterator rendChildComposite() throw()
		{
			return ChildCompositeReverseIterator(beginChildComposite());
		}



		typedef std::reverse_iterator<ChildCompositeConstIterator> ChildCompositeConstReverseIterator;

		ChildCompositeConstReverseIterator rbeginChildComposite() const throw()
		{
			return ChildCompositeConstReverseIterator(endChildComposite());
		}

		ChildCompositeConstReverseIterator rendChildComposite() const throw()
		{
			return ChildCompositeConstReverseIterator(beginChildComposite());
		}

		class CompositeIteratorPosition
		{
			friend class Composite;

			public:

			CompositeIteratorPosition()
				throw()
				:	empty_stack_(0),
					stack_(0),
					current_(0),
					continue_(false),
					traversing_forward_(true)
			{
			}

			CompositeIteratorPosition(const CompositeIteratorPosition& position)
				throw()
				:	empty_stack_(position.empty_stack_),
					stack_(position.stack_),
					current_(position.current_),
					continue_(position.continue_),
					traversing_forward_(position.traversing_forward_)
			{
			}

			Composite* getCurrent()
				throw()
			{
				return current_;
			}
		
			const Composite* getCurrent() const
				throw()
			{
				return current_;
			}

			void clear()
				throw()
			{
				empty_stack_ = stack_ = current_ = 0;
	
				continue_ = false;
				traversing_forward_ = true;
			}

			const CompositeIteratorPosition& operator = (const CompositeIteratorPosition& position)
				throw()
			{
				if (this != &position)
				{
					empty_stack_ = position.empty_stack_;
					stack_ = position.stack_;
					current_ = position.current_;
					continue_ = position.continue_;
					traversing_forward_ = position.traversing_forward_;
				}
			
				return *this;
			}

			bool operator == (const Composite::CompositeIteratorPosition& position) const
				throw()
			{
				return (current_ == position.current_);
			}

			bool operator != (const Composite::CompositeIteratorPosition& position) const
				throw()
			{
				return (current_ != position.current_);
			}

			private:

			Composite* empty_stack_;
			Composite* stack_;
			Composite* current_;
			bool continue_;
			bool traversing_forward_;
		};



		class CompositeIteratorTraits
		{
			public:

			CompositeIteratorTraits()
				throw()
				:	bound_(0),
					position_()
			{
			}
		
			CompositeIteratorTraits(const Composite& composite)
				throw()
				:	bound_((Composite*)&composite),
					position_()
			{
			}
		
			CompositeIteratorTraits(const CompositeIteratorTraits& traits, bool /* deep */ = true)
				throw()
				:	bound_(traits.bound_),
					position_(traits.position_)
			{
			}
		
			BALL_INLINE 
			bool isValid() const throw()
			{
				return ((bound_ != 0)	&& (position_.getCurrent() != 0));
			}

			BALL_INLINE 
			const CompositeIteratorTraits& operator = (const CompositeIteratorTraits& traits) throw()
			{
				bound_ = traits.bound_;
				position_ = traits.position_;
				return *this;
			}

			BALL_INLINE Composite* getContainer()	throw()	{	return bound_; }

			BALL_INLINE const Composite* getContainer() const	throw() { return bound_; }
		
			BALL_INLINE bool isSingular() const	throw()	{	return (bound_ == 0);	}
		
			BALL_INLINE CompositeIteratorPosition& getPosition() throw() { return position_;	}

			BALL_INLINE const CompositeIteratorPosition& getPosition() const throw() { return position_; }

			BALL_INLINE Composite& getData() throw() { return *(position_.getCurrent()); }

			BALL_INLINE const Composite& getData() const throw() { return *(position_.getCurrent()); }

			BALL_INLINE bool operator == (const CompositeIteratorTraits& traits) const throw() { return (position_	== traits.position_); }
		
			BALL_INLINE bool operator != (const CompositeIteratorTraits& traits) const throw() { return (position_ != traits.position_); }
		
			BALL_INLINE void invalidate() throw()	
			{ 
				bound_ = 0;	
				position_.clear();
			}

			BALL_INLINE void toBegin() throw()
			{
				Composite::setCurrentPreorderIteratorPosition
					(Composite::getFirstPreorderIteratorPosition(*bound_), position_, false);
			}

			BALL_INLINE bool isBegin() const throw()
			{
				return (position_.getCurrent() == &Composite::getFirstPreorderIteratorPosition(*bound_));
			}

			BALL_INLINE void toEnd() throw() 
			{
				toRBegin();
				forward();
			}

			BALL_INLINE bool isEnd() const throw()
			{
				return (position_.getCurrent() == 0);
			}

			BALL_INLINE void toRBegin() throw()
			{
				Composite::setLastPreorderIteratorPosition(*bound_, position_,false);
			}

			BALL_INLINE bool isRBegin() const throw()
			{
				return (position_.getCurrent() == &Composite::getLastPreorderIteratorPosition(*bound_)); 
			}
		
			BALL_INLINE void toREnd() throw()
			{
				Composite::setCurrentPreorderIteratorPosition(*bound_, position_, false);
				backward();	
			}

			BALL_INLINE bool isREnd() const throw()
			{
				return (position_.getCurrent() == 0);
			}
		
			BALL_INLINE void forward() throw()
			{
				bound_->getNextPreorderIteratorPosition(position_);
			}

			BALL_INLINE void backward()	throw()
			{
				if (!isEnd())
				{
					bound_->getPreviousPreorderIteratorPosition(position_);
				}
				else
				{
					Composite::setLastPreorderIteratorPosition(*bound_, position_, false);
				}
			}

			protected:

			/// A pointer to the "container" the iterator is bound to
			Composite* bound_;

			/// The current iterator position
			CompositeIteratorPosition	position_;
		};

		friend class CompositeIteratorTraits;

		typedef BidirectionalIterator<Composite, Composite, CompositeIteratorPosition, CompositeIteratorTraits>
			CompositeIterator;

		CompositeIterator beginComposite() throw() { return CompositeIterator::begin(*this); }

		CompositeIterator endComposite() throw() { return CompositeIterator::end(*this); }

		typedef ConstBidirectionalIterator<Composite, Composite, CompositeIteratorPosition, CompositeIteratorTraits>
			CompositeConstIterator;

		CompositeConstIterator beginComposite() const throw() 
		{	
			return CompositeConstIterator::begin(*this);
		}

		CompositeConstIterator endComposite() const	throw()
		{
			return CompositeConstIterator::end(*this);
		}


		typedef std::reverse_iterator<CompositeIterator> CompositeReverseIterator;

		CompositeReverseIterator rbeginComposite() throw()
		{
			return CompositeReverseIterator(endComposite());
		}

		CompositeReverseIterator rendComposite() throw()
		{
			return CompositeReverseIterator(beginComposite());
		}


		typedef std::reverse_iterator<CompositeConstIterator> CompositeConstReverseIterator;

		CompositeConstReverseIterator rbeginComposite() const throw()
		{
			return CompositeConstReverseIterator(endComposite());
		}

		CompositeConstReverseIterator rendComposite() const throw()
		{
			return CompositeConstReverseIterator(beginComposite());
		}



		class SubcompositeIteratorTraits
			: public CompositeIteratorTraits
		{
			public:

			BALL_INLINE SubcompositeIteratorTraits() throw()
				:	CompositeIteratorTraits()
			{
			}
		
			SubcompositeIteratorTraits(const Composite& composite) throw()
				:	CompositeIteratorTraits(composite)
			{
			}
		
			BALL_INLINE SubcompositeIteratorTraits(const SubcompositeIteratorTraits& traits)
				throw()
				:	CompositeIteratorTraits(traits)
			{
			}

			BALL_INLINE void toBegin() throw()
			{
				Composite::setCurrentPreorderIteratorPosition(Composite::getFirstPreorderIteratorPosition(*bound_), position_, true);
			}

			BALL_INLINE void toEnd() throw()
			{
				toRBegin();
				forward();
			}

			BALL_INLINE void toRBegin()	throw()
			{
				Composite::setLastPreorderIteratorPosition(*bound_, position_, true);
			}

			BALL_INLINE void toREnd()	throw()
			{
				Composite::setCurrentPreorderIteratorPosition(*bound_, position_, true);
				backward();
			}
		};

		friend class SubcompositeIteratorTraits;

		typedef BidirectionalIterator<Composite, Composite, CompositeIteratorPosition, SubcompositeIteratorTraits>
			SubcompositeIterator;

		SubcompositeIterator beginSubcomposite() throw()
		{
			return SubcompositeIterator::begin(*this);
		}

		SubcompositeIterator endSubcomposite() throw()
		{
			return SubcompositeIterator::end(*this);
		}

		typedef ConstBidirectionalIterator<Composite, Composite, CompositeIteratorPosition, SubcompositeIteratorTraits>
			SubcompositeConstIterator;

		SubcompositeConstIterator beginSubcomposite() const throw()
		{
			return SubcompositeConstIterator::begin(*this);
		}

		SubcompositeConstIterator endSubcomposite() const throw()
		{
			return SubcompositeConstIterator::end(*this);
		}



		typedef std::reverse_iterator<SubcompositeIterator>	SubcompositeReverseIterator;

		SubcompositeReverseIterator rbeginSubcomposite() throw()
		{
			return SubcompositeReverseIterator(endSubcomposite());
		}
			

		SubcompositeReverseIterator rendSubcomposite() throw()
		{
			return SubcompositeReverseIterator(beginSubcomposite());
		}



		typedef std::reverse_iterator<SubcompositeConstIterator> SubcompositeConstReverseIterator;

		SubcompositeConstReverseIterator rbeginSubcomposite() const throw()
		{
			return SubcompositeConstReverseIterator(endSubcomposite());
		}

		SubcompositeConstReverseIterator rendSubcomposite() const throw()
		{
			return SubcompositeConstReverseIterator(beginSubcomposite());
		}

		private:
		
		enum Property_
		{
			PROPERTY__COLLAPSED_ = 1
		};

		Size getHeight_(Size size, Size& max_height) const
			throw();
	
		Size countDescendants_() const throw();

		void clone_(Composite& parent, Composite& stack) const throw();

		// traverse forward, valid for composites and subcomposites
		static Composite* setCurrentPreorderIteratorPosition
			(Composite& composite, CompositeIteratorPosition& position, bool subcomposite)
				throw();
		
		// get first iterator, valid for composites and subcomposites
		static Composite& getFirstPreorderIteratorPosition(Composite& composite)
			throw();
	
		// get last iterator, valid for composites only
		static Composite& getLastPreorderIteratorPosition(Composite& composite)
			throw();
		
		// set last iterator, valid for composites and subcomposites
		static Composite& setLastPreorderIteratorPosition
			(Composite& composite, CompositeIteratorPosition& position, bool subcomposite)
				throw();
		
		// get next iterator, valid for composites and subcomposites
		Composite* getNextPreorderIteratorPosition(CompositeIteratorPosition& position)	throw();

		// get previous iterator, valid for composites and subcomposites
		Composite* getPreviousPreorderIteratorPosition(CompositeIteratorPosition& position)	throw();

		static Composite* setCurrentPreorderForward_
			(Composite& composite, CompositeIteratorPosition& position, bool subcomposite)	throw();

		static Composite* setCurrentPreorderBackward_
			(Composite& composite, CompositeIteratorPosition& position, bool subcomposite) throw();

		template <typename T>
		bool applyLevelNostart_(UnaryProcessor<T>& processor, long level)
			throw(Exception::GeneralException);

		template <typename T>
		bool applyChildNostart_(UnaryProcessor<T>& processor)
			throw(Exception::GeneralException);

		template <typename T>
		bool applyPreorderNostart_(UnaryProcessor<T>& processor)
			throw(Exception::GeneralException);

		template <typename T>
		bool applyDescendantPreorderNostart_(UnaryProcessor<T>& processor)
			throw(Exception::GeneralException);

		template <typename T>
		bool applyDescendantPostorderNostart_(UnaryProcessor<T>& processor)
			throw(Exception::GeneralException);


		void updateSelection_() throw();
		void determineSelection_() throw();
		void select_(bool update_parent = true) throw();
		void deselect_(bool update_parent = true) throw();

		// private attributes
		
		Size 						number_of_children_;
		Composite*			parent_;
		Composite* 			previous_;
		Composite* 			next_;
		Composite* 			first_child_;
		Composite* 			last_child_;
		unsigned char		properties_;
		bool						contains_selection_;
		Size						number_of_selected_children_;
		Size						number_of_children_containing_selection_;
		TimeStamp				selection_stamp_;
		TimeStamp				modification_stamp_;
	};

	template <typename T>
	bool Composite::applyAncestor(UnaryProcessor<T>& processor)
		throw(Exception::GeneralException)
	{
		if (processor.start() == false)
		{
			return false;
		}

		Processor::Result result;

		for (Composite* composite = parent_;
				 composite != 0; composite = composite->parent_)
		{
			T* t_ptr;
			if ((t_ptr = dynamic_cast<T*>(composite)) != 0)
			{	
				result = processor(*t_ptr);
				if (result <= Processor::BREAK)
				{
					return (result == Processor::BREAK);
				}
			}
		}

		return processor.finish();
	}
	
	template <typename T>
	bool Composite::applyChild(UnaryProcessor<T>& processor)
		throw(Exception::GeneralException)
	{
		return processor.start() && applyChildNostart_(processor) && processor.finish();
	}

	template <typename T>
	bool Composite::applyChildNostart_(UnaryProcessor<T>& processor)
		throw(Exception::GeneralException)
	{
		Processor::Result result = Processor::CONTINUE;

		if (!isCollapsed())
		{
			for (Composite* composite = first_child_;
					 composite != 0; composite = composite->next_)
			{
				T* t_ptr;
				if ((t_ptr = dynamic_cast<T*>(composite)) != 0)
				{
					result = processor(*t_ptr);
					if (result <= Processor::BREAK)
					{
						break;
					}
				}
			}
		}

		return (result >= Processor::BREAK);
	}
 
	template <typename T>
	bool Composite::applyDescendantPreorder(UnaryProcessor<T>& processor)
		throw(Exception::GeneralException)
	{
		return processor.start() && applyDescendantPreorderNostart_(processor) && processor.finish();
	}

	template <typename T>
	bool Composite::applyDescendantPreorderNostart_(UnaryProcessor<T>& processor)
		throw(Exception::GeneralException)
	{
		if (isCollapsed() == true)
		{
			return true;
		}

		Processor::Result result;

		for (Composite* composite = first_child_;
				 composite != 0; composite = composite->next_)
		{
			T* t_ptr;
			if ((t_ptr = dynamic_cast<T*>(composite)) != 0)
			{	
				result = processor(*t_ptr);

				if (result <= Processor::BREAK)
				{
					return (result == Processor::BREAK);
				}
			}

			if (composite->first_child_ != 0  && composite->applyDescendantPreorderNostart_(processor) == false)
			{
				return false;
			}
		}

		return true;
	}

	template <typename T>
	bool Composite::applyDescendantPostorder(UnaryProcessor<T>& processor)
		throw(Exception::GeneralException)
	{
		return processor.start() && applyDescendantPostorderNostart_(processor) && processor.finish();
	}

	template <typename T>
	bool Composite::applyDescendantPostorderNostart_(UnaryProcessor<T>& processor)
		throw(Exception::GeneralException)
	{
		if (isCollapsed() == true)
		{
			return true;
		}

		Processor::Result result;

		for (Composite* composite = first_child_;
				 composite != 0; composite = composite->next_)
		{
			if (composite->first_child_ != 0 && 
					composite->applyDescendantPostorderNostart_(processor) == false)
			{
				return false;
			}

			T* t_ptr = dynamic_cast<T*>(composite);
			if (t_ptr != 0)
			{
				result = processor(*t_ptr);

				if (result <= Processor::BREAK)
				{
					return (result == Processor::BREAK);
				}
			}
		}

		return true;
	}

	template <typename T>  
	bool Composite::applyPostorder(UnaryProcessor<T>& processor)
		throw(Exception::GeneralException)
	{ 
		if (!processor.start() || !applyDescendantPostorderNostart_(processor))
		{
			return false;
		}

		T* t_ptr = dynamic_cast<T*>(this);

		return (t_ptr != 0													  && 
						processor(*t_ptr) >= Processor::BREAK && 
						processor.finish()											);
	}

	template <typename T>
	bool Composite::applyLevel(UnaryProcessor<T>& processor, long level)
		throw(Exception::GeneralException)
	{
		return processor.start() && applyLevelNostart_(processor, level) && processor.finish();
	}

	template <typename T>
	bool Composite::applyLevelNostart_(UnaryProcessor<T>& processor, long level)
		throw(Exception::GeneralException)
	{
		if (level == 0)
		{
			T* t_ptr = dynamic_cast<T*>(this);
			if (t_ptr != 0)
			{
			 Processor::Result result = processor(*t_ptr);

				if (result <= Processor::BREAK)
				{
					return (result == Processor::BREAK);
				}
			}
		}
		else 
		{
			if (--level == 0)
			{
				return applyChildNostart_(processor);
			}
			else 
			{
				if (level > 0)
				{
					for (Composite* composite = first_child_;
							 composite != 0; composite = composite->next_)
					{
						if (composite->first_child_ != 0 && composite->applyLevelNostart_(processor, level) == false)
						{
							return false;
						}
					}
				}
			}
		}
		return true;
	}

	template <typename T>
	bool Composite::applyPreorderNostart_(UnaryProcessor<T>& processor)
		throw(Exception::GeneralException)
	{
		Processor::Result result;
		bool return_value;
		T* t_ptr = dynamic_cast<T*>(this);
		if (t_ptr != 0)
		{
			result = processor(*t_ptr);
	
			if (result <= Processor::BREAK)
			{
				return_value = (result == Processor::BREAK);
			} 
			else 
			{
				return_value =  applyDescendantPreorderNostart_(processor);
			}
		} 
		else 
		{
			return_value =  applyDescendantPreorderNostart_(processor);
		}
		
		return return_value;
	}

	template <typename T>
	bool Composite::applyDescendant(UnaryProcessor<T>& processor)
		throw(Exception::GeneralException)
	{
		return applyDescendantPreorder(processor);
	}

	template <typename T>
	bool Composite::applyPreorder(UnaryProcessor<T>& processor)
		throw(Exception::GeneralException)
	{
		return processor.start() && applyPreorderNostart_(processor) && processor.finish();
	}

	template <typename T>
	BALL_INLINE 
	bool Composite::apply(UnaryProcessor<T>& processor)
		throw(Exception::GeneralException)
	{
		return applyPreorder(processor);
	}

	template <typename T>
	BALL_INLINE 
	T* Composite::getAncestor(const T& /* dummy */)
		throw()
	{
		T* T_ptr = 0;
		
		for (Composite* composite_ptr = parent_;
				 composite_ptr != 0; composite_ptr = composite_ptr->parent_)
		{
			T_ptr = dynamic_cast<T*>(composite_ptr);
			if (T_ptr != 0)
			{
				break;
			}	
		}
		
		return T_ptr;
	}

	template <class T>
	BALL_INLINE 
	const T* Composite::getAncestor(const T& /* dummy */) const
		throw()
	{
		T* t_ptr = 0;
		for (Composite* composite_ptr = parent_;
				 composite_ptr != 0; composite_ptr = composite_ptr->parent_)
		{
			if ((t_ptr = dynamic_cast<T*>(composite_ptr)) != 0)
			{
				break;
			}	
		}
		
		return const_cast<const T*>(t_ptr);
	}

	template <typename T>
	BALL_INLINE 
	T* Composite::getPrevious(const T& /* dummy */)
		throw()
	{
		// create an iterator bound to the root of the subtree
		SubcompositeIterator it(getRoot().endSubcomposite());

		// set its position to the current composite
		Composite::setCurrentPreorderIteratorPosition
			(Composite::getFirstPreorderIteratorPosition(*this), it.getTraits().getPosition(), false);


		// walk back until we find something	
		// or we cannot walk any further
		if (+it)
		{
			do 
			{
				--it;
			} 
			while (+it && !RTTI::isKindOf<T>(*it));
		}

		// return a NULL pointer if nothing was found
		Composite* ptr = 0;
		if (+it)
		{
			ptr = &*it;
		}
		
		return dynamic_cast<T*>(ptr);
	}

	template <class T>
	BALL_INLINE 
	const T* Composite::getPrevious(const T& dummy) const
		throw()
	{
		// cast away the constness of this and call the non-const method
		Composite* nonconst_this = const_cast<Composite*>(this);

		return const_cast<const T*>(nonconst_this->getPrevious(dummy));
	}

	template <typename T>
	BALL_INLINE 
	T* Composite::getNext(const T& /* dummy */)
		throw()
	{
		// create an iterator bound to the root of the subtree
		SubcompositeIterator it(getRoot().beginSubcomposite());

		// set its position to the current composite
		Composite::setCurrentPreorderIteratorPosition
			(Composite::getFirstPreorderIteratorPosition(*this), it.getTraits().getPosition(), false);

		// walk forward until we find something	
		// or we cannot walk any further
		do 
		{
			it++;
		} 
		while (it.isValid() && !RTTI::isKindOf<T>(*it));


		// return a NULL pointer if nothing was found
		Composite* ptr = 0;
		if (+it)
		{
			ptr = &*it;
		}
		
		return dynamic_cast<T*>(ptr);
	}

	template <class T>
	BALL_INLINE 
	const T* Composite::getNext(const T& dummy) const
		throw()
	{
		// cast away the constness of this and call the non-const method
		Composite* nonconst_this = const_cast<Composite*>(this);

		return const_cast<const T*>(nonconst_this->getNext(dummy));
	}

	template <typename T>
	BALL_INLINE 
	bool Composite::hasAncestor(const T& dummy ) const 
		throw()
	{
		return (getAncestor(dummy) != 0);	
	}

#	ifndef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/CONCEPT/composite.iC>
#	endif


} // namespace BALL

#endif // BALL_CONCEPT_COMPOSITE_H
