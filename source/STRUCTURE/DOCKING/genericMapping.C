// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: genericMapping.C,v 1.1.2.1 2006/05/19 14:18:24 leonhardt Exp $

#include <vector>
#include <iostream>

#include "genericMapping.h"

using namespace std;


namespace BALL
{
	GenericMapping::GenericMapping()
	{
	}
	
	GenericMapping::~GenericMapping() 
	{
	}
	
	GenericMapping::GenericMapping(const GenericMapping& gm)
	{
		gp_ = gm.gp_;
	}
  
	GenericMapping& GenericMapping::operator=(const GenericMapping& gm)
	{
		if (this != &gm)
			return *this;
		
		return *this;
	}
	
	
	void GenericMapping::setPool(GeneticPool* gp)
	{
		gp_ = gp;
	}
}
