// $Id: selector.C,v 1.18 2000/10/29 21:25:47 amoll Exp $

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
		:	number_of_selected_atoms_(0)
	{
	}
	
	Selector::Selector(const String& expression_string)
		:	number_of_selected_atoms_(0),
			expression_(expression_string)
	{
	}

	Selector::Selector(const Selector& selector)
		:	UnaryProcessor<Composite>(),
			number_of_selected_atoms_(selector.number_of_selected_atoms_)
	{
	}

	Selector::~Selector()
	{
	}

	Size Selector::getNumberOfSelectedAtoms() const
	{
		return number_of_selected_atoms_;
	}

	bool Selector::start() 
	{
		// reset the number of selected atoms
		number_of_selected_atoms_ = 0;

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
				number_of_selected_atoms_++;
			}
		}

    return Processor::CONTINUE;
	}

} // namespace BALL
