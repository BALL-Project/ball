// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: partialChargeBase.C,v 1.1 2004/05/11 07:28:21 oliver Exp $
//

#include <BALL/QSAR/partialChargeBase.h>
#include <BALL/QSAR/partialChargeProcessor.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/bondIterator.h>
#include <BALL/KERNEL/atomIterator.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/fragment.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/CONCEPT/timeStamp.h>

#include <utility>
#include <iostream>

using namespace std;

#define BALL_QSAR_PARTIALCHARGEBASE_DEBUG
#undef  BALL_QSAR_PARTIALCHARGEBASE_DEBUG

namespace BALL
{
	PartialChargeBase::PartialChargeBase()
		:	Descriptor()
	{
	}

	PartialChargeBase::PartialChargeBase(const PartialChargeBase& pcb)
		:	Descriptor(pcb)
	{
	}

	PartialChargeBase::PartialChargeBase(const String& name)
		:	Descriptor(name)
	{
	}

	PartialChargeBase::PartialChargeBase(const String& name, const String& unit)
		:	Descriptor(name, unit)
	{
	}

	PartialChargeBase::~PartialChargeBase()
	{
	}

	PartialChargeBase& PartialChargeBase::operator = (const PartialChargeBase& pcb)
	{
		this->setName(pcb.getName());
		this->setUnit(pcb.getUnit());
		return *this;
	}

  bool PartialChargeBase::isValid(Molecule& molecule)
  {
		static HashMap<Handle, PreciseTime> mod_times;
		PreciseTime last_mod = molecule.getModificationTime();
		Handle mol_handle = molecule.getHandle();
		if (mod_times.has(mol_handle))
		{
			if (mod_times[mol_handle] == last_mod)
			{
				#ifdef BALL_QSAR_PARTIALCHARGEBASE_DEBUG
				cerr << ">> PartialChargeBase::isValid: molcule valid!" << endl;
				#endif
				return true;
			}
			else
			{
				mod_times[mol_handle] = last_mod;
				#ifdef BALL_QSAR_SIMPLEBASE_DEBUG
				cerr << ">> PartialChargeBase::isValid: molecule not valid, modified!" << endl;
				#endif
				return false;
			}
		}
		else
		{
			mod_times.insert(make_pair(mol_handle, last_mod));
			#ifdef BALL_QSAR_PARTIALCHARGEBASE_DEBUG
			cerr << ">> PartialChargeBase::isValid: molecule not valid, first call!" << endl;
			#endif
			return false;
		}
	}


	void PartialChargeBase::calculate(Molecule& molecule)
	{
		// sets partial charges;
		PartialChargeProcessor pcp;
		molecule.apply(pcp);
	
		//HashMap<Atom*, double>::Iterator it = charges.begin();	
		AtomIterator it = molecule.beginAtom();
		// assign the calculated values to the descriptors
		double tot_pos(0), tot_neg(0), rel_pos(0), rel_neg(0), max_charge(0), min_charge(0);	
		for (it=molecule.beginAtom();it!=molecule.endAtom();++it)
		{
			double charge = it->getProperty("PEOEPartialCharge").getDouble();
			if (charge > 0)
			{
				tot_pos += charge;
			} 
			else
			{
				tot_neg += charge;
			}
			if (charge > max_charge)
			{
				max_charge = charge;
			}
			if (charge < min_charge)
			{
				min_charge = charge;
			}
			// set partial charge as atom property
		}

		// this might be paranoid, but you never know
		if (max_charge > 0 && tot_pos > 0)
		{
			rel_pos = max_charge/tot_pos;
		}
		// this might be also paranoid, but you still never know
		if (min_charge < 0 && tot_neg < 0)
		{
			rel_neg = min_charge/tot_neg;
		}
		
		molecule.setProperty("TotalPositivePartialCharge", tot_pos);
		molecule.setProperty("TotalNegativePartialCharge", tot_neg);
		molecule.setProperty("RelPositivePartialCharge", rel_pos);
		molecule.setProperty("RelNegativePartialCharge", rel_neg);
	}

} // namespace BALL
