// $Id: randomCoilShiftProcessor.C,v 1.2 2000/09/19 13:34:28 oliver Exp $

#include<BALL/NMR/randomCoilShiftProcessor.h>

using namespace std;

namespace BALL
{
	//Konstruktor

	RandomCoilShiftProcessor::RandomCoilShiftProcessor()
		throw()
	{
		ini_filename_="/KM/fopra/compbio/burch/BALL/source/NMR/dat/nmr.ini";
	}

		
	//Destruktor

	RandomCoilShiftProcessor::~RandomCoilShiftProcessor()
		throw()
	{
	}


	//StartFunktion

	bool RandomCoilShiftProcessor::start()
		throw()
	{
		// aufbau der ParameterSection ShiftData
		
		//cout << "RandomCoilShiftProcessor::start()" << endl;
		
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

	bool RandomCoilShiftProcessor::finish()
		throw()
	{ 
		return true;
	}

	Processor::Result RandomCoilShiftProcessor::operator()(Composite&  object)
		throw()
	{
		// lese aus der rc_table den entsprechenden Eintrag und addiere zum shift

		//cout << "RandomCoilShiftProcessor::operator()" << endl;

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

