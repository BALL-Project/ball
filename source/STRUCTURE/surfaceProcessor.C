// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: surfaceProcessor.C,v 1.5 2002/12/18 03:14:33 amoll Exp $

#include <BALL/STRUCTURE/surfaceProcessor.h>

namespace BALL
{

	SurfaceProcessor::SurfaceProcessor()
		:	ses_(true),
			surface_(),
			spheres_(),
			density_(4.5),
			probe_radius_(1.5),
			radius_offset_(0.0),
			vdw_factor_(1.0)
	{
	}


	bool SurfaceProcessor::start()
	{
		spheres_.clear();
		return true;
	}


	Processor::Result SurfaceProcessor::operator () (Atom& atom)
	{
		Vector3 float_position(atom.getPosition());
		TVector3<double> position((double)float_position.x,
				                      (double)float_position.y,
				                      (double)float_position.z);
		
		if (atom.getElement() != Element::UNKNOWN)
		{
			if (atom.getElement().getVanDerWaalsRadius() > 0.0)
			{
				spheres_.push_back(TSphere3<double>(position, vdw_factor_*atom.getElement().getVanDerWaalsRadius() + radius_offset_));
			}
			else
			{
				spheres_.push_back(TSphere3<double>(position, radius_offset_+vdw_factor_));
			}
		}
		else
		{
			spheres_.push_back(TSphere3<double>(position, radius_offset_+vdw_factor_));
		}
		return Processor::CONTINUE;
	}


	bool SurfaceProcessor::finish()
	{
		ReducedSurface* reduced_surface = new ReducedSurface(spheres_,probe_radius_);
		reduced_surface->compute();

		if (ses_)
		{
			SolventExcludedSurface* ses = new SolventExcludedSurface(reduced_surface);
			ses->compute();
			double diff = (probe_radius_ < 1.5 ? 0.1 : -0.1);
			int i = 0;
			bool ok = false;
			while (!ok && (i < 10))
			{
				i++;
				ok = ses->check();
				if (!ok)
				{
					delete ses;
					delete reduced_surface;
					probe_radius_ += diff;
					reduced_surface = new ReducedSurface(spheres_,probe_radius_);
					reduced_surface->compute();
					ses = new SolventExcludedSurface(reduced_surface);
					ses->compute();
				}
			}
			if (ok)
			{
				TriangulatedSES* surface = new TriangulatedSES(ses,density_);
				surface->compute();
				surface->exportSurface(surface_);
				delete surface;
			}
			delete ses;
#ifdef BALL_DEBUG
			Log.error() << "    ProbeRadius: " << probe_radius_ << "    Durchläufe: " << i << std::endl;
#endif
		}
		else
		{
			SolventAccessibleSurface* sas = new SolventAccessibleSurface(reduced_surface);
			sas->compute();
			TriangulatedSAS* surface = new TriangulatedSAS(sas,density_);
			surface->compute();
			surface->exportSurface(surface_);
			delete surface;
			delete sas;
		}
		delete reduced_surface;
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

	void SurfaceProcessor::setProbeRadius(double radius)
	{
		probe_radius_ = radius;
	}

	double SurfaceProcessor::getProbeRadius()
	{
		return probe_radius_;
	}

	void SurfaceProcessor::setDensity(double density)
	{
		density_ = density;
	}

	double SurfaceProcessor::getDensity()
	{
		return density_;
	}

	vector< TSphere3<double> >& SurfaceProcessor::getSpheres()
	{
		return spheres_;
	}

} // namespace
