// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: surfaceProcessor.h,v 1.35 2003/05/08 11:29:41 oliver Exp $

#include <BALL/STRUCTURE/reducedSurface.h>
#include <BALL/STRUCTURE/solventExcludedSurface.h>
#include <BALL/STRUCTURE/triangulatedSES.h>
#include <BALL/STRUCTURE/solventAccessibleSurface.h>
#include <BALL/STRUCTURE/triangulatedSAS.h>
#include <BALL/STRUCTURE/triangulatedSurface.h>
#include <BALL/MATHS/surface.h>
#include <BALL/CONCEPT/processor.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/PTE.h>

#include <string>

namespace BALL
{

	/** SurfaceProcessor.
	\ingroup Surface			
	*/
	class SurfaceProcessor
		:	public UnaryProcessor<Atom>,
			public UnaryProcessor<Atom*>
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

		/** @name Processor related methods.
		*/
		//@{

		///
		virtual bool start();

		///
		virtual bool finish();

		///
 		virtual Processor::Result operator () (Atom&  atom);

		///
 		virtual Processor::Result operator () (Atom*& atom) { return operator() (*atom);}
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
