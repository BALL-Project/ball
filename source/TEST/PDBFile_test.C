// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: PDBFile_test.C,v 1.5.2.1 2003/01/07 13:22:38 anker Exp $

#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/FORMAT/PDBFile.h>
#include <BALL/FORMAT/HINFile.h>
#include <BALL/STRUCTURE/fragmentDB.h>

///////////////////////////

START_TEST(PDBFile, "$Id: PDBFile_test.C,v 1.5.2.1 2003/01/07 13:22:38 anker Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

PDBFile* pdb_file;
CHECK(PDBFile::PDBFile())											
pdb_file = new PDBFile;
TEST_NOT_EQUAL(pdb_file, 0)
RESULT

CHECK(PDBFile::~PDBFile())
delete pdb_file;
RESULT

CHECK(PDBFile::read(System&))
RESULT

CHECK(PDBFile::selectModel())
	PDBFile f("data/PDBFile_test_models.pdb");
	System s;
	f.read(s);
	TEST_EQUAL(s.countAtoms(), 1)
	s.clear();
	f.selectModel(2);
	f.read(s);
	TEST_EQUAL(s.countAtoms(), 2)
	s.clear();
	f.selectModel(3);
	f.read(s);
	TEST_EQUAL(s.countAtoms(), 3)
	s.clear();
	f.selectModel(4);
	f.read(s);
	TEST_EQUAL(s.countAtoms(), 0)
	s.clear();
	f.selectModel(0);
	f.read(s);
	TEST_EQUAL(s.countAtoms(), 6)
	s.clear();
	f.options.setInteger(GenericPDBFile::Option::CHOOSE_MODEL, 0);
	f.read(s);
	TEST_EQUAL(s.countAtoms(), 6)
	s.clear();
	f.options.setInteger(GenericPDBFile::Option::CHOOSE_MODEL, 1);
	f.read(s);
	TEST_EQUAL(s.countAtoms(), 1)
	s.clear();
	f.options.setInteger(GenericPDBFile::Option::CHOOSE_MODEL, 2);
	f.read(s);
	TEST_EQUAL(s.countAtoms(), 2)
	s.clear();
	f.options.setInteger(GenericPDBFile::Option::CHOOSE_MODEL, 3);
	f.read(s);
	TEST_EQUAL(s.countAtoms(), 3)
	s.clear();
	f.options.setInteger(GenericPDBFile::Option::CHOOSE_MODEL, 4);
	f.read(s);
	TEST_EQUAL(s.countAtoms(), 0)

	PDBFile g("data/PDBFile_test_no_model.pdb");
	s.clear();
	g.read(s);
	TEST_EQUAL(s.countAtoms(), 3)
	s.clear();
	g.selectModel(1);
	g.read(s);
	TEST_EQUAL(s.countAtoms(), 3)
	s.clear();
	g.selectModel(0);
	g.read(s);
	TEST_EQUAL(s.countAtoms(), 3)
	s.clear();
	g.selectModel(2);
	g.read(s);
	TEST_EQUAL(s.countAtoms(), 0)
RESULT

CHECK(PDBFile strict line checking)
	PDBFile f("data/PDBFile_test_line_checking.pdb");
	System s;
	f.read(s);
	TEST_EQUAL(s.countAtoms(), 3)
	s.clear();
	f.options.setBool(GenericPDBFile::Option::STRICT_LINE_CHECKING, true);
	f.read(s);
	TEST_EQUAL(s.countAtoms(), 2)
RESULT

CHECK(PDBFile::write(System&))
	PDBFile f;
	f.open("data/PDBFile_test2.pdb");
	String tmp_filename;
	NEW_TMP_FILE(tmp_filename)
	System S;
	f.read(S);
	TEST_EQUAL(S.countAtoms(), 892);
	f.close();

	f.open(tmp_filename, std::ios::out);
	f.write(S);
	f.close();

	TEST_FILE_REGEXP(tmp_filename.c_str(), "data/PDBFile_test2.txt")

	f.open(tmp_filename, std::ios::out);
	f.write(S);
	f.close();

	TEST_FILE_REGEXP(tmp_filename.c_str(), "data/PDBFile_test2.txt")
RESULT


CHECK(writing of Systems containing Atoms instead of PDBAtoms)
	FragmentDB db;
	System* system = new System;
	Protein* protein = new Protein;
	Chain* chain = new Chain;

	String filename;
	NEW_TMP_FILE(filename)
	PDBFile outfile(filename, std::ios::out);
	outfile << *system;
	outfile.close();
	TEST_FILE_REGEXP(filename.c_str(), "data/PDBFile_test_write_empty.txt")

	system->insert(*protein);
	protein->insert(*chain);
	chain->insert(*db.getResidueCopy("ALA"));
	TEST_EQUAL(system->countAtoms(), 10)
	NEW_TMP_FILE(filename)
	STATUS(system->countBonds())
	outfile.open(filename, std::ios::out);
	outfile << *system;
	outfile.close();
	TEST_FILE_REGEXP(filename.c_str(), "data/PDBFile_test3.txt")

	HINFile methane("data/methane.hin");
	system->clear();
	methane.read(*system);
	TEST_EQUAL(system->countAtoms(), 5)
	NEW_TMP_FILE(filename)
	outfile.open(filename, std::ios::out);
	outfile << *system;
	outfile.close();
	TEST_FILE_REGEXP(filename.c_str(), "data/PDBFile_test_write_methane.txt")

	delete system;
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
