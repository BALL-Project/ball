// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: surfaceDescriptors.C,v 1.1.28.1 2007/03/16 00:06:50 bertsch Exp $
// 

#include <BALL/QSAR/surfaceDescriptors.h>

#include <BALL/KERNEL/fragment.h>

namespace BALL 
{
	//////////////////////////
	// positive vdw surface
	//////////////////////////
	PositiveVdWSurface::PositiveVdWSurface()
		: SurfaceBase("PositiveVdWSurface", "A^2")
	{
	}

	PositiveVdWSurface::PositiveVdWSurface(const PositiveVdWSurface& ps)
		:	SurfaceBase(ps)
	{
	}

	PositiveVdWSurface::~PositiveVdWSurface()
	{
	}
	
	PositiveVdWSurface& PositiveVdWSurface::operator = (const PositiveVdWSurface& ps)
	{
		this->setName(ps.getName());
		this->setUnit(ps.getUnit());
		return *this;
	}

	///////////////////////////
	// negative vdw surface
	///////////////////////////
	NegativeVdWSurface::NegativeVdWSurface()
		:	SurfaceBase("NegativeVdWSurface", "A^2")
	{
	}

	NegativeVdWSurface::NegativeVdWSurface(const NegativeVdWSurface& ns)
		:	SurfaceBase(ns)
	{
	}

	NegativeVdWSurface::~NegativeVdWSurface()
	{
	}

	NegativeVdWSurface& NegativeVdWSurface::operator = (const NegativeVdWSurface& ns)
	{
		this->setName(ns.getName());
		this->setUnit(ns.getUnit());
		return *this;
	}

	///////////////////////////////
	// positive polar vdw surface
	///////////////////////////////
	PositivePolarVdWSurface::PositivePolarVdWSurface()
		:	SurfaceBase("PositivePolarVdWSurface", "A^2")
	{
	}

	PositivePolarVdWSurface::PositivePolarVdWSurface(const PositivePolarVdWSurface& pps)
		:	SurfaceBase(pps)
	{
	}

	PositivePolarVdWSurface::~PositivePolarVdWSurface()
	{
	}

	PositivePolarVdWSurface& PositivePolarVdWSurface::operator = (const PositivePolarVdWSurface& pps)
	{
		this->setName(pps.getName());
		this->setUnit(pps.getUnit());
		return *this;
	}

	///////////////////////////////
	// negative polar vdw surface
	//////////////////////////////
	NegativePolarVdWSurface::NegativePolarVdWSurface()
		:	SurfaceBase("NegativePolarVdWSurface", "A^2")
	{
	}

	NegativePolarVdWSurface::NegativePolarVdWSurface(const NegativePolarVdWSurface& nps)
		:	SurfaceBase(nps)
	{
	}

	NegativePolarVdWSurface::~NegativePolarVdWSurface()
	{
	}

	NegativePolarVdWSurface& NegativePolarVdWSurface::operator = (const NegativePolarVdWSurface& nps)
	{
		this->setName(nps.getName());
		this->setUnit(nps.getUnit());
		return *this;
	}

	/////////////////////////////////
	// hydrophobic vdw surface
	/////////////////////////////////
	HydrophobicVdWSurface::HydrophobicVdWSurface()
		:	SurfaceBase("HydrophobicVdWSurface", "A^2")
	{
	}

	HydrophobicVdWSurface::HydrophobicVdWSurface(const HydrophobicVdWSurface& hs)
		:	SurfaceBase(hs)
	{
	}

	HydrophobicVdWSurface::~HydrophobicVdWSurface()
	{
	}
	
	HydrophobicVdWSurface& HydrophobicVdWSurface::operator = (const HydrophobicVdWSurface& hs)
	{
		this->setName(hs.getName());
		this->setUnit(hs.getUnit());
		return *this;
	}

	////////////////////////
	// polar vdw surface
	////////////////////////
	PolarVdWSurface::PolarVdWSurface()
		:	SurfaceBase("PolarVdWSurface", "A^2")
	{
	}
	
	PolarVdWSurface::PolarVdWSurface(const PolarVdWSurface& ps)
		:	SurfaceBase(ps)
	{
	}
	
	PolarVdWSurface::~PolarVdWSurface()
	{
	}
	
