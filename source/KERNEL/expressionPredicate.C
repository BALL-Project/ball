// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: expressionPredicate.C,v 1.6 2003/08/26 09:17:49 oliver Exp $
//

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


	ExpressionPredicate& ExpressionPredicate::operator = (const ExpressionPredicate& predicate) 
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
	}


	bool ExpressionPredicate::operator == (const ExpressionPredicate& predicate) const 
		throw()
	{
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
