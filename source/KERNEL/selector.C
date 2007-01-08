// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: selector.C,v 1.28 2003/12/20 16:00:53 amoll Exp $
//

#include <BALL/KERNEL/selector.h>
#include <BALL/KERNEL/atom.h>

namespace BALL 
{
	
	Selector::Selector()
		throw()
		:	UnaryProcessor<Composite>(),
			selected_atoms_(),
			expression_()
	{
	}
	
	Selector::Selector(const String& expression_string)
		throw()
		:	UnaryProcessor<Composite>(),
			selected_atoms_(),
			expression_(expression_string)
	{
	}

	Selector::Selector(const Selector& selector)
		throw()
		:	UnaryProcessor<Composite>(selector),
			selected_atoms_(selector.selected_atoms_),
			expression_(selector.expression_)
	{
	}

	Selector::~Selector()
		throw()
	{
		clear();
	}

	void Selector::clear()
		throw()
	{
		expression_.clear();
		selected_atoms_.clear();
	}

	Selector& Selector::operator = (const Selector& selector)
		throw()
	{
		selected_atoms_ = selector.selected_atoms_;
		expression_ = selector.expression_;
		
		return *this;
	}

	bool Selector::operator == (const Selector& selector) const
		throw()
	{
		return ((selected_atoms_ == selector.selected_atoms_)
				&& (expression_ == selector.expression_));
	}

	Size Selector::getNumberOfSelectedAtoms() const
		throw()
	{
		return selected_atoms_.size();
	}

	void Selector::setExpression(const Expression& expression)
		throw()
	{
		clear();
		expression_ = expression;
	}

	const Expression& Selector::getExpression() const
		throw()
	{
		return expression_;
	}

	bool Selector::start() 
		throw()
	{
		// reset the number of selected atoms
		selected_atoms_.clear();

		// and continue
		return true;
	}

  Processor::Result Selector::operator () (Composite& composite)
		throw()
  {
		// if the composite is an atom, we apply the expression tree...
		if (RTTI::isKindOf<Atom>(composite))
		{
			Atom& atom = dynamic_cast<Atom&>(composite);
			if (expression_.operator () (atom))
			{
				// select the atoms and increase the atom counter
				atom.select();
				selected_atoms_.push_back(&atom);
			}
		}

    return Processor::CONTINUE;
	}

	List<Atom*>& Selector::getSelectedAtoms()
		throw()
	{
		return selected_atoms_;
	}

} // namespace BALL
