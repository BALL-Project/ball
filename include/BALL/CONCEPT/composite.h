// $Id: composite.h,v 1.22 2000/08/29 20:00:14 amoll Exp $

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

#ifndef BALL_CONCEPT_REVERSEBIDIRECTIONALITERATOR_H
#	include <BALL/CONCEPT/reverseBidirectionalIterator.h>
#endif

#ifndef BALL_CONCEPT_FORWARDITERATOR_H
#	include <BALL/CONCEPT/forwardIterator.h>
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

namespace BALL 
{
	/**	Composite Class.
			This class implements a variant of the composite design pattern.
			A Composite may contain an arbitrary number of other composites, thus forming
			a tree. All BALL kernel classes are derived from Composite. This provides a unique
			interface for all kernel classes.\\

			The composite class provides a selection mechanism that allows hierarchical selection
			and deselection of arbitrary subtrees. The time of the last selection/deselection
			operation is stored as well as the time of the last modification operation in time stamps
			that can be accessed via \Ref{getModificationTime} and \Ref{getSelectionTime}.
			
			Selecting or deselecting a Composite automatically selects or deselects all its children (recursively!).
			Selecting or deselecting all children of a node deselects their parent as well.	
			Selection information is propagated upwards in the tree.
		
	
			Composites are persistent objects. 
			\\
			{\bf Definition:} \URL{BALL/CONCEPT/composite.h}
	*/
	class Composite
		: public PersistentObject,
			public Object,
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
				As it is used in as a predicate for all kernel classes,
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
				
		BALL_CREATE(Composite)

		static UnaryProcessor<Composite> DEFAULT_PROCESSOR;
		static KernelPredicateType DEFAULT_UNARY_PREDICATE;
		
		/**	@name	Construction and Destruction 
		*/
		//@{
		
		/**	Default constructor.
				This constructor creates an empty composite object.
		*/
		Composite();

		/**	Copy constructor.
				Creates a copy of a composite. {\em Deep} copies include the whole
				composite tree, {\em shallow} copies contain anly a single composite.
				@param	composite the composite to be cloned (the root of the tree in the case of a deep copy)
				@param	deep make a deep copy ({\bf true}) or shallow copy ({\bf false})
		*/
		Composite(const Composite& composite, bool deep = true);

		/**	Destructor.	
				The destructor calls \Ref{destroy} to remove the composite from potential tree structures.
				It also recursively destructs all children of the composite.
		*/
		virtual ~Composite();

		/**	Clear the composite properties.	
				This method removes the composite's children and destructs them if they are
				auto-deletable.\\
				It does not remove the composite from any parental structure.\\
				This  method updates the modification time stamp of {\tt this}.
				@see stamp
				@see	AutoDeletable
				@see	destroy
		*/
		virtual void clear();
	
		/**	Destroy the composite.
				This method removes the composite from potential parental structures and
				then calls \Ref{clear} to destruct all children.\\
				This  method updates the modification time stamp of {\tt this}.
				@see stamp
				@see	~Composite
				@see	clear
		*/
		virtual void destroy();

		/**	Non-virtual destroy method.
				This method behaves exactly like destroy except for a small difference:
				when called with {\bf true}, it calls the {\em virtual} clear function.
				If called with {\bf false} it calls the original clear function of Composite.
				This is useful when implementing the behaviour of derived classes.\\
				This  method updates the modification time stamp of {\tt this}.
				@see stamp
				@param	virtual_destroy call the virtual clear method ({\bf true}) or
								{\tt Composite::clear()} ({\bf false})
		*/		
		void destroy(bool virtual_destroy);

		/**	Clone with a predicate.
				This method copies the attributes of {\tt this} composite to root (shallow copy) and
				then adds recursively each composite of the tree that fulfils the {\tt predicate}.
				@param	root the cloning target root is {\tt destroy}ed prior to any copying 
				@param	predicate the predicate
				@param  a pointer to the root composite ({\tt &root})
		*/
		void* clone(Composite& root, KernelPredicateType& predicate) const;

		//@}		

		/**	@name	Persistence 
		*/
		//@{
		
		/** Write a persistent copy of the object.
				@param	pm the persistence manager
				@param	name the object name
		*/
		virtual void persistentWrite(PersistenceManager& pm, const char* name = 0) const;

		/** Read a persistent object.
				@param	pm the persistence manager
		*/
		virtual void persistentRead(PersistenceManager& pm);
		//@}

		/**	@name	Modifying and Accessing the Tree 
		*/
		//@{

		/**	Predicative assignment from a tree.
		*/
		void set(const Composite& composite, KernelPredicateType& predicate);

		/**	Regular tree assignment.
		*/
		void set(const Composite& composite, bool deep = true);

		/**	Assignment operator.
		*/
		Composite& operator = (const Composite& composite);

		/**	Predicative assignment of a tree to another.
				Assign a copy of {\bf this} tree to another tree.
				Only nodes fulfilling the {\tt predicate}	are copied.
				The assigned {\tt composite} is destroyed first.
				@param	composite the composite to be assigned to
				@param	predicate the predicate used to decide which composites are copied
		*/
		void get(Composite& composite, KernelPredicateType& predicate) const;

