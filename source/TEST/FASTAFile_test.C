// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: FASTAFile_test.C,2011/06/06$
//
// Author:
//   Nikola Koch
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>


///////////////////////////

#include <BALL/KERNEL/protein.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/system.h>
#include <BALL/STRUCTURE/peptides.h>
#include <BALL/SEQUENCE/sequence.h>
#include <BALL/FORMAT/fastaFile.h>
#include <iostream>
///////////////////////////

START_TEST(FASTAFile, "$Id: FASTAFile_test.C,2011/06/06$")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

	FASTAFile *fp= new FASTAFile();
		
//////////////////////////////////////// Constructor and Deconstructor Test ///////////////////////////////////////////////////

CHECK(FASTAFile())
	TEST_NOT_EQUAL(fp,0);
RESULT


CHECK(~FASTAFile())
	delete fp;
RESULT



//Checks whether Exception is thrown if filename is invalid
CHECK(FASTAFile(const BALL::String& filename,BALL::File::OpenMode open_mode=std::ios::in, bool trim_whitespaces=false))
	cout<< "TEST EXCEPTION"<<endl;

	FASTAFile *fp;
	TEST_EXCEPTION(Exception::FileNotFound, fp= new FASTAFile("x"))
	TEST_EXCEPTION(Exception::FileNotFound, fp=new FASTAFile("abscdf"))
RESULT


	fp=new FASTAFile();

	////////////////////open the File ///////////////////////////

	String* seq=new String();
	
//////////////////////////////////////////////////////////////////// Reading Tests //////////////////////////////////////////////////77

	Protein *pp = new Protein();
	//Molecule *mp;
	System *sp = new System();
	String *rs= new String("AGTACGTAGTAGCTGCTGCTACGTGCGCTAGCTAGTACGTCACGACGTAGATGCTAGCTGACTCGATGC");
	
CHECK(read (Protein& protein))

	fp->open(BALL_TEST_DATA_PATH(FASTAFile_test1.fasta));


	fp->read(*pp);

	 *seq = Peptides::GetSequence(*pp);
	
	TEST_EQUAL(*seq,*rs);

RESULT

////////TODO CHECK(operator>>(FASTAFile file, Molecule& molecue))

	
	/**	fp>>mp;
	seq=
	TEST_EQUAL(
	*/
	

CHECK(read(System& system))

	seq->destroy();
	
	fp->open(BALL_TEST_DATA_PATH(FASTAFile_test1.fasta));

	fp->read(*sp);

	//Get the Sequence out of the System	
	for(ProteinIterator it = sp->beginProtein(); +it; ++it) {
    	*seq = Peptides::GetSequence(*it);
	}

	
        TEST_EQUAL(*seq, *rs);
RESULT

//delete no more needed pointer
	
	delete pp;
//	delete mp;
	
	
	delete sp;




	//instantiate new ones
	 Protein *np=new Protein();
         System *ns=new System();


CHECK(operator>>(Protein& protein))

	seq->destroy();
	fp->open(BALL_TEST_DATA_PATH(FASTAFile_test1.fasta));


        fp->read(*np);
        *seq=Peptides::GetSequence(*pp);
        TEST_EQUAL(*seq, *rs)
RESULT


        
CHECK(operator>>(System& system))

	seq->destroy();
	fp->open(BALL_TEST_DATA_PATH(FASTAFile_test1.fasta));

	fp->read(*ns);
         for(ProteinIterator it = ns->beginProtein(); +it; ++it) {
         *seq = Peptides::GetSequence(*it);
        }
        TEST_EQUAL(*seq, *rs)
RESULT          









/** TODO test the write function
                void write(Protein& protein);
                        void write(Molecule& molecule);
                        void write(System& system);
*/
                ////////////////////////////////////////////////// Misc ////////////////////////////////////////////////////

						

/////////////////////////////////////////////////////////////
/////

END_TEST
