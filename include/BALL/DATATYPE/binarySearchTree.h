// $Id: binarySearchTree.h,v 1.14 2000/08/09 10:09:25 amoll Exp $

#ifndef BALL_DATATYPE_BINARYSEARCHTREE_H
#define BALL_DATATYPE_BINARYSEARCHTREE_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif
 
#ifndef BALL_COMMON_COMMON_H
#	include <BALL/COMMON/rtti.h>
#endif
 
#ifndef BALL_CONCEPT_COMPARATOR_H
#	include <BALL/CONCEPT/comparator.h>
#endif

#ifndef BALL_CONCEPT_FORWARDITERATOR_H
#	include <BALL/CONCEPT/forwardIterator.h>
#endif

#ifndef BALL_CONCEPT_VISITOR_H
#	include <BALL/CONCEPT/visitor.h>
#endif

#ifndef BALL_DATATYPE_FOREACH_H
#	include <BALL/DATATYPE/forEach.h>
#endif

#ifndef BALL_CONCEPT_PROCESSOR_H
#	include <BALL/CONCEPT/processor.h>
#endif

#ifndef BALL_DATATYPE_LISTSTACK_H
#	include <BALL/DATATYPE/listStack.h>
#endif

namespace BALL 
{

	/**	Binary Search Tree Item.
			A red-black tree is a binary search tree with one extra bit of storage per node:
			its color, which can be either RED or BLACK. 
			A binary search tree is a red-black tree if it satisfies the following red-black
			properties:
			{\em 1.} Every node is either red or black.
			{\em 2.} Every leaf is black.
			{\em 3.} If a node is red, then both its children are black.
			{\em 4.} Every simple path from a node to a descendant leaf contains the same
							 number of black nodes.
	*/
	class BSTreeItem 
	{ 
		public:

		/**	@name	Enums
		*/
		//@{

		enum
		{
			/**	
			*/
			RED   = 0,

			/**	
			*/
			BLACK = 1
		};
		//@}

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
				Create a new and empty BSTreeItem
		*/
		BSTreeItem();

		/**	Copy constructor.
				Create a new BSTreeItem from another
		*/
		BSTreeItem(const BSTreeItem& item, bool /* deep */= true);

		/**	Detailled Constructor.
				Create a new BSTreeItem with color {\tt color} and the two child items
				{\tt left_item} and {\tt right_item}.
				@param	color the node color
				@param	left_item the left child
				@param	right_item the right child
		*/
		BSTreeItem(BSTreeItem* left_item, BSTreeItem* right_item, char color = BSTreeItem::BLACK);

		/**	Destructor.
				Destruct the BSTreeItem
		*/
		virtual ~BSTreeItem();
		//@}

		/**	@name	Accessors
		*/
		//@{

		/**	Assign the left child.
				The item's left child is replaced by {\tt item}. The replaced item
				is not destructed.
				@param	item the new left child
		*/
		void setLeftChild(BSTreeItem* item);

		/**	Return the left child.
				@return the item's left child item
		*/
		BSTreeItem*& getLeftChild() const;
		
		/**	Assign the right child.
				The item's right child is replaced by {\tt item}. The replaced item
				is not destructed.
				@param	item the new right child
		*/
		void setRightChild(BSTreeItem *item);

		/**	Return the right child.
				@return the item's right child item
		*/
		BSTreeItem*& getRightChild() const;

		/**	Return the color of this instance.
				@return char the color 0 = RED, 1 = BLACK
		*/
		char getColor() const;

		/**	Set the color of this instance.
				If no suitable value for color is given nothing happens.
				@param color the color 0 = RED, 1 = BLACK
		*/
		void setColor(char color);

		/**	Get the size of this instance.
				The size is the number of all offsprings + 1.
				(The instance itselfs counts also).
				@return Size the size
		*/
		Size getSize() const;

		/**	Get the height of the tree with this instance as root.
				(The instance itselfs counts also).
				@return Size the height
		*/
		Size getHeight() const;

		/** Get the minimum node.
				The minimum is the leftmost node in the tree.
				@return BSTreeItem* the minimum node
		*/
		const BSTreeItem* getMinimum() const;

		/** Get the maximum node.
				The maximum is the rightmost node in the tree.
				@return BSTreeItem* the maximum node
		*/
		const BSTreeItem* getMaximum() const;

		/** Get the parent of the minimum node.
				The minimum node is the leftmost node in the tree.
				@return BSTreeItem* the parent of the minimum node
		*/
		BSTreeItem* getParentOfMinimum(); // ??? warum hier nich const?

		/** Get the parent of the maximum node.
				The maximum node is the rightmost node in the tree.
				@return BSTreeItem* the parent of the maximum node
		*/
		BSTreeItem* getParentOfMaximum();

		/** Get the parent of the predecessor.
				The successor of a node x is the node with the largest
				key smaller than key[x].
				Predecessor is the right child of node returned, unless item 
				itself is the parent. Then the left child is the predecessor. 
				If this is a leaf, 0 is returned. ASSUMES item isn't null.
		*/
		BSTreeItem* getParentOfPredecessor();	// ??? warum hier nich const?

		/** Get the parent of the successor.
				The successor of a node x is the node with the smallest
				key greater than key[x].
				Successor is the left child of node returned, unless item itself is the parent.
				Then the right child is the successor. 
				If item is a leaf, 0 is returned. ASSUMES item isn't null.
		*/
		BSTreeItem* getParentOfSuccessor();

		/** Detaches node item with parent p from the tree.
				Node item is the left child if right_side = false,
				else it's the right child.
				If p is 0, it means item is the root, and that is handled
				accordingly. Redundantly returns the pointer item. May
				have to update root pointer.
				@param root gets the address of the replacement if no parent of t was given
				@param p pointer to the parent of t
				@param t the item to remove
				@param right_side defines where to place the replacement, default = true => right child
				@return BSTreeItem* pointer to the removed item
		*/
		static BSTreeItem* detachNode
			(BSTreeItem*& root, BSTreeItem* t,
			 BSTreeItem* p, bool right_side);

