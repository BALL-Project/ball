// $Id: reducedSurface.h,v 1.7 2000/12/13 15:14:28 strobel Exp $

//#define debug_rs
//#define print cout

#ifndef BALL_STRUCTURE_REDUCEDSURFACE_H
#define BALL_STRUCTURE_REDUCEDSURFACE_H

#ifndef BALL_STRUCTURE_RSVERTEX_H
#	include <BALL/STRUCTURE/RSVertex.h>
#endif

#ifndef BALL_STRUCTURE_RSEDGE_H
#	include <BALL/STRUCTURE/RSEdge.h>
#endif

#ifndef BALL_STRUCTURE_RSFACE_H
#	include <BALL/STRUCTURE/RSFace.h>
#endif

#ifndef BALL_STRUCTURE_BSDTREE_H
#	include <BALL/STRUCTURE/BSDTree.h>
#endif

#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector3.h>
#endif

#ifndef BALL_MATHS_CIRCLE3_H
#	include <BALL/MATHS/circle3.h>
#endif

#ifndef BALL_MATHS_PLANE3_H
#	include <BALL/MATHS/plane3.h>
#endif

#ifndef BALL_MATHS_LINE3_H
#	include <BALL/MATHS/line3.h>
#endif

#ifndef BALL_MATHS_ANALYTICALGEOMETRY_H
#	include <BALL/MATHS/analyticalGeometry.h>
#endif

#ifndef BALL_MATHS_COMMON_H
#	include <BALL/MATHS/common.h>
#endif

#ifndef BALL_COMMON_EXCEPTION_H
#	include <BALL/COMMON/exception.h>
#endif


#include <vector>
#include <list>
#include <fstream>


namespace BALL
{

//	int HALT;
	/** Generic ReducedSurface Class.
			\\
			{\bf Definition:} \URL{BALL/STRUCTURE/reducedSurface.h}
			\\
	*/
	template <class T>
	class TReducedSurface
	{
		public:

		BALL_CREATE(TReducedSurface)

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
				This method creates a new ReducedSurface object. All components are
				initialized to {\tt (T)0} or {\tt NULL}, respectivly.
		*/
		TReducedSurface()
			:	atom_(), probe_radius_((T)0), tree_(), vertices_(), edges_(), faces_(), r_max_((T)0)
		{
		}

		/**	Copy constructor.
				Create a new ReducedSurface object from another.
				@param reduced_surface the ReducedSurface object to be copied
				@param bool ignored - just for interface consistency
		*/
		TReducedSurface(const TReducedSurface<T>& reduced_surface,bool /* deep */ = true)
			: atom_(reduced_surface.atom_), probe_radius_(reduced_surface.probe_radius_),
				tree_(reduced_surface.tree_), vertices_(reduced_surface.vertices_),
				edges_(reduced_surface.edges_), faces_(reduced_surface.faces_), r_max_(reduced_surface.r_max_)
		{
		}

		/**	Detailled constructor.
				Create a new ReducedSurface object from a list of spheres.
				@param	atom assigned to the vector of atoms
		*/
		TReducedSurface(const std::vector< TSphere3<T> >& spheres, const T& probe_radius)
			:	atom_(), probe_radius_(probe_radius), tree_(),
				vertices_(), edges_(), faces_(), r_max_((T)0)
		{
			#ifdef debug_rs
			print.open("ReducedSurface.log");
			pre = "  ";
			print << "START\n";
			print << pre << "ReducedSurface(" << spheres.size() << ")\n";
			pre += "  ";
			#endif
			std::vector< TVector3<T> > point_list;
			for (Position i = 0; i < spheres.size(); i++)
			{
				atom_.push_back(spheres[i]);
				point_list.push_back(spheres[i].p);
				r_max_ = Maths::max(r_max_,spheres[i].radius);
			}
			tree_ = new TBSDTree<T>(point_list);
			tree_->build();
			#ifdef debug_rs
			for (Position i = 0; i < atom_.size(); i++)
			{
				print << pre << "Atom " << atom_[i] << " eingefügt (" << i << ")\n";
			}
			pre.replace(0,2,"");
			print << pre << "end\n";
			#endif
		}

		/**	Destructor.
				Destructs the ReducedSurface object.
		*/
		virtual ~TReducedSurface()
		{
			#ifdef debug_rs
			print << *this;
			#endif
			clear();
			delete tree_;
			#ifdef debug_rs
			print << pre << "~ReducedSurface()\n";
			print << pre << "end\n";
			print << "ENDE\n";
			#endif
		}
		//@}

		/**	@name	Accessors
		*/
		//@{

		/** Delete all vertices, edges and faces
		*/
		void clear()
		{
			for (Position i = 0; i < vertices_.size(); i++)
			{
				if (vertices_[i] != NULL)
				{
					delete vertices_[i];
				}
			}
			for (Position i = 0; i < edges_.size(); i++)
			{
				delete edges_[i];
			}
			for (Position i = 0; i < faces_.size(); i++)
			{
				delete faces_[i];
			}
			vertices_.clear();
			edges_.clear();
			faces_.clear();
		}

		/** Return the number of atoms.
				@return Size the number of atoms
		*/
		Size numberOfAtoms()
		{
			return atom_.size();
		}

		/** Return the number of rsvertices.
				@return Size the number of rsvertices
		*/
		Size numberOfVertices()
		{
			return vertices_.size();
		}

		/** Return the number of rsedges.
				@return Size the number of rsedges
		*/
		Size numberOfEdges()
		{
			return edges_.size();
		}

		/** Return the number of rsfaces.
				@return Size the number of rsfaces
		*/
		Size numberOfFaces()
		{
			return faces_.size();
		}
		
		/** Return the probe radius
				@return T the radius of the used probe sphere
		*/
		T getProbeRadius()
		{
			return probe_radius_;
		}

		/** Return the maximal radius of all spheres
				@return T the maximal radius
		*/
    T getMaximalRadius()
   	{
   		return r_max_;
   	}

		/** Return the i'th sphere.
				@param	i	the index of the sphere that should be given back
				@return TSpherer3<T>, the i'th sphere
		*/
    TSphere3<T> getSphere(Position i)
    {
    	if (i < atom_.size())
    	{
    		return atom_[i];
    	}
    	else
    	{
    		throw Exception::IndexOverflow(__FILE__, __LINE__,i,atom_.size());
    	}
    }

		/** Return the vertices
				@return vector< TRSVertex<T>* > all vertices
		*/
		std::vector< TRSVertex<T>* > getVertices()
		{
			return vertices_;
		}

		/** Return the i'th rsvertex.
				@param	i	the index of the rsvertex that should be given back
				@return TSpherer3<T>, the i'th rsvertex
		*/
		TRSVertex<T>* getVertex(Position i)
		{
			if (i < vertices_.size())
			{
				return vertices_[i];
			}
			else
			{
				throw Exception::IndexOverflow(__FILE__, __LINE__,i,vertices_.size());
			}
		}

		/** Return the edges
				@return vector< TRSEdge<T>* > all edges
		*/
		std::vector< TRSEdge<T>* > getEdges()
		{
			return edges_;
		}

