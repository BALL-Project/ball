// $Id: LEF.C,v 1.3 2000/07/25 21:24:54 oliver Exp $

#include<BALL/NMR/LEF.h>

using namespace std;

namespace BALL 
{

//Konstruktor

LEFShift::LEFShift()
	{	
	//cout << endl << "LEFShift::LEFShift()";
	//pb_=new FDPB;
	//pb_->options.setReal(FDPB::Option::SOLVENT_DC,1.0);
	}

	
//Destruktor

LEFShift::~LEFShift()
	{
	//cout << endl << "LEFShift::~LEFShift()";
	}


//StartFunktion

bool LEFShift::start()
	{
	// hier passiert nichts
	//cout << endl << "LEFShift::start()";
	return 1;
	}
	

//FinishFunktion

bool LEFShift::finish()
	{
	//cout << endl << "LEF Modul";

	Vector3 proton,bindung,atom,prot_bin,prot_atom;
	float sc,theta,ladung,Ez,Efact,b_prot_bin,b_prot_atom;

	const float QC=1.6;
	const float QN=-1.7;
	const float QH=0.7;
	const float QO=-2.3;
	const float sigmaE=0.6;

	list<PDBAtom*>::iterator proton_iter;
	list<PDBAtom*>::iterator atom_iter;
	proton_iter=proton_list_.begin();
	
	if ( (proton_list_.size() > 0) && (atom_list_.size() > 0))
		{
		for(;proton_iter!=proton_list_.end();++proton_iter)
			{
			//cout << endl<< "Bearbeite jetzt Hydrogen : " << (*proton_iter)->getName();
			proton=(*proton_iter)->getPosition();
			bindung=(*proton_iter)->getBond(0)->getBoundAtom(*(*proton_iter))->getPosition();
			prot_bin=proton-bindung;
			Ez=0;
			for(atom_iter=atom_list_.begin();atom_iter!=atom_list_.end();++atom_iter)
				{
				//cout << endl <<"	"<< (*atom_iter)->getName()<<"?";
				if ( (*atom_iter)->getResidue()!=(*proton_iter)->getResidue() )
					{
					//cout << " jep";
					if ((*atom_iter)->getName()=="C") ladung=QC;
					if ((*atom_iter)->getName()=="N") ladung=QN;
					if ((*atom_iter)->getName()=="H") ladung=QH;
					if ((*atom_iter)->getName()=="O") ladung=QO;
					//cout << "	ladung:"<< ladung;
					atom=(*atom_iter)->getPosition();
					prot_atom=proton-atom;			
					sc=prot_bin*prot_atom;
					b_prot_bin=prot_bin.getLength();
					b_prot_atom=prot_atom.getLength();
					theta=sc / (b_prot_bin*b_prot_atom);
					Efact=ladung/(b_prot_atom*b_prot_atom);
					Ez=Ez+(theta*Efact);
					}
				}
			shift_=(*proton_iter)->getProperty("chemical_shift").getFloat();
			shift_-=Ez*sigmaE;
			(*proton_iter)->setProperty("chemical_shift", shift_);
			//Log.level(LogStream::INFORMATION)  << "chemical_shift :" << shift_ << endl;
			//cout << endl <<"LEF:Residue :"<<(*proton_iter)->getResidue()->getName()<<" Atom :"<<(*proton_iter)->getName()<< " chemical_shift :" << shift_;
			
			}
		}
	return 1;
	}
	
	
//apply Funktion

Processor::Result LEFShift::operator()(Object&  object)
	{
	// ist das Objekt ein Atom und noch dazu ein Wasserstoff
	// und hängt es an einem Kohlenstoff, so wird es in der _proton_list gespeichert
	
	if (RTTI::isKindOf<PDBAtom>(object))
		{
		//cout  << endl << "Object is ProteinAtom";

		// Baustelle : überprüfe ob Hydrogen an einem Kohlenstoff hängt !!
		patom_=RTTI::castTo<PDBAtom>(object);
		//cout  << endl << "		atom name : " << patom_->getName();
		//cout  << endl << "		Element : " << (patom_->getElement()).getName();
		if (patom_->getElement()==PTE[Element::H])
			{
			if (patom_->getName()=="H")
				{
				atom_list_.push_back(patom_);
				//cout << "...in atom_list_ eingefuegt.";
				}
			else 
				{

				// Wasserstoffe haben nur eine Bindung : Index = 1 oder 0 ?
				// Hole diese Bindung und überprüfe ob Kohlenstoff

				if (patom_->isBound())
					{
					if ((patom_->getBond(0)->getBoundAtom(*patom_)->getElement())==PTE[Element::C])
						{ 
						proton_list_.push_back(patom_);
	 	 				//cout  << "...in proton_list_ eingefuegt."; 	
	 					} 				
					}
				}
			}
		else if ( (patom_->getName()=="C") || (patom_->getName()=="N") || (patom_->getName()=="O") )
			{
			atom_list_.push_back(patom_);
			//cout << "...in atom_list_ eingefuegt.";
			}	

		} // Ende is kind of ProteinAtom : die Listen wurden gesetzt
		
	return Processor::CONTINUE;
	}

} // namespace Ball
