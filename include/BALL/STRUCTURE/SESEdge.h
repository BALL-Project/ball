// $Id: SESEdge.h,v 1.8 2001/06/21 08:58:41 amoll Exp $

#ifndef BALL_STRUCTURE_SESEDGE_H
#define BALL_STRUCTURE_SESEDGE_H

#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector3.h>
#endif

#ifndef BALL_MATHS_CIRCLE3_H
#	include <BALL/MATHS/circle3.h>
#endif

namespace BALL
{

	template <typename T>
	class TSESFace;

	template <typename T>
	class TSESVertex;

	template <typename T>
	class TRSEdge;

	/** Generic SESEdge Class.
			{\bf Definition:} \URL{BALL/STRUCTURE/SESEdge.h} 
	*/
	template <class T>
	class TSESEdge
	{
		public:

		BALL_CREATE(TSESEdge)

		/**	@name	Enums
		*/
		//@{

		/** type of SESEdge:
				{\tt 0} concave
				{\tt 1} convex
				{\tt 2} singular
		*/
		enum Type
		{
			TYPE_CONCAVE  = 0,
			TYPE_CONVEX = 1,
			TYPE_SINGULAR = 2
		};

		//@}
		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
				This method creates a new SESEdge object. All components are
				initialized to {\tt (T)0} or {\tt NULL}, respectivly.
		*/
		TSESEdge()
			: vertex1(NULL), vertex2(NULL), face1(NULL), face2(NULL), circle(), rsedge(NULL), type(0), index(-1)
		{
		}

		/**	Copy constructor.
				Create a new SESEdge object from another.
				@param sesedge the SESEdge object to be copied
				@param bool ignored - just for interface consistency
		*/
		TSESEdge(const TSESEdge<T>& sesedge, bool /* deep */ = true)
			: vertex1(sesedge.vertex1), vertex2(sesedge.vertex2), face1(sesedge.face1), face2(sesedge.face2),
				circle(sesedge.circle), rsedge(sesedge.rsedge), type(sesedge.type), index(sesedge.index)
		{
		}

		/**	Detailled constructor.
				Create a new SESEdge object from two pointers to sesvertices, two pointers to sesedges,
				a noraml vector, a center and a radius of a circle and a rsedge
				@param	v1 assigned to the first vertex
				@param	v2 assigned to the second vertex
				@param	f1 assigned to the first face
				@param	f2 assigned to the second face
				@param	c assigned to the circle
				@param	rse assigned to the rsedge
				@param	i assigned to the index
		*/
		TSESEdge(TSESVertex<T>* v1, TSESVertex<T>* v2, TSESFace<T>* f1, TSESFace<T>* f2,
						 const TCircle3<T>& c, TRSEdge<T>* rse, const int& i)
			: vertex1(v1), vertex2(v2), face1(f1), face2(f2), circle(c), rsedge(rse), type(0), index(i)
		{
		}

		/**	Destructor.
				Destructs the SESEdge object. As there are no dynamic
				data structures, nothing happens.
		*/
		virtual ~TSESEdge()
		{
		}

		//@}
		/**	@name	Assignment
		*/
		//@{

		//@}
		/**	@name	Accessors
		*/
		//@{

		TSESFace<T>* other(TSESFace<T>* face)
		{
			if (face1 == face)
			{
				return face2;
			}
			else
			{
				return face1;
			}
		}

		bool substituteVertex(TSESVertex<T>* old, TSESVertex<T>* new_)
		{
			if (vertex1 == old)
			{
				vertex1 = new_;
				return true;
			}
			if (vertex2 == old)
			{
				vertex2 = new_;
				return true;
			}
			return false;
		}

		//@}
		/**	@name	Predicates
		*/
		//@{

		/**	similar.
				@return bool, {\bf true} if all vertices are representing the same pointequal modulo order,
											{\bf false} otherwise
		*/
		bool similar(const TSESEdge& sesedge) const
		{
			if ((vertex1->p != sesedge.vertex1->p) && (vertex1->p != sesedge.vertex2->p))
				{
					return false;
				}
			if ((vertex2->p != sesedge.vertex1->p) && (vertex2->p != sesedge.vertex2->p))
				{
					return false;
				}
			return true;
		}

		bool isFree()
		{
			if (rsedge == NULL)
			{
				return false;
			}
			return rsedge->isFree();
		}

		//@}
		/**	@name	Attributes
		*/
		//@{

		/**	vertex1.
				A pointer to the first vertex that belongs to the edge.
		*/
		TSESVertex<T>* vertex1;

		/**	vertex2.
				A pointr to the second vertex that belongs to the edge.
		*/
		TSESVertex<T>* vertex2;

		/**	face1.
				A pointer to the first face the edge belongs to.
		*/
		TSESFace<T>* face1;

		/**	face2.
				A pointer to the second face the edge belongs to.
		*/
		TSESFace<T>* face2;

		/** circle
				The circle on which the edge lies.
		*/
		TCircle3<T> circle;

		/** rsdge.
				A pointer to the corresponding RSEdge.
		*/
		TRSEdge<T>* rsedge;
		
		int type;
		
		/** index.
		*/
		int index;

		//@}
	};

	/**	@name	Storers
	*/
	//@{

	/*	Input- Operator
			reads in a TVector3 and a {\bf T} value : p, radius
	*/
/*
	template <typename T>
	std::istream& operator >> (std::istream& s, TRSEdge<T>& rsedge)
	{
		char c;
		for (int i=0; i<7 ; i++)
		{
			s >> c;
		}
		s >> rsedge.p >> rsedge.radius >> c;
		return s;
	}
*/

	/**	Output- Operator
	*/
	template <typename T>
	std::ostream& operator << (std::ostream& s, const TSESEdge<T>& sesedge)
	{
		return (s << "SESEDGE" << sesedge.index << "(["
							<< ((sesedge.vertex1 == NULL) ? -2 : sesedge.vertex1->index) << ' '
							<< ((sesedge.vertex2 == NULL) ? -2 : sesedge.vertex2->index) << "] ["
							<< ((sesedge.face1 == NULL) ? -2 : sesedge.face1->index) << ' '
							<< ((sesedge.face2 == NULL) ? -2 : sesedge.face2->index) << "] "
							<< sesedge.circle << ' ' << ((sesedge.rsedge == NULL) ? -2 : sesedge.rsedge->getIndex())
							<< ((sesedge.type == 0) ? " convex)" : ((sesedge.type == 1) ? " concave)"
																																					: " singular)")));
	}
	//@}


	/**	The Default SESEdge Type.
			If double precision is not needed, {\tt SESEdge<float>} should
			be used. It is predefined as {\tt SESEdge} for convenience.
	*/
	typedef TSESEdge<float> SESEdge;

} // namespace BALL

#endif // BALL_STRUCTURE_SESEDGE_H
