// $Id: splayTree.h,v 1.4 2000/08/22 17:03:31 amoll Exp $

#ifndef BALL_DATATYPE_SPLAYTREE_H
#define BALL_DATATYPE_SPLAYTREE_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_CONCEPT_VISITOR_H
#	include <BALL/CONCEPT/visitor.h>
#endif

#ifndef BALL_DATATYPE_FOREACH_H
#	include <BALL/DATATYPE/forEach.h>
#endif

#ifndef BALL_DATATYPE_BSTREE_H
#	include <BALL/DATATYPE/binarySearchTree.h>
#endif

namespace BALL 
{

	/** Splay Tree Class.
	*/
	class SplayTree 
	{
		public:
		
			static BSTreeItem* linkRight(BSTreeItem* parent,  BSTreeItem* right);

			static BSTreeItem* linkLeft(BSTreeItem* parent,  BSTreeItem* left);

			static void assemble(BSTreeItem* parent, BSTreeItem* left, BSTreeItem* right);

			static BSTreeItem* splayMinimum(BSTreeItem*);

			static BSTreeItem* splayMaximum(BSTreeItem*);

			static BSTreeItem* detachMinimum(BSTreeItem*& root);

			static BSTreeItem* detachMaximum(BSTreeItem*& root);

	};



	/**	Splay Tree Class */
	template<class DataType>
	class TSplayTree : public TBSTree<DataType>
	{
		public:
		
			/**	@name	Constructors and Destructors */
			//@{

			/// Default constructor
			TSplayTree();

			/// Copy constructor
			TSplayTree(const TSplayTree& splay_tree, bool deep = true);

			/// Destructor
			virtual ~TSplayTree();
			//@}
		
			/**	@name	Assignment */
			//@{

			///
			//void set(const TSplayTree& splay_tree, bool deep = true);

			///
			//void set(const TSplayTree &splay_tree,  BSTreeIterator::WalkOrder walk_order);

			///
			//TSplayTree& operator = (const TSplayTree& splay_tree);

			///
			//void get(TSplayTree& splay_tree, bool deep = true) const;

			///
			//void get(TSplayTree& splay_tree, BSTreeIterator::WalkOrder walk_order) const;
			
			//@}

			/**	@name	 Accessors */
			//@{

			///
			const DataType* getMinimum();

			///
			//const DataType* getMinimum() const;

			///
			const DataType* getMaximum();

			///
			//const DataType* getMaximum() const;

			///
			const DataType* find(const DataType& data);

			///
			const DataType* find(const DataType& data) const;

			///
			TBSTreeItem<DataType>* insert(const DataType& data, bool multiple = true);

			///
			TBSTreeItem<DataType>* insertNotSplayed(const DataType& data, bool multiple = true);

			///
			TBSTreeItem<DataType>* detach(const DataType& data);

			///
			TBSTreeItem<DataType>* detachMinimum();

			///
			TBSTreeItem<DataType>* detachMaximum();

			///
			bool remove(const DataType& data);

			///
			bool removeAll(const DataType& data);

			///
			bool removeMinimum();

			///
			bool removeMaximum();
			//@}

			/**	@name	Miscellaneous */
			//@{

			///
			//void host(Visitor<TSplayTree>& visitor);
			//@}

			/**	@name	Predicates */
			//@{

			///
			bool has(const DataType& data);

			///
			bool has(const DataType& data) const;
			//@}

		protected:
			
		private:

			void findAndSplay_(const DataType& data); 
	};

	template<class DataType>
	BALL_INLINE 
	TSplayTree<DataType>::TSplayTree()
			:	TBSTree<DataType>()
	{
	}

	template<class DataType>
	BALL_INLINE 
	TSplayTree<DataType>::TSplayTree
		(const TSplayTree<DataType> &splay_tree, bool deep)
			:	TBSTree<DataType>(splay_tree, deep)
	{
	}

