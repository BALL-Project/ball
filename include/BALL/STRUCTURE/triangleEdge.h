// $Id:

#ifndef BALL_STRUCTURE_TRIANGLEEDGE_H
#define BALL_STRUCTURE_TRIANGLEEDGE_H

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
	class TTriangle;

	template <class T>
	class TTrianglePoint;

	template <class T>
	class TTriangulatedSurface;

	template <class T>
	class TTriangulatedSES;


	/** Generic TriangleEdge Class.
			\\
			{\bf Definition:}\URL{BALL/STRUCTURE/triangleEdge.h}
			\\
	*/
	template <class T>
	class TTriangleEdge	:	public GraphEdge< TTrianglePoint<T>,TTriangle<T> >
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
		friend class TTrianglePoint<T>;

		BALL_CREATE(TTriangleEdge)

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
				This method creates a new TriangleEdge object.
		*/
		TTriangleEdge()
			throw();

		/**	Copy constructor.
				Create a new TriangleEdge object from another.
				@param	edge	the TriangleEdge object to be copied
				@param	deep	if deep = false, all pointers are set to NULL (default). Otherwise the new	
											TriangleEdge object is linked to the neighbours of the old TriangleEdge object.
		*/
		TTriangleEdge(const TTriangleEdge<T>& edge, bool deep = false)
			throw();

		/**	Destructor.
				Destructs the TriangleEdge object.
		*/
		virtual ~TTriangleEdge()
			throw();
		//@}

		/**	@name	Accessors
		*/
		//@{

		/** Set one of the two points of the TriangleEdge.
				@param	i			the first point is changed if i = 0, the second otherwise
				@param	point	a pointer to the new point
		*/
		void setPoint(Position i, TTrianglePoint<T>* point)
			throw();

		/** Return one of the two points of the TriangleEdge.
				@param	i
				@return	TTrianglePoint<T>*	a pointer to the first point if i = 0,	
																		a pointer to the second point otherwise
		*/
		TTrianglePoint<T>* getPoint(Position i) const
			throw();

		/** Set one of the two triangles of the TriangleEdge.
				@param	i					change the first triangle, if i = 0, the second otherwise
				@param	triangle	a pointer to the new triangle
		*/
		void setTriangle(Position i, TTriangle<T>* triangle)
			throw();

		/** Return one of the two triangles of the TriangleEdge.
				@param	i
				@return	TTriangle<T>*	a pointer to the first triangle if i = 0,	
															a pointer to the second triangle otherwise
		*/
		TTriangle<T>* getTriangle(Position i) const
			throw();

		//void del(TTriangle<T>* t)
		//	throw();
		//@}

		/**	@name	Predicates
		*/
		//@{

		/** Equality operator
		*/
		virtual bool operator == (const TTriangleEdge<T>& e)
			throw();

		/** Inequality operator
		*/
		virtual bool operator != (const TTriangleEdge<T>& e)
			throw();

		//@}

	};


	template <class T>
	TTriangleEdge<T>::TTriangleEdge()
		throw()
		:	GraphEdge< TTrianglePoint<T>,TTriangle<T> >()
	{
	}


	template <class T>
	TTriangleEdge<T>::TTriangleEdge(const TTriangleEdge<T>& edge, bool deep)
		throw()
		:	GraphEdge< TTrianglePoint<T>,TTriangle<T> >(edge,deep)
	{
	}


	template <class T>
	TTriangleEdge<T>::~TTriangleEdge()
		throw()
	{
	}


	//template <class T>
	//void TTriangleEdge<T>::del(TTriangle<T>* t)
	//	throw()
	//{
	//	typename std::vector<TTriangle<T>*>::iterator i = triangle.begin();
	//	while (i != triangle.end())
	//	{
	//		if (*i == t)
	//		{
	//			triangle.erase(i);
	//			i = triangle.end();
	//		}
	//		else
	//		{
	//			i++;
	//		}
	//	}
	//}


	template <class T>
	bool TTriangleEdge<T>::operator == (const TTriangleEdge<T>& e)
		throw()
	{
		return ( ((vertex_[0]->point_ == e.vertex_[0]->point_) &&
							(vertex_[1]->point_ == e.vertex_[1]->point_))		||
						 ((vertex_[0]->point_ == e.vertex_[1]->point_) &&
							(vertex_[1]->point_ == e.vertex_[0]->point_))				);
	}


	template <class T>
	bool TTriangleEdge<T>::operator != (const TTriangleEdge<T>& e)
		throw()
	{
		return ( !(*this == e));
	}


	template <class T>
	std::ostream& operator << (std::ostream& s, const TTriangleEdge<T>& edge)
	{
		s << "EDGE" << edge.getIndex() << "("
			<< (edge.getVertex(0) == NULL ? -2 : edge.getVertex(0)->getIndex()) << "-"
			<< (edge.getVertex(1) == NULL ? -2 : edge.getVertex(1)->getIndex()) << " "
			<< (edge.getFace(0) == NULL ? -2 : edge.getFace(0)->getIndex()) << "|"
			<< (edge.getFace(1) == NULL ? -2 : edge.getFace(1)->getIndex()) << ")";
		return s;
	}


}	// namespace BALL


#endif	// BALL_STRUCTURE_TRIANGULATEDSURFACE_H