		/** Return the i'th rsedge.
				@param	i	the index of the rsedge that should be given back
				@return TSpherer3<T>, the i'th rsedge
		*/
		TRSEdge<T>* getEdge(Position i)
		{
			if (i < edges_.size())
			{
				return edges_[i];
			}
			else
			{
				throw Exception::IndexOverflow(__FILE__, __LINE__,i,edges_.size());
			}
		}

		/** Return the faces
				@return vector< TRSFace<T>* > all faces
		*/
		std::vector< TRSFace<T>* > getFaces()
		{
			return faces_;
		}

		/** Return the i'th rsface.
				@param	i	the index of the rsface that should be given back
				@return TSpherer3<T>, the i'th rsface
		*/
		TRSFace<T>* getFace(Position i)
		{
			if (i < faces_.size())
			{
				return faces_[i];
			}
			else
			{
				throw Exception::IndexOverflow(__FILE__, __LINE__,i,faces_.size());
			}
		}

		/** compute the reduced surface
		*/
		void compute()
		{
			#ifdef debug_rs
			print << pre << "computing ...\n";
			pre += "  ";
			#endif
			list<Index> indices;
			list<Index> all_indices;
			for (Position i = 0; i < atom_.size(); i++)
			{
				indices.push_back(i);
				all_indices.push_back(i);
			}
			std::list< TRSVertex<T>* > new_vertices;
			TRSVertex<T>* start_vertex(NULL);
			TRSEdge<T>* start_edge(NULL);
			TRSFace<T>* start_face(NULL);
			std::vector< std::list< TRSVertex<T>* > > vertices(atom_.size());
			Byte start = 1;
			while (start != 0)
			{
				start = getStartPosition(indices,new_vertices,start_vertex,start_edge,start_face,vertices);
				try
				{
					switch (start)
					{
						case 2 :	while (extendComponent(indices,new_vertices,vertices));
											break;
						case 3 :	//cout << "Start-Face gefunden: " << *start_face; //cin >> HALT;
											getRSComponent(indices,start_face,new_vertices,vertices);
											while (extendComponent(indices,new_vertices,vertices));
											break;
						default :	break;
					}
				}
				catch (Exception::GeneralException e)
				{
					if (e.getMessage() == "probe touches four atoms")
					{
						clear();
						indices = all_indices;
						cout << "\nProbeSphere berührt vier Atome, starte neu ...\n\n";
						compute();
						return;
					}
					else
					{
						throw;
					}
				}				
			}
			std::vector< TRSVertex<T>* > v;
			for (Position i = 0; i < vertices_.size(); i++)
			{
				if (vertices_[i] != NULL)
				{
					vertices_[i]->setIndex(v.size());
					v.push_back(vertices_[i]);
				}
			}
			vertices_ = v;
			#ifdef debug_rs
			pre.replace(0,2,"");
			print << pre << "end        // computing\n";
			#endif
		}

		//@}


/************ computing reduced surface ********************************************************************/

		private:

		void getRSComponent(std::list<Index>& indices, TRSFace<T>* face,
												std::list< TRSVertex<T>* >& new_vertices,
												std::vector< std::list< TRSVertex<T>* > >& vertices)
		{
			#ifdef debug_rs
			print << pre << "getRSComponent(" << indices.size() << "," << *face << ","
						<< new_vertices.size() << ")\n";
			pre += "  ";
			#endif
			list< TRSEdge<T>* > new_edges;
			new_edges.push_back(face->getEdge(0));
			new_edges.push_back(face->getEdge(1));
			new_edges.push_back(face->getEdge(2));
			//int counter = 0;
			while (new_edges.size() > 0)
			{
				//cout << "new_edges enthält " << new_edges.size() << " Edges"; //cin >> HALT;
				//std::ofstream file("rs.log");
				//file << *this;
				//file.close();
				//cout << "RS besteht aus " << edges_.size() << " Edges und " << faces_.size() << " Faces";
				//cin >> HALT;
				treatEdge(new_edges.front(),indices,new_edges,new_vertices,vertices);
				//if (counter > 36)
				//{
				//	int in; //cin >> in;
				//}
				//counter++;
			}
			#ifdef debug_rs
			pre.replace(0,2,"");
			print << pre << "end        // getRSComponent(" << indices.size() << "," << *face << ","
						<< new_vertices.size() << ")\n";
			#endif
		}


