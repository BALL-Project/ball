// $Id: solventExcludedSurface.h,v 1.5 2000/10/30 00:19:27 amoll Exp $

#ifndef BALL_STRUCTURE_SOLVENTEXCLUDEDSURFACE_H
#define BALL_STRUCTURE_SOLVENTEXCLUDEDSURFACE_H


#ifndef BALL_STRUCTURE_SESVERTEX_H
#	include <BALL/STRUCTURE/SESVertex.h>
#endif

#ifndef BALL_STRUCTURE_SESEDGE_H
#	include <BALL/STRUCTURE/SESEdge.h>
#endif

#ifndef BALL_STRUCTURE_SESFACE_H
#	include <BALL/STRUCTURE/SESFace.h>
#endif

#ifndef BALL_STRUCTURE_REDUCEDSURFACE_H
#	include <BALL/STRUCTURE/reducedSurface.h>
#endif

#ifndef BALL_MATHS_ANALYTICALGEOMETRY_H
#	include <BALL/MATHS/analyticalGeometry.h>
#endif

#include <vector>
#include <list>


namespace BALL
{

	/** Generic SolventExcludedSurface Class.
			\\
			{\bf Definition:} \URL{BALL/STRUCTURE/solventExcludedSurface.h}
			\\
	*/
	template <class T>
	class TSolventExcludedSurface
	{
		public:

		BALL_CREATE(TSolventExcludedSurface)

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
				This method creates a new SolventExcludedSurface object. All components are
				initialized to {\tt (T)0} or {\tt NULL}, respectivly.
		*/
		TSolventExcludedSurface()
		{
		}

		/**	Copy constructor.
				Create a new SolventExcludedSurface object from another.
				@param s_e_surface the SolventExcludedSurface object to be copied
				@param bool ignored - just for interface consistency
		*/
		TSolventExcludedSurface(const TSolventExcludedSurface<T>& ses, bool /* deep */ = true)
			:	vertices(ses.vertices), edges(ses.edges), contact_faces(ses.contact_faces),
				toric_faces(ses.toric_faces), spheric_faces(ses.spheric_faces)
		{
		}

		/** Detailed constructor
		*/
		TSolventExcludedSurface(TReducedSurface<T>* reduced_surface)
		{
			print.open("SolventExcludedSurface.log");
			pre = "  ";
			print << "START\n";
			print << pre << "SolventExcludedSurface(reducedSurface)\n";
			pre += "  ";
			vector< TRSVertex<T>* > vertices(reduced_surface->getVertices());
			for (Position i = 0; i < vertices.size(); i++)
			{
				if (vertices[i] != NULL)
				{
					TSESFace<T>* face = new TSESFace<T>();
					face->type = TSESFace<T>::TYPE_CONTACT;
					face->rsvertex = vertices[i];
					face->rsedge = NULL;
					face->rsface = NULL;
					face->index = contact_faces.size();
					contact_faces.push_back(face);
				}
				else
				{
					contact_faces.push_back(NULL);
				}
			}
			vector< TRSEdge<T>* > edges(reduced_surface->getEdges());
			for (Position i = 0; i < edges.size(); i++)
			{
				TSESFace<T>* face = new TSESFace<T>();
				face->type = TSESFace<T>::TYPE_TORIC;
				face->rsvertex = NULL;
				face->rsedge = edges[i];
				face->rsface = NULL;
				face->index = toric_faces.size();
				toric_faces.push_back(face);
			}
			vector< TRSFace<T>* > faces(reduced_surface->getFaces());
			for (Position i = 0; i < faces.size(); i++)
			{
				TSESFace<T>* face = new TSESFace<T>();
				face->type = TSESFace<T>::TYPE_SPHERIC;
				face->rsvertex = NULL;
				face->rsedge = NULL;
				face->rsface = faces[i];
				face->index = spheric_faces.size();
				spheric_faces.push_back(face);
			}
			pre.replace(0,2,"");
			print << pre << "end\n";
		}