		/** Rotate right a node an its children.
				PRECONDITION: left child exists.
				If left child does not exists 0 is returned.
		*/
		BSTreeItem* rotateRight();

		/** Rotate left a node an its children.
				PRECONDITION: right child exists.
				If right child does not exists 0 is returned.
		*/
		BSTreeItem* rotateLeft();

		//@}

		/**	@name	Processor Application Methods
		*/	
		//@{

		/** Apply a processor in preorder to the tree.
				Preorder means the root is processed first, than the left subtree
				and at the end the right subtree.
		*/
		bool applyPreorder(UnaryProcessor<BSTreeItem>& processor);
		
		/** Apply a processor in inorder to the tree.
				Inorder means the left subtree is processed first, than the root
				and at the end the right subtree.
		*/
		bool applyInorder(UnaryProcessor<BSTreeItem>& processor);

		/** Apply a processor in postorder to the tree.
				Postorder means the left subtree is processed first, than the right subtree
				and at the end the root.
		*/
		bool applyPostorder(UnaryProcessor<BSTreeItem>& processor);

		/** Apply a processor in levelorder to the tree.
				Levelorder means root is processed first, than all child, 
				after that all grandchilds, etc.
		*/
		bool applyLevelorder(UnaryProcessor<BSTreeItem>& processor);

		/**
		*/
		bool applyPreorderFlat(UnaryProcessor<BSTreeItem>& processor);

		/**
		*/
		bool applyInorderFlat(UnaryProcessor<BSTreeItem>& processor);

		/**
		*/
		bool applyPostorderFlat(UnaryProcessor<BSTreeItem>& processor);

		/** Apply a processor to all nodes of the tree.
				This method uses Preorder.
				@see applyPreorder
		*/
		bool apply(UnaryProcessor<BSTreeItem>& processor);
		//@}


		/**	@name Predicates 
		*/
		//@{

		/**	Return true if the node is a leaf of the tree.
				This method returns {\bf true} if the node is 
				a leaf of the tree, i.e. if it has no children.
				@return bool {\bf true} if the node has no children
		*/
		bool isLeaf() const;

		/**	Return true if the node is an interior node of the tree.
				This method returns {\bf true} if the node is 
				an interior node of the tree, i.e. if it has children.
				@return bool {\bf true} if the node has children
		*/
		bool isInterior() const;
		//@}

		struct Pack 
		{
			BSTreeItem* t;
			BSTreeItem* p;
			BSTreeItem* g;
			BSTreeItem* gg;
			BSTreeItem* s;
			BSTreeItem* rs;
			BSTreeItem* ls;
			BSTreeItem* m;
			BSTreeItem* pm;
		};

		static BSTreeItem* insertBalance(BSTreeItem* root, BSTreeItem::Pack& pp);
		
		static BSTreeItem* removeBalance(BSTreeItem* root, BSTreeItem::Pack& pp);
		
		static BSTreeItem* replace(BSTreeItem* root, BSTreeItem::Pack& pp);
		
		static BSTreeItem* detachMinimum(BSTreeItem*& root);
		
		static BSTreeItem* detachMaximum(BSTreeItem*& root);

		protected:

		/**	The left child.
				{\tt left} is a pointer to the left child of this node.
		*/
		BSTreeItem* left_;

		/**	The right child.
				{\tt right} is a pointer to the right child of this node.
		*/
		BSTreeItem* right_;

		/**	The color of the node.
				Each node has a color: either {\tt RED} or {\tt BLACK}.
		*/
		char color_;

		private:

		Processor::Result applyPreorder_(UnaryProcessor<BSTreeItem>& processor);
		
		Processor::Result applyInorder_(UnaryProcessor<BSTreeItem>& processor);

		Processor::Result applyPostorder_(UnaryProcessor<BSTreeItem>& processor);

		Processor::Result applyLevelorder_(UnaryProcessor<BSTreeItem>& processor);

		Processor::Result applyPreorderFlat_(UnaryProcessor<BSTreeItem>& processor);

		Processor::Result applyInorderFlat_(UnaryProcessor<BSTreeItem>& processor);

		Processor::Result applyPostorderFlat_(UnaryProcessor<BSTreeItem>& processor);
	};