		void treatEdge(TRSEdge<T>* edge, std::list<Index>& indices,
									 std::list< TRSEdge<T>* >& new_edges, std::list< TRSVertex<T>* >& new_vertices,
									 std::vector< std::list< TRSVertex<T>* > >& vertices)
		{
			// find third atom
			#ifdef debug_rs
			print << pre << "treat " << *edge << "\n";
			pre += "  ";
			#endif
			//cout << "behandle " << *edge; //cin >> HALT;
			TAngle<T> phi;
			TSphere3<T> probe;
			TRSFace<T>* start_face(edge->getFace(0));
			//cout << "starting face: " << *start_face; //cin >> HALT;
			#ifdef debug_rs
			print << pre << "starting face: " << *start_face << "\n";
			#endif
			TRSVertex<T>* vertex1(edge->getVertex(0));
			TRSVertex<T>* vertex2(edge->getVertex(1));
			TRSVertex<T>* vertex3(NULL);
			Index atom1(vertex1->getAtom());
			Index atom2(vertex2->getAtom());
			//cout << "Vertex1: " << *vertex1 << " auf " << atom1 << "\n";
			//cout << "Vertex2: " << *vertex2 << " auf " << atom2; //cin >> HALT;
			Index atom3(thirdAtom(vertex1,vertex2,start_face,probe,phi));
			//cout << "Also:\n";
			//cout << "Vertex1: " << *vertex1 << " auf " << atom1 << "\n";
			//cout << "Vertex2: " << *vertex2 << " auf " << atom2 << "\n";
			//cout << "Vertex3 auf " << atom3; //cin >> HALT;
			if (atom3 == -1)
			{
				throw Exception::GeneralException(__FILE__,__LINE__,"CanNotTreatEdge","no third atom found");
			}
			#ifdef debug_rs
			print << pre << "new face: " << atom1 << "  " << atom2 << "  " << atom3 << "\n";
			#endif
			TSphere3<T> sphere1(atom_[atom1]);
			TSphere3<T> sphere2(atom_[atom2]);
			TSphere3<T> sphere3(atom_[atom3]);
			//cout << "Atom1: " << sphere1 << "\n";
			//cout << "Atom2: " << sphere2 << "\n";
			//cout << "Atom3: " << sphere3; //cin >> HALT;
			// build a new face and two new edges
			vertex3 = new TRSVertex<T>(atom3);
			TRSEdge<T>* edge1;
			TRSEdge<T>* edge2;
			TRSFace<T>* new_face
					= new TRSFace<T>(vertex1,vertex2,vertex3,NULL,NULL,NULL,
													 probe.p,getFaceNormal(sphere1,sphere2,sphere3,probe),false,-1);
			//cout << "neuer Vertex: " << *vertex3; //cin >> HALT;
			//cout << "neue Face: " << *new_face; //cin >> HALT;
			#ifdef debug_rs
			print << pre << "face " << *new_face << " gebaut\n";
			#endif
			//cout << "Teste, ob face schon existiert"; //cin >> HALT;
			TRSFace<T>* test = faceExists(new_face,vertices[vertex1->getAtom()]);
			//cout << "Test erfolgreich"; //cin >> HALT;
			if (test == NULL)
			{
				//cout << "Face existiert noch nicht"; //cin >> HALT;
				vertex3->setIndex(vertices_.size());
				vertices_.push_back(vertex3);
				indices.remove(atom3);
				new_vertices.push_back(vertex3);
				//cout << "Vertex3 in vertices_ gepusht"; //cin >> HALT;
				vertices[atom3].push_back(vertex3);
				//cout << "Vertex3 in vertices[" << atom3 << "] gepusht"; //cin >> HALT;
				edge1 = new TRSEdge<T>();
				edge2 = new TRSEdge<T>();
				//cout << "zwei neue Edges erzeugt"; //cin >> HALT;
				updateFaceAndEdges(vertex1,vertex2,vertex3,probe,6,new_face,edge,edge1,edge2);
				new_face->setIndex(faces_.size());
				faces_.push_back(new_face);
				//cout << "Face und Edges geupdatet:\n";
				//cout << *new_face << "\n";
				//cout << *edge << "\n";
				//cout << *edge1 << "\n";
				//cout << *edge2; //cin >> HALT;
				vertex1->pushFace(new_face);
				vertex2->pushFace(new_face);
				vertex3->pushFace(new_face);
				vertex1->pushEdge(edge2);
				vertex2->pushEdge(edge1);
				vertex3->pushEdge(edge1);
				vertex3->pushEdge(edge2);
				new_edges.push_back(edge1);
				new_edges.push_back(edge2);
				//cout << "Vertices geupdatet:\n";
				//cout << *vertex1 << "\n";
				//cout << *vertex2 << "\n";
				//cout << *vertex3; //cin >> HALT;
				#ifdef debug_rs
				print << pre << "... geupdatet " << *new_face << "\n";
				print << pre << "... in vertices_[" << vertex1->getIndex() << "], vertices_["
							<< vertex2->getIndex() << "], " << "vertices_[" << vertex3->getIndex() << "] gepusht\n";
				print << pre << "neue edges:\n" << pre << *edge1 << "\n" << pre << *edge2
							<< "\n" << pre << "... in new_edges gepusht\n";
				#endif
			}
			else
			{
				#ifdef debug_rs
				print << pre << "... existiert schon: " << *test << "\n";
				#endif
				//cout << "Face existiert schon: " << *test; //cin >> HALT;
				Index i = 0;
				TRSEdge<T>* test_edge(test->getEdge(0));
				while (test_edge->similar(*edge) == false)
				{
					i++;
					if (i > 2)
					{
						throw Exception::GeneralException(__FILE__,__LINE__,
																							"CanNotTreatEdge","corresponding edge in face not found");
					}
					test_edge = test->getEdge(i);
				}
				//cout << "Korrespondierende Edge gefunden: " << *test_edge; //cin >> HALT;
				TRSVertex<T>* test_vertex1 = test_edge->getVertex(0);
				TRSVertex<T>* test_vertex2 = test_edge->getVertex(1);
				//cout << "Vertices der korrespondierenden Edge:\n";
				//cout << "  " << *test_vertex1 << "\n";
				//cout << "  " << *test_vertex2; //cin >> HALT;
				if (test_vertex1->similar(*vertex1))
				{
					//cout << "Korrespondierende Vertices:\n";
					//cout << *vertex1 << " - " << *test_vertex1 << "\n";
					//cout << *vertex2 << " - " << *test_vertex2; //cin >> HALT;
					if (*vertex1 != *test_vertex1)
					{
						//cout << "erstes Paar verschieden, vereinige und ersetze"; //cin >> HALT;
						vertex1->join(*test_vertex1);
						test_vertex1->substitute(vertex1);
						vertices_[test_vertex1->getIndex()] = NULL;
						new_vertices.remove(test_vertex1);
						vertices[test_vertex1->getAtom()].remove(test_vertex1);
						//cout << *vertex1; //cin >> HALT;
						//cout << "lösche " << *test_vertex1; //cin >> HALT;
						delete test_vertex1;
					}
					if (*vertex2 != *test_vertex2)
					{
						//cout << "zweites Paar verschieden, vereinige und ersetze"; //cin >> HALT;
						vertex2->join(*test_vertex2);
						test_vertex2->substitute(vertex2);
						vertices_[test_vertex2->getIndex()] = NULL;
						new_vertices.remove(test_vertex2);
						vertices[test_vertex2->getAtom()].remove(test_vertex2);
						//cout << *vertex2; //cin >> HALT;
						//cout << "lösche " << *test_vertex2; //cin >> HALT;
						delete test_vertex2;
					}
					//cout << "lösche korrespondierende Edge aus Vertex1 und Vertex2"; //cin >> HALT;
					vertex1->deleteEdge(test_edge);
					vertex2->deleteEdge(test_edge);
				}
				else
				{
					//cout << "Korrespondierende Vertices:\n";
					//cout << *vertex1 << " - " << *test_vertex2 << "\n";
					//cout << *vertex2 << " - " << *test_vertex1; //cin >> HALT;
					if (*vertex1 != *test_vertex2)
					{
						//cout << "erstes Paar verschieden, vereinige und ersetze"; //cin >> HALT;
						vertex1->join(*test_vertex2);
						test_vertex2->substitute(vertex1);
						vertices_[test_vertex2->getIndex()] = NULL;
						new_vertices.remove(test_vertex2);
						vertices[test_vertex2->getAtom()].remove(test_vertex2);
						//cout << *vertex1; //cin >> HALT;
						//cout << "lösche " << *test_vertex2; //cin >> HALT;
						delete test_vertex2;
					}
					if (*vertex2 != *test_vertex1)
					{
						//cout << "zweites Paar verschieden, vereinige und ersetze"; //cin >> HALT;
						vertex2->join(*test_vertex1);
						test_vertex1->substitute(vertex2);
						vertices_[test_vertex1->getIndex()] = NULL;
						new_vertices.remove(test_vertex1);
						vertices[test_vertex1->getAtom()].remove(test_vertex1);
						//cout << *vertex2; //cin >> HALT;
						//cout << "lösche " << *test_vertex1; //cin >> HALT;
						delete test_vertex1;
					}
					//cout << "lösche korrespondierende Edge aus Vertex1 und Vertex2"; //cin >> HALT;
					vertex1->deleteEdge(test_edge);
					vertex2->deleteEdge(test_edge);
				}
				//cout << "ersetze korrespondierende Edge in existierender Face"; //cin >> HALT;
				test->setEdge(i,edge);
				//cout << *test; //cin >> HALT;
				#ifdef debug_rs
				print << pre << i << ". edge ersetzt: " << *test << "\n";
				#endif
				//cout << "lösche korrespondierende Edge aus new_edges"; //cin >> HALT;
				new_edges.remove(test_edge);
				delete new_face;
				delete test_edge;
				delete vertex3;
				//cout << "erzeugte face gelöscht"; //cin >> HALT;
				//cout << "korrespondierende Edge gelöscht"; //cin >> HALT;
				//cout << "erzeugter vertex gelöscht"; //cin >> HALT;
				#ifdef debug_rs
				print << pre << "konstruierte face gelöscht\n";
				#endif
				new_face = test;
			}
			// update edge
			#ifdef debug_rs
			print << pre << "updating " << *edge << ":\n";
			#endif
			//cout << "update " << *edge; //cin >> HALT;
			TCircle3<T> circle1;
			TCircle3<T> circle2;
			TCircle3<T> circle3;
			getCircles(atom1,atom2,circle1,circle2,circle3);
			TVector3<T> ip1;
			TVector3<T> ip2;
			TLine3<T> line(sphere1.p,sphere2.p,TLine3<T>::FORM__TWO_POINTS);
			bool singular(GetIntersection(probe,line,ip1,ip2));
			if (Maths::isLess(ip1.getDistance(sphere2.p),ip2.getDistance(sphere2.p)))
			{
				ip1.swap(ip2);
			}
			edge->set(vertex1,vertex2,start_face,new_face,circle1.p,circle1.radius,phi,
								circle2,circle3,ip1,ip2,singular,edges_.size());
			//cout << "erfolgreich"; //cin >> HALT;
			#ifdef debug_rs
			print << pre << "... " << *edge << "\n";
			#endif
			edges_.push_back(edge);
			new_edges.remove(edge);
			#ifdef debug_rs
			pre.replace(0,2,"");
			print << pre << "end        // treat " << *edge << "\n";
			#endif
			//cout << "Edge aus new_edges gelöscht, in edges_ gepusht"; //cin >> HALT;
			//cout << "Ergebnis:\n"
			//		 << *start_face << "\n  "
			//		 << *start_face->getEdge(0) << "\n  "
			//		 << *start_face->getEdge(1) << "\n  "
			//		 << *start_face->getEdge(2) << "\n"
			//		 << *new_face << "\n  "
			//		 << *new_face->getEdge(0) << "\n  "
			//		 << *new_face->getEdge(1) << "\n  "
			//		 << *new_face->getEdge(2) << "\n"
			//		 << *vertex1 << "\n" << *vertex2 << "\n"
			//		 << *start_face->thirdVertex(vertex1,vertex2) << "\n"
			//		 << *new_face->thirdVertex(vertex1,vertex2) << "\n";
			//cin >> HALT;
		}
	

