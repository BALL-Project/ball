#include<BALL/NMR/assignShiftProcessor.h>

using namespace std;

namespace BALL
{

	//Konstruktor
	BALL_INLINE 
	AssignShiftProcessor::AssignShiftProcessor(const vector<NMRAtomData*>& atomData)
		: 	atomData_(atomData)
	{
	}
					
	//Destruktor
	BALL_INLINE
	AssignShiftProcessor::~AssignShiftProcessor()
	{
	}

	bool AssignShiftProcessor::start()
	{
		for (Position i = 0; i < atomData_.size() ; i++)
		{
			shiftTable_[atomData_[i]->atomName] = atomData_[i]->shiftValue;
		}

		return true;
	}

	BALL_INLINE
	bool AssignShiftProcessor::finish()
	{ 
		return true;
	}

	Processor::Result AssignShiftProcessor::operator()(Object&  object)
	{
		// lese aus der shift_table den entsprechenden Eintrag und setze die Property
		float shift;
		String atomName;

		if (RTTI::isKindOf<PDBAtom>(object))
		{
			cerr  << endl << "Object ist ProteinAtom";
			patom_=RTTI::castTo<PDBAtom>(object);

			if (patom_->getElement()==PTE[Element::H])
			{
				atomName=patom_->getFullName();
				atomName.substitute("-", "");
				patom_->clearProperty("chemical_shift");
			
				if (shiftTable_.has(atomName))
				{
					cerr << endl <<"Eintrag gefunden:"<<atomName;
					shift = shiftTable_[atomName];
					patom_->setProperty("chemical_shift", shift);
				}
				else 
				{
					cerr << endl << "Eintrag nicht gefunden:"<<atomName;   
				}               
			}
		}
		else
		{
			cerr << endl << "uebergebenes Object ist kein PDBAtom" << endl;
		}

		return Processor::CONTINUE;
	}

}//namespace BALL