	template<class DataType>
	BALL_INLINE 
	TSplayTree<DataType>::~TSplayTree()
	{
		clear();
	}
/*
	template<class DataType>
	BALL_INLINE 
	void TSplayTree<DataType>::set(const TSplayTree &splay_tree, bool deep)
	{
		TBSTree<DataType>::set(splay_tree, deep);
	}

	template<class DataType>
	BALL_INLINE 
	void TSplayTree<DataType>::set
		(const TSplayTree<DataType> &splay_tree, BSTreeIterator::WalkOrder walk_order)
	{
		clear();
		
		TBSTreeIterator< TBSTreeItem<DataType> > tree_it((TBSTreeItem<DataType> *)splay_tree.root_, walk_order);
		
		for (const TBSTreeItem<DataType>* item = tree_it.forward();
				 item != 0; item = tree_it.forward())
		{
			insert(item->data_, true);
		}
	}

	template<class DataType>
	BALL_INLINE 
	TSplayTree<DataType>& TSplayTree<DataType>::operator = (const TSplayTree<DataType>& splay_tree)
	{
		TBSTree<DataType>::set(splay_tree);

		return *this;
	}

	template<class DataType>
	BALL_INLINE 
	void TSplayTree<DataType>::get(TSplayTree& splay_tree, bool deep) const
	{
		TBSTree<DataType>::get(splay_tree, deep);
	}

	template<class DataType>
	BALL_INLINE 
	void TSplayTree<DataType>::get(TSplayTree<DataType>& splay_tree, BSTreeIterator::WalkOrder walk_order) const
	{
		splay_tree.set(*this, walk_order);
	}
*/
	template<class DataType>
	BALL_INLINE 
	const DataType* TSplayTree<DataType>::getMinimum()
	{
		if (root_ == 0)
		{
			return 0;
		}
		else
		{
			root_ = (TBSTreeItem<DataType> *)splayMinimum(root_);

			return &(((TBSTreeItem<DataType> *)root_)->data_);
		}
	}
/*
	template<class DataType>
	BALL_INLINE 
	const DataType* TSplayTree<DataType>::getMinimum() const
	{
		return TBSTree<DataType>::getMinimum();
	}
*/
	template<class DataType>
	BALL_INLINE 
	const DataType* TSplayTree<DataType>::getMaximum()
	{
		if (root_ == 0)
		{
			return 0;
		}
		else
		{
			root_ = (TBSTreeItem<DataType> *)SplayTree::splayMaximum(root_);

			return &(((TBSTreeItem<DataType> *)root_)->data_);
		}
	}
/*
	template<class DataType>
	BALL_INLINE 
	const DataType* TSplayTree<DataType>::getMaximum() const
	{
		return TBSTree<DataType>::getMaximum();
	}
*/
	template<class DataType>
	BALL_INLINE 
	const DataType* TSplayTree<DataType>::find(const DataType& data)
	{
		if (root_ != 0)
		{
			findAndSplay_(data);

			if (comparator_->isEqual(root_->data_, data) == true)
			{
				return &(root_->data_);
			}
		}
			 
		return 0;
	}

	template<class DataType>
	BALL_INLINE 
	const DataType* TSplayTree<DataType>::find(const DataType& data) const
	{
		return TBSTree<DataType>::find(data);
	}

	template<class DataType>
	TBSTreeItem<DataType>* TSplayTree<DataType>::insert(const DataType& data, bool multiple)
	{  
		TBSTreeItem<DataType>* item = 0;

		findAndSplay_(data); // Moves closest match to the root
		
		if (multiple == true || !(root_ && (comparator_->isEqual(root_->data_, data) == true))) 
		{ 
			item = (TBSTreeItem<DataType> *)newItem(data);

			// item is about to become the new root
			if (root_) 
			{ // Determine which side of item the old root should go.
			 
				if (comparator_->isLess(data, root_->data_) == true)
				{ 
					item->right = root_; 
					item->left = root_->left;
					root_->left = 0;
				}
				else 
				{ 
					item->left = root_; 
					item->right = root_->right;
					root_->right = 0;
				}
			}
			root_ = item;
		}

		return item;
	}

	template<class DataType>
	BALL_INLINE 
	TBSTreeItem<DataType>* TSplayTree<DataType>::insertNotSplayed(const DataType& data, bool multiple)
	{
		return TBSTree<DataType>::insert(data, multiple);
	}

	template<class DataType>
	TBSTreeItem<DataType>* TSplayTree<DataType>::detach(const DataType& data)
	{  
		if (root_ == 0)
		{
			return 0;
		}
		
		findAndSplay_(data);
		
		if (comparator_->isEqual(root_->data_, data) == true)
		{ // Replace the matching node, now at the root,
			// with it's successor, found by splaying the 
			// minimum node of the right subtree to the top.
			// If there is no right subtree, use the left 
			// child as the replacement.
			 TBSTreeItem<DataType>* oldroot = root_;

			 if (root_->right)
			 {
				 root_ = (TBSTreeItem<DataType> *)SplayTree::splayMinimum(root_->right);
				 root_->left = oldroot->left;
			 }
			 else
			 {
				 root_ = (TBSTreeItem<DataType> *)root_->left;
			 }
			 
			 return oldroot;
		}
		
		return 0;
	}

	template<class DataType>
	BALL_INLINE 
	TBSTreeItem<DataType>* TSplayTree<DataType>::detachMinimum()
	{
		return (TBSTreeItem<DataType> *)SplayTree::detachMinimum((BSTreeItem *&)root_);
	}