		bool extendComponent(std::list<Index>& indices, std::list< TRSVertex<T>* >& new_vertices,
												 std::vector< std::list< TRSVertex<T>* > >& vertices)
		{
			#ifdef debug_rs
			print << pre << "extendComponent(" << indices.size() << "," << new_vertices.size() << ")\n";
			pre += "  ";
			#endif
			if (new_vertices.size() == 0)
			{
				#ifdef debug_rs
				pre.replace(0,2,"");
				print << pre << "end ... false\n";
				#endif
				return false;
			}
			TRSFace<T>* face = NULL;
			TRSVertex<T>* vertex1 = new_vertices.front();
			Index atom1(vertex1->getAtom());
			list<Index> atom_list(neighboursOfOneAtom(atom1,indices));
			#ifdef debug_rs
			print << pre << "Atom1: " << atom1 << ", Nachbarn: " << atom_list.size() << "\n";
			#endif
			list<Index>::iterator i = atom_list.begin();
			while (i != atom_list.end())
			{
				Index atom2 = *i;
				list< pair< Index,TSphere3<T> > > candidates(findThirdAtom(atom1,atom2,atom_list));
				#ifdef debug_rs
				print << pre << "Atom2: " << atom2 << ", Nachbarn: " << candidates.size() << "\n";
				#endif
				if (candidates.size() == 0)
				{
					TRSVertex<T>* vertex2 = new TRSVertex<T>(atom2);
					TRSEdge<T>* edge = createFreeEdge(vertex1,vertex2,atom_list);
					if (edge != NULL)
					{
						edge->setIndex(edges_.size());
						edges_.push_back(edge);
						vertex1->pushEdge(edge);
						vertex2->pushEdge(edge);
						vertex2->setIndex(vertices_.size());
						vertices_.push_back(vertex2);
						indices.remove(atom2);
						vertices[atom2].push_back(vertex2);
						new_vertices.push_back(vertex1);
						new_vertices.push_back(vertex2);
						#ifdef debug_rs
						print << pre << "freie edge erzeugt: " << *edge << "\n";
						pre.replace(0,2,"");
						print << pre << "end ... true\n";
						#endif
						return true;
					}
					else
					{
						#ifdef debug_rs
						print << pre << "bilden keine freie edge\n";
						#endif
						delete vertex2;
					}
				}
				list< pair< Index,TSphere3<T> > >::iterator j = candidates.begin();
				//if (i != atom_list.end())
				//{
					while (j != candidates.end())
					{
						Index atom3 = j->first;
						TSphere3<T> probe = j->second;
						#ifdef debug_rs
						print << pre << "Atom3: " << atom3 << ", probe: " << probe << " ...\n";
						#endif
						if (checkProbe(probe) == true)
						{
							#ifdef debug_rs
							print << pre << "... passt\n";
							#endif
							face = new TRSFace<T>();
							TRSEdge<T>* edge1 = new TRSEdge<T>();
							TRSEdge<T>* edge2 = new TRSEdge<T>();
							TRSEdge<T>* edge3 = new TRSEdge<T>();
							TRSVertex<T>* vertex2 = new TRSVertex<T>(atom2);
							TRSVertex<T>* vertex3 = new TRSVertex<T>(atom3);
							vertices[atom2].push_back(vertex2);
							vertices[atom3].push_back(vertex3);
							updateFaceAndEdges(vertex1,vertex2,vertex3,probe,7,face,edge1,edge2,edge3);
							face->setIndex(faces_.size());
							faces_.push_back(face);
							vertex2->setIndex(vertices_.size());
							vertices_.push_back(vertex2);
							indices.remove(atom2);
							vertex3->setIndex(vertices_.size());
							vertices_.push_back(vertex3);
							indices.remove(atom3);
							vertex1->pushFace(face);
							vertex2->pushFace(face);
							vertex3->pushFace(face);
							vertex1->pushEdge(edge1);
							vertex2->pushEdge(edge1);
							vertex2->pushEdge(edge2);
							vertex3->pushEdge(edge2);
							vertex3->pushEdge(edge3);
							vertex1->pushEdge(edge3);
							new_vertices.push_back(vertex1);
							new_vertices.push_back(vertex2);
							new_vertices.push_back(vertex3);
							i = atom_list.end();
							j = candidates.end();
						}
						else
						{
							#ifdef debug_rs
							print << pre << "... passt nicht\n";
							#endif
							j++;
						}
					} // while j
				//}
				if (i != atom_list.end())
				{
					i++;
				}
			} // while i
			new_vertices.pop_front();
			if (face != NULL)
			{
				getRSComponent(indices,face,new_vertices,vertices);
			}
			#ifdef debug_rs
			pre.replace(0,2,"");
			print << pre << "end ... true\n";
			#endif
			return true;
		}


