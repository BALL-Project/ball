// $Id: numericalSAS.h,v 1.1 1999/08/26 07:53:20 oliver Exp $

#ifndef BALL_STRUCTURE_NUMERICALSAS_H
#define BALL_STRUCTURE_NUMERICALSAS_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_CONCEPT_COMPOSITE_H
#	include <BALL/CONCEPT/composite.h>
#endif

namespace BALL 
{

	/**	Calculate the solvent accessible surface area numerically.
			This method uses the algorithm by Eisenhaber, Lijnzaad, Argos, Sander,
			and Scharf ("The Double Cubic Lattice Method:: Efficient Approaches to 
			numerical Integration of Surface Area and Volume and to Dot
			Surface Contouring of Molecular Assemblies", J. Comput. Chem. (1995),
			{\underline 15}, 273-284).
		
			@param	composite the composite containing the atoms
			@param	probe_radius the probe radius used for the SAS
			@param	number_of_dots the number of dots used per atom
	*/
	float calculateNumericalSASArea
		(const Composite& composite, float probe_radius = 1.5, 
		 int number_of_dots = 300);
	
} // namespace BALL

#endif // BALL_STRUCTURE_NUMERICALSAS_H
