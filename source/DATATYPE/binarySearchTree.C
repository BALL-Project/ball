// $Id: binarySearchTree.C,v 1.1 1999/08/26 08:02:34 oliver Exp $

#include <BALL/DATATYPE/binarySearchTree.h>

namespace BALL 
{

	Size BSTreeItem::getSize() const
	{
		const BSTreeItem *item = this;
		Size size = 1;

		while(item) 
		{
			if (item->left)
			{
				size += item->left->getSize();
			}
			
			if (item->right) 
			{
				++size; 
			}
			
			item = item->right;
		}

		return size;
	}

	Size BSTreeItem::getHeight() const
	{
		Size lhsize = (left) ? left->getHeight() : 0;
		Size rhsize = (right) ? right->getHeight() : 0;

		return (((lhsize > rhsize) ? lhsize : rhsize) + 1);
	}

	Processor::Result BSTreeItem::applyPreorder_(UnaryProcessor<BSTreeItem>& processor)
	{
		Processor::Result result = Processor::ABORT;
		BSTreeItem *item = this;

		while(item) 
		{
			result = processor(*item);
				
			if (result <= Processor::BREAK)
			{
				return result;
			}
			
			if (item->left)
			{
				result = item->left->applyPreorder_(processor);
				
				if (result <= Processor::BREAK)
				{
					return result;
				}
			}
			
			item = item->right;
		}

		return Processor::CONTINUE;
	}

	Processor::Result 
	BSTreeItem::applyInorder_(UnaryProcessor<BSTreeItem>& processor)
	{
		Processor::Result result = Processor::ABORT;
		BSTreeItem *item = this;

		while(item) 
		{
			if (item->left)
			{
				result = item->left->applyInorder_(processor);
				
				if (result <= Processor::BREAK)
				{
					return result;
				}
			}
			
			result = processor(*item);
				
			if (result <= Processor::BREAK)
			{
				return result;
			}
			
			item = item->right;
		}

		return Processor::CONTINUE;
	}

	Processor::Result BSTreeItem::applyPostorder_(UnaryProcessor<BSTreeItem>& processor)
	{
		Processor::Result result = Processor::ABORT;

		if (left)
		{
			result = left->applyPostorder_(processor);
				
			if (result <= Processor::BREAK)
			{
				return result;
			}
		}
		
		if (right)
		{
			result = right->applyPostorder_(processor);
				
			if (result <= Processor::BREAK)
			{
				return result;
			}
		}
		
		return processor(*this);
	}

	Processor::Result BSTreeItem::applyPreorderFlat_(UnaryProcessor<BSTreeItem>& processor)
	{
		Processor::Result result = Processor::ABORT;
		TSStack<BSTreeItem *> path_;
		BSTreeItem *item = this;
		bool loop = true;

		while (loop)
		{
			if (item) 
			{
				 result = processor(*item);
				 
				 if (result <= Processor::BREAK)
				 {
						return result;
				 }

				 path_.push(item);
				 item = item->left;
			}
			else 
			{
				if (path_.pop(item) == 0)
				{
					loop = false;
					break;
				}
				
				item = item->right;
			}
		}

		return Processor::CONTINUE;
	}

	Processor::Result BSTreeItem::applyInorderFlat_(UnaryProcessor<BSTreeItem>& processor)
	{
		Processor::Result result = Processor::ABORT;
		TSStack<BSTreeItem *> path_;
		BSTreeItem *item = this;
		bool loop = true;

		while (loop)
		{
			if (item) 
			{
				path_.push(item);
				item = item->left;
			}
			else 
			{
				if (path_.pop(item) == 0)
				{
					loop = false;
					break;
				}
				
				result = processor(*item);

				if (result <= Processor::BREAK)
				{
					return result;
				}
				
				item = item->right;
			}
		}

		return Processor::CONTINUE;
	}

