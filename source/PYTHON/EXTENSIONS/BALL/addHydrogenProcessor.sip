class AddHydrogenProcessor : CompositeProcessor
{
%TypeHeaderCode
	#include <BALL/STRUCTURE/addHydrogenProcessor.h>
	using namespace BALL;
%End

	public:
		AddHydrogenProcessor();
		virtual ~AddHydrogenProcessor();
		virtual bool start();
		virtual Processor::Result operator() (Composite &composite);
		Size getConnectivity(const Atom& atom) const;
		// For this the RingPerceptionProcessor must be wrapped
		//void setRings(const vector<vector<Atom*> >& rings);
		Size getNumberOfAddedHydrogens() const;

	protected:
		Processor::Result placePeptideBondH_(Residue* res);

		Size countBondOrders(const Atom& atom) const;
		void addHydrogen_(Atom& atom, Vector3 position);
		bool isRingAtom_(const Atom& atom) const;
		vector<Atom*> getPartners_(Atom& atom) const;
		Vector3 getNormal_(const Vector3& v) const;
		bool normalize_(Vector3& v) const;
		bool hasMultipleBond_(const Atom& atom) const;
		float getBondLength_(Position element) const;
};
