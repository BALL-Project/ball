// $Id: selector.C,v 1.21 2001/07/13 13:14:22 anker Exp $

#include <BALL/KERNEL/selector.h>

#include <BALL/CONCEPT/selectable.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/residue.h>
#include <BALL/KERNEL/nucleotide.h>
#include <BALL/KERNEL/bond.h>

using namespace std;

namespace BALL 
{
	
	Selector::Selector()
		throw()
		:	UnaryProcessor<Composite>(),
			number_of_selected_atoms_(0),
			expression_()
	{
	}
	
	Selector::Selector(const String& expression_string)
		throw()
		:	UnaryProcessor<Composite>(),
			number_of_selected_atoms_(0),
			expression_(expression_string)
	{
	}

	Selector::Selector(const Selector& selector)
		throw()
		:	UnaryProcessor<Composite>(selector),
			number_of_selected_atoms_(selector.number_of_selected_atoms_),
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
		// BAUSTELLE:
		// UnaryProcessor<> does not implement the OCI
		// UnaryProcessor<Composite>::clear();
		number_of_selected_atoms_ = 0;
		expression_.clear();
	}

	const Selector& Selector::operator = (const Selector& selector)
		throw()
	{
		// BAUSTELLE:
		// UnaryProcessor<> does not implement the OCI
		// UnaryProcessor<Composite>::operator = (selector);
		number_of_selected_atoms_ = selector.number_of_selected_atoms_;
		expression_ = selector.expression_;
		
		return *this;
	}

	bool Selector::operator == (const Selector& selector) const
		throw()
	{
		// BAUSTELLE:
		// UnaryProcessor<> does not implement the OCI
		// return ((UnaryProcessor<Composite>::operator == (selector))
		return ((number_of_selected_atoms_ == selector.number_of_selected_atoms_)
				&& (expression_ == selector.expression_));
	}

	Size Selector::getNumberOfSelectedAtoms() const
		throw()
	{
		return number_of_selected_atoms_;
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
		number_of_selected_atoms_ = 0;

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
				number_of_selected_atoms_++;
			}
		}

    return Processor::CONTINUE;
	}

} // namespace BALL
