// $Id: composite.h,v 1.2 1999/09/06 22:20:43 oliver Exp $

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

namespace BALL 
{
	/**	Composite Class.
			This class implements a variant of the composite design pattern.
			A Composite may contain an arbitrary number of other composites, thus forming
			a tree. All BALL kernel classes are derived from Composite. This provides a unique
			interface for all kernel classes.\\
	
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

		/**	@name	Type Definitions
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
		//@}
				
		BALL_CREATE(Composite)

		static UnaryProcessor<Composite> DEFAULT_PROCESSOR;
		static UnaryPredicate<Composite> DEFAULT_UNARY_PREDICATE;
		
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
				It does not remove the composite from any parental structure.
				@see	AutoDeletable
				@see	destroy
		*/
		virtual void clear();
	
		/**	Destroy the composite.
				This method removes the composite from potential parental structures and
				then calls \Ref{clear} to destruct all children.
				@see	~Composite
				@see	clear
		*/
		virtual void destroy();

		/**	Non-virtual destroy method.
				This method behaves exactly like destroy except for a small difference:
				when called with {\bf true}, it calls the {\em virtual} clear function.
				If called with false it calls the original clear function of Composite.
				This is useful when implementing the behaviour of derived classes.
				@param	virtual_destroy call the virtual clear method ({\bf true}) or
								{\tt Composite::clear()} ({\bf false})
		*/		
		void destroy(bool virtual_destroy);

		/**	Clone with a predicate.
				This method copies the attributes of {\tt this} composite to root (shallow copy) and
				then adds recursively each composite of the tree that fulfils the {\tt predicate}.
				@param	root the cloning target root is {\tt destroy}ed prior to any copying 
				@param	predicate the predicate
				@param  a pointer to the root composite ({\tt \&root})
		*/
		void* clone(Composite& root, UnaryPredicate<Composite>& predicate) const;

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

		/**	Predicative assign a tree.
		*/
		void set
			(const Composite& composite,
			 UnaryPredicate<Composite>& predicate);

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
		void get
			(Composite& composite,
			 UnaryPredicate<Composite>& predicate) const;

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

		/**
		*/
		static Size getPathLength(const Composite& a, const Composite& b);

		/**
		*/
		static Size getDepth(const Composite& composite);

		/**
		*/
		Size getDepth() const;

		/**
		*/
		static Size getHeight(const Composite& composite);

		/**
		*/
		Size getHeight() const;

		/**
		*/
		Composite& getRoot();

		/**
		*/
		const Composite& getRoot() const;

		/**
		*/
		Composite* getLowestCommonAncestor(Composite& composite);

		/**
		*/
		const Composite* getLowestCommonAncestor(const Composite& composite) const;

		/**	Find the first ancestor of type T.
				This method walks up the tree from parent to paretn and
				checks whether the composite object is a kind of {\tt T}.
				This method is useful to identify special container classes.
				@param	t an (usually default) instance of a certain class type
				@return a pointer to the first composite found that is a kind of T or 0 if no
								matching composite was found to the root of the tree
		*/
		template <class T>
		Composite* getAncestor(const T& /* t */)
		{
			
			for (Composite* composite_ptr = parent_;
					 composite_ptr != 0; composite_ptr = composite_ptr->parent_)
			{
				if (RTTI<T>::isKindOf(*composite_ptr))
					return composite_ptr;
			}
			
			return 0;
		}

		/**	Find the first ancestor of type T (const method).
				This method operates also on constant trees.
				@param	t an (usually default) instance of a certain class type
				@return a pointer to the first composite found that is a kind of T or 0 if no
								matching composite was found to the root of the tree
		*/
		template <class T>
		const Composite* getAncestor(const T& t) const
		{
			return (const_cast<Composite *>(this))->getAncestor(t);
		}

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
		*/
		Composite* getSibling(Index index);

