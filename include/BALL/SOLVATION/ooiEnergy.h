// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: ooiEnergy.h,v 1.10 2003/03/26 13:08:26 sturm Exp $

#ifndef BALL_SOLVATION_OOIENERGY_H
#define BALL_SOLVATION_OOIENERGY_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_MOLMEC_COMMON_TYPERULEPROCESSOR_H
#	include <BALL/MOLMEC/COMMON/typeRuleProcessor.h>
#endif

namespace BALL 
{
	class AtomContainer;

	/** Calculate the solvation energy after OOI et al. 
			(Proc. of the National US Academy of Sciences)
			The energy is returned in units of kJ/mol.
			Unparametrized atoms are simply ignored, this leads to unreasonable values.  \par
			<b>Files:</b> \par
 types are taken from <tt>solvation/Ooi.ini</tt> \par
	\ingroup Solvation	
	*/ 
	double calculateOoiEnergy(AtomContainer& atoms);
   	
} // namespace BALL

#endif
