// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: MMFF94Processors.C,v 1.1.2.3 2006/02/23 15:42:27 amoll Exp $
//

#include <BALL/MOLMEC/MMFF94/MMFF94Processors.h>

#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/bond.h>

//    #define BALL_DEBUG_MMFF
#define BALL_DEBUG_TEST

using namespace std;

namespace BALL 
{

MMFF94ChargeProcessor::MMFF94ChargeProcessor()
	:	UnaryProcessor<Atom>(),
		es_parameters_(0)
{
}

MMFF94ChargeProcessor::MMFF94ChargeProcessor(const MMFF94ChargeProcessor& p)
	:	UnaryProcessor<Atom>(p),
		es_parameters_(0)
{
}

void MMFF94ChargeProcessor::clear()
	throw()
{
}

const MMFF94ChargeProcessor& MMFF94ChargeProcessor::operator = (const MMFF94ChargeProcessor&)
	throw()
{
	return *this;
}


Processor::Result MMFF94ChargeProcessor::operator () (Atom& atom)
{
	// ???????? partial charge
	
	double charge = atom.getCharge();
Log.error() << "#~~#   7 "   << atom.getName() << " " << charge         ;
	AtomBondIterator bit = atom.beginBond();
	Index at1 = atom.getType();
	for (; +bit; ++bit)
	{
		const Atom* const atom2 = bit->getPartner(atom);
		Index at2 = atom2->getType();
		Position bt = bit->hasProperty("MMFF94SBMB"); // ????
		double pcharge;
		
		if (at1 < at2) pcharge = - es_parameters_->getPartialCharge(at1, at2, bt);
		else           pcharge =   es_parameters_->getPartialCharge(at2, at1, bt);

		if (fabs(pcharge) == 99) 
		{
			unassigned_atoms_.insert(&atom);
			unassigned_atoms_.insert((Atom*)atom2);
			continue;
		}
Log.error() << " " << atom2->getName() << " " << pcharge;
		charge += pcharge;
	}

	atom.setCharge(charge);
Log.error() << "      :  "    << charge         << " "  << __FILE__ << "  " << __LINE__<< std::endl;

	return Processor::CONTINUE;
}

bool MMFF94ChargeProcessor::start()
{
	unassigned_atoms_.clear();
	return es_parameters_ != 0;
}

} // namespace BALL
