// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

class StructureMapper
	:	TransformationProcessor
{
%TypeHeaderCode
	#include <BALL/STRUCTURE/structureMapper.h>
	using namespace BALL;
%End
	public:

	StructureMapper();
	StructureMapper(AtomContainer&, AtomContainer&);

	virtual ~StructureMapper();

	void set(AtomContainer&, AtomContainer&);
	double calculateRMSD();
	bool calculateTransformation();
	void calculateDefaultBijection();
	const AtomBijection& getBijection() const;
	AtomBijection calculateFragmentBijection(const vector<Fragment*>&, const vector<Fragment*>&);

	bool mapFragments (const vector<Fragment*>&, const vector<Fragment*>&, 
		 Matrix4x4*, double,	double);

	static Matrix4x4 matchPoints
		(const Vector3&,	const Vector3&, const Vector3&,	
		 const Vector3&,	const Vector3&, const Vector3&);

	static Matrix4x4 matchBackboneAtoms(const Residue&, const Residue&);

	Size mapResiduesByBackbone(const list<Residue*>&, const list<Residue*>&);
 
//	vector<vector<Fragment*> >& searchPattern
//		(vector<Fragment*>&, AtomContainer&,
//		 double,	   double,
//		 double, double);

//	Matrix4x4 mapProteins(Protein& /P1/, Protein& /P2/, std::map<String, Size>&,
//		 Size&, double&, double, double, double);

};