	template<class DataType>
	BALL_INLINE 
	TBSTreeItem<DataType>* TSplayTree<DataType>::detachMaximum()
	{
		return (TBSTreeItem<DataType> *)SplayTree::detachMaximum((BSTreeItem *&)root_);
	}

	template<class DataType>
	BALL_INLINE 
	bool TSplayTree<DataType>::remove(const DataType& data)
	{  
		if (root_ == 0)
		{
			return 0;
		}

		TBSTreeItem<DataType> *item = detach(data);
		if (item) 
		{
			deleteItem(item);
			return true;
		}
		else
		{
			return false;
		}
	}

	template<class DataType>
	BALL_INLINE 
	bool TSplayTree<DataType>::removeAll(const DataType& data)
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
		else
		{
			return false;
		}
	}

	template<class DataType>
	BALL_INLINE 
	bool TSplayTree<DataType>::removeMinimum()
	{
		TBSTreeItem<DataType>* item = detachMinimum();
		if (item != 0)
		{
			deleteItem(item);
			return true;
		}
		else
		{
			return false;
		}
	}

	template<class DataType>
	BALL_INLINE 
	bool TSplayTree<DataType>::removeMaximum()
	{
		TBSTreeItem<DataType>* item = detachMaximum();
		if (item != 0)
		{
			deleteItem(item);
			return true;
		}
		else
		{
			return false;
		}
	}
/*
	template <class DataType>
	BALL_INLINE 
	void TSplayTree<DataType>::host(Visitor< TSplayTree<DataType> >& visitor)
	{
		visitor.visit(*this);
	}*/

	template<class DataType>
	BALL_INLINE 
	bool TSplayTree<DataType>::has(const DataType& data)
	{
		return (bool)(find(data) != 0);
	}

	template<class DataType>
	BALL_INLINE 
	bool TSplayTree<DataType>::has(const DataType &data) const
	{
		return (bool)(find(data) != 0);
	}

	template<class DataType>
	void TSplayTree<DataType>::findAndSplay_(const DataType& data)
	// Search for node containing the key data, moving that
	// node to the root using top-down splaying.
	{
		if (root_ == 0)
		{
			return;
		}

		BSTreeItem left, right; // Temporary subtrees holding no data

		left.left = &left; 
		left.right = 0;

		right.left = 0;  
		right.right = &right;

		while (comparator_->isNotEqual(data, root_->data_) == true)
		{
			if (comparator_->isLess(data, root_->data_) == true)
			{ // Search left down the tree
				if (root_->left) 
				{
					if (comparator_->isEqual(data, root_->getLeftChild()->data_) == true)
					{ // zig
						root_ = (TBSTreeItem<DataType> *)SplayTree::linkRight(root_, &right);
					}
					else if (comparator_->isLess(data, root_->getLeftChild()->data_) == true) 
					{ // zig-zig
						root_ = (TBSTreeItem<DataType> *)root_->rotateRight();
						if (root_->left)
						{
							root_ = (TBSTreeItem<DataType> *)SplayTree::linkRight(root_, &right);
						}
					}
					else if (comparator_->isLess(root_->getLeftChild()->data_, data) == true)
					{ // zig-zag
						root_ = (TBSTreeItem<DataType> *)SplayTree::linkRight(root_, &right);
			
						if (root_->right)
						{
							root_ = (TBSTreeItem<DataType> *)SplayTree::linkLeft(root_, &left);
						}
					}
				}
				else
				{
					break; // Node isn't in the tree
				}
			}
			else 
			{ // search right down the tree

				if (root_->right) 
				{
					if (comparator_->isEqual(data, root_->getRightChild()->data_) == true) 
					{ // zig
						root_ = (TBSTreeItem<DataType> *)SplayTree::linkLeft(root_, &left);
					}
					else if (comparator_->isLess(root_->getRightChild()->data_, data) == true)
					{ // zig-zig
						root_ = (TBSTreeItem<DataType> *)root_->rotateLeft();
			
						if (root_->right)
						{
							root_ = (TBSTreeItem<DataType> *)SplayTree::linkLeft(root_, &left);
						}
					}
					else if (comparator_->isLess(data, root_->getRightChild()->data_) == true)
					{ // zig-zag
						root_ = (TBSTreeItem<DataType> *)SplayTree::linkLeft(root_, &left);
			
						if (root_->left)
						{
							root_ = (TBSTreeItem<DataType> *)SplayTree::linkRight(root_, &right);
						}
					}
				} 
				else
				{
					break; // Node isn't in the tree
				}
			}
		}

		SplayTree::assemble(root_, &left, &right);
	}

} // namespace BALL

#endif // BALL_DATATYPE_SPLAYTREE_H
