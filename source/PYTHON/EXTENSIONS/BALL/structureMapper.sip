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
	StructureMapper(AtomContainer& /A/, AtomContainer& /B/);

	virtual ~StructureMapper();

	void set(AtomContainer& /A/, AtomContainer& /B/);
	double calculateRMSD();
	bool calculateTransformation();
	void calculateDefaultBijection();
	const AtomBijection& getBijection() const;
	AtomBijection calculateFragmentBijection(const vector<Fragment*>& /A/, const vector<Fragment*>& /B/);

	bool mapFragments (const vector<Fragment*>& /A/, const vector<Fragment*>& /B/, 
		 Matrix4x4* /transformation/, double /upper_bound/,	double /lower_bound/);

	static Matrix4x4 matchPoints
		(const Vector3& /w1/,	const Vector3& /w2/, const Vector3& /w3/,	
		 const Vector3& /v1/,	const Vector3& /v2/, const Vector3& /v3/);

	static Matrix4x4 matchBackboneAtoms(const Residue& /r1/, const Residue& /r2/);

	Size mapResiduesByBackbone(const list<Residue*>& /l1/, const list<Residue*>& /l2/);
 
//	vector<vector<Fragment*> >& searchPattern
//		(vector<Fragment*>& /pattern/, AtomContainer& /composite/,
//		 double /max_rmsd/,	   double /max_center_tolerance/,
//		 double /upper_bound/, double /lower_bound/);

//	Matrix4x4 mapProteins(Protein& /P1/, Protein& /P2/, std::map<String, Size>&	/type_map/,
//		 Size&	/no_matched_ca/, double& /rmsd/, double	/upper_bound/, double	/lower_bound/, double	/tolerance/);

};