	template<typename DataType>
	class TBSTreeItem
		: public BSTreeItem 
	{
		public:
			
		BALL_CREATE_NODEEP(TBSTreeItem)


		/**	@name Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
				Create a new and empty TBSTreeItem
		*/
		TBSTreeItem()
			:	BSTreeItem()
		{
		}

		/**	Detailed constructor.
				Create a new TBSTreeItem from its {\tt data}, {\tt color},
				{\tt left_item} and {\tt right_item}.
				@param	data	the node's data
				@param	color the node's color
				@param	left_item the left child of the node
				@param	right_item the right child of the node
		*/
		TBSTreeItem
			(const DataType& data, TBSTreeItem* left_item, 
			 TBSTreeItem* right_item, char color = BSTreeItem::BLACK)
			: BSTreeItem(left_item, right_item, color),
				data_(data)
		{
		}

		/**	Destructor.
				Destruct the item.
		*/
		virtual ~TBSTreeItem()
		{
		}
		//@}
		
		/**	@name	Accessors
		*/
		//@{

		/**	Set the nodes data.
		*/
		void setData(const DataType& data)
		{
			data_ = data;
		}

		/**	Return the nodes data.
				@return the item's data
		*/
		const DataType& getData() const
		{
			return data_;
		}

		/**	Return the left child.
				@return the item's left child item
		*/
		TBSTreeItem*& getLeftChild() const
		{
			return (TBSTreeItem*&)left_;
		}
		
		/**	Return the right child.
				@return the item's right child item
		*/
		TBSTreeItem*& getRightChild() const
		{
			return (TBSTreeItem*&)right_;
		}

		/**
		*/
		Size count(const DataType& data, const Comparator<DataType>* comparator) const
		{
			if (comparator == 0)
			{
				throw Exception::NullPointer(__FILE__, __LINE__);
			}

			const TBSTreeItem* item = this;
			Size size = 0;

			while(item) 
			{
				if (comparator->isEqual(item->data_, data) == true)
				{
					++size;
				}

				if (item->left_)
				{
					size += ((TBSTreeItem*)item->left_)->count(data, comparator);
				}
	
				item = (TBSTreeItem*)item->right_;
			}

			return size;
		}
		//@}

		/**	@name	Processor application methods
		*/
		//@{

		/**
		*/
		bool applyPreorder(UnaryProcessor<DataType>& processor)
		{
			TreeItemProcessor_ tree_item_proc(&processor);
			
			return BSTreeItem::applyPreorder(tree_item_proc);
		}
		
		/**
		*/
		bool applyInorder(UnaryProcessor<DataType>& processor)
		{
			TreeItemProcessor_ tree_item_proc(&processor);
			
			return BSTreeItem::applyInorder(tree_item_proc);
		}

		/**
		*/
		bool applyPostorder(UnaryProcessor<DataType>& processor)
		{
			TreeItemProcessor_ tree_item_proc(&processor);
			
			return BSTreeItem::applyPostorder(tree_item_proc);
		}

		/**
		*/
		bool applyLevelorder(UnaryProcessor<DataType>& processor)
		{
			TreeItemProcessor_ tree_item_proc(&processor);
			
			return BSTreeItem::applyLevelorder(tree_item_proc);
		}

		/**
		*/
		bool applyPreorderFlat(UnaryProcessor<DataType>& processor)
		{
			TreeItemProcessor_ tree_item_proc(&processor);
			
			return BSTreeItem::applyPreorderFlat(tree_item_proc);
		}

		/**
		*/
		bool applyInorderFlat(UnaryProcessor<DataType>& processor)
		{
			TreeItemProcessor_ tree_item_proc(&processor);
			
			return BSTreeItem::applyInorderFlat(tree_item_proc);
		}

		/**
		*/
		bool applyPostorderFlat(UnaryProcessor<DataType>& processor)
		{
			TreeItemProcessor_ tree_item_proc(&processor);
			
			return BSTreeItem::applyPostorderFlat(tree_item_proc);
		}

		/**
		*/
		bool apply(UnaryProcessor<DataType>processor)
		{
			return applyPreorder(processor);
		}
		//@}

		protected:

		DataType data_;

		private:

		class TreeItemProcessor_
			: public UnaryProcessor<BSTreeItem>
		{
			public:

			TreeItemProcessor_(UnaryProcessor<DataType>* processor = 0)
				:	processor_(processor)
			{
			}
			
			virtual Processor::Result operator () (BSTreeItem& item)
			{
				return processor_->operator()(((TBSTreeItem<DataType> &)item).data_);
			}

			UnaryProcessor<DataType>* processor_;
		};
		friend class TreeItemProcessor_;

	};



	class BSTreeIterator 
	{
		public:
			
			enum WalkOrder
			{
				WALK_ORDER__PREORDER,
				WALK_ORDER__INORDER,
				WALK_ORDER__POSTORDER,
				WALK_ORDER__LEVELORDER
			};

			BSTreeIterator
				(const BSTreeItem* item = 0, WalkOrder walk_order = BSTreeIterator::WALK_ORDER__PREORDER);
			
			BSTreeIterator(const BSTreeIterator& iterator);
			
			virtual ~BSTreeIterator(){}

			void set(const BSTreeItem* item, WalkOrder walk_order = BSTreeIterator::WALK_ORDER__PREORDER);

			void set(const BSTreeIterator& iterator);
			
			BSTreeIterator& operator = (const BSTreeIterator& iterator);
			
			const BSTreeItem* forward();

			void clear();

			bool operator == (const BSTreeIterator& iterator) const;
			
			bool operator != (const BSTreeIterator& iterator) const;
			
		protected:
		
			const BSTreeItem* forwardPreOrder();
			
			const BSTreeItem* forwardInOrder();
			
			const BSTreeItem* forwardPostOrder();
			
			const BSTreeItem* forwardLevelOrder();
			
			const BSTreeItem *(BSTreeIterator::* forward_)();
			TSStack<const BSTreeItem *> path_;
			const BSTreeItem *root_;
			const BSTreeItem *current_;
			WalkOrder walk_order_;
			bool state_;
	};



	template<typename BSTreeItemType>
	class TBSTreeIterator
		: public BSTreeIterator 
	{
		public:
		
			TBSTreeIterator(const BSTreeItemType* itemType = 0)
				: BSTreeIterator(itemType)
			{
			}

			TBSTreeIterator(const BSTreeItemType* itemType, BSTreeIterator::WalkOrder walk_order)
				:	BSTreeIterator(itemType, walk_order)
			{
			}

			TBSTreeIterator(const TBSTreeIterator& iterator)
				:	BSTreeIterator(iterator)
			{
			}

			~TBSTreeIterator()
			{
			}

			TBSTreeIterator& operator = (const TBSTreeIterator& iterator)
			{
				BSTreeIterator::set(iterator);

				return *this;
			}   
		
			const BSTreeItemType* forward()
			{
				return (BSTreeItemType *)((this->*forward_)());
			}
	};



	template<typename DataType>
	class TBSTree 
	{
		public:
		
		BALL_CREATE(TBSTree)
	
		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
				Creates a new TBSTree object.
		*/
		TBSTree();

		/**	Copy constructor.
				Creates a new TBSTree object from anothe object.
				@param	tree the TBSTree object to be copied
				@param	deep {\bf true} deep copy, shallow otherwise
		*/
		TBSTree(const TBSTree& tree, bool deep = true);

		/**	Destructor
		*/
		virtual ~TBSTree();

		/**	
		*/
		virtual void clear();

		/**
		*/
		void destroy();

		//@}

		/**	@name	Assignment */
		//@{

		/**	
		*/
		void set(const TBSTree& tree, bool deep = true);

		/**	
		*/
		void set(const TBSTree& tree, BSTreeIterator::WalkOrder walk_order);

		/**	
		*/
		TBSTree& operator = (const TBSTree& tree);

		/**	
		*/
		void get(TBSTree& tree, bool deep = true) const;

		/**	
		*/
		void get(TBSTree& tree, BSTreeIterator::WalkOrder walk_order) const;