	Processor::Result BSTreeItem::applyPostorderFlat_(UnaryProcessor<BSTreeItem>& processor)
	{
		Processor::Result result = Processor::ABORT;
		TSStack<BSTreeItem *> path_;
		BSTreeItem *item = this;
		bool state = false;
		// just to prevent tedious compiler remarks like "control expression is constant"
		bool loop = true;

		while (loop)
		{
			if (state == false) 
			{ 
				if (item) 
				{
					path_.push(item);
					item = item->left;
				}
				else 
				{
					state = true;
				}
			}
			else 
			{ 
				BSTreeItem *leftitem = item;
				
				if (path_.isEmpty())
				{
					loop = false;
					break;
				}
				
				item = *path_.top();
				
				if (leftitem == item->left && item->right)
				{
					item = item->right;
					state = false;
				}
				else 
				{
		result = processor(*item);
		
		if (result <= Processor::BREAK)
		{
			return result;
		}

		path_.pop();
				}
			}
		}
		
		return Processor::CONTINUE;
	}

	Processor::Result BSTreeItem::applyLevelorder_(UnaryProcessor<BSTreeItem>& processor)
	{
		Processor::Result result = Processor::ABORT;
		TSStack<BSTreeItem *> path_;
		BSTreeItem *parentitem = 0;
		// just to prevent tedious compiler remarks like "control expression is constant"
		bool loop = true;

		path_.pushBottom(this);

		while(loop) 
		{
			if (path_.pop(parentitem) == 0)
			{
				loop = false;
				break;
			}
			
			result = processor(*parentitem);

			if (result <= Processor::BREAK)
				return result;
			
			if (parentitem->left)
				path_.pushBottom(parentitem->left);
			
			if (parentitem->right)
				path_.pushBottom(parentitem->right);
		}

		return Processor::CONTINUE;
	}


	#ifdef OLD

	BSTreeItem *
	BSTreeItem::rotateRight
		()
	{
		BSTreeItem *item = this;

		if (item->left != 0) 
		{
			BSTreeItem *p = item;
			item = item->left;
			p->left = item->right;
			item->right = p;
		}

		return item;
	}

	BSTreeItem *
	BSTreeItem::rotateLeft
		()
	{
		BSTreeItem *item = this;

		if (item->right != 0) 
		{
			BSTreeItem *p = item;
			item = item->right;
			p->right = item->left;
			item->left = p;
		}

		return item;
	}

	#else

	BSTreeItem *
	BSTreeItem::rotateRight
		()
	// PRECONDITION left child of p exists.
	{
		BSTreeItem *p = this;
		BSTreeItem *item = p->left;
		p->left = item->right;
		item->right = p;
		return item;
	}

	BSTreeItem *
	BSTreeItem::rotateLeft
		()
	// PRECONDITION right child of p exists.
	{
		BSTreeItem *p = this;
		BSTreeItem *item = p->right;
		p->right = item->left;
		item->left = p;
		return item;
	}

	#endif

	const BSTreeItem *
	BSTreeItem::getMinimum
		() const
	{
		const BSTreeItem *item = this;

		while(item->left)
		{
			item = item->left;
		}

		return item;
	}

	const BSTreeItem *
	BSTreeItem::getMaximum
		() const
	{
		const BSTreeItem *item = this;

		while(item->right)
		{
			item = item->right;
		}

		return item;
	}


	BSTreeItem *
	BSTreeItem::getParentOfMinimum
		()
	{
		BSTreeItem *item = this;
		BSTreeItem *p = 0;

		while(item->left) 
		{
			p = item;
			item = item->left;
		}

		return p;
	}

	BSTreeItem *
	BSTreeItem::getParentOfMaximum
		()
	{
		BSTreeItem *item = this;
		BSTreeItem *p = 0;

		while(item->right) 
		{
			p = item;
			item = item->right;
		}

		return p;
	}

