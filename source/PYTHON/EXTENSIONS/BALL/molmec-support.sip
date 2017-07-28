%ModuleCode
	#include <BALL/MOLMEC/COMMON/support.h>
	using namespace BALL::MolmecSupport;
%End

enum PairListAlgorithmType
{
	HASH_GRID,
	BRUTE_FORCE
};

void adaptWaterBox(System& system, const SimpleBox3& box);
void calculateMinimumImage (Vector3& distance, const Vector3& period);
