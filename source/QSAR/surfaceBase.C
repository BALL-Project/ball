// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: surfaceBase.C,v 1.2.28.1 2007-03-16 00:06:50 bertsch Exp $
//

#include <BALL/QSAR/surfaceBase.h>
#include <BALL/QSAR/partialChargeDescriptors.h>
#include <BALL/QSAR/simpleDescriptors.h>
#include <BALL/KERNEL/atomIterator.h>
#include <BALL/KERNEL/fragment.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/CONCEPT/timeStamp.h>
#include <BALL/STRUCTURE/numericalSAS.h>

#define BALL_QSAR_SURFACEBASE_DEBUG
#undef  BALL_QSAR_SURFACEBASE_DEBUG

using namespace std;

namespace BALL
{
	SurfaceBase::SurfaceBase()
		:	Descriptor()
	{
	}

	SurfaceBase::SurfaceBase(const SurfaceBase& sb)
		:	Descriptor(sb)
	{
	}
	
	SurfaceBase::SurfaceBase(const String& name)
		:	Descriptor(name)
	{
	}

	SurfaceBase::SurfaceBase(const String& name, const String& unit)
		:	Descriptor(name, unit)
	{
	}

	SurfaceBase& SurfaceBase::operator = (const SurfaceBase& sb)
	{
		this->setName(sb.getName());
		this->setUnit(sb.getUnit());
		return *this;
	}

	SurfaceBase::~SurfaceBase()
	{
	}

  bool SurfaceBase::isValid_(AtomContainer& ac)
  {
		static HashMap<Handle, PreciseTime> mod_times;
		PreciseTime last_mod = ac.getModificationTime(); 
		Handle mol_handle = ac.getHandle();
		if (mod_times.has(mol_handle))
		{
			if (mod_times[mol_handle] == last_mod)
			{
				return true;
			}
			else
			{
				mod_times[mol_handle] = last_mod;
				return false;
			}
		}
		else
		{
			mod_times.insert(std::make_pair(mol_handle, last_mod));
			return false;
		}
	}

	void SurfaceBase::computeAllDescriptors(AtomContainer& ac)
	{	
		if (!isValid_(ac))
		{
			calculate_(ac);
		}
	}

	void SurfaceBase::calculate_(AtomContainer& ac)
	{
		// first we must be shure that the PEOE charges are calculated
		TotalPositivePartialCharge tppc;
		ac.apply(tppc);
		
		MolecularWeight mw;
		ac.apply(mw);
		

		// assign van der Waals radii for the SAS calculator
		for (AtomIterator it = ac.beginAtom(); it != ac.endAtom(); ++it)
		{
		  it->setRadius(it->getElement().getVanDerWaalsRadius());
		}
		
		// calc the areas for each atom
		NumericalSAS sas;
		sas.options.setInteger(NumericalSAS::Option::NUMBER_OF_POINTS, 400);
		sas.options.setReal(NumericalSAS::Option::PROBE_RADIUS,         0.);

		sas(ac);

		HashMap<const Atom*, float> atom_areas = sas.getAtomAreas();

		double tot_pos(0), tot_neg(0), tot_pos_pol(0), tot_neg_pol(0), tot_hyd(0), tot_pol(0), tot(0);
		
		// add the atom areas
		HashMap<const Atom*, float>::ConstIterator a_it(atom_areas.begin()); 
		for (; a_it != atom_areas.end(); ++a_it)
		{
			float area = a_it->second;
			double charge = a_it->first->getProperty("PEOEPartialCharge").getDouble();
			// total positive polar van der Waals surface area
			if (charge > 0.2)	{	tot_pos_pol += area; } 
			// total negative polar van der Waals surface area
			if (charge < -0.2) { tot_neg_pol += area; 	}
			// total polar van der Waals surface area
			if (fabs(charge) > 0.2)	{	tot_pol += area; }
			// total hydrophobic van der Waals surface area
			if (fabs(charge) <= 0.2)	{	tot_hyd += area; }
			// total positive van der Waals surface area
			if (charge > 0)	{	tot_pos += area; }
			// total negative van der Waals surface area
			if (charge < 0)	{	tot_neg += area; }
			// total van der Waals surface area
			tot += area;
		}

		double vol = sas.getTotalVolume();
		double rho(0);
		if (vol != 0)
		{
			rho = ac.getProperty("MolecularWeight").getDouble()/vol;		
		}

		ac.setProperty("PositiveVdWSurface", tot_pos);
		ac.setProperty("NegativeVdWSurface", tot_neg);
		ac.setProperty("PositivePolarVdWSurface", tot_pos_pol);
		ac.setProperty("NegativePolarVdWSurface", tot_neg_pol);
		ac.setProperty("HydrophobicVdWSurface", tot_hyd);
		ac.setProperty("PolarVdWSurface", tot_pol);
		ac.setProperty("VdWSurface", tot);
		ac.setProperty("VdWVolume", vol);
		ac.setProperty("Density", rho);
	}

} // namespace BALL

