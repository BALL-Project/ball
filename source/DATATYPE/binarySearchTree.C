// $Id: binarySearchTree.C,v 1.6 2000/08/22 17:03:42 amoll Exp $

#include <BALL/DATATYPE/binarySearchTree.h>

namespace BALL 
{

	Size BSTreeItem::getSize() const
	{
		const BSTreeItem *item = this;
		Size size = 1;

		while(item) 
		{
			if (item->left_)
			{
				size += item->left_->getSize();
			}
			
			if (item->right_) 
			{
				++size; 
			}
			
			item = item->right_;
		}

		return size;
	}

	Size BSTreeItem::getHeight() const
	{
		Size lhsize = (left_) ? left_->getHeight() : 0;
		Size rhsize = (right_) ? right_->getHeight() : 0;

		return (((lhsize > rhsize) ? lhsize : rhsize) + 1);
	}

	Processor::Result BSTreeItem::applyPreorder_
		(UnaryProcessor<BSTreeItem>& processor)
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
			
			if (item->left_)
			{
				result = item->left_->applyPreorder_(processor);
				
				if (result <= Processor::BREAK)
				{
					return result;
				}
			}
			
			item = item->right_;
		}

		return Processor::CONTINUE;
	}

	Processor::Result BSTreeItem::applyInorder_
		(UnaryProcessor<BSTreeItem>& processor)
	{
		Processor::Result result = Processor::ABORT;
		BSTreeItem *item = this;

		while(item) 
		{
			if (item->left_)
			{
				result = item->left_->applyInorder_(processor);
				
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
			
			item = item->right_;
		}

		return Processor::CONTINUE;
	}

	Processor::Result BSTreeItem::applyPostorder_
		(UnaryProcessor<BSTreeItem>& processor)
	{
		Processor::Result result = Processor::ABORT;

		if (left_)
		{
			result = left_->applyPostorder_(processor);
				
			if (result <= Processor::BREAK)
			{
				return result;
			}
		}
		
		if (right_)
		{
			result = right_->applyPostorder_(processor);
				
			if (result <= Processor::BREAK)
			{
				return result;
			}
		}
		
		return processor(*this);
	}

	Processor::Result BSTreeItem::applyPreorderFlat_
		(UnaryProcessor<BSTreeItem>& processor)
	{
		Processor::Result result = Processor::ABORT;
		TSStack<BSTreeItem*> path;
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

				 path.push(item);
				 item = item->left_;
			}
			else 
			{
				if (path.pop(item) == 0)
				{
					loop = false;
					break;
				}
				
				item = item->right_;
			}
		}

		return Processor::CONTINUE;
	}

	Processor::Result BSTreeItem::applyInorderFlat_
		(UnaryProcessor<BSTreeItem>& processor)
	{
		Processor::Result result = Processor::ABORT;
		TSStack<BSTreeItem*> path;
		BSTreeItem *item = this;
		bool loop = true;

		while (loop)
		{
			if (item) 
			{
				path.push(item);
				item = item->left_;
			}
			else 
			{
				if (path.pop(item) == 0)
				{
					loop = false;
					break;
				}
				
				result = processor(*item);

				if (result <= Processor::BREAK)
				{
					return result;
				}
				
				item = item->right_;
			}
		}

		return Processor::CONTINUE;
	}

	Processor::Result BSTreeItem::applyPostorderFlat_
		(UnaryProcessor<BSTreeItem>& processor)
	{
		Processor::Result result = Processor::ABORT;
		TSStack<BSTreeItem *> path;
		BSTreeItem* item = this;
		bool state = false;
		// just to prevent tedious compiler remarks like "control expression is constant"
		bool loop = true;

		while (loop)
		{
			if (state == false) 
			{ 
				if (item) 
				{
					path.push(item);
					item = item->left_;
				}
				else 
				{
					state = true;
				}
			}
			else 
			{ 
				BSTreeItem *leftitem = item;
				
				if (path.isEmpty())
				{
					loop = false;
					break;
				}
				
				item = *path.top();
				
				if (leftitem == item->left_ && item->right_)
				{
					item = item->right_;
					state = false;
				}
				else 
				{
					result = processor(*item);
		
					if (result <= Processor::BREAK)
					{
						return result;
					}

					path.pop();
				}
			}
		}
		
		return Processor::CONTINUE;
	}

	Processor::Result BSTreeItem::applyLevelorder_(UnaryProcessor<BSTreeItem>& processor)
	{
		Processor::Result result = Processor::ABORT;
		TSStack<BSTreeItem *> path;
		BSTreeItem *parentitem = 0;
		// just to prevent tedious compiler remarks like "control expression is constant"
		bool loop = true;

		path.pushBottom(this);

		while(loop) 
		{
			if (path.pop(parentitem) == 0)
			{
				loop = false;
				break;
			}
			
			result = processor(*parentitem);

			if (result <= Processor::BREAK)
			{
				return result;
			}
			if (parentitem->left_)
			{
				path.pushBottom(parentitem->left_);
			}
			if (parentitem->right_)
			{
				path.pushBottom(parentitem->right_);
			}
		}

		return Processor::CONTINUE;
	}

	BSTreeItem* BSTreeItem::rotateRight()
	{	// PRECONDITION left child of p exists.
		if (left_ == 0) 
		{
			return 0;
		}
 
		BSTreeItem* item = this; // x
		BSTreeItem* l = left_;  // y

		left_ = l->right_;
		l->right_ = item;
		item = l;

		return l;
	}

	BSTreeItem*	BSTreeItem::rotateLeft()
	{	// PRECONDITION right child of p exists.
		if (right_ == 0) 
		{
			return 0;
		}
 
		BSTreeItem* item = this; // x
		BSTreeItem* r = right_;  // y

		right_ = r->left_;
		r->left_ = item;
		item = r;

		return r;
	}

	const BSTreeItem*	BSTreeItem::getMinimum() const
	{
		const BSTreeItem *item = this;

		while(item->left_)
		{
			item = item->left_;
		}

		return item;
	}

	const BSTreeItem*	BSTreeItem::getMaximum() const
	{
		const BSTreeItem *item = this;

		while(item->right_)
		{
			item = item->right_;
		}

		return item;
	}

	BSTreeItem* BSTreeItem::getParentOfMinimum()
	{
		BSTreeItem* item = this;
		BSTreeItem* p = 0;

		while(item->left_) 
		{
			p = item;
			item = item->left_;
		}

		return p;
	}

	BSTreeItem* BSTreeItem::getParentOfMaximum()
	{
		BSTreeItem* item = this;
		BSTreeItem* p = 0;

		while(item->right_) 
		{
			p = item;
			item = item->right_;
		}

		return p;
	}

	// Predecessor is the right child of node returned, unless item 
	// itself is the parent. Then the left child is the predecessor. 
	// If this is a leaf, 0 is returned. ASSUMES item isn't null!
	BSTreeItem* BSTreeItem::getParentOfPredecessor()
	{
		BSTreeItem* item = this;
		BSTreeItem* p = 0;
		// Go left, then all the way right
		BSTreeItem* q = item->left_;

		if (q) 
		{
			p = item;
			while(q->right_) 
			{
				p = q;
				q = q->right_;
			}
		}

		return p;
	}

	// Successor is the left child of node returned, unless item 
	// itself is the parent. Then the right child is the successor. 
	// If item is a leaf, 0 is returned. ASSUMES item isn't null.
	BSTreeItem* BSTreeItem::getParentOfSuccessor()
	{
		BSTreeItem *item = this;
		BSTreeItem *p = 0;
		// Go right, then all the way left
		BSTreeItem *q = item->right_;

		if (q) 
		{
			p = item;
			while(q->left_) 
			{
				p = q;
				q = q->left_;
			}
		}

		return p;
	}

	// Detaches node item with parent p from the tree. Node item is
	// the left child if right_side = false, else it's the right child.
	// If p is 0, it means item is the root, and that is handled
	// accordingly. Redundantly returns the pointer item. May
	// have to update root pointer.
	BSTreeItem* BSTreeItem::detachNode
		(BSTreeItem*& root, BSTreeItem* t,
		 BSTreeItem* p, bool right_side)
	{
		BSTreeItem* psucc = 0;
		BSTreeItem* replacement = 0;

		if (t == 0)	
		{
			return 0;
		}

		if (t->left_ == 0 || t->right_ == 0) 
		{
			// At least one child is null, so use the other 
			// as the replacement. (It may be null too.)
			replacement = (t->left_) ? t->left_ : t->right_;
		}
		else 
		{	// Neither child is null
			psucc = t->getParentOfSuccessor(); // guaranteed not null
			if (psucc == t) 
			{ // Immediate successor
				replacement = psucc->right_;
			}
			else 
			{ // Detach replacement from where it is and relocate
				// it to where t used to be.
				replacement = psucc->left_;
				psucc->left_ = psucc->left_->right_;
				replacement->right_ = t->right_;
			}
			// Finish relocating replacement to go where t used to.
			replacement->left_ = t->left_;
		}

		if (p != 0) 
		{ // Fixup parent of t to point to replacement
			if (right_side) 
			{
				p->right_ = replacement; 
			}
			else 
			{
				p->left_ = replacement;
			}
		}
		else
		{	// No parent, so t was the root
			root = replacement; 
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


	// Balance adjusting for top down insertions. Eliminates
	// both p and t from being red by doing rotations and
	// color changes. g, p, t ASSUMED not null coming in. 
	// gg may be null. At the end of this routine, only t 
	// and p will be valid wrt each other. g and gg will 
	// not reflect the proper ordering.
	BSTreeItem* BSTreeItem::insertBalance
		(BSTreeItem* root, BSTreeItem::Pack& pp)
	{
		BSTreeItem* cofgg = 0; // New child of great-grandparent
		bool side = (bool)(GG && GG->right_ == G);

		if (G->left_ == P)
		{
			if (P->right_ == T) 
			{	// Do double rotate
				G->left_ = T->right_;
				T->right_ = G;
				P->right_ = T->left_;
				T->left_ = P;
				P = GG;
				cofgg = T;
			}
			else 
			{ // Do single rotate right
				G->left_ = P->right_;
				P->right_ = G;
				cofgg = P;
			}
		}
		else 
		{	// G->right == p
			if (P->left_ == T) 
			{  // Do double rotate
				G->right_ = T->left_;
				T->left_ = G;
				P->left_ = T->right_;
				T->right_ = P;
				P = GG;
				cofgg = T;
			}
			else 
			{ // Do single rotate left
				G->right_ = P->left_;
				P->left_ = G;
				cofgg = P;
			}
		}
	 
		cofgg->color_= BSTreeItem::BLACK;
		G->color_ = BSTreeItem::RED;

		if (GG) 
		{
			if (side)
			{	
				GG->right_ = cofgg;
			}
			else 
			{
				GG->left_ = cofgg;
			}
		}
		else root = cofgg;

		return root;
	}

	// Balancing code during top down deletion
	BSTreeItem* BSTreeItem::removeBalance
		(BSTreeItem* root, BSTreeItem::Pack& pp)
	{
		if ((T == 0 || T->color_ == BSTreeItem::BLACK) && 
				S && S->color_ == BSTreeItem::RED) 
		{
			// Case: t black, s red. t might be null,
			// but s and p must not be.

			// Fix g child to be s. Also s may become p of m
			if (G) 
			{
				if (G->left_ == P) 
				{	
					G->left_ = S;
				}
				else 
				{
					G->right_ = S;
				}
			}
			else 
			{
				root = S;	
			}
			
			if (P == M)
			{
				PM = S;
			}
			// Finish rotating
			if (P->left_ == T) 
			{
				// RotateLeft(p);
				P->right_ = LS;
				S->left_ = P;
				G = S;
				S = LS;
			}
			else 
			{
				// RotateRight(p);
				P->left_ = RS;
				S->right_ = P;
				G = S;
				S = RS;
			}
	#ifndef TRYIT
			// Fixup children of s
			if (S) 
			{ 
				LS = S->getLeftChild(); 
				RS = S->getRightChild();
			}
			// Fixup colors
			P->color_ = BSTreeItem::RED; G->color_ = BSTreeItem::BLACK;
	#else
			// Fixup children of s and color of s
			if (S) 
			{ 
				LS = S->getLeftChild(); 
				RS = S->getRightChild(); 
				S->color_ = BSTreeItem::RED; 
			}
			// Finish fixing up colors
			T->color_ = BSTreeItem::RED; 
			G->color_ = BSTreeItem::BLACK;
	#endif
		}

		if (T && T->color_ == BSTreeItem::BLACK &&
				(T->left_ == 0 || T->getLeftChild()->color_ == BSTreeItem::BLACK) &&
				(T->right_ == 0 || T->getRightChild()->color_ == BSTreeItem::BLACK)) 
		{
			// Case: t is a black 2 node.
			if (S && S->color_ == BSTreeItem::BLACK &&
					 (S->left_ == 0 || S->getLeftChild()->color_ == BSTreeItem::BLACK) &&
					 (S->right_ == 0 || S->getRightChild()->color_ == BSTreeItem::BLACK))
			{
				 // Case: t and s are black 2 nodes.
				 P->color_ = BSTreeItem::BLACK; 
				 T->color_ = BSTreeItem::RED; 
				 S->color_ = BSTreeItem::RED;
			}
			else 
			{	
				if (LS && LS->color_ == BSTreeItem::RED) 
				{
					 // Case: t is a black 2 node, s is 3 or 4 node,
					 // LS is red (RS might be)

					 if (P->left_ == T) 
					 {	// Fix colors
							LS->color_ = P->color_; P->color_ = BSTreeItem::BLACK; T->color_ = BSTreeItem::RED;
							// Fix g child to be LS. ALSo LS may become p of m
							if (G) 
							{
								if (G->left_ == P)
								{
									G->left_ = LS; 
								}
								else 
								{
									G->right_ = LS;
								}
							}
							else 
							{
								root = LS;
							}
							if (P == M) 
							{
								PM = LS;
							}
							// Finish: DoubleRotateLeft(s, p);
							P->right_ = LS->left_;
							LS->left_ = P;
							S->left_ = LS->right_;
							LS->right_ = S;
							G = LS;
							// We won't fix up s, LS, and since they get reassigned
							// at the top of next loop
					 }
					 else 
					 {	// Fixup colors
							S->color_ = P->color_; LS->color_ = BSTreeItem::BLACK;
							P->color_ = BSTreeItem::BLACK; T->color_ = BSTreeItem::RED;
							// Fixup g child to be s. ALSo s may become p of m
							if (G) 
							{
								if (G->left_ == P) 
								{
									G->left_ = S; 
								}
								else 
								{
									G->right_ = S;
								}
							}
							else 
							{
								root = S;
							}
							if (P == M) 
							{
								PM = S;
							}
							// Finish: RotateRight(p);
							P->left_ = RS;
							S->right_ = P;
							G = S;
							// We won't fix up s, LS, and since they get reassigned
							// at the top of next loop
					 }
			 }
			 else if (RS && RS->color_ == BSTreeItem::RED) 
			 { // Case: t is a 2 node, s is a 3 node, LS black, RS red
					if (P->left_ == T) 
					{	// Fix colors
						RS->color_ = BSTreeItem::BLACK; 
						S->color_ = P->color_;
						P->color_ = BSTreeItem::BLACK; 
						T->color_ = BSTreeItem::RED;
						// Fix g child to be s. ALSo, s may become p of m
						if (G) 
						{
							 if (G->left_ == P) 
							 {
								G->left_ = S; 
							 }
							 else 
							 {
								G->right_ = S;
  						 }
						}
						else 
						{
							root = S;
						}
						if (P == M) 
						{
							PM = S;
						}
						// Finish: RotateLeft(p);
						P->right_ = LS;
						S->left_ = P;
						G = S;
						// We won't fix up s, LS, and since they get reassigned
						// at the top of next loop
					}
					else 
					{
						// Fix colors
						RS->color_ = P->color_; 
						P->color_ = BSTreeItem::BLACK; 
						T->color_ = BSTreeItem::RED;
						// Fix g child to become RS. ALSo, RS may become p of m.
						if (G) 
						{
								if (G->left_ == P) 
							  {
									G->left_ = RS;
								}
								else 
								{
									G->right_ = RS;
								}
						}
						else 
						{
							root = RS;
						}
						if (P == M) 
						{
							PM = RS;
						}
						// Finish: DoubleRotateRight(s, p);
						P->left_ = RS->right_;
						RS->right_ = P;
						S->right_ = RS->left_;
						RS->left_ = S;
						G = RS;
						// We won't fix up s, LS, and since they get reassigned
						// at the top of next loop
					}
				}
			}
		}

		return root;
	}

	// At this point, m is the node to delete, pm is it's parent,
	// p is the replacement node, g is it's parent. If m has no
	// successor, p will = m, and replacement is the non-null
	// child of m.
	BSTreeItem*	BSTreeItem::replace(BSTreeItem* root, BSTreeItem::Pack& pp)
	{
		if (M) 
		{ // Matching node was found
			if (P == M || M->left_ == 0 || M->right_ == 0) 
			{
				// No successor, and/or at least one child null
				// Get non-null child, if any, else p will be null
				P = (M->left_) ? M->getLeftChild() : M->getRightChild();
			}
			else 
			{ // m has a successor to use as a replacement
				if (G != M) 
				{
					// Successor isn't immediate child of m, so detach
					// from where it is, attach to where m is
					G->left_  = P->right_;
					P->right_ = M->right_;
				}
				// Finish attaching where m is.
				P->left_ = M->left_;
			}
			// p should have same color as m since it's going where m was
			if (P) 
			{
				P->color_ = M->color_;
			}
		}

		// Fixup pm child link to be p
		if (M) 
		{
			if (PM) 
			{
				if (PM->left_ == M) 
				{
					PM->left_ = P;
				}
				else 
				{
					PM->right_ = P;
				}
			}
			else 
			{
				root = P; // New root, possibly null
			}
		}

		return root;
	}

	BSTreeItem* BSTreeItem::detachMinimum(BSTreeItem *&root)
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

			T = P->getLeftChild();
			S = P->getRightChild();

			if (S) 
			{
				 LS = S->getLeftChild(); 
				 RS = S->getRightChild();
			}
			else 
			{
				 LS = 0;
				 RS = 0;
			}

			root = removeBalance(root, pp);

		} // end of while loop

		root = replace(root, pp);

		if (root)
		{
			root->color_ = BSTreeItem::BLACK;
		}

		return M; // Node to delete
	}

	BSTreeItem*	BSTreeItem::detachMaximum(BSTreeItem *&root)
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
				LS = S->getLeftChild();
				RS = S->getRightChild();
			}
			else
			{
				LS = RS = 0;
			}

			root = removeBalance(root, pp);

		} // end of while loop

		root = replace(root, pp);

		if (root)
		{
			root->color_ = BSTreeItem::BLACK;
		}

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
				current_ = current_->getLeftChild();
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
					current_ = current_->getRightChild();
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
				current_ = current_->getLeftChild();
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
					current_ = current_->getRightChild();
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
					current_ = current_->getLeftChild();
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
				if (item == current_->getLeftChild() && current_->getRightChild())
				{ // Coming back up the tree from the left, and
					// there is a right child, so go right. 
					// Note that current_ is still on top of stack.
					current_ = current_->getRightChild();
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

	const BSTreeItem*	BSTreeIterator::forwardLevelOrder()
	{
		if (path_.pop(current_) == 0)
		{
			return 0;
		}
		if (current_->getLeftChild() != 0)
		{
			path_.pushBottom(current_->getLeftChild());
		}
		if (current_->getRightChild() != 0)
		{
			path_.pushBottom(current_->getRightChild());
		}
		return current_;
	}



#	ifdef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/DATATYPE/binarySearchTree.iC>
#	endif

} // namespace BALL
