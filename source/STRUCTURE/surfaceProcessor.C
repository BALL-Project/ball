// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/STRUCTURE/surfaceProcessor.h>

namespace BALL
{

	SurfaceProcessor::SurfaceProcessor()
		:	UnaryProcessor<Atom>(),
			radius_offset_(0.0),
			vdw_factor_(1.0),
			surface_type_(SurfaceProcessor::SOLVENT_EXCLUDED_SURFACE),
			surface_(),
			spheres_(),
			density_(4.5),
			probe_radius_(1.5)
	{
	}


	bool SurfaceProcessor::start()
	{
		spheres_.clear();
		return true;
	}


	Processor::Result SurfaceProcessor::operator () (Atom& atom)
	{
		TVector3<double> position((double)atom.getPosition().x,
				                      (double)atom.getPosition().y,
				                      (double)atom.getPosition().z);
		
		if (atom.getElement() != Element::UNKNOWN && atom.getElement().getVanDerWaalsRadius() > 0.0)
		{
			spheres_.push_back(TSphere3<double>(position, vdw_factor_ * atom.getElement().getVanDerWaalsRadius() + radius_offset_));
		}
		else
		{
			spheres_.push_back(TSphere3<double>(position, radius_offset_ + vdw_factor_));
		}

		return Processor::CONTINUE;
	}


	bool SurfaceProcessor::finish()
	{
		if (spheres_.empty())
		{
			Log.error() << "empty surface" << std::endl;
			return true;
		}
	
		ReducedSurface* reduced_surface = new ReducedSurface(spheres_, probe_radius_);
		reduced_surface->compute();

		if (surface_type_ == SurfaceProcessor::SOLVENT_EXCLUDED_SURFACE)
		{
			SolventExcludedSurface* ses = new SolventExcludedSurface(reduced_surface);
			ses->compute();
			double diff = (probe_radius_ < 1.5 ? 0.01 : -0.01);
			Size i = 0;
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
					reduced_surface = new ReducedSurface(spheres_, probe_radius_);
					reduced_surface->compute();
					ses = new SolventExcludedSurface(reduced_surface);
					ses->compute();
				}
			}
			if (ok)
			{
				TriangulatedSES* surface = new TriangulatedSES(ses, density_);
				surface->compute();
				surface->exportSurface(surface_);
				delete surface;
			}
			delete ses;
		}
		else
		{
			SolventAccessibleSurface* sas = new SolventAccessibleSurface(reduced_surface);
			sas->compute();

			TriangulatedSAS* surface = new TriangulatedSAS(sas, density_);
			surface->compute();
			surface->exportSurface(surface_);

			delete surface;
			delete sas;
		}

		delete reduced_surface;

		return true;
	}

} // namespace
