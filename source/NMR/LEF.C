// $Id: LEF.C,v 1.6 2000/09/08 07:13:24 oliver Exp $

#include<BALL/NMR/LEF.h>
#include<BALL/KERNEL/atom.h>
#include<BALL/KERNEL/bond.h>
#include<BALL/KERNEL/PTE.h>

using namespace std;

namespace BALL
{
	// def. constructor
	LEFShift::LEFShift()
	{
	}


	// destructor
	LEFShift::~LEFShift()
	{
	}


	// finish method
	bool LEFShift::finish()
	{
		//cout << endl << "LEF Modul";

		Vector3 proton;
		Vector3 bond;
		Vector3 atom;
		Vector3 prot_bin, prot_atom;
		float sc, theta, charge, Ez, Efact, b_prot_bin, b_prot_atom;


		// units? / e0 = 1.602e-19 C = 4.80287e-10 esu (charge)
		//  values are in 1e-10 esu:
		const float QC = 1.6;				// =  0.333 e0
		const float QN = -1.7;			// = -0.354 e0
		const float QH = 0.7;				// =  0.146 e0
		const float QO = -2.3;			// = -0.479 e0
		const float sigmaE = 0.6;

	  list<Atom*>::iterator proton_iter;
	  list<Atom*>::iterator atom_iter;
	  proton_iter = proton_list_.begin();

		if ((proton_list_.size() > 0) && (atom_list_.size() > 0))
		{
			for (; proton_iter != proton_list_.end(); ++proton_iter)
			{
				//cout << endl<< "Bearbeite jetzt Hydrogen : " << (*proton_iter)->getName();
				proton = (*proton_iter)->getPosition();
				bond = (*proton_iter)->getBond (0)->getBoundAtom (*(*proton_iter))->getPosition();
				prot_bin = proton - bond;
				Ez = 0;
				for (atom_iter = atom_list_.begin(); atom_iter != atom_list_.end(); ++atom_iter)
				{
					//cout << endl <<"  "<< (*atom_iter)->getName()<<"?";
					if ((*atom_iter)->getFragment() != (*proton_iter)->getFragment())
					{
						if ((*atom_iter)->getName() == "C")
						{
							charge = QC;
						}
						if ((*atom_iter)->getName() == "N")
						{
							charge = QN;
						}
						if ((*atom_iter)->getName() == "H")
						{
							charge = QH;
						}
						if ((*atom_iter)->getName() == "O")
						{
							charge = QO;
						}

						//cout << " charge:"<< charge;

						atom = (*atom_iter)->getPosition();
						prot_atom = proton - atom;
						sc = prot_bin * prot_atom;
						b_prot_bin = prot_bin.getLength();
						b_prot_atom = prot_atom.getLength();
						theta = sc / (b_prot_bin * b_prot_atom);
						Efact = charge / (b_prot_atom * b_prot_atom);
						Ez = Ez + (theta * Efact);
					}
				}
				float shift = (*proton_iter)->getProperty("chemical_shift").getFloat();
				shift -= Ez * sigmaE;
				(*proton_iter)->setProperty("chemical_shift", shift);
			}
		}

		return true;
	}


	//apply method
	Processor::Result LEFShift::operator() (Composite& composite)
	{
		// ist das Objekt ein Atom und noch dazu ein Wasserstoff
		// und hängt es an einem Kohlenstoff, so wird es in der _proton_list gespeichert

		if (RTTI::isKindOf<Atom>(composite))
		{
			// Baustelle : überprüfe ob Hydrogen an einem Kohlenstoff hängt !!
			patom_ = RTTI::castTo<Atom>(composite);
			//cout  << endl << "    atom name : " << patom_->getName();
			//cout  << endl << "    Element : " << (patom_->getElement()).getName();
			if (patom_->getElement() == PTE[Element::H])
			{
				if (patom_->getName() == "H")
				{
					atom_list_.push_back (patom_);
				}
				else
				{

					// Wasserstoffe haben nur eine Bindung : Index = 1 oder 0 ?
					// Hole diese Bindung und überprüfe ob Kohlenstoff
					if (patom_->isBound())
					{
						if ((patom_->getBond (0)->getBoundAtom (*patom_)->getElement()) == PTE[Element::C])
						{
							proton_list_.push_back (patom_);
						}
					}
				}
			}
			else if ((patom_->getName() == "C") || (patom_->getName() == "N") || (patom_->getName() == "O"))
			{
				atom_list_.push_back (patom_);
			}

		}	

		return Processor::CONTINUE;
	}

}	// namespace BALL
