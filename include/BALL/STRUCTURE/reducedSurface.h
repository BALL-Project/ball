// $Id: reducedSurface.h,v 1.2 2000/10/11 09:39:07 oliver Exp $

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
#include <set>
#include <fstream>

namespace BALL
{

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
			:	atom_(), probe_radius_((T)0), vertices_(), edges_(), faces_(), r_max_((T)0)
		{
		}

		/**	Copy constructor.
				Create a new ReducedSurface object from another.
				@param reduced_surface the ReducedSurface object to be copied
				@param bool ignored - just for interface consistency
		*/
		TReducedSurface(const TReducedSurface<T>& reduced_surface,bool /* deep */ = true)
			: atom_(reduced_surface.atom_), probe_radius_(reduced_surface.probe_radius_),
				vertices_(reduced_surface.vertices_), edges_(reduced_surface.edges_),
				faces_(reduced_surface.faces_), r_max_(reduced_surface.r_max_)
		{
		}

		/**	Detailled constructor.
				Create a new ReducedSurface object from a list of spheres.
				@param	atom assigned to the vector of atoms
		*/
		TReducedSurface(const vector< TSphere3<T> >& spheres, const T& probe_radius)
		{
#ifdef debug_rs
			print.open("ReducedSurface.log");
			pre = "  ";
			print << "START\n";
			print << pre << "ReducedSurface(" << spheres.size() << ")\n";
			pre += "  ";
#endif
			vector< TSphere3<T> > atom(spheres.size());
			vector< TRSVertex<T>* > vertices(spheres.size());
			vector< TRSEdge<T>* > edges;
			vector< TRSFace<T>* > faces;
			vector< TVector3<T> > point_list;
			list<Index> index_list;
			T r_max = 0;
			for (Position i = 0; i < spheres.size(); i++)
			{
				atom[i] = spheres[i];
				vertices[i] = NULL;
				point_list.push_back(spheres[i].p);
				index_list.push_back(i);
				if (Maths::isLess(r_max,spheres[i].radius))
				{
					r_max = spheres[i].radius;
				}
			}
			atom_ = atom;
			probe_radius_ = probe_radius;
			vertices_ = vertices;
			edges_ = edges;
			free_edges_ = edges;
			faces_ = faces;
			tree_ = new TBSDTree<T>(point_list,index_list,"","");
			tree_->build();
			r_max_ = r_max;
#ifdef debug_rs
			for (Position i = 0; i < atom_.size(); i++)
				{
					print << pre << "Atom " << atom_[i] << " eingefügt\n";
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
			for (Position i = 0; i < free_edges_.size(); i++)
			{
				delete free_edges_[i];
			}
			for (Position i = 0; i < faces_.size(); i++)
			{
				delete faces_[i];
			}
			delete tree_;
#ifdef debuf_rs
			print << pre << "~ReducedSurface()\n";
			print << pre << "end\n";
			print << "ENDE\n";
#endif
		}
		//@}

		/**	@name	Accessors
		*/
		//@{

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

		/** Return the number of free rsedges.
				@return Size the number of free rsedges
		*/
		Size numberOfFreeEdges()
		{
			return free_edges_.size();
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
		vector< TRSVertex<T>* > getVertices()
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
		vector< TRSEdge<T>* > getEdges()
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

		/** Return the free edges
				@return vector< TRSEdge<T>* > all free edges
		*/
		vector< TRSEdge<T>* > getFreeEdges()
		{
			return free_edges_;
		}

		/** Return the i'th free rsedge.
				@param	i	the index of the free rsedge that should be given back
				@return TSpherer3<T>, the i'th free rsedge
		*/
		TRSEdge<T>* getFreeEdge(Position i)
		{
			if (i < free_edges_.size())
			{
				return free_edges_[i];
			}
			else
			{
				throw Exception::IndexOverflow(__FILE__, __LINE__,i,free_edges_.size());
			}
		}

		/** Return the faces
				@return vector< TRSFace<T>* > all faces
		*/
		vector< TRSFace<T>* > getFaces()
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
			list<Index> indices;
			list<Index> deleted_indices;
			for (Position i = 0; i < atom_.size(); i++)
			{
				indices.push_back(i);
			}
			list< TRSVertex<T>* > new_vertices;
			TRSVertex<T>* start_vertex(NULL);
			TRSEdge<T>* start_edge(NULL);
			TRSFace<T>* start_face(NULL);
			Byte start = 1;
			while (start != 0)
			{
				start = getStartPosition(indices,deleted_indices,new_vertices,start_vertex,start_edge,start_face);
				switch (start)
				{
					case 2 :	while (extendComponent(indices,deleted_indices,new_vertices));
										break;
					case 3 :	getRSComponent(indices,deleted_indices,start_face,new_vertices);
										while (extendComponent(indices,deleted_indices,new_vertices));
										break;
					default :	break;
				}
				for (list<Index>::iterator i = deleted_indices.begin(); i != deleted_indices.end(); i++)
				{
					tree_->remove(*i);
				}
				deleted_indices.clear();
			}
		}

		//@}


/************ computing reduced surface ********************************************************************/

		private:

		void getRSComponent(list<Index>& indices, list<Index>& deleted_indices, TRSFace<T>* face,
												list< TRSVertex<T>* >& new_vertices)
		{
			list< TRSEdge<T>* > new_edges;
			new_edges.push_back(face->getEdge(0));
			new_edges.push_back(face->getEdge(1));
			new_edges.push_back(face->getEdge(2));
			while (new_edges.size() > 0)
			{
				treatEdge(new_edges.front(),indices,deleted_indices,new_edges,new_vertices);
			}
		}


		void treatEdge(TRSEdge<T>* edge, list<Index>& indices, list<Index>& deleted_indices,
									 list< TRSEdge<T>* >& new_edges, list< TRSVertex<T>* >& new_vertices)
		{
			// find third atom
#ifdef debug_rs
print << pre << "treat " << *edge << "\n";
pre += "  ";
#endif
			TAngle<T> phi;
			TSphere3<T> probe;
			TRSFace<T>* start_face(edge->getFace(0));
#ifdef debug_rs
print << pre << "starting face: " << *start_face << "\n";
#endif
			Index atom1(edge->getVertex(0));
			Index atom2(edge->getVertex(1));
			Index atom3(thirdAtom(atom1,atom2,start_face,probe,phi));
#ifdef debug_rs
print << pre << "new face: " << atom1 << "  " << atom2 << "  " << atom3 << "\n";
#endif
			TSphere3<T> sphere1(atom_[atom1]);
			TSphere3<T> sphere2(atom_[atom2]);
			TSphere3<T> sphere3(atom_[atom3]);
			// build a new rsvertex if it yet not exists
			if (vertices_[atom3] == NULL)
			{
				indices.remove(atom3);
				deleted_indices.push_back(atom3);
				vertices_[atom3] = new TRSVertex<T>(atom3);
				new_vertices.push_back(vertices_[atom3]);
#ifdef debug_rs
print << pre << "vertices_[" << atom3 << "] initialisiert\n";
#endif
			}
			// build a new face and two new edges
			TRSEdge<T>* edge1;
			TRSEdge<T>* edge2;
			TRSFace<T>* new_face
					= new TRSFace<T>(atom1,atom2,atom3,NULL,NULL,NULL,probe.p,TVector3<T>(),false,-1);
			TRSFace<T>* test = vertices_[atom1]->has(new_face);
#ifdef debug_rs
print << pre << "face " << *new_face << " gebaut\n";
#endif
			if (test == NULL)		// new_face does not exist yet
			{
				edge1 = new TRSEdge<T>();
				edge2 = new TRSEdge<T>();
				updateFaceAndEdges(atom1,atom2,atom3,probe,6,new_face,edge,edge1,edge2);
				new_face->setIndex(faces_.size());
				faces_.push_back(new_face);
				vertices_[atom1]->pushFace(new_face);
				vertices_[atom2]->pushFace(new_face);
				vertices_[atom3]->pushFace(new_face);
				new_edges.push_back(edge1);
				new_edges.push_back(edge2);
#ifdef debug_rs
print << pre << "... geupdatet " << *new_face << "\n";
print << pre << "... in vertices_[" << atom1 << "], vertices_[" << atom2 << "], "
			<< "vertices_[" << atom3 << "] gepusht\n";
print << pre << "neue edges:\n" << pre << *edge1 << "\n" << pre << *edge2
			<< "\n" << pre << "... in new_edges gepusht\n";
#endif
			}
			else
			{
#ifdef debug_rs
print << pre << "... existiert schon: " << *test << "\n";
#endif
				Index i = 0;
				TRSEdge<T>* test_edge(test->getEdge(0));
				while (test_edge->similar(*edge) == false)
				{
					i++;
					test_edge = test->getEdge(i);
				}
				test->setEdge(i,edge);
#ifdef debug_rs
print << pre << i << ". edge ersetzt: " << *test << "\n";
#endif
				new_edges.remove(test_edge);
				delete new_face;
#ifdef debug_rs
print << pre << "konstruierte face gelöscht\n";
#endif
				new_face = test;
			}
			// update edge
#ifdef debug_rs
print << pre << "updating " << *edge << ":\n";
#endif
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
			edge->set(atom1,atom2,start_face,new_face,circle1.p,circle1.radius,phi,
								circle2,circle3,ip1,ip2,singular,edges_.size());
#ifdef debug_rs
print << pre << "... " << *edge << "\n";
#endif
			edges_.push_back(edge);
			vertices_[atom1]->pushEdge(edge);
			vertices_[atom2]->pushEdge(edge);
#ifdef debug_rs
print << pre << "... in vertices_[" << atom1 << "], vertices_[" << atom2 << "] gepusht\n";
#endif
			new_edges.remove(edge);
#ifdef debug_rs
pre.replace(0,2,"");
#endif
		}
	

		bool extendComponent(list<Index>& indices, list<Index>& deleted_indices,
												 list< TRSVertex<T>* >& new_vertices)
		{
			if (new_vertices.size() == 0)
			{
				return false;
			}
			TRSFace<T>* face = NULL;
			TRSVertex<T>* vertex1 = new_vertices.front();
			Index atom1(vertex1->getAtom());
			list<Index> atom_list(neighboursOfOneAtom(atom1,indices));
			list<Index>::iterator i = atom_list.begin();
			while (i != atom_list.end())
			{
				Index atom2 = *i;
				list< pair< Index,TSphere3<T> > > candidates(findThird(atom1,atom2,atom_list));
				if (candidates.size() == 0)
				{
					TRSEdge<T>* edge = createFreeEdge(atom1,atom2);
					if (edge != NULL)
					{
						edge->setIndex(free_edges_.size());
						free_edges_.push_back(edge);
						vertices_[atom2] = new TRSVertex<T>(atom2);
						indices.remove(atom2);
						deleted_indices.push_back(atom2);
						vertices_[atom1]->pushEdge(edge);
						vertices_[atom2]->pushEdge(edge);
						new_vertices.push_back(vertices_[atom1]);
						new_vertices.push_back(vertices_[atom2]);
						return true;
					}
					i++;
				}
				list< pair< Index,TSphere3<T> > >::iterator j = candidates.begin();
				while ((j != candidates.end()) && (i != atom_list.end()))
				{
					Index atom3 = j->first;
					TSphere3<T> probe = j->second;
					face = new TRSFace<T>();
					TRSEdge<T>* edge1 = new TRSEdge<T>();
					TRSEdge<T>* edge2 = new TRSEdge<T>();
					TRSEdge<T>* edge3 = new TRSEdge<T>();
					updateFaceAndEdges(atom1,atom2,atom3,probe,7,face,edge1,edge2,edge3);
					face->setIndex(faces_.size());
					faces_.push_back(face);
//					new_edges.push_back(edge1);
//					new_edges.push_back(edge2);
//					new_edges.push_back(edge3);
					vertices_[atom2] = new TRSVertex<T>(atom2);
					indices.remove(atom2);
					deleted_indices.push_back(atom2);
					vertices_[atom3] = new TRSVertex<T>(atom3);
					indices.remove(atom3);
					deleted_indices.push_back(atom3);
					vertices_[atom1]->pushFace(face);
					vertices_[atom2]->pushFace(face);
					vertices_[atom3]->pushFace(face);
					new_vertices.push_back(vertices_[atom1]);
					new_vertices.push_back(vertices_[atom2]);
					new_vertices.push_back(vertices_[atom3]);
					i = atom_list.end();
					j = candidates.end();
				} // while j
			} // while i
			new_vertices.pop_front();
			if (face != NULL)
			{
				getRSComponent(indices,deleted_indices,face,new_vertices);
			}
			return true;
		}


		Index thirdAtom(Index atom1, Index atom2, TRSFace<T>* face, TSphere3<T>& probe, TAngle<T>& phi)
		{
#ifdef debug_rs
			pre += "  ";
#endif
			list<Index> atom_list;
			TSphere3<T> sphere1(atom_[atom1]);
			TSphere3<T> sphere2(atom_[atom2]);
			sphere1.radius += probe_radius_;
			sphere2.radius += probe_radius_;
			TCircle3<T> circle;
			GetIntersection(sphere1,sphere2,circle);
			atom_list = tree_->get(circle.p,circle.radius+r_max_+probe_radius_);
			list< pair< Index,TSphere3<T> > > candidates = findThird(atom1,atom2,atom_list);
			list< pair< Index,TSphere3<T> > >::iterator k;
			Index back(-1);
#ifdef debug_rs
			print << pre << atom1 << "  " << atom2 << "\n";
			print << pre << *face << "\n";
			print << pre << "Kandidaten:\n";
#endif
			TAngle<T> old_angle(2*Constants::PI,true);
			TAngle<T> new_angle;
			TAngle<T> pi(Constants::PI,true);
			TVector3<T> norm = atom_[atom1].p-atom_[atom2].p;
			TVector3<T> test_vector = face->getNormal()%norm;
#ifdef debug_rs
print << pre << "  Daten zur Drehachse:\n";
print << pre << "  Atom1:      " << atom_[atom1] << "\n";
print << pre << "  Atom2:      " << atom_[atom2] << "\n";
print << pre << "  norm:       " << norm << "\n";
print << pre << "  Testvektor: " << test_vector << "\n";
print << pre << "  Testpunkt:  " << atom_[face->thirdVertexIndex(atom1,atom2)].p << "\n";
#endif
			if (Maths::isLess(test_vector*atom_[face->thirdVertexIndex(atom1,atom2)].p,
												test_vector*atom_[atom1].p))
			{
				norm.negate();
#ifdef debug_rs
print << pre << "Dreahachse: " << atom1 << " --> " << atom2 << "\n";
#endif
			}
			else
			{
#ifdef debug_rs
print << pre << "Dreahachse: " << atom2 << " --> " << atom1 << "\n";
#endif
			}
#ifdef debug_rs
print << pre << "  norm:       " << norm << "\n";
#endif
			TVector3<T> v1 = face->getCenter()-circle.p;
			for (k = candidates.begin(); k != candidates.end(); k++)
			{
				if (((k->first == face->getVertex(0)) || (k->first == face->getVertex(1)) ||
						 (k->first == face->getVertex(2))) && (k->second.p == face->getCenter()))
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
/*					new_angle = getOrientedAngle(v1,v2,norm);
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

		Byte getStartPosition(list<Index>& indices, list<Index>& deleted_indices,
													list< TRSVertex<T>* >& new_vertices,
													TRSVertex<T>*& vertex, TRSEdge<T>*& edge, TRSFace<T>*& face)
		{
			Index vertex1;
			Index vertex2;
			Index vertex3;
			Byte test = findStartPosition(indices,deleted_indices,0,0,vertex,edge,face);
			switch (test)
			{
				case 1 :	
									vertices_[vertex->getAtom()] = vertex;
									indices.remove(vertex->getAtom());
									deleted_indices.push_back(vertex->getAtom());
									new_vertices.push_back(vertex);
									break;
				case 2 :	
									edge->setIndex(free_edges_.size());
									free_edges_.push_back(edge);
									vertex1 = edge->getVertex(0);
									vertex2 = edge->getVertex(1);
									vertices_[vertex1]->pushEdge(edge);
									vertices_[vertex2]->pushEdge(edge);
									indices.remove(vertex1);
									indices.remove(vertex2);
									deleted_indices.push_back(vertex1);
									deleted_indices.push_back(vertex2);
									new_vertices.push_back(vertices_[vertex1]);
									new_vertices.push_back(vertices_[vertex2]);
									break;
				case 3 :	
									face->setIndex(faces_.size());
									faces_.push_back(face);
									vertex1 = face->getVertex(0);
									vertex2 = face->getVertex(1);
									vertex3 = face->getVertex(2);
									vertices_[vertex1]->pushFace(face);
									vertices_[vertex2]->pushFace(face);
									vertices_[vertex3]->pushFace(face);
									indices.remove(vertex1);
									indices.remove(vertex2);
									indices.remove(vertex3);
									deleted_indices.push_back(vertex1);
									deleted_indices.push_back(vertex2);
									deleted_indices.push_back(vertex3);
									new_vertices.push_back(vertices_[vertex1]);
									new_vertices.push_back(vertices_[vertex2]);
									new_vertices.push_back(vertices_[vertex3]);
									break;
			}
			return test;
		}


		Byte findStartPosition(list<Index>& indices, list<Index>& deleted_indices, Byte direction, Byte extrem,
													 TRSVertex<T>*& vertex, TRSEdge<T>*& edge, TRSFace<T>*& face)
		{
#ifdef debug_rs
			print << pre << "findFace(" << direction << "," << extrem << ")\n";
			pre += "  ";
#endif
//			vertex = NULL;
//			edge = NULL;
//			face = NULL;
			if (indices.size() == 0)
			{
#ifdef debug_rs
				pre.replace(0,2,"");
				print << pre << "end ... 0\n";
#endif
				return 0;
			}
			Index a1 = findFirst(indices,direction,extrem);
#ifdef debug_rs
			print << pre << "1. Atom: " << a1 << ": " << atom_[a1] << "\n";
#endif
			list<Index> atom_list = tree_->get(atom_[a1].p,atom_[a1].radius+r_max_+2*probe_radius_);
			Index a2 = findSecond(a1,atom_list,direction,extrem);
			if (a2 == -1)
			{
				vertex = new TRSVertex<T>(a1);
#ifdef debug_rs
				pre.replace(0,2,"");
				print << pre << "end ... 1\n";
#endif
				return 1;
			}
#ifdef debug_rs
			print << pre << "2. Atom: " << a2 << ": " << atom_[a2] << "\n";
#endif
			TSphere3<T> s1(atom_[a1].p,atom_[a1].radius+probe_radius_);
			TSphere3<T> s2(atom_[a2].p,atom_[a2].radius+probe_radius_);
			TCircle3<T> circle;
			GetIntersection(s1,s2,circle);
			atom_list = tree_->get(circle.p,circle.radius+r_max_+probe_radius_);
			list< pair< Index,TSphere3<T> > > candidates = findThird(a1,a2,atom_list);
			if (candidates.size() == 0)
			{
#ifdef debug_rs
				pre.replace(0,2,"");
				print << pre << "end ... 2\n";
#endif
				edge = createFreeEdge(a1,a2);
				if (vertices_[a1] == NULL)
				{
					vertices_[a1] = new TRSVertex<T>(a1);
					indices.remove(a1);
					deleted_indices.push_back(a1);
				}
				if (vertices_[a2] == NULL)
				{
					vertices_[a2] = new TRSVertex<T>(a2);
					indices.remove(a2);
					deleted_indices.push_back(a2);
				}
				return 2;
			}
			list< pair< Index,TSphere3<T> > >::iterator i = candidates.begin();
			Index a3 = candidates.begin()->first;
			TSphere3<T> probe = candidates.begin()->second;
			while (checkProbe(probe) == false)
			{
				i++;
				a3 = i->first;
				probe = i->second;
			}
#ifdef debug_rs
			print << pre << "3. Atom: " << a3 << ": " << atom_[a3] << "\n"
						<< pre << "  Probe: " << probe << "\n";
#endif
			if (vertices_[a1] == NULL)
			{
				vertices_[a1] = new TRSVertex<T>(a1);
				indices.remove(a1);
				deleted_indices.push_back(a1);
			}
			if (vertices_[a2] == NULL)
			{
				vertices_[a2] = new TRSVertex<T>(a2);
				indices.remove(a2);
				deleted_indices.push_back(a2);
			}
			if (vertices_[a3] == NULL)
			{
				vertices_[a3] = new TRSVertex<T>(a3);
				indices.remove(a3);
				deleted_indices.push_back(a3);
			}
			TRSEdge<T>* e1 = new TRSEdge<T>();
			TRSEdge<T>* e2 = new TRSEdge<T>();
			TRSEdge<T>* e3 = new TRSEdge<T>();
			face = new TRSFace<T>();
			updateFaceAndEdges(a1,a2,a3,probe,7,face,e1,e2,e3);
//			new_edges.push_back(e1);
//			new_edges.push_back(e2);
//			new_edges.push_back(e3);
//			atom[a1]->done = atom[a2]->done = atom[a3]->done = true;
//			new_vertices.push_back(a1); new_vertices.push_back(a2); new_vertices.push_back(a3);
#ifdef debug_rs
			pre.replace(0,2,"");
			print << pre << "end ... 3\n";
#endif
			return 3;
		}


		Index findFirst(list<Index> indices, Byte direction, Byte extrem)
		{
			TSphere3<T> next_atom(atom_[*indices.begin()]);
			T dir_ex = ((extrem == 0) ? next_atom.p[direction]-next_atom.radius
																: next_atom.p[direction]+next_atom.radius);
			Index a_ex = *indices.begin();
			list<Index>::iterator i;
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


		Index findSecond(Index atom1, list<Index> atom_list, Byte direction, Byte extrem)
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


		list< pair< Index,TSphere3<T> > > findThird(Index atom1, Index atom2, list<Index> atom_list)
		{
			list< pair< Index,TSphere3<T> > > back;
			pair< Index,TSphere3<T> > candidate;
			list<Index> third = neighboursOfTwoAtoms(atom1,atom2,atom_list);
			if (third.size() == 0)
			{
				return back;
			}
			list<Index>::iterator l = third.begin();
			while (l != third.end())
			{
				TVector3<T> center1, center2;
				if (centerOfProbe(atom1,atom2,*l,center1,center2))
				{
					TSphere3<T> probe(center1,probe_radius_);
					candidate.first = *l;
					candidate.second = probe;
					back.push_back(candidate);
					probe.p = center2;
					candidate.first = *l;
					candidate.second = probe;
					back.push_back(candidate);
				}
				l++;
			}
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


/************ creating / updating edges / faces ************************************************************/

		TRSEdge<T>* createFreeEdge(Index atom1, Index atom2)
		{
			TCircle3<T> circle1;
			TCircle3<T> circle2;
			TCircle3<T> circle3;
			if (getCircles(atom1,atom2,circle1,circle2,circle3) &&
					Maths::isGreater(circle1.radius,probe_radius_))
			{
				TVector3<T> vector(0,0,0);
				TRSEdge<T>* edge = new TRSEdge<T>(atom1,atom2,NULL,NULL,circle1.p,circle1.radius,
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


		bool updateFaceAndEdges(Index a1, Index a2, Index a3, const TSphere3<T>& probe, Byte mode,
														TRSFace<T>* f, TRSEdge<T>* e1, TRSEdge<T>* e2, TRSEdge<T>* e3)
		{
			TVector3<T> vector(0,0,0);
			TAngle<T> angle(0,true);
			TCircle3<T> circle(vector,vector,0);
			if ((mode & 1) == 1)
			{
				e1->set(a1,a2,f,NULL,vector,(T)0,angle,circle,circle,vector,vector,false,-1);
			}
			if ((mode & 2) == 2)
			{
				e2->set(a2,a3,f,NULL,vector,(T)0,angle,circle,circle,vector,vector,false,-1);
			}
			if ((mode & 4) == 4)
			{
				e3->set(a3,a1,f,NULL,vector,(T)0,angle,circle,circle,vector,vector,false,-1);
			}
			TPlane3<T> plane(atom_[a1].p,atom_[a2].p,atom_[a3].p);
			TVector3<T> norm(plane.n);
			if (Maths::isLess(norm*probe.p,norm*atom_[a1].p))
			{
				norm.negate();
			}
			TCircle3<T> dummy;
			f->set(a1,a2,a3,e1,e2,e3,probe.p,norm,GetIntersection(probe,plane,dummy),-1);
			return true;
		}


/************ finding a probe sphere ***********************************************************************/

		bool centerOfProbe(Index a1, Index a2, Index a3, TVector3<T>& c1, TVector3<T>& c2)
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
						return GetIntersection(line,s1,c1,c2);
					}
				}
			}
			return false;
		}


    T getCircleExtremum(const TCircle3<T> circle, Position direction, Position extrem)
    {
      T min, max;
      TVector3<T> norm2 = TVector3<T>(circle.n.x * circle.n.x, circle.n.y * circle.n.y,
																			circle.n.z * circle.n.z);
      switch (direction)
      {
        case 0 :  
					if (Maths::isEqual(circle.n.y, 0) && Maths::isEqual(circle.n.z,0))
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
					if (Maths::isEqual(circle.n.x, 0) && Maths::isEqual(circle.n.z, 0))
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
					if (Maths::isEqual(circle.n.x, 0) && Maths::isEqual(circle.n.y, 0))
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
      if (Maths::isEqual(extrem, 0))
      {
        return Maths::min(min, max);
			}
      else
      {
        return Maths::max(min, max);
			}
		}
 
		bool checkProbe(const TSphere3<T>& probe)
		{
			list<Index> atom_list;
			atom_list = tree_->get(probe.p,probe.radius+r_max_);
			for (list<Index>::iterator i = atom_list.begin(); i != atom_list.end(); i++)
			{
				TCircle3<T> intersection_circle;
				if (GetIntersection(probe,atom_[*i],intersection_circle))
				{
					if (Maths::isNotZero(intersection_circle.radius))
					{
						return false;
					}
				}
			}
			return true;
		}



		protected:

    vector< TSphere3<T> > atom_;
    T probe_radius_;
    TBSDTree<T>* tree_;
    vector< TRSVertex<T>* > vertices_;
    vector< TRSEdge<T>* > edges_;
    vector< TRSEdge<T>* > free_edges_;
    vector< TRSFace<T>* > faces_;
    T r_max_;
#ifdef debug_rs
    ofstream print;
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
		  s << "free RSEdges:\n";
			for (Position i = 0; i < rs.numberOfFreeEdges(); i++)
			{
				s << "  " << *(rs.getFreeEdge(i)) << "\n";
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