		/**	Destructor.
				Destructs the SolventExcludedSurface object. As there are no dynamic
				data structures, nothing happens.
		*/
		virtual ~TSolventExcludedSurface()
		{
			print << pre << "~SolventExcludedSurface\n";
			pre += "  ";
			for (Position i = 0; i < vertices.size(); i++)
			{
				delete vertices[i];
			}
			for (Position i = 0; i < edges.size(); i++)
			{
				if (edges[i] != NULL)
				{
					delete edges[i];
				}
			}
			for (Position i = 0; i < contact_faces.size(); i++)
			{
				delete contact_faces[i];
			}
			for (Position i = 0; i < toric_faces.size(); i++)
			{
				if (toric_faces[i] != NULL)
				{
					delete toric_faces[i];
				}
			}
			for (Position i = 0; i < singular_toric_faces.size(); i++)
			{
				delete singular_toric_faces[i];
			}
			for (Position i = 0; i < spheric_faces.size(); i++)
			{
				if (spheric_faces[i] != NULL)
				{
					delete spheric_faces[i];
				}
			}
			pre.replace(0,2,"");
			print << pre << "end\n";
			print << "ENDE\n";
		}
		//@}

		/**	@name	Accessors
		*/
		//@{
		
		/**	get.
				Computes the solvent excluded surface from a ReducedSurface object and radius of the probe sphere
				@param rs	the ReducedSurface object
		*/
		void get(TReducedSurface<T>* rs)
		{
			for (Position i = 0; i < spheric_faces.size(); i++)
			{
				createSphericFace(i,rs);
			}
			for (Position i = 0; i < toric_faces.size(); i++)
			{
//cout << i << "\n";
				createToricFace(i,rs);
			}
		}

		void createSphericFace(Position j, TReducedSurface<T>* rs)
		{
			TSESFace<T>* face = spheric_faces[j];
			TRSFace<T>* rsface = face->rsface;
			TSphere3<T> probe(rsface->getCenter(),rs->getProbeRadius());
			for (Position i = 0; i < 3; i++)
			{
				Index index = rsface->getVertex(i);
				pushVertex(face,probe,rs->getSphere(index),index);
//				pushVertex(face,probe,rs->vertices[rsface->vertex[i]]);
			}
			T radius_of_probe = rs->getProbeRadius();
			pushConcaveEdge(face,0,1,radius_of_probe);
			pushConcaveEdge(face,1,2,radius_of_probe);
			pushConcaveEdge(face,2,0,radius_of_probe);
		}

		TSESVertex<T>* createVertex(const TVector3<T>& center, const TSphere3<T>& atom,
																Index index)//, const T& dist)
			{
				TSESVertex<T>* vertex = new TSESVertex<T>();
//				vertex->p = getPoint(center,atom.p,dist);
				vertex->p = getPoint(atom.p,center,atom.radius);
//				vertex->n = center-atom.p;
				vertex->atom = index;
				vertex->index = vertices.size();
				return vertex;
			}

		void pushVertex(TSESFace<T>* face, const TSphere3<T>& probe, const TSphere3<T>& atom, Index index)
		{
			TRSFace<T>* rsface = face->rsface;
			TSESVertex<T>* vertex;
			vertex = createVertex(probe.p,atom,index);//,probe.radius);
			face->vertex.push_back(vertex);
			Index tf1, tf2;
			rsface->getEdges(index, tf1, tf2);
			toric_faces[tf1]->vertex.push_back(vertex);
			toric_faces[tf2]->vertex.push_back(vertex);
			contact_faces[index]->vertex.push_back(vertex);
			vertices.push_back(vertex);
		}
		

		TSESEdge<T>* createConcaveEdge
			(TSESFace<T>* spheric_face, Position p1, Position p2,
			 Index index, const T& radius_of_probe)
		{
			TSESEdge<T>* edge = new TSESEdge<T>();
			edge->vertex1 = spheric_face->vertex[p1];
			edge->vertex2 = spheric_face->vertex[p2];
			edge->face1 = spheric_face;
			edge->face2 = toric_faces[index];
			edge->rsedge = NULL;
			edge->type = 1;
			edge->index = edges.size();
			TRSFace<T>* rsface = spheric_face->rsface;
			TVector3<T> normal = (rsface->getCenter()-edge->vertex1->p)%(rsface->getCenter()-edge->vertex2->p);
			TVector3<T> test_point = spheric_face->vertex[3-p1-p2]->p;      // third vertex
			if (Maths::isLess((test_point*normal),(rsface->getCenter()*normal)))
			{
				normal.negate();
			}
			edge->circle = TCircle3<T>(rsface->getCenter(),normal,radius_of_probe);

			return edge;
		}

