// $Id: randomCoil.C,v 1.3 2000/09/08 07:13:24 oliver Exp $

#include<BALL/NMR/randomCoil.h>

using namespace std;

namespace BALL
{

//Konstruktor

	RandomCoilShift::RandomCoilShift ()
	{
		// setzen des Standardpfades für die randomcoilshift daten

		file_ = "../div/random.dat";

	}


//Destruktor

	RandomCoilShift::~RandomCoilShift ()
	{
	}


//StartFunktion

	bool RandomCoilShift::start ()
	{
		// aufbau der Stringhashmap rc_table aus dem file random.dat

		String residue, atom, eintrag;
		float rc_shift;

		ifstream infile (file_, ios::in);

		do
		{
			infile >> residue;
			infile >> atom;
			infile >> rc_shift;
			eintrag = residue;
			eintrag.append (":");
			eintrag.append (atom);
			rc_table_[eintrag] = rc_shift;
		}
		while (residue != "END");

		return 1;
	}

	bool RandomCoilShift::finish ()
	{
		// hier passiert nichts
		return 1;
	}

	Processor::Result RandomCoilShift::operator () (Composite& composite)
	{
		// lese aus der rc_table den entsprechenden Eintrag und addiere zum shift

		float rc_shift;
		String residue, atom, eintrag;

		if (RTTI::isKindOf<PDBAtom>(composite))
		{
			//cout  << endl << "Object is ProteinAtom";
			patom_ = RTTI::castTo<PDBAtom>(composite);

			if (patom_->getElement () == PTE[Element::H])
			{
				residue = patom_->getResidue ()->getName ();
				atom = patom_->getName ();
				eintrag = residue;
				eintrag.append (":");
				eintrag.append (atom);
				if (rc_table_.has (eintrag))
				{
					//cout << endl <<"Eintrag gefunden:"<<eintrag;
					rc_shift = rc_table_[eintrag];
					shift_ = patom_->getProperty ("chemical_shift").getFloat ();
					shift_ -= rc_shift;
					patom_->setProperty ("chemical_shift", shift_);
				}
				else
				{
					shift_ = 1000;
					patom_->setProperty ("chemical_shift", shift_);
					//cout << endl << "Eintrag nicht gefunden:"<<eintrag; 
				}
				//cout << endl <<"RC:Residue :"<<patom_->getResidue()->getName()<<" Atom :"<<patom_->getName()<< " chemical_shift :" << shift_;

			}
		}
		return Processor::CONTINUE;
	}

}	//namespace BALL
