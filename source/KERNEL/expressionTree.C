// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: expressionTree.C,v 1.13 2003/08/26 09:17:49 oliver Exp $
//

#include <BALL/KERNEL/expressionTree.h>

using namespace::std;
namespace BALL
{

	ExpressionTree::ExpressionTree()
		:	type_(INVALID),
			negate_(false),
			predicate_(0),
			children_()
	{
	}


	void ExpressionTree::dump(std::ostream& os, Size depth) const
	{
		BALL_DUMP_STREAM_PREFIX(os);
		BALL_DUMP_HEADER(os, this, this);
		BALL_DUMP_DEPTH(os, depth);
		os << "[type = " << type_ 
			<< "  negate = " << negate_ 
			<< "  arg = " << (predicate_ == 0 ? "<NULL>" : predicate_->getArgument().c_str()) 
			<< "]" << endl;
		list<const ExpressionTree*>::const_iterator it = children_.begin();
		for (; it != children_.end(); ++it)
		{
			(*it)->dump(os, depth + 2);
		}
		BALL_DUMP_STREAM_SUFFIX(os);
	}


	ExpressionTree::ExpressionTree
		(Type type,	list<const ExpressionTree*>	children, bool negate)
		:	type_(type),
			negate_(negate),
			predicate_(0),
			children_(children)
	{
	}


	ExpressionTree::ExpressionTree(ExpressionPredicate* predicate, bool negate)
		:	type_(INVALID),
			negate_(negate),
			predicate_(predicate),
			children_()
	{
	}
	

	ExpressionTree::ExpressionTree(const ExpressionTree& tree)
		:	type_(tree.type_),
			negate_(tree.negate_),
			predicate_(0),
			children_()
	{
		// Clone the predicate
		if (tree.predicate_ != 0)
		{
			predicate_ = (ExpressionPredicate*)tree.predicate_->create();
		}

		// Clone the children.
		std::list<const ExpressionTree*>::const_iterator it(tree.children_.begin());
		for (; it != tree.children_.end(); ++it)
		{
			children_.push_back(new ExpressionTree(**it));
		}
	}
			

	ExpressionTree::~ExpressionTree()
	{
		clear();
	}


	void ExpressionTree::clear()
	{
		type_ = INVALID;
		negate_ = false;
		
		// Delete the predicate.
		delete predicate_;
		predicate_ = 0;

		// Delete the node's children.
		std::list<const ExpressionTree*>::const_iterator it(children_.begin());
		for (; it != children_.end(); ++it)
		{
			delete *it;
		}
		children_.clear();
	}


	ExpressionTree& ExpressionTree::operator = (const ExpressionTree& tree)
	{
		// Avoid self assignment.
		if (&tree == this)
		{
			return *this;
		}

		clear();

		type_ = tree.type_;
		negate_ = tree.negate_;

		// Clone the predicate.
		if (tree.predicate_ != 0)
		{
			predicate_ = (ExpressionPredicate*)tree.predicate_->create();
		}

		// Clone the children.
		std::list<const ExpressionTree*>::const_iterator it(tree.children_.begin());
		for (; it != tree.children_.end(); ++it)
		{
			children_.push_back(new ExpressionTree(**it));
		}

		return *this;
	}


	bool ExpressionTree::operator () (const Atom& atom) const 
	{

    bool result = true;
    if (type_ == LEAF)
    {

			// if there is a predicate associated with this leaf, return its
			// value, else return false

      if (predicate_ != 0)
      {
        result = (negate_ ^ (predicate_->operator () (atom)));
			} 
			else 
			{
        result = false;
			}
		} 
		else 
		{
			// this is either a conjunction or a disjunction or a node that
			// represents a bracket pair (which is marked as INVALID)

      // the empty clause is always true
      if (children_.empty())
      {
        return (!negate_);
			}

			// evaluate all children
      list<const ExpressionTree*>::const_iterator list_it = children_.begin();
      bool abort = false;
      for (; !abort && list_it != children_.end(); ++list_it)
      {
        result = (*list_it)->operator () (atom);

				// OR expressions may be aborted, if the first subexpression yields true
        if (type_ == OR)
        {
          if (result == true)
          {
            abort = true;
					}
				}
				else
				{
					// AND expressions may be aborted, if the first subexpression
					// yields false
          if (result == false)
          {
            abort = true;
					}
				}
			}
		}

    return result; 
	}


	bool ExpressionTree::operator == (const ExpressionTree& tree) const
	{
		if ((predicate_ == 0) && (tree.predicate_ == 0))
		{
			// both pointers are null pointers. Expressions should have null
			// pointers only if they are default constructed, so a consistency
			// check might be useful 
			return ((type_ == tree.type_)
					&& (negate_ == tree.negate_)
					&& compareChildren_(tree));
		}
		else
		{
			if ((predicate_ == 0) || (tree.predicate_ == 0))
			{
				// one of the pointers is NULL, so hte instances cannot be equal.
				return false;
			}
			else
			{
				return ((type_ == tree.type_)
						&& (negate_ == tree.negate_)
						&& (*predicate_ == *tree.predicate_)
						&& compareChildren_(tree));
			}
		}
	}

	bool ExpressionTree::operator != (const ExpressionTree& tree) const
	{
		return ! operator == (tree);
	}

	bool ExpressionTree::compareChildren_(const ExpressionTree& tree) const
	{
		if (children_.size() != tree.children_.size())
		{
			return false;
		}
		list<const ExpressionTree*>::const_iterator it1 = children_.begin();
		list<const ExpressionTree*>::const_iterator it2 = tree.children_.begin();
		for (; it1 != children_.end(); ++it1, ++it2)
		{
			if (**it1 != **it2)
			{
				return false;
			}
		}
		return true;
	}


	void ExpressionTree::setType(Type type)
	{
		type_ = type;
	}


	ExpressionTree::Type ExpressionTree::getType() const 
	{
		return type_;
	}


	void ExpressionTree::setNegate(bool negate)
	{
		negate_= negate;
	}


	bool ExpressionTree::getNegate() const 
	{
		return negate_;
	}


	void ExpressionTree::setPredicate(ExpressionPredicate* predicate)
	{
		predicate_ = predicate;
	}


	ExpressionPredicate*  ExpressionTree::getPredicate() const 
	{
		return predicate_;
	}


	void ExpressionTree::appendChild(const ExpressionTree* child)
	{
		children_.push_back(child);
	}


	const list<const ExpressionTree*>& ExpressionTree::getChildren() const
	{
		return children_;
	}

}