		/**	
		*/
		void swap(TBSTree &tree);

		//@}

		/**	@name	Accessors 
		*/
		//@{

		/**	
		*/
		void setComparator(const Comparator<DataType>& comparator);
	
		/**	
		*/
		void resetComparator();
	
		/**	
		*/
		const Comparator<DataType>* getComparator() const;
	
		/**	
		*/
		const TBSTreeItem<DataType>* getRoot() const;

		/**	
		*/
		Size getHeight() const;
		
		/**	
		*/
		Size getSize() const;
		
		/**	
		*/
		const DataType* getMinimum() const;

		/**	
		*/
		const DataType* getMaximum() const;

		/**	
		*/
		const DataType* find(const DataType& data) const;

		/**	
		*/
		Size count(const DataType& data) const;
		
		/**	
		*/
		TBSTreeItem<DataType>* insert(const DataType& data, bool multiple = true);

		/**	
		*/
		TBSTreeItem<DataType>* detach(const DataType& data);

		/**	
		*/
		TBSTreeItem<DataType>* detachMinimum();

		/**	
		*/
		TBSTreeItem<DataType>* detachMaximum();

		/**	
		*/
		bool remove(const DataType& data);

		/**	
		*/
		bool removeAll(const DataType& data);

		/**	
		*/
		bool removeMinimum();

		/**	
		*/
		bool removeMaximum();
			
		//@}
	
		/**	@name Miscellaneous
		*/
		//@{

		/**	
		*/
		void host(Visitor<TBSTree>& visitor);

		//@}


		/**	@name	Predicates 
		*/
		//@{

		/**	
		*/
		bool operator == (const TBSTree& tree) const;
	
		/**	
		*/
		bool operator != (const TBSTree& tree) const;
	
		/**	
		*/
		bool has(const DataType& data) const;

		/**	
		*/
		bool isEmpty() const;
		//@}

		/**	@name	Debugging and Diagnostics 
		*/
		//@{

		/**	
		*/
		virtual bool isValid() const;

		/**	
		*/
		virtual void dump(std::ostream& s = std::cout, Size depth = 0) const;
		//@}

		/**		@name Internal Iterators
		*/
		//@{

		/**	
		*/
		bool applyPreorder(UnaryProcessor<DataType>& processor);
		
		/**	
		*/
		bool applyInorder(UnaryProcessor<DataType>& processor);

		/**	
		*/
		bool applyPostorder(UnaryProcessor<DataType>& processor);

		/**	
		*/
		bool applyLevelorder(UnaryProcessor<DataType>& processor);

		/**	
		*/
		bool applyPreorderFlat(UnaryProcessor<DataType>& processor);

		/**	
		*/
		bool applyInorderFlat(UnaryProcessor<DataType>& processor);

		/**	
		*/
		bool applyPostorderFlat(UnaryProcessor<DataType>& processor);

		/**	
		*/
		bool apply(UnaryProcessor<DataType>& processor);

		//@}

		/**	@name External Iterators
		*/
		//@{

		/**	
		*/
		typedef const TBSTreeItem<DataType>* IteratorPosition;
	
		class PreorderIteratorTraits_
		{
			public:

			BALL_CREATE(PreorderIteratorTraits_)

			PreorderIteratorTraits_()
				:	bound_(0),
					iterator_(),
					position_(0)
			{
			}
			
			PreorderIteratorTraits_(const TBSTree& tree)
				:	bound_((TBSTree *)&tree),
					iterator_(),
					position_(0)
			{
			}
			
			PreorderIteratorTraits_(const PreorderIteratorTraits_& traits)
				:	bound_(traits.bound_),
					iterator_(traits.iterator_),
					position_(traits.position_)
			{
			}
			
			PreorderIteratorTraits_& operator =	(const PreorderIteratorTraits_& traits)
			{
				bound_ = traits.bound_;
				iterator_ = traits.iterator_;
				position_ = traits.position_;
		
				return *this;
			}

			TBSTree* getContainer()
			{
				return bound_;
			}
			
			const TBSTree* getContainer() const
			{
				return bound_;
			}
			
			bool isSingular() const
			{
				return (bool)(bound_ == 0);
			}
			
			IteratorPosition& getPosition()
			{
				return position_;
			}

			const IteratorPosition& getPosition() const
			{
				return position_;
			}

			bool operator == (const PreorderIteratorTraits_& traits) const
			{
				return (bool)(position_ == traits.position_);
			}

			bool operator != (const PreorderIteratorTraits_& traits) const
			{
				return (bool)(position_ != traits.position_);
			}
			
			bool isValid() const
			{
				return (bool)(bound_ != 0 && position_ != 0);
			}

			void invalidate()
			{
				bound_ = 0;
				iterator_.clear();
				position_ = 0;
			}
		
			void toBegin()
			{
				iterator_.set(bound_->getRoot(), BSTreeIterator::WALK_ORDER__PREORDER);
				position_ = iterator_.forward();
			}

			bool isBegin() const
			{
				TBSTreeIterator< TBSTreeItem<DataType> > iterator_
					(bound_->getRoot(), BSTreeIterator::WALK_ORDER__PREORDER);

				IteratorPosition first_position = iterator_.forward();

				return (bool)(position_ == first_position);
			}

			void toEnd()
			{
				position_ = 0;
			}
			
			bool isEnd() const
			{
				return (bool)(position_ == 0);
			}
			
			const DataType& getData() const
			{
				return position_->data_;
			}
			
			void forward()
			{
				position_ = iterator_.forward();
			} 

			protected:

			TBSTree<DataType>*												bound_;
			TBSTreeIterator< TBSTreeItem<DataType> >	iterator_;
			IteratorPosition													position_;
		};

		friend class PreorderIteratorTraits_;

		typedef ForwardIterator
			<TBSTree<DataType>, DataType,
			 const TBSTreeItem<DataType>*,
			 PreorderIteratorTraits_>
			PreorderIterator;

		PreorderIterator beginPreorder()
		{
			return PreorderIterator::begin(*this);
		}

