// $Id: expressionTree.C,v 1.4 2001/12/11 16:05:29 amoll Exp $

#include <BALL/KERNEL/expressionTree.h>

using namespace ::std;

namespace BALL
{

	ExpressionTree::ExpressionTree()
		throw()
		:	type_(INVALID),
			negate_(false),
			predicate_(0),
			children_()
	{
	}


	void ExpressionTree::dump(::std::ostream& os, Size depth) const
		throw()
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
		throw()
		:	type_(type),
			negate_(negate),
			predicate_(0),
			children_(children)
	{
	}


	ExpressionTree::ExpressionTree(ExpressionPredicate* predicate, bool negate)
		throw()
		:	type_(INVALID),
			negate_(negate),
			predicate_(predicate),
			children_()
	{
	}
	

	// BAUSTELLE: 
	// should we clone predicate_ or should we only set the pointer?
	ExpressionTree::ExpressionTree(const ExpressionTree& tree)
		throw()
		:	type_(tree.type_),
			negate_(tree.negate_),
			predicate_(tree.predicate_),
			children_(tree.children_)
	{
	}
			

	ExpressionTree::~ExpressionTree()
		throw()
	{
		clear();
	}


	void ExpressionTree::clear()
		throw()
	{
		type_ = INVALID;
		negate_ = false;
		// BAUSTELLE
		// memory leak if nobody cares for the predicate.
		predicate_ = 0;
		children_.clear();
	}


	const ExpressionTree& ExpressionTree::operator = 
		(const ExpressionTree& tree)
		throw()
	{
		clear();

		type_ = tree.type_;
		negate_ = tree.negate_;
		predicate_ = tree.predicate_;
		children_ = tree.children_;

		return *this;
	}


	bool ExpressionTree::operator () (const Atom& atom) const 
		throw()
	{

    bool result;
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
      if (children_.size() == 0)
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
		throw()
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
		throw()
	{
		return ! operator == (tree);
	}

	bool ExpressionTree::compareChildren_(const ExpressionTree& tree) const
		throw()
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
		throw()
	{
		type_ = type;
	}


	ExpressionTree::Type ExpressionTree::getType() const 
		throw()
	{
		return type_;
	}


	void ExpressionTree::setNegate(bool negate)
		throw()
	{
		negate_= negate;
	}


	bool ExpressionTree::getNegate() const 
		throw()
	{
		return negate_;
	}


	void ExpressionTree::setPredicate(ExpressionPredicate* predicate)
		throw()
	{
		predicate_= predicate;
	}


	ExpressionPredicate*  ExpressionTree::getPredicate() const 
		throw()
	{
		return predicate_;
	}


	void ExpressionTree::appendChild(const ExpressionTree* child)
		throw()
	{
		children_.push_back(child);
	}


	const list<const ExpressionTree*>& ExpressionTree::getChildren() const
		throw()
	{
		return children_;
	}

}