		Index thirdAtom(TRSVertex<T>* vertex1, TRSVertex<T>* vertex2, TRSFace<T>* face,
										TSphere3<T>& probe, TAngle<T>& phi)
		{
			#ifdef debug_rs
			pre += "  ";
			#endif
			Index atom1(vertex1->getAtom());
			Index atom2(vertex2->getAtom());
			TSphere3<T> sphere1(atom_[atom1]);
			TSphere3<T> sphere2(atom_[atom2]);
			sphere1.radius += probe_radius_;
			sphere2.radius += probe_radius_;
			TCircle3<T> circle;
			GetIntersection(sphere1,sphere2,circle);
			list<Index> atom_list = tree_->get(circle.p,circle.radius+r_max_+probe_radius_);
			if (atom_list.size() == 0)
			{
				#ifdef debug_rs
				print << "\n\n" << *this << "\n\n";
				print << *vertex1 << "\n";
				print << *vertex2 << "\n";
				#endif
				throw Exception::GeneralException(__FILE__,__LINE__,"CanNotFindThirdAtom","no atom close enougth");
			}
 			list< pair< Index,TSphere3<T> > > candidates = findThirdAtom(atom1,atom2,atom_list);
 			list< pair< Index,TSphere3<T> > >::iterator k;
 			Index back(-1);
			#ifdef debug_rs
			print << pre << atom1 << "  " << atom2 << "\n";
			print << pre << *face << "\n";
			print << pre << "Kandidaten: (" << candidates.size() << ")\n";
			#endif
			TAngle<T> old_angle(2*Constants::PI,true);
			TAngle<T> new_angle;
			TAngle<T> pi(Constants::PI,true);
			TVector3<T> norm = atom_[atom1].p-atom_[atom2].p;
			TVector3<T> test_vector = face->getNormal()%norm;
			if (Maths::isLess(test_vector*atom_[face->thirdVertex(vertex1,vertex2)->getAtom()].p,
												test_vector*atom_[atom1].p))
			{
				norm.negate();
			#ifdef debug_rs
			print << pre << "  Dreahachse: " << atom1 << " --> " << atom2 << "\n";
			#endif
			}
			else
			{
			#ifdef debug_rs
			print << pre << "  Dreahachse: " << atom2 << " --> " << atom1 << "\n";
			#endif
			}
			TVector3<T> v1 = face->getCenter()-circle.p;
			for (k = candidates.begin(); k != candidates.end(); k++)
			{
				if (((k->first == face->getVertex(0)->getAtom()) ||
						 (k->first == face->getVertex(1)->getAtom()) ||
						 (k->first == face->getVertex(2)->getAtom())   ) &&
						Maths::isGreaterOrEqual(face->getNormal()*k->second.p,face->getNormal()*sphere1.p) )
				{
					// found the starting face
				}
				else
				{
					#ifdef debug_rs
					print << pre << "{" << k->first << "," << k->second << "} ... ";
					#endif
					TVector3<T> v2 = k->second.p-circle.p;
					new_angle = getOrientedAngle(v1,v2,norm)+pi;
					/*new_angle = getOrientedAngle(v1,v2,norm);
					if (Maths::isLess(new_angle.toRadian(),(T)0))
					{
						#ifdef debug_rs
						print << new_angle << " -> ";
						#endif
						new_angle += pi;
						new_angle += pi;
					}*/
					#ifdef debug_rs
					print << new_angle << "\n";
					#endif
					if (new_angle == TAngle<T>(0,true))
					{
						atom_[k->first].radius -= 0.1;
						throw Exception::GeneralException(__FILE__,__LINE__,
																							"can't compute RS","probe touches four atoms");
					}
					if (new_angle < old_angle)
					{
						old_angle = new_angle;
						back = k->first;
						probe = k->second;
						phi = new_angle;
					}
				}
			}
			#ifdef debug_rs
			pre.replace(0,2,"");
			#endif
			return back;
		}


/************ finding a start position *********************************************************************/

		Byte getStartPosition(list<Index>& indices, list< TRSVertex<T>* >& new_vertices,
													TRSVertex<T>*& vertex, TRSEdge<T>*& edge, TRSFace<T>*& face,
													std::vector< std::list< TRSVertex<T>* > >& vertices)
		{
			face = findFirstFace(indices);
			if (face != NULL)
			{
				TRSVertex<T>* vertex1 = face->getVertex(0);
				TRSVertex<T>* vertex2 = face->getVertex(1);
				TRSVertex<T>* vertex3 = face->getVertex(2);
				new_vertices.push_back(vertex1);
				new_vertices.push_back(vertex2);
				new_vertices.push_back(vertex3);
				vertices[vertex1->getAtom()].push_back(vertex1);
				vertices[vertex2->getAtom()].push_back(vertex2);
				vertices[vertex3->getAtom()].push_back(vertex3);
				#ifdef debug_rs
				print << pre << "starting face: " << vertex1->getAtom() << " " << vertex2->getAtom() << " "
							<< vertex3->getAtom() << "\n";
				#endif
				return 3;
			}
			edge = findFirstEdge(indices);
			if (edge != NULL)
			{
				TRSVertex<T>* vertex1 = edge->getVertex(0);
				TRSVertex<T>* vertex2 = edge->getVertex(1);
				new_vertices.push_back(vertex1);
				new_vertices.push_back(vertex2);
				vertices[vertex1->getAtom()].push_back(vertex1);
				vertices[vertex2->getAtom()].push_back(vertex2);
				return 2;
			}
			vertex = findFirstVertex(indices);
			if (vertex != NULL)
			{
				new_vertices.push_back(vertex);
				vertices[vertex->getAtom()].push_back(vertex);
				return 1;
			}
			return 0;
		}

/*----------- finding a first face ------------------------------------------------------------------------*/

		TRSFace<T>* findFirstFace(list<Index>& indices)
		{
			for (Byte direction = 0; direction < 3; direction++)
			{
				for (Byte extrem = 0; extrem < 1; extrem++)
				{
					TRSFace<T>* face = findFace(indices,direction,extrem);
					if (face != NULL)
					{
						return face;
					}
				}
			}
			return NULL;
		}


		TRSFace<T>* findFace(list<Index>& indices, Byte direction, Byte extrem)
		{
			if (indices.size() == 0)
			{
				return NULL;
			}
			Index a1 = findFirstAtom(indices,direction,extrem);
			list<Index> atom_list = tree_->get(atom_[a1].p,atom_[a1].radius+r_max_+2*probe_radius_);
			Index a2 = findSecondAtom(a1,atom_list,direction,extrem);
			if (a2 == -1)
			{
				return NULL;
			}
			TSphere3<T> s1(atom_[a1].p,atom_[a1].radius+probe_radius_);
			TSphere3<T> s2(atom_[a2].p,atom_[a2].radius+probe_radius_);
			TCircle3<T> circle;
			GetIntersection(s1,s2,circle);
			atom_list = tree_->get(circle.p,circle.radius+r_max_+probe_radius_);
			list< pair< Index,TSphere3<T> > > candidates = findThirdAtom(a1,a2,atom_list);
			if (candidates.size() == 0)
			{
				return NULL;
			}
			list< pair< Index,TSphere3<T> > >::iterator i = candidates.begin();
			Index a3 = candidates.begin()->first;
			TSphere3<T> probe = candidates.begin()->second;
			while (checkProbe(probe) == false)
			{
				i++;
				if (i == candidates.end())
				{
					return NULL;
				}
				a3 = i->first;
				probe = i->second;
			}
			TRSVertex<T>* vertex1 = new TRSVertex<T>(a1);
			TRSVertex<T>* vertex2 = new TRSVertex<T>(a2);
			TRSVertex<T>* vertex3 = new TRSVertex<T>(a3);
			TRSEdge<T>* e1 = new TRSEdge<T>();
			TRSEdge<T>* e2 = new TRSEdge<T>();
			TRSEdge<T>* e3 = new TRSEdge<T>();
			TRSFace<T>* face = new TRSFace<T>();
			updateFaceAndEdges(vertex1,vertex2,vertex3,probe,7,face,e1,e2,e3);
			face->setIndex(faces_.size());
			faces_.push_back(face);
			vertex1->pushFace(face);
			vertex2->pushFace(face);
			vertex3->pushFace(face);
			vertex1->pushEdge(e1);
			vertex2->pushEdge(e1);
			vertex2->pushEdge(e2);
			vertex3->pushEdge(e2);
			vertex3->pushEdge(e3);
			vertex1->pushEdge(e3);
			indices.remove(a1);
			indices.remove(a2);
			indices.remove(a3);
			vertex1->setIndex(vertices_.size());
			vertices_.push_back(vertex1);
			vertex2->setIndex(vertices_.size());
			vertices_.push_back(vertex2);
			vertex3->setIndex(vertices_.size());
			vertices_.push_back(vertex3);
			return face;
		}

/*----------- finding a first edge ------------------------------------------------------------------------*/

