// $Id: HBondShiftProcessor.C,v 1.2 2001/02/15 14:55:04 anhi Exp $

#include <BALL/NMR/HBondShiftProcessor.h>
#include <stdio.h>

using namespace std;

namespace BALL
{
  const char* HBondShiftProcessor::PROPERTY__HBOND_SHIFT = "HBondShift";

  HBondShiftProcessor::HBondShiftProcessor()
    throw()
    : ShiftModule()
  {
  }

  HBondShiftProcessor::HBondShiftProcessor(const HBondShiftProcessor& processor)
    throw()
    : ShiftModule(processor),
      donator_list_(processor.donator_list_),
      acceptor_list_(processor.acceptor_list_),
      a_(processor.a_),
      b_(processor.b_),
      minimum_bond_length_(processor.minimum_bond_length_),
      maximum_bond_length_(processor.maximum_bond_length_)
  {
  }

  HBondShiftProcessor::~HBondShiftProcessor()
    throw()
  {
  }

  void HBondShiftProcessor::init()
    throw()
  {
		// by default, we assume the worst...
    valid_ = false;

    // if no parameters are assigned, abort immediately
    if (parameters_ == 0)
    {
      return;
    }

    // check that the parameter file contains the correct section...
    ParameterSection parameter_section;
    parameter_section.extractSection(*parameters_, "HBondEffect");

    // ...and that this section contains the correct parameters, and if so, assign them to variables
    if (!parameter_section.hasVariable("a") || !parameter_section.hasVariable("b")
	|| !parameter_section.hasVariable("minimum_bond_length") || !parameter_section.hasVariable("maximum_bond_length"))
    {
      return;
    }

    String numdum;
    numdum = parameter_section.getValue("a", "value");
    a_ = numdum.toFloat();
    numdum = parameter_section.getValue("b", "value");
    b_ = numdum.toFloat();
    numdum = parameter_section.getValue("minimum_bond_length", "value");
    minimum_bond_length_ = numdum.toFloat();
    numdum = parameter_section.getValue("maximum_bond_length", "value");
    maximum_bond_length_ = numdum.toFloat();
 
    // mark the module as initialized
    valid_ = true;
  }

  bool HBondShiftProcessor::start()
    throw()
  {
    // if the module is invalid, abort
    if (!isValid())
    {
      return false;
    }

    // clear the donator list and the acceptor list
    donator_list_.clear();
    acceptor_list_.clear();

    return true;
  }

  bool HBondShiftProcessor::finish()
    throw()
  {
    // if the module is invalid, abort
    if (!isValid())
    {
      return false;
    }

    // if there were no donators or acceptors, return immediately
    if ((donator_list_.size() == 0) || acceptor_list_.size() == 0)
    {
      return true;
    }

    // iterate over all donators
    list<Atom*>::iterator donator_it = donator_list_.begin();
    for (; donator_it != donator_list_.end(); ++donator_it)
    {
      Vector3 donator_pos = (*donator_it)->getPosition();
      float delta_HBOND = 0.0;
      Size hbond_number = 0;
      float nearest_acc = 0.0;
		 
      // iterate over all acceptors
      list<Atom*>::iterator acceptor_it = acceptor_list_.begin();
      for (; acceptor_it != acceptor_list_.end(); ++acceptor_it)
      { 

	// Test: use the distances of the hydrogens instead of the ones of the residues.
	BALL::Atom::BondConstIterator bi = (*donator_it)->beginBond();
	
	for (; +bi; ++bi)
	  {
	    Atom* at = (*bi).getPartner(**donator_it);
	    if (at->getElement() == PTE[Element::H])
	      {
		// print the partners of the assumed hydrogen bond
		
		// compute the euclidian distance between acceptor and donator
		Vector3 acceptor_pos = (*acceptor_it)->getPosition();
		Vector3 donator_h_pos = (at)->getPosition();
		
		// this is used to compute the shift	
		float b_length = donator_h_pos.getDistance(acceptor_pos);
		
		// this is used to decide whether there is an hbond or not
		float distance = donator_pos.getDistance(acceptor_pos);
	
		// this is a test! empirical guess
		// float b_length = (2. / 3) * distance;
		
		// do they form a bond?
		// like in the paper by wuethrich, no bonds are accepted: - outside a certain range
		//                                                        - on the same residue
		//                                                        - on neighbouring residues in the chain
		if ((minimum_bond_length_ <= distance) && (maximum_bond_length_ >= distance) 
		    && (*acceptor_it)->getResidue() != (*donator_it)->getResidue()
		    && (abs((*acceptor_it)->getResidue()->getID().toInt() - (*donator_it)->getResidue()->getID().toInt()) > 1))
		  {
		    // increase the number of found HBonds for this donator
		    cout << (*donator_it)->getResidue()->getFullName() << ":" << (*donator_it)->getResidue()->getID() << " -> ";
		    cout << (*acceptor_it)->getResidue()->getFullName() << ":" << (*acceptor_it)->getResidue()->getID() << " " << endl;
		    hbond_number++;
		    if (hbond_number > 1)
		      {
			// only accept the nearest partner
			if (b_length <= nearest_acc)
			  {
			    // compute their contribution to the chemical shift
			    delta_HBOND = a_ * 1./pow(b_length, 3) + b_;
			    cout << "#: " << hbond_number << " length: " << b_length << " " << distance << " " << delta_HBOND << endl;
			    nearest_acc = b_length;
			    hbond_number = 1;
cerr << (*donator_it)->getResidue()->getFullName() << (*donator_it)->getResidue()->getID() << " " << (*donator_it)->getName() << " -> ";
cerr << (*acceptor_it)->getResidue()->getFullName() << (*acceptor_it)->getResidue()->getID() << " " << (*acceptor_it)->getName() << " " << b_length << " " << distance << endl;
			  }
		      }
		    else
		      {
cerr << (*donator_it)->getResidue()->getFullName() << (*donator_it)->getResidue()->getID() << " " << (*donator_it)->getName() << " -> ";
cerr << (*acceptor_it)->getResidue()->getFullName() << (*acceptor_it)->getResidue()->getID() << " " << (*acceptor_it)->getName() << " " << b_length << " " << distance << endl;
			delta_HBOND = a_ * 1./pow(b_length, 3) + b_;
			cout << "#: " << hbond_number << " length: " << b_length << " " << distance << " " << delta_HBOND << endl;
			nearest_acc = b_length;
		      }
		  }
	      }
	  }
	bi = (*donator_it)->beginBond();
	
      }
      BALL::Atom::BondConstIterator bi = (*donator_it)->beginBond();
      if (hbond_number != 0)
	{ 
	  delta_HBOND /= (float) hbond_number; 
	  for (; +bi; ++bi)
	    {
	      Atom* at = (*bi).getPartner(**donator_it);
	      if (at->getElement() == PTE[Element::H])
		{
		  float shift = (at)->getProperty(ShiftModule::PROPERTY__SHIFT).getFloat();
		  shift += delta_HBOND;	
		  (at)->setProperty(ShiftModule::PROPERTY__SHIFT, shift);
		  (at)->setProperty(PROPERTY__HBOND_SHIFT, delta_HBOND);
		}
	    }
	}
    } 
    return true;
  }
  
