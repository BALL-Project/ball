// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: ooiEnergy.h,v 1.6 2003/02/21 16:06:15 anhi Exp $

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
			<b>Definition:</b> BALL/SOLVATION/ooiEnergy.h
	*/ 
	double calculateOoiEnergy(AtomContainer& atoms);
		
} // namespace BALL

#endif