		/**	Regular assignment of a tree to another.
				Create a deep ({\tt deep} = {\bf true}) or shallow copy of a composite
				and assign it to {\tt composite}. {\tt composite} is destroyed first.
				@param	composite the composite to assign the copy to
				@param	deep {\bf true} for a deep copy
		*/
		void get(Composite& composite, bool deep = true) const;

		/**	Return the degree of node.
				This method returns the number of children of a composite object.
				@return the number of children
		*/
		Size getDegree() const;

		/**	Count the number of nodes fulfilling a predicate in this subtree.
				@param	predicate the predicate
				@return he number of nodes in the subtree stisfying the predicate
		*/
		Size count(const KernelPredicateType& predicate) const;

		/**	Count the number of descendants.
				@return he number of descendants of this node
		*/
		Size countDescendants() const;

		/** Get the length of the path between two composite objects.
				If no path exists {\tt INVALID_SIZE} is returned.
				@param composite the second object
				@return Size the size of the path
		*/
		Size getPathLength(const Composite& composite) const;

		/** Get the depth of this item in its tree.
				The depth of a root item is 0.
				@return Size the depth
		*/
		Size getDepth() const;

		/** Get the height of this item in its tree.
				The hight of a leaf is 0.
				@return Size the height
		*/
		Size getHeight() const;

		/** Get the root of this item.
				@return Composite& the root
		*/
		Composite& getRoot();

		/** Get a const reference to the root of this item.
				@return Composite& the root
		*/
		const Composite& getRoot() const;

		/** Get the lowest common ancestor of this item with an other.
				If no comman ancestor exists 0 is returned.
				@return Composite& the lowest common ancestor
		*/
		Composite* getLowestCommonAncestor(Composite& composite);

		/** Get a const reference to the lowest common ancestor of this item with an other.
				If no comman ancestor exists 0 is returned.
				@return Composite& the lowest common ancestor
		*/
		const Composite* getLowestCommonAncestor(const Composite& composite) const;

		/**	Find the first ancestor of type T.
				This method walks up the tree from parent to parent and
				checks whether the composite object is a kind of {\tt T}.
				This method is useful to identify special container classes.
				@return a pointer to the first composite found that is a kind of T or 0 if no
								matching composite was found up to the root of the tree
		*/
		template <typename T>
		T* getAncestor(const T& /* dummy */);

		/**	Find the first ancestor of type T (const method).
				This method operates also on constant trees.
				@return a pointer to the first composite found that is a kind of T or 0 if no
								matching composite was found to the root of the tree
		*/
		template <class T>
		const T* getAncestor(const T& /* dummy */) const;

		/**	Find the nearest previous composite of type T.
				This method walks backward in the tree from composite to composite and
				checks whether the composite object is a kind of {\tt T}.
				@return a pointer to the first composite found that is a kind of T or 0 if no
								matching composite was found up to the root of the tree
		*/
		template <typename T>
		T* getPrevious(const T& /* dummy */);

		/**	Find the nearest previous composite of type T (const method).
				This method walks backward in the tree from composite to composite and
				checks whether the composite object is a kind of {\tt T}.
				@return a pointer to the first composite found that is a kind of T or 0 if no
								matching composite was found up to the root of the tree
		*/
		template <class T>
		const T* getPrevious(const T& dummy) const;

		/**	Find the next composite of type T.
				This method walks backward in the tree from composite to composite and
				checks whether the composite object is a kind of {\tt T}.
				@return a pointer to the first composite found that is a kind of T or 0 if no
								matching composite was found up to the root of the tree
		*/
		template <typename T>
		T* getNext(const T& /* dummy */);

		/**	Find the next composite of type T (const method).
				This method walks backward in the tree from composite to composite and
				checks whether the composite object is a kind of {\tt T}.
				@return a pointer to the first composite found that is a kind of T or 0 if no
								matching composite was found up to the root of the tree
		*/
		template <class T>
		const T* getNext(const T& dummy) const;

		/**	Return the composite's parent.
				@return a pointer to the parent composite or 0 if no parent exists
		*/
		Composite* getParent();

		/**	Return the composite's parent (const method).
				@return a pointer to the parent composite or 0 if no parent exists
		*/
		const Composite* getParent() const;

		/**	Return the {\em index}th child of this composite.
				If no such child exists, 0 is returned.
				The index of the first child is {\bf 0}.
				@param	index the index of the child to return
		*/
		Composite* getChild(Index index);
	
		/**	Return a const pointer to the {\em index}th child of this composite.
				If no such child exists, 0 is returned.
				The index of the first child is {\bf 0}.
				@param	index the index of the child to return
		*/
		const Composite* getChild(Index index) const;
	
		/**	Return a pointer to the sibling index positions from this composite.
				A pointer to the sibling {\tt index} positions to the right (for
				positive values of {\tt index}) or {\tt -index} positions to the left 
				(for negative values of {\tt index}) is returned.
				For Index = 0 the this-pointer is returned.
		*/
		Composite* getSibling(Index index);

		/**	Return a const pointer to the sibling index positions from this composite.
				A pointer to the sibling {\tt index} positions to the right (for
				positive values of {\tt index}) or {\tt -index} positions to the left 
				(for negative values of {\tt index}) is returned.
				For Index = 0 the this-pointer is returned.
		*/
		const Composite* getSibling(Index index) const;

