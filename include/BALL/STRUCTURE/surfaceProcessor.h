// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: surfaceProcessor.h,v 1.29 2003/02/19 16:16:02 amoll Exp $

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
			{\bf Definition:} \URL{BALL/STRUCTURE/surfaceProcessor.h}
	*/
	class SurfaceProcessor
		:	public UnaryProcessor<Atom>,
			public UnaryProcessor<Atom*>
	{
		public:

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
 		virtual Processor::Result operator () (Atom*& atom)
		{ return operator() (*atom);}

		//@}
		/** @name Accessors.
		*/
		//@{

		///
		const Surface& getSurface() const;
		
		///
		void getSurface(Surface& surface) const;
	
		///
		void setProbeRadius(double radius);
		
		///
		double getProbeRadius();
		
		///
		void setDensity(double radius);
		
		///
		double getDensity();
		
		///
		std::vector< TSphere3<double> >& getSpheres();

		//@}

		///
		double													radius_offset_;

		///
		double													vdw_factor_;

		protected:

		//_
		bool														ses_;
		
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
