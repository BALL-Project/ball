// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: partialChargeDescriptors.C,v 1.1.28.1 2007/03/16 00:06:47 bertsch Exp $
// 

#include <BALL/QSAR/partialChargeDescriptors.h>
#include <BALL/KERNEL/fragment.h>

using namespace std;

namespace BALL
{

	////////////////////////////////
	// Sum of all positive charges
	////////////////////////////////
	TotalPositivePartialCharge::TotalPositivePartialCharge()
		:	PartialChargeBase("TotalPositivePartialCharge", "eV")
	{
	}

	TotalPositivePartialCharge::TotalPositivePartialCharge(const TotalPositivePartialCharge& tppc)
		:	PartialChargeBase(tppc)
	{
	}

	TotalPositivePartialCharge::~TotalPositivePartialCharge()
	{
	}

	TotalPositivePartialCharge& TotalPositivePartialCharge::operator = (const TotalPositivePartialCharge& tppc)
	{
		this->setName(tppc.getName());
		this->setUnit(tppc.getUnit());
		return *this;
	}
	
	///////////////////////////////
	// Sum of all negative charges
	///////////////////////////////
	TotalNegativePartialCharge::TotalNegativePartialCharge()
		: PartialChargeBase("TotalNegativePartialCharge", "eV")
	{
	}

	TotalNegativePartialCharge::TotalNegativePartialCharge(const TotalNegativePartialCharge& tnpc)
		:	PartialChargeBase(tnpc)
	{
	}

	TotalNegativePartialCharge::~TotalNegativePartialCharge()
	{
	}

	TotalNegativePartialCharge& TotalNegativePartialCharge::operator = (const TotalNegativePartialCharge& tnpc)
	{
		this->setName(tnpc.getName());
		this->setUnit(tnpc.getUnit());
		return *this;
	}

	
	/////////////////////////////////////////////////////////
	// smallest charge devided by the sum of all negatives
	/////////////////////////////////////////////////////////
	RelPositivePartialCharge::RelPositivePartialCharge()
		:	PartialChargeBase("RelPositivePartialCharge", "")
	{
	}

	RelPositivePartialCharge::RelPositivePartialCharge(const RelPositivePartialCharge& rppc)
		:	PartialChargeBase(rppc)
	{
	}

	RelPositivePartialCharge::~RelPositivePartialCharge()
	{
	}

	RelPositivePartialCharge& RelPositivePartialCharge::operator = (const RelPositivePartialCharge& rppc)
	{
		this->setName(rppc.getName());
		this->setUnit(rppc.getUnit());
		return *this;
	}


	/////////////////////////////////////////////////////
	// biggest charge devided by the sum of all charges
	/////////////////////////////////////////////////////
	RelNegativePartialCharge::RelNegativePartialCharge()
		:	PartialChargeBase("RelNegativePartialCharge", "")
	{
	}

	RelNegativePartialCharge::RelNegativePartialCharge(const RelNegativePartialCharge& rnpc)
		:	PartialChargeBase(rnpc)
	{
	}

	RelNegativePartialCharge::~RelNegativePartialCharge()
	{
	}

	RelNegativePartialCharge& RelNegativePartialCharge::operator = (const RelNegativePartialCharge& rnpc)
	{
		this->setName(rnpc.getName());
		this->setUnit(rnpc.getUnit());
		return *this;
	}

} // namespace BALL
