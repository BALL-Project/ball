// $Id:

#ifndef BALL_STRUCTURE_TRIANGLEPOINT_H
#define BALL_STRUCTURE_TRIANGLEPOINT_H

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

	template <class T>
	class TTriangleEdge;

	template <class T>
	class TTriangle;

	template <class T>
	class TTriangulatedSurface;

	template <class T>
	class TTriangulatedSES;


	/** Generic TriangleEdge Class.
			\\
			{\bf Definition:}\URL{BALL/STRUCTURE/triangle.h}
			\\
	*/
	template <class T>
	class TTrianglePoint	:	public GraphVertex< TTriangleEdge<T>,TTriangle<T> >
	{
		
		public:

		/** @name Class friends
				\begin{itemize}
					\item class GraphEdge< TTrianglePoint<T>,TTriangle<T> >
					\item class GraphFace< TTrianglePoint<T>,TTriangleEdge<T> >
					\item class GraphVertex< TTriangleEdge<T>,TTriangle<T> >
					\item class TTriangulatedSurface<T>
					\item class TTriangulatedSES<T>
					\item class TTriangle<T>;
					\item class TTriangleEdge<T>;
				\end{itemize}
		*/
		friend class GraphEdge< TTrianglePoint<T>,TTriangle<T> >;
		friend class GraphFace< TTrianglePoint<T>,TTriangleEdge<T> >;
		friend class GraphVertex< TTriangleEdge<T>,TTriangle<T> >;
		friend class TTriangulatedSurface<T>;
		friend class TTriangulatedSES<T>;
		friend class TTriangle<T>;
		friend class TTriangleEdge<T>;

		BALL_CREATE(TTrianglePoint)

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
				This method creates a new TrianglePoint object.
		*/
		TTrianglePoint()
			throw();

		/**	Copy constructor.
				Create a new TrianglePoint object from another.
				@param	point	the TrianglePoint object to be copied
				@param	deep	if deep = false, all pointers are set to NULL (default). Otherwise the new	
											TrianglePoint object is linked to the neighbours of the old TrianglePoint object.
		*/
		TTrianglePoint(const TTrianglePoint<T>& point, bool deep = false)
			throw();

		/**	Destructor.
				Destructs the TrianglePoint object.
		*/
		virtual ~TTrianglePoint()
			throw();

		//@}

		/**	@name	Accessors
		*/
		//@{

		/** Get the point
		*/
		TVector3<T> getPoint() const
			throw();

		/** Set the point
		*/
		void setPoint(const TVector3<T>& point)
			throw();

		/** Get the normal of the TrianglePoint
		*/
		TVector3<T> getNormal() const
			throw();

		/** Set the normal of the TrianglePoint
		*/
		void setNormal(const TVector3<T>& normal)
			throw();

		//@}

		/**	@name	Predicates
		*/
		//@{

		/** Equality operator
		*/
		virtual bool operator == (const TTrianglePoint& point) const
			throw();

		/** Inequality operator
		*/
		virtual bool operator != (const TTrianglePoint& point) const
			throw();

		//@}

		protected:

		/*_ The point itselfe
		*/
		TVector3<T> point_;
		/*_ The normal vector of the point
		*/
		TVector3<T> normal_;
		/*_ The state of the point
		*/
		Index state_;

	};


	/**	@name	Storers
	*/
	//@{

	/**	Output- Operator
	*/
	template <class T>
	std::ostream& operator << (std::ostream& s, const TTrianglePoint<T>& point)
	{
		s << "POINT";
		s << point.getIndex();
		s << "( " << point.getPoint() << " " << point.getNormal() << " {";
		std::list<TTriangleEdge<T>*> edges = point.getEdges();
		typename std::list<TTriangleEdge<T>*>::const_iterator e;
		for (e = edges.begin(); e != edges.end(); e++)
		{
			s << (*e)->getIndex() << " ";
		}
		s << "} [";
		std::list<TTriangle<T>*> triangles = point.getFaces();
		typename std::list<TTriangle<T>*>::const_iterator t;
		for (t = triangles.begin(); t != triangles.end(); t++)
		{
			s << (*t)->getIndex() << " ";
		}
		s << "] )";
		return s;
	}

	//@}


	/**	The Default Triangle Type.
			If double precision is not needed, {\tt TTrianglePoint<float>} should	
			be used. It is predefined as {\tt TrianglePoint} for convenience.
	*/
	typedef TTrianglePoint<float> TrianglePoint;




	template <class T>
	TTrianglePoint<T>::TTrianglePoint()
		throw()
		:	GraphVertex< TTriangleEdge<T>,TTriangle<T> >(),
			point_(),
			normal_()
	{
	}


	template <class T>
	TTrianglePoint<T>::TTrianglePoint(const TTrianglePoint<T>& point, bool deep)
		throw()
		:	GraphVertex< TTriangleEdge<T>,TTriangle<T> >(point,deep),
			point_(point.point_),
			normal_(point.normal_)
	{
	}


	template <class T>
	TTrianglePoint<T>::~TTrianglePoint()
		throw()
	{
	}


	template <class T>
	TVector3<T> TTrianglePoint<T>::getPoint() const
		throw()
	{
		return point_;
	}


	template <class T>
	void TTrianglePoint<T>::setPoint(const TVector3<T>& point)
		throw()
	{
		point_ = point;
	}


	template <class T>
	TVector3<T> TTrianglePoint<T>::getNormal() const
		throw()
	{
		return normal_;
	}


	template <class T>
	void TTrianglePoint<T>::setNormal(const TVector3<T>& normal)
		throw()
	{
		normal_ = normal;
	}


	template <class T>
	bool TTrianglePoint<T>::operator == (const TTrianglePoint<T>& point) const
		throw()
	{
		return (point_ == point.point_);
	}


	template <class T>
	bool TTrianglePoint<T>::operator != (const TTrianglePoint<T>& point) const
		throw()
	{
		return (point_ != point.point_);
	}







}	// namespace BALL


#endif	// BALL_STRUCTURE_TRIANGLEPOINT_H

