// $Id: surfaceProcessor.h,v 1.16 2001/06/28 17:22:55 strobel Exp $

#include <BALL/STRUCTURE/reducedSurface.h>
#include <BALL/STRUCTURE/solventExcludedSurface.h>
#include <BALL/STRUCTURE/singularities.h>
#include <BALL/STRUCTURE/triangulatedSurface.h>
#include <BALL/STRUCTURE/triangulation.h>
#include <BALL/MATHS/surface.h>
#include <BALL/CONCEPT/processor.h>
#include <BALL/KERNEL/atom.h>

#include <string>

//#define debug_surface_processor

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
		try
		{
			double old_epsilon = Constants::EPSILON;
			Constants::EPSILON = 1e-4;
					probe_radius_ = 1.5;
					density_ = 4.5;

					#ifdef debug_surface_processor
					/*std::cerr << "probe radius:    "; std::cin >> probe_radius_;
					spheres_.clear();
					char* filename;
					string name;
					std::cerr << "xyzr-file:       "; std::cin >> name;
					filename = (char*)malloc(sizeof(char)*(name.size()+1));
					for (Position i = 0; i < name.size(); i++)
					{
						filename[i] = name[i];
					}
					filename[name.size()] = '\0';
					std::cerr << filename << "\n";
					float std_radius;
					std::cerr << "standart radius: "; std::cin >> std_radius;
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
					}*/
				
					::std::cerr << "initialise reduced surface ...\n";
					#endif
			ReducedSurface* rs = new ReducedSurface(spheres_,probe_radius_);
					#ifdef debug_surface_processor
					::std::cerr << "... ok\ncompute reduced surface ...\n";
					#endif
			rs->compute();
					#ifdef debug_surface_processor
					std::ofstream rsfile("ReducedSurface.log");
					rsfile << *rs;
					rsfile.close();
					/*for (Position p = 0; p < rs->numberOfFaces(); p++)
					{
						Vector3 p1(rs->getSphere(rs->getFace(p)->getVertex(0)->getAtom()).p);
						Vector3 p2(rs->getSphere(rs->getFace(p)->getVertex(1)->getAtom()).p);
						Vector3 p3(rs->getSphere(rs->getFace(p)->getVertex(2)->getAtom()).p);
						Atom* a1 = new Atom;
						Atom* a2 = new Atom;
						Atom* a3 = new Atom;
						a1->setPosition(p1);
						a2->setPosition(p2);
						a3->setPosition(p3);
						a1->createBond(*a2);
						a1->createBond(*a3);
						a3->createBond(*a2);
						Molecule* m = new Molecule;
						m->insert(*a1);
						m->insert(*a2);
						m->insert(*a3);					
						System* s = new System;
						s->insert(*m);
						HINFile hin("HIN/RS/face"+IndexToString(p,0.0)+".hin",ios::out);
						hin << *s;
						hin.close();
						delete s;
					}*/
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
			ses->get(rs);
					#ifdef debug_surface_processor
					std::ofstream sesfile("SolventExcludedSurface.log");
					sesfile << *ses;
					sesfile.close();
					::std::cerr << "... ok\ntreat singularities ...\n";
					#endif
			TreatSingularities(ses,rs,probe_radius_);
					#ifdef debug_surface_processor
					rsfile.open("ReducedSurface.clean.log");
					rsfile << *rs;
					rsfile.close();
					sesfile.open("SolventExcludedSurface.clean.log");
					sesfile << *ses;
					sesfile.close();
					::std::cerr << "... ok\ntriangulate surface ...\n";
					#endif
			TriangulatedSurface* surface = Triangulate(ses,rs,density_);
					#ifdef debug_surface_processor
					std::cerr << "... " << surface->triangles.size() << " Dreiecke ... ok\n";
					//cout << *surface;
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
		catch (Exception::GeneralException)
		{
			return false;
		}
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