	BSTreeItem *
	BSTreeItem::getParentOfPredecessor
		()
	// Returns parent of predecessor of item, assumed to be 
	// a binary search tree. Predecessor is the right child
	// of node returned, unless item itself is the parent. Then 
	// the left child is the predecessor. If t is a leaf, a 
	// 0 is returned. ASSUMES item isn't null.
	{
		BSTreeItem *item = this;
		BSTreeItem *p = 0;
		// Go left, then all the way right
		BSTreeItem *q = item->left;

		if (q) 
		{
			p = item;
			while(q->right) 
			{
				p = q;
				q = q->right;
			}
		}

		return p;
	}

	BSTreeItem *
	BSTreeItem::getParentOfSuccessor
		()
	// Returns parent of successor of item, assumed to be 
	// a binary search tree. Successor is the left child
	// of node returned, unless item itself is the parent.
	// Then the right child is the successor. If item is a 
	// leaf, a 0 is returned. ASSUMES t isn't null.
	{
		BSTreeItem *item = this;
		BSTreeItem *p = 0;
		// Go right, then all the way left
		BSTreeItem *q = item->right;

		if (q) 
		{
			p = item;
			while(q->left) 
			{
				p = q;
				q = q->left;
			}
		}

		return p;
	}

	BSTreeItem *
	BSTreeItem::detachNode
		(BSTreeItem *&root, 
		 BSTreeItem *t, 
		 BSTreeItem *p, 
		 bool right_side)
	// Detaches node item with parent p from the tree. Node item is
	// the left child if right_side = false, else it's the right child.
	// If p is 0, it means item is the root, and that is handled
	// accordingly. Redundantly returns the pointer item. May
	// have to update root pointer.
	{
		BSTreeItem *psucc = 0, *replacement = 0;

		if (t) {
			 if (t->left == 0 || t->right == 0) {
					// At least one child is null, so use the other 
					// as the replacement. (It may be null too.)
					replacement = (t->left) ? t->left : t->right;
			 }
			 else { // Neither child is null
					psucc = t->getParentOfSuccessor(); // guaranteed not null
					if (psucc == t) { // Immediate successor
						 replacement = psucc->right;
					}
					else { 
						 // Detach replacement from where it is and relocate
						 // it to where t used to be.
						 replacement = psucc->left;
						 psucc->left = psucc->left->right;
						 replacement->right = t->right;
					}
					// Finish relocating replacement to go where t used to.
					replacement->left = t->left;
			 }
			 if (p) { // Fixup parent of t to point to replacement
					if (right_side) p->right = replacement; else p->left = replacement;
			 }
			 else root = replacement; // No parent, so t was the root
		}
		return t;
	}



	#define T  (pp.t)
	#define P  (pp.p)
	#define G  (pp.g)
	#define GG (pp.gg)
	#define S  (pp.s)
	#define RS (pp.rs)
	#define LS (pp.ls)
	#define M  (pp.m)
	#define PM (pp.pm)

	BSTreeItem *
	BSTreeItem::insertBalance
		(BSTreeItem *root, 
		 BSTreeItem::Pack &pp)
	// Balance adjusting for top down insertions. Eliminates
	// both p and t from being red by doing rotations and
	// color changes. g, p, t ASSUMED not null coming in. 
	// gg may be null. At the end of this routine, only t 
	// and p will be valid wrt each other. g and gg will 
	// not reflect the proper ordering.
	{
		BSTreeItem *cofgg = 0; // New child of great-grandparent
		bool side = (bool)(GG && GG->right == G);

		if (G->left == P) {
			 if (P->right == T) { // Do double rotate
					G->left = T->right;
					T->right = G;
					P->right = T->left;
					T->left = P;
					P = GG;
					cofgg = T;
			 }
			 else { // Do single rotate right
					G->left = P->right;
					P->right = G;
					cofgg = P;
			 }
		}
		else { // G->right == p
			if (P->left == T) {  // Do double rotate
				 G->right = T->left;
				 T->left = G;
				 P->left = T->right;
				 T->right = P;
				 P = GG;
				 cofgg = T;
			}
			else { // Do single rotate left
				 G->right = P->left;
				 P->left = G;
				 cofgg = P;
			}
		}
	 
		cofgg->color = BSTreeItem::BLACK;
		G->color = BSTreeItem::RED;

		if (GG) {
			 if (side) GG->right = cofgg; else GG->left = cofgg;
		}
		else root = cofgg;

		return root;
	}