	PolarVdWSurface& PolarVdWSurface::operator = (const PolarVdWSurface& ps)
	{
		this->setName(ps.getName());
		this->setUnit(ps.getUnit());
		return *this;
	}

	/////////////////////////////////
	// relative positive vdw surface
	/////////////////////////////////
	RelPositiveVdWSurface::RelPositiveVdWSurface()
		: SurfaceBase("RelPositiveVdWSurface", "")
	{
	}

	RelPositiveVdWSurface::RelPositiveVdWSurface(const RelPositiveVdWSurface& rps)
		:	SurfaceBase(rps)
	{
	}

	RelPositiveVdWSurface::~RelPositiveVdWSurface()
	{
	}

	RelPositiveVdWSurface& RelPositiveVdWSurface::operator = (const RelPositiveVdWSurface& rps)
	{
		this->setName(rps.getName());
		this->setUnit(rps.getUnit());
		return *this;
	}

	double RelPositiveVdWSurface::compute(AtomContainer& ac)
	{
		if (!isValid_(ac))
		{
			calculate_(ac);
		}
		return ac.getProperty("PositiveVdWSurface").getDouble()/ac.getProperty("VdWSurface").getDouble();
	}


	///////////////////////////////////
	// relative negative vdw surface
	///////////////////////////////////
	RelNegativeVdWSurface::RelNegativeVdWSurface()
		:	SurfaceBase("RelNegativeVdWSurface", "")
	{
	}

	RelNegativeVdWSurface::RelNegativeVdWSurface(const RelNegativeVdWSurface& rns)
		:	SurfaceBase(rns)
	{
	}

	RelNegativeVdWSurface::~RelNegativeVdWSurface()
	{
	}

	RelNegativeVdWSurface& RelNegativeVdWSurface::operator = (const RelNegativeVdWSurface& rns)
	{
		this->setName(rns.getName());
		this->setUnit(rns.getUnit());
		return *this;
	}

	double RelNegativeVdWSurface::compute(AtomContainer& ac)
	{
		if (!isValid_(ac))
		{
			calculate_(ac);
		}
		if (ac.getProperty("VdWSurface").getDouble() != 0)
		{
			return ac.getProperty("NegativeVdWSurface").getDouble()/ac.getProperty("VdWSurface").getDouble();
		}
		else
		{
			return -1;
		}
	}


	////////////////////////////////////////
	// relative positive polar vdw surface
	////////////////////////////////////////
	RelPositivePolarVdWSurface::RelPositivePolarVdWSurface()
		:	SurfaceBase("RelPositivePolarVdWSurface", "")
	{
	}

	RelPositivePolarVdWSurface::RelPositivePolarVdWSurface(const RelPositivePolarVdWSurface& rpps)
		:	SurfaceBase(rpps)
	{
	}

	RelPositivePolarVdWSurface::~RelPositivePolarVdWSurface()
	{
	}
	
	RelPositivePolarVdWSurface& RelPositivePolarVdWSurface::operator = (const RelPositivePolarVdWSurface& rpps)
	{
		this->setName(rpps.getName());
		this->setUnit(rpps.getUnit());
		return *this;
	}
	
	double RelPositivePolarVdWSurface::compute(AtomContainer& ac)
	{
		if (!isValid_(ac))
		{
			calculate_(ac);
		}
		if (ac.getProperty("VdWSurface").getDouble() != 0)
		{
			return ac.getProperty("PositivePolarVdWSurface").getDouble()/ac.getProperty("VdWSurface").getDouble();
		}
		else
		{
			return -1;
		}
	}


	//////////////////////////////////////////
	// relative negative polar vdw surface
	//////////////////////////////////////////
	RelNegativePolarVdWSurface::RelNegativePolarVdWSurface()
		:	SurfaceBase("RelNegativePolarVdWSurface", "")
	{
	}

	RelNegativePolarVdWSurface::RelNegativePolarVdWSurface(const RelNegativePolarVdWSurface& rnps)
		:	SurfaceBase(rnps)
	{
	}

	RelNegativePolarVdWSurface::~RelNegativePolarVdWSurface()
	{
	}

	RelNegativePolarVdWSurface& RelNegativePolarVdWSurface::operator = (const RelNegativePolarVdWSurface& rnps)
	{
		this->setName(rnps.getName());
		this->setUnit(rnps.getUnit());
		return *this;
	}

