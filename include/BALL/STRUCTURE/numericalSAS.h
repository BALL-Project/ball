// $Id: numericalSAS.h,v 1.3 2000/05/30 10:43:27 oliver Exp $

#ifndef BALL_STRUCTURE_NUMERICALSAS_H
#define BALL_STRUCTURE_NUMERICALSAS_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

namespace BALL 
{

	class Atom;
	class BaseFragment;
	template <typename Key, typename T>
  class HashMap;

	/**	Calculate the areas of the atoms on the solvent accessible surface area numerically.
			This method uses the algorithm by Eisenhaber, Lijnzaad, Argos, Sander,
			and Scharf ("The Double Cubic Lattice Method:: Efficient Approaches to 
			numerical Integration of Surface Area and Volume and to Dot
			Surface Contouring of Molecular Assemblies", J. Comput. Chem. (1995),
			{\underline 15}, 273-284).
		
			@param  atom_areas a hash map containing the areas of the atoms (returned)
			@param	fragment the kernel object containing the atoms
			@param	probe_radius the probe radius used for the SAS
			@param	number_of_dots the number of dots used per atom
	*/
	void	calculateNumericalSASAtomAreas
					(HashMap<Atom*,float>& atom_areas,
					 const BaseFragment& fragment, float probe_radius = 1.5, 
					 Size number_of_dots = 400);
	
	/** Calculate the solvent accessible surface area numerically.
			This method uses the algorithm by Eisenhaber, Lijnzaad, Argos, Sander,
			and Scharf ("The Double Cubic Lattice Method:: Efficient Approaches to
			numerical Integration of Surface Area and Volume and to Dot
			Surface Contouring of Molecular Assemblies", J. Comput. Chem. (1995),
			{\underline 15}, 273-284).

			@param  fragment the kernel object containing the atoms
			@param  probe_radius the probe radius used for the SAS
			@param  number_of_dots the number of dots used per atom
	*/
	float calculateNumericalSASArea
					(const BaseFragment& fragment, float probe_radius = 1.5,
					 Size number_of_dots = 300);                                                 

} // namespace BALL

#endif // BALL_STRUCTURE_NUMERICALSAS_H