		void pushConcaveEdge(TSESFace<T>* face, Position p1, Position p2, const T& radius_of_probe)
		{
			TRSFace<T>* rsface = face->rsface;
			Index index;
			rsface->getEdge(rsface->getVertex(p1),rsface->getVertex(p2),index);
			TSESEdge<T>* edge;
			edge = createConcaveEdge(face,p1,p2,index,radius_of_probe);
			face->edge.push_back(edge);
			face->orientation.push_back(0);
			toric_faces[index]->edge.push_back(edge);
			toric_faces[index]->orientation.push_back(1);
			edges.push_back(edge);
		}

		TSESEdge<T>* createConvexEdge(TSESFace<T>* toric_face, Index atom)
		{
			TSESEdge<T>* edge = new TSESEdge<T>();
			Position i = 0;
			while (toric_face->vertex[i]->atom != atom)
			{
				i++;
			}
			edge->vertex1 = toric_face->vertex[i];						// first vertex on the surface of the rsvertex
			i++;
			while (toric_face->vertex[i]->atom != atom)
			{
				i++;
			}
			edge->vertex2 = toric_face->vertex[i];						// second vertex on the surface of the rsvertex
			edge->face1 = toric_face;
			edge->face2 = contact_faces[atom];
			edge->rsedge = toric_face->rsedge;
			edge->type = 0;
			edge->index = edges.size();
			if (edge->rsedge->getVertex(0) == atom)
			{
				TCircle3<T> circle0(edge->rsedge->getContactCircle(0));
				TCircle3<T> circle1(edge->rsedge->getContactCircle(1));
				edge->circle.set(circle0.p,circle0.p-circle1.p,circle0.radius);
			}
			else
			{
				TCircle3<T> circle0(edge->rsedge->getContactCircle(0));
				TCircle3<T> circle1(edge->rsedge->getContactCircle(1));
				edge->circle.set(circle1.p,circle1.p-circle0.p,circle1.radius);
			}
			return edge;
		}

		void createToricFace(Position i, TReducedSurface<T>* rs)
		{
			TSESFace<T>* face = toric_faces[i];
			if (face->isFree())
			{
				createFreeToricFace(i);
				return;
			}
			TSESEdge<T>* edge;
			Index index = face->rsedge->getVertex(0);
			edge = createConvexEdge(face,index);
				face->edge.push_back(edge);
				face->orientation.push_back(1);
				contact_faces[index]->edge.push_back(edge);
				contact_faces[index]->orientation.push_back(0);
				edges.push_back(edge);
			index = face->rsedge->getVertex(1);
			edge = createConvexEdge(face,index);
				face->edge.push_back(edge);
				face->orientation.push_back(1);
				contact_faces[index]->edge.push_back(edge);
				contact_faces[index]->orientation.push_back(0);
				edges.push_back(edge);
			if (face->rsedge->isSingular())
			{
				treatSingularToricFace(i,rs->getProbeRadius());
			}
		}

