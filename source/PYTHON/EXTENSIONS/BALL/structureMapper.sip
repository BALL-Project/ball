class StructureMapper
	: TransformationProcessor
{
%TypeHeaderCode
	#include <BALL/PYTHON/EXTENSIONS/pyUnaryProcessors.h>
	#include <BALL/STRUCTURE/structureMapper.h>
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
};
