// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/MOLMEC/COMMON/support.h>
#include <BALL/KERNEL/system.h>

#include <set>

///////////////////////////

namespace BALL
{
	template <>
	HashIndex Hash(const std::pair<Atom*, Atom*>& item)
	{
		return Hash(static_cast<LongSize>(reinterpret_cast<PointerSizeUInt>(item.first)) / 2
								+ static_cast<LongSize>(reinterpret_cast<PointerSizeUInt>(item.second)) / 2);
	}
}

START_TEST(class_name)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;


System S;
ForceField ff;

ff.options.setVector(PeriodicBoundary::Option::PERIODIC_BOX_LOWER, Vector3(-8.0));
ff.options.setVector(PeriodicBoundary::Option::PERIODIC_BOX_UPPER, Vector3(8.0));
ff.periodic_boundary.enable();
ff.setup(S);

CHECK(calculateNonBondedAtomPairs())
	ForceField::PairVector pair_vector;
	pair_vector.reserve(2000000);
			MolmecSupport::calculateNonBondedAtomPairs
				(pair_vector, ff.getAtoms(), ff.periodic_boundary.getBox(),
				 4.0, true, MolmecSupport::HASH_GRID);

	std::cout << pair_vector.size() << std::endl;
	HashSet<ForceField::PairVector::value_type> pair_set_hash_grid;
	std::copy(pair_vector.begin(), pair_vector.end(), std::inserter(pair_set_hash_grid, pair_set_hash_grid.begin()));
	pair_vector.clear();

	MolmecSupport::calculateNonBondedAtomPairs
		(pair_vector, ff.getAtoms(), ff.periodic_boundary.getBox(),
		 4.0, true, MolmecSupport::BRUTE_FORCE);

	STATUS("first atom handle: " << ff.getAtoms()[0]->getHandle())
	STATUS("last atom handle: " << ff.getAtoms()[ff.getAtoms().size() - 1]->getHandle())

	std::cout << pair_vector.size() << std::endl;
	HashSet<ForceField::PairVector::value_type> pair_set_brute_force;
	std::copy(pair_vector.begin(), pair_vector.end(), std::inserter(pair_set_brute_force, pair_set_brute_force.begin()));
	pair_vector.clear();

	STATUS("# of pairs in hash grid set: " << pair_set_hash_grid.size())
	STATUS("# of pairs in brute force set: " << pair_set_brute_force.size())
	
	pair_set_brute_force -= pair_set_hash_grid;
	
	HashSet<ForceField::PairVector::value_type> union_set = pair_set_hash_grid + pair_set_brute_force;
	STATUS("# of pairs in union: " << union_set.size())

	STATUS("# of missing pairs: " << pair_set_brute_force.size())

	HashSet<ForceField::PairVector::value_type>::ConstIterator it = pair_set_brute_force.begin();
	for (; +it; ++it)
	{
		STATUS("pair: " << it->first->getPosition() << " / " << it->second->getPosition())
		STATUS("missing pair: " << it->first->getHandle() << " - " << it->second->getHandle())
	}
	
	MolmecSupport::calculateNonBondedAtomPairs
		(pair_vector, ff.getAtoms(), ff.periodic_boundary.getBox(),
		 4.0, false, MolmecSupport::HASH_GRID);
	std::cout << pair_vector.size() << std::endl;

	pair_vector.clear();
	MolmecSupport::calculateNonBondedAtomPairs
		(pair_vector, ff.getAtoms(), ff.periodic_boundary.getBox(),
		 4.0, false, MolmecSupport::BRUTE_FORCE);
	std::cout << pair_vector.size() << std::endl;
	std::cout << S.countAtoms() << std::endl;
RESULT											

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
