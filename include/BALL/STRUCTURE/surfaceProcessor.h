#include "reducedSurface.h"
#include "solventExcludedSurface.h"
#include "singularities.h"
#include "triangulatedSurface.h"
#include "triangulation.h"
#include <BALL/MATHS/surface.h>

class SurfaceProcessor
	:	public UnaryProcessor<Atom>
{
	public:

	virtual bool start();
	virtual bool finish();

	virtual Processor::Result operator () (Atom&);

	const Surface& getSurface() const;

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
	spheres_.clear();
	return true;
}


Processor::Result SurfaceProcessor::operator () (Atom& atom)
{
	spheres_.push_back(Sphere3(atom.getPosition(), atom.getRadius()));
	return Processor::CONTINUE;
}


bool SurfaceProcessor::finish()
{
	ReducedSurface* rs = new ReducedSurface(spheres_,probe_radius_);
	rs->compute();
	SolventExcludedSurface* ses = new SolventExcludedSurface(rs);
	ses->get(rs);
	TreatSingularities(ses,probe_radius_);
	TriangulatedSurface* surface = new TriangulatedSurface(Triangulate(ses,rs,density_));
	surface_ = surface.export();
	delete surface_;
	delete ses;
	delete rs;
	return true;
}


const Surface& getSurface() const
{
	return surface_;
}

void setProbeRadius(float radius)
{
	probe_radius_ = radius;
}

float getProbeRadius()
{
	return probe_radius_;
}

void setDensity(float density);
{
	density_ = density;
}

float getDensity()
{
	return density_;
}