		PreorderIterator endPreorder()
		{
			return PreorderIterator::end(*this);
		}

		
	
		typedef ConstForwardIterator
			<TBSTree<DataType>, DataType,
			 const TBSTreeItem<DataType> *,
			 PreorderIteratorTraits_>
			ConstPreorderIterator;

		ConstPreorderIterator beginPreorder() const
		{
			return ConstPreorderIterator::begin(*this);
		}

		ConstPreorderIterator endPreorder() const
		{
			return ConstPreorderIterator::end(*this);
		}


		class InorderIteratorTraits_
			: public PreorderIteratorTraits_
		{
			public:

			InorderIteratorTraits_()
				:	PreorderIteratorTraits_()
			{
			}
			
			InorderIteratorTraits_(const TBSTree& tree)
				:	PreorderIteratorTraits_(tree)
			{
			}
			
			InorderIteratorTraits_(const InorderIteratorTraits_& traits)
				:	PreorderIteratorTraits_(traits)
			{
			}

			void toBegin()
			{
				iterator_.set(bound_->getRoot(), BSTreeIterator::WALK_ORDER__INORDER);
		
				position_ = iterator_.forward();
			}

			bool isBegin() const
			{
				TBSTreeIterator< TBSTreeItem<DataType> > iterator_
					(bound_->getRoot(), BSTreeIterator::WALK_ORDER__INORDER);

				IteratorPosition first_position = iterator_.forward();

				return (bool)(position_ == first_position);
			}
		};

		friend class InorderIteratorTraits_;

		typedef ForwardIterator
			<TBSTree<DataType>, DataType,
			 const TBSTreeItem<DataType>*,
			 InorderIteratorTraits_>
			InorderIterator;

		InorderIterator beginInorder()
		{
			return InorderIterator::begin(*this);
		}

		InorderIterator endInorder()
		{
			return InorderIterator::end(*this);
		}



		typedef ConstForwardIterator
			<TBSTree<DataType>, DataType,
			 const TBSTreeItem<DataType>*,
			 InorderIteratorTraits_>
			ConstInorderIterator;

		ConstInorderIterator beginInorder() const
		{
			return ConstInorderIterator::begin(*this);
		}

		ConstInorderIterator endInorder() const
		{
			return ConstInorderIterator::end(*this);
		}


		class PostorderIteratorTraits_
			: public PreorderIteratorTraits_
		{
			public:

			PostorderIteratorTraits_()
				:	PreorderIteratorTraits_()
			{
			}
		
			PostorderIteratorTraits_(const TBSTree& tree)
				:	PreorderIteratorTraits_(tree)
			{
			}
			
			PostorderIteratorTraits_(const PostorderIteratorTraits_& traits)
				:	PreorderIteratorTraits_(traits)
			{
			}

			void toBegin()
			{
				iterator_.set(bound_->getRoot(), BSTreeIterator::WALK_ORDER__POSTORDER);
		
				position_ = iterator_.forward();
			}

			bool isBegin() const
			{
				TBSTreeIterator< TBSTreeItem<DataType> > iterator_
					(bound_->getRoot(), BSTreeIterator::WALK_ORDER__POSTORDER);

				IteratorPosition first_position = iterator_.forward();

				return (bool)(position_ == first_position);
			}
		};

		friend class PostorderIteratorTraits_;

		typedef ForwardIterator
			<TBSTree<DataType>, DataType,
			 const TBSTreeItem<DataType>*,
			 PostorderIteratorTraits_>
			PostorderIterator;

		PostorderIterator beginPostorder()
		{
			return PostorderIterator::begin(*this);
		}

		PostorderIterator endPostorder()
		{
			return PostorderIterator::end(*this);
		}


		typedef ConstForwardIterator
			<TBSTree<DataType>, DataType,
			 const TBSTreeItem<DataType>*,
			 PostorderIteratorTraits_>
			ConstPostorderIterator;

		ConstPostorderIterator beginPostorder() const
		{
			return ConstPostorderIterator::begin(*this);
		}

		ConstPostorderIterator endPostorder() const
		{
			return ConstPostorderIterator::end(*this);
		}



		class LevelorderIteratorTraits_
			: public PreorderIteratorTraits_
		{
			public:

			LevelorderIteratorTraits_()
				:	PreorderIteratorTraits_()
			{
			}
			
			LevelorderIteratorTraits_(const TBSTree& tree)
				:	PreorderIteratorTraits_(tree)
			{
			}
			
			LevelorderIteratorTraits_(const LevelorderIteratorTraits_& traits)
				:	PreorderIteratorTraits_(traits)
			{
			}

			void toBegin()
			{
				iterator_.set(bound_->getRoot(), BSTreeIterator::WALK_ORDER__LEVELORDER);
		
				position_ = iterator_.forward();
			}

			bool isBegin() const
			{
				TBSTreeIterator< TBSTreeItem<DataType> > iterator_
					(bound_->getRoot(), BSTreeIterator::WALK_ORDER__LEVELORDER);

				IteratorPosition first_position = iterator_.forward();

				return (bool)(position_ == first_position);
			}
		};

		friend class LevelorderIteratorTraits_;

		typedef ForwardIterator
			<TBSTree<DataType>, DataType, const TBSTreeItem<DataType>*,
			 LevelorderIteratorTraits_>	LevelorderIterator;

		LevelorderIterator beginLevelorder()
		{
			return LevelorderIterator::begin(*this);
		}

		LevelorderIterator endLevelorder()
		{
			return LevelorderIterator::end(*this);
		}



		typedef ConstForwardIterator
			<TBSTree<DataType>, DataType, const TBSTreeItem<DataType>*,
			 LevelorderIteratorTraits_>	ConstLevelorderIterator;

		ConstLevelorderIterator beginLevelorder() const
		{
			return ConstLevelorderIterator::begin(*this);
		}

		ConstLevelorderIterator endLevelorder() const
		{
			return ConstLevelorderIterator::end(*this);
		}



		typedef InorderIterator Iterator;

		Iterator begin()
		{
			return Iterator::begin(*this);
		}

		Iterator end()
		{
			return Iterator::end(*this);
		}

		
		typedef ConstInorderIterator ConstIterator;