		TRSEdge<T>* findFirstEdge(list<Index>& indices)
		{
			for (Byte direction = 0; direction < 3; direction++)
			{
				for (Byte extrem = 0; extrem < 1; extrem++)
				{
					TRSEdge<T>* edge = findEdge(indices,direction,extrem);
					if (edge != NULL)
					{
						return edge;
					}
				}
			}
			return NULL;
		}


		TRSEdge<T>* findEdge(list<Index>& indices, Byte direction, Byte extrem)
		{
			if (indices.size() == 0)
			{
				return NULL;
			}
			Index a1 = findFirstAtom(indices,direction,extrem);
			list<Index> atom_list = tree_->get(atom_[a1].p,atom_[a1].radius+r_max_+2*probe_radius_);
			Index a2 = findSecondAtom(a1,atom_list,direction,extrem);
			if (a2 == -1)
			{
				return NULL;
			}
			TRSVertex<T>* vertex1 = new TRSVertex<T>(a1);
			TRSVertex<T>* vertex2 = new TRSVertex<T>(a2);
			atom_list = neighboursOfTwoAtoms(a1,a2,atom_list);
			TRSEdge<T>* edge = createFreeEdge(vertex1,vertex2,atom_list);
			if (edge != NULL)
			{
				edge->setIndex(edges_.size());
				edges_.push_back(edge);
				vertex1->pushEdge(edge);
				vertex1->setIndex(vertices_.size());
				vertices_.push_back(vertex1);
				indices.remove(a1);
				vertex2->pushEdge(edge);
				vertex2->setIndex(vertices_.size());
				vertices_.push_back(vertex2);
				indices.remove(a2);
			}
			else
			{
				delete vertex1;
				delete vertex2;
			}
			return edge;
		}

/*----------- finding a first vertex ----------------------------------------------------------------------*/

		TRSVertex<T>* findFirstVertex(list<Index>& indices)
		{
			if (indices.size() == 0)
			{
				return NULL;
			}
			for (Byte direction = 0; direction < 3; direction++)
			{
				for (Byte extrem = 0; extrem < 1; extrem++)
				{
					Index v = findFirstAtom(indices,direction,extrem);
					if (v != -1)
					{
						TRSVertex<T>* vertex = new TRSVertex<T>(v);
						vertex->setIndex(vertices_.size());
						vertices_.push_back(vertex);
						return vertex;
					}
				}
			}
			return NULL;
		}

/*----------- some utilities ------------------------------------------------------------------------------*/

		Index findFirstAtom(std::list<Index> indices, Byte direction, Byte extrem)
		{
			TSphere3<T> next_atom(atom_[*indices.begin()]);
			T dir_ex = ((extrem == 0) ? next_atom.p[direction]-next_atom.radius
																: next_atom.p[direction]+next_atom.radius);
			Index a_ex = *indices.begin();
			std::list<Index>::iterator i;
			for (i = indices.begin(); i != indices.end(); i++)
			{
				next_atom = atom_[*i];
				T extremum = ((extrem == 0) ? next_atom.p[direction]-next_atom.radius
																		: next_atom.p[direction]+next_atom.radius);
				if (((extrem == 0) && Maths::isLess(extremum,dir_ex)) ||
						((extrem != 0) && Maths::isGreater(extremum,dir_ex)))
				{
					dir_ex = extremum;
					a_ex = *i;
				}
			}
			return a_ex;
		}


		Index findSecondAtom(Index atom1, std::list<Index> atom_list, Byte direction, Byte extrem)
		{
			list<Index> neighbour = neighboursOfOneAtom(atom1,atom_list);
			if (neighbour.size() == 0)
			{
				return -1;
			}
			TSphere3<T> first_atom(atom_[atom1]);
			first_atom.radius += probe_radius_;
			TCircle3<T> intersection_circle;
			T x_min = ((extrem == 0) ? first_atom.p[direction]+first_atom.radius
															 : first_atom.p[direction]-first_atom.radius);
			Index a_min = -1;
			list<Index>::iterator l1;
			for (l1 = neighbour.begin(); l1 != neighbour.end(); l1++)
			{
				TSphere3<T> next_atom(atom_[*l1]);
				next_atom.radius += probe_radius_;
				if (GetIntersection(first_atom,next_atom,intersection_circle))
				{
					T next_min = getCircleExtremum(intersection_circle, direction, extrem);
					if (((extrem == 0) && Maths::isLess(next_min,x_min)) ||
							((extrem != 0) && Maths::isGreater(next_min,x_min)))
					{
						x_min = next_min;
						a_min = *l1;
					}
				}
			}
			return a_min;
		}


		list< pair< Index,TSphere3<T> > >
				findThirdAtom(Index atom1, Index atom2, list<Index> /*atom_list*/ third)
		{
			#ifdef debug_rs
			pre += "    ";
			print << pre << atom1 << ", " << atom2 << ", ...\n";
			#endif
			//list<Index> third = neighboursOfTwoAtoms(atom1,atom2,atom_list);
			list< pair< Index,TSphere3<T> > > back;
			if (third.size() == 0)
			{
				throw Exception::GeneralException(__FILE__,__LINE__,"CanNotFindThirdAtom","no third atom found");
 				//return back;
			}
			pair< Index,TSphere3<T> > candidate;
			list<Index>::iterator l = third.begin();
			while (l != third.end())
			{
				#ifdef debug_rs
				print << pre << "  ... " << *l << ": ";
				#endif
				TVector3<T> center1, center2;
				if (centerOfProbe(atom1,atom2,*l,center1,center2))
				{
					TSphere3<T> probe(center1,probe_radius_);
					candidate.first = *l;
					candidate.second = probe;
					#ifdef debug_rs
					print << probe.p << " / ";
					#endif
					back.push_back(candidate);
					probe.p = center2;
					candidate.first = *l;
					candidate.second = probe;
					#ifdef debug_rs
					print << probe.p;
					#endif
					back.push_back(candidate);
				}
				#ifdef debug_rs
				print << "\n";
				#endif
				l++;
			}
			#ifdef debug_rs
			pre.replace(0,4,"");
			#endif
			return back;
		}


