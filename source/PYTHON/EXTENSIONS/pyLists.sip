// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: pyLists.sip,v 1.1.2.1 2007/03/27 21:30:42 amoll Exp $
//

// The following lines define the SIP interface for all derived Composite lists
// To shorten the code, a special macro is involved.
// (Unfortunately, SIP will not create correct code if the if line is missing.)

%MappedType AtomList
{
%TypeHeaderCode
	#include "sipBALLAtom.h"
  #include <BALL/KERNEL/extractors.h>
  #include <BALL/PYTHON/pyBALLSipHelper.h>
  #include <BALL/PYTHON/pyListHelper.h>
%End

%ConvertFromTypeCode
	BALL_CONVERT_FROM(Atom)
%End
 
%ConvertToTypeCode
  if (sipCppPtr == 0) return 0;
	BALL_CONVERT_TO(Atom)
%End
};


%MappedType PDBAtomList
{
%TypeHeaderCode
	#include "sipBALLPDBAtom.h"
  #include <BALL/KERNEL/extractors.h>
  #include <BALL/PYTHON/pyBALLSipHelper.h>
  #include <BALL/PYTHON/pyListHelper.h>
%End

%ConvertFromTypeCode
	BALL_CONVERT_FROM(PDBAtom)
%End
 
%ConvertToTypeCode
  if (sipCppPtr == 0) return 0;
	BALL_CONVERT_TO(PDBAtom)
%End
};


%MappedType BondList
{
%TypeHeaderCode
	#include "sipBALLBond.h"
  #include <BALL/KERNEL/extractors.h>
  #include <BALL/PYTHON/pyBALLSipHelper.h>
  #include <BALL/PYTHON/pyListHelper.h>
%End

%ConvertFromTypeCode
	BALL_CONVERT_FROM(Bond)
%End
 
%ConvertToTypeCode
  if (sipCppPtr == 0) return 0;
	BALL_CONVERT_TO(Bond)
%End
};


%MappedType AtomContainerList
{
%TypeHeaderCode
	#include "sipBALLAtomContainer.h"
  #include <BALL/KERNEL/extractors.h>
  #include <BALL/PYTHON/pyBALLSipHelper.h>
  #include <BALL/PYTHON/pyListHelper.h>
%End

%ConvertFromTypeCode
	BALL_CONVERT_FROM(AtomContainer)
%End
 
%ConvertToTypeCode
  if (sipCppPtr == 0) return 0;
	BALL_CONVERT_TO(AtomContainer)
%End
};


%MappedType FragmentList
{
%TypeHeaderCode
	#include "sipBALLFragment.h"
  #include <BALL/KERNEL/extractors.h>
  #include <BALL/PYTHON/pyBALLSipHelper.h>
  #include <BALL/PYTHON/pyListHelper.h>
%End

%ConvertFromTypeCode
	BALL_CONVERT_FROM(Fragment)
%End
 
%ConvertToTypeCode
  if (sipCppPtr == 0) return 0;
	BALL_CONVERT_TO(Fragment)
%End
};


%MappedType MoleculeList
{
%TypeHeaderCode
	#include "sipBALLMolecule.h"
  #include <BALL/KERNEL/extractors.h>
  #include <BALL/PYTHON/pyBALLSipHelper.h>
  #include <BALL/PYTHON/pyListHelper.h>
%End

%ConvertFromTypeCode
	BALL_CONVERT_FROM(Molecule)
%End
 
%ConvertToTypeCode
  if (sipCppPtr == 0) return 0;
	BALL_CONVERT_TO(Molecule)
%End
};


%MappedType ResidueList
{
%TypeHeaderCode
	#include "sipBALLResidue.h"
  #include <BALL/KERNEL/extractors.h>
  #include <BALL/PYTHON/pyBALLSipHelper.h>
  #include <BALL/PYTHON/pyListHelper.h>
%End

%ConvertFromTypeCode
	BALL_CONVERT_FROM(Residue)
%End
 
%ConvertToTypeCode
  if (sipCppPtr == 0) return 0;
	BALL_CONVERT_TO(Residue)
%End
};


%MappedType SecondaryStructureList
{
%TypeHeaderCode
	#include "sipBALLSecondaryStructure.h"
  #include <BALL/KERNEL/extractors.h>
  #include <BALL/PYTHON/pyBALLSipHelper.h>
  #include <BALL/PYTHON/pyListHelper.h>
%End

%ConvertFromTypeCode
	BALL_CONVERT_FROM(SecondaryStructure)
%End
 
%ConvertToTypeCode
  if (sipCppPtr == 0) return 0;
	BALL_CONVERT_TO(SecondaryStructure)
%End
};


%MappedType ChainList
{
%TypeHeaderCode
	#include "sipBALLChain.h"
  #include <BALL/KERNEL/extractors.h>
  #include <BALL/PYTHON/pyBALLSipHelper.h>
  #include <BALL/PYTHON/pyListHelper.h>
%End

%ConvertFromTypeCode
	BALL_CONVERT_FROM(Chain)
%End
 
%ConvertToTypeCode
  if (sipCppPtr == 0) return 0;
	BALL_CONVERT_TO(Chain)
%End
};


%MappedType ProteinList
{
%TypeHeaderCode
	#include "sipBALLProtein.h"
  #include <BALL/KERNEL/extractors.h>
  #include <BALL/PYTHON/pyBALLSipHelper.h>
  #include <BALL/PYTHON/pyListHelper.h>
%End

%ConvertFromTypeCode
	BALL_CONVERT_FROM(Protein)
%End
 
%ConvertToTypeCode
  if (sipCppPtr == 0) return 0;
	BALL_CONVERT_TO(Protein)
%End
};


%MappedType NucleotideList
{
%TypeHeaderCode
	#include "sipBALLNucleotide.h"
  #include <BALL/KERNEL/extractors.h>
  #include <BALL/PYTHON/pyBALLSipHelper.h>
  #include <BALL/PYTHON/pyListHelper.h>
%End

%ConvertFromTypeCode
	BALL_CONVERT_FROM(Nucleotide)
%End
 
%ConvertToTypeCode
  if (sipCppPtr == 0) return 0;
	BALL_CONVERT_TO(Nucleotide)
%End
};


%MappedType NucleicAcidList
{
%TypeHeaderCode
	#include "sipBALLNucleicAcid.h"
  #include <BALL/KERNEL/extractors.h>
  #include <BALL/PYTHON/pyBALLSipHelper.h>
  #include <BALL/PYTHON/pyListHelper.h>
%End

%ConvertFromTypeCode
	BALL_CONVERT_FROM(NucleicAcid)
%End
 
%ConvertToTypeCode
  if (sipCppPtr == 0) return 0;
	BALL_CONVERT_TO(NucleicAcid)
%End
};

