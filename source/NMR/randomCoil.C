// $Id: randomCoil.C,v 1.5 2000/09/18 17:09:43 oliver Exp $

#include<BALL/NMR/randomCoil.h>

using namespace std;

namespace BALL
{
	//Konstruktor

	RandomCoilShift::RandomCoilShift()
		throw()
	{
		ini_filename_="/KM/fopra/compbio/burch/BALL/source/NMR/dat/nmr.ini";
	}

		
	//Destruktor

	RandomCoilShift::~RandomCoilShift()
		throw()
	{
	}


	//StartFunktion

	bool RandomCoilShift::start()
		throw()
	{
		// aufbau der ParameterSection ShiftData
		
		//cout << "RandomCoilShift::start()" << endl;
		
		int number_of_keys;
		int counter;
		
		parameters_.setFilename(ini_filename_);
		
		// einlesen der shift Atome und liste von expressions aufbauen
		
		parameter_section_.extractSection(parameters_, "RC-ShiftAtoms");
		
		number_of_keys = parameter_section_.getNumberOfKeys();
		
		expressions_.clear();
		
		Position description_column = parameter_section_.getColumnIndex("description");
		
		String description;
		
		for(counter=0;counter<number_of_keys;counter++)
		{
			description = parameter_section_.getValue(counter,description_column);
			while (description.has('_'))
				description.substitute("_"," ");
			//cout << description << endl;
				
			expressions_.push_back(Expression(description));
		}
		
		return true;
	}

	bool RandomCoilShift::finish()
		throw()
	{ 
		return true;
	}

	Processor::Result RandomCoilShift::operator()(Composite&  object)
		throw()
	{
		// lese aus der rc_table den entsprechenden Eintrag und addiere zum shift

		//cout << "RandomCoilShift::operator()" << endl;

		parameter_section_.extractSection(parameters_,"RC-ShiftData");

		float rc_shift;
		String residue,atom,eintrag;
		Position counter;
		
		if (RTTI::isKindOf<PDBAtom>(object))
		{
			//cout  << endl << "Object is ProteinAtom";
			PDBAtom* atom_ptr = RTTI::castTo<PDBAtom>(object);
			
			for(counter = 0; counter < expressions_.size(); counter++)
			{
				if (expressions_[counter](*atom_ptr))
				{
					residue = atom_ptr->getResidue()->getName();
					atom= atom_ptr->getName();
					eintrag=residue;
					eintrag.append(" ");
					eintrag.append(atom);
					//cout << "suche nach :" <<eintrag << endl;
					if (parameter_section_.has(eintrag))
						{
						//cout <<"Eintrag gefunden:"<<eintrag<<endl;
						rc_shift=(parameter_section_.getValue(eintrag,"shift")).toFloat();
						rc_shift*=-1;
						//cout <<"Eintrag ist :"<<rc_shift << endl;
						
						float shift = atom_ptr->getProperty("chemical_shift").getFloat();
						shift +=rc_shift;
						atom_ptr->setProperty("chemical_shift", shift);
						atom_ptr->setProperty("RC",rc_shift);
						}
						else 
							{
							atom_ptr->clearProperty("chemical_shift");
							cout << "Eintrag nicht gefunden:"<<eintrag <<endl;
							}		
					//cout << endl <<"RC:Residue :"<<atom_ptr->getResidue()->getName()<<" Atom :"<<atom_ptr->getName()<< " chemical_shift :" << shift;
				
					}
				}
			}
			return Processor::CONTINUE;
		}

}//namespace BALL

