// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>
///////////////////////////

#include <BALL/SEQUENCE/homologyModeller.h>
#include <BALL/SEQUENCE/sequence.h>
#include <BALL/SEQUENCE/alignment.h>
#include <BALL/SEQUENCE/alignAlgorithm.h>

#include <BALL/MATHS/angle.h>
#include <BALL/COMMON/constants.h>

#include <BALL/FORMAT/PDBFile.h>

#include <BALL/STRUCTURE/geometricTransformations.h>
#include <BALL/STRUCTURE/geometricProperties.h>
#include <BALL/STRUCTURE/peptides.h>
#include <BALL/STRUCTURE/peptideBuilder.h>
#include <BALL/STRUCTURE/fragmentDB.h>
///////////////////////////


START_TEST(homologyModeller)
PRECISION(1e-5)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;


HomologyModeller* hmp;

CHECK(HomologyModeller())
	hmp= new HomologyModeller();
	TEST_NOT_EQUAL(hmp,0)
RESULT

CHECK(~HomologyModeller())
	delete hmp;
RESULT


CHECK(bool createModel(Alignment& alignment, Protein& query))

	// read pdb
	PDBFile f;
	f.open(BALL_TEST_DATA_PATH(PDBFile_test2.pdb));
	System S;
	f.read(S);
	TEST_EQUAL(S.countAtoms(), 892);
	f.close();

	// get Sequence 
  Protein* protein = RTTI::castTo<Protein>(*(S.getMolecule(0)));
  String olc_seq = Peptides::GetSequence(*protein);

	// convert to three-letter code
  std::list<String> tlc_seq = Peptides::OneLetterToThreeLetter(olc_seq);

	// create a peptide
  vector<BALL::Peptides::AminoAcidDescriptor> descriptor_seq;

	for (std::list<String>::iterator seq_it = tlc_seq.begin(); seq_it != tlc_seq.end(); seq_it++)
	{
		// add it to the sequence
		descriptor_seq.push_back(BALL::Peptides::AminoAcidDescriptor(*seq_it));
	}

  // create a builder using the sequence
  BALL::Peptides::PeptideBuilder* pb = new BALL::Peptides::PeptideBuilder(descriptor_seq);

  // "link" the fragment db for adding missing information
  FragmentDB fdb("");
  pb->setFragmentDB(&fdb);

  // now build the peptide 
  Protein* protein_copy = pb->construct();

	// create alignment	
	Alignment al;
	al.read(*protein_copy);
	al.read(*protein);
	AlignAlgorithm algo;
	algo.align(al);

	TEST_EQUAL(al.isAligned(), true);
	TEST_EQUAL(al.getScore(), 58);

	//std::cout << std::endl << Peptides::GetSequence(*protein) << std::endl;
	//std::cout << Peptides::GetSequence(*protein_copy) << std::endl;

	// move somewhere
	// compute the translation vector to the origin
	GeometricCenterProcessor center;
	protein_copy->apply(center);
	Vector3 toOrigin = center.getCenter().negate();

	// create the translation
	TranslationProcessor translation;
	translation.setTranslation(toOrigin);

	// translate the molecule
	protein_copy->apply(translation);

	// compute RMSD before
	GeometricCenterProcessor geo_center;
	protein_copy->apply(geo_center);
	TEST_EQUAL(geo_center.getCenter(), Vector3(7.96074e-06, 5.96418e-06, 1.1665e-05))
	S.apply(geo_center);
	TEST_EQUAL(geo_center.getCenter(), Vector3(12.481, 89.3941, 10.8307))

	// create the model
  HomologyModeller hmp;
	TEST_EQUAL(hmp.createSimpleModel(al, *protein_copy), true)


	// compute RMSD after
	protein_copy->apply(geo_center);
	TEST_EQUAL(geo_center.getCenter(), Vector3(12.4331, 88.7789, 10.7648))
	S.apply(geo_center);
	TEST_EQUAL(geo_center.getCenter(), Vector3(12.481, 89.3941, 10.8307))


RESULT


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
