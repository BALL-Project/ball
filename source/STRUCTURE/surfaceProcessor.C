// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: surfaceProcessor.C,v 1.3 2002/12/17 14:13:21 anker Exp $

#include <BALL/STRUCTURE/surfaceProcessor.h>

namespace BALL
{

	SurfaceProcessor::SurfaceProcessor()
		:	ses_(true),
			surface_(),
			spheres_(),
			density_(4.5),
			probe_radius_(1.5)
	{
		radius_offset_ = 0.0;
		vdw_factor_ = 1.0;
	}


	bool SurfaceProcessor::start()
	{
		spheres_.clear();
		return true;
	}


	Processor::Result SurfaceProcessor::operator () (Atom& atom)
	{
		Vector3 pos_(atom.getPosition());
		TVector3<double> pos((double)pos_.x,(double)pos_.y,(double)pos_.z);
		/*if (Maths::isNotZero(atom.getRadius()))
		{
			spheres_.push_back(Sphere3(atom.getPosition(),atom.getRadius()));
		}
		else
		{
			if (atom.getElement() != Element::UNKNOWN)
			{
				if (atom.getElement().getVanDerWaalsRadius() > 0.0)
				{
					spheres_.push_back(Sphere3(atom.getPosition(),
																		 atom.getElement().getVanDerWaalsRadius()));
				}
				else
				{
					spheres_.push_back(Sphere3(atom.getPosition(), 1.0));
				}
			}
			else
			{
				spheres_.push_back(Sphere3(atom.getPosition(), 1.0));
			}
		}
		return Processor::CONTINUE;*/
		if (atom.getElement() != Element::UNKNOWN)
		{
			if (atom.getElement().getVanDerWaalsRadius() > 0.0)
			{
				spheres_.push_back(TSphere3<double>(pos,
													 vdw_factor_*atom.getElement().getVanDerWaalsRadius()+radius_offset_));
			}
			else
			{
				spheres_.push_back(TSphere3<double>(pos,radius_offset_+vdw_factor_));
			}
		}
		else
		{
			spheres_.push_back(TSphere3<double>(pos,radius_offset_+vdw_factor_));
		}
		return Processor::CONTINUE;
	}


	bool SurfaceProcessor::finish()
	{
		ReducedSurface* rs = new ReducedSurface(spheres_,probe_radius_);
		rs->compute();
		if (ses_)
		{
			SolventExcludedSurface* ses = new SolventExcludedSurface(rs);
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
						delete rs;
						probe_radius_ += diff;
						rs = new ReducedSurface(spheres_,probe_radius_);
						rs->compute();
						ses = new SolventExcludedSurface(rs);
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
				std::cout << "    ProbeRadius: " << probe_radius_
									<< "    Durchläufe: " << i;
		}
		else
		{
			SolventAccessibleSurface* sas = new SolventAccessibleSurface(rs);
			sas->compute();
			TriangulatedSAS* surface = new TriangulatedSAS(sas,density_);
			surface->compute();
			surface->exportSurface(surface_);
			delete surface;
			delete sas;
		}
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

}
