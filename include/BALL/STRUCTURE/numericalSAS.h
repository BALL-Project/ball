// $Id: numericalSAS.h,v 1.7 2000/07/04 08:13:40 oliver Exp $

#ifndef BALL_STRUCTURE_NUMERICALSAS_H
#define BALL_STRUCTURE_NUMERICALSAS_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

namespace BALL 
{

	class Atom;
	class Surface;
	class BaseFragment;
	template <typename Key, typename T>
  class HashMap;

	/**	@name	Fast Numerical Calculation of Solvent Accessible Surface Area.
			These functions use the algorithm by Eisenhaber, Lijnzaad, Argos, Sander,
			and Scharf ("The Double Cubic Lattice Method: Efficient Approaches to 
			numerical Integration of Surface Area and Volume and to Dot
			Surface Contouring of Molecular Assemblies", J. Comput. Chem. (1995),
			{\underline 15}, 273-284).
	*/
	
	/** Calculate the solvent accessible surface area numerically.
			This method returns the total 
			Solvent Accessible Surface (SAS) of a BALL kernel object. 
			Atoms with a radius of 0 are ignored.

			@param  fragment the kernel object containing the atoms
			@param  probe_radius the probe radius used for the SAS
			@param  number_of_dots the number of dots used per atom
			@return the total SAS area in $\AA^2$
	*/
	float calculateSASArea
					(const BaseFragment& fragment, float probe_radius = 1.5,
					 Size number_of_dots = 400); 

	/** Calculate the solvent accessible volume numerically.
			This method returns the total volume enclosd by the 
			Solvent Accessible Surface (SAS) of a BALL kernel object. 
			Atoms with a radius of 0 are ignored

			@param  fragment the kernel object containing the atoms
			@param  probe_radius the probe radius used for the SAS
			@param  number_of_dots the number of dots used per atom
			@return the volume in $\AA^3$
	*/
	float calculateSASVolume
					(const BaseFragment& fragment, float probe_radius = 1.5,
					 Size number_of_dots = 400); 

	/**	Calculate the Solvent Accessible Surface area for each atom.
			This method returns the surface fraction of each atom at the 
			Solvent Accessible Surface (SAS). Atoms with a radius of 0 are 
			ignored. All areas are in $\AA^2$.
			@param  atom_areas a hash map containing the areas of the atoms (returned)
			@param	fragment the kernel object containing the atoms
			@param	probe_radius the probe radius used for the SAS
			@param	number_of_dots the number of dots used per atom
			@return the total SAS area in $\AA^2$
	*/
	float calculateSASAtomAreas
					(const BaseFragment& fragment, HashMap<Atom*,float>& atom_areas,
					 float probe_radius = 1.5, Size number_of_dots = 400);
	
	/**	Calculate a point set on the Solvent Accessible Surface.
			This method returns the points on the Solvent Accessible
			Surface (SAS) used to calculate the surface area.
			The \Ref{Surface} object holds just the vertices, it
			does not contain any triangles. The normals for each point
			normals to the SAS in that point, their length equals the 
			fraction of the surface area represented by this point in $\AA^2$:
			\[	
					|\vec{n_i}| = \frac{\mathrm{SAS of atom} i}{\mathrm{number of points on the SAS of atom} i}
			\]
			Atoms with a radius of 0 are ignored.
			@param  surface a surface object containing the point coordinates and their normals (returned)
			@param	fragment the kernel object containing the atoms
			@param	probe_radius the probe radius used for the SAS
			@param	number_of_dots the number of dots used per atom
			@return the total SAS area in $\AA^2$
	*/
	float calculateSASPoints
					(const BaseFragment& fragment, Surface& surface_points,
					 float probe_radius = 1.5,  Size number_of_dots = 400);

} // namespace BALL

#endif // BALL_STRUCTURE_NUMERICALSAS_H
