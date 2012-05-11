class NucleotideMapping
{
%TypeHeaderCode
	#include <BALL/STRUCTURE/nucleotideMapping.h>
	using namespace BALL;
	typedef unsigned int uint;
%End
	public:
		typedef std::pair<const std::string, const std::string> Alignment;

		NucleotideMapping();

		static NucleotideMapping assignNaively(Chain& a, Chain& b, unsigned int offset_a = 0, unsigned int offset_b = 0);
		static NucleotideMapping assignFromDistances(Chain& a, Chain& b);
//		static NucleotideMapping assignFromAlignment(Chain& a, Chain& b, const Alignment& alignment);

		Chain* getFirstStrand() const;
		Chain* getSecondStrand() const;

		Residue* firstToSecond(Residue* a) const;
		Residue* secondToFirst(Residue* b) const;
};