		ConstIterator begin() const
		{
			return ConstIterator::begin(*this);
		}

		ConstIterator end() const
		{
			return ConstIterator::end(*this);
		}

		//@}



		protected:
	
		virtual BSTreeItem* newItem
			(const DataType& data, BSTreeItem* left_item = 0, 
			 BSTreeItem* right_item = 0, char color = BSTreeItem::BLACK) const;

		virtual void deleteItem(BSTreeItem* item) const;

		virtual BSTreeItem* clone_(const BSTreeItem* item) const;

		void destroy_(TBSTreeItem<DataType>* item);

		bool isValid_(const TBSTreeItem<DataType>* parent, const TBSTreeItem<DataType>* child) const;

		void dump_(std::ostream& s, Size depth, const TBSTreeItem<DataType>* item) const;

		TBSTreeItem<DataType>*				root_;
		const Comparator<DataType>*		comparator_;
	};

	template<typename DataType>
	TBSTree<DataType>::TBSTree()
		:	root_(0),
			comparator_(&(RTTI::getDefault<Comparator<DataType> >()))
	{
	}

	template<typename DataType>
	TBSTree<DataType>::TBSTree(const TBSTree<DataType>& tree, bool deep)
		:	root_(0),
			comparator_(0)
	{
		if (deep == false)
		{
			throw Exception::NotImplemented(__FILE__, __LINE__);
		}
		
		set(tree);
	}

	template<typename DataType>
	TBSTree<DataType>::~TBSTree()
	{
		clear();
	}

	template<typename DataType>
	BALL_INLINE 
	void TBSTree<DataType>::clear()
	{
		destroy_(root_);
		root_ = 0;
	}

	template<typename DataType>
	BALL_INLINE 
	void TBSTree<DataType>::destroy()
	{
		clear();
	}

	template<typename DataType>
	BALL_INLINE 
	void TBSTree<DataType>::set
		(const TBSTree<DataType>& tree, bool deep)
	{
		if (deep == false)
		{
			throw Exception::NotImplemented(__FILE__, __LINE__);
		}
		
		clear();
		comparator_ = tree.comparator_;
		root_ = (TBSTreeItem<DataType> *)clone_(tree.root_);
	}

	template<typename DataType>
	void TBSTree<DataType>::set(const TBSTree<DataType>& tree, BSTreeIterator::WalkOrder walk_order)
	{
		clear();
		comparator_ = tree.comparator_;

		TBSTreeIterator< TBSTreeItem<DataType> > iterator(tree.root_, walk_order);
		
		for (const TBSTreeItem<DataType> *item = iterator.forward();
				 item != 0; item = iterator.forward())
		{
			insert(item->getData(), true);
		}
	}

	template<typename DataType>
	BALL_INLINE 
	TBSTree<DataType>& TBSTree<DataType>::operator = (const TBSTree<DataType>& tree)
	{
		if (this != &tree)
		{
			set(tree);
		}
		return *this;
	}

	template<typename DataType>
	BALL_INLINE
	void TBSTree<DataType>::get(TBSTree<DataType>& tree, bool deep) const
	{
		tree.set(*this, deep);
	}

	template<typename DataType>
	BALL_INLINE 
	void TBSTree<DataType>::get(TBSTree<DataType>& tree, BSTreeIterator::WalkOrder walk_order) const
	{
		tree.set(*this, walk_order);
	}

	template<typename DataType>
	BALL_INLINE 
	void TBSTree<DataType>::swap(TBSTree<DataType>& tree)
	{
		TBSTreeItem<DataType>* root_item = root_;
		root_ = tree.root_;
		tree.root_ = root_item;

		const Comparator<DataType>* temp = comparator_;
		comparator_ = tree.comparator_;
		tree.comparator_ = temp;
	}

	template <class DataType>
	BALL_INLINE 
	void TBSTree<DataType>::setComparator(const Comparator<DataType>& comparator)
	{
		comparator_ = &comparator;
	}

	template <class DataType>
	BALL_INLINE 
	void TBSTree<DataType>::resetComparator()
	{
		comparator_ = &(RTTI::getDefault<Comparator<DataType> >());
	}

	template <class DataType>
	BALL_INLINE 
	const Comparator<DataType>* TBSTree<DataType>::getComparator() const
	{
		return comparator_;
	}

	template<typename DataType>
	BALL_INLINE 
	const TBSTreeItem<DataType>* TBSTree<DataType>::getRoot() const
	{
		return root_;
	}
			
	template<typename DataType>
	BALL_INLINE 
	Size TBSTree<DataType>::getHeight() const
	{
		if (root_ == 0)
		{
			return 0;
		}
		return root_->getHeight();
	}
			
	template<typename DataType>
	BALL_INLINE 
	Size TBSTree<DataType>::getSize() const
	{
		if (root_ == 0)
		{
			return 0;
		}
		return root_->getSize();
	}
			
	template<typename DataType>
	BALL_INLINE 
	const DataType* TBSTree<DataType>::getMinimum() const
	{
		if (root_ == 0) 
		{
			return 0;
		}
		return &(((TBSTreeItem<DataType> *)root_->getMinimum())->getData());
	}

	template<typename DataType>
	BALL_INLINE 
	const DataType* TBSTree<DataType>::getMaximum() const
	{
		if (root_ == 0) 
		{
			return 0;
		}
		return &(((TBSTreeItem<DataType> *)root_->getMaximum())->getData());
	}

	template<typename DataType>
	const DataType* TBSTree<DataType>::find(const DataType &data) const
	{
		if (root_ != 0)
		{
			TBSTreeItem<DataType> *item = root_;
				 
			for (; item != 0 && (comparator_->isNotEqual(data, item->getData()) == true);
					 item = (comparator_->isLess(data, item->getData()) == true) 
						? item->getLeftChild() 
						: item->getRightChild())
			{
			}

			if (item != 0)
			{
				return &(item->getData());
			}
		}

		return 0;
	}

	template<typename DataType>
	BALL_INLINE 
	Size TBSTree<DataType>::count(const DataType &data) const
	{
		if (root_ == 0)
		{
			return 0;
		}
		return root_->count(data, comparator_);
	}

