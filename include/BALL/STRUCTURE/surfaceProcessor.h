// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: surfaceProcessor.h,v 1.39 2003/09/07 17:23:59 oliver Exp $
//

#ifndef BALL_STRUCTURE_SURFACE_PROCESSOR_H
#define BALL_STRUCTURE_SURFACE_PROCESSOR_H

#ifndef BALL_STRUCTURE_REDUCEDSURFACE_H
#	include <BALL/STRUCTURE/reducedSurface.h>
#endif
#ifndef BALL_STRUCTURE_SOLVENTEXCLUDEDSURFACE_H
#	include <BALL/STRUCTURE/solventExcludedSurface.h>
#endif
#ifndef BALL_STRUCTURE_TRIANGULATEDSES_H
#	include <BALL/STRUCTURE/triangulatedSES.h>
#endif
#ifndef BALL_STRUCTURE_SOLVENTACCESSIBLESURFACE_H
#	include <BALL/STRUCTURE/solventAccessibleSurface.h>
#endif
#ifndef BALL_STRUCTURE_TRIANGULATEDSAS_H
#	include <BALL/STRUCTURE/triangulatedSAS.h>
#endif
#ifndef BALL_STRUCTURE_TRIANGULATEDSURFACE_H
#	include <BALL/STRUCTURE/triangulatedSurface.h>
#endif
#ifndef BALL_MATHS_SURFACE_H
#	include <BALL/MATHS/surface.h>
#endif
#ifndef BALL_CONCEPT_PROCESSOR_H
#	include <BALL/CONCEPT/processor.h>
#endif
#ifndef BALL_KERNEL_ATOM_H
#	include <BALL/KERNEL/atom.h>
#endif
#ifndef BALL_KERNEL_PTE_H
#	include <BALL/KERNEL/PTE.h>
#endif

namespace BALL
{

	/** SurfaceProcessor.
	\ingroup Surface			
	*/
	class SurfaceProcessor
		:	public UnaryProcessor<Atom>
	{
		public:

		/**	@name Enums
		*/
		//@{
		///
		enum SurfaceType
		{
			SOLVENT_EXCLUDED_SURFACE,
			SOLVENT_ACCESSIBLE_SURFACE
		};
		//@}

		/** @name Constructors and destructor.
		*/
		//@{
		
		/// Default constructor
		SurfaceProcessor();
	
		//@}

		/** @name Processor-related methods.
		*/
		//@{

		///
		virtual bool start();

		///
		virtual bool finish();

		///
 		virtual Processor::Result operator () (Atom&  atom);
		//@}
		/** @name Accessors.
		*/
		//@{

		///
		const Surface& getSurface() const { return surface_; }
		
		///
		Surface& getSurface() { return surface_; }
	
		///
		void setProbeRadius(double radius) { probe_radius_ = radius; }
		
		///
		double getProbeRadius() const { return probe_radius_; }
		
		///
		void setDensity(double density) { density_ = density; }
		
		///
		double getDensity() const { return density_; }
		
		///
		std::vector<TSphere3<double> >& getSpheres() { return spheres_; }

		/** Set the surface type to be computed.
				Default is SOLVENT_EXCLUDED_SURFACE.
		*/
		void setType(SurfaceType type) { surface_type_ = type; }

		/// Get the surface type to be computed.
		SurfaceType getType() const  { return surface_type_; }
		//@}

		protected:

		///
		double													radius_offset_;

		///
		double													vdw_factor_;

		//_
		SurfaceType											surface_type_;
		
		//_
		Surface													surface_;
		
		//_
		std::vector<TSphere3<double> >	spheres_;
		
		//_
		double													density_;
		
		//_
		double													probe_radius_;
	};
   
}

#endif //  BALL_STRUCTURE_SURFACE_PROCESSOR_H
