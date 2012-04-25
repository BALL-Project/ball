// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_STRUCTURE_NUMERICALSAS_H
#define BALL_STRUCTURE_NUMERICALSAS_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_DATATYPE_OPTIONS_H
# include <BALL/DATATYPE/options.h>
#endif

#ifndef BALL_MATHS_SURFACE_H
# include <BALL/MATHS/surface.h>
#endif

#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector3.h>
#endif

namespace BALL 
{
	class Atom;
	class AtomContainer;
	template <typename Key, typename T>
  class HashMap;
	class TriangulatedSphere;

	/**	@name	Fast Numerical Calculation of Solvent Accessible Surface Area.
			These functions are loosely based on the algorithm by Eisenhaber, Lijnzaad, Argos,
			Sander, and Scharf ("The Double Cubic Lattice Method: Efficient Approaches to
			numerical Integration of Surface Area and Volume and to Dot Surface Contouring
			of Molecular Assemblies", J. Comput. Chem. (1995), <b>  15 </b>, 273-284)
			and Eisenhaber and Argos ("Improved Strategy in Analytic Surface Calculation
			for Molecular Systems: Handling of Singularities and Computational Efficiency"
			J. Comput. Chem. (1993), <b> 14 </b>, 1272-1280).

			\ingroup Surface
			@{
	*/
	class BALL_EXPORT NumericalSAS
	{
		public:
			/**	@name Constant Definitions
			*/
			//@{

			/** Option names
			 */
			struct BALL_EXPORT Option
			{
				/** This flag decides whether total area and area per atom will
				 *  be computed (default = true).
				 */
				static const String COMPUTE_AREA;

				/** This flag decides whether total volume and volume per atom will
				 *  be computed (default = true).
				 */
				static const String COMPUTE_VOLUME;

				/** This flag decides whether surface points and normals will be
				 *  explicitly computed and stored in the class (default = false).
				 *
				 *  Please note that the normals will not have unit length, but will
				 *  be scaled according such that the sum over all normal lengths yields
				 *  the surface area.
				 */
				static const String COMPUTE_SURFACE;

				/** This flag decides whether surface points and normals per atom
				 *  will be explicitly computed and stored in the class 
				 *  (default = false).
				 *
				 *  Please note that the normals will not have unit length, but will
				 *  be scaled according such that the sum over all normal lengths yields
				 *  the surface area.
				 */
				static const String COMPUTE_SURFACE_PER_ATOM;

				/** This flag decides whether a vector of (atom center, surface) per atom
				 *  will be explicitly computed and stored in the class (default = false).
				 *
				 *  Please note that the normals will not have unit length, but will
				 *  be scaled according such that the sum over all normal lengths yields
				 *  the surface area.
				 */
				static const String COMPUTE_SURFACE_MAP;

				/** The number of point samples per sphere. Please note that this
				 *  number is a lower limit, since sampling is performed through
				 *  icosahedron or dodecahedron refinement that cannot produce
				 *  arbitrary point numbers.
				 */
				static const String NUMBER_OF_POINTS;

				/** The radius of the spherical probe used for the SAS definition.
				 */
				static const String PROBE_RADIUS;
			};

			/** Default values for NumericalSAS options.
			 */
			struct BALL_EXPORT Default
			{
				/** This flag decides whether total area and area per atom will
				 *  be computed (default = true).
				 */
				static const bool COMPUTE_AREA;

				/** This flag decides whether total volume and volume per atom will
				 *  be computed (default = true).
				 */
				static const bool COMPUTE_VOLUME;

				/** This flag decides whether surface points and normals will be
				 *  explicitly computed and stored in the class (default = false).
				 */
				static const bool COMPUTE_SURFACE;

				/** This flag decides whether surface points and normals per atom
				 *  will be explicitly computed and stored in the class 
				 *  (default = false).
				 */
				static const bool COMPUTE_SURFACE_PER_ATOM;

				/** This flag decides whether a vector of (atom center, surface) per atom
				 *  will be explicitly computed and stored in the class (default = false).
				 */
				static const bool COMPUTE_SURFACE_MAP;

				/** Default number of point samples per sphere (400). Please note 
				 *  that this number is a lower limit, since sampling is performed 
				 *  through icosahedron or dodecahedron refinement that cannot 
				 *  produce arbitrary point numbers. (@see Option::NUMBER_OF_POINTS)
				 */
				static const Size NUMBER_OF_POINTS;

				/** Default radius of the spherical probe used for the SAS
				 *  definition (1.5 \AA). (@see Option::PROBE_RADIUS)
				 */
				static const float PROBE_RADIUS;
			};
			//@}


			/** @name Constructors and Destructors. **/
			//@{

			BALL_CREATE(NumericalSAS)

			/** Default Constructor.
			 */
			NumericalSAS();

			/** Detailed Constructor.
			 */
			NumericalSAS(const Options& options);

