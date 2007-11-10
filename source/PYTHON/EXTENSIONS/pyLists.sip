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
	#include "sipAPIBALL.h"
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
	#include "sipAPIBALL.h"
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
	#include "sipAPIBALL.h"
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
	#include "sipAPIBALL.h"
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
	#include "sipAPIBALL.h"
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
	#include "sipAPIBALL.h"
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
	#include "sipAPIBALL.h"
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
	#include "sipAPIBALL.h"
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
	#include "sipAPIBALL.h"
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
	#include "sipAPIBALL.h"
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
	#include "sipAPIBALL.h"
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
	#include "sipAPIBALL.h"
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


%MappedType GeometricObjectList
{
%TypeHeaderCode
	#include "sipAPIBALL.h"
	#include <BALL/VIEW/KERNEL/representation.h>
	#include <BALL/PYTHON/pyListHelper.h>
%End

%ConvertFromTypeCode
	BALL_CONVERT_FROM(GeometricObject)
%End

%ConvertToTypeCode
	if (sipCppPtr == 0) return 0;
	BALL_CONVERT_TO(GeometricObject)
%End
};


%MappedType RepresentationList
{
%TypeHeaderCode
	#include "sipAPIBALL.h"
	#include <BALL/VIEW/KERNEL/representation.h>
	#include <BALL/PYTHON/pyListHelper.h>
%End

%ConvertFromTypeCode
	BALL_CONVERT_FROM(Representation)
%End

%ConvertToTypeCode
	if (sipCppPtr == 0) return 0;
	BALL_CONVERT_TO(Representation)
%End
};


%MappedType RegularData3DList
{
%TypeHeaderCode
	#include "sipAPIBALL.h"
	#include <BALL/PYTHON/pyListHelper.h>
%End

%ConvertFromTypeCode
	BALL_CONVERT_FROM(RegularData3D)
%End

%ConvertToTypeCode
	if (sipCppPtr == 0) return 0;
	BALL_CONVERT_TO(RegularData3D)
%End
};