		/**	Return a pointer to the first child.
				If the composite does not contain a child, 0 is returned.
		*/
		Composite* getFirstChild();

		/**	Return a const pointer to the first child.
				If the composite does not contain a child, 0 is returned.
		*/
		const Composite* getFirstChild() const;

		/**	Return a pointer to the last child.
				If the composite does not contain a child, 0 is returned.
		*/
		Composite* getLastChild();

		/**	Return a const pointer to the last child.
				If the composite does not contain a child, 0 is returned.
		*/
		const Composite* getLastChild() const;
			
		/**	Return the time of last modification
		*/
		const Time& getModificationTime() const throw();

		/**	Return the time of last change of selection.
		*/
		const Time& getSelectionTime() const throw();

		/**	Modify a time stamp.
				Update one or both of the two time stamps with the
				current time. The time stamp is then propagated up to the
				root of the composite tree. Each composite contains two stamps. 
				the \emph{modification stamp} is update each time the tree structure
				changes, while the \emph{selection stamp} is updated each time the
				selection status changes.
				@param stamp the time stamp type 
		*/
		void stamp(StampType stamp = BOTH) throw();
			
		/**	Expand a collapsed composite.
				Only expanded composite subtrees are iterated.
				@see collapse
		*/
		void expand();

		/**	Collapse a composite.
				Collapsed composite subtrees are ignored by iterators.
				This is usefull for accelerating an iteration process.
				@see expand
		*/
		void collapse();

		/**	Insert a composite as the first child of this composite.
				Updates the modification time stamp.
				@see stamp
				@param	composite the composite to be inserted
		*/
		void prependChild(Composite& composite);

		/**	Insert a composite as the last child of this composite.
				Updates the modification time stamp.
				@see stamp
				@param	composite the composite to be inserted
		*/
		void appendChild(Composite& composite);

		/**	Insert a new parent node.
				This method is used to combine a range of nodes into a 
				single parent. First, the {\tt parent} composite is {\tt destroy}ed.
				Then, all nodes from {\tt first} through {\tt last} are inserted into
				{\tt parent} and {\tt parent} is inserted in the former position of {\tt first}.
				The method returns {\bf false}, if {\tt first} or {\tt last} have differing parents,
				if {\tt parent} is identical with either {\tt first} or {\tt last}, or if {\tt first}
				is already a descendant of {\tt parent}.\\
				
				This method updates the modification time stamp.
				@see stamp
				@param parent the new parent of the nodes from {\tt first} through {\tt last}
				@param first the first of the nodes to be inserted into {\tt parent}
				@param last the last of the nodes to be inserted into {\tt parent}
				@param destroy_parent keeps the current contents of {\tt parent} if set to {\tt true}
		*/
		static bool insertParent
			(Composite& parent, Composite& first, 
			 Composite& last, bool destroy_parent = true);

		/**	Insert a node before this node.
				This method inserts {\tt composite} before {\tt this} node,
				if {\tt this} node has a parent and is not a descendant of {\tt composite}.
				Self-insertion is recognized and ignored (nothing is done).\\

				This method updates the modification time stamp.
				@see stamp
				@param	composite the node to be inserted in the tree before {\tt this}
		*/
		void insertBefore(Composite& composite);

		/**	Insert a node after this node.
				This method inserts {\tt composite} after {\tt this} node,
				if {\tt this} node has a parent and is not a descendant of {\tt composite}.
				Self-insertion is recognized and ignored (nothing is done).\\

				This method updates the modification time stamp.
				@see stamp
				@param	composite the node to be inserted in the tree after of {\tt this}
		*/
		void insertAfter(Composite& composite);

		/**	Prepend all children of {\tt composite} to the children of this composite.
				The method does nothing, if {\tt composite} is identical to {\tt this} or
				is a descendent of {\tt this}.\\

				This method updates the modification time stamp.
				@see stamp
				@param the composite to be spliced
		*/
		void spliceBefore(Composite& composite);

		/**	Append all children of {\tt composite} to the children of this composite.
				The method does nothing, if {\tt composite} is identical to {\tt this} or
				is a descendent of {\tt this}.\\

				This method updates the modification time stamp.
				@see stamp
				@param composite the composite to be spliced
		*/
		void spliceAfter(Composite& composite);

		/**	Insert the children of composite into this composite.
				The children of {\tt composite} are inserted at the position of 
				{\tt composite} if {\tt composite} is a child of {\tt this}.
				Otherwise the children are inserted using \Ref{spliceBefore}.
				\\
				This method updates the modification time stamp.
				@see stamp
				@param composite the composite to be spliced
		*/
		void splice(Composite& composite);

		/**	Remove a child from its parent.
				{\tt child} is only removed, if it is a true child of {\tt this}.
				\\
				This method updates the modification time stamp of {\tt this}.
				@see stamp
				@param composite the composite to be spliced
				@return false if child could not be removed
		*/
		bool removeChild(Composite& child);

		/** This instance and its subtree is removed form its tree and 
				replaced by {\tt composite} and its subtree.
				This method updates the modification time stamp of 
				{\tt this} and {\tt composite}.
				@see stamp
		*/
		void replace(Composite& composite);

		/**	Swap the contents of two composites.
				This  method updates the modification time stamp of {\tt this} and {\tt composite}.
				@see stamp
		*/
		void swap(Composite& composite);

