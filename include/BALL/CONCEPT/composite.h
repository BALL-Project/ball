// $Id: composite.h,v 1.1 1999/08/26 07:53:16 oliver Exp $

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

#ifndef BALL_CONCEPT_VISITOR_H
#	include <BALL/CONCEPT/visitor.h>
#endif

#ifndef BALL_CONCEPT_PROCESSOR_H
#	include <BALL/CONCEPT/processor.h>
#endif

namespace BALL {

	class Aggregate;
	class Composite;

	/**
	*/
	class AggregateCompositeItem
		: public PersistentObject,
			public Object
			
	{
			friend class Aggregate;
			friend class Composite;

		public:

			/**	@name Constructors and Destructors */
			//@{
			
			/** Default constructor.
					Creates an empty AggregateCompositeItem
			*/
			AggregateCompositeItem();

			/**	Destructor.
					Destructs the AggregateCompositeItem
			*/
			virtual ~AggregateCompositeItem();
			//@}

			/**	@name	Persistence */
			//@{
			
			/** Writes a persistent copy of the object.
					@param	pm the persistence manager to write to
					@param	name the name of the obejct to be written ("-" in the case of a baseclass)
					@param	is_baseclass a flag indicating that a baseclass is written
			*/
		  virtual void persistentWrite(PersistenceManager& pm, const char* name = 0) const;

			/** Reads a persistent object.
					@param	pm the persistence manager to write to
			*/
			virtual void persistentRead(PersistenceManager& pm);
			//@}


			Handle getHandle() const;

			Aggregate& getAggregate();

			const Aggregate& getAggregate() const;

			Composite& getComposite();

			const Composite& getComposite() const;

			AggregateCompositeItem* getNextAggregate();

			const AggregateCompositeItem* getNextAggregate() const;

			AggregateCompositeItem* getPreviousAggregate();

			const AggregateCompositeItem* getPreviousAggregate() const;

			AggregateCompositeItem* getNextComposite();

			const AggregateCompositeItem* getNextComposite() const;

			AggregateCompositeItem* getPreviousComposite();

			const AggregateCompositeItem* getPreviousComposite() const;

			virtual void clear();
		
			virtual void destroy();
		
			virtual void dump(ostream& s = cout, unsigned long depth = 0) const;

		protected:

			AggregateCompositeItem
				(Aggregate& aggregate, Composite& composite,
				 AggregateCompositeItem* aggregate_next = 0,
				 AggregateCompositeItem* composite_next = 0);

		public:
		
			Aggregate* aggregate_;
			Composite* composite_;

			AggregateCompositeItem *aggregate_next_;
			AggregateCompositeItem *composite_next_;

			AggregateCompositeItem *aggregate_previous_;
			AggregateCompositeItem *composite_previous_;

			Handle handle_;
	};

