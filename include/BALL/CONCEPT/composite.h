// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
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
	class Atom;

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
			be accessed via  \link getModificationTime getModificationTime \endlink  
			and  \link getSelectionTime getSelectionTime \endlink .
			Selecting or deselecting a Composite automatically selects or
			deselects all its children (recursively!).  Selecting or deselecting
			all children of a node deselects their parent as well.	Selection
			information is propagated upwards in the tree.
			 \par
			Composites are persistent objects. 
			 \par
			
			\ingroup ConceptsMiscellaneous 
	*/
	class BALL_EXPORT Composite
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
			;

		/**	Copy constructor.
				Creates a copy of a composite. <b>  Deep </b> copies include the whole
				composite tree, <b>  shallow </b> copies contain anly a single composite.
				@param	composite the composite to be cloned (the root of the tree in 
								the case of a deep copy)
				@param	deep make a deep copy (<b>true</b>) or shallow copy 
								(<b>false</b>)
		*/
		Composite(const Composite& composite, bool deep = true)
			;

		/**	Destructor.	
				The destructor calls  \link destroy destroy \endlink  to remove the composite from 
				potential tree structures. It also recursively destructs all
				children of the composite.
		*/
		virtual ~Composite() 
			;

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
			;
	
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
			;

		/**	Non-virtual destroy method.
				This method behaves exactly like destroy except for a small
				difference: when called with <b>true</b>, it calls the <b>  virtual </b>
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
			;

		/**	Clone with a predicate.
				This method copies the attributes of <tt>this</tt> composite to root
				(shallow copy) and then adds recursively each of its children.
				@param	root the cloning target root is <tt>destroy</tt>ed prior to
								any copying 
				@return  a pointer to the root composite (<tt>&root</tt>)
		*/
		void* clone(Composite& root) const
			;

		//@}		

		/**	@name	Persistence 
		*/
		//@{
		
		/** Write a persistent copy of the object.
				@param	pm the persistence manager
				@param	name the object name
				\throws Exception::GeneralException
		*/
		virtual void persistentWrite(PersistenceManager& pm, const char* name = 0) const;

		/** Read a persistent object.
				@param	pm the persistence manager
				\throws Exception::GeneralException
		*/
		virtual void persistentRead(PersistenceManager& pm);

		//@}

		/**	@name	Modifying and Accessing the Tree 
		*/
		//@{

		/**	Assignment.
				@param	composite the Composite tree to assign from
				@param	deep a <tt>bool</tt> deciding whether the assignment will be
								deep or shallow.
		*/
		void set(const Composite& composite, bool deep = true) ;

		/**	Assignment operator.
				@param	composite the Composite tree to assign from
				@return	a const reference to <b>this</b>
		*/
		Composite& operator = (const Composite& composite) ;

		/**	Assignment of a tree to another.
				Create a deep (<tt>deep</tt> = <b>true</b>) or shallow copy of a composite
				and assign it to <tt>composite</tt>. <tt>composite</tt> is destroyed first.
				@param	composite the composite to assign the copy to
				@param	deep <b>true</b> for a deep copy
		*/
		void get(Composite& composite, bool deep = true) const ;

		/**	Return the degree of the node.
				This method returns the number of children of a composite object.
				@return Size the number of children
		*/
		Size getDegree() const ;

		/**	Count the number of nodes fulfilling a predicate in this subtree.
				@param	predicate the predicate
				@return Size the number of nodes in the subtree satisfying the predicate
		*/
		Size count(const KernelPredicateType& predicate) const ;

		/**	Count the number of descendants.
				@return Size the number of descendants of this node
		*/
		Size countDescendants() const ;

		/** Get the length of the path between two composite objects.
				If no path exists <tt>INVALID_SIZE</tt> is returned.
				@param composite the second object
				@return Size the size of the path
		*/
		Size getPathLength(const Composite& composite) const ;

		/** Get the depth of this item in its tree.
				The depth of a root item is 0.
				@return Size the depth
		*/
		Size getDepth() const ;

		/** Get the height of this item in its tree.
				The hight of a leaf is 0.
				@return Size the height
		*/
		Size getHeight() const
			;

		/** Get the root of this item.
				@return Composite& the root
		*/
		Composite& getRoot() ;

		/** Get a const reference to the root of this item.
				@return Composite& the root
		*/
		const Composite& getRoot() const ;

		/** Get the lowest common ancestor of this item with an other.
				If no common ancestor exists 0 is returned.
				@return Composite& the lowest common ancestor
		*/
		Composite* getLowestCommonAncestor(const Composite& composite)
			;

		/** Get a const reference to the lowest common ancestor of this item
				with an other. If no common ancestor exists, 0 is returned.
				@return Composite& the lowest common ancestor
		*/
		const Composite* getLowestCommonAncestor(const Composite& composite) const
			;

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
			;

		/**	Find the first ancestor of type T (const method).
				This method operates also on constant trees.
				@return a pointer to the first composite found that is a kind of T
								or 0 if no matching composite was found to the root of the
								tree
		*/
		template <typename T>
		const T* getAncestor(const T& /* dummy */) const ;

		/**	Find the nearest previous composite of type T.
				This method walks backward in the tree from composite to composite and
				checks whether the composite object is a kind of <tt>T</tt>.
				@return a pointer to the first composite found that is a kind of T
								or 0 if no matching composite was found up to the root of
								the tree
		*/
		template <typename T>
		T* getPrevious(const T& /* dummy */) ;

		/**	Find the nearest previous composite of type T (const method).
				This method walks backward in the tree from composite to composite and
				checks whether the composite object is a kind of <tt>T</tt>.
				@return a pointer to the first composite found that is a kind of T
								or 0 if no matching composite was found up to the root of
								the tree
		*/
		template <typename T>
		const T* getPrevious(const T& dummy) const ;

		/**	Find the next composite of type T.
				This method walks backward in the tree from composite to composite and
				checks whether the composite object is a kind of <tt>T</tt>.
				@return a pointer to the first composite found that is a kind of T
								or 0 if no matching composite was found up to the root of
								the tree
		*/
		template <typename T>
		T* getNext(const T& /* dummy */) ;

		/**	Find the next composite of type T (const method).
				This method walks backward in the tree from composite to composite and
				checks whether the composite object is a kind of <tt>T</tt>.
				@return a pointer to the first composite found that is a kind of T
								or 0 if no matching composite was found up to the root of
								the tree
		*/
		template <typename T>
		const T* getNext(const T& dummy) const ;

		/**	Return the composite's parent.
				@return a pointer to the parent composite or 0 if no parent exists
		*/
		Composite* getParent() ;

		/**	Return the composite's parent (const method).
				@return a pointer to the parent composite or 0 if no parent exists
		*/
		const Composite* getParent() const ;

		/**	Return the <b>  index </b>th child of this composite.
				If no such child exists, 0 is returned.
				The index of the first child is <b>0</b>.
				@param	index the index of the child to return
				@return	a pointer to the child or 0 if there is no such child.
		*/
		Composite* getChild(Index index) ;
	
		/**	Return a const pointer to the <b>  index </b>th child of this composite.
				If no such child exists, 0 is returned.
				The index of the first child is <b>0</b>.
				@param	index the index of the child to return
				@return	a const pointer to the child or 0 if there is no such child.
		*/
		const Composite* getChild(Index index) const ;
	
		/**	Return a pointer to the sibling index positions from this composite.
				A pointer to the sibling <tt>index</tt> positions to the right (for
				positive values of <tt>index</tt>) or <tt>-index</tt> positions to the left 
				(for negative values of <tt>index</tt>) is returned.
				For Index = 0 the this-pointer is returned.
				@param index the index of the sibling
				@return	a pointer to the child or 0 if there is no such sibling.
		*/
		Composite* getSibling(Index index) ;

		/**	Return a const pointer to the sibling index positions from this composite.
				A pointer to the sibling <tt>index</tt> positions to the right (for
				positive values of <tt>index</tt>) or <tt>-index</tt> positions to the left 
				(for negative values of <tt>index</tt>) is returned.
				For Index = 0 the this-pointer is returned.
				@param index the index of the sibling
				@return	a const pointer to the child or 0 if there is no such sibling.
		*/
		const Composite* getSibling(Index index) const ;

		/**	Return a pointer to the first child.
				@return a pointer to the first child or 0 if there is no child.
		*/
		Composite* getFirstChild() ;

		/**	Return a const pointer to the first child.
				@return a const pointer to the first child or 0 if there is no child.
		*/
		const Composite* getFirstChild() const ;

		/**	Return a pointer to the last child.
				@return a  pointer to the last child or 0 if there is no child.
		*/
		Composite* getLastChild() ;

		/**	Return a const pointer to the last child.
				@return a const pointer to the last child or 0 if there is no child.
		*/
		const Composite* getLastChild() const ;
			
		/**	Return the time of last modification
				@return the last modification time
		*/
		const PreciseTime& getModificationTime() const ;

		/**	Return the time of last change of selection.
				@return the last time of change of selection
		*/
		const PreciseTime& getSelectionTime() const ;

		/**	Modify a time stamp.
				Update one or both of the two time stamps with the
				current time. The time stamp is then propagated up to the
				root of the composite tree. Each composite contains two stamps. 
				the <em>modification stamp</em> is updated each time the tree structure
				changes, while the <em>selection stamp</em> is updated each time the
				selection status changes.
				@param stamp the time stamp type 
		*/
		void stamp(StampType stamp = BOTH) ;
			
		/**	Insert a composite as the first child of this composite.
				Updates the modification time stamp.
				@see stamp
				@param	composite the composite to be inserted
		*/
		void prependChild(Composite& composite) ;

		/**	Insert a composite as the last child of this composite.
				Updates the modification time stamp. <b>Note</b> that this method
				alters the composite tree from which <tt>composite</tt> is taken,
				if there is such a tree.
				@see stamp
				@param	composite the composite to be inserted
		*/
		void appendChild(Composite& composite) ;

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
			;

		/**	Insert a node before this node.
				This method inserts <tt>composite</tt> before <tt>this</tt> node, if
				<tt>this</tt> node has a parent and is not a descendant of <tt>composite</tt>.
				Self-insertion is recognized and ignored (nothing is done).
				 \par
				This method updates the modification time stamp.
				@see stamp
				@param	composite the node to be inserted in the tree before <tt>this</tt>
		*/
		void insertBefore(Composite& composite) ;

		/**	Insert a node after this node.
				This method inserts <tt>composite</tt> after <tt>this</tt> node, if 
				<tt>this</tt> node has a parent and is not a descendant of <tt>composite</tt>.
				Self-insertion is recognized and ignored (nothing is done).
				 \par
				This method updates the modification time stamp.
				@see stamp
				@param	composite the node to be inserted in the tree after of <tt>this</tt>
		*/
		void insertAfter(Composite& composite) ;

		/**	Prepend all children of <tt>composite</tt> to the children of this
				composite.  The method does nothing, if <tt>composite</tt> is
				identical to <tt>this</tt> or is a descendent of <tt>this</tt>.
				 \par
				This method updates the modification time stamp.
				@see stamp
				@param the composite to be spliced
		*/
		void spliceBefore(Composite& composite) ;

		/**	Append all children of <tt>composite</tt> to the children of this
				composite.  The method does nothing, if <tt>composite</tt> is
				identical to <tt>this</tt> or is a descendent of <tt>this</tt>.
				 \par
				This method updates the modification time stamp.
				@see stamp
				@param composite the composite to be spliced
		*/
		void spliceAfter(Composite& composite) ;

		/**	Insert the children of composite into this composite.
				The children of <tt>composite</tt> are inserted at the position of 
				<tt>composite</tt> if <tt>composite</tt> is a child of <tt>this</tt>.
				Otherwise the children are inserted using  \link spliceBefore spliceBefore \endlink .
				 \par
				This method updates the modification time stamp.
				@see stamp
				@param composite the composite to be spliced
		*/
		void splice(Composite& composite) ;

		/**	Remove a child from its parent.
				<tt>child</tt> is only removed, if it is a true child of <tt>this</tt>.
				 \par
				This method updates the modification time stamp of <tt>this</tt>.
				@see stamp
				@param child the child to remove
				@return false if child could not be removed
		*/
		bool removeChild(Composite& child) ;


		/**	Remove selected subcomposites.
				This method iterates over all children of the current composite
				and removes all selected composites by <tt>delete</tt>ing them.
				If the respective Composite are not \link AutoDeletable \endlink,
				they are just \link remove\endlink d from the hierarchy, but not
				deleted.

				This method is useful in combination with the \link Selector \endlink
				class in order to remove unwanted partitions of kernel data structures.

				@return the number of composites deleted.
		*/
		Size removeSelected() ;

		/** Remove unselected subcomposites.
		    This method iterates over all children of the current composite
		    and removes all unselected composites by <tt>delete</tt>ing them.
		    If the respective Composite are not \link AutoDeletable \endlink,
		    they are just \link remove\endlink d from the hierarchy, but not
		    deleted.

		    This method is useful in combination with the \link Selector \endlink
		    class in order to remove unwanted partitions of kernel data structures.

		    @return the number of composites deleted.
		*/
		Size removeUnselected();

		/** This instance and its subtree is removed form its tree and 
				replaced by <tt>composite</tt> and its subtree.
				 \par
				This method updates the modification time stamp of 
				<tt>this</tt> and <tt>composite</tt>.
				@see stamp
				@param	composite the composite which will be inserted
		*/
		void replace(Composite& composite) ;

		/**	Swap the contents of two composites.
				 \par
				This  method updates the modification time stamp of <tt>this</tt> and
				<tt>composite</tt>.
				@see stamp
				@param	composite the composite with which the contents will be
								swapped
		*/
		void swap(Composite& composite) ;

		/**	Select a composite.
				This method selects the composite and all the composites therein.
				 \par
				If the state of this composite is modified, its selection time
				stamp is updated and that of its ancestors (up to and including the
				root composite) as well. The time stamps of descendants that
				changed their selection state are update, too.
		*/	
		virtual void select() ;

		/**	Deselect a composite.
				This method deselects the composite and all the composites therein.
				 \par
				If the state of this composite is modified, its selection time
				stamp is updated and that of its ancestors (up to and including the
				root composite) as well. The time stamps of descendants that
				changed their selection state are update, too.
		*/	
		virtual void deselect() ;
		//@}

		/**	@name	Predicates */
		//@{

		/**	Equality operator.
				Compares the handles of two Composite objects, therefore two
				Composite objects can never be equal.
				@see Object::operator ==
				@param	composite the composite against which equality will be tested
		*/
		bool operator == (const Composite& composite) const ;

		/**	Inequality operator.
				@see operator ==
B		*/
		bool operator != (const Composite& composite) const
			;

		/**	Return true if the node does not contain children.
				@return bool <b>true</b> if <tt>number_of_children_ == 0</tt>
		*/
		bool isEmpty() const ;

		/**	Return true if the node has no parent.
				@return bool <b>true</b> if <tt>parent_ == 0</tt>
		*/
		bool isRoot() const ;
	
		/** Return true if the node is root of composite.
		*/
		bool isRootOf(const Composite& composite) const ;
	
		/** Return true if the node is not the root or a leaf.
		*/
		bool isInterior() const ;
	
		/** Return true if the node has a child.
		*/
		bool hasChild() const ;
	
		/** Return true if the node has the parent <tt>composite</tt>.
		*/
		bool isChildOf(const Composite& composite) const ;
	
		/** Return true if the node is the first child of its parent.
		*/
		bool isFirstChild() const ;
	
		/** Return true if the node is the first child of <tt>composite</tt>.
		*/
		bool isFirstChildOf(const Composite& composite) const ;
	
		/** Return true if the node is the last child of its parent.
		*/
		bool isLastChild() const ;
	
		/** Return true if the node is the last child of <tt>composite</tt>.
		*/
		bool isLastChildOf(const Composite& composite) const ;
	
		/** Return true if the node has a parent.
		*/
		bool hasParent() const ;

		/** Return true if the node is the parent of <tt>composite</tt>.
		*/
		bool isParentOf(const Composite& composite) const ;

		/** Return true if the node has a sibling.
				(Its parent has other childs.)
		*/
		bool hasSibling() const ;
			
		/** Return true if the node is a sibling of <tt>composite</tt>.
		*/
		bool isSiblingOf(const Composite& composite) const ;
			
		/** Return true if the node has a previous sibling.
				(Its parent has a child before this.)
		*/
		bool hasPreviousSibling() const ;
	
		/** Return true if the node is a previous sibling of <tt>composite</tt>.
		*/
		bool isPreviousSiblingOf(const Composite& composite) const ;
	
		/** Return true if the node has a previous sibling.
				(Its parent has a child after this.)
		*/
		bool hasNextSibling() const ;

		/** Return true if the node is a next sibling of <tt>composite</tt>.
		*/
		bool isNextSiblingOf(const Composite& composite) const ;
		
		/** Return true if the node is a descendent of <tt>composite</tt>.
		*/
		bool isDescendantOf(const Composite& composite) const ;

		/** Return true if the node has a ancestor of the same type as dummy.
		*/
		template <typename T>
		bool hasAncestor(const T& dummy) const	;

		/** Return true if the node has composite as descendent.
		*/
		bool isAncestorOf(const Composite& composite) const	;

		/** Return true if the node has composite as ancestor or
				composite is ancestor of this node.
		*/
		bool isRelatedWith(const Composite& composite) const ;
	
		/** Return true if composite is homomorph to this node.
				(The subtrees of the two instances have to be of the same form.)
		*/
		bool isHomomorph(const Composite& composite) const ;

		/**	Return true if any descendant is selected.
				This method does not check all nodes recursively. Instead, on each
				modification of the tree, internal flags are updated and the
				information is propagated upwards in the tree.
				 \par
				Complexity: O(1)
				 \par
				@return bool <b>true</b> if any node in the subtree is selected
		*/
		bool containsSelection() const ;
		//@}

		/** @name Type predicates
		 *
		 * These predicates should be used for type checking instead of dynamic_casts
		 * as a virtual function call is orders of magnitudes faster.
		 */
		//@{
		/**
		 * Test if the Composite is an Atom
		 */
		virtual bool isAtom() const { return false; }

		/**
		 * Test if the Composite is a Molecule
		 */
		virtual bool isMolecule() const { return false; }

		/**
		 * Test if the Composite is a Protein
		 */
		virtual bool isProtein() const { return false; }

		/**
		 * Test if the Composite is a Fragment
		 */
		virtual bool isFragment() const { return false; }

		/**
		 * Test if the Composite is a Residue
		 */
		virtual bool isResidue() const { return false; }

		/**
		 * Test if the Composite is a Chain
		 */
		virtual bool isChain() const { return false; }
		//@}

		/**	@name	Debugging and Diagnostics */
		//@{
		/** Test if the subtree with this node as root is valid.
				(The structure of the subtree has to be valid.)
		*/
		virtual bool isValid() const ;

		/** Dump the constent of this instance to an ostream.
				@param	s the stream to which we will dump
				@param	depth the indentation depth of the output
		*/
		virtual void dump(std::ostream& s = std::cout, Size depth = 0) const
			;

		//@}
		/**	@name	Application and Hosting */
		//@{

		/**	Visitor host method.
				Composites may be visited.
				For an example look into Composite_test.C
				@param	visitor	the visitor
				\throws Exception::GeneralException
		*/
		void host(Visitor<Composite>& visitor);

		/** Apply a processor to all ancestors of this node.
				@return true if the processor could be applied.
				\throws Exception::GeneralException
		*/
		template <typename T>
		bool applyAncestor(UnaryProcessor<T>& processor);

		/** Apply a const processor to all ancestors of this node.
				@return true if the processor could be applied.
				\throws Exception::GeneralException
		*/
		template <typename T>
		bool applyAncestor(ConstUnaryProcessor<T>& processor) const;


		/** Apply a processor to all children of this node.
				@return true if the processor could be applied.
				\throws Exception::GeneralException
		*/
		template <typename T>
		bool applyChild(UnaryProcessor<T>& processor);

		/** Apply a const processor to all children of this node.
				@return true if the processor could be applied.
				\throws Exception::GeneralException
		*/
		template <typename T>
		bool applyChild(ConstUnaryProcessor<T>& processor) const;

		/** Apply a processor to all descendents of this node.
				The node itself is not processed.
				The root of a subtree is accessed before the nodes in its left 
				and right subtree.
				@return true if the processor could be applied.
				\throws Exception::GeneralException
		*/
		template <typename T>
		bool applyDescendantPreorder(UnaryProcessor<T>& processor);

		/** Apply a const processor to all descendents of this node.
				The node itself is not processed.
				The root of a subtree is accessed before the nodes in its left 
				and right subtree.
				@return true if the processor could be applied.
				\throws Exception::GeneralException
		*/
		template <typename T>
		bool applyDescendantPreorder(ConstUnaryProcessor<T>& processor) const;

		/** Apply a processor to all descendents of this node.
				The node itself is not processed.
				The root of a subtree is accessed after the nodes in its left 
				and right subtree.
				@return true if the processor could be applied.
				\throws Exception::GeneralException
		*/
		template <typename T>
		bool applyDescendantPostorder(UnaryProcessor<T>& processor);

		/** Apply a const processor to all descendents of this node.
				The node itself is not processed.
				The root of a subtree is accessed after the nodes in its left 
				and right subtree.
				@return true if the processor could be applied.
				\throws Exception::GeneralException
		*/
		template <typename T>
		bool applyDescendantPostorder(ConstUnaryProcessor<T>& processor) const;

		/** Apply a processor to all descendents of this node.
				The node itself is not processed.
				applyDescendantPreorder is used.
				@see applyDescendantPreorder
				@return true if the processor could be applied.
				\throws Exception::GeneralException
		*/
		template <typename T>
		bool applyDescendant(UnaryProcessor<T>& processor);

		/** Apply a const processor to all descendents of this node.
				The node itself is not processed.
				applyDescendantPreorder is used.
				@see applyDescendantPreorder
				@return true if the processor could be applied.
				\throws Exception::GeneralException
		*/
		template <typename T>
		bool applyDescendant(ConstUnaryProcessor<T>& processor) const;

		/** Apply a processor to the node and its subtree.
				The root of a subtree is accessed before the nodes in its left 
				and right subtree.
				@return true if the processor could be applied.
				\throws Exception::GeneralException
		*/
		template <typename T>
		bool applyPreorder(UnaryProcessor<T>& processor);

		/** Apply a const processor to the node and its subtree.
				The root of a subtree is accessed before the nodes in its left 
				and right subtree.
				@return true if the processor could be applied.
				\throws Exception::GeneralException
		*/
		template <typename T>
		bool applyPreorder(ConstUnaryProcessor<T>& processor) const;

		/** Apply a processor to the node and its subtree.
				The root of a subtree is accessed after the nodes in its left 
				and right subtree.
				@return true if the processor could be applied.
				\throws Exception::GeneralException
		*/
		template <typename T>
		bool applyPostorder(UnaryProcessor<T>& processor);

		/** Apply a const processor to the node and its subtree.
				The root of a subtree is accessed after the nodes in its left 
				and right subtree.
				@return true if the processor could be applied.
				\throws Exception::GeneralException
		*/
		template <typename T>
		bool applyPostorder(ConstUnaryProcessor<T>& processor) const;

		/** Apply a processor to the node and its subtree.
				applyPreorder is used.
				@see applyPreorder
				@return true if the processor could be applied.
				\throws Exception::GeneralException
		*/
		template <typename T>
		bool apply(UnaryProcessor<T>& processor);

		/** Apply a const processor to the node and its subtree.
				applyPreorder is used.
				@see applyPreorder
				@return true if the processor could be applied.
				\throws Exception::GeneralException
		*/
		template <typename T>
		bool apply(ConstUnaryProcessor<T>& processor) const;

		/** Apply a processor to the node and its siblings.
				@return true if the processor could be applied.
				\throws Exception::GeneralException
		*/
		template <typename T>
		bool applyLevel(UnaryProcessor<T>& processor, long level);
		//@}			

		/** Apply a const processor to the node and its siblings.
				@return true if the processor could be applied.
				\throws Exception::GeneralException
		*/
		template <typename T>
		bool applyLevel(ConstUnaryProcessor<T>& processor, long level) const;
		//@}			



	
		class BALL_EXPORT AncestorIteratorTraits
		{
			public:

			BALL_INLINE
			AncestorIteratorTraits()
				
				:	bound_(0),
					ancestor_(0)
			{
			}
		
			BALL_INLINE
			AncestorIteratorTraits(const Composite& composite)
				
				:	bound_(const_cast<Composite*>(&composite)),
					ancestor_(0)
			{
			}
		
			BALL_INLINE
			AncestorIteratorTraits(const AncestorIteratorTraits& traits)
				
				:	bound_(traits.bound_),
					ancestor_(traits.ancestor_)
			{
			}
		
			BALL_INLINE
			const AncestorIteratorTraits& operator = (const AncestorIteratorTraits& traits)
				
			{
				bound_ = traits.bound_;
				ancestor_ = traits.ancestor_;
				return *this;
			}

			BALL_INLINE	Composite* getContainer()	 { return bound_; }

			BALL_INLINE	const Composite* getContainer() const	 { return bound_; }

			BALL_INLINE	bool isSingular() const		{	return (bound_ == 0);	}

			BALL_INLINE	Composite* getPosition()		{	return ancestor_;	}

			BALL_INLINE	Composite* const& getPosition() const	 {	return ancestor_;	}

			BALL_INLINE	bool operator == (const AncestorIteratorTraits& traits) const		{	return (ancestor_ == traits.ancestor_);	}
		
			BALL_INLINE	bool operator != (const AncestorIteratorTraits& traits) const		{	return !(ancestor_ == traits.ancestor_); }

			BALL_INLINE	bool isValid() const		{	return (bound_ != 0 && ancestor_ != 0);	}

			BALL_INLINE	void invalidate()		{	bound_ 	= ancestor_ = 0; }
			
			BALL_INLINE	void toBegin()		{	ancestor_ = bound_->parent_; }

			BALL_INLINE	bool isBegin() const  { return (ancestor_ == bound_->parent_); }

			BALL_INLINE void toEnd()  { ancestor_ = 0;	}

			BALL_INLINE	bool isEnd() const  { return (ancestor_ == 0); }

			BALL_INLINE Composite& getData()  { return *ancestor_;	}

			BALL_INLINE	const Composite& getData() const  { return *ancestor_; }

			BALL_INLINE void forward()  { ancestor_ = ancestor_->parent_; }

			private:

			Composite* bound_;
			Composite* ancestor_;
		};

		friend class AncestorIteratorTraits;

		typedef ForwardIterator <Composite, Composite, Composite*, AncestorIteratorTraits>
			AncestorIterator;

		AncestorIterator beginAncestor() 
		{
			return AncestorIterator::begin(*this);
		}

		AncestorIterator endAncestor() 
		{
			return AncestorIterator::end(*this);
		}

		typedef ConstForwardIterator<Composite, Composite, Composite*, AncestorIteratorTraits>
			AncestorConstIterator;

		AncestorConstIterator beginAncestor() const 
		{
			return AncestorConstIterator::begin(*this);
		}

		AncestorConstIterator endAncestor() const 
		{
			return AncestorConstIterator::end(*this);
		}

		class BALL_EXPORT ChildCompositeIteratorTraits
		{
			public:

			ChildCompositeIteratorTraits()
				
				:	bound_(0),
					child_(0)
			{
			}
			
			ChildCompositeIteratorTraits(const Composite& composite)
				
				:	bound_((Composite *)&composite),
					child_(0)
			{
			}
		
			ChildCompositeIteratorTraits(const ChildCompositeIteratorTraits& traits)
				
				:	bound_(traits.bound_),
					child_(traits.child_)
			{
			}
		
			const ChildCompositeIteratorTraits& operator = (const ChildCompositeIteratorTraits& traits)
				
			{
				bound_ = traits.bound_;
				child_ = traits.child_;
				return *this;
			}

			BALL_INLINE Composite* getContainer()	 {	return bound_; }

			BALL_INLINE const Composite* getContainer() const 	{	return bound_; }

			BALL_INLINE bool isSingular() const 	{ return (bound_ == 0);	}

			BALL_INLINE Composite* getPosition() 	{ return child_; }

			BALL_INLINE Composite* const& getPosition() const		{ return child_; }

			BALL_INLINE bool operator == (const ChildCompositeIteratorTraits& traits) const  { return (child_ == traits.child_); }
		
			BALL_INLINE bool operator != (const ChildCompositeIteratorTraits& traits) const  { return !(child_ == traits.child_); }
		
			BALL_INLINE bool isValid() const  { return (bound_ != 0 && child_ != 0); }

			BALL_INLINE void invalidate()  { bound_ = child_ = 0; }

			BALL_INLINE void toBegin()  { child_ = bound_->first_child_; }

			BALL_INLINE bool isBegin() const  { return (child_ == bound_->first_child_); }

			BALL_INLINE void toEnd()  { child_ = 0; }

			BALL_INLINE bool isEnd() const  { return (child_ == 0); }

			BALL_INLINE void toRBegin()  { child_ = bound_->last_child_; }

			BALL_INLINE bool isRBegin() const  { return (child_ == bound_->last_child_); }

			BALL_INLINE void toREnd()  { child_ = 0; }

			BALL_INLINE bool isREnd() const  { return (child_ == 0); }

			BALL_INLINE Composite& getData()  { return *child_; }

			BALL_INLINE const Composite& getData() const  { return *child_; }

			BALL_INLINE void forward()  {	child_ = child_->next_;	}

			BALL_INLINE void backward()		
			{		
				if (child_ == 0) 
				{ 
					// Allow decrementation for past-the-end iterators
					child_ = bound_->last_child_; 
				}
				else  
				{ 
					child_ = child_->previous_; 
				}	
			}

			private:

			Composite* bound_;
			Composite* child_;
		};

		friend class ChildCompositeIteratorTraits;

		typedef BidirectionalIterator<Composite, Composite, Composite *, ChildCompositeIteratorTraits>
			ChildCompositeIterator;

		ChildCompositeIterator beginChildComposite()
			
		{
			return ChildCompositeIterator::begin(*this);
		}

		ChildCompositeIterator endChildComposite()
			
		{
			return ChildCompositeIterator::end(*this);
		}



		typedef ConstBidirectionalIterator<Composite, Composite, Composite *, ChildCompositeIteratorTraits>
			ChildCompositeConstIterator;

		ChildCompositeConstIterator beginChildComposite() const
			
		{
			return ChildCompositeConstIterator::begin(*this);
		}

		ChildCompositeConstIterator endChildComposite() const
			
		{
			return ChildCompositeConstIterator::end(*this);
		}



		typedef std::reverse_iterator<ChildCompositeIterator> ChildCompositeReverseIterator;

		ChildCompositeReverseIterator rbeginChildComposite() 
		{
			return ChildCompositeReverseIterator(endChildComposite());
		}

		ChildCompositeReverseIterator rendChildComposite() 
		{
			return ChildCompositeReverseIterator(beginChildComposite());
		}



		typedef std::reverse_iterator<ChildCompositeConstIterator> ChildCompositeConstReverseIterator;

		ChildCompositeConstReverseIterator rbeginChildComposite() const 
		{
			return ChildCompositeConstReverseIterator(endChildComposite());
		}

		ChildCompositeConstReverseIterator rendChildComposite() const 
		{
			return ChildCompositeConstReverseIterator(beginChildComposite());
		}

		class BALL_EXPORT CompositeIteratorTraits
		{
			public:

			BALL_INLINE CompositeIteratorTraits()
				
				:	bound_(0),
					position_(0)
			{
			}
		
			CompositeIteratorTraits(const Composite& composite)
				
				:	bound_(const_cast<Composite*>(&composite)),
					position_(0)
			{
			}
		
			CompositeIteratorTraits(const CompositeIteratorTraits& traits)
				
				:	bound_(traits.bound_),
					position_(traits.position_)
			{
			}

			BALL_INLINE ~CompositeIteratorTraits()  {}
		
			BALL_INLINE bool isValid() const  
			{ 
				return ((bound_ != 0) && (position_ != 0)); 
			}

			BALL_INLINE CompositeIteratorTraits& operator = (const CompositeIteratorTraits& traits) 
			{
				bound_ = traits.bound_;
				position_ = traits.position_;
				return *this;
			}

			BALL_INLINE Composite* getContainer()		{	return bound_; }

			BALL_INLINE const Composite* getContainer() const	 { return bound_; }
		
			BALL_INLINE bool isSingular() const		{	return (bound_ == 0);	}
		
			BALL_INLINE Composite* getPosition()  { return position_;	}
			
			BALL_INLINE const Composite* getPosition() const  { return position_; }
			BALL_INLINE void setPosition(Composite* position)  { position_ = position; }


			BALL_INLINE Composite& getData()  { return *position_; }

			BALL_INLINE const Composite& getData() const  { return *position_; }

			BALL_INLINE bool operator == (const CompositeIteratorTraits& traits) const  
			{ 
				return (position_ == traits.position_); 
			}
		
			BALL_INLINE bool operator != (const CompositeIteratorTraits& traits) const  
			{	
				return !(position_ == traits.position_); 
			}
		
			BALL_INLINE void invalidate() 	
			{ 
				bound_ = 0;	
				position_ = 0;
			}

			BALL_INLINE void toBegin() 
			{
				position_ = bound_;
			}

			BALL_INLINE bool isBegin() const 
			{
				return (position_ == bound_);
			}

			BALL_INLINE void toEnd()  
			{
				position_ = 0;
			}

			BALL_INLINE bool isEnd() const 
			{
				return (position_ == 0);
			}

			BALL_INLINE void toRBegin() 
			{
				if (bound_ != 0)
				{
					position_ = findPreviousPosition(0);
				}
			}

			BALL_INLINE bool isRBegin() const 
			{
				return (position_ == findPreviousPosition(0));
			}
		
			BALL_INLINE void toREnd() 
			{	
				position_ = bound_;
			}

			BALL_INLINE bool isREnd() const 
			{
				return (position_ == bound_);
			}
		
			BALL_INLINE void forward() 
			{
				position_ = findNextPosition(position_);
			}

			BALL_INLINE void backward()	
			{
				position_ = findPreviousPosition(position_);
			}

			protected:

			/// A pointer to the "container" the iterator is bound to
			Composite* bound_;

			/// The current iterator position
			Composite* position_;

			Composite* findPreviousPosition(Composite* p) const
			{
				// If we are at the root of the iterator, the 
				// decrementing it results in an invalid iterator
				// (past-the-end).
				if (p == bound_)
				{
					return 0;
				}
				// If we decrement a past-the-end-iterator, we
				// start at the root and "fall down" on the right
				// hand side following the last_child_ pointers
				// until we hit bottom.
				else if (p == 0)
				{
					if (bound_->last_child_ == 0)
					{
						return bound_;
					}
					else
					{
						p = bound_->last_child_;
					}
					while (p->last_child_ != 0)
					{
						p = p->last_child_;
					}
				}
				// Normally, we just grab the guy to the
				// left in the doubly-linked child list.
				else if (p->previous_ != 0)
				{
					p = p->previous_;

					// If the guy to the left hast children,
					// we do the drop on the rigth again.
					while (p->last_child_ != 0)
					{
						p = p->last_child_;
					}
				}
				// Finally, if we can't go down and we can't go 
				// left, we have to go upwards.
				else if (p != bound_)
				{
					p = p->parent_;
				}

				return p;
			}

			Composite* findNextPosition(Composite* p) const
			{
				// If we are in a past-the-end position, we stay put.
				if (p == 0)
				{
					return 0;
				}
				// Otherwise, we try the first child. If there's one,
				// that's our next position.
				else 
				{
					if (p->first_child_ != 0)
					{
						p = p->first_child_;
					}
					else 
					{
						// If we are already in the root node, we are done.
						if (p == bound_)
						{
							return 0;
						}
						// Otherwise, we try to walk to the right at the current level.
						if (p->next_ != 0)
						{
							p = p->next_;
						}
						// If that doesn't work out, we'll have to climb up again.
						// Now, we either revisit a node we have already been to, or we
						// are trying to climb up *beyond* our iteration root (bound_).
						// In the latter case, we return a past-the-end-iterator (0).
						else
						{
							// If we could not walk left or right and we are at the root
							// again, then we are done with the iteration (this is the
							// case if bound_ is a leaf node).
							while (p->next_ == 0)
							{
								p = p->parent_;
								if ((p == bound_) || (p == 0))
								{
									return 0;
								}
							}
							p = p->next_;
						}
					}
				}
				return p;
			}
		};

		friend class CompositeIteratorTraits;

		typedef BidirectionalIterator<Composite, Composite, Composite*, CompositeIteratorTraits>
			CompositeIterator;

		CompositeIterator beginComposite()  { return CompositeIterator::begin(*this); }

		CompositeIterator endComposite()  { return CompositeIterator::end(*this); }

		typedef ConstBidirectionalIterator<Composite, Composite, Composite*, CompositeIteratorTraits>
			CompositeConstIterator;

		CompositeConstIterator beginComposite() const  
		{	
			return CompositeConstIterator::begin(*this);
		}

		CompositeConstIterator endComposite() const	
		{
			return CompositeConstIterator::end(*this);
		}


		typedef std::reverse_iterator<CompositeIterator> CompositeReverseIterator;

		CompositeReverseIterator rbeginComposite() 
		{
			return CompositeReverseIterator(endComposite());
		}

		CompositeReverseIterator rendComposite() 
		{
			return CompositeReverseIterator(beginComposite());
		}


		typedef std::reverse_iterator<CompositeConstIterator> CompositeConstReverseIterator;

		CompositeConstReverseIterator rbeginComposite() const 
		{
			return CompositeConstReverseIterator(endComposite());
		}

		CompositeConstReverseIterator rendComposite() const 
		{
			return CompositeConstReverseIterator(beginComposite());
		}

		/*
		 * This function removes and deletes all composites that are
		 * supplied in the list of children.
		 */
		void deleteChildrenList_(std::list<Composite*>& composites);

		private:
		
		///
		Size getHeight_(Size size, Size& max_height) const ;
	
		///
		Size countDescendants_() const ;

		///
		void clone_(Composite& parent, Composite& stack) const ;

		// \throws Exception::GeneralException
		template <typename T>
		bool applyLevelNostart_(UnaryProcessor<T>& processor, long level);

		// \throws Exception::GeneralException
		template <typename T>
		bool applyLevelNostart_(ConstUnaryProcessor<T>& processor, long level) const;

		// \throws Exception::GeneralException
		template <typename T>
		bool applyChildNostart_(UnaryProcessor<T>& processor);

		// \throws Exception::GeneralException
		template <typename T>
		bool applyChildNostart_(ConstUnaryProcessor<T>& processor) const;

		// \throws Exception::GeneralException
		template <typename T>
		bool applyPreorderNostart_(UnaryProcessor<T>& processor);

		// \throws Exception::GeneralException
		bool applyPreorderNostart_(UnaryProcessor<Atom>& processor);

		// \throws Exception::GeneralException
		template <typename T>
		bool applyPreorderNostart_(ConstUnaryProcessor<T>& processor) const;

		// \throws Exception::GeneralException
		template <typename T>
		bool applyDescendantPreorderNostart_(UnaryProcessor<T>& processor);

		// \throws Exception::GeneralException
		bool applyDescendantPreorderNostart_(UnaryProcessor<Atom>& processor);

		// \throws Exception::GeneralException
		template <typename T>
		bool applyDescendantPreorderNostart_(ConstUnaryProcessor<T>& processor) const;

		// \throws Exception::GeneralException
		template <typename T>
		bool applyDescendantPostorderNostart_(UnaryProcessor<T>& processor);

		// \throws Exception::GeneralException
		template <typename T>
		bool applyDescendantPostorderNostart_(ConstUnaryProcessor<T>& processor) const;

		void updateSelection_();
		void determineSelection_();
		void select_(bool update_parent = true);
		void deselect_(bool update_parent = true);

		void destroyChildren_();

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
	{
		if (processor.start() == false)
		{
			return false;
		}

		Processor::Result result;

		for (Composite* composite = parent_; composite != 0; composite = composite->parent_)
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
	bool Composite::applyAncestor(ConstUnaryProcessor<T>& processor) const
	{
		if (processor.start() == false)
		{
			return false;
		}

		Processor::Result result;

		for (const Composite* composite = parent_; composite != 0; composite = composite->parent_)
		{
			const T* t_ptr;
			if ((t_ptr = dynamic_cast<const T*>(composite)) != 0)
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
	{
		return processor.start() && applyChildNostart_(processor) && processor.finish();
	}

	template <typename T>
	bool Composite::applyChild(ConstUnaryProcessor<T>& processor) const
	{
		return processor.start() && applyChildNostart_(processor) && processor.finish();
	}

	template <typename T>
	bool Composite::applyChildNostart_(UnaryProcessor<T>& processor)
	{
		Processor::Result result = Processor::CONTINUE;

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

		return (result >= Processor::BREAK);
	}

	template <typename T>
	bool Composite::applyChildNostart_(ConstUnaryProcessor<T>& processor) const
	{
		Processor::Result result = Processor::CONTINUE;

		for (const Composite* composite = first_child_;
				 composite != 0; composite = composite->next_)
		{
			const T* t_ptr;
			if ((t_ptr = dynamic_cast<const T*>(composite)) != 0)
			{
				result = processor(*t_ptr);
				if (result <= Processor::BREAK)
				{
					break;
				}
			}
		}

		return (result >= Processor::BREAK);
	}

	template <typename T>
	bool Composite::applyDescendantPreorder(UnaryProcessor<T>& processor)
	{
		return processor.start() && applyDescendantPreorderNostart_(processor) && processor.finish();
	}

	template <typename T>
	bool Composite::applyDescendantPreorder(ConstUnaryProcessor<T>& processor) const
	{
		return processor.start() && applyDescendantPreorderNostart_(processor) && processor.finish();
	}

	template <typename T>
	bool Composite::applyDescendantPreorderNostart_(UnaryProcessor<T>& processor)
	{
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
	bool Composite::applyDescendantPreorderNostart_(ConstUnaryProcessor<T>& processor) const
	{
		Processor::Result result;

		for (const Composite* composite = first_child_;
				 composite != 0; composite = composite->next_)
		{
			const T* t_ptr;
			if ((t_ptr = dynamic_cast<const T*>(composite)) != 0)
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
	{
		return processor.start() && applyDescendantPostorderNostart_(processor) && processor.finish();
	}

	template <typename T>
	bool Composite::applyDescendantPostorder(ConstUnaryProcessor<T>& processor) const
	{
		return processor.start() && applyDescendantPostorderNostart_(processor) && processor.finish();
	}

	template <typename T>
	bool Composite::applyDescendantPostorderNostart_(UnaryProcessor<T>& processor)
	{
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
	bool Composite::applyDescendantPostorderNostart_(ConstUnaryProcessor<T>& processor) const
	{
		Processor::Result result;

		for (const Composite* composite = first_child_;
				 composite != 0; composite = composite->next_)
		{
			if (composite->first_child_ != 0 &&
					composite->applyDescendantPostorderNostart_(processor) == false)
			{
				return false;
			}

			const T* t_ptr = dynamic_cast<const T*>(composite);
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
	{
		if (!processor.start() || !applyDescendantPostorderNostart_(processor))
		{
			return false;
		}

		T* t_ptr = dynamic_cast<T*>(this);

		return (t_ptr != 0                            &&
						processor(*t_ptr) >= Processor::BREAK &&
						processor.finish()                      );
	}

	template <typename T>
	bool Composite::applyPostorder(ConstUnaryProcessor<T>& processor) const
	{
		if (!processor.start() || !applyDescendantPostorderNostart_(processor))
		{
			return false;
		}

		const T* t_ptr = dynamic_cast<const T*>(this);

		return (t_ptr != 0                            &&
						processor(*t_ptr) >= Processor::BREAK &&
						processor.finish()                      );
	}


	template <typename T>
	bool Composite::applyLevel(UnaryProcessor<T>& processor, long level)
	{
		return processor.start() && applyLevelNostart_(processor, level) && processor.finish();
	}

	template <typename T>
	bool Composite::applyLevel(ConstUnaryProcessor<T>& processor, long level) const
	{
		return processor.start() && applyLevelNostart_(processor, level) && processor.finish();
	}

	template <typename T>
	bool Composite::applyLevelNostart_(UnaryProcessor<T>& processor, long level)
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
	bool Composite::applyLevelNostart_(ConstUnaryProcessor<T>& processor, long level) const
	{
		if (level == 0)
		{
			const T* t_ptr = dynamic_cast<const T*>(this);
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
					for (const Composite* composite = first_child_;
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
	bool Composite::applyPreorderNostart_(ConstUnaryProcessor<T>& processor) const
	{
		Processor::Result result;
		bool return_value;
		const T* t_ptr = dynamic_cast<const T*>(this);
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
	{
		return applyDescendantPreorder(processor);
	}

	template <typename T>
	bool Composite::applyDescendant(ConstUnaryProcessor<T>& processor) const
	{
		return applyDescendantPreorder(processor);
	}

	template <typename T>
	bool Composite::applyPreorder(UnaryProcessor<T>& processor)
	{
		return processor.start() && applyPreorderNostart_(processor) && processor.finish();
	}

	template <typename T>
	bool Composite::applyPreorder(ConstUnaryProcessor<T>& processor) const
	{
		return processor.start() && applyPreorderNostart_(processor) && processor.finish();
	}

	template <typename T>
	BALL_INLINE
	bool Composite::apply(UnaryProcessor<T>& processor)
	{
		return applyPreorder(processor);
	}

	template <typename T>
	BALL_INLINE
	bool Composite::apply(ConstUnaryProcessor<T>& processor) const
	{
		return applyPreorder(processor);
	}

	template <typename T>
	BALL_INLINE 
	T* Composite::getAncestor(const T& /* dummy */)
		
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

	template <typename T>
	BALL_INLINE 
	const T* Composite::getAncestor(const T& /* dummy */) const
		
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
		
	{
		// create an iterator bound to the root of the subtree
		CompositeIterator it(getRoot().endComposite());

		// set its position to the current composite
		it.getTraits().setPosition(this);

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

	template <typename T>
	BALL_INLINE 
	const T* Composite::getPrevious(const T& dummy) const
		
	{
		// cast away the constness of this and call the non-const method
		Composite* nonconst_this = const_cast<Composite*>(this);

		return const_cast<const T*>(nonconst_this->getPrevious(dummy));
	}

	template <typename T>
	BALL_INLINE 
	T* Composite::getNext(const T& /* dummy */)
		
	{
		// create an iterator bound to the root of the subtree
		CompositeIterator it(getRoot().beginComposite());

		// set its position to the current composite
		it.getTraits().setPosition(this);

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

	template <typename T>
	BALL_INLINE 
	const T* Composite::getNext(const T& dummy) const
		
	{
		// cast away the constness of this and call the non-const method
		Composite* nonconst_this = const_cast<Composite*>(this);

		return const_cast<const T*>(nonconst_this->getNext(dummy));
	}

	template <typename T>
	BALL_INLINE 
	bool Composite::hasAncestor(const T& dummy ) const 
		
	{
		return (getAncestor(dummy) != 0);	
	}

#	ifndef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/CONCEPT/composite.iC>
#	endif


} // namespace BALL

#endif // BALL_CONCEPT_COMPOSITE_H
