// $Id: numericalSAS.h,v 1.2 2000/02/05 12:46:28 len Exp $

#ifndef BALL_STRUCTURE_NUMERICALSAS_H
#define BALL_STRUCTURE_NUMERICALSAS_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_CONCEPT_COMPOSITE_H
#	include <BALL/CONCEPT/composite.h>
#endif

#ifndef BALL_DATATYPE_HASHMAP_H
#	include <BALL/DATATYPE/hashMap.h>
#endif 

#ifndef	BALL_KERNEL_ATOM_H
#	include <BALL/KERNEL/atom.h>
#endif

namespace BALL 
{

	/**	Calculate the areas of the atoms on the solvent accessible surface area numerically.
			This method uses the algorithm by Eisenhaber, Lijnzaad, Argos, Sander,
			and Scharf ("The Double Cubic Lattice Method:: Efficient Approaches to 
			numerical Integration of Surface Area and Volume and to Dot
			Surface Contouring of Molecular Assemblies", J. Comput. Chem. (1995),
			{\underline 15}, 273-284).
		
			@param  hashmap that contains the areas of the atoms 
			@param	composite the composite containing the atoms
			@param	probe_radius the probe radius used for the SAS
			@param	number_of_dots the number of dots used per atom
	*/
	void	calculateNumericalSASAtomAreas
		(HashMap<Atom*,float>& aareas,
		 const Composite& composite, float probe_radius = 1.5, 
		 int number_of_dots = 400);
	
        /**     Calculate the solvent accessible surface area numerically.
                        This method uses the algorithm by Eisenhaber, Lijnzaad, Argos, Sander,
                        and Scharf ("The Double Cubic Lattice Method:: Efficient Approaches to
                        numerical Integration of Surface Area and Volume and to Dot
                        Surface Contouring of Molecular Assemblies", J. Comput. Chem. (1995),
                        {\underline 15}, 273-284).

                        @param  composite the composite containing the atoms
                        @param  probe_radius the probe radius used for the SAS
                        @param  number_of_dots the number of dots used per atom
        */
        float calculateNumericalSASArea
                (const Composite& composite, float probe_radius = 1.5,
                 int number_of_dots = 300);                                                 

} // namespace BALL

#endif // BALL_STRUCTURE_NUMERICALSAS_H
