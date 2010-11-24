class DNAMutator
{
%TypeHeaderCode
	#include <BALL/STRUCTURE/DNAMutator.h>
	using namespace BALL;
%End
	public:
		enum MatchingMode { MINIMUM_ANGLE, MATCH_TORSION };

		DNAMutator(EnergyMinimizer* mini = NULL, ForceField* ff = NULL, FragmentDB* frag = NULL);

		~DNAMutator();

		virtual void setup();

		void setMinimizer(EnergyMinimizer* mini);

		void setForceField(ForceField* ff);

		void setMaxOptimizationSteps(Size steps);
		void setUsedProperty(unsigned int p);
		bool optimize();

		void setStrands(Chain* s1, Chain* s2);
		void setFirstStrand(Chain* s1);
		void setSecondStrand(Chain* s2);

		void setNucleotideMapping(NucleotideMapping& bij);

		void addMutation(Index i, const String& new_frag_name);
		void clearMutations();

		void setMatchingMode(MatchingMode mmode);
};
