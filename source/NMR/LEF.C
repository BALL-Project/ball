// $Id: LEF.C,v 1.7 2000/09/15 07:42:34 oliver Exp $

#include<BALL/NMR/LEF.h>

using namespace std;

namespace BALL 
{


	const int MAX_EXPR = 5;

	// default ctor
	LEFShiftProcessor::LEFShiftProcessor()
	{	
		ini_file_name_ = "/KM/fopra/compbio/burch/BALL/source/NMR/dat/nmr.ini";	
	}

		
	// destructor
	LEFShiftProcessor::~LEFShiftProcessor()
	{
	}

	void LEFShiftProcessor::setFilename(const String& filename)
	{
		ini_filename_ = filename;
	}
		
	// Processor start method
	bool LEFShiftProcessor::start()
	{
		// hier werden die Ladungen aus einem File eingelesen und zugewiesen
		// nein : wurden bereits in calculate_shifts() von NMRSPectrum zugewiesen
		// dort ist ein pointer auf das System das fuer den acp Processor benoetigt wird.

		// hier wird die Expression Liste aufgebaut fuer die shiftatome
		
		int number_of_keys;
		int counter;
		String description;
		
		parameters_ = new Parameters(ini_file_name_);
		parameter_section_ = new ParameterSection;
		
		parameter_section_->extractSection(*parameters_,"LEF-ShiftAtoms");
		
		number_of_keys = parameter_section_->getNumberOfKeys();
		number_of_expressions_ = number_of_keys;
		
		expressions_ = new Expression[MAX_EXPR];
		
		Position description_column = parameter_section_->getColumnIndex("description");
		
		for (counter = 0; counter < number_of_keys; counter++)
		{
			description = parameter_section_->getValue(counter,description_column);
			while (description.has('_'))
			{
				description.substitute("_"," ");
			}
				
			expressions_[counter].setExpression(description);
		}	
		
		// Aufbau der Expression fuer die Bindungsatom der ShiftAtome
		
		isSecondAtom_ = new Expression[MAX_EXPR];
		
		Position second_atom_column = parameter_section_->getColumnIndex("second_atom");
		
		for (counter = 0; counter < number_of_keys; counter++)
		{
			description = parameter_section_->getValue(counter, second_atom_column);
			while (description.has('_'))
			{
				description.substitute("_"," ");
			}
				
			isSecondAtom_[counter].setExpression(description);
		}
				
		return 1;
	}
		

	// Processor finish method
	bool LEFShiftProcessor::finish()
	{
		parameter_section_->extractSection(*parameters_,"LEF-ShiftAtoms");

		Vector3 proton,bindung,atom,prot_bin,prot_atom;
		float sigmaE,sc,theta,ladung,Ez,Efact,b_prot_bin,b_prot_atom,hshift;
		int test=0;
		int counter,key;

		int number_of_bonds;
		Bond *bond;

		list<PDBAtom*>::iterator atom_iter;
		list<PDBAtom*>::iterator effector_iter;
		atom_iter=atom_list_.begin();
		
		if ((atom_list_.begin() != NULL) && (effector_list_.begin() != NULL))
		{
			for (;atom_iter != atom_list_.end(); ++atom_iter)
			{
				test = 0;
				for(counter = 0; counter < number_of_expressions_; counter++)
				{
					if (expressions_[counter](*(*atom_iter)))
					{
						key=counter;
						counter=MAX_EXPR+1;
					}
				}
					
				sigmaE = parameter_section_->getValue(key,"sigmaE").toFloat();
				
				proton=(*atom_iter)->getPosition();
				
				number_of_bonds = (*atom_iter)->countBonds();
				for(counter=0;counter<number_of_bonds;counter++)
				{
					bond = (*atom_iter)->getBond(counter);
					if (isSecondAtom_[key](*bond->getBoundAtom(*(*atom_iter)))) 
					{
						bindung = (*atom_iter)->getBond(counter)->getBoundAtom(*(*atom_iter))->getPosition();
						counter = number_of_bonds + 1;
					}
				}
									
				bindung = (*atom_iter)->getBond(0)->getBoundAtom(*(*atom_iter))->getPosition();
				prot_bin = proton-bindung;
				Ez=0;
				for(effector_iter=effector_list_.begin();effector_iter!=effector_list_.end();++effector_iter)
				{
					if ( (*effector_iter)->getResidue()!=(*atom_iter)->getResidue() )
					{
						ladung = (*effector_iter)->getCharge();
						
						atom=(*effector_iter)->getPosition();
						prot_atom=proton-atom;			
						sc=prot_bin*prot_atom;
						b_prot_bin=prot_bin.getLength();
						b_prot_atom=prot_atom.getLength();
						//if (test) cout << "	abstand :" << b_prot_atom;
						theta=sc / (b_prot_bin*b_prot_atom);
						Efact=ladung/(b_prot_atom*b_prot_atom);
						Ez=Ez+(theta*Efact);
					}
				}
				hshift = -Ez*sigmaE;
				shift_ = (*atom_iter)->getProperty("chemical_shift").getFloat();
				shift_ -= hshift;
				(*atom_iter)->setProperty("chemical_shift", shift_);
				(*atom_iter)->setProperty("LEF",hshift);
			}
		}

		return 1;
	}
		
		
	// Processor operator ()
	Processor::Result LEFShiftProcessor::operator () (Composite& object)
	{
		// ist das Objekt ein Atom und noch dazu ein Wasserstoff
		// und hängt es an einem Kohlenstoff, so wird es in der _proton_list gespeichert
		int counter;
	
		if (RTTI::isKindOf<PDBAtom>(object))
		{
			patom_ = RTTI::castTo<PDBAtom>(object);
				
			for(counter=0;counter<number_of_expressions_;counter++)
			{
				if (expressions_[counter](*patom_))
				{
					atom_list_.push_back(patom_);
					counter=MAX_EXPR+1;
				}
			}
				
			if (patom_->getCharge()!=0) effector_list_.push_back(patom_);
				
		} // Ende is kind of PDBAtom : die Liste wurde um alle entsprechenden Atome  erweitert
			
				
		return Processor::CONTINUE;
	}

} // namespace BALL