  Processor::Result HBondShiftProcessor::operator () (Composite& object)
    throw()
  {
    // here, we collect all possible acceptors and donators
    if (RTTI::isKindOf<Atom>(object))
    {
      Atom* atom_ptr = RTTI::castTo<Atom>(object);

      // ??? Do I have to use the fragment or the residue of the atom???
      const String& residue_name = atom_ptr->getFragment()->getName();
      const String& atom_name = atom_ptr->getName();

      if (   ((residue_name == "ALA") && (atom_name == "N"))
	  || ((residue_name == "ARG") && ((atom_name == "N") || (atom_name == "NH1") || (atom_name == "NH2")))
	  || ((residue_name == "ASN") && ((atom_name == "N") || (atom_name == "ND2")))
	  || ((residue_name == "ASP") && (atom_name == "N"))
	  || ((residue_name == "CYS") && (atom_name == "N"))
	  || ((residue_name == "GLN") && ((atom_name == "N") || (atom_name == "NE2")))
	  || ((residue_name == "GLU") && (atom_name == "N"))
	  || ((residue_name == "GLY") && (atom_name == "N"))
	  || ((residue_name == "HIS") && ((atom_name == "N") || (atom_name == "NE2")))
	  || ((residue_name == "ILE") && (atom_name == "N"))
	  || ((residue_name == "LEU") && (atom_name == "N"))
	  || ((residue_name == "LYS") && ((atom_name == "N") || (atom_name == "NZ")))
	  || ((residue_name == "MET") && (atom_name == "N"))
	  || ((residue_name == "PHE") && (atom_name == "N"))
	  || ((residue_name == "PRO") && (atom_name == "N"))
	  || ((residue_name == "SER") && ((atom_name == "N") || (atom_name == "OG")))
	  || ((residue_name == "THR") && ((atom_name == "N") || (atom_name == "OG1")))
	  || ((residue_name == "TRP") && (atom_name == "N"))
	  || ((residue_name == "TYR") && ((atom_name == "N") || (atom_name == "OH")))
	  || ((residue_name == "VAL") && (atom_name == "N")))
      {
	donator_list_.push_back(atom_ptr);
      }
     
      if (   ((residue_name == "ALA") && (atom_name == "O"))
	  || ((residue_name == "ARG") && (atom_name == "O"))
	  || ((residue_name == "ASN") && (atom_name == "O"))
	  || ((residue_name == "ASP") && ((atom_name == "O") || (atom_name == "OD1") || (atom_name == "OD2")))
	  || ((residue_name == "CYS") && ((atom_name == "O") || (atom_name == "SG")))
	  || ((residue_name == "GLN") && (atom_name == "O"))
	  || ((residue_name == "GLU") && ((atom_name == "O") || (atom_name == "OE1") || (atom_name == "OE2")))
	  || ((residue_name == "GLY") && (atom_name == "O"))
	  || ((residue_name == "HIS") && ((atom_name == "O") || (atom_name == "ND1") || (atom_name == "NE2")))
	  || ((residue_name == "ILE") && (atom_name == "O"))
	  || ((residue_name == "LEU") && (atom_name == "O"))
	  || ((residue_name == "LYS") && (atom_name == "O"))
	  || ((residue_name == "MET") && ((atom_name == "O") || (atom_name == "SD")))
	  || ((residue_name == "PHE") && (atom_name == "O"))
	  || ((residue_name == "PRO") && (atom_name == "O"))
	  || ((residue_name == "SER") && ((atom_name == "O") || (atom_name == "OG")))
	  || ((residue_name == "THR") && ((atom_name == "O") || (atom_name == "OG1")))
	  || ((residue_name == "TRP") && (atom_name == "O"))
	  || ((residue_name == "TYR") && (atom_name == "OH"))
	  || ((residue_name == "VAL") && (atom_name == "O")))
      { 
	acceptor_list_.push_back(atom_ptr);
      }
    }

    return Processor::CONTINUE;
  }
} // namespace BALL