	BSTreeItem *
	BSTreeItem::removeBalance
		(BSTreeItem *root, 
		 BSTreeItem::Pack &pp)
	// Balancing code during top down deletion
	{
		if ((T == 0 || T->color == BSTreeItem::BLACK) && S && S->color == BSTreeItem::RED) {

			 // Case: t black, s red. t might be null,
			 // but s and p must not be.

			 // Fix g child to be s. Also s may become p of m
			 if (G) {
					if (G->left == P) G->left = S; else G->right = S;
			 }
			 else root = S;
			 if (P == M) PM = S;
			 // Finish rotating
			 if (P->left == T) {
					// RotateLeft(p);
					P->right = LS;
					S->left = P;
					G = S;
					S = LS;
			 }
			 else {
					// RotateRight(p);
					P->left = RS;
					S->right = P;
					G = S;
					S = RS;
			 }
	#ifndef TRYIT
			 // Fixup children of s
			 if (S) { LS = S->getLeftChild(); RS = S->getRightChild(); }
			 // Fixup colors
			 P->color = BSTreeItem::RED; G->color = BSTreeItem::BLACK;
	#else
			 // Fixup children of s and color of s
			 if (S) { LS = S->getLeftChild(); RS = S->getRightChild(); S->color = BSTreeItem::RED; }
			 // Finish fixing up colors
			 T->color = BSTreeItem::RED; G->color = BSTreeItem::BLACK;
	#endif
		}

		if (T && T->color == BSTreeItem::BLACK &&
				(T->left == 0 || T->getLeftChild()->color == BSTreeItem::BLACK) &&
				(T->right == 0 || T->getRightChild()->color == BSTreeItem::BLACK)) {

			 // Case: t is a black 2 node.

			 if (S && S->color == BSTreeItem::BLACK &&
					 (S->left == 0 || S->getLeftChild()->color == BSTreeItem::BLACK) &&
					 (S->right == 0 || S->getRightChild()->color == BSTreeItem::BLACK)) {

				 // Case: t and s are black 2 nodes.

				 P->color = BSTreeItem::BLACK; T->color = BSTreeItem::RED; S->color = BSTreeItem::RED;

			 }
			 else if (LS && LS->color == BSTreeItem::RED) {

				 // Case: t is a black 2 node, s is 3 or 4 node,
				 // LS is red (RS might be)

				 if (P->left == T) {
						// Fix colors
						LS->color = P->color; P->color = BSTreeItem::BLACK; T->color = BSTreeItem::RED;
						// Fix g child to be LS. ALSo LS may become p of m
						if (G) {
							 if (G->left == P) G->left = LS; else G->right = LS;
						}
						else root = LS;
						if (P == M) PM = LS;
						// Finish: DoubleRotateLeft(s, p);
						P->right = LS->left;
						LS->left = P;
						S->left = LS->right;
						LS->right = S;
						G = LS;
						// We won't fix up s, LS, and since they get reassigned
						// at the top of next loop
				 }
				 else {
						// Fixup colors
						S->color = P->color; LS->color = BSTreeItem::BLACK;
						P->color = BSTreeItem::BLACK; T->color = BSTreeItem::RED;
						// Fixup g child to be s. ALSo s may become p of m
						if (G) {
							 if (G->left == P) G->left = S; else G->right = S;
						}
						else root = S;
						if (P == M) PM = S;
						// Finish: RotateRight(p);
						P->left = RS;
						S->right = P;
						G = S;
						// We won't fix up s, LS, and since they get reassigned
						// at the top of next loop
				 }

			 }
			 else if (RS && RS->color == BSTreeItem::RED) {

				 // Case: t is a 2 node, s is a 3 node, LS black, RS red

				 if (P->left == T) {
						// Fix colors
						RS->color = BSTreeItem::BLACK; S->color = P->color;
						P->color = BSTreeItem::BLACK; T->color = BSTreeItem::RED;
						// Fix g child to be s. ALSo, s may become p of m
						if (G) {
							 if (G->left == P) G->left = S; else G->right = S;
						}
						else root = S;
						if (P == M) PM = S;
						// Finish: RotateLeft(p);
						P->right = LS;
						S->left = P;
						G = S;
						// We won't fix up s, LS, and since they get reassigned
						// at the top of next loop
				 }
				 else {
						// Fix colors
						RS->color = P->color; P->color = BSTreeItem::BLACK; T->color = BSTreeItem::RED;
						// Fix g child to become RS. ALSo, RS may become p of m.
						if (G) {
							 if (G->left == P) G->left = RS; else G->right = RS;
						}
						else root = RS;
						if (P == M) PM = RS;
						// Finish: DoubleRotateRight(s, p);
						P->left = RS->right;
						RS->right = P;
						S->right = RS->left;
						RS->left = S;
						G = RS;
						// We won't fix up s, LS, and since they get reassigned
						// at the top of next loop
				 }

			 }
		}

		return root;
	}