	/**	Composite Class.
			This class implements a variant of the composite design pattern.
			A Composite may contain an arbitrary number of other composites, thus forming
			a tree. All BALL kernel classes are derived from Composite. This provides a unique
			interface for all kernel classes.
	
			Composites are persistent objects
			\\
			{\bf Definition:} \URL{BALL/CONCEPT/composite.h}
	*/
	class Composite
		: public PersistentObject,
			public Object
	{
			friend class Aggregate;
			friend class AggregateCompositeItem;

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
					This method behaves exactly like dsetroy except for a small difference:
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

			/**
			*/
			void set
				(const Composite& composite,
				 UnaryPredicate<Composite>& predicate);

			/**
			*/
			void set(const Composite& composite, bool deep = true);

			/**
			*/
			Composite& operator = (const Composite& composite);

			/**
			*/
			void get
				(Composite& composite,
				 UnaryPredicate<Composite>& predicate) const;

			/**
			*/
			void get(Composite& composite, bool deep = true) const;

			/**
			*/
			Size getDegree() const;

			/**
			*/
			Size getSize() const;
			
			/**
			*/
			Size count(const KernelPredicateType& predicate) const;

			/**
			*/
			Size countChildren() const;

			/**
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
			static Size getLevel(const Composite& composite);

			/**
			*/
			Size getLevel() const;

			/**
			*/
			static Composite& getRoot(Composite& composite);

			/**
			*/
			Composite& getRoot();

			/**
			*/
			const Composite& getRoot() const;

			/**
			*/
			Composite *getLowestCommonAncestor(Composite& composite);

			/**
			*/
			const Composite *getLowestCommonAncestor(const Composite& composite) const;

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

			/**	Set the composite's parent.
					This method calls \Ref{composite.prependChild(*this)} to 
					insert the current composite as a child into {\tt composite}.
					It does not perform this insertion if
					\begin{itemize}
						\item {\tt composite == *this} (self insertion)
						\item {\tt isAncestorOf(composite) == true} ({\tt composite} is a descendant of {\tt *this})
						\item {\tt composite} is already the parent of {\tt *this}
					\end{itemize}
					@param	composite the new parent
			*/
			void setParent(Composite& composite);

			/**	Return the composite's parent.
					@return a pointer to the parent composite or 0 if no parent exists
			*/
			Composite* getParent();

			/**	Return the composite's parent (const method).
					@return a pointer to the parent composite or 0 if no parent exists
			*/
			const Composite* getParent() const;

			///
			void setChild(Composite& composite);

			///
			Composite *getChild(Index index);
		
			///
			const Composite *getChild(Index index) const;
		
			///
			void setPreviousSibling(Composite& composite);

			///
			Composite *getPreviousSibling(Index index = 1);

			///
			const Composite *getPreviousSibling(Index index = 1) const;

			///
			void setNextSibling(Composite& composite);

			///
			Composite *getNextSibling(Index index = 1);

			///
			const Composite *getNextSibling(Index index = 1) const;

			///
			Composite *getSibling(Index index);

			///
			const Composite *getSibling(Index index) const;

			///
			void setFirstChild(Composite& composite);

			///
			Composite *getFirstChild();

			///
			const Composite *getFirstChild() const;

			///
			void setLastChild(Composite& composite);

			///
			Composite *getLastChild();

			///
			const Composite *getLastChild() const;

			///
			void expand();

			///
			void collapse();

			///
			void prependChild(Composite& composite);

			///
			void appendChild(Composite& composite);

			///
			static bool insertParent
				(Composite& parent, Composite& first, 
				 Composite& last, bool destroy_parent = true);

			///
			void insertBefore(Composite& composite);

			///
			void insertAfter(Composite& composite);

			///
			void spliceBefore(Composite& composite);

			///
			void spliceAfter(Composite& composite);

			///
			void splice(Composite& composite);

			///
			void splitBefore(Composite& childcomposite, Composite& composite);

			///
			void splitAfter(Composite& childcomposite, Composite& composite);

			///
			bool removeAggregates();

			///
			bool removeChild(Composite& composite);

			///
			void replace(Composite& composite);

			///
			void swap(Composite& composite);

			///
			void sortChildren(const Comparator<Composite *>& comparator);
		
			///
			void sortSiblings(const Comparator<Composite *>& comparator);
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
			bool isChildOf(const Aggregate& aggregate) const;

			///
			bool hasFirstChild() const;
		
			///
			bool isFirstChild() const;
		
			///
			bool isFirstChildOf(const Composite& composite) const;
		
			///
			bool hasLastChild() const;
		
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
			bool applyAggregateCompositeItem(UnaryProcessor<AggregateCompositeItem>& processor);

			///
			bool applyParentAggregate(UnaryProcessor<Aggregate>& processor);

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



			typedef AggregateCompositeItem* AggregateCompositeItemIteratorPosition_;

			class ParentAggregateIteratorTraits_
			{
				public:

				BALL_CREATE(ParentAggregateIteratorTraits_)

				ParentAggregateIteratorTraits_()
					:	position_(0),
						bound_(0)
				{
				}
			
				ParentAggregateIteratorTraits_(const Composite& composite)
					:	position_(0),
						bound_((Composite *)&composite)
				{
				}
			
				ParentAggregateIteratorTraits_(const ParentAggregateIteratorTraits_& traits, bool /* deep */ = true)
					:	position_(traits.position_),
						bound_(traits.bound_)
				{
				}
			
				bool isValid() const
				{
					return (bool)(bound_ != 0 && position_ != 0);
				}

				ParentAggregateIteratorTraits_& operator = (const ParentAggregateIteratorTraits_& traits)
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
			
				AggregateCompositeItemIteratorPosition_& getPosition()
				{
					return position_;
				}
			
				const AggregateCompositeItemIteratorPosition_& getPosition() const
				{
					return position_;
				}

				Aggregate& getData()
				{
					return *(position_->aggregate_);
				}

				const Aggregate& getData() const
				{ 
					return *(position_->aggregate_);
				}

				bool operator == (const ParentAggregateIteratorTraits_& traits) const
				{
					return (bool)(position_ == traits.position_);
				}

				bool operator != (const ParentAggregateIteratorTraits_& traits) const
				{
					return (bool)(position_ != traits.position_);
				}
			
				void invalidate()
				{
					bound_ = 0;
					position_ = 0;
				}

				void toBegin()
				{
					position_ = bound_->first_;
				}

				bool isBegin() const
				{
					return (bool)(position_ == bound_->first_);
				}

				void toEnd()
				{
					position_ = 0;
				}
			
				bool isEnd() const
				{
					return (bool)(position_ == 0);
				}

				void forward()
				{
					position_  = position_->composite_next_;
				}


				protected:

				AggregateCompositeItemIteratorPosition_ position_;


				private:

				Composite *bound_;
			};

			friend class ParentAggregateIteratorTraits_;

			typedef ForwardIterator<Composite, Aggregate, AggregateCompositeItemIteratorPosition_, ParentAggregateIteratorTraits_>
				ParentAggregateIterator;

			ParentAggregateIterator beginParentAggregate()
			{
				return ParentAggregateIterator::begin(*this);
			}

			ParentAggregateIterator endParentAggregate()
			{
				return ParentAggregateIterator::end(*this);
			}



			typedef ConstForwardIterator<Composite, Aggregate, AggregateCompositeItemIteratorPosition_, ParentAggregateIteratorTraits_>
				ParentAggregateConstIterator;

			ParentAggregateConstIterator beginParentAggregate() const
			{
				return ParentAggregateConstIterator::begin(*this);
			}

			ParentAggregateConstIterator endParentAggregate() const
			{
				return ParentAggregateConstIterator::end(*this);
			}

		

			class AggregateCompositeItemIteratorTraits_
				: public ParentAggregateIteratorTraits_
			{
				public:

				BALL_CREATE(AggregateCompositeItemIteratorTraits_)

				AggregateCompositeItemIteratorTraits_()
					:	ParentAggregateIteratorTraits_()
				{
				}
			
				AggregateCompositeItemIteratorTraits_(const Composite& composite)
					:	ParentAggregateIteratorTraits_(composite)
				{
				}
			
				AggregateCompositeItemIteratorTraits_(const AggregateCompositeItemIteratorTraits_& traits, bool /* deep */ = true)
					:	ParentAggregateIteratorTraits_(traits)
				{
				}
			
				AggregateCompositeItemIteratorPosition_& getData()
				{
					return position_;
				}

				const AggregateCompositeItemIteratorPosition_& getData() const
				{ 
					return position_;
				}
			};

			typedef ForwardIterator<Composite, AggregateCompositeItem *, 
					AggregateCompositeItemIteratorPosition_,
					AggregateCompositeItemIteratorTraits_>
				AggregateCompositeItemIterator;

			AggregateCompositeItemIterator beginAggregateCompositeItem()
			{
				return AggregateCompositeItemIterator::begin(*this);
			}

			AggregateCompositeItemIterator endAggregateCompositeItem()
			{
				return AggregateCompositeItemIterator::end(*this);
			}


			typedef ConstForwardIterator
					<Composite, AggregateCompositeItem *,
					 AggregateCompositeItemIteratorPosition_,
					 AggregateCompositeItemIteratorTraits_>
				AggregateCompositeItemConstIterator;

			AggregateCompositeItemConstIterator beginAggregateCompositeItem() const
			{
				return AggregateCompositeItemConstIterator::begin(*this);
			}

			AggregateCompositeItemConstIterator endAggregateCompositeItem() const
			{
				return AggregateCompositeItemConstIterator::end(*this);
			}
				
		protected:

		private:

			enum Property_
			{
				PROPERTY__COLLAPSED_ = 1
			};

			AggregateCompositeItem* find_(const Aggregate& aggregate) const;

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

		

			Size 										number_of_children_;
			AggregateCompositeItem* first_;
			Composite*							parent_;
			Composite* 							previous_;
			Composite* 							next_;
			Composite* 							first_child_;
			Composite* 							last_child_;
			unsigned char						properties_;
	};


