// $Id: SESFace.h,v 1.6 2001/01/29 00:32:27 amoll Exp $

#ifndef BALL_STRUCTURE_SESFACE_H
#define BALL_STRUCTURE_SESFACE_H

#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector3.h>
#endif

#ifndef BALL_STRUCTURE_SESEDGE_H
#	include <BALL/STRUCTURE/SESEdge.h>
#endif

namespace BALL
{

	template <typename T>
	class TRSEdge;

	template <typename T>
	class TRSVertex;

	template <typename T>
	class TRSFace;

	/** Generic SESFace Class.
			\\
			{\bf Definition:} \URL{BALL/STRUCTURE/SESFace.h}
			\\
	*/
	template <class T>
	class TSESFace
	{
		public:

		BALL_CREATE(TSESFace)

		/**	@name	Enums
		*/
		//@{

		/** type of SESFace:
				{\tt 0} spheric reentrant face
				{\tt 1} toric reentrant face
				{\tt 2} contact face
		*/
		enum Type
		{
			TYPE_SPHERIC  = 0,
			TYPE_TORIC = 1,
			TYPE_CONTACT = 2,
			TYPE_TORIC_SINGULAR = 3
		};
		//@}

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
				This method creates a new SESFace object. All components are
				initialized to {\tt (T)0} or {\tt NULL}, respectivly.
		*/
		TSESFace()
			: rsvertex(), rsedge(), rsface(), index(-1)
		{
		}

		/**	Copy constructor.
				Create a new SESFace object from another.
				@param sesface the SESFace object to be copied
				@param bool ignored - just for interface consistency
		*/
		TSESFace(const TSESFace<T>& sesface,bool /* deep */ = true)
			: type(sesface.type), rsvertex(sesface.rsvertex), rsedge(sesface.rsedge), rsface(sesface.rsface),
				vertex(sesface.vertex), edge(sesface.edge), orientation(sesface.orientation), index(sesface.index)
		{
		}

		/**	Detailled constructor.
				Create a new SESFace object
				@param	t	assigned the type
				@param	rsv assigned to the rsvertex
				@param	rse assigned to the rsedge
				@param	rsf assigned to the rsface
				@param	v assigned to the list of vertices
				@param	e assigned to the list of edges
				@param	o assigned to the list of orientations
				@param	i assigned to the index
		*/
		TSESFace(const Type& t, TRSVertex<T> *rsv, TRSEdge<T> *rse, TRSFace<T> *rsf,
						 vector< TSESVertex<T>* > v, vector< TSESEdge<T>* > e, vector<bool> o, const int& i)
			: type(t), rsvertex(rsv), rsedge(rse), rsface(rsf), vertex(v), edge(e), orientation(o), index(i)
		{
		}

		/**	Detailled constructor.
				Create a new SESFace object
				@param	t	assigned the type
				@param	rsv assigned to the rsvertex
				@param	rse assigned to the rsedge
				@param	rsf assigned to the rsface
		*/
		TSESFace(const Type& t, TRSVertex<T> *rsv, TRSEdge<T> *rse, TRSFace<T> *rsf)
			: type(t), rsvertex(rsv), rsedge(rse), rsface(rsf), index(-1)
		{
		}

		/**	Destructor.
				Destructs the SESFace object. As there are no dynamic
				data structures, nothing happens.
		*/
		virtual ~TSESFace()
		{
		}
		//@}

		/**	@name	Accessors
		*/
		//@{

		bool getEdge(const Index v1, const Index v2, Index& e)
		{
			bool test1 = false;
			bool test2 = false;
			for (Position i = 0; i < vertex.size(); i++)
				{
					if (vertex[i]->index == v1)
						{
							test1 = true;
						}
					if (vertex[i]->index == v2)
						{
							test2 = true;
						}
				}
			if ((test1 && test2) == false)
				{
					return false;
				}
			for (Position i = 0; i < edge.size(); i++)
				{
					if (edge[i] != NULL)
						{
							if (((edge[i]->vertex1->index == v1) && (edge[i]->vertex2->index == v2)) ||
							    ((edge[i]->vertex2->index == v1) && (edge[i]->vertex1->index == v2))    )
								{
									e = edge[i]->index;
									return true;
								}
						}
				}
			return false;
		}

		Index getRelativeVertexIndex(const Position i)
		{
			for (Position j = 0; j < vertex.size(); j++)
				{
					if (vertex[j]->index == i)
						{
							return j;
						}
				}
			return -1;
		}

		Index getRelativeEdgeIndex(const Index i)
		{
			for (Position j = 0; j < edge.size(); j++)
				{
					if (edge[j] != NULL)
						{
							if (edge[j]->index == i)
								{
									return j;
								}
						}
				}
			return -1;
		}

/*		bool substituteVertex(TSESVertex<T>* old, TSESVertex<T>* new_)
		{
			for (Position i = 0;  i < vertex.size(); i++)
				{
					if (vertex[i] == old)
						{
							vertex[i] = new_;
							return true;
						}
				}
			return false;
		}*/

