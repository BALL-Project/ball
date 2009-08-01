// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: pyBALLSipHelper.C,v 1.4.28.2 2007-03-28 13:58:08 amoll Exp $

#include <typeinfo>
#include "sipAPIVIEW.h"
#include <BALL/KERNEL/PDBAtom.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/secondaryStructure.h>
#include <BALL/KERNEL/chain.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/protein.h>
#include <BALL/KERNEL/nucleicAcid.h>
#include <BALL/KERNEL/nucleotide.h>
#include <BALL/KERNEL/molecule.h>

namespace BALL
{

#define BALL_TO_SIP_MAP_BASECLASS(type)\
	if (dynamic_cast<const type*>(&object) != NULL)\
		return BALL_CONVERT_FROM_INSTANCE(&object, type, 0);

	PyObject* pyMapBALLObjectToSip(Composite& object)
	{
		BALL_TO_SIP_MAP_BASECLASS(PDBAtom)
		BALL_TO_SIP_MAP_BASECLASS(Atom)
		BALL_TO_SIP_MAP_BASECLASS(Bond)
		BALL_TO_SIP_MAP_BASECLASS(Residue)
		BALL_TO_SIP_MAP_BASECLASS(SecondaryStructure)
		BALL_TO_SIP_MAP_BASECLASS(Chain)
		BALL_TO_SIP_MAP_BASECLASS(System)
		BALL_TO_SIP_MAP_BASECLASS(Protein)
		BALL_TO_SIP_MAP_BASECLASS(NucleicAcid)
		BALL_TO_SIP_MAP_BASECLASS(Nucleotide)
		BALL_TO_SIP_MAP_BASECLASS(Molecule)
		BALL_TO_SIP_MAP_BASECLASS(Fragment)
		BALL_TO_SIP_MAP_BASECLASS(AtomContainer)

		// last resort - this *should* work!
		return BALL_CONVERT_FROM_INSTANCE(&object, Composite, 0);
	}
}
