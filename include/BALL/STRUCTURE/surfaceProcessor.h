// $Id: surfaceProcessor.h,v 1.4 2000/10/31 09:11:23 oliver Exp $

#include "reducedSurface.h"
#include "solventExcludedSurface.h"
#include "singularities.h"
#include "triangulatedSurface.h"
#include "triangulation.h"
#include <BALL/MATHS/surface.h>
#include <BALL/CONCEPT/processor.h>
#include <BALL/KERNEL/atom.h>

namespace BALL
{
	/**
	*/
	class SurfaceProcessor
		:	public UnaryProcessor<Atom>
	{
		public:

		virtual bool start();
		virtual bool finish();

		virtual Processor::Result operator () (Atom&);

		const Surface& getSurface() const;
		void getSurface(Surface& surface) const;

		void setProbeRadius(float radius);

		float getProbeRadius();

		void setDensity(float radius);

		float getDensity();

		protected:
		Surface					surface_;
		vector<Sphere3>	spheres_;
		float						density_;
		float						probe_radius_;
	};


	bool SurfaceProcessor::start()
	{
		std::cerr << "SurfaceProcessor::start()" << std::endl;
		spheres_.clear();
		return true;
	}


	Processor::Result SurfaceProcessor::operator () (Atom& atom)
	{
		// spheres_.push_back(Sphere3(atom.getPosition(), atom.getRadius()));
		spheres_.push_back(Sphere3(atom.getPosition(), 1.2));
		std::cerr << "SurfaceProcessor::operator (" << atom.getFullName() << ")" << std::endl;
		return Processor::CONTINUE;
	}


	bool SurfaceProcessor::finish()
	{
		std::cerr << "SurfaceProcessor::finish()" << std::endl;
		ReducedSurface* rs = new ReducedSurface(spheres_,probe_radius_);
		rs->compute();
		SolventExcludedSurface* ses = new SolventExcludedSurface(rs);
		ses->get(rs);
		TreatSingularities(ses,probe_radius_);
		TriangulatedSurface* surface = new TriangulatedSurface(*Triangulate(ses,rs,density_));
		surface_ = surface->exportSurface();
		delete surface;
		delete ses;
		delete rs;
		return true;
	}


	const Surface& SurfaceProcessor::getSurface() const
	{
		return surface_;
	}

	void SurfaceProcessor::getSurface(Surface& surface) const
	{
		surface = surface_;
	}

	void SurfaceProcessor::setProbeRadius(float radius)
	{
		probe_radius_ = radius;
	}

	float SurfaceProcessor::getProbeRadius()
	{
		return probe_radius_;
	}

	void SurfaceProcessor::setDensity(float density)
	{
		density_ = density;
	}

	float SurfaceProcessor::getDensity()
	{
		return density_;
	}
}