		/**	Select a composite.
				This method selects the composite and all the composites therein.
				If the state of this composite is modified, its selection time stamp
				is updated and that of its ancestors (up to and including the root composite)
				as well. The time stamps of descendants that changed their selection state 
				are update, too.
		*/	
		virtual void select();

		/**	Deselect a composite.
				This method deselects the composite and all the composites therein.
				If the state of this composite is modified, its selection time stamp
				is updated and that of its ancestors (up to and including the root composite)
				as well. The time stamps of descendants that changed their selection state 
				are update, too.
		*/	
		virtual void deselect();

		//@}
	
		/**	@name	Predicates */
		//@{
		/** Test if instance is expanded.
				@see expand
		*/
		bool isExpanded() const;
		
		/** Test if instance is collapsed.
				@see collapse
		*/
		bool isCollapsed() const;
	
		/**	Return true if the node does not contain children.
				@return bool {\bf true} if {\tt number_of_children_ == 0}
		*/
		bool isEmpty() const;

		/**	Return true if the node has no parent.
				@return bool {\bf true} if {\tt parent_ == 0}
		*/
		bool isRoot() const;
	
		/** Return true if the node is root of composite.
		*/
		bool isRootOf(const Composite& composite) const;
	
		/** Return true if the node is not the root or a leaf.
		*/
		bool isInterior() const;
	
		/** Return true if the node has a child.
		*/
		bool hasChild() const;
	
		/** Return true if the node has the parent {\tt composite}.
		*/
		bool isChildOf(const Composite& composite) const;
	
		/** Return true if the node is the first child of its parent.
		*/
		bool isFirstChild() const;
	
		/** Return true if the node is the first child of {\tt composite}.
		*/
		bool isFirstChildOf(const Composite& composite) const;
	
		/** Return true if the node is the last child of its parent.
		*/
		bool isLastChild() const;
	
		/** Return true if the node is the last child of {\tt composite}.
		*/
		bool isLastChildOf(const Composite& composite) const;
	
		/** Return true if the node has a parent.
		*/
		bool hasParent() const;

		/** Return true if the node is the parent of {\tt composite}.
		*/
		bool isParentOf(const Composite& composite) const;

		/** Return true if the node has a sibling.
				(Its parent has other childs.)
		*/
		bool hasSibling() const;
			
		/** Return true if the node is a sibling of {\tt composite}.
		*/
		bool isSiblingOf(const Composite& composite) const;
			
		/** Return true if the node has a previous sibling.
				(Its parent has a child before this.)
		*/
		bool hasPreviousSibling() const;
	
		/** Return true if the node is a previous sibling of {\tt composite}.
		*/
		bool isPreviousSiblingOf(const Composite& composite) const;
	
		/** Return true if the node has a previous sibling.
				(Its parent has a child after this.)
		*/
		bool hasNextSibling() const;

		/** Return true if the node is a next sibling of {\tt composite}.
		*/
		bool isNextSiblingOf(const Composite& composite) const;
		
		/** Return true if the node is a descendent of {\tt composite}.
		*/
		bool isDescendantOf(const Composite& composite) const;

		/** Return true if the node has a parent of the same type as dummy.
		*/
		template <typename T>
		bool hasAncestor(const T& dummy ) const; 

		/** Return true if the node has composite as descendent.
		*/
		bool isAncestorOf(const Composite& composite) const;

		/** Return true if the node has is ancestor or composite or
				composite is ancestor of this node.
		*/
		bool isRelatedWith(const Composite& composite) const;
	
		/** Return true if composite is homomorph to this node.
				(The subtrees of the two instances have to be of the same form.)
		*/
		bool isHomomorph(const Composite& composite) const;

		/**	Return true if any descendant is selected.
				This method does not check all nodes recursively. Instead, on each 
				modification of the tree, internal flags are updated and the information 
				is propagated  upwards in the tree.\\
				Complexity: O(1)\\
				@return bool {\bf true} if any node in the subtree is selected
		*/
		bool containsSelection() const;
		//@}

		/**	@name	Debugging and Diagnostics */
		//@{
		/** Test if the subtree with this node as root is valid.
				(The structure of the subtree has to be valid.)
		*/
		virtual bool isValid() const;

		/** Dump the constent of this instance to an ostream.
		*/
		virtual void dump(std::ostream& s = std::cout, Size depth = 0) const;
		//@}


		/**	@name	Application and Hosting */
		//@{

		///
		void host(Visitor<Composite>& visitor);


		/** Apply a processor to all ancestors of this node.
				@return true if the processor could be applied.
		*/
		template <typename T>
		bool applyAncestor(UnaryProcessor<T>& processor);

		/** Apply a processor to all childs of this node.
				@return true if the processor could be applied.
		*/
		template <typename T>
		bool applyChild(UnaryProcessor<T>& processor);
		
		/** Apply a processor to all descendents of this node.
				The node itself is not processed.
				The root of a subtree is accessed before the nodes in its left 
				and right subtree.
				@return true if the processor could be applied.
		*/
		template <typename T>
		bool applyDescendantPreorder(UnaryProcessor<T>& processor);

		/** Apply a processor to all descendents of this node.
				The node itself is not processed.
				The root of a subtree is accessed after the nodes in its left 
				and right subtree.
				@return true if the processor could be applied.
		*/
		template <typename T>
		bool applyDescendantPostorder(UnaryProcessor<T>& processor);
	
