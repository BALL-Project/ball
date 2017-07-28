class ResidueRotamerSet
{
%TypeHeaderCode
	#include <BALL/STRUCTURE/residueRotamerSet.h>
%End
	public:
	ResidueRotamerSet();
	ResidueRotamerSet(const ResidueRotamerSet& rotamer_set);
	ResidueRotamerSet(const Residue& residue, Size number_of_torsions);
	virtual ~ResidueRotamerSet();

	const String& getName() const;
	bool isValid() const;
	Size getNumberOfRotamers() const;
	Size getNumberOfTorsions() const;
	bool hasTorsionPhi() const;
	Angle getTorsionPhi() const;
	void setTorsionPhi(const Angle& phi);
	bool hasTorsionPsi() const;
	Angle getTorsionPsi() const;
	void setTorsionPsi(const Angle& psi);
	bool setTemplateResidue(const Residue& residue, Size number_of_torsions);
	bool setRotamer(Residue& residue, const Rotamer& rotamer);
	Rotamer getRotamer(const Residue& residue) const;
	const Rotamer& getRotamer(Position index) const;
	void setName(const String& name);
	void addRotamer(const Rotamer& rotamer);
	void sort();
};
