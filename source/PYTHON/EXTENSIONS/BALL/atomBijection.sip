class AtomBijection
{
%TypeHeaderCode
	#include <BALL/STRUCTURE/atomBijection.h>
	using namespace std;
	using namespace BALL;
%End
	public:
		typedef std::pair<Atom*, Atom*> AtomPair;
		typedef std::vector<std::pair<Atom*, Atom*> > PairVector;

		AtomBijection();
		AtomBijection(AtomContainer& A, AtomContainer& B);

		Size assignTrivial(AtomContainer& A, AtomContainer& B);
		Size assignByName(AtomContainer& A, AtomContainer& B);
		Size assignCAlphaAtoms(AtomContainer& A, AtomContainer& B);
		Size assignBackboneAtoms(AtomContainer& A, AtomContainer& B);
};