		/**	Return a const pointer to the sibling index positions from this composite.
				A pointer to the sibling {\tt index} positions to the right (for
				positive values of {\tt index}) or {\tt -index} positions to the left 
				(for negative values of {\tt index}) is returned.
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

		/**	Expand a collapsed composite.
		*/
		void expand();

		/**	Collapse a composite.
		*/
		void collapse();

		/**	Insert a composite as the first child of this composite.
				@param	composite the composite to be inserted
		*/
		void prependChild(Composite& composite);

		/**	Insert a composite as the last child of this composite.
				@param	composite the composite to be inserted
		*/
		void appendChild(Composite& composite);

		/**	Insert a new parent node.
				This method is used to combine a range of nodes into a 
				single parent. First, the {\tt parent} composite is {\tt destroy}ed.
				Then, all nodes from {\tt first} through {\tt last} are inserted into
				{\tt parent} and {\tt parent} is inserted in the former position of {\tt first}.
		*/
		static bool insertParent
			(Composite& parent, Composite& first, 
			 Composite& last, bool destroy_parent = true);

		/**	Insert a node before this node.
				This method inserts {\tt composite} before {\tt this} node,
				if {\tt this} node has a parent and is not a descendant of {\tt composite}.
				Self-insertion is recognized and ignored (nothing is done).
				@param	composite the node to be inserted in the tree before {\tt this}
		*/
		void insertBefore(Composite& composite);

		/**	Insert a node after this node.
				This method inserts {\tt composite} after {\tt this} node,
				if {\tt this} node has a parent and is not a descendant of {\tt composite}.
				Self-insertion is recognized and ignored (nothing is done).
				@param	composite the node to be inserted in the tree after of {\tt this}
		*/
		void insertAfter(Composite& composite);

		/**	Prepend all children of {\tt composite} to the children of this composite.
		*/
		void spliceBefore(Composite& composite);

		/**	Append all children of {\tt composite} to the children of this composite.
		*/
		void spliceAfter(Composite& composite);

		/**	Insert the children of composite into this composite.
				The children of {\tt composite} are inserted at the position of 
				{\tt composite} if {\tt composite} is a child of {\tt this}.
				Otherwise the children are inserted using \Ref{spliceBefore}.
		*/
		void splice(Composite& composite);

		/**	Remove a child from a composite.
				This method removes a child from {\tt this} composite.
				The child is only removed, if the following preconditions hold:
				\begin{itemize}
					\item {\tt child != this}
					\item {\tt child} has a parent
					\item {\tt this} is not a descendant of {\tt child}
				@return false if composite could not be removed
		*/
		bool removeChild(Composite& child);

		///
		void replace(Composite& composite);

		///
		void swap(Composite& composite);

		/**	Select a composite.
				This method selects the composite and all the composites therein.
		*/	
		virtual void select();

		/**	Deselect a composite.
				This method deselects the composite and all the composites therein.
		*/	
		virtual void deselect();
		//@}
	
		/**	@name	Predicates */
		//@{
		///
		bool isExpanded() const;
		
		///
		bool isCollapsed() const;
	
		///
		bool isEmpty() const;

		///
		bool isFree() const;

		///
		bool isRoot() const;
	
		///
		bool isRootOf(const Composite& composite) const;
	
		///
		bool isInterior() const;
	
		///
		bool isInteriorOf(const Composite& composite) const;

		///
		bool isLeaf() const;

		///
		bool isLeafOf(const Composite& composite) const;

		///
		bool hasChild() const;

		///
		bool isChild() const;
	
		///
		bool isChildOf(const Composite& composite) const;
	
		///
		bool isFirstChild() const;
	
		///
		bool isFirstChildOf(const Composite& composite) const;
	
		///
		bool isLastChild() const;
	
		///
		bool isLastChildOf(const Composite& composite) const;
	
		///
		bool hasParent() const;

		///
		bool isParent() const;

		///
		bool isParentOf(const Composite& composite) const;

		///
		bool hasSibling() const;
			