		void treatSingularToricFace(Position i, const T& radius_of_probe)
		{
			TSESFace<T>* face = toric_faces[i];
			TSESEdge<T>* edge0;
			TSESEdge<T>* edge1;
			TSESEdge<T>* edge2;
			TSESEdge<T>* edge3;
			TSESVertex<T>* p0;
			TSESVertex<T>* p1;
			TSESVertex<T>* p2;
			TSESVertex<T>* p3;
			Position j = 0;
			while (face->edge[j]->type != 1)
			{
				j++;
			}
			edge0 = face->edge[j];								// edge0 = first concave edge
			j++;
			while (face->edge[j]->type != 1)
			{
				j++;
			}
			edge2 = face->edge[j];								// edge2 = second concave edge
			p0 = edge0->vertex1;
			p1 = edge0->vertex2;
			Index e;
			if (face->getEdge(p1->index,edge2->vertex1->index,e))
			{
				edge1 = edges[e];
				p2 = edge2->vertex1;
				p3 = edge2->vertex2;
			}
			else
			{
				face->getEdge(p1->index,edge2->vertex2->index,e);					//	  	____e3____
				edge1 = edges[e];                                         //     /          \					x
				p2 = edge2->vertex2;                                      //  p0 \          /p3
				p3 = edge2->vertex1;                                      //      \        /
			}                                                           //    e0|        |e2
			if (face->getEdge(p0->index,p3->index,e))                   //      |        |
			{                                                           //      /________\					x
				edge3 = edges[e];                                         //   p1/   e1     \p2
			}
//cout << "Face" << face->index << ":  " << *face << "\n";
//cout << "RSEdge: " << *(face->rsedge) << "\n";
//cout << "Edge0:  " << *edge0 << "\n";
//cout << "Edge1:  " << *edge1 << "\n";
//cout << "Edge2:  " << *edge2 << "\n";
//cout << "Edge3:  " << *edge3 << "\n";
//cout << "Point0: " << *p0 << "\n";
//cout << "Point1: " << *p1 << "\n";
//cout << "Point2: " << *p2 << "\n";
//cout << "Point3: " << *p3 << "\n";
//cout << p1->atom << "  " << p3->atom << "  "
//		 << face->rsedge->vertex[0] << "  " << face->rsedge->vertex[1] << "\t";
			Position ip1 = ((p1->atom == face->rsedge->getVertex(0)) ? 0 : 1);
			Position ip3 = 1-ip1;
//cout << "=>  " << ip1 << "  " << ip3 << "\n";
// create the new points
			TSESVertex<T>* new_point1 = new TSESVertex<T>(face->rsedge->getIntersectionPoint(ip1),
																										face->rsedge->getVertex(ip1),vertices.size());
			vertices.push_back(new_point1);
			TSESVertex<T>* new_point3 = new TSESVertex<T>(face->rsedge->getIntersectionPoint(ip3),
																										face->rsedge->getVertex(ip3),vertices.size());
			vertices.push_back(new_point3);
// create the new edges
			TSESEdge<T>* new_edge0 = new TSESEdge<T>(*edge0);
			TSESEdge<T>* new_edge1 = new TSESEdge<T>(*edge0);
			TSESEdge<T>* new_edge2 = new TSESEdge<T>(*edge2);
			TSESEdge<T>* new_edge3 = new TSESEdge<T>(*edge2);
			new_edge0->vertex1 = p0;
			new_edge0->vertex2 = new_point3;
			new_edge1->vertex1 = p1;
			new_edge1->vertex2 = new_point1;
			new_edge2->vertex1 = p2;
			new_edge2->vertex2 = new_point1;
			new_edge3->vertex1 = p3;
			new_edge3->vertex2 = new_point3;
			new_edge0->index = edges.size();
			edges.push_back(new_edge0);
			new_edge1->index = edges.size();
			edges.push_back(new_edge1);
			new_edge2->index = edges.size();
			edges.push_back(new_edge2);
			new_edge3->index = edges.size();
			edges.push_back(new_edge3);
			TSESFace<T>* neighbour0 = edge0->other(face);
			TSESFace<T>* neighbour2 = edge2->other(face);
			TSphere3<T> probe1(neighbour0->rsface->getCenter(),radius_of_probe);
			TSphere3<T> probe2(neighbour2->rsface->getCenter(),radius_of_probe);
			TCircle3<T> intersection_circle;
			GetIntersection(probe1,probe2,intersection_circle);
			TSESEdge<T>* new_edge4 = new TSESEdge<T>(new_point1,new_point3,neighbour0,neighbour2,
																							 intersection_circle,face->rsedge,edges.size());
			edges.push_back(new_edge4);
			singular_edges.push_back(new_edge4);
// update the neighbour faces
			for (j = 0; j < neighbour0->edge.size(); j++)
			{
				if (neighbour0->edge[j] == edge0)
				{
					neighbour0->edge[j] = NULL;
				}
			}
			for (j = 0; j < neighbour2->edge.size(); j++)
			{
				if (neighbour2->edge[j] == edge2)
				{
					neighbour2->edge[j] = NULL;
				}
			}
			neighbour0->edge.push_back(new_edge0);
			neighbour0->edge.push_back(new_edge1);
			neighbour0->edge.push_back(new_edge4);
			neighbour2->edge.push_back(new_edge2);
			neighbour2->edge.push_back(new_edge3);
			neighbour2->edge.push_back(new_edge4);
			neighbour0->vertex.push_back(new_point1);
			neighbour0->vertex.push_back(new_point3);
			neighbour2->vertex.push_back(new_point1);
			neighbour2->vertex.push_back(new_point3);
			neighbour0->orientation.push_back(0);
			neighbour0->orientation.push_back(0);
			neighbour2->orientation.push_back(0);
			neighbour2->orientation.push_back(0);
			if (Maths::isGreater(new_edge4->circle.n*neighbour0->rsface->getCenter(),
													 new_edge4->circle.n*new_edge4->circle.p))
			{
				neighbour0->orientation.push_back(0);
				neighbour2->orientation.push_back(1);
			}
			else
			{
				neighbour0->orientation.push_back(1);
				neighbour2->orientation.push_back(0);
			}
// update the toric face
			vector< TSESVertex<T>* > face_vertices(6);
			vector< TSESEdge<T>* > face_edges(6);
			vector<bool> orientations(0);
			face_vertices[0] = p0;
			face_vertices[1] = p1;
			face_vertices[2] = p2;
			face_vertices[3] = p3;
			face_vertices[4] = new_point1;
			face_vertices[5] = new_point3;
			face_edges[0] = edge1;
			face_edges[1] = edge3;
			face_edges[2] = new_edge0;
			face_edges[3] = new_edge1;
			face_edges[4] = new_edge2;
			face_edges[5] = new_edge3;
//			face_edges[6] = new_edge4;
			TSESFace<T>* new_face =
					new TSESFace<T>(TSESFace<T>::TYPE_TORIC_SINGULAR,NULL,face->rsedge,NULL,
													face_vertices,face_edges,orientations,singular_toric_faces.size());
			new_edge0->face1 = new_face;
			new_edge0->face2 = neighbour0;
			new_edge1->face1 = new_face;
			new_edge1->face2 = neighbour0;
			new_edge2->face1 = new_face;
			new_edge2->face2 = neighbour2;
			new_edge3->face1 = new_face;
			new_edge3->face2 = neighbour2;
			edge1->face1 = edge1->other(face);
			edge1->face2 = new_face;
			edge3->face1 = edge3->other(face);
			edge3->face2 = new_face;
			singular_toric_faces.push_back(new_face);
// delete the intersecting edges
			Position index = edge0->index;
			delete edge0;
			edges[index] = NULL;
			index = edge2->index;
			delete edge2;
			edges[index] = NULL;
// move the toric face into the list of singular toric faces
			toric_faces[face->index] = NULL;
			delete face;
//cout << "new_edge0: " << *new_edge0 << "\n";
//cout << "new_edge1: " << *new_edge1 << "\n";
//cout << "new_edge2: " << *new_edge2 << "\n";
//cout << "new_edge3: " << *new_edge3 << "\n";
//cout << "new_edge4: " << *new_edge4 << "\n";
//cout << *neighbour0 << "\n";
//cout << *neighbour2 << "\n";
//			face->index = singular_toric_faces.size();
//			singular_toric_faces.push_back(face);
		}