		/** Apply a processor to all descendents of this node.
				The node itself is not processed.
				applyDescendantPreorder is used.
				@see applyDescendantPreorder
				@return true if the processor could be applied.
		*/
		template <typename T>
		bool applyDescendant(UnaryProcessor<T>& processor);
		
		/** Apply a processor to the node and its subtree.
				The root of a subtree is accessed before the nodes in its left 
				and right subtree.
				@return true if the processor could be applied.
		*/
		template <typename T>
		bool applyPreorder(UnaryProcessor<T>& processor);
		
		/** Apply a processor to the node and its subtree.
				The root of a subtree is accessed after the nodes in its left 
				and right subtree.
				@return true if the processor could be applied.
		*/
		template <typename T>
		bool applyPostorder(UnaryProcessor<T>& processor);

		/** Apply a processor to the node and its subtree.
				applyPreorder is used.
				@see applyPreorder
				@return true if the processor could be applied.
		*/
		template <typename T>
		bool apply(UnaryProcessor<T>& processor);
		
		/** Apply a processor to the node and its siblings.
				@return true if the processor could be applied.
		*/
		template <typename T>
		bool applyLevel(UnaryProcessor<T>& processor, long level);
		//@}			


	
		class AncestorIteratorTraits_
		{
			public:

			BALL_CREATE(AncestorIteratorTraits_)

			AncestorIteratorTraits_()
				:	bound_(0),
					ancestor_(0)
			{
			}
		
			AncestorIteratorTraits_(const Composite& composite)
				:	bound_((Composite *)&composite),
					ancestor_(0)
			{
			}
		
			AncestorIteratorTraits_(const AncestorIteratorTraits_& traits, bool /* deep */ = true)
				:	bound_(traits.bound_),
					ancestor_(traits.ancestor_)
			{
			}
		
			AncestorIteratorTraits_& operator = (const AncestorIteratorTraits_& traits)
			{
				bound_ = traits.bound_;
				ancestor_ = traits.ancestor_;
				return *this;
			}

			Composite* getContainer()
			{
				return bound_;
			}

			const Composite* getContainer() const
			{
				return bound_;
			}

			bool isSingular() const
			{
				return (bound_ == 0);
			}

			Composite*& getPosition()
			{
				return ancestor_;
			}

			Composite* const& getPosition() const
			{
				return ancestor_;
			}

			bool operator == (const AncestorIteratorTraits_& traits) const
			{
				return (ancestor_ == traits.ancestor_);
			}
		
			bool operator != (const AncestorIteratorTraits_& traits) const
			{
				return (ancestor_ != traits.ancestor_);
			}
		
			bool isValid() const
			{
				return (bound_ != 0 && ancestor_ != 0);
			}

			void invalidate()
			{
				bound_ 	= ancestor_ = 0; 
			}

			void toBegin()
			{
				ancestor_ = bound_->parent_;
			}

			bool isBegin() const
			{
				return (ancestor_ == bound_->parent_);
			}

			void toEnd()
			{
				ancestor_ = 0;
			}

			bool isEnd() const
			{
				return (ancestor_ == 0);
			}

			Composite& getData()
			{
				return *ancestor_;
			}

			const Composite& getData() const
			{
				return *ancestor_;
			}

			void forward()
			{
				ancestor_ = ancestor_->parent_;
			}


			private:

			Composite* bound_;
			Composite* ancestor_;
		};

		friend class AncestorIteratorTraits_;

		typedef ForwardIterator <Composite, Composite, Composite *, AncestorIteratorTraits_>
			AncestorIterator;

		AncestorIterator beginAncestor()
		{
			return AncestorIterator::begin(*this);
		}

		AncestorIterator endAncestor()
		{
			return AncestorIterator::end(*this);
		}



		typedef ConstForwardIterator<Composite, Composite, Composite *, AncestorIteratorTraits_>
			AncestorConstIterator;

		AncestorConstIterator beginAncestor() const
		{
			return AncestorConstIterator::begin(*this);
		}

		AncestorConstIterator endAncestor() const
		{
			return AncestorConstIterator::end(*this);
		}



		class ChildCompositeIteratorTraits_
		{
			public:

			BALL_CREATE(ChildCompositeIteratorTraits_)

			ChildCompositeIteratorTraits_()
				:	bound_(0),
					child_(0)
			{
			}
			
			ChildCompositeIteratorTraits_(const Composite& composite)
				:	bound_((Composite *)&composite),
					child_(0)
			{
			}
		
			ChildCompositeIteratorTraits_(const ChildCompositeIteratorTraits_& traits, bool /* deep */ = true)
				:	bound_(traits.bound_),
					child_(traits.child_)
			{
			}
		
			ChildCompositeIteratorTraits_& operator = (const ChildCompositeIteratorTraits_& traits)
			{
				bound_ = traits.bound_;
				child_ = traits.child_;
				return *this;
			}

			Composite *getContainer()
			{
				return bound_;
			}

			const Composite *getContainer() const
			{
				return bound_;
			}

			bool isSingular() const
			{
				return (bound_ == 0);
			}

			Composite *&getPosition()
			{
				return child_;
			}

			Composite *const& getPosition() const
			{
				return child_;
			}