		bool substituteEdge(TSESEdge<T>* old, TSESEdge<T>* new_)
		{
			bool back = false;
			for (Position i = 0;  i < edge.size(); i++)
				{
					if (edge[i] == old)
						{
							edge[i] = new_;
							back = true;
						}
				}
			if (back)
				{
					if (old->vertex1->p == new_->vertex1->p)
						{
							for (Position i = 0;  i < edge.size(); i++)
								{
									if (edge[i] != NULL)
										{
											edge[i]->substituteVertex(old->vertex1,new_->vertex1);
											edge[i]->substituteVertex(old->vertex2,new_->vertex2);
										}
								}
						}
						else
						{
							for (Position i = 0;  i < edge.size(); i++)
								{
									if (edge[i] != NULL)
										{
											edge[i]->substituteVertex(old->vertex1,new_->vertex2);
											edge[i]->substituteVertex(old->vertex2,new_->vertex1);
										}
								}
						}
					for (Position i = 0;  i < vertex.size(); i++)
						{
							if (vertex[i] == old->vertex1)
								{
									vertex[i] = new_->vertex1;
								}
								else
								{
									if (vertex[i] == old->vertex2)
										{
											vertex[i] = new_->vertex2;
										}
								}
						}
				}
			return back;
		}

		//@}

		/**	@name	Predicates
		*/
		//@{
		bool isNeighbouredTo(TSESFace<T>* face)
		{
			for (Position i = 0; i < edge.size(); i++)
			{
				if (edge[i] != NULL)
				{
					if (edge[i]->face1 == face)
					{
						return true;
					}
					if (edge[i]->face2 == face)
					{
						return true;
					}
				}
			}
			return false;
		}


		bool isFree()
		{
			if (type != TYPE_TORIC)
			{
				return false;
			}
			return rsedge->isFree();
		}
		//@}

		/**	@name	Attributes
		*/
		//@{

		/**	type.
				The type of the face.
		*/
		Type type;

		/**	rsvertex.
				A pointer to the corresponding RSVertex (if type is TYPE_CONTACT).
		*/
		TRSVertex<T> *rsvertex;

		/**	rsedge.
				A pointer to the corresponding RSEdge (if type is TYPE_TORIC).
		*/
		TRSEdge<T> *rsedge;

		/**	rsface.
				A pointer to the corresponding RSFace (if type is TYPE_SPHERIC).
		*/
		TRSFace<T> *rsface;

		/**	vertex.
				A vector of pointers to the SESVertices of the face.
		*/
		vector< TSESVertex<T>* > vertex;

		/**	edge.
				A vector of pointers to the SESEdges of the face.
		*/
		vector< TSESEdge<T>* > edge;

		/**	orientation.
				A vector of booleans to indicate the orientation of each sesedge.
		*/
		vector<bool> orientation;

		/** index.
		*/
		int index;

		//@}
	};

	/**	@name	Storers
	*/
	//@{

	/**	Input- Operator.
			reads in a TVector3 and a {\bf T} value : p, radius.
	*/
	template <typename T>
	std::istream& operator >> (std::istream& s, TSESFace<T>& sesface)
	{
		// BAUSTELLE
		return s;
	}

		/**	Output- Operator
		*/
		template <typename T>
		std::ostream& operator << (std::ostream& s, const TSESFace<T>& sesface)
		{
			s << "SESFACE" << sesface.index << "("
				<< (sesface.type == TSESFace<T>::TYPE_CONTACT ? "contact " :
							 (sesface.type == TSESFace<T>::TYPE_SPHERIC ? "spheric " : "toric "));
			if (sesface.rsvertex == NULL)
			{
				s << "(nil) ";
			}
			else
			{
				s << "(" << sesface.rsvertex->getAtom() << ") ";
			}
			if (sesface.rsedge == NULL)
			{
				s << "(nil) ";
			}
			else
			{
				s << "(" << sesface.rsedge->getIndex() << ") ";
			}
			if (sesface.rsface == NULL)
			{
				s << "(nil) [";
			}
			else
			{
				s << "(" << sesface.rsface->getIndex() << ") [";
			}
			for (Position i = 0; i < sesface.vertex.size(); i++)
			{
				s << sesface.vertex[i]->index << ' ';
			}
			s << "] [";
			for (Position i = 0; i < sesface.edge.size(); i++)
			{
				if (sesface.edge[i] == NULL)
				{
					s << "(nil) ";
				}
				else
				{
					s << sesface.edge[i]->index << ' ';
				}
			}
			s << "] [";
			for (Position i = 0; i < sesface.orientation.size(); i++)
				{
					s << sesface.orientation[i] << ' ';
				}
			s << "])";
			return s;
		}
	//@}


	/**	The Default SESFace Type.
			If double precision is not needed, {\tt SESFace<float>} should
			be used. It is predefined as {\tt SESFace} for convenience.
	*/
	typedef TSESFace<float> SESFace;

} // namespace BALL

#endif // BALL_STRUCTURE_SESFACE_H
