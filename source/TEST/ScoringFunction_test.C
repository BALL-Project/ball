// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

#include <BALL/SCORING/COMMON/gridBasedScoring.h>
#include <BALL/SCORING/FUNCTIONS/MMScoring.h>
#include <BALL/DOCKING/COMMON/structurePreparer.h>
#include <BALL/DOCKING/IMGDOCK/IMGDock.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/FORMAT/MOL2File.h>


using namespace std;
using namespace BALL;

///////////////////////////

START_TEST(ScoringFunction)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////


PRECISION(1E-3)

Log.remove(cout);

PDBFile pdb(BALL_TEST_DATA_PATH(1b5i_pocket.pdb));
MOL2File mol2(BALL_TEST_DATA_PATH(1b5i_ligand.mol2));
System pocket; pdb >> pocket;
System ligand; mol2 >> ligand;

StructurePreparer sp;
sp.prepare(&pocket,"Amber/amber96-docking.ini");
sp.prepare(&ligand,"Amber/amber96-docking.ini");


CHECK(MMScoring)
	Options options;
	MMScoring scoring_function(pocket,ligand,options);
	scoring_function.setLigand(ligand);
	scoring_function.update();
	scoring_function.updateScore();

	vector<double> score_contributions;
	vector<String> names;
	scoring_function.getScoreContributions(score_contributions, names);

	TEST_EQUAL(names.size(),5)
	TEST_EQUAL(score_contributions.size(),5)

	TEST_REAL_EQUAL(scoring_function.getScore(),-68.7552);
	TEST_REAL_EQUAL(score_contributions[0],-8.52705) // ES
	TEST_REAL_EQUAL(score_contributions[1],-19.7856) // vdW
	TEST_REAL_EQUAL(score_contributions[2],-20.5528) // frag. solvation
	TEST_REAL_EQUAL(score_contributions[3],-24.8898) // hydroggen bonds
	TEST_REAL_EQUAL(score_contributions[4],5) // nRot
RESULT


// Do a fast re-docking, just in order to check whether it works. Note, that the below settings are therefore _not_ useful for a normal docking.
Options options;
options.set("iterations",1);
options.set("no_solutions",20);
options.set("step_width",90);
options.set("scoring_type","GridedMM");
options.set("scoregrid_resolution",1.0);
set<String> types;
types.insert("C");types.insert("H");
types.insert("O");types.insert("N");
IMGDock docker(pocket,ligand,options);
GridBasedScoring* grid_scoring = dynamic_cast<GridBasedScoring*>(docker.getScoringFunction());
TEST_NOT_EQUAL(grid_scoring,0);
grid_scoring->setAtomTypeNames(types);


CHECK(Grid Precalculation)
	grid_scoring->precalculateGrids();
	grid_scoring->update();
	grid_scoring->updateScore(); // evaluate binding of reference ligand
//	grid_scoring->printResult();
	TEST_REAL_EQUAL(grid_scoring->getScore(),-57.424)
RESULT


CHECK(Scoregrid storing and loading)
	grid_scoring->saveGridSetsToFile("test.grd","1b5i_pocket");
	grid_scoring->replaceGridSetFromFile("test.grd");
	grid_scoring->update();
	grid_scoring->updateScore();
	TEST_REAL_EQUAL(grid_scoring->getScore(),-57.424)
RESULT

CHECK(Compressed scoregrid storing and loading)
	grid_scoring->saveGridSetsToFile("test.grd.gz","1b5i_pocket");
	grid_scoring->replaceGridSetFromFile("test.grd.gz");
	grid_scoring->update();
	grid_scoring->updateScore();
	TEST_REAL_EQUAL(grid_scoring->getScore(),-57.424)
RESULT

CHECK(Binary scoregrid storing and loading)
	grid_scoring->saveGridSetsToFile("test.bngrd","1b5i_pocket");
	grid_scoring->replaceGridSetFromFile("test.bngrd");
	grid_scoring->update();
	grid_scoring->updateScore();
	TEST_REAL_EQUAL(grid_scoring->getScore(),-57.424)
RESULT


CHECK(IMeedyDock)
	System ligand2 = ligand; // copy reference ligand for this simple test

	// move ligand2 out of pocket
	for(AtomIterator it=ligand2.beginAtom(); +it; it++)
	{
		it->setPosition(it->getPosition()+Vector3(20,20,20));
	}

	docker.dockLigand(ligand2);
	TEST_REAL_EQUAL(docker.getScoringFunction()->getScore(),-59.7147);
	MOL2File mol2res(BALL_TEST_DATA_PATH(1b5i_ligand-dockOut.mol2));
	System expected_dockres;
	mol2res >> expected_dockres;

	AtomConstIterator it1 = ligand2.beginAtom();
	AtomConstIterator it2 = expected_dockres.beginAtom();
	for(; +it1 && +it2; it1++,it2++)
	{
		Vector3 position1 = it1->getPosition();
		Vector3 position2 = it2->getPosition();
		TEST_REAL_EQUAL(position1[0],position2[0])
		TEST_REAL_EQUAL(position1[1],position2[1])
		TEST_REAL_EQUAL(position1[2],position2[2])
	}
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

END_TEST