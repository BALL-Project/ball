// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: genericEvolutionaryAlgorithm.C,v 1.1.2.1 2006/05/19 14:18:22 leonhardt Exp $

#include "genericEvolutionaryAlgorithm.h"

namespace BALL
{
	GenericEvolutionaryAlgorithm::GenericEvolutionaryAlgorithm() 
	{
	}
	
	
	GenericEvolutionaryAlgorithm::~GenericEvolutionaryAlgorithm()
	{
	}
	
	
	
	// GenericEvolutionaryAlgorithm::GenericEvolutionaryAlgorithm(const GenericEvolutionaryAlgorithm& gea)
	// {
	// } 
	
	
	
	GenericEvolutionaryAlgorithm& GenericEvolutionaryAlgorithm::operator=(const GenericEvolutionaryAlgorithm& gea)
	{
		if (this == &gea)
			return *this;
		
		return *this;
	}
}
