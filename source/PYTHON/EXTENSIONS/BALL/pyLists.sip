// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

// The following lines define the SIP interface for all derived Composite lists
// To shorten the code, a special macro is involved.
// (Unfortunately, SIP will not create correct code if the if line is missing.)

%MappedType AtomList
{
%TypeHeaderCode
  #include <BALL/KERNEL/extractors.h>
  #include <BALL/PYTHON/pyBALLSipHelper.h>
  #include <BALL/PYTHON/pyListHelper.h>
	using namespace BALL;
%End

%ConvertFromTypeCode
	BALL_CONVERT_LIST_FROM(Atom)
%End
 
%ConvertToTypeCode
  if (sipCppPtr == 0) return 0;
	BALL_CONVERT_LIST_TO(Atom)
%End
};


%MappedType PDBAtomList
{
%TypeHeaderCode
  #include <BALL/KERNEL/extractors.h>
  #include <BALL/PYTHON/pyBALLSipHelper.h>
  #include <BALL/PYTHON/pyListHelper.h>
	using namespace BALL;
%End

%ConvertFromTypeCode
	BALL_CONVERT_LIST_FROM(PDBAtom)
%End
 
%ConvertToTypeCode
  if (sipCppPtr == 0) return 0;
	BALL_CONVERT_LIST_TO(PDBAtom)
%End
};


%MappedType BondList
{
%TypeHeaderCode
  #include <BALL/KERNEL/extractors.h>
  #include <BALL/PYTHON/pyBALLSipHelper.h>
  #include <BALL/PYTHON/pyListHelper.h>
	using namespace BALL;
%End

%ConvertFromTypeCode
	BALL_CONVERT_LIST_FROM(Bond)
%End
 
%ConvertToTypeCode
  if (sipCppPtr == 0) return 0;
	BALL_CONVERT_LIST_TO(Bond)
%End
};


%MappedType AtomContainerList
{
%TypeHeaderCode
  #include <BALL/KERNEL/extractors.h>
  #include <BALL/PYTHON/pyBALLSipHelper.h>
  #include <BALL/PYTHON/pyListHelper.h>
	using namespace BALL;
%End

%ConvertFromTypeCode
	BALL_CONVERT_LIST_FROM(AtomContainer)
%End
 
%ConvertToTypeCode
  if (sipCppPtr == 0) return 0;
	BALL_CONVERT_LIST_TO(AtomContainer)
%End
};


%MappedType FragmentList
{
%TypeHeaderCode
  #include <BALL/KERNEL/extractors.h>
  #include <BALL/PYTHON/pyBALLSipHelper.h>
  #include <BALL/PYTHON/pyListHelper.h>
	using namespace BALL;
%End

%ConvertFromTypeCode
	BALL_CONVERT_LIST_FROM(Fragment)
%End
 
%ConvertToTypeCode
  if (sipCppPtr == 0) return 0;
	BALL_CONVERT_LIST_TO(Fragment)
%End
};


%MappedType MoleculeList
{
%TypeHeaderCode
  #include <BALL/KERNEL/extractors.h>
  #include <BALL/PYTHON/pyBALLSipHelper.h>
  #include <BALL/PYTHON/pyListHelper.h>
	using namespace BALL;
%End

%ConvertFromTypeCode
	BALL_CONVERT_LIST_FROM(Molecule)
%End
 
%ConvertToTypeCode
  if (sipCppPtr == 0) return 0;
	BALL_CONVERT_LIST_TO(Molecule)
%End
};


%MappedType ResidueList
{
%TypeHeaderCode
  #include <BALL/KERNEL/extractors.h>
  #include <BALL/PYTHON/pyBALLSipHelper.h>
  #include <BALL/PYTHON/pyListHelper.h>
	using namespace BALL;
%End

%ConvertFromTypeCode
	BALL_CONVERT_LIST_FROM(Residue)
%End
 
%ConvertToTypeCode
  if (sipCppPtr == 0) return 0;
	BALL_CONVERT_LIST_TO(Residue)
%End
};


%MappedType SecondaryStructureList
{
%TypeHeaderCode
  #include <BALL/KERNEL/extractors.h>
  #include <BALL/PYTHON/pyBALLSipHelper.h>
  #include <BALL/PYTHON/pyListHelper.h>
	using namespace BALL;
%End

%ConvertFromTypeCode
	BALL_CONVERT_LIST_FROM(SecondaryStructure)
%End
 
%ConvertToTypeCode
  if (sipCppPtr == 0) return 0;
	BALL_CONVERT_LIST_TO(SecondaryStructure)
%End
};


%MappedType ChainList
{
%TypeHeaderCode
  #include <BALL/KERNEL/extractors.h>
  #include <BALL/PYTHON/pyBALLSipHelper.h>
  #include <BALL/PYTHON/pyListHelper.h>
	using namespace BALL;
%End

%ConvertFromTypeCode
	BALL_CONVERT_LIST_FROM(Chain)
%End
 
%ConvertToTypeCode
  if (sipCppPtr == 0) return 0;
	BALL_CONVERT_LIST_TO(Chain)
%End
};


%MappedType ProteinList
{
%TypeHeaderCode
  #include <BALL/KERNEL/extractors.h>
  #include <BALL/PYTHON/pyBALLSipHelper.h>
  #include <BALL/PYTHON/pyListHelper.h>
	using namespace BALL;
%End

%ConvertFromTypeCode
	BALL_CONVERT_LIST_FROM(Protein)
%End
 
%ConvertToTypeCode
  if (sipCppPtr == 0) return 0;
	BALL_CONVERT_LIST_TO(Protein)
%End
};


%MappedType NucleotideList
{
%TypeHeaderCode
  #include <BALL/KERNEL/extractors.h>
  #include <BALL/PYTHON/pyBALLSipHelper.h>
  #include <BALL/PYTHON/pyListHelper.h>
	using namespace BALL;
%End

%ConvertFromTypeCode
	BALL_CONVERT_LIST_FROM(Nucleotide)
%End
 
%ConvertToTypeCode
  if (sipCppPtr == 0) return 0;
	BALL_CONVERT_LIST_TO(Nucleotide)
%End
};


%MappedType NucleicAcidList
{
%TypeHeaderCode
  #include <BALL/KERNEL/extractors.h>
  #include <BALL/PYTHON/pyBALLSipHelper.h>
  #include <BALL/PYTHON/pyListHelper.h>
	using namespace BALL;
%End

%ConvertFromTypeCode
	BALL_CONVERT_LIST_FROM(NucleicAcid)
%End
 
%ConvertToTypeCode
  if (sipCppPtr == 0) return 0;
	BALL_CONVERT_LIST_TO(NucleicAcid)
%End
};


%MappedType RegularData3DList
{
%TypeHeaderCode
	#include <BALL/PYTHON/pyListHelper.h>
	using namespace BALL;
%End

%ConvertFromTypeCode
	BALL_CONVERT_LIST_FROM(RegularData3D)
%End

%ConvertToTypeCode
	if (sipCppPtr == 0) return 0;
	BALL_CONVERT_LIST_TO(RegularData3D)
%End
};

