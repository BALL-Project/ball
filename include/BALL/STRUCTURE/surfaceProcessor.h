// $Id: surfaceProcessor.h,v 1.10 2001/02/22 16:23:22 strobel Exp $

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

		vector<Sphere3>	getSpheres();

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
		//spheres_.push_back(Sphere3(atom.getPosition(), atom.getRadius()));
		//spheres_.push_back(Sphere3(atom.getPosition(), atom.getElement().getVanDerWaalsRadius()));
		spheres_.push_back(Sphere3(atom.getPosition(), 1.0));
		return Processor::CONTINUE;
	}


	bool SurfaceProcessor::finish()
	{
		double old_epsilon = Constants::EPSILON;
		Constants::EPSILON = 1e-4;
				/*
				cout << "probe radius:    "; cin >> probe_radius_;
				spheres_.clear();
				char* filename;
				string name;
				cout << "xyzr-file:       "; cin >> name;
				filename = (char*)malloc(sizeof(char)*(name.size()+1));
				for (Position i = 0; i < name.size(); i++)
				{
					filename[i] = name[i];
				}
				filename[name.size()] = '\0';
				cout << filename << "\n";
				float std_radius;
				cout << "standart radius: "; cin >> std_radius;
				std::ifstream input(filename);
				Position size;
				input >> size;
				for (Position i = 0; i < size; i++)
				{
					float x, y, z, r;
					input >> x >> y >> z >> r;
					if (Maths::isZero(std_radius))
					{
						spheres_.push_back(Sphere3(Vector3(x,y,z),r));
					}
					else
					{
						spheres_.push_back(Sphere3(Vector3(x,y,z),std_radius));
					}
				}
				*/
				std::cerr << "initialising reduced surface ...\n";
		ReducedSurface* rs = new ReducedSurface(spheres_,probe_radius_);
				std::cerr << "... ok\ncomputing reduced surface ...\n";
		rs->compute();
				std::ofstream rsfile("ReducedSurface.log");
				rsfile << *rs;
				rsfile.close();
				/*for (Position p = 0; p < rs->numberOfVertices(); p++)
				{
					Index atom = rs->getVertex(p)->getAtom();
					Sphere3 sphere(rs->getSphere(atom));
					surface_.vertex.push_back(sphere.p);
					surface_.normal.push_back(Vector3(0,0,0));
				}
				for (Position t = 0; t < rs->numberOfFaces(); t++)
				{
					Surface::Triangle triangle;
					triangle.v1 = rs->getFace(t)->getVertex(0)->getIndex();
					triangle.v2 = rs->getFace(t)->getVertex(1)->getIndex();
					triangle.v3 = rs->getFace(t)->getVertex(2)->getIndex();
					Vector3 test((rs->getSphere(rs->getFace(t)->getVertex(0)->getAtom()).p-
												rs->getSphere(rs->getFace(t)->getVertex(1)->getAtom()).p) %
											 (rs->getSphere(rs->getFace(t)->getVertex(0)->getAtom()).p-
												rs->getSphere(rs->getFace(t)->getVertex(2)->getAtom()).p)		);
					test.normalize();
					if (test != rs->getFace(t)->getNormal())
					{
						Index temp = triangle.v2;
						triangle.v2 = triangle.v3;
						triangle.v3 = temp;
					}
					surface_.normal[triangle.v1] += rs->getFace(t)->getNormal();
					surface_.normal[triangle.v2] += rs->getFace(t)->getNormal();
					surface_.normal[triangle.v3] += rs->getFace(t)->getNormal();
					surface_.triangle.push_back(triangle);
				}*/
				std::cerr << "... ok\ninitialising solvent excluded surface ...\n";
		SolventExcludedSurface* ses = new SolventExcludedSurface(rs);
				std::cerr << "... ok\ncomputing solvent excluded surface ...\n";
		ses->get(rs);
				std::ofstream sesfile("SolventExcludedSurface.log");
				sesfile << *ses;
				sesfile.close();
				std::cerr << "... ok\ntreating singularities ...\n";
		//TreatSingularities(ses,probe_radius_);
				density_ = 4.5;
				std::cerr << "... ok\ntriangulating surface ...\n";
		TriangulatedSurface* surface = Triangulate(ses,rs,density_);
				std::cerr << "... " surface->triangles.size() << " Dreiecke ... ok\n";
				//cout << *surface;
				std::cerr << "exporting surface ...\n";
		surface->exportSurface(surface_);
				std::cerr << "... ok\ndeleting surface ...\n";
		delete surface;
				std::cerr << "... ok\ndeleting solvent excluded surface ...\n";
		delete ses;
				std::cerr << "... ok\ndeleting reduced surface ...\n";
		delete rs;
				std::cerr << "... ok\n";
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

	vector<Sphere3>	SurfaceProcessor::getSpheres()
	{
		return spheres_;
	}

}
