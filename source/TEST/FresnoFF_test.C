// $Id: FresnoFF_test.C,v 1.1.2.2 2003/02/10 17:22:42 anker Exp $

#include <BALL/CONCEPT/classTest.h>

#include <BALL/MOLMEC/FRESNO/fresno.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/FORMAT/HINFile.h>
#include <BALL/STRUCTURE/fragmentDB.h>
#include <BALL/MOLMEC/COMMON/radiusRuleProcessor.h>
#include <BALL/SYSTEM/path.h>
#include <BALL/DATATYPE/regularData1D.h>

START_TEST(FresnoFF, "$Id: FresnoFF_test.C,v 1.1.2.2 2003/02/10 17:22:42 anker Exp $")

using namespace BALL;

FragmentDB db;

FresnoFF* fresno;
CHECK(FresnoFF())
	fresno = new FresnoFF();
	TEST_NOT_EQUAL(fresno, 0)
RESULT

CHECK(~FresnoFF())
	delete fresno;
RESULT

CHECK(FresnoFF(const FresnoFF& force_field, bool clone_deep = true))
	FresnoFF f1;
	FresnoFF f2(f1);
RESULT
	
System system;

PDBFile protein_file("data/FresnoFF_test_protein.pdb");
protein_file >> system;
protein_file.close();
Molecule* protein = system.getMolecule(0);

PDBFile ligand_file("data/FresnoFF_test_ligand.pdb");
ligand_file >> system;
ligand_file.close();
Molecule* ligand = system.getMolecule(1);

system.apply(db.build_bonds);
system.apply(db.normalize_names);

Path path;
String tmp = path.find("solvation/PARSE.rul");
INIFile radius_ini(tmp);
radius_ini.read();
RadiusRuleProcessor radii(radius_ini);
system.apply(radii);
	
FresnoFF ff;

CHECK("setup")
	ff.setProtein(protein);
	ff.setLigand(ligand);
	ff.setup(system);
RESULT

CHECK("type assignment")
	HashMap<const Atom*, short> fresno_types = ff.getFresnoTypes();
	TRegularData1D<short> reference;
	File reference_file("data/FresnoFF_test_type_reference.dat");
	reference_file >> reference;
	TEST_EQUAL(reference.getSize(), ligand->countAtoms())

	short this_type;

	AtomIterator atom_it = ligand->beginAtom();
	for (Size index = 0; +atom_it; ++atom_it, ++index)
	{
		this_type = fresno_types[&*atom_it];
		TEST_EQUAL(this_type, reference[index])
	}

RESULT

// CHECK("energy test")
// RESULT

END_TEST