			bool operator == (const ChildCompositeIteratorTraits_& traits) const
			{
				return (child_ == traits.child_);
			}
		
			bool operator != (const ChildCompositeIteratorTraits_& traits) const
			{
				return (child_ != traits.child_);
			}
		
			bool isValid() const
			{
				return (bound_ != 0 && child_ != 0);
			}

			void invalidate()
			{
				bound_ = child_ = 0; 
			}

			void toBegin()
			{
				child_ = bound_->first_child_;
			}

			bool isBegin() const
			{
				return (child_ == bound_->first_child_);
			}

			void toEnd()
			{
				child_ = 0;
			}

			bool isEnd() const
			{
				return (child_ == 0);
			}

			void toRBegin
				()
			{
				child_ = bound_->last_child_;
			}

			bool isRBegin() const
			{
				return (child_ == bound_->last_child_);
			}

			void toREnd()
			{
				child_ = 0;
			}

			bool isREnd() const
			{
				return (child_ == 0);
			}

			Composite& getData()
			{
				return *child_;
			}

			const Composite& getData() const
			{
				return *child_;
			}

			void forward()
			{
				child_ = child_->next_;
			}

			void backward()
			{
				child_ = child_->previous_;
			}

			private:

			Composite *bound_;
			Composite *child_;
		};

		friend class ChildCompositeIteratorTraits_;

		typedef BidirectionalIterator<Composite, Composite, Composite *, ChildCompositeIteratorTraits_>
			ChildCompositeIterator;

		ChildCompositeIterator beginChildComposite()
		{
			return ChildCompositeIterator::begin(*this);
		}

		ChildCompositeIterator endChildComposite()
		{
			return ChildCompositeIterator::end(*this);
		}



		typedef ConstBidirectionalIterator<Composite, Composite, Composite *, ChildCompositeIteratorTraits_>
			ChildCompositeConstIterator;

		ChildCompositeConstIterator beginChildComposite() const
		{
			return ChildCompositeConstIterator::begin(*this);
		}

		ChildCompositeConstIterator endChildComposite() const
		{
			return ChildCompositeConstIterator::end(*this);
		}



		typedef ReverseBidirectionalIterator<Composite, Composite, Composite *, ChildCompositeIteratorTraits_>
			ChildCompositeReverseIterator;

		ChildCompositeReverseIterator rbeginChildComposite()
		{
			return ChildCompositeReverseIterator::begin(*this);
		}

		ChildCompositeReverseIterator rendChildComposite()
		{
			return ChildCompositeReverseIterator::end(*this);
		}



		typedef ConstReverseBidirectionalIterator<Composite, Composite, Composite *, ChildCompositeIteratorTraits_>
			ChildCompositeConstReverseIterator;

		ChildCompositeConstReverseIterator rbeginChildComposite() const
		{
			return ChildCompositeConstReverseIterator::begin(*this);
		}

		ChildCompositeConstReverseIterator rendChildComposite() const
		{
			return ChildCompositeConstReverseIterator::end(*this);
		}



		class CompositeIteratorPosition_
		{
			friend class Composite;

			public:

			CompositeIteratorPosition_()
				:	empty_stack_(0),
					stack_(0),
					current_(0),
					continue_(false),
					traversing_forward_(true)
			{
			}

			CompositeIteratorPosition_(const CompositeIteratorPosition_& position)
				:	empty_stack_(position.empty_stack_),
					stack_(position.stack_),
					current_(position.current_),
					continue_(position.continue_),
					traversing_forward_(position.traversing_forward_)
			{
			}

			Composite* getCurrent()
			{
				return current_;
			}
		
			const Composite* getCurrent() const
			{
				return current_;
			}

			void clear()
			{
				empty_stack_ = stack_ = current_ = 0;
	
				continue_ = false;
				traversing_forward_ = true;
			}

			CompositeIteratorPosition_& operator = (const CompositeIteratorPosition_& position)
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

			bool operator == (const Composite::CompositeIteratorPosition_& position) const
			{
				return (current_ == position.current_);
			}

			bool operator != (const Composite::CompositeIteratorPosition_& position) const
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



		class CompositeIteratorTraits_
		{
			public:

			BALL_CREATE(CompositeIteratorTraits_)

			CompositeIteratorTraits_()
				:	bound_(0),
					position_()
			{
			}
		
			CompositeIteratorTraits_(const Composite& composite)
				:	bound_((Composite *)&composite),
					position_()
			{
			}
		
			CompositeIteratorTraits_(const CompositeIteratorTraits_& traits, bool /* deep */ = true)
				:	bound_(traits.bound_),
					position_(traits.position_)
			{
			}
		
			bool isValid() const
			{
				return (bound_ != 0	&& position_.getCurrent() != 0);
			}

			CompositeIteratorTraits_& operator = (const CompositeIteratorTraits_& traits)
			{
				bound_ = traits.bound_;
				position_ = traits.position_;
				return *this;
			}

			Composite* getContainer()
			{
				return bound_;
			}

			const Composite* getContainer() const
			{
				return bound_;
			}
		
			bool isSingular() const
			{
				return (bound_ == 0);
			}
		
			CompositeIteratorPosition_& getPosition()
			{
				return position_;
			}

			const CompositeIteratorPosition_& getPosition() const
			{
				return position_;
			}

