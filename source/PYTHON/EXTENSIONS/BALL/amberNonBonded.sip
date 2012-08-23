// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class AmberNonBonded 
  : ForceFieldComponent
{
%TypeHeaderCode
	#include <BALL/MOLMEC/COMMON/support.h>
	#include <BALL/MOLMEC/AMBER/amberNonBonded.h>
	#include <BALL/COMMON/exception.h>
	using namespace BALL;
	using namespace BALL::Exception;
	using namespace BALL::MolmecSupport;
%End
  public:
  // BALL_CREATE(AmberNonBonded)
  AmberNonBonded();
  AmberNonBonded(ForceField&);
  AmberNonBonded(const AmberNonBonded&);
  ~AmberNonBonded();
	virtual void clear();
  virtual bool setup() throw(TooManyErrors);
  virtual double updateEnergy();
  virtual void updateForces();
  virtual void update() throw(TooManyErrors);
  virtual double getElectrostaticEnergy() const;
  virtual double getVdwEnergy() const;
  virtual PairListAlgorithmType determineMethodOfAtomPairGeneration();
//	virtual void buildVectorOfNonBondedAtomPairs
//		(const std::vector<std::pair<Atom*, Atom*> >& atom_vector,
//		 const LennardJones& lennard_jones,
//		 const Potential1210& hydrogen_bond)
//		throw(TooManyErrors);
};
  
