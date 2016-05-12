// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: electrostaticPotentialCalculator.C,v 1.4 2004/05/10 11:47:17 amoll Exp $

#include <BALL/SOLVATION/electrostaticPotentialCalculator.h>
#include <BALL/STRUCTURE/fragmentDB.h>

namespace BALL
{
	
	const char* ElectrostaticPotentialCalculator::Option::LOCALITY = "locality";
	
	const bool ElectrostaticPotentialCalculator::Default::LOCALITY = true;
	
	ElectrostaticPotentialCalculator::ElectrostaticPotentialCalculator()
		
		: mySys_(),
			frag_db_(0)
	{
		options.setDefaultBool(Option::LOCALITY, Default::LOCALITY);
	}

	ElectrostaticPotentialCalculator::ElectrostaticPotentialCalculator(const ElectrostaticPotentialCalculator& epc)
		
		: mySys_(epc.mySys_),
		  frag_db_(epc.frag_db_)
	{
		options.setDefaultBool(Option::LOCALITY, Default::LOCALITY);
		// TODO...
	}

	ElectrostaticPotentialCalculator::~ElectrostaticPotentialCalculator()
		
	{
	}

	void ElectrostaticPotentialCalculator::apply(System &S)
		throw(Exception::NullPointer)
	{
		if (frag_db_ == 0)
		{
			throw(Exception::NullPointer(__FILE__, __LINE__));
		}
		mySys_ = S;

		mySys_.apply(frag_db_->normalize_names);

		radii_.setFilename("PARSE.siz");
		charges_.setFilename("PARSE.crg");
		mySys_.apply(radii_);
		mySys_.apply(charges_);
	}

	float ElectrostaticPotentialCalculator::operator() (const Vector3& pos)
		
	{
		float phi_nonloc=0.;
		float length;
		static float eps=78.;
		
		if (options.getBool(Option::LOCALITY))
		{
			AtomIterator atIt;

			for (atIt=mySys_.beginAtom();+atIt;++atIt)
			{
				length = (pos-(*atIt).getPosition()).getLength();

				phi_nonloc += (*atIt).getCharge()/length;
			}
			phi_nonloc*=1./(4*M_PI*8.85e-22*eps)*1.6e-19;
		}
		else
		{
			static double lambda  = 20.;
			static double eps_inf = 1.8;

			static double xi     = sqrt(eps/eps_inf);
			static double xiol   = xi/lambda; 
			static double weight = (eps - eps_inf)/eps_inf;

			float length;

			AtomIterator atIt;

			for (atIt=mySys_.beginAtom();+atIt;++atIt)
			{
				length = (pos-(*atIt).getPosition()).getLength();

				phi_nonloc += (*atIt).getCharge()/length * (1.+weight*sinh((*atIt).getRadius() * xiol)/((*atIt).getRadius() * xiol)
						* exp(-xiol*length));
			}

			phi_nonloc*=1./(4*M_PI*8.85e-22*eps)*1.6e-19;
		}
		
		return phi_nonloc;
	}

	void ElectrostaticPotentialCalculator::setFragmentDB(const FragmentDB* db)
		
	{
		frag_db_ = (FragmentDB*) db;
	}

	const FragmentDB* ElectrostaticPotentialCalculator::getFragmentDB() const
		
	{
		return frag_db_;
	}

}	// namespace
