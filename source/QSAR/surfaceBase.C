// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: surfaceBase.C,v 1.1 2004/05/11 07:28:22 oliver Exp $
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

  bool SurfaceBase::isValid(Molecule& molecule)
  {
		static HashMap<Handle, PreciseTime> mod_times;
		PreciseTime last_mod = molecule.getModificationTime();
		Handle mol_handle = molecule.getHandle();
		if (mod_times.has(mol_handle))
		{
			if (mod_times[mol_handle] == last_mod)
			{
				#ifdef BALL_QSAR_SURFACEBASE_DEBUG
				cerr << ">> SurfaceBase::isValid: molcule valid!" << endl;
				#endif
				return true;
			}
			else
			{
				mod_times[mol_handle] = last_mod;
				#ifdef BALL_QSAR_SURFACEBASE_DEBUG
				cerr << ">> SurfaceBase::isValid: molecule not valid, modified!" << endl;
				#endif
				return false;
			}
		}
		else
		{
			mod_times.insert(make_pair(mol_handle, last_mod));
			#ifdef BALL_QSAR_SURFACEBASE_DEBUG
			cerr << ">> SurfaceBase::isValid: molecule not valid, first call!" << endl;
			#endif
			return false;
		}
	}


	void SurfaceBase::calculate(Molecule& molecule)
	{
		// first we must be shure that the PEOE charges are calculated
		TotalPositivePartialCharge tppc;
		molecule.apply(tppc);
		
		MolecularWeight mw;
		molecule.apply(mw);
		

		// assign van der Waals radii for the SAS calculator
		for (AtomIterator it=molecule.beginAtom();it!=molecule.endAtom(); ++it)
		{
		  it->setRadius(it->getElement().getVanDerWaalsRadius());
		}
		
		// calc the areas for each atom
		HashMap<const Atom*, float> atom_areas;
		calculateSASAtomAreas(molecule, atom_areas, 0, 400);
		double tot_pos(0), tot_neg(0), tot_pos_pol(0), tot_neg_pol(0), tot_hyd(0), tot_pol(0), tot(0);
		
		// add the atom areas
		HashMap<const Atom*, float>::ConstIterator a_it(atom_areas.begin()); 
		for (;a_it!=atom_areas.end();++a_it)
		{
			float area = a_it->second;
			double charge = a_it->first->getProperty("PEOEPartialCharge").getDouble();
			// total positive polar van der Waals surface area
			if (charge > 0.2)	{	tot_pos_pol += area; } 
			// total negative polar van der Waals surface area
			if (charge < -0.2) { tot_neg_pol += area;	}
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

		double vol = calculateSASVolume(molecule, 0, 400);
		double rho(0);
		if (vol != 0)
		{
			rho = molecule.getProperty("MolecularWeight").getDouble()/vol;		
		}

		molecule.setProperty("PositiveVdWSurface", tot_pos);
		molecule.setProperty("NegativeVdWSurface", tot_neg);
		molecule.setProperty("PositivePolarVdWSurface", tot_pos_pol);
		molecule.setProperty("NegativePolarVdWSurface", tot_neg_pol);
		molecule.setProperty("HydrophobicVdWSurface", tot_hyd);
		molecule.setProperty("PolarVdWSurface", tot_pol);
		molecule.setProperty("VdWSurface", tot);
		molecule.setProperty("VdWVolume", vol);
		molecule.setProperty("Density", rho);
	}

} // namespace BALL

