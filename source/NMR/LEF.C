// $Id: LEF.C,v 1.8 2000/09/15 08:52:22 oliver Exp $

#include<BALL/NMR/LEF.h>
#include <BALL/KERNEL/bond.h>

using namespace std;

namespace BALL 
{


	const int MAX_EXPR = 5;

	// default ctor
	LEFShiftProcessor::LEFShiftProcessor()
	{	
		ini_filename_ = "/KM/fopra/compbio/burch/BALL/source/NMR/dat/nmr.ini";	
	}

		
	// destructor
	LEFShiftProcessor::~LEFShiftProcessor()
	{
	}

	void LEFShiftProcessor::setFilename(const String& filename)
	{
		ini_filename_ = filename;
	}
		
	const String& LEFShiftProcessor::getFilename() const
	{
		return ini_filename_;
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
		
		parameters_.setFilename(ini_filename_);
		parameter_section_.extractSection(parameters_, "LEF-ShiftAtoms");
		
		number_of_keys = parameter_section_.getNumberOfKeys();

		// clear the arrays containing the expressions
		first_atom_expressions_.clear();
		second_atom_expressions_.clear();
		
		Position description_column = parameter_section_.getColumnIndex("description");
		
		for (counter = 0; counter < number_of_keys; counter++)
		{
			description = parameter_section_.getValue(counter,description_column);
			while (description.has('_'))
			{
				description.substitute("_"," ");
			}
				
			first_atom_expressions_.push_back(Expression(description));
		}	
		
		// Aufbau der Expression fuer die Bindungsatom der ShiftAtome
		
		Position second_atom_column = parameter_section_.getColumnIndex("second_atom");
		
		for (counter = 0; counter < number_of_keys; counter++)
		{
			description = parameter_section_.getValue(counter, second_atom_column);
			while (description.has('_'))
			{
				description.substitute("_"," ");
			}
				
			second_atom_expressions_.push_back(Expression(description));
		}
				
		return true;
	}
		

	// Processor finish method
	bool LEFShiftProcessor::finish()
	{
		parameter_section_.extractSection(parameters_,"LEF-ShiftAtoms");

		Vector3 proton, second_atom,atom,prot_bin,prot_atom;
		float sigmaE,sc,theta,ladung,Ez,Efact,b_prot_bin,b_prot_atom,hshift;
		Position test=0;
		Position counter, key;

		Bond *bond;

		list<Atom*>::iterator atom_iter;
		list<Atom*>::iterator effector_iter;
		atom_iter = atom_list_.begin();
		
		if ((atom_list_.size() > 0) && (effector_list_.size() > 0))
		{
			for (; atom_iter != atom_list_.end(); ++atom_iter)
			{
				test = 0;
				for (counter = 0; counter < first_atom_expressions_.size(); counter++)
				{
					if (first_atom_expressions_[counter](**atom_iter))
					{
						// exit the loop if we found a matching expression
						key = counter;
						break;
					}
				}
					
				sigmaE = parameter_section_.getValue(key, "sigmaE").toFloat();
				
				proton = (*atom_iter)->getPosition();
				
				Size number_of_bonds = (*atom_iter)->countBonds();
				for (counter = 0; counter < number_of_bonds; counter++)
				{
					bond = (*atom_iter)->getBond(counter);
					if (second_atom_expressions_[key](*bond->getBoundAtom(*(*atom_iter)))) 
					{
						second_atom = (*atom_iter)->getBond(counter)->getBoundAtom(*(*atom_iter))->getPosition();
						counter = number_of_bonds + 1;
					}
				}
									
				second_atom = (*atom_iter)->getBond(0)->getBoundAtom(*(*atom_iter))->getPosition();
				prot_bin = proton - second_atom;
				Ez=0;
				for(effector_iter=effector_list_.begin();effector_iter!=effector_list_.end();++effector_iter)
				{
					if ( (*effector_iter)->getFragment()!=(*atom_iter)->getFragment() )
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
				double shift = (*atom_iter)->getProperty("chemical_shift").getFloat();
				shift -= hshift;
				(*atom_iter)->setProperty("chemical_shift", shift);
				(*atom_iter)->setProperty("LEF", hshift);
			}
		}

		return true;
	}
		
		
	// Processor operator ()
	Processor::Result LEFShiftProcessor::operator () (Composite& object)
	{
		// ist das Objekt ein Atom und noch dazu ein Wasserstoff
		// und hängt es an einem Kohlenstoff, so wird es in der _proton_list gespeichert
		Position counter;
	
		if (RTTI::isKindOf<PDBAtom>(object))
		{
			Atom* atom_ptr = RTTI::castTo<PDBAtom>(object);
				
			for (counter = 0; counter < first_atom_expressions_.size(); counter++)
			{
				if (first_atom_expressions_[counter](*atom_ptr))
				{
					atom_list_.push_back(atom_ptr);
					counter=MAX_EXPR+1;
				}
			}
				
			if (atom_ptr->getCharge() != 0.0)
			{
				effector_list_.push_back(atom_ptr);
			}
		}
			
				
		return Processor::CONTINUE;
	}

} // namespace BALL