		list<Index> neighboursOfOneAtom(Index atom0, list<Index> atom_list)
		{
			list<Index> back;
			TSphere3<T> first_atom(atom_[atom0]);
			list<Index>::iterator i;
			for (i = atom_list.begin(); i != atom_list.end(); i++)
			{
				if (*i != atom0)
				{
					TSphere3<T> next_atom(atom_[*i]);
					T dist = next_atom.p.getDistance(first_atom.p);
					T max_dist = next_atom.radius+first_atom.radius+2*probe_radius_;
					if (Maths::isLessOrEqual(dist,max_dist))
					{
						back.push_back(*i);
					}
				}
			}
			return back;
		}


		list<Index> neighboursOfTwoAtoms(Index atom1, Index atom2, list<Index> atom_list)
		{
			list<Index> neighbour;
			TSphere3<T> first_atom(atom_[atom1]);
			first_atom.radius += probe_radius_;
			TSphere3<T> second_atom(atom_[atom2]);
			second_atom.radius += probe_radius_;
			TCircle3<T> intersection_circle;
			GetIntersection(first_atom,second_atom,intersection_circle);
			list<Index>::iterator i;
			for (i = atom_list.begin(); i != atom_list.end(); i++)
			{
				if ((*i != atom1) && (*i != atom2))
				{
					T dist = atom_[*i].p.getDistance(intersection_circle.p);
					T max_dist = probe_radius_+intersection_circle.radius+atom_[*i].radius;
					if (Maths::isLess(dist,max_dist))
					{
						neighbour.push_back(*i);
					}
				}
			}
			return neighbour;
		}


    T getCircleExtremum(const TCircle3<T>& circle, Position direction, Position extrem)
    {
      T min, max;
      TVector3<T> norm2 = TVector3<T>(circle.n.x * circle.n.x, circle.n.y * circle.n.y,
																			circle.n.z * circle.n.z);
      switch (direction)
      {
        case 0 :
        	if (Maths::isZero(circle.n.y) && Maths::isZero(circle.n.z))
					{
						min = max = circle.p.x;
					}
					else
					{
						T x_norm = norm2.y + norm2.z;
						x_norm /= norm2.x+x_norm;
						x_norm = circle.radius * sqrt(x_norm);
						min = (circle.p.x) - x_norm;
						max = (circle.p.x) + x_norm;
					}
					break;
        case 1 :
					if (Maths::isZero(circle.n.x) && Maths::isZero(circle.n.z))
					{
						min = max = circle.p.y;
					}
					else
					{
						T y_norm = norm2.x + norm2.z;
						y_norm /= norm2.y + y_norm;
						y_norm = circle.radius * sqrt(y_norm);
						min = (circle.p.y)-y_norm;
						max = (circle.p.y)+y_norm;
					}
					break;
        case 2 :
					if (Maths::isZero(circle.n.x) && Maths::isZero(circle.n.y))
					{
						min = max = circle.p.z;
					}
					else
					{
						T z_norm = norm2.x + norm2.y;
						z_norm /= norm2.z + z_norm;
						z_norm = circle.radius * sqrt(z_norm);
						min = circle.p.z - z_norm;
						max = circle.p.z + z_norm;
					}
					break;
			}
      if (extrem == 0)
      {
        return Maths::min(min, max);
			}
      else
      {
        return Maths::max(min, max);
			}
		}


/************ creating / updating edges / faces ************************************************************/

		TRSEdge<T>* createFreeEdge(TRSVertex<T>* vertex1, TRSVertex<T>* vertex2, std::list<Index> neighbours)
		{
			TCircle3<T> circle1;
			TCircle3<T> circle2;
			TCircle3<T> circle3;
			if (getCircles(vertex1->getAtom(),vertex2->getAtom(),circle1,circle2,circle3) &&
					Maths::isGreater(circle1.radius,probe_radius_)		 )
			{
				TPlane3<T> plane(circle1.p,circle1.n);
				for (list<Index>::iterator i = neighbours.begin(); i != neighbours.end(); i++)
				{
					TCircle3<T> test_circle;
					TSphere3<T> sphere(atom_[*i].p,atom_[*i].radius+probe_radius_);
					if (GetIntersection(sphere,plane,test_circle))
					{
						if (Maths::isGreaterOrEqual(Maths::abs(test_circle.radius-circle1.radius),
																				test_circle.p.getDistance(circle1.p)          ) )
						{
							return NULL;
						}
					}
				}
				TVector3<T> vector(0,0,0);
				TRSEdge<T>* edge = new TRSEdge<T>(vertex1,vertex2,NULL,NULL,circle1.p,circle1.radius,
																					TAngle<T>(2*Constants::PI,true),circle2,circle3,
																					vector,vector,false,-1);
				return edge;
			}
			return NULL;
		}


		bool getCircles(Index atom1, Index atom2,
										TCircle3<T>& circle1, TCircle3<T>& circle2, TCircle3<T>& circle3)
		{
			TSphere3<T> sphere1(atom_[atom1]);
			TSphere3<T> sphere2(atom_[atom2]);
			sphere1.radius += probe_radius_;
			sphere2.radius += probe_radius_;
			if (GetIntersection(sphere1,sphere2,circle1))
			{
				T ratio1(atom_[atom1].radius/sphere1.radius);
				circle2.radius = circle1.radius*ratio1;
				circle2.p = sphere1.p+(circle1.p-sphere1.p)*(ratio1);
				T ratio2(atom_[atom2].radius/sphere2.radius);
				circle3.radius = circle1.radius*ratio2;
				circle3.p = sphere2.p+(circle1.p-sphere2.p)*(ratio2);
				return true;
			}
			return false;
		}


		TVector3<T> getFaceNormal(const TSphere3<T>& atom1, const TSphere3<T>& atom2, const TSphere3<T>& atom3,
															const TSphere3<T>& probe)
		{
			TPlane3<T> plane(atom1.p,atom2.p,atom3.p);
			TVector3<T> norm(plane.n);
			if (Maths::isLess(norm*probe.p,norm*atom1.p))
			{
				norm.negate();
			}
			norm.normalize();
			return norm;
		}


		bool updateFaceAndEdges(TRSVertex<T>* v1, TRSVertex<T>* v2, TRSVertex<T>* v3,
														const TSphere3<T>& probe, Byte mode,
														TRSFace<T>* f, TRSEdge<T>* e1, TRSEdge<T>* e2, TRSEdge<T>* e3)
		{
			TVector3<T> vector(0,0,0);
			TAngle<T> angle(0,true);
			TCircle3<T> circle(vector,vector,0);
			if ((mode & 1) == 1)
			{
				e1->set(v1,v2,f,NULL,vector,(T)0,angle,circle,circle,vector,vector,false,-1);
			}
			if ((mode & 2) == 2)
			{
				e2->set(v2,v3,f,NULL,vector,(T)0,angle,circle,circle,vector,vector,false,-1);
			}
			if ((mode & 4) == 4)
			{
				e3->set(v3,v1,f,NULL,vector,(T)0,angle,circle,circle,vector,vector,false,-1);
			}
			TVector3<T> norm(f->getNormal());
			if (norm == TVector3<T>(0,0,0))
			{
				norm = getFaceNormal(atom_[v1->getAtom()],atom_[v2->getAtom()],atom_[v3->getAtom()],probe);
			}
			TPlane3<T> plane(atom_[v1->getAtom()].p,atom_[v2->getAtom()].p,atom_[v3->getAtom()].p);
			TCircle3<T> dummy;
			f->set(v1,v2,v3,e1,e2,e3,probe.p,norm,GetIntersection(probe,plane,dummy),-1);
			return true;
		}


