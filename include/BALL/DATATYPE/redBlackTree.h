// $Id: redBlackTree.h,v 1.2 1999/12/04 18:34:14 oliver Exp $

#ifndef BALL_DATATYPE_REDBLACKTREE_H
#define BALL_DATATYPE_REDBLACKTREE_H

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

namespace BALL {

	template<class DataType>
	class TRBTree: public TBSTree<DataType>
	{
		public:
		
			// --- CONSTRUCTORS

			TRBTree();

			TRBTree(const TRBTree& tree, bool deep = true);

			// --- DESTRUCTORS

			virtual ~TRBTree();

			// --- ASSIGNMENT METHODS

			void set(const TRBTree& tree, bool deep = true);

			void set(const TRBTree& tree, BSTreeIterator::WalkOrder walk_order);

			TRBTree& operator = (const TRBTree& tree);

			void get(TRBTree& tree, bool deep = true) const;

			void get(TRBTree& tree, BSTreeIterator::WalkOrder walk_order) const;
	
			// --- ACCESSORS: INSPECTORS and MUTATORS

			TBSTreeItem<DataType> *insert(const DataType& data, bool multiple = true);

			TBSTreeItem<DataType> *detach(const DataType& data);

			TBSTreeItem<DataType> *detachMinimum();

			TBSTreeItem<DataType> *detachMaximum();

			bool remove(const DataType& data);

			bool removeAll(const DataType &data);

			bool removeMinimum();

			bool removeMaximum();

			// --- MISCELLANEOUS

			void host(Visitor<TRBTree>& visitor);

	};

	template<class DataType>
	TRBTree<DataType>::TRBTree()
		:	TBSTree<DataType>()
	{
	}

	template<class DataType>
	TRBTree<DataType>::TRBTree(const TRBTree<DataType>& tree, bool deep)
		:	TBSTree<DataType>(tree, deep)
	{
	}

	template<class DataType>
	TRBTree<DataType>::~TRBTree()
	{
		clear();
	}

	template<class DataType>
	BALL_INLINE 
	void TRBTree<DataType>::set(const TRBTree& tree, bool deep)
	{
		TBSTree<DataType>::set(tree, deep);
	}

	template<class DataType>
	void TRBTree<DataType>::set(const TRBTree<DataType> &tree,  BSTreeIterator::WalkOrder walk_order)
	{
		clear();

		TBSTreeIterator< TBSTreeItem<DataType> > BS_tree_it (tree.root__, walk_order);
		
		for (register const TBSTreeItem<DataType> *tree_item = BS_tree_it.forward();
				 tree_item != 0; tree_item = BS_tree_it.forward())
		{
			insert(tree_item->data_, true);
		}
	}

	template<class DataType>
	BALL_INLINE 
	TRBTree<DataType> & TRBTree<DataType>::operator = (const TRBTree<DataType> &tree)
	{
		TBSTree<DataType>::set(tree);

		return *this;
	}

	template<class DataType>
	BALL_INLINE 
	void TRBTree<DataType>::get(TRBTree &tree, bool deep) const
	{
		TBSTree<DataType>::get(tree, deep);
	}

	template<class DataType>
	BALL_INLINE 
	void TRBTree<DataType>::get(TRBTree<DataType>& tree, BSTreeIterator::WalkOrder walk_order) const
	{
		tree.set(*this, walk_order);
	}

	template<class DataType>
	TBSTreeItem<DataType>*  TRBTree<DataType>::insert(const DataType& data, bool multiple)
	{
		BSTreeItem::Pack pp;
		bool side;

		pp.t = root_; 
		pp.p = pp.g = pp.gg = 0;

		while(pp.t) 
		{
			if (multiple == false && comparator_->isEqual(data, ((TBSTreeItem<DataType> *)(pp.t))->data_) == true)
				break;
			
			// If on a 4-node, we must split it into two 2-nodes
			if ((pp.t->left && pp.t->getLeftChild()->color == BSTreeItem::RED)
					&& (pp.t->right && pp.t->getRightChild()->color == BSTreeItem::RED)) 
			{
				pp.t->color = BSTreeItem::RED;
				pp.t->getLeftChild()->color = BSTreeItem::BLACK;
				pp.t->getRightChild()->color = BSTreeItem::BLACK;

				// Check for two reds in a row, and adjust if so
				if (pp.p && pp.p->color == BSTreeItem::RED)
					root_ = (TBSTreeItem<DataType> *)BSTreeItem::insertBalance(root_,pp);
			}
			
			pp.gg = pp.g;
			pp.g = pp.p;
			pp.p = pp.t;
			
			if (comparator_->isLessOrEqual(data, ((TBSTreeItem<DataType> *)(pp.t))->data_) == true)
			{
				pp.t = pp.t->getLeftChild(); 
				side = false;
			} else {
				pp.t = pp.t->getRightChild(); 
				side = true;
			}
		}

		// Reached the bottom, with no matching node
		if (pp.t == 0) 
		{
			pp.t = newItem(data, 0, 0, BSTreeItem::RED);
			if (pp.p) 
			{
				if (side)
				{
					pp.p->right = pp.t;
				} else {
					pp.p->left = pp.t;
				}
			} else {
				root_ = (TBSTreeItem<DataType> *)(pp.t);
			}

			// Check for two reds in a row, and adjust if so
			if (pp.p && pp.p->color == BSTreeItem::RED)
			{
				root_ = (TBSTreeItem<DataType> *)BSTreeItem::insertBalance(root_, pp);
			}
		}

		root_->color = BSTreeItem::BLACK; // Root always made black

		return (TBSTreeItem<DataType> *)(pp.t);
	}