		///
		bool isSibling() const;
			
		///
		bool isSiblingOf(const Composite& composite) const;
			
		///
		bool hasPreviousSibling() const;
	
		///
		bool isPreviousSibling() const;
	
		///
		bool isPreviousSiblingOf(const Composite& composite) const;
	
		///
		bool hasNextSibling() const;

		///
		bool isNextSibling() const;

		///
		bool isNextSiblingOf(const Composite& composite) const;
	
		///
		bool hasPreceedingSibling() const;

		///
		bool isPreceedingSibling() const;

		///
		bool isPreceedingSiblingOf(const Composite& composite) const;
	
		///
		bool hasSucceedingSibling() const;

		///
		bool isSucceedingSibling() const;

		///
		bool isSucceedingSiblingOf(const Composite& composite) const;

		///
		bool hasDescendant() const;
	
		///
		bool isDescendant() const;
	
		///
		bool isDescendantOf(const Composite& composite) const;

		///
		bool hasAnyAncestor() const;

		///
		template <class T>
		bool hasAncestor(const T& t) const 
		{
			return (getAncestor(t) != 0);	
		}

		///
		bool isAncestor() const;

		///
		bool isAncestorOf(const Composite& composite) const
		{
			return composite.isDescendantOf(*this);
		}

		///
		bool isRelatedWith(const Composite& composite) const;
	
		///
		bool isHomomorph(const Composite& composite) const;

		/**	Return true if any descendant is selected.
				Complexity: O(1)\\
				This method does not check all nodes recursively. Instead, on each 
				modification in the tree internal flags are updated and propagated 
				upwards in the tree.
				@return {\bf true} if any node in the subtree is selected
		*/
		bool containsSelection() const;
		//@}

		/**	@name	Debugging and Diagnostics */
		//@{
		///
		virtual bool isValid() const;

		///
		virtual void dump(ostream& s = cout, unsigned long depth = 0) const;
		//@}


		/**	@name	Application and Hosting */
		//@{
		///
		void host(Visitor<Composite>& visitor);


		///
		bool applyAncestor(UnaryProcessor<Composite>& processor);

		///
		bool applyChild(UnaryProcessor<Composite>& processor);
		
		///
		bool applyDescendantPreorder(UnaryProcessor<Composite>& processor);

		///
		bool applyDescendantPostorder(UnaryProcessor<Composite>& processor);
	
		///
		bool applyDescendant(UnaryProcessor<Composite>& processor);
		
		///
		bool applyPreorder(UnaryProcessor<Composite>& processor);
		
		///
		bool applyPostorder(UnaryProcessor<Composite>& processor);

		///
		bool apply(UnaryProcessor<Composite>& processor);
		