	template<typename DataType>
	TBSTreeItem<DataType>* TBSTree<DataType>::insert(const DataType& data, bool multiple)
	{
		TBSTreeItem<DataType> *parent = 0;
		TBSTreeItem<DataType> *item = root_;
		bool right_child = false;

		if (multiple == true)
		{
			while (item) 
			{
				parent = item;
				if (comparator_->isLessOrEqual(data, item->getData()) == true)
				{
					right_child = false;
					item = item->getLeftChild();
				}
				else 
				{ 
					right_child = true;
					item = item->getRightChild();
				}
			}
		}
		else 
		{
			while (item) 
			{
				if (comparator_->isEqual(data, item->getData()) == true)
				{
					return item;
				}

				parent = item;
				if (comparator_->isLess(data, item->getData()) == true)
				{
					right_child = false;
					item = item->getLeftChild();
				}
				else 
				{
					right_child = true;
					item = item->getRightChild();
				}
			}
		}

		item = (TBSTreeItem<DataType> *)newItem(data, 0, item);
		if (parent) 
		{
			if (right_child)
			{
				parent->setRightChild(item);
			}
			else 
			{
				parent->setLeftChild(item);
			}
		}
		else 
		{
			root_ = item;
		}

		return item;
	}

	template<typename DataType>
	TBSTreeItem<DataType>* TBSTree<DataType>::detach(const DataType& data)
	{
		bool right_child = false;
		TBSTreeItem<DataType> *parent = 0;
		TBSTreeItem<DataType> *item = root_;

		while (item) 
		{
			if (comparator_->isEqual(data, item->getData()) == true)
			{
				break;
			}
			
			parent = item;
			
			if (comparator_->isLess(data, item->getData()) == true)
			{
				right_child = false;
				item = item->getLeftChild();
			}
			else 
			{
				right_child = true;
				item = item->getRightChild();
			}
		}

		return (TBSTreeItem<DataType> *)BSTreeItem::detachNode
			((BSTreeItem *&)root_, item, parent, right_child);
	}

	template<typename DataType>
	BALL_INLINE 
	TBSTreeItem<DataType>* TBSTree<DataType>::detachMinimum()
	{
		BSTreeItem* parent = root_->getParentOfMinimum();
		
		if (parent && parent->getLeftChild())
		{
			return (TBSTreeItem<DataType> *)BSTreeItem::detachNode
				((BSTreeItem *&)root_, parent->getLeftChild(), parent, false);
		}

		return (TBSTreeItem<DataType> *)BSTreeItem::detachNode
			((BSTreeItem *&)root_, root_, 0, false);
	}

	template<typename DataType>
	BALL_INLINE 
	TBSTreeItem<DataType>* TBSTree<DataType>::detachMaximum()
	{
		BSTreeItem* parent = root_->getParentOfMaximum();
		
		if (parent && parent->getRightChild())
		{
			return (TBSTreeItem<DataType> *)BSTreeItem::detachNode
				((BSTreeItem *&)root_, parent->getRightChild(), parent, true);
		}

		return (TBSTreeItem<DataType> *)BSTreeItem::detachNode
			((BSTreeItem *&)root_, root_, 0, false);
	}

	template<typename DataType>
	BALL_INLINE 
	bool TBSTree<DataType>::remove(const DataType &data)
	{
		if (root_ == 0)
		{
			return false;
		}

		TBSTreeItem<DataType>* item = detach(data);
		if (item != 0)
		{
			deleteItem(item);
			return true;
		}
		return false;
	}

	template<typename DataType>
	bool TBSTree<DataType>::removeAll(const DataType &data)
	{
		if (root_ == 0)
		{
			return false;
		}

		TBSTreeItem<DataType> *item = detach(data);

		if (item != 0)
		{
			for(; item != 0; item = detach(data))
			{
				deleteItem(item);
			}
			return true;
		}
		return false;
	}

	template<typename DataType>
	BALL_INLINE 
	bool TBSTree<DataType>::removeMinimum()
	{
		TBSTreeItem<DataType>* item = detachMinimum();
		if (item != 0)
		{
			deleteItem(item);
			return true;
		}
		return false;
	}

	template<typename DataType>
	BALL_INLINE 
	bool TBSTree<DataType>::removeMaximum()
	{
		TBSTreeItem<DataType>* item = detachMaximum();
		if (item != 0)
		{
			deleteItem(item);
			return true;
		}
		return false;
	}

	template <class DataType>
	BALL_INLINE 
	void TBSTree<DataType>::host(Visitor< TBSTree<DataType> >& visitor)
	{
		visitor.visit(*this);
	}

	template<typename DataType>
	bool TBSTree<DataType>::operator == (const TBSTree<DataType>& tree) const
	{
		TBSTreeIterator< TBSTreeItem<DataType> > this_iterator(root_);
		
		TBSTreeIterator< TBSTreeItem<DataType> > iterator(tree.root_);
		
		const TBSTreeItem<DataType>* this_item = this_iterator.forward();
		const TBSTreeItem<DataType>* item = iterator.forward();

		for (; this_item != 0 && item != 0; this_item = this_iterator.forward(), item = iterator.forward())
		{
			if (comparator_->isNotEqual(this_item->getData(), item->getData()) == true)
			{
				return false;
			}
		}

		return (bool)(this_item == item);
	}

	template<typename DataType>
	BALL_INLINE 
	bool TBSTree<DataType>::operator != (const TBSTree<DataType>& tree) const
	{
		return (bool)!(*this == tree);
	}

	template<typename DataType>
	BALL_INLINE 
	bool TBSTree<DataType>::has(const DataType& data) const
	{
		return (bool)(find(data) != 0);
	}

	template<typename DataType>
	BALL_INLINE 
	bool TBSTree<DataType>::isEmpty() const
	{
		return (bool)(root_ == 0);
	}

	template<typename DataType>
	BALL_INLINE 
	bool TBSTree<DataType>::isValid() const
	{
		return (bool)(isValid_(0, root_) == true && comparator_ != 0);
	}

