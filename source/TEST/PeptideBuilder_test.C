// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: PeptideBuilder_test.C,v 1.6 2003/06/12 18:03:18 oliver Exp $

#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/STRUCTURE/peptideBuilder.h>
#include <vector>

///////////////////////////

START_TEST(PeptideBuilder, "$Id: PeptideBuilder_test.C,v 1.6 2003/06/12 18:03:18 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
using namespace BALL::Peptides;

PeptideBuilder* pbptr = 0;
CHECK(PeptideBuilder())
	pbptr = new PeptideBuilder;
	TEST_NOT_EQUAL(pbptr, 0)
RESULT								

CHECK(~PeptideBuilder())
	delete pbptr;
	pbptr = 0;
RESULT

AminoAcidDescriptor* aad = 0;
CHECK(AminoAcidDescriptor())
	aad = new AminoAcidDescriptor;
	TEST_NOT_EQUAL(aad,0)
RESULT

CHECK(~AminoAcidDescriptor())
	delete aad;
  aad= 0; 
RESULT

CHECK(AminoAcidDescriptor(const String& type, const Angle& phi,
													const Angle& psi, const Angle& omega))
	aad= new AminoAcidDescriptor("HIS", Angle(M_PI, true), Angle(M_PI, true));
	TEST_NOT_EQUAL(aad, 0)
	TEST_EQUAL(aad->getType(), "HIS")
	TEST_EQUAL(aad->getPhi(), Angle(M_PI, true))
	TEST_EQUAL(aad->getPsi(), Angle(M_PI, true))
RESULT

AminoAcidDescriptor* aad2=0;
CHECK(AminoAcidDescriptor(const AminoAcidDescriptor& aad))
	aad2 = new AminoAcidDescriptor(*aad);
	TEST_NOT_EQUAL(aad2, 0)
	TEST_EQUAL(aad2->getType(), "HIS")
	TEST_EQUAL(aad2->getPhi(), Angle(M_PI, true))
	TEST_EQUAL(aad2->getPsi(), Angle(M_PI, true))
RESULT


//
// Tests for the complement get and set-methods
//

CHECK(setAminoAcidType(const String& type))
  aad->setAminoAcidType("GLY");
	TEST_EQUAL(aad->getType(),"GLY")
  aad2->setAminoAcidType("V");
	TEST_EQUAL(aad2->getType(),"VAL")
RESULT

CHECK(setPhi(const Angle& phi))
  aad->setPhi(Angle(-47., false));
	TEST_EQUAL(aad->getPhi(), Angle(-47., false) )
  aad2->setPhi(Angle(-77., false));
	TEST_EQUAL(aad2->getPhi(), Angle(-77., false) )
RESULT

CHECK(setPsi(const Angle& phi))
  aad->setPsi(Angle(-58., false));
	TEST_EQUAL(aad->getPsi(), Angle(-58., false) )
  aad2->setPsi(Angle(8.15, false));
	TEST_EQUAL(aad2->getPsi(), Angle(8.15, false) )
RESULT

CHECK(setOmega(const Angle& phi))
  aad->setOmega(Angle(90., false));
	TEST_EQUAL(aad->getOmega(), Angle(90., false))
  aad2->setOmega(Angle(M_PI, true));
	TEST_EQUAL(aad2->getOmega(), Angle(180, false))
RESULT

	/*CHECK(~AminoAcidDescriptor()) used for the addAminoAcid()-Test
	delete aad ;
  aad = 0; 
	RESULT*/

//
//   Tests of class PeptideBuilder 
//

PeptideBuilder* pb = 0;
CHECK(PeptideBuilder())
  pb= new PeptideBuilder;
	TEST_NOT_EQUAL(pb, 0)
RESULT

//
//CHECK(PeptideBuilder(const std::vector<AminoAcidDescriptor>& sequence)) later on
//

CHECK(addAminoAcid(const String& type, const Angle& phi, const Angle& psi, const Angle& omega))
  pb->addAminoAcid("LEU");
  pb->addAminoAcid("SER",Angle(M_PI, true),Angle(M_PI, true));
	TEST_NOT_EQUAL(pb, 0)
RESULT

CHECK(addAminoAcid(const AminoAcidDescriptor& aad))
  pb->addAminoAcid(*aad);
  pb->addAminoAcid(*aad2);
	TEST_NOT_EQUAL(pb, 0)
RESULT

CHECK(setChainName(const String& name)/getChainName())
  pb->setChainName("TestChainName");
	TEST_EQUAL(pb->getChainName(),"TestChainName")
RESULT

CHECK(setProteinName(const String& name)/getProteinName())
	pb->setProteinName("TestProteinName");
	TEST_EQUAL(pb->getProteinName(),"TestProteinName")
RESULT

PeptideBuilder* pb2 = 0;
CHECK(PeptideBuilder(const PeptideBuilder& pb))
  pb2= new PeptideBuilder(*pb);
	TEST_NOT_EQUAL(pb2, 0)
	TEST_EQUAL(pb2->getChainName(),"TestChainName")
	TEST_EQUAL(pb2->getProteinName(),"TestProteinName")
RESULT

PeptideBuilder* pb3=0;
CHECK(PeptideBuilder(const std::vector<AminoAcidDescriptor>& sequence))
  std::vector<AminoAcidDescriptor> v;
  v.push_back(*aad);
	v.push_back(*aad2);
  pb3 = new PeptideBuilder(v);
  TEST_NOT_EQUAL(pb3, 0)
RESULT

CHECK(construct())
	Protein *prot=pb->construct();
	ResidueIterator resIt;
  resIt = prot->beginResidue();

	TEST_EQUAL(resIt->getName(), "LEU")
	TEST_REAL_EQUAL(resIt->getTorsionPhi(), Angle(0,false));
  TEST_REAL_EQUAL(resIt->getTorsionPsi(), Angle(-58.,false));
  ++resIt;
	TEST_EQUAL(resIt->getName(), "SER")
	PRECISION(1e-3)
  TEST_REAL_EQUAL(fabs(resIt->getTorsionPhi().toRadian()), M_PI)
	TEST_REAL_EQUAL(fabs(resIt->getTorsionPsi().toRadian()), M_PI)
  ++resIt;
	PRECISION(2E-2)
	TEST_EQUAL(resIt->getName(), "GLY")
	TEST_REAL_EQUAL(resIt->getTorsionPhi(), Angle(-47, false))
  TEST_REAL_EQUAL(resIt->getTorsionPsi(), -1.01);
  ++resIt;
	TEST_EQUAL(resIt->getName(), "VAL")
  TEST_REAL_EQUAL(resIt->getTorsionPhi(), Angle(-77., false));
	TEST_REAL_EQUAL(resIt->getTorsionPsi(), Angle(0, false));
	
	prot = pb3->construct();
  resIt = prot->beginResidue();
		
	TEST_EQUAL(resIt->getName(), "GLY")
	TEST_REAL_EQUAL(resIt->getTorsionPhi(), Angle(0,false));
  TEST_REAL_EQUAL(resIt->getTorsionPsi(), Angle(-58.,false));
  ++resIt;
	TEST_EQUAL(resIt->getName(), "VAL")
  TEST_REAL_EQUAL(resIt->getTorsionPhi(), Angle(-77.,false));
	TEST_REAL_EQUAL(resIt->getTorsionPsi(), Angle(0,false));

	PeptideBuilder pb4;
	pb4.addAminoAcid("PRO");
	pb4.addAminoAcid("PRO", Angle(-12., false), Angle(42, false));
	pb4.addAminoAcid("PRO", Angle(28, false), Angle(33, false));

	prot = pb4.construct();
	
  resIt = prot->beginResidue();

	TEST_EQUAL(resIt->getName(), "PRO")
	TEST_REAL_EQUAL(resIt->getTorsionPhi(), Angle(0,false));
  TEST_REAL_EQUAL(resIt->getTorsionPsi(), Angle(-80.,false));
  ++resIt;
	TEST_EQUAL(resIt->getName(), "PRO")
  TEST_REAL_EQUAL(resIt->getTorsionPhi(), Angle(-12, false));
	TEST_REAL_EQUAL(resIt->getTorsionPsi(), Angle(-80, false));
  ++resIt;
	TEST_EQUAL(resIt->getName(), "PRO")
	TEST_REAL_EQUAL(resIt->getTorsionPhi(), Angle(28,false));
  TEST_EQUAL(resIt->getTorsionPsi(), 0);
RESULT

//empty sequence
CHECK(construct())
	pb->construct();
  TEST_NOT_EQUAL(pb, 0)
RESULT

PeptideBuilder* pb4=0;
CHECK(PeptideBuilder())
  pb4 = new PeptideBuilder(*pb);
	TEST_NOT_EQUAL(pb, 0)
RESULT

// Proline as first residue 
CHECK(~PeptideBuilder() throw())
	delete pb;
	pb = 0;
	delete pb2;
  pb2= 0; 
	delete pb3;
  pb3= 0; 
	delete pb4;
  pb4= 0; 
RESULT

CHECK(~AminoAcidDescriptor() throw())
	delete aad;
  aad = 0; 
	delete aad2;
  aad2= 0; 
RESULT


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
