// $Id: expressionPredicate.C,v 1.2 2001/12/30 13:28:46 sturm Exp $

#include <BALL/KERNEL/expressionPredicate.h>

namespace BALL
{

	ExpressionPredicate::ExpressionPredicate() 
		throw()
		: UnaryPredicate<Atom>(),
			argument_("")
	{
	}


	ExpressionPredicate::ExpressionPredicate
		(const ExpressionPredicate& predicate) 
		throw()
		:	UnaryPredicate<Atom>(predicate),
			argument_(predicate.argument_)
	{
	}


	ExpressionPredicate::ExpressionPredicate(const String& argument) 
		throw()
		:	UnaryPredicate<Atom>(),
			argument_(argument)
	{
	}


	ExpressionPredicate::~ExpressionPredicate() 
		throw()
	{
		clear();
	}


	const ExpressionPredicate& ExpressionPredicate::operator =
		(const ExpressionPredicate& predicate) 
		throw()
	{
		UnaryPredicate<Atom>::operator = (predicate);
		argument_ = predicate.argument_;

		return *this;
	}


	void ExpressionPredicate::clear() 
		throw()
	{
		argument_ = "";
		// ?????
		// UnaryPredicate does not implement the OCI
		// UnaryPredicate<Atom>::clear();
	}


	bool ExpressionPredicate::operator == (const ExpressionPredicate& predicate) const 
		throw()
	{
		// ?????
		// UnaryPredicate does not implement the OCI
		// return (UnaryPredicate<Atom>::operator == (predicate)
		//	&& (argument_ == predicate.argument_));
		return (argument_ == predicate.argument_);
	}


	bool ExpressionPredicate::operator () (const Atom& /* atom */) const 
		throw()
	{
		return true;
	}


	void ExpressionPredicate::setArgument(const String& args) 
		throw()
	{
		argument_ = args;
	}


	const String& ExpressionPredicate::getArgument() const 
		throw()
	{
		return argument_;
	}

}
