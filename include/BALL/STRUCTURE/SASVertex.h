// $Id: SASVertex.h,v 1.1.2.1 2002/08/22 17:17:55 oliver Exp $

#ifndef BALL_STRUCTURE_SASVERTEX_H
#define BALL_STRUCTURE_SASVERTEX_H

#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector3.h>
#endif

#ifndef BALL_STRUCTURE_GRAPHEDGE_H
#	include <BALL/STRUCTURE/graphEdge.h>
#endif

#ifndef BALL_STRUCTURE_GRAPHFACE_H
#	include <BALL/STRUCTURE/graphFace.h>
#endif

#ifndef BALL_STRUCTURE_GRAPHVERTEX_H
#	include <BALL/STRUCTURE/graphVertex.h>
#endif

namespace BALL
{

	template <typename T>
	class TSolventAccessibleSurface;

	template <typename T>
	class TSASEdge;

	template <typename T>
	class TSASFace;

	template <typename T>
	class TTriangulatedSAS;

	/** Generic SASVertex Class.	
      {\bf Definition:} \URL{BALL/STRUCTURE/SASVertex.h}
	*/
	template <typename T>
	class TSASVertex	:	public GraphVertex< TSASEdge<T>,TSASFace<T> >
	{
		public:

		/** @name Class friends
				\begin{itemize}
					\item class GraphEdge< TRSVertex<T>,TRSFace<T> >
					\item class GraphFace< TRSVertex<T>,TRSEdge<T> >
					\item class GraphVertex< TRSEdge<T>,TRSFace<T> >
					\item class TSolventAccessibleSurface<T>
					\item class TSASEdge<T>
					\item class TSASFace<T>
					\item class TTriangulatedSAS<T>
				\end{itemize}
		*/
		friend class GraphEdge< TSASVertex<T>,TSASFace<T> >;
		friend class GraphFace< TSASVertex<T>,TSASEdge<T> >;
		friend class GraphVertex< TSASEdge<T>,TSASFace<T> >;
		friend class TSolventAccessibleSurface<T>;
		friend class TSASEdge<T>;
		friend class TSASFace<T>;
		friend class TTriangulatedSAS<T>;

		BALL_CREATE(TSASVertex)

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
				This method creates a new SASVertex object.
		*/
		TSASVertex()
			throw();

		/**	Copy constructor.
				Create a new SASVertex object from another.
				@param	sasvertex	the SASVertex object to be copied
				@param	bool			ignored - just for interface consistency
		*/
		TSASVertex(const TSASVertex<T>& sasvertex, bool = false)
			throw();

		/**	Detailled constructor.
				Create a new SASVertex object from a lot of nice objects.
				@param	point		assigned to the point representet by the vertex
				@param	normal	assigned to the normal vector of the vertex
				@param	atom		assigned to the index of the closest atom
				@param	index		assigned to the index of the vertex
		*/
		TSASVertex(const TVector3<T>& point,
				Index index)
			throw();

		/**	Destructor.
				Destructs the RSVertex object.
		*/
		virtual ~TSASVertex()
			throw();

		//@}
		/**	@name	Assignment
		*/
		//@{

		/**	Swap the contents of two SASVertices.
				@param	sasvertex	the SASVertex to swap contents with
		*/
		//void swap(TSASVertex<T>& sasvertex)
		//	throw();

		/**	Assign from another SASVertex.
				@param	sasvertex	the SASVertex object to assign from
				@param	bool			ignored
		*/
		void set(const TSASVertex<T>& sasvertex, bool = false)
			throw();

		/**	Assign to two vectors and two indices.
				@param	point		assigned to the point represented by the vertex
				@param	index		assigned to the index of the vertex
		*/
		void set(const TVector3<T> point,
				Index index)
			throw();

		/**	Set the point represented by the SASVertex.
				@param	point	the new point
		*/
		void setPoint(const TVector3<T>& point)
			throw();

		/**	Return the point represented by the SASVertex.
				@return	TVector3<T>	the point represented by the vertex
		*/
		TVector3<T> getPoint() const
			throw();

		//@}
		/**	@name	Predicates
		*/
		//@{

		protected:

		//@}
		/**	@name	Attributes
		*/
		//@{

		/*_	The coordinates of the vertex.
		*/
		TVector3<T> point_;

		//@}
	};

	/**	@name	Storers
	*/
	//@{

	/**	Output- Operator
	*/
	template <typename T>
	std::ostream& operator << (std::ostream& s, const TSASVertex<T>& sasvertex)
	{
		s << "SASVERTEX" << sasvertex.getIndex() << "(" << sasvertex.getPoint() << " [";
		HashSet< TSASEdge<T>* > edges = sasvertex.getEdges();
		for (typename HashSet<TSASEdge<T>*>::Iterator i = edges.begin(); i != edges.end(); i++)
		{
			s << (*i)->getIndex() << ' ';
		}
		s << "] [";
		HashSet< TSASFace<T>* > faces = sasvertex.getFaces();
		for (typename HashSet<TSASFace<T>*>::Iterator i = faces.begin(); i != faces.end(); i++)
		{
			s << (*i)->getIndex() << ' ';
		}
		s << "] )";
		return s;
	}


	/**	The Default SASVertex Type.
			If double precision is not needed, {\tt SASVertex<float>} should
			be used. It is predefined as {\tt SASVertex} for convenience.
	*/
	typedef TSASVertex<float> SASVertex;

	//@}



	template <typename T>
	TSASVertex<T>::TSASVertex()
		throw()
		: GraphVertex< TSASEdge<T>,TSASFace<T> >(),
			point_()
	{
	}


	template <typename T>
	TSASVertex<T>::TSASVertex(const TSASVertex<T>& sasvertex, bool)
		throw()
		: GraphVertex< TSASEdge<T>,TSASFace<T> >(sasvertex,false),
			point_(sasvertex.point_)
	{
	}


	template <typename T>
	TSASVertex<T>::TSASVertex(const TVector3<T>& point,
			Index index)
		throw()
		: GraphVertex< TSASEdge<T>,TSASFace<T> >(),
			point_(point)
	{
		index_ = index;
	}


	template <typename T>
	TSASVertex<T>::~TSASVertex()
		throw()
	{
	}


	/*template <typename T>
	void TSASVertex<T>::swap(TSASVertex<T>& sasvertex)
		throw()
	{
		TVector3<T> temp_point(point_);
		point_ = sasvertex.point_;
		sasvertex.point_ = temp_point;

		temp = index_;
		index_ = sasvertex.index_;
		sasvertex.index_ = temp;
	}*/


	template <typename T>
	void TSASVertex<T>::set(const TSASVertex<T>& sasvertex, bool)
		throw()
	{
		point_ = sasvertex.point_;
		index_ = sasvertex.index_;
	}


	template <typename T>
	void TSASVertex<T>::set(const TVector3<T> point,
			Index index)
		throw()
	{
		point_ = point;
		index_ = index;
	}


	template <typename T>
	void TSASVertex<T>::setPoint(const TVector3<T>& point)
		throw()
	{
		point_ = point;
	}


	template <typename T>
	TVector3<T> TSASVertex<T>::getPoint() const
		throw()
	{
		return point_;
	}




} // namespace BALL

#endif // BALL_STRUCTURE_SASVERTEX_H
