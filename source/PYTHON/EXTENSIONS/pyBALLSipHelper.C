// $Id: pyBALLSipHelper.C,v 1.1 2001/09/03 16:53:12 oliver Exp $

#include <typeinfo>

#include "sipBALLDeclBALL.h"
#include "sipBALLComposite.h"
#include "sipBALLAtom.h"
#include "sipBALLBond.h"
#include "sipBALLPDBAtom.h"
#include "sipBALLAtomContainer.h"
#include "sipBALLFragment.h"
#include "sipBALLResidue.h"
#include "sipBALLNucleotide.h"
#include "sipBALLMolecule.h"
#include "sipBALLProtein.h"
#include "sipBALLNucleicAcid.h"
#include "sipBALLSecondaryStructure.h"
#include "sipBALLChain.h"
#include "sipBALLSystem.h"
#include "sipBALLObject.h"

namespace BALL
{

#define BALL_TO_SIP_MAP(type)\
	if (typeid(object) == typeid(RTTI::getDefault<type>()))\
	{\
		return sipMapCppToSelf(&object, sipClass_##type);\
	}\

#define BALL_TO_SIP_MAP_BASECLASS(type)\
	if (dynamic_cast<const type*>(&object) != NULL)\
		return sipMapCppToSelf(&object, sipClass_##type);

	PyObject* pyMapBALLObjectToSip(Composite& object)
	{
		BALL_TO_SIP_MAP(Chain)
		BALL_TO_SIP_MAP(Atom)
		BALL_TO_SIP_MAP(PDBAtom)
		BALL_TO_SIP_MAP(Fragment)
		BALL_TO_SIP_MAP(Residue)
		BALL_TO_SIP_MAP(Molecule)
		BALL_TO_SIP_MAP(Protein)
		BALL_TO_SIP_MAP(SecondaryStructure)
		BALL_TO_SIP_MAP(AtomContainer)
		BALL_TO_SIP_MAP(NucleicAcid)
		BALL_TO_SIP_MAP(Nucleotide)
		BALL_TO_SIP_MAP(System)
		BALL_TO_SIP_MAP(Bond)

		BALL_TO_SIP_MAP_BASECLASS(Atom)
		BALL_TO_SIP_MAP_BASECLASS(PDBAtom)
		BALL_TO_SIP_MAP_BASECLASS(Fragment)
		BALL_TO_SIP_MAP_BASECLASS(Residue)
		BALL_TO_SIP_MAP_BASECLASS(Molecule)
		BALL_TO_SIP_MAP_BASECLASS(Chain)
		BALL_TO_SIP_MAP_BASECLASS(Protein)
		BALL_TO_SIP_MAP_BASECLASS(SecondaryStructure)
		BALL_TO_SIP_MAP_BASECLASS(AtomContainer)
		BALL_TO_SIP_MAP_BASECLASS(NucleicAcid)
		BALL_TO_SIP_MAP_BASECLASS(Nucleotide)
		BALL_TO_SIP_MAP_BASECLASS(System)
		BALL_TO_SIP_MAP_BASECLASS(Bond)

		// last resort - this *should* work!
		return sipMapCppToSelf(&object, sipClass_Composite);
	}
}