		///
		bool applyLevel(UnaryProcessor<Composite>& processor, long level);
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
				return (bool)(bound_ == 0);
			}

			Composite *&getPosition()
			{
				return ancestor_;
			}

			Composite *const& getPosition() const
			{
				return ancestor_;
			}

			bool operator == (const AncestorIteratorTraits_& traits) const
			{
				return (bool)(ancestor_ == traits.ancestor_);
			}
		
			bool operator !=(const AncestorIteratorTraits_& traits) const
			{
				return (bool)(ancestor_ != traits.ancestor_);
			}
		
			bool isValid() const
			{
				return (bool)(bound_ != 0 && ancestor_ != 0);
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
				return (bool)(ancestor_ == bound_->parent_);
			}

			void toEnd()
			{
				ancestor_ = 0;
			}

			bool isEnd() const
			{
				return (bool)(ancestor_ == 0);
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
				return (bool)(bound_ == 0);
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
				return (bool)(child_ == traits.child_);
			}
		
			bool operator != (const ChildCompositeIteratorTraits_& traits) const
			{
				return (bool)(child_ != traits.child_);
			}
		
			bool isValid() const
			{
				return (bool)(bound_ != 0 && child_ != 0);
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
				return (bool)(child_ == bound_->first_child_);
			}

			void toEnd()
			{
				child_ = 0;
			}

			bool isEnd() const
			{
				return (bool)(child_ == 0);
			}

			void toRBegin
				()
			{
				child_ = bound_->last_child_;
			}

			bool isRBegin() const
			{
				return (bool)(child_ == bound_->last_child_);
			}

			void toREnd()
			{
				child_ = 0;
			}

			bool isREnd() const
			{
				return (bool)(child_ == 0);
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

				CompositeIteratorPosition_(const Composite::CompositeIteratorPosition_& position)
					:	empty_stack_(position.empty_stack_),
						stack_(position.stack_),
						current_(position.current_),
						continue_(position.continue_),
						traversing_forward_(position.traversing_forward_)
				{
				}

				Composite *getCurrent()
				{
					return current_;
				}
			
				const Composite *getCurrent() const
				{
					return current_;
				}

				void clear()
				{
					empty_stack_ = stack_ = current_ = 0;
		
					continue_ = false;
					traversing_forward_ = true;
				}

				CompositeIteratorPosition_& operator = (const Composite::CompositeIteratorPosition_& position)
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
					return (bool)(current_ == position.current_);
				}

				bool operator != (const Composite::CompositeIteratorPosition_& position) const
				{
					return (bool)(current_ != position.current_);
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
					return (bool)(bound_ != 0	&& position_.getCurrent() != 0);
				}

				CompositeIteratorTraits_& operator = (const CompositeIteratorTraits_& traits)
				{
					bound_ = traits.bound_;
					position_ = traits.position_;
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
					return (bool)(bound_ == 0);
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
					return (bool)(position_	== traits.position_);
				}
			
				bool operator != (const CompositeIteratorTraits_& traits) const
				{
					return (bool)(position_ != traits.position_);
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
					return (bool)(position_.getCurrent() == &Composite::getFirstPreorderIteratorPosition_(*bound_));
				}

				void toEnd()
				{
					toRBegin();
					forward();
				}

				bool isEnd() const
				{
					return (bool)(position_.getCurrent() == 0);
				}

				void toRBegin()
				{
					Composite::setLastPreorderIteratorPosition_(*bound_, position_,false);
				}

				bool isRBegin() const
				{
					return (bool)(position_.getCurrent() == &Composite::getLastPreorderIteratorPosition_(*bound_)); 
				}
			
				void toREnd()
				{
					Composite::setCurrentPreorderIteratorPosition_(*bound_, position_, false);
					backward();	
				}

				bool isREnd() const
				{
					return (bool)(position_.getCurrent() == 0);
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

				Composite *bound_;

				CompositeIteratorPosition_ position_;

			private:
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

		void clone_(Composite& parent, Composite& stack, UnaryPredicate<Composite>& predicate) const;

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

		bool applyLevelNostart_(UnaryProcessor<Composite>& processor, long level);
		bool applyChildNostart_(UnaryProcessor<Composite>& processor);
		bool applyPreorderNostart_(UnaryProcessor<Composite>& processor);
		bool applyDescendantPreorderNostart_(UnaryProcessor<Composite>& processor);
		bool applyDescendantPostorderNostart_(UnaryProcessor<Composite>& processor);


		void updateSelection_();
		void determineSelection_();
		void select_(bool update_parent = true);
		void deselect_(bool update_parent = true);

		// private attributes
		
		Size 										number_of_children_;
		Composite*							parent_;
		Composite* 							previous_;
		Composite* 							next_;
		Composite* 							first_child_;
		Composite* 							last_child_;
		unsigned char						properties_;
		bool										contains_selection_;
		Size										number_of_selected_children_;
		Size										number_of_children_containing_selection_;
	};

#	ifndef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/CONCEPT/composite.iC>
#	endif

} // namespace BALL

#endif // BALL_CONCEPT_COMPOSITE_H
