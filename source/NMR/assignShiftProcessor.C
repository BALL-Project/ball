// $Id: assignShiftProcessor.C,v 1.4 2000/09/16 07:44:12 oliver Exp $

#include<BALL/NMR/assignShiftProcessor.h>

#include <BALL/KERNEL/PTE.h>

using namespace std;

namespace BALL
{

	bool AssignShiftProcessor::start()
	{
		if (!valid_)
		{
			Log.error() << endl << "AssignShiftProcessor: shift data were not assigned" << endl;
			return false;
		}

		for (Position i = 0; i < atom_data_.size() ; i++)
		{
			shift_table_[atom_data_[i]->atomName] = atom_data_[i]->shiftValue;
		}

		return true;
	}

	Processor::Result AssignShiftProcessor::operator()(Composite&  object)
	{
		float shift;
		String atomName;
		PDBAtom *patom_;

		if (RTTI::isKindOf<PDBAtom>(object))
		{
			patom_=RTTI::castTo<PDBAtom>(object);

			if (patom_->getElement() == PTE[Element::H] || patom_->getElement() == PTE[Element::N])
			{
				atomName = patom_->getFullName();
				atomName.substitute("-", "");
				patom_->clearProperty("chemical_shift");
			
				if (shift_table_.has(atomName))
				{
					shift = shift_table_[atomName];
					patom_->setProperty("chemical_shift", shift);
				}
				else 
				{
					Log.error() << endl << "AssignShiftProcessor: entry not found:"<<atomName;   
				}               
			}
		}
		else
		{
			Log.error() << endl << "AssignShiftProcessor: given object is not a PDBAtom" << endl;
		}

		return Processor::CONTINUE;
	}

}//namespace BALL
