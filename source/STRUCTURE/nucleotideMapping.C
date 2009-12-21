#include <BALL/STRUCTURE/nucleotideMapping.h>

#include <BALL/COMMON/exception.h>
#include <BALL/KERNEL/residue.h>
#include <BALL/KERNEL/protein.h>

namespace BALL
{
	NucleotideMapping::NucleotideMapping()
		: a_(0), b_(0)
	{
	}

	NucleotideMapping::NucleotideMapping(Chain& a, Chain& b, const NucleotideMap& a_to_b, const NucleotideMap& b_to_a)
		: a_(&a), b_(&b), a_to_b_(a_to_b), b_to_a_(b_to_a)
	{
	}

	Chain* NucleotideMapping::getFirstStrand() const
	{
		return a_;
	}

	Chain* NucleotideMapping::getSecondStrand() const
	{
		return b_;
	}

	Residue* NucleotideMapping::firstToSecond(Residue* a) const
	{
		NucleotideMap::const_iterator it = a_to_b_.find(a);

		if (it == a_to_b_.end())
		{
			return 0;
		}

		return it->second;
	}

	Residue* NucleotideMapping::secondToFirst(Residue* b) const
	{
		NucleotideMap::const_iterator it = b_to_a_.find(b);

		if (it == b_to_a_.end())
		{
			return 0;
		}

		return it->second;
	}

	NucleotideMapping NucleotideMapping::assignNaively(Chain& a, Chain& b, unsigned int offset_a, unsigned int offset_b)
	{
		ResidueIterator at = a.beginResidue();
		for(unsigned int i = 0; i < offset_a; ++i, ++at) {
			if(at == a.endResidue()) {
				return NucleotideMapping();
			}
		}

		ResidueIterator bt = b.beginResidue();
		bt.toRBegin();
		ResidueIterator bREnd = b.beginResidue();
		bREnd.toREnd();
		for(unsigned int i = 0; i < offset_b; ++i, --bt) {
			if(bt == bREnd) {
				return NucleotideMapping();
			}
		}

		NucleotideMap a_to_b;
		NucleotideMap b_to_a;

		for (; (at != a.endResidue()) && (bt != bREnd); ++at, --bt) {
			a_to_b.insert(NucleotideMap::value_type(&*at, &*bt));
			b_to_a.insert(NucleotideMap::value_type(&*bt, &*at));
		}

		return NucleotideMapping(a, b, a_to_b, b_to_a);
	}

	NucleotideMapping NucleotideMapping::assignFromDistances(Chain& a, Chain& b)
	{
		NucleotideMap a_to_b;
		NucleotideMap b_to_a;

		throw Exception::NotImplemented(__FILE__, __LINE__);

		return NucleotideMapping(a, b, a_to_b, b_to_a);
	}

	NucleotideMapping NucleotideMapping::assignFromAlignment(Chain& a, Chain& b, const Alignment& alignment)
	{
		NucleotideMap a_to_b;
		NucleotideMap b_to_a;

		throw Exception::NotImplemented(__FILE__, __LINE__);

		return NucleotideMapping(a, b, a_to_b, b_to_a);
	}

}