			Composite& getData()
			{
				return *(position_.getCurrent());
			}

			const Composite& getData() const
			{
				return *(position_.getCurrent());
			}

			bool operator == (const CompositeIteratorTraits_& traits) const
			{
				return (position_	== traits.position_);
			}
		
			bool operator != (const CompositeIteratorTraits_& traits) const
			{
				return (position_ != traits.position_);
			}
		
			void invalidate()
			{
				bound_ = 0;
				position_.clear();
			}

			void toBegin()
			{
				Composite::setCurrentPreorderIteratorPosition_
					(Composite::getFirstPreorderIteratorPosition_(*bound_), position_, false);
			}

			bool isBegin() const
			{
				return (position_.getCurrent() == &Composite::getFirstPreorderIteratorPosition_(*bound_));
			}

			void toEnd()
			{
				toRBegin();
				forward();
			}

			bool isEnd() const
			{
				return (position_.getCurrent() == 0);
			}

			void toRBegin()
			{
				Composite::setLastPreorderIteratorPosition_(*bound_, position_,false);
			}

			bool isRBegin() const
			{
				return (position_.getCurrent() == &Composite::getLastPreorderIteratorPosition_(*bound_)); 
			}
		
			void toREnd()
			{
				Composite::setCurrentPreorderIteratorPosition_(*bound_, position_, false);
				backward();	
			}

			bool isREnd() const
			{
				return (position_.getCurrent() == 0);
			}
		
			void forward()
			{
				bound_->getNextPreorderIteratorPosition_(position_);
			}

			void backward()
			{
				bound_->getPreviousPreorderIteratorPosition_(position_);
			}


			protected:

			/** A pointer to the "container" the iterator is bound to
			*/
			Composite*									bound_;

			/** The current iterator position
			*/
			CompositeIteratorPosition_	position_;
		};

		friend class CompositeIteratorTraits_;

		typedef BidirectionalIterator<Composite, Composite, CompositeIteratorPosition_, CompositeIteratorTraits_>
			CompositeIterator;

		CompositeIterator beginComposite()
		{
			return CompositeIterator::begin(*this);
		}

		CompositeIterator endComposite()
		{
			return CompositeIterator::end(*this);
		}



		typedef ConstBidirectionalIterator<Composite, Composite, CompositeIteratorPosition_, CompositeIteratorTraits_>
			CompositeConstIterator;

		CompositeConstIterator beginComposite() const
		{
			return CompositeConstIterator::begin(*this);
		}

		CompositeConstIterator endComposite() const
		{
			return CompositeConstIterator::end(*this);
		}



		typedef ReverseBidirectionalIterator<Composite, Composite, CompositeIteratorPosition_, CompositeIteratorTraits_>
			CompositeReverseIterator;

		CompositeReverseIterator rbeginComposite()
		{
			return CompositeReverseIterator::begin(*this);
		}

		CompositeReverseIterator rendComposite()
		{
			return CompositeReverseIterator::end(*this);
		}



		typedef ConstReverseBidirectionalIterator<Composite, Composite, CompositeIteratorPosition_,CompositeIteratorTraits_>
			CompositeConstReverseIterator;

		CompositeConstReverseIterator rbeginComposite() const
		{
			return CompositeConstReverseIterator::begin(*this);
		}

		CompositeConstReverseIterator rendComposite() const
		{
			return CompositeConstReverseIterator::end(*this);
		}



		class SubcompositeIteratorTraits_
			: public CompositeIteratorTraits_
		{
			public:

			BALL_CREATE(SubcompositeIteratorTraits_)

			SubcompositeIteratorTraits_()
				: CompositeIteratorTraits_()
			{
			}
		
			SubcompositeIteratorTraits_(const Composite& composite)
				:	CompositeIteratorTraits_(composite)
			{
			}
		
			SubcompositeIteratorTraits_(const SubcompositeIteratorTraits_& traits, bool /* deep */ = true)
				:	CompositeIteratorTraits_(traits)
			{
			}

			void toBegin()
			{
				Composite::setCurrentPreorderIteratorPosition_
					(Composite::getFirstPreorderIteratorPosition_(*bound_), position_, true);
			}

			void toEnd()
			{
				toRBegin();
				forward();
			}

			void toRBegin()
			{
				Composite::setLastPreorderIteratorPosition_(*bound_, position_, true);
			}

			void toREnd()
			{
				Composite::setCurrentPreorderIteratorPosition_(*bound_, position_, true);
				backward();
			}
		};

		friend class SubcompositeIteratorTraits_;

		typedef BidirectionalIterator<Composite, Composite, CompositeIteratorPosition_, SubcompositeIteratorTraits_>
			SubcompositeIterator;

		SubcompositeIterator beginSubcomposite()
		{
			return SubcompositeIterator::begin(*this);
		}

		SubcompositeIterator endSubcomposite()
		{
			return SubcompositeIterator::end(*this);
		}



		typedef ConstBidirectionalIterator<Composite, Composite, CompositeIteratorPosition_, SubcompositeIteratorTraits_>
			SubcompositeConstIterator;

		SubcompositeConstIterator beginSubcomposite() const
		{
			return SubcompositeConstIterator::begin(*this);
		}

		SubcompositeConstIterator endSubcomposite() const
		{
			return SubcompositeConstIterator::end(*this);
		}