	template<typename DataType>
	void TBSTree<DataType>::dump(std::ostream& s, Size depth) const
	{
		BALL_DUMP_STREAM_PREFIX(s);

		BALL_DUMP_DEPTH(s, depth);
		BALL_DUMP_HEADER(s, this, this);

		BALL_DUMP_DEPTH(s, depth);
		s << "  comparator: " << (void *)comparator_ << endl;

		BALL_DUMP_DEPTH(s, depth);
		s << "  height: " << getHeight() << endl;

		BALL_DUMP_DEPTH(s, depth);
		s << "  size: " << getSize() << endl;

		BALL_DUMP_DEPTH(s, depth);
		s << "  items: " << endl;

		BALL_DUMP_DEPTH(s, depth);
		s << "    root: ";
		dump_(s, depth + 1, root_);
		
		BALL_DUMP_STREAM_SUFFIX(s);
	}

	template<typename DataType>
	BALL_INLINE 
	bool TBSTree<DataType>::applyPreorder(UnaryProcessor<DataType>& processor)
	{
		if (root_ != 0)
		{
			return root_->applyPreorder(processor);
		}
		return true;
	}
			
	template<typename DataType>
	BALL_INLINE 
	bool TBSTree<DataType>::applyInorder(UnaryProcessor<DataType>& processor)
	{
		if (root_ != 0)
		{
			return root_->applyInorder(processor);
		}
		return true;
	}

	template<typename DataType>
	BALL_INLINE 
	bool TBSTree<DataType>::applyPostorder(UnaryProcessor<DataType>& processor)
	{ 
		if (root_ != 0)
		{
			return root_->applyPostorder(processor);
		}
		return true;
	}

	template<typename DataType>
	BALL_INLINE 
	bool TBSTree<DataType>::applyLevelorder(UnaryProcessor<DataType>& processor)
	{
		if (root_ != 0)
		{
			return root_->applyLevelorder(processor);
		}
		return true;
	}

	template<typename DataType>
	BALL_INLINE 
	bool TBSTree<DataType>::applyPreorderFlat(UnaryProcessor<DataType>& processor)
	{
		if (root_ != 0)
		{
			return root_->applyPreorderFlat(processor);
		}
		return true;
	}

	template<typename DataType>
	BALL_INLINE 
	bool TBSTree<DataType>::applyInorderFlat(UnaryProcessor<DataType>& processor)
	{
		if (root_ != 0)
		{
			return root_->applyInorderFlat(processor);
		}
		return true;
	}

	template<typename DataType>
	BALL_INLINE 
	bool TBSTree<DataType>::applyPostorderFlat(UnaryProcessor<DataType>& processor)
	{
		if (root_ != 0)
		{
			return root_->applyPostorderFlat(processor);
		}
		return true;
	}

	template<typename DataType>
	BALL_INLINE 
	bool TBSTree<DataType>::apply(UnaryProcessor<DataType>& processor)
	{
		return applyPreorder(processor);
	}

	template<typename DataType>
	BSTreeItem* TBSTree<DataType>::newItem
		(const DataType& data, BSTreeItem* left_item, 
		 BSTreeItem* right_item, char color) const
	{
		return new TBSTreeItem<DataType>
			(data, (TBSTreeItem<DataType>*)left_item, 
			 (TBSTreeItem<DataType>*)right_item, color);
	}

	template<typename DataType>
	void TBSTree<DataType>::deleteItem(BSTreeItem *item) const
	{
		delete item;
	}

	template<typename DataType>
	BSTreeItem* TBSTree<DataType>::clone_(const BSTreeItem *item) const
	{
		if (item == 0)
		{
			return 0;
		}

		return newItem
			(((TBSTreeItem<DataType> *)item)->getData(), 
			 clone_(item->getLeftChild()), clone_(item->getRightChild()), item->getColor());
	}

	template<typename DataType>
	void TBSTree<DataType>::destroy_(TBSTreeItem<DataType>* item)
	{
		if (item != 0)
		{
			destroy_(item->getLeftChild());
			destroy_(item->getRightChild());
			deleteItem(item);
		}
	}

	template<typename DataType>
	bool TBSTree<DataType>::isValid_
		(const TBSTreeItem<DataType>* parent,
		 const TBSTreeItem<DataType>* childitem) const
	{
		if (childitem != 0)
		{
			if (parent != 0)
			{
				if (parent->getLeftChild() == (BSTreeItem *)childitem)
				{
					if (comparator_->isLess(parent->getData(), childitem->getData()) == true)
					{
						return false;
					}
				}
				else 
				{
					if (parent->getRightChild() == (BSTreeItem *)childitem)
					{
						if (comparator_->isLess(childitem->getData(), parent->getData()) == true)
						{
							return false;
						}
					}
				}
			}
			
			if (!isValid_(childitem, (const TBSTreeItem<DataType> *)childitem->getLeftChild()) ||
					!isValid_(childitem, (const TBSTreeItem<DataType> *)childitem->getRightChild()))
			{
				return false;
			}
		}

		return true;
	}

	template <class DataType>
	void TBSTree<DataType>::dump_
		(std::ostream& s, Size depth, const TBSTreeItem<DataType>* item) const
	{
		BALL_DUMP_STREAM_PREFIX(s);

		if (item != 0)
		{
			s << item->getData() << " (" << (int)item->getColor() << ')' << endl;

			if (item->getRightChild() != 0)
			{
				BALL_DUMP_DEPTH(s, depth);
				s << "  r: ";
				dump_(s, depth + 1, (const TBSTreeItem<DataType> *)item->getRightChild());
			}

			if (item->getLeftChild() != 0)
			{
				BALL_DUMP_DEPTH(s, depth);
				s << "  l: ";
				dump_(s, depth + 1, (const TBSTreeItem<DataType> *)item->getLeftChild());
			}
		}

		BALL_DUMP_STREAM_SUFFIX(s);
	}

#	ifndef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/DATATYPE/binarySearchTree.iC>
#	endif

} // namespace BALL

#endif // BALL_DATATYPE_BINARYSEARCHTREE_H
