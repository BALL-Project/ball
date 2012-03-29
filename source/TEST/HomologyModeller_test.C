// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: Alignment_test.C, 2011/06/14$
//
// Author:
//   Nikola Koch
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>
///////////////////////////

#include <BALL/SEQUENCE/homologyModeller.h>
#include <BALL/SEQUENCE/sequence.h>
#include<BALL/SEQUENCE/alignment.h>
#include<BALL/SEQUENCE/alignAlgorithm.h>
#include <BALL/MATHS/angle.h>
#include <BALL/COMMON/constants.h>

#include <BALL/STRUCTURE/peptideBuilder.h>
#include <BALL/STRUCTURE/fragmentDB.h>
///////////////////////////


START_TEST(homologyModeller)

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

	Alignment al; 

CHECK(bool createModel(Alignment& alignment, Protein& templ);
)
	Sequence seq;
	

   // we need to define the peptid's AA sequence as a vector of descriptors
   vector<BALL::Peptides::AminoAcidDescriptor> descriptor_seq;

   // each aminoacid is represented by a descriptor
   BALL::Peptides::AminoAcidDescriptor* aad1 = new BALL::Peptides::AminoAcidDescriptor;

   // define a glycin
   aad1->setAminoAcidType("GLY");
   aad1->setPhi(Angle(-47., false));
   aad1->setPsi(Angle(-77., false));



   // add it to the sequence
   descriptor_seq.push_back(*aad1);

 		// each aminoacid is represented by a descriptor
   	BALL::Peptides::AminoAcidDescriptor* aad2 = new BALL::Peptides::AminoAcidDescriptor;

	 // define a glycin
   aad2->setAminoAcidType("GLY");
   aad2->setPhi(Angle(-47., false));
   aad2->setPsi(Angle(-77., false));

   // add it to the sequence
   descriptor_seq.push_back(*aad2);

   // create a builder using the sequence
   BALL::Peptides::PeptideBuilder* pb = new BALL::Peptides::PeptideBuilder(descriptor_seq);

   // "link" the fragment db for adding missing information
   FragmentDB fdb("");
  pb->setFragmentDB(&fdb);

   // now build the peptide 
   Protein* p1 = pb->construct();

   // give sensible names
   pb->setChainName("new_glycin");
   pb->setProteinName("new_protein");

   // check what is done
   ResidueIterator res_it = p1->beginResidue();

   
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	seq.setOrigin(p1);
	al.addSequence(seq);

  descriptor_seq.clear();

   // each aminoacid is represented by a descriptor
   BALL::Peptides::AminoAcidDescriptor* aad21 = new BALL::Peptides::AminoAcidDescriptor;

   // define a glycin
   aad21->setAminoAcidType("GLY");
   aad21->setPhi(Angle(-47., false));
   aad21->setPsi(Angle(-77., false));


   // add it to the sequence
   descriptor_seq.push_back(*aad21);

 		// each aminoacid is represented by a descriptor
   	BALL::Peptides::AminoAcidDescriptor* aad22 = new BALL::Peptides::AminoAcidDescriptor;

	 // define a glycin
   aad22->setAminoAcidType("ALA");
   aad22->setPhi(Angle(-47., false));
   aad22->setPsi(Angle(-77., false));

   // add it to the sequence
   descriptor_seq.push_back(*aad22);

   
   // now build the peptide 
   Protein* p2 = pb->construct();

   // give sensible names
   pb->setChainName("new_alanine");
   pb->setProteinName("new_protein2");



  Sequence seq2;
	
	seq2.setOrigin(p2);
	al.addSequence(seq2);

	TEST_EQUAL(al.getSequence(1) == seq2, true)

	SequenceCharacter *tmp2 = new SequenceCharacter('G', &seq2, SequenceCharacter::type::CHAR, p1->getResidue(1));
	

hmp = new HomologyModeller();

TEST_EQUAL(hmp->createModel(al, *p1), false)

AlignAlgorithm algo;

algo.align(al);

TEST_EQUAL(hmp->createModel(al,*p1), true);

RESULT


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