		void createFreeToricFace(Position i)
		{
			TSESFace<T>* face(toric_faces[i]);
			TCircle3<T> circle1(face->rsedge->getContactCircle(0));
			TCircle3<T> circle2(face->rsedge->getContactCircle(1));
			Index vertex1(face->rsedge->getVertex(0));
			Index vertex2(face->rsedge->getVertex(1));
			TSESEdge<T>* edge = new TSESEdge<T>();
				edge->type = 0;
				edge->vertex1 = NULL; edge->vertex2 = NULL;
				edge->rsedge = face->rsedge;
				edge->face1 = face;
				edge->face2 = contact_faces[vertex1];
				edge->circle = TCircle3<T>(circle1.p,circle1.p-circle2.p,circle1.radius);
				edge->index = edges.size();
				face->edge.push_back(edge);
				face->orientation.push_back(1);
				contact_faces[vertex1]->edge.push_back(edge);
				contact_faces[vertex1]->orientation.push_back(0);
				edges.push_back(edge);
			edge = new TSESEdge<T>();
				edge->type = 0;
				edge->vertex1 = NULL; edge->vertex2 = NULL;
				edge->rsedge = face->rsedge;
				edge->face1 = face;
				edge->face2 = contact_faces[vertex2];
				edge->circle = TCircle3<T>(circle2.p,circle2.p-circle1.p,circle2.radius);
				edge->index = edges.size();
				face->edge.push_back(edge);
				face->orientation.push_back(1);
				contact_faces[vertex2]->edge.push_back(edge);
				contact_faces[vertex2]->orientation.push_back(0);
				edges.push_back(edge);
		}


