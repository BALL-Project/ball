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
		:	UnaryProcessor<Composite>(),
			selected_atoms_(),
			expression_()
	{
	}
	
	Selector::Selector(const String& expression_string)
		:	UnaryProcessor<Composite>(),
			selected_atoms_(),
			expression_(expression_string)
	{
	}

	Selector::Selector(const Selector& selector)
		:	UnaryProcessor<Composite>(selector),
			selected_atoms_(selector.selected_atoms_),
			expression_(selector.expression_)
	{
	}

	Selector::~Selector()
	{
		clear();
	}

	void Selector::clear()
	{
		expression_.clear();
		selected_atoms_.clear();
	}

	Selector& Selector::operator = (const Selector& selector)
	{
		selected_atoms_ = selector.selected_atoms_;
		expression_ = selector.expression_;
		
		return *this;
	}

	bool Selector::operator == (const Selector& selector) const
	{
		return ((selected_atoms_ == selector.selected_atoms_)
				&& (expression_ == selector.expression_));
	}

	Size Selector::getNumberOfSelectedAtoms() const
	{
		return selected_atoms_.size();
	}

	void Selector::setExpression(const Expression& expression)
	{
		clear();
		expression_ = expression;
	}

	const Expression& Selector::getExpression() const
	{
		return expression_;
	}

	bool Selector::start() 
	{
		// reset the number of selected atoms
		selected_atoms_.clear();

		// and continue
		return true;
	}

  Processor::Result Selector::operator () (Composite& composite)
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

	list<Atom*>& Selector::getSelectedAtoms()
	{
		return selected_atoms_;
	}

} // namespace BALL