		TRSFace<T>* faceExists(TRSFace<T>* face, std::list< TRSVertex<T>* >& vertices)
		{
			std::list< TRSVertex<T>* >::iterator v;
			for (v = vertices.begin(); v != vertices.end(); v++)
			{
				TRSFace<T>* f = (*v)->has(face);
				if (f != NULL)
				{
					return f;
				}
			}
			return NULL;
		}


/************ finding a probe sphere ***********************************************************************/

		bool centerOfProbe(Index a1, Index a2, Index a3, TVector3<T>& c1, TVector3<T>& c2)
		{
			TSphere3<double> s1(TVector3<double>((double)atom_[a1].p.x,
																					 (double)atom_[a1].p.y,
																					 (double)atom_[a1].p.z ),
													(double)atom_[a1].radius+(double)probe_radius_);
			TSphere3<double> s2(TVector3<double>((double)atom_[a2].p.x,
																					 (double)atom_[a2].p.y,
																					 (double)atom_[a2].p.z ),
													(double)atom_[a2].radius+(double)probe_radius_);
			TSphere3<double> s3(TVector3<double>((double)atom_[a3].p.x,
																					 (double)atom_[a3].p.y,
																					 (double)atom_[a3].p.z ),
													(double)atom_[a3].radius+(double)probe_radius_);
			TCircle3<double> intersection_circle1;
			TCircle3<double> intersection_circle2;
			if (GetIntersection(s1,s2,intersection_circle1))
			{
				if (GetIntersection(s1,s3,intersection_circle2))
				{
					TPlane3<double> plane1(intersection_circle1.p,intersection_circle1.n);
					TPlane3<double> plane2(intersection_circle2.p,intersection_circle2.n);
					TLine3<double> line;
					if (GetIntersection(plane1,plane2,line))
					{
						TVector3<double> center1;
						TVector3<double> center2;
						if (GetIntersection(line,s1,center1,center2))
						{
							c1 = TVector3<T>((T)center1.x,(T)center1.y,(T)center1.z);
							c2 = TVector3<T>((T)center2.x,(T)center2.y,(T)center2.z);
							if (s1.has(center1,true) == false)
							{
								return false;
							}
							if (s1.has(center2,true) == false)
							{
								return false;
							}
							if (s2.has(center1,true) == false)
							{
								return false;
							}
							if (s2.has(center2,true) == false)
							{
								return false;
							}
							if (s3.has(center1,true) == false)
							{
								return false;
							}
							if (s3.has(center2,true) == false)
							{
								return false;
							}
							return true;
						}
					}
				}
			}
			return false;
		}
		/*bool centerOfProbe(Index a1, Index a2, Index a3, TVector3<T>& c1, TVector3<T>& c2)
		{
			TSphere3<T> s1(atom_[a1].p,atom_[a1].radius+probe_radius_);
			TSphere3<T> s2(atom_[a2].p,atom_[a2].radius+probe_radius_);
			TSphere3<T> s3(atom_[a3].p,atom_[a3].radius+probe_radius_);
			TCircle3<T> intersection_circle1;
			TCircle3<T> intersection_circle2;
			if (GetIntersection(s1,s2,intersection_circle1))
			{
				if (GetIntersection(s1,s3,intersection_circle2))
				{
					TPlane3<T> plane1(intersection_circle1.p,intersection_circle1.n);
					TPlane3<T> plane2(intersection_circle2.p,intersection_circle2.n);
					TLine3<T> line;
					if (GetIntersection(plane1,plane2,line))
					{
						if (GetIntersection(line,s1,c1,c2))
						{
							if (s1.has(c1,true) == false)
							{
								return false;
							}
							if (s1.has(c2,true) == false)
							{
								return false;
							}
							if (s2.has(c1,true) == false)
							{
								return false;
							}
							if (s2.has(c2,true) == false)
							{
								return false;
							}
							if (s3.has(c1,true) == false)
							{
								return false;
							}
							if (s3.has(c2,true) == false)
							{
								return false;
							}
							return true;
						}
					}
				}
			}
			return false;
		}*/


		bool checkProbe(const TSphere3<T>& probe)
		{
			list<Index> atom_list;
			atom_list = tree_->get(probe.p,probe.radius+r_max_);
			for (list<Index>::iterator i = atom_list.begin(); i != atom_list.end(); i++)
			{
				if (Maths::isLess(probe.p.getDistance(atom_[*i].p),probe.radius+atom_[*i].radius))
				{
					#ifdef debug_rs
					print << pre << " ... Konflikt mit Atom " << *i << " ... "
								<< probe.p.getDistance(atom_[*i].p) << " < " << probe.radius+atom_[*i].radius << "\n";
					#endif
					return false;
				}
			}
			return true;
		}



		protected:

    std::vector< TSphere3<T> > atom_;
    T probe_radius_;
    TBSDTree<T>* tree_;
    std::vector< TRSVertex<T>* > vertices_;
    std::vector< TRSEdge<T>* > edges_;
    std::vector< TRSFace<T>* > faces_;
    T r_max_;
		#ifdef debug_rs
    std::ofstream print;
    string pre;
		#endif
	
	};


	/**	@name	Storers
	*/
	//@{

	/**	Input- Operator
			reads in a ReducedSurface
	*/
	template <typename T>
	std::istream& operator >> (std::istream& s, TReducedSurface<T>& rs)
	{
		throw Exception::NotImplemented(__FILE__, __LINE__);
	}

	/**	Output- Operator
			prints out a ReducedSurface
	*/
		template <typename T>
		std::ostream& operator << (std::ostream& s, TReducedSurface<T>& rs)
		{
			s << "Spheres:\n";
			for (Position i = 0; i < rs.numberOfAtoms(); i++)
			{
				s << "  " << rs.getSphere(i) << "\n";
		  }
		  s << "RSVertices:\n";
			for (Position i = 0; i < rs.numberOfVertices(); i++)
			{
				if (rs.getVertex(i) == NULL)
	      {
					s << "  --\n";
	      }
	      else
	      {
					s << "  " << *(rs.getVertex(i)) << "\n";
	      }
		  }
		  s << "RSEdges:\n";
			for (Position i = 0; i < rs.numberOfEdges(); i++)
			{
				s << "  " << *(rs.getEdge(i)) << "\n";
		  }
		  s << "RSFaces:\n";
			for (Position i = 0; i < rs.numberOfFaces(); i++)
			{
				s << "  " << *(rs.getFace(i)) << "\n";
		  }
		  return s;
		}
	//@}


	/**	The Default ReducedSurface Type.
			If double precision is not needed, {\tt TReducedSurface<float>} should
			be used. It is predefined as {\tt ReducedSurface} for convenience.
	*/
	typedef TReducedSurface<float> ReducedSurface;

} // namespace BALL

#endif  // BALL_STRUCTURE_REDUCEDSURFACE_H
