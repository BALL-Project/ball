// $Id: splayTree.C,v 1.1 1999/08/26 08:02:34 oliver Exp $

#include <BALL/DATATYPE/splayTree.h>

namespace BALL {

	// The tree at parent_item is truncated on the left, (ie. a cut
	// is made between parent_item and parent_item->left.) The cut tree (ie.
	// parent_item and all nodes to the right of parent_item), are added to the
	// right subtree right_item, becoming the left subtree of the
	// minimum node in right_item. All of the nodes in subtree right_item
	// are to the left of right_item. Since right_item->right is unused, we
	// use it to point directly to the minimum node.
	// parent_item->left becomes the new parent of the tree that parent_item
	// used to belong to, so we return parent_item->left.
	// Returns 0 if parent_item->left is 0.
	BSTreeItem* SplayTree::linkRight(BSTreeItem *parent_item, BSTreeItem *right_item)
	{
		BSTreeItem *newparent_item = parent_item->left;

		parent_item->left = 0;
		right_item->right->left = parent_item;
		right_item->right = parent_item;

		return newparent_item;
	}

	// The tree at parent_item is truncated at the right, (ie. a cut
	// is made between parent_item and parent_item->right.) The cut tree (ie.
	// parent_item and all nodes to the left of parent_item), are added to the
	// left subtree left_item, becoming the right subtree of the
	// maximum node in left_item. All of the nodes in subtree left_item
	// are to the right of left_item. Since left_item->left is unused, we
	// use it to point directly to the maximum node.
	// parent_item->right becomes the new parent of the tree that parent_item
	// used to belong to, so we return parent_item->right.
	// Returns 0 if parent_item->right is 0.
	BSTreeItem* SplayTree::linkLeft(BSTreeItem *parent_item, BSTreeItem *left_item)
	{
		BSTreeItem *newparent_item = parent_item->right;

		parent_item->right = 0;
		left_item->left->right = parent_item;
		left_item->left = parent_item;
	 
		return newparent_item;
	}

	// assembles parent_item, left_item, and right_item into one tree, 
	// with parent_item as the root, left_item & right_item 
	// as the left and right subtrees of parent_item, and the old
	// left and right subtrees of parent_item as right and left subtrees
	// of left_item & right_item, respectively.
	void SplayTree::assemble(BSTreeItem *parent_item, BSTreeItem *left_item, BSTreeItem *right_item)
	{
		if (left_item->right) 
		{
			left_item->left->right = parent_item->left;
			parent_item->left = left_item->right;
		}

		if (right_item->left) 
		{
			right_item->right->left = parent_item->right;
			parent_item->right = right_item->left;
		}
	}

	// Move the minimum of item up the tree, replacing item
	// as the root. The new root is returned.
	BSTreeItem* SplayTree::splayMinimum(BSTreeItem *item)
	{
		if (item == 0)
			return 0;

		BSTreeItem left_item, right_item; 

		left_item.left = &left_item; 
		left_item.right = 0;

		right_item.left = 0;  
		right_item.right = &right_item;

		while(item->left) 
		{ // Go left as far as possible, splaying along the way.
			// We have either a zig-zig, or a zig orientation

			if (item->left->left)
			{
				item = item->rotateRight();
			}

			item = linkRight(item, &right_item); // item->left guaranteed not null
		}

		assemble(item, &left_item, &right_item);

		return item;
	}

	BSTreeItem *
	SplayTree::splayMaximum
		(BSTreeItem *item)
	// Move the maximum of t up the tree, replacing item
	// as the root. The new root is returned.
	{
		if (item == 0)
		{
			return 0;
		}

		BSTreeItem left_item, right_item; 

		left_item.left = &left_item; 
		left_item.right = 0;

		right_item.left = 0;  
		right_item.right = &right_item;

		while(item->right) 
		{ // Go right as far as possible, splaying along the way
			// We have either a zig-zig, or a zig orientation

			if (item->right->right)
			{
				item = item->rotateLeft();
			}

			item = linkLeft(item, &left_item); // item->right guaranteed not null
		}

		assemble(item, &left_item, &right_item);

		return item;
	}


	// Detach the minimum node of the tree, returning
	// a pointer to it, or 0 if tree is empty.
	BSTreeItem* SplayTree::detachMinimum(BSTreeItem*& root)
	{  
		if (root == 0)
			return 0;

		root = splayMinimum(root);

		// Replace the minimum node, now at the root,
		// with it's successor, found by splaying the 
		// minimum node of the right subtree to the top.
		// If there is no right subtree, it means we're
		// empty, since the left child is always null.
		BSTreeItem *old_root_item = root;

		if (root->right) 
		{
			root = splayMinimum(root->right);
			root->left = old_root_item->left;
		}
		//// else root = root->left;
		else
		{
			root = 0;
		}

		return old_root_item;
	}


	BSTreeItem *
	SplayTree::detachMaximum(BSTreeItem *&root)
	// Detach the maximum node of the tree, returning
	// a pointer to it, or 0 if tree is empty.
	{
		if (root == 0)
		{
			return 0;
		}

		root = splayMaximum(root);

		// Replace the maximum node, now at the root,
		// with it's predecessor, found by splaying the 
		// maximum node of the left subtree to the top.
		// If there is no left subtree, it means we're
		// empty, since the right child is always null.
		BSTreeItem *old_root_item = root;

		if (root->left) 
		{
			root = splayMaximum(root->left);
			root->right = old_root_item->right;
		}
		////  else root = root->right;
		else
		{
			root = 0;
		}

		return old_root_item;
	}

} // namespace BALL
