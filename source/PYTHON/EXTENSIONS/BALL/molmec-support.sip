%ModuleCode
#include <BALL/MOLMEC/COMMON/support.h>
using namespace BALL;
using namespace BALL::MolmecSupport;
%End

enum PairListAlgorithmType
{
			HASH_GRID,
			BRUTE_FORCE
};

void adaptWaterBox(System& system, const SimpleBox3& box);
void calculateMinimumImage (Vector3& distance, const Vector3& period);

//Size calculateNonBondedAtomPairs
//		(PairVector& pair_vector,
//		 const AtomVector& atom_vector, const SimpleBox3& box,
//		 double distance,	bool periodic_boundary_enabled,
//		 PairListAlgorithmType type)
//		throw(OutOfMemory);

//Size sortNonBondedAtomPairsAfterSelection(PairVector& pair_vector);
//Size addNonOverlappingMolecules
//			(System& system, const HashGrid3<const Atom*>& solute_grid,
//			 const System& solvent, const SimpleBox3& box, double distance);

//template <typename TorsionType, typename AtomIteratorType>
//Size computeTorsions
//	(const AtomIteratorType& start, const AtomIteratorType& end,
//	 std::vector<TorsionType>& torsions, bool use_selection = false);

//template <typename TorsionType, typename AtomIteratorType>
//Size computeTorsions
//	(const AtomIteratorType& start, const AtomIteratorType& end,
//	 std::vector<TorsionType>& torsions, bool use_selection)