		typedef ReverseBidirectionalIterator<Composite, Composite, CompositeIteratorPosition_, SubcompositeIteratorTraits_>
			SubcompositeReverseIterator;

		SubcompositeReverseIterator rbeginSubcomposite()
		{
			return SubcompositeReverseIterator::begin(*this);
		}
			

		SubcompositeReverseIterator rendSubcomposite()
		{
			return SubcompositeReverseIterator::end(*this);
		}



		typedef ConstReverseBidirectionalIterator<Composite, Composite, CompositeIteratorPosition_, SubcompositeIteratorTraits_>
			SubcompositeConstReverseIterator;

		SubcompositeConstReverseIterator rbeginSubcomposite() const
		{
			return SubcompositeConstReverseIterator::begin(*this);
		}

		SubcompositeConstReverseIterator rendSubcomposite() const
		{
			return SubcompositeConstReverseIterator::end(*this);
		}

		private:
		
		enum Property_
		{
			PROPERTY__COLLAPSED_ = 1
		};

		Size getHeight_(Size size, Size& max_height) const;
	
		Size countDescendants_() const;

		void clone_(Composite& parent, Composite& stack, KernelPredicateType& predicate) const;

		// traverse forward, valid for composites and subcomposites
		static Composite* setCurrentPreorderIteratorPosition_
			(Composite& composite, CompositeIteratorPosition_& position, bool subcomposite);
		
		// get first iterator, valid for composites and subcomposites
		static Composite& getFirstPreorderIteratorPosition_(Composite& composite);
	
		// get last iterator, valid for composites only
		static Composite& getLastPreorderIteratorPosition_(Composite& composite);
		
		// set last iterator, valid for composites and subcomposites
		static Composite& setLastPreorderIteratorPosition_
			(Composite& composite,
			 CompositeIteratorPosition_& position,
			 bool subcomposite);
		
		// get next iterator, valid for composites and subcomposites
		Composite* getNextPreorderIteratorPosition_(CompositeIteratorPosition_& position);

		// get previous iterator, valid for composites and subcomposites
		Composite* getPreviousPreorderIteratorPosition_(CompositeIteratorPosition_& position);

		static Composite* setCurrentPreorderForward_
			(Composite& composite, CompositeIteratorPosition_& position, bool subcomposite);

		static Composite* setCurrentPreorderBackward_
			(Composite& composite, CompositeIteratorPosition_& position, bool subcomposite);

		template <typename T>
		bool applyLevelNostart_(UnaryProcessor<T>& processor, long level);

		template <typename T>
		bool applyChildNostart_(UnaryProcessor<T>& processor);

		template <typename T>
		bool applyPreorderNostart_(UnaryProcessor<T>& processor);

		template <typename T>
		bool applyDescendantPreorderNostart_(UnaryProcessor<T>& processor);

		template <typename T>
		bool applyDescendantPostorderNostart_(UnaryProcessor<T>& processor);


		void updateSelection_();
		void determineSelection_();
		void select_(bool update_parent = true);
		void deselect_(bool update_parent = true);

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
  {
    return processor.start() && applyChildNostart_(processor) && processor.finish();
	}

	template <typename T>
  bool Composite::applyChildNostart_(UnaryProcessor<T>& processor)
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
  {
    return processor.start() && applyDescendantPreorderNostart_(processor) && processor.finish();
	}

	template <typename T>
  bool Composite::applyDescendantPreorderNostart_(UnaryProcessor<T>& processor)
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
  {
    return processor.start() && applyDescendantPostorderNostart_(processor) && processor.finish();
	}

	template <typename T>
  bool Composite::applyDescendantPostorderNostart_(UnaryProcessor<T>& processor)
  {
    if (isCollapsed() == true)
    {
      return true;
		}

    Processor::Result result;

    for (Composite* composite = first_child_;
         composite != 0; composite = composite->next_)
    {
      if (composite->first_child_ != 0 && composite->applyDescendantPostorderNostart_(processor) == false)
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
	{
		bool return_value = processor.start() && applyDescendantPostorderNostart_(processor);

		T* t_ptr = dynamic_cast<T*>(this);
		if (t_ptr != 0)
		{
			return_value &= (processor(*t_ptr) < Processor::BREAK) && processor.finish();
		} 
		
		return return_value;
	}

	template <typename T>
  bool Composite::applyLevel(UnaryProcessor<T>& processor, long level)
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
    else if (--level == 0)
    {
      return applyChildNostart_(processor);
		}
    else if (level > 0)
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
			} else {
				return_value =  applyDescendantPreorderNostart_(processor);
			}
		} else {
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
	bool Composite::applyPreorder(UnaryProcessor<T>& processor)
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

	template <class T>
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
		SubcompositeReverseIterator it(getRoot().rbeginSubcomposite());

		// set its position to the current composite
		Composite::setCurrentPreorderIteratorPosition_
			(Composite::getFirstPreorderIteratorPosition_(*this), it.getTraits().getPosition(), false);


		// walk back until we find something	
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
		SubcompositeIterator it(getRoot().beginSubcomposite());

		// set its position to the current composite
		Composite::setCurrentPreorderIteratorPosition_
			(Composite::getFirstPreorderIteratorPosition_(*this), it.getTraits().getPosition(), false);


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