	BSTreeItem *
	BSTreeItem::replace
		(BSTreeItem *root, 
		 BSTreeItem::Pack &pp)
	{
		// At this point, m is the node to delete, pm is it's parent,
		// p is the replacement node, g is it's parent. If m has no
		// successor, p will = m, and replacement is the non-null
		// child of m.

		if (M) { // Matching node was found
			if (P == M || M->left == 0 || M->right == 0) {
				 // No successor, and/or at least one child null
				 // Get non-null child, if any, else p will be null
				 P = (M->left) ? M->getLeftChild() : M->getRightChild();
			}
			else { // m has a successor to use as a replacement
				 if (G != M) {
					 // Successor isn't immediate child of m, so detach
					 // from where it is, attach to where m is
					 G->left = P->right;
					 P->right = M->right;
				 }
				 // Finish attaching where m is.
				 P->left = M->left;
			}
			// p should have same color as m since it's going where m was
			if (P) P->color = M->color;
		}

		// Fixup pm child link to be p
		if (M) {
			 if (PM) {
					if (PM->left == M) PM->left = P; else PM->right = P;
			 }
			 else root = P; // New root, possibly null
		}

		return root;
	}

	BSTreeItem *
	BSTreeItem::detachMinimum
		(BSTreeItem *&root)
	{
		BSTreeItem::Pack pp;

		if (root == 0)
		{
			return 0;
		}

		T = root; 
		P = G = M = PM = 0;

		while (T) {

			// Pretend we're on the matching node
			M = T; PM = P;

			// Update ancestor pointers
			G = P; P = T;

			// Then go all the way left or right

			T = P->getLeftChild(); S = P->getRightChild();

			if (S) {
				 LS = S->getLeftChild(); RS = S->getRightChild();
			}
			else {
				 LS = 0; RS = 0;
			}

			root = removeBalance(root, pp);

		} // end of while loop

		root = replace(root, pp);

		if (root) root->color = BSTreeItem::BLACK;

		return M; // Node to delete
	}

