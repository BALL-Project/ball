// $Id: surfaceProcessor.h,v 1.19 2001/09/19 17:55:33 strobel Exp $

#include <BALL/STRUCTURE/reducedSurface.h>
#include <BALL/STRUCTURE/solventExcludedSurface.h>
#include <BALL/STRUCTURE/singularities.h>
#include <BALL/STRUCTURE/triangulatedSurface.h>
#include <BALL/STRUCTURE/triangulation.h>
#include <BALL/MATHS/surface.h>
#include <BALL/CONCEPT/processor.h>
#include <BALL/KERNEL/atom.h>

#include <string>

namespace BALL
{
	/** SurfaceProcessor.
			{\bf Definition:} \URL{BALL/STRUCTURE/surfaceProcessor.h}
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

		vector<Sphere3>& getSpheres();

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
		if (Maths::isNotZero(atom.getRadius()))
		{
			spheres_.push_back(Sphere3(atom.getPosition(), atom.getRadius()));
		}
		else
		{
			if (atom.getElement() != Element::UNKNOWN)
			{
				if (atom.getElement().getVanDerWaalsRadius() > 0.0)
				{
					spheres_.push_back(Sphere3(atom.getPosition(), atom.getElement().getVanDerWaalsRadius()));
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

		return Processor::CONTINUE;
	}


	bool SurfaceProcessor::finish()
	{
		double old_epsilon = Constants::EPSILON;
		Constants::EPSILON = 1e-4;
				probe_radius_ = 1.5;
				density_ = 4.5;

				#ifdef debug_surface_processor
				::std::cerr << "initialise reduced surface ...\n";
				#endif
		ReducedSurface* rs = new ReducedSurface(spheres_,probe_radius_);
				#ifdef debug_surface_processor
				::std::cerr << "... ok\ncompute reduced surface ...\n";
				#endif
		try
		{
			rs->compute();
		}
		catch (Exception::GeneralException e)
		{
			delete rs;
			::std::cerr << "can not compute rs: exception " << e.getName() << " (" << e.getMessage()
									<< ") in line " << e.getLine() << " of file " << e.getFile() << "\n";
			return false;
		}
				#ifdef debug_surface_processor_print
				std::ofstream rsfile("ReducedSurface.log");
				rsfile << *rs;
				rsfile.close();
				::std::cerr << "... " << rs->numberOfAtoms() << " atoms, "
										<< rs->numberOfVertices() << " vertices, "
										<< rs->numberOfEdges() << " edges, "
										<< rs->numberOfFaces() << " faces ... ok\n"
										<< "initialise solvent excluded surface ...\n";
				#endif
		SolventExcludedSurface* ses = new SolventExcludedSurface(rs);
				#ifdef debug_surface_processor
				::std::cerr << "... ok\ncompute solvent excluded surface ...\n";
				#endif
		try
		{
			ses->get(rs);
		}
		catch (Exception::GeneralException e)
		{
			delete ses;
			delete rs;
			::std::cerr << "can not compute ses: exception " << e.getName() << " (" << e.getMessage()
									<< ") in line " << e.getLine() << " of file " << e.getFile() << "\n";
			return false;
		}
				#ifdef debug_surface_processor_print
				std::ofstream sesfile("SolventExcludedSurface.log");
				sesfile << *ses;
				sesfile.close();
				::std::cerr << "... ok\ntreat singularities ...\n";
				#endif
		try
		{
			TreatSingularities(ses,rs,probe_radius_);
		}
		catch (Exception::GeneralException e)
		{
			delete ses;
			delete rs;
			::std::cerr << "can not treat singularities: exception " << e.getName() << " (" << e.getMessage()
									<< ") in line " << e.getLine() << " of file " << e.getFile() << "\n";
			return false;
		}
				#ifdef debug_surface_processor_print
				rsfile.open("ReducedSurface.clean.log");
				rsfile << *rs;
				rsfile.close();
				sesfile.open("SolventExcludedSurface.clean.log");
				sesfile << *ses;
				sesfile.close();
				::std::cerr << "... ok\ntriangulate surface ...\n";
				#endif
		TriangulatedSurface* surface;
		try
		{
			surface = Triangulate(ses,rs,density_);
		}
		catch (Exception::GeneralException e)
		{
			delete ses;
			delete rs;
			::std::cerr << "can not triangulate: exception " << e.getName() << " (" << e.getMessage()
									<< ") in line " << e.getLine() << " of file " << e.getFile() << "\n";
			return false;
		}
				#ifdef debug_surface_processor
				std::cerr << "... " << surface->points.size() << " Punkte, "
									<< surface->triangles.size() << " Dreiecke ... ok\n";
				::std::cerr << "export surface ...\n";
				#endif
		surface->exportSurface(surface_);
				#ifdef debug_surface_processor
				::std::cerr << "... ok\ndelete surface ...\n";
				#endif
		delete surface;
				#ifdef debug_surface_processor
				::std::cerr << "... ok\ndelete solvent excluded surface ...\n";
				#endif
		delete ses;
				#ifdef debug_surface_processor
				::std::cerr << "... ok\ndelete reduced surface ...\n";
				#endif
		delete rs;
				#ifdef debug_surface_processor
				::std::cerr << "... ok\n";
				#endif
		Constants::EPSILON = old_epsilon;
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

	vector<Sphere3>&	SurfaceProcessor::getSpheres()
	{
		return spheres_;
	}

}