	double RelNegativePolarVdWSurface::compute(AtomContainer& ac)
	{
		if (!isValid_(ac))
		{
			calculate_(ac);
		}
		if (ac.getProperty("VdWSurface").getDouble() != 0)
		{
			return ac.getProperty("NegativePolarVdWSurface").getDouble()/ac.getProperty("VdWSurface").getDouble();
		}
		else
		{
			return -1;
		}
	}


	/////////////////////////////////////////////
	// relative hydrophobic vdw surface
	/////////////////////////////////////////////
	RelHydrophobicVdWSurface::RelHydrophobicVdWSurface()
		:	SurfaceBase("RelHydrophobicVdWSurface", "")
	{
	}

	RelHydrophobicVdWSurface::RelHydrophobicVdWSurface(const RelHydrophobicVdWSurface& rhs)
		:	SurfaceBase(rhs)
	{
	}

	RelHydrophobicVdWSurface::~RelHydrophobicVdWSurface()
	{
	}
	
	RelHydrophobicVdWSurface& RelHydrophobicVdWSurface::operator = (const RelHydrophobicVdWSurface& rhs)
	{
		this->setName(rhs.getName());
		this->setUnit(rhs.getUnit());
		return *this;
	}
	
	double RelHydrophobicVdWSurface::compute(AtomContainer& ac)
	{
		if (!isValid_(ac))
		{
			calculate_(ac);
		}
		if (ac.getProperty("VdWSurface").getDouble() != 0)
		{
			return ac.getProperty("HydrophobicVdWSurface").getDouble()/ac.getProperty("VdWSurface").getDouble();
		}
		else
		{
			return -1;
		}
	}
	

	////////////////////////////////
	// relative polar vdw surface
	////////////////////////////////
	RelPolarVdWSurface::RelPolarVdWSurface()
		:	SurfaceBase("RelPolarVdWSurface", "")
	{
	}
	
	RelPolarVdWSurface::RelPolarVdWSurface(const RelPolarVdWSurface& rps)
		:	SurfaceBase(rps)
	{
	}
	
	RelPolarVdWSurface::~RelPolarVdWSurface()
	{
	}
	
	RelPolarVdWSurface& RelPolarVdWSurface::operator = (const RelPolarVdWSurface& rps)
	{
		this->setName(rps.getName());
		this->setUnit(rps.getUnit());
		return *this;
	}
	
	double RelPolarVdWSurface::compute(AtomContainer& ac)
	{
		if (!isValid_(ac))
		{
			calculate_(ac);
		}
		if (ac.getProperty("VdWSurface").getDouble() != 0)
		{
			return ac.getProperty("PolarVdWSurface").getDouble()/ac.getProperty("VdWSurface").getDouble();
		}
		else
		{
			return -1;
		}
	}	
	

	////////////////
	// vdw volume
	////////////////
	VdWVolume::VdWVolume()
		:	SurfaceBase("VdWVolume", "A^3")
	{
	}

	VdWVolume::VdWVolume(const VdWVolume& vol)
		:	SurfaceBase(vol)
	{
	}
	
	VdWVolume::~VdWVolume()
	{
	}
	
	VdWVolume& VdWVolume::operator = (const VdWVolume& vol)
	{
		this->setName(vol.getName());
		this->setUnit(vol.getUnit());
		return *this;
	}
	
	//////////////////
	// density
	//////////////////
	Density::Density()
		:	SurfaceBase("Density", "u/A^3")
	{
	}

	Density::Density(const Density& rho)
		:	SurfaceBase(rho)
	{
	}

	Density::~Density()
	{
	}
	
	Density& Density::operator = (const Density& rho)
	{
		this->setName(rho.getName());
		this->setUnit(rho.getUnit());
		return *this;
	}
	

	///////////////////////
	// vdw surface
	///////////////////////
	VdWSurface::VdWSurface()
		:	SurfaceBase("VdWSurface", "A^2")
	{
	}

	VdWSurface::VdWSurface(const VdWSurface& surf)
		:	SurfaceBase(surf)
	{
	}

	VdWSurface::~VdWSurface()
	{
	}

	VdWSurface& VdWSurface::operator = (const VdWSurface& surf)
	{
		this->setName(surf.getName());
		this->setUnit(surf.getUnit());
		return *this;
	}

} //namespace BALL