	BSTreeItem *
	BSTreeItem::detachMaximum
		(BSTreeItem *&root)
	{
		BSTreeItem::Pack pp;

		if (root == 0)
		{
			return 0;
		}

		T = root; 
		P = G = M = PM = 0;

		while (T)
		{
			// Pretend we're on the matching node
			M = T;
			PM = P;

			// Update ancestor pointers
			G = P;
			P = T;

			// Then go all the way left or right

			T = P->getRightChild();
			S = P->getLeftChild();

			if (S)
			{
				LS = S->getLeftChild(); RS = S->getRightChild();
			}
			else
			{
				LS = RS = 0;
			}

			root = removeBalance(root, pp);

		} // end of while loop

		root = replace(root, pp);

		if (root) root->color = BSTreeItem::BLACK;

		return M; // Node to delete
	}



	void BSTreeIterator::set(const BSTreeItem* item, BSTreeIterator::WalkOrder walk_order)
	{
		root_ = item;
		walk_order_ = walk_order;
		switch (walk_order_)
		{
			case BSTreeIterator::WALK_ORDER__PREORDER:		forward_ = &BSTreeIterator::forwardPreOrder;  break;
			case BSTreeIterator::WALK_ORDER__INORDER:			forward_ = &BSTreeIterator::forwardInOrder;   break;
			case BSTreeIterator::WALK_ORDER__POSTORDER:		forward_ = &BSTreeIterator::forwardPostOrder; break;
			case BSTreeIterator::WALK_ORDER__LEVELORDER:
			default:																			forward_ = &BSTreeIterator::forwardLevelOrder;
		}
		state_ = false;
		current_ = root_;
		path_.clear();
		if (root_ && walk_order_ == BSTreeIterator::WALK_ORDER__LEVELORDER)
		{
			path_.pushBottom(root_);
		}
	}

	const BSTreeItem* BSTreeIterator::forwardPreOrder()
	{
		// just to avoid compiler warnings like "control expression constant"
		bool loop = true;
		while (loop)
		{
			if (current_)
			{
				path_.push(current_);
				const BSTreeItem *item = current_;
				current_ = current_->left;
				return item;
			}
			else
			{
				if (path_.pop(current_) == 0)
				{
					return current_ = 0;
				}
				else
				{
					current_ = current_->right;
				}
			}
		}
		return 0;
	}

	const BSTreeItem* BSTreeIterator::forwardInOrder()
	{
		// just to prevent tedious compiler remarks like "control expression is constant"
		bool loop = true;

		while (loop)
		{
			if (current_)
			{
				path_.push(current_);
				current_ = current_->left;
			}
			else
			{
				if (path_.pop(current_) == 0)
				{
					return current_ = 0;
				}
				else
				{
					const BSTreeItem *item = current_;
					current_ = current_->right;
					return item;
				}
			}
		}
		return 0;
	}

	const BSTreeItem* BSTreeIterator::forwardPostOrder()
	{
		// just to avoid compiler warnings like "control expression constant"
		bool loop = true;

		while (loop)
		{
			if (state_ == false)
			{ // Ready to go down the tree to left
				if (current_)
				{
					path_.push(current_);
					current_ = current_->left;
				}
				else
				{
					state_ = true;
				}
			}
			else
			{ // Ready to come up the tree
				const BSTreeItem *item = current_;
				if (path_.isEmpty())
				{
		return (current_ = 0); // At root
				}
				current_ = *path_.top();
				if (item == current_->left
			&& current_->right)
				{ // Coming back up the tree from the left, and
		// there is a right child, so go right. 
		// Note that current_ is still on top of stack.
		current_ = current_->right;
		state_ = false;
				 }
				 else
				 { // Coming back up the tree from the right,
		 // or there was no right child, so visit
		 // the node, and continue on up. (state stays at true.)
		 path_.pop();
		 return current_;
				 }
			}
		}
		return 0;
	}

	const BSTreeItem *
	BSTreeIterator::forwardLevelOrder
		()
	{
		if (path_.pop(current_) == 0)
		{
			return 0;
		}
		if (current_->left != 0)
		{
			path_.pushBottom(current_->left);
		}
		if (current_->right != 0)
		{
			path_.pushBottom(current_->right);
		}
		return current_;
	}


} // namespace BALL
