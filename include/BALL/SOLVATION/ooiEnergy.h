// $Id: ooiEnergy.h,v 1.3 2000/08/30 19:58:25 oliver Exp $

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

	/** Calculate the solvation energy.
			Calculates the solvation energy after
			OOI et al. (Proc. of the National US Academy of Sciences)
			The energy is returned in units of kJ/mol
			Unparametrized atoms are simply ignored, this leads to
			unreasonable values.\\

			{\bf Files:}\\
			types are taken from {\tt solvation/Ooi.ini}\\
			{\bf Definition:} \URL{BALL/SOLVATION/ooiEnergy.h}
			\\
	*/ 
	double calculateOoiEnergy(AtomContainer& atoms);

		
} // namespace BALL

#endif
