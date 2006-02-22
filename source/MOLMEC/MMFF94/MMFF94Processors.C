// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: MMFF94Processors.C,v 1.1.2.2 2006/02/22 23:09:28 amoll Exp $
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
	AtomBondIterator bit = atom.beginBond();
	for (; +bit; ++bit)
	{
		const Atom* const atom2 = bit->getPartner(atom);
		Position bt = bit->hasProperty("MMFF94SBMB"); // ????
		const double pcharge = es_parameters_->getPartialCharge(atom.getType(), atom2->getType(), bt);
		if (pcharge == -99) 
		{
			unassigned_atoms_.insert(&atom);
			unassigned_atoms_.insert((Atom*)atom2);
		}
		charge += pcharge;
	}

	atom.setCharge(charge);

	return Processor::CONTINUE;
}

bool MMFF94ChargeProcessor::start()
{
	return es_parameters_ != 0;
}

} // namespace BALL