	/**
	*/
	class Aggregate
		: public PersistentObject,
			public Object
	{
			friend class AggregateCompositeItem;

		public:			
		
			BALL_CREATE(Aggregate)

			/**	@name Constructors and Destructors */
			//@{

			/// Default constructor
			Aggregate();

			/// Copy constructor
			Aggregate(const Aggregate& aggregate, bool deep = true);

			///
			virtual ~Aggregate();

			/**
			*/
			void* clone(Aggregate& root, bool deep = true) const;

			/**
			*/
			virtual void clear();

			/**
			*/
			void clear(bool deep);

			/**
			*/
			virtual void destroy();

			/**
			*/
			void destroy(bool deep);

			//@}

			/**	@name	Persistence */
			//@{
			
			/// Writes a persistent copy of the object
		  virtual void persistentWrite(PersistenceManager& pm, const char* name = 0) const;

			/// Reads a persistent object
			virtual void persistentRead(PersistenceManager& pm);
			//@}


			virtual AggregateCompositeItem *newItem
				(Aggregate& aggregate, Composite& composite,
				 AggregateCompositeItem* aggregate_next = 0,
				 AggregateCompositeItem* composite_next = 0) const;

			void set(const Aggregate& aggregate, bool deep = true);
				
			Aggregate& operator = (const Aggregate& aggregate);
				
			void get(Aggregate& aggregate, bool deep = true) const;
				
			Size getSize() const;
			
			void insert(Composite& composite);

			void destroyComposites(UnaryPredicate<Composite>& predicate = Composite::DEFAULT_UNARY_PREDICATE);

			bool removeComposites(UnaryPredicate<Composite>& predicate = Composite::DEFAULT_UNARY_PREDICATE);

			bool remove(const Composite& composite);

			void splice
				(Aggregate& aggregate,
				 UnaryPredicate<Composite>& predicate = Composite::DEFAULT_UNARY_PREDICATE);

			static Aggregate& join
				(Aggregate& a, Aggregate& b, Aggregate& result,
				 UnaryPredicate<Composite>& predicate = Composite::DEFAULT_UNARY_PREDICATE);

			static Aggregate& intersect
				(Aggregate& a, Aggregate& b, Aggregate& result,
				 UnaryPredicate<Composite>& predicate = Composite::DEFAULT_UNARY_PREDICATE);

			static Aggregate& subtract
				(Aggregate& a, Aggregate& b, Aggregate& result,
				 UnaryPredicate<Composite>& predicate = Composite::DEFAULT_UNARY_PREDICATE);

			static Aggregate& symsubtract
				(Aggregate& a, Aggregate& b, Aggregate& result,
				 UnaryPredicate<Composite>& predicate = Composite::DEFAULT_UNARY_PREDICATE);

			Aggregate operator | (Aggregate& aggregate) const;

			Aggregate operator& (Aggregate& aggregate) const;

			Aggregate operator - (Aggregate& aggregate) const;

			Aggregate operator ^ (Aggregate& aggregate) const;

			Aggregate& join
				(const Aggregate& aggregate,
				 UnaryPredicate<Composite>& predicate = Composite::DEFAULT_UNARY_PREDICATE);

			Aggregate& intersect
				(const Aggregate& aggregate,
				 UnaryPredicate<Composite>& predicate = Composite::DEFAULT_UNARY_PREDICATE);

			Aggregate& subtract
				(const Aggregate& aggregate,
				 UnaryPredicate<Composite>& predicate = Composite::DEFAULT_UNARY_PREDICATE);

			Aggregate& symsubtract
				(const Aggregate& aggregate,
				 UnaryPredicate<Composite>& predicate = Composite::DEFAULT_UNARY_PREDICATE);

			Aggregate& operator |= (const Aggregate& aggregate);

			Aggregate& operator &= (const Aggregate& aggregate);

			Aggregate& operator -= (const Aggregate& aggregate);

			Aggregate& operator ^= (const Aggregate& aggregate);

			bool isEmpty() const;

			bool isParent() const;

			bool isParentOf(const Composite& composite) const;

			bool hasChildren() const;

			bool isHomomorph(const Aggregate& aggregate) const;

			virtual bool isValid() const;

			void host(Visitor<Aggregate>& visitor);



			bool applyAggregateCompositeItem(UnaryProcessor<AggregateCompositeItem>& processor);
			
			bool applyCompositeRoot(UnaryProcessor<Composite>& processor);

			bool applyComposite(UnaryProcessor<Composite>& processor);


		
			virtual void dump(ostream& s = cout, unsigned long depth = 0) const;


		
			typedef AggregateCompositeItem *AggregateCompositeItemIteratorPosition_;

			class CompositeRootIteratorTraits_
			{
				public:

				BALL_CREATE(CompositeRootIteratorTraits_)

				CompositeRootIteratorTraits_()
					:	position_(0),
						bound_(0)
				{
				}
			
				CompositeRootIteratorTraits_(const Aggregate& aggregate)
					: position_(0),
						bound_((Aggregate *)&aggregate)
				{
				}
			
				CompositeRootIteratorTraits_(const CompositeRootIteratorTraits_& traits, bool /* deep */ = true)
					:	position_(traits.position_),
						bound_(traits.bound_)
				{
				}
			
				bool isValid() const
				{
					return (bool)(bound_ != 0 && position_ != 0);
				}

				CompositeRootIteratorTraits_& operator = (const CompositeRootIteratorTraits_& traits)
				{
					bound_ = traits.bound_;

					position_ = traits.position_;

					return *this;
				}

				Aggregate *getContainer()
				{
					return bound_;
				}
			
				const Aggregate *getContainer() const
				{
					return bound_;
				}

				bool isSingular() const
				{
					return (bool)(bound_ == 0);
					}
			
				AggregateCompositeItemIteratorPosition_& getPosition()
				{
					return position_;
				}
			
				const AggregateCompositeItemIteratorPosition_& getPosition() const
				{
					return position_;
				}

				Composite& getData()
				{
					return *(position_->composite_);
				}

				const Composite& getData() const
				{ 
					return *(position_->composite_);
				}

				bool operator == (const CompositeRootIteratorTraits_& traits) const
				{
					return (bool)(position_ == traits.position_);
				}

				bool operator != (const CompositeRootIteratorTraits_& traits) const
				{
					return (bool)(position_ != traits.position_);
				}
			
				void invalidate()
				{
					bound_ = 0;
					position_ = 0;
				}	

				void toBegin()
				{
					position_ = bound_->first_;
				}

				bool isBegin() const
				{
					return (bool)(position_ == bound_->first_);
				}

				void toEnd()
				{
					position_ = 0;
				}
			
				bool isEnd() const
				{
					return (bool)(position_ == 0);
				}

				void forward()
				{
					position_ = position_->aggregate_next_;
				}


				protected:

				AggregateCompositeItemIteratorPosition_ position_;


				private:

				Aggregate* bound_;
			};

			friend class CompositeRootIteratorTraits_;

			typedef ForwardIterator
				<Aggregate, Composite,
				 AggregateCompositeItemIteratorPosition_,
				 CompositeRootIteratorTraits_>
				CompositeRootIterator;

			CompositeRootIterator beginCompositeRoot()
			{
				return CompositeRootIterator::begin(*this);
			}

			CompositeRootIterator endCompositeRoot()
			{
				return CompositeRootIterator::end(*this);
			}



			typedef ConstForwardIterator
				<Aggregate, Composite,
				 AggregateCompositeItemIteratorPosition_,
				 CompositeRootIteratorTraits_>
				CompositeRootConstIterator;

			CompositeRootConstIterator beginCompositeRoot() const
			{
				return CompositeRootConstIterator::begin(*this);
			}

			CompositeRootConstIterator endCompositeRoot() const
			{
				return CompositeRootConstIterator::end(*this);
			}
		


			class AggregateCompositeItemIteratorTraits_
				: public CompositeRootIteratorTraits_
			{
				public:

				BALL_CREATE(AggregateCompositeItemIteratorTraits_)

				AggregateCompositeItemIteratorTraits_()
					:	CompositeRootIteratorTraits_()
				{
				}
			
				AggregateCompositeItemIteratorTraits_(const Aggregate& aggregate)
					:	CompositeRootIteratorTraits_(aggregate)
				{
				}
			
				AggregateCompositeItemIteratorTraits_(const AggregateCompositeItemIteratorTraits_& traits, bool /* deep */ = true)
					:	CompositeRootIteratorTraits_(traits)
				{
				}
			
				AggregateCompositeItemIteratorPosition_& getData()
				{
					return position_;
				}

				const AggregateCompositeItemIteratorPosition_& getData() const
				{ 
					return position_;
				}
			};

			typedef ForwardIterator
				<Aggregate, AggregateCompositeItem *,
				 AggregateCompositeItemIteratorPosition_,
				 AggregateCompositeItemIteratorTraits_>
				AggregateCompositeItemIterator;

			AggregateCompositeItemIterator beginAggregateCompositeItem()
			{
				return AggregateCompositeItemIterator::begin(*this);
			}

			AggregateCompositeItemIterator endAggregateCompositeItem()
			{
				return AggregateCompositeItemIterator::end(*this);
			}



			typedef ConstForwardIterator
				<Aggregate, AggregateCompositeItem *,
				 AggregateCompositeItemIteratorPosition_,
				 AggregateCompositeItemIteratorTraits_>
				AggregateCompositeItemConstIterator;

			AggregateCompositeItemConstIterator beginAggregateCompositeItem() const
			{
				return AggregateCompositeItemConstIterator::begin(*this);
			}

			AggregateCompositeItemConstIterator endAggregateCompositeItem() const
			{
				return AggregateCompositeItemConstIterator::end(*this);
			}
				

		
			typedef AggregateCompositeItem *CompositeIteratorPosition_;

			class CompositeIteratorTraits_
			{
				public:

				CompositeIteratorTraits_()
					:	bound_(0),
						position_(0),
						subcomposite_iterator_()
				{
				}
			
				CompositeIteratorTraits_(const Aggregate& aggregate)
					:	bound_((Aggregate *)&aggregate),
						position_(0),
						subcomposite_iterator_()
				{
				}
			
				CompositeIteratorTraits_(const CompositeIteratorTraits_& traits, bool /* deep */ = true)
					:	bound_(traits.bound_),
						position_(traits.position_),
						subcomposite_iterator_(traits.subcomposite_iterator_)
				{
				}
				
				bool isValid() const
				{
					return (bool)(bound_ != 0 && position_ != 0 && subcomposite_iterator_.isValid() == true);
				}

				CompositeIteratorTraits_& operator = (const CompositeIteratorTraits_& traits)
				{
					bound_ = traits.bound_;
					position_ = traits.position_;
					subcomposite_iterator_ = traits.subcomposite_iterator_;
					return *this;
				}

				Aggregate* getContainer()
				{
					return bound_;
				}
			
				const Aggregate* getContainer() const
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
					return *subcomposite_iterator_;
				}

				const Composite& getData() const
				{ 
					return *subcomposite_iterator_;
				}

				bool operator == (const CompositeIteratorTraits_& traits) const
				{
					return (bool)(subcomposite_iterator_ == traits.subcomposite_iterator_);
				}

				bool operator != (const CompositeIteratorTraits_& traits) const
				{
					return (bool)(subcomposite_iterator_ != traits.subcomposite_iterator_);
				}
			
				void invalidate()
				{
					bound_ = 0;
					position_ = 0;
					subcomposite_iterator_.invalidate();
				}

				void toBegin()
				{
					position_ = bound_->first_;

					if (position_ != 0)
					{
						subcomposite_iterator_ = position_->composite_->beginSubcomposite();
					}
				}	

				bool isBegin() const
				{
					return (bool)(position_ == bound_->first_ && subcomposite_iterator_.isBegin() == true);
				}

				void toEnd()
				{
					position_ = 0;
					subcomposite_iterator_.toEnd();
				}
			
				bool isEnd() const
				{
					return (bool)(position_ == 0);
				}

				void forward()
				{
					++subcomposite_iterator_;

					if (subcomposite_iterator_.isEnd() == true)
					{
						position_ = position_->aggregate_next_;

						if (position_ != 0)
						{
							subcomposite_iterator_ = position_->composite_->beginSubcomposite();
						}
					}
				}		

				private:

				Aggregate*												bound_;
				CompositeIteratorPosition_ 				position_;
				Composite::SubcompositeIterator 	subcomposite_iterator_;
			};

			friend class CompositeIteratorTraits_;

			typedef ForwardIterator
				<Aggregate, Composite,
				 CompositeIteratorPosition_,
				 CompositeIteratorTraits_>
				CompositeIterator;

			CompositeIterator beginComposite()
			{
				return CompositeIterator::begin(*this);
			}

			CompositeIterator endComposite()
			{
				return CompositeIterator::end(*this);
			}



			typedef ConstForwardIterator
				<Aggregate, Composite,
				 CompositeIteratorPosition_,
				 CompositeIteratorTraits_>
				CompositeConstIterator;

			CompositeConstIterator beginComposite() const
			{
				return CompositeConstIterator::begin(*this);
			}

			CompositeConstIterator endComposite() const
			{
				return CompositeConstIterator::end(*this);
			}
		
		protected:

		private:

			Size 										size_;
			AggregateCompositeItem* first_;
			AggregateCompositeItem* last_;
	};

#	ifndef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/CONCEPT/composite.iC>
#	endif

} // namespace BALL

#endif // BALL_CONCEPT_COMPOSITE_H