		TVector3<T> getPoint(const TVector3<T>& p1, const TVector3<T>& p2, const T& dist)
		{
			TVector3<T> diff(p2-p1);
			diff.normalize();
			diff *= dist;
			return p1+diff;
		}


		void write()
		{
			print << pre << "Vertices:\n";
			for (Position i = 0; i < vertices.size(); i++)
			{
				print << pre << "  " << *vertices[i] << "\n";
			}
			print << pre << "Edges:\n";
			for (Position i = 0; i < edges.size(); i++)
			{
				if (edges[i] != NULL)
				{
					print << pre << "  " << *edges[i] << "\n";
				}
				else
				{
					print << pre << "  --\n";
				}
			}
			print << pre << "singular Edges:\n";
			for (Position i = 0; i < singular_edges.size(); i++)
			{
				if (singular_edges[i] != NULL)
				{
					print << pre << "  " << *singular_edges[i] << "\n";
				}
				else
				{
					print << pre << "  --\n";
				}
			}
			print << pre << "contact Faces:\n";
			for (Position i = 0; i < contact_faces.size(); i++)
			{
				print << pre << "  " << *contact_faces[i] << "\n";
			}
			print << pre << "toric Faces:\n";
			for (Position i = 0; i < toric_faces.size(); i++)
			{
				if (toric_faces[i] != NULL)
				{
					print << pre << "  " << *toric_faces[i] << "\n";
				}
				else
				{
					print << pre << "  --\n";
				}
			}
			print << pre << "singular toric Faces:\n";
			for (Position i = 0; i < singular_toric_faces.size(); i++)
			{
				print << pre << "  " << *singular_toric_faces[i] << "\n";
			}
			print << pre << "spheric Faces:\n";
			for (Position i = 0; i < spheric_faces.size(); i++)
			{
				if (spheric_faces[i] != NULL)
				{
					print << pre << "  " << *spheric_faces[i] << "\n";
				}
				else
				{
					print << pre << "  --\n";
				}
			}
		}


    //@}

		/**	@name	Attributes
		*/
		//@{

		vector< TSESVertex<T>* > vertices;
		vector< TSESEdge<T>* > edges;
		vector< TSESEdge<T>* > singular_edges;
		vector< TSESFace<T>* > contact_faces;
		vector< TSESFace<T>* > toric_faces;
		vector< TSESFace<T>* > singular_toric_faces;
		vector< TSESFace<T>* > spheric_faces;

    std::ofstream print;
    string pre;

    //@}
	};


	/**	The Default SolventExcludedSurface type.
			If double precision is not needed, {\tt TSolventExcludedSurface<float>} should
			be used. It is predefined as {\tt SolventExcludedSurface} for convenience.
	*/
	typedef TSolventExcludedSurface<float> SolventExcludedSurface;

} // namespace BALL

#endif  // BALL_STRUCTURE_SOLVENTEXCLUDEDSURFACE_H