	template<class DataType>
	TBSTreeItem<DataType> *
	TRBTree<DataType>::detach(const DataType& data)
	{
		if (root_ == 0) 
			return 0;

		BSTreeItem::Pack pp;
		pp.t = root_; 
		pp.p = pp.g = pp.m = pp.pm = 0;

		while (pp.t) 
		{
			// Go down the tree one level, searching for node to delete
			if (comparator_->isEqual(((TBSTreeItem<DataType> *)(pp.t))->data_, data ) == true)
			{
				pp.m = pp.t;  // Record matching node
				pp.pm = pp.p; // And it's parent
			}

			// Update ancestor pointers
			pp.g = pp.p; 
			pp.p = pp.t;

			if (comparator_->isLess(data, ((TBSTreeItem<DataType> *)(pp.t))->data_) == true)
			{
				pp.t = pp.p->getLeftChild(); 
				pp.s = pp.p->getRightChild();
			} else { // Walk down even if t matches, to look for successor
				pp.t = pp.p->getRightChild(); 
				pp.s = pp.p->getLeftChild();
			}

			if (pp.s) 
			{
				pp.ls = pp.s->getLeftChild(); 
				pp.rs = pp.s->getRightChild();
			} else {
				 pp.ls = 0; 
				 pp.rs = 0;
			}

			root_ = (TBSTreeItem<DataType> *)BSTreeItem::removeBalance(root_, pp);
		}

		root_ = (TBSTreeItem<DataType> *)BSTreeItem::replace(root_,pp);

		if (root_)
			root_->color = BSTreeItem::BLACK;

		return (TBSTreeItem<DataType> *)pp.m; // Node to delete
	}

	template<class DataType>
	BALL_INLINE TBSTreeItem<DataType> *
	TRBTree<DataType>::detachMinimum()
	{
		return (TBSTreeItem<DataType> *)BSTreeItem::detachMinimum((BSTreeItem *&)root_);
	}

	template<class DataType>
	BALL_INLINE 
	TBSTreeItem<DataType>* TRBTree<DataType>::detachMaximum()
	{
		return (TBSTreeItem<DataType> *)BSTreeItem::detachMaximum((BSTreeItem *&)root_);
	}

	template<class DataType>
	BALL_INLINE 
	bool TRBTree<DataType>::remove(const DataType &data)
	{
		if (root_ == 0)
			return false;

		TBSTreeItem<DataType> *tree_item = detach(data);
		if (tree_item != 0) 
		{
			deleteItem(tree_item);
			return true;
		} else {
			return false;
		}
	}

	template<class DataType>
	bool TRBTree<DataType>::removeAll(const DataType& data)
	{
		if (root_ == 0)
			return false;

		register TBSTreeItem<DataType> *tree_item = detach(data);

		if (tree_item != 0)
		{
			for(; tree_item != 0; tree_item = detach(data))
				deleteItem(tree_item);

			return true;
		} else {
			return false;
		}
	}

	template<class DataType>
	BALL_INLINE 
	bool TRBTree<DataType>::removeMinimum()
	{
		TBSTreeItem<DataType> *tree_item = detachMinimum();
		if (tree_item != 0) 
		{
			deleteItem(tree_item);
			return true;
		} else {
			return false;
		}
	}

	template<class DataType>
	BALL_INLINE 
	bool TRBTree<DataType>::removeMaximum()
	{
		TBSTreeItem<DataType> *tree_item = detachMaximum();
		if (tree_item != 0) 
		{
			deleteItem(tree_item);
			return true;
		} else {
			return false;
		}
	}

	template<class DataType>
	BALL_INLINE 
	void TRBTree<DataType>::host(Visitor< TRBTree<DataType> >& visitor)
	{
		visitor.visit(*this);
	}

} // namespace BALL

#endif // BALL_DATATYPE_REDBLACKTREE_H