			/** Destructor.
			 */
			virtual ~NumericalSAS();
			//@}
			
			/** @name Accessors. **/
			//@{

			void operator() (const AtomContainer& fragment);

			/** Returns the total area of the fragment.
			 *
			 * 	This function only returns sensible values after a call
			 * 	to operator() and only if area computation has not been
			 * 	disabled through the options.
			 */
			float getTotalArea() const {return total_area_;}

			/** Returns the area per atom of the fragment.
			 *
			 * 	This function only returns sensible values after a call
			 * 	to operator() and only if area computation has not been
			 * 	disabled through the options.
			 */
			HashMap<const Atom*, float>& getAtomAreas() {return atom_areas_;}

			/** Returns the area per atom of the fragment, const version.
			 *
			 * 	This function only returns sensible values after a call
			 * 	to operator() and only if area computation has not been
			 * 	disabled through the options.
			 */
			const HashMap<const Atom*, float>& getAtomAreas() const {return atom_areas_;}

			/** Returns the total volume of the fragment.
			 *
			 * 	This function only returns sensible values after a call
			 * 	to operator() and only if volume computation has not been
			 * 	disabled through the options.
			 */
			float getTotalVolume() const {return total_volume_;}

			/** Returns the volume per atom of the fragment.
			 *
			 * 	This function only returns sensible values after a call
			 * 	to operator() and only if volume computation has not been
			 * 	disabled through the options.
			 */
			HashMap<const Atom*, float>& getAtomVolumes() {return atom_volumes_;}

			/** Returns the volume per atom of the fragment, const version.
			 *
			 * 	This function only returns sensible values after a call
			 * 	to operator() and only if volume computation has not been
			 * 	disabled through the options.
			 */
			const HashMap<const Atom*, float>& getAtomVolumes() const {return atom_volumes_;}

			/** Returns the solvent accessible surface as a collection of points and normals.
			 *
			 * 	This function only returns sensible values after a call
			 * 	to operator() and only if surface computation has not been
			 * 	disabled through the options.
			 */
			Surface& getSurface() {return surface_;}

			/** Returns the solvent accessible surface as a collection of points and normals, const version.
			 *
			 * 	This function only returns sensible values after a call
			 * 	to operator() and only if surface computation has not been
			 * 	disabled through the options.
			 */
			const Surface& getSurface() const {return surface_;}

			/** Returns the solvent accessible surface as a collection of points and normals per atom.
			 *
			 * 	This function only returns sensible values after a call
			 * 	to operator() and only if surface computation per atom has not been
			 * 	disabled through the options.
			 */
			HashMap<const Atom*, Surface>& getSurfacePerAtom() {return atom_surfaces_;}

			/** Returns the solvent accessible surface as a collection of points and normals per atom, const version.
			 *
			 * 	This function only returns sensible values after a call
			 * 	to operator() and only if surface computation per atom has not been
			 * 	disabled through the options.
			 */
			const HashMap<const Atom*, Surface>& getSurfacePerAtom() const {return atom_surfaces_;}

			/** Returns the solvent accessible surface as a collection of points and normals per atom center.
			 *
			 * 	This function only returns sensible values after a call
			 * 	to operator() and only if surface computation per atom center has not been
			 * 	disabled through the options.
			 */
			std::vector< std::pair<Vector3, Surface> >& getSurfaceMap() {return atom_surface_map_;}

			/** Returns the solvent accessible surface as a collection of points and normals per atom center, const version.
			 *
			 * 	This function only returns sensible values after a call
			 * 	to operator() and only if surface computation per atom center has not been
			 * 	disabled through the options.
			 */
			const std::vector< std::pair<Vector3, Surface> >& getSurfaceMap() const {return atom_surface_map_;}
			
			//@}

			/** Options.
			 */
			Options options;

		protected:
			/** Set values from options. */
			void setDefaultOptions_();

			/** Precompute the sphere tesselation for the requested
			 *  number of points.
			 */
			Size computeSphereTesselation_(TriangulatedSphere& result, int num_points);

			/// the AtomContainer we are bound to
			AtomContainer const* fragment_;

			/// mapping of atom to SAS area
			HashMap<Atom const*, float> atom_areas_;

			/// total solvent accessible area of the fragment
			float total_area_;

			/// mapping of atom to SAS volume
			HashMap<Atom const*, float> atom_volumes_;

			/// total solvent accessible volume of the fragment
			float total_volume_;

			/// the SAS as a surface
			Surface surface_;

			/// mapping of atom to surface
			HashMap<Atom const*, Surface> atom_surfaces_;

			/// vector of (atom center, surface)
			std::vector< std::pair<Vector3, Surface> > atom_surface_map_;
	};

   /** @} */
} // namespace BALL

#endif // BALL_STRUCTURE_NUMERICALSAS_H
