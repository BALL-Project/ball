// $Id:

#ifndef BALL_STRUCTURE_TRIANGLE_H
#define BALL_STRUCTURE_TRIANGLE_H

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
	class TTrianglePoint;

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
	class TTriangle	:	public GraphFace< TTrianglePoint<T>,TTriangleEdge<T> >
	{
		
		public:

		/** @name Class friends
				\begin{itemize}
					\item class GraphEdge< TTrianglePoint<T>,TTriangle<T> >
					\item class GraphFace< TTrianglePoint<T>,TTriangleEdge<T> >
					\item class GraphVertex< TTriangleEdge<T>,TTriangle<T> >
					\item class TTriangulatedSurface<T>
					\item class TTriangulatedSES<T>
					\item class TTrianglePoint<T>;
					\item class TTriangleEdge<T>;
				\end{itemize}
		*/
		friend class GraphEdge< TTrianglePoint<T>,TTriangle<T> >;
		friend class GraphFace< TTrianglePoint<T>,TTriangleEdge<T> >;
		friend class GraphVertex< TTriangleEdge<T>,TTriangle<T> >;
		friend class TTriangulatedSurface<T>;
		friend class TTriangulatedSES<T>;
		friend class TTrianglePoint<T>;
		friend class TTriangleEdge<T>;

		BALL_CREATE(TTriangle)

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
				This method creates a new TriangleEdge object.
		*/
		TTriangle()
			throw();

		/**	Copy constructor.
				Create a new TriangleEdge object from another.
				@param	edge	the TriangleEdge object to be copied
				@param	deep	if deep = false, all pointers are set to NULL (default). Otherwise the new	
											TriangleEdge object is linked to the neighbours of the old TriangleEdge object.
		*/
		TTriangle(const TTriangle<T>& edge, bool deep = false)
			throw();

		/**	Destructor.
				Destructs the TriangleEdge object.
		*/
		virtual ~TTriangle()
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
			throw(Exception::IndexOverflow);

		/** Return one of the two points of the TriangleEdge.
				@param	i
				@return	TTrianglePoint<T>*	a pointer to the first point if i = 0,	
																		a pointer to the second point otherwise
		*/
		TTrianglePoint<T>* getPoint(Position i) const
			throw(Exception::IndexOverflow);

		/**	Remove an edge from the Triangle.
				The edge is set to NULL.
				@param	edge	the edge to delete
		*/
		void remove(TTriangleEdge<T>* edge)
			throw();

		/** Return the third point of the Triangle.
				@param	p1									a pointer to the first point
				@param	p2									a pointer to the second point
				@return	TTrianglePoint<T>*	a pointer to the third point
		*/
		TTrianglePoint<T>* thirdPoint(TTrianglePoint<T>* p1, TTrianglePoint<T>* p2)
			throw();

		/** Get the edge of the Triangle which lies on the opposite side of a point.
				@param	point							a point of the Triangle
				@return	TTriangleEdge<T>*	a pointer to the opposite Edge
		*/
		TTriangleEdge<T>* getOppositeEdge(TTrianglePoint<T>* point)
			throw();

		//@}

	};


	/**	@name	Storers
	*/
	//@{

	/**	Output- Operator
	*/
	template <class T>
	std::ostream& operator << (std::ostream& s, const TTriangle<T>& triangle)
	{
		s << "TRIANGLE" << triangle.getIndex()
			<< "([" << (triangle.getVertex(0) == NULL ? -2 : triangle.getVertex(0)->getIndex()) << ' '
			<<				 (triangle.getVertex(1) == NULL ? -2 : triangle.getVertex(1)->getIndex()) << ' '
			<<				 (triangle.getVertex(2) == NULL ? -2 : triangle.getVertex(2)->getIndex()) << "] "
			<< "[" << (triangle.getEdge(0) == NULL ? -2 : triangle.getEdge(0)->getIndex()) << ' '
			<<				(triangle.getEdge(1) == NULL ? -2 : triangle.getEdge(1)->getIndex()) << ' '
			<<				(triangle.getEdge(2) == NULL ? -2 : triangle.getEdge(2)->getIndex()) << "] ";
		return s;
	}

	//@}


	/**	The Default Triangle Type.
			If double precision is not needed, {\tt TTriangle<float>} should	
			be used. It is predefined as {\tt Triangle} for convenience.
	*/
	typedef TTriangle<float> Triangle;




	template <class T>
	TTriangle<T>::TTriangle()
		throw()
		:	GraphFace< TTrianglePoint<T>,TTriangleEdge<T> >()
	{
		vertex_.push_back(NULL);
		vertex_.push_back(NULL);
		vertex_.push_back(NULL);
		number_of_vertices_ = 3;
		edge_.push_back(NULL);
		edge_.push_back(NULL);
		edge_.push_back(NULL);
		number_of_edges_ = 3;
	}


	template <class T>
	TTriangle<T>::TTriangle(const TTriangle<T>& edge, bool deep)
		throw()
		:	GraphFace< TTrianglePoint<T>,TTriangleEdge<T> >(edge,deep)
	{
	}


	template <class T>
	TTriangle<T>::~TTriangle()
		throw()
	{
	}


	template <typename T>
	void TTriangle<T>::setPoint(Position i, TTrianglePoint<T>* vertex)
		throw(Exception::IndexOverflow)
	{
		switch (i)
		{
			case 0	:	vertex_[0] = vertex; break;
			case 1	:	vertex_[1] = vertex; break;
			case 2	:	vertex_[2] = vertex; break;
			default	:	throw Exception::IndexOverflow(__FILE__,__LINE__,i,2);
		}
	}


	template <typename T>
	TTrianglePoint<T>* TTriangle<T>::getPoint(Position i) const
		throw(Exception::IndexOverflow)
	{
		switch (i)
		{
			case 0	:	return vertex_[0];
			case 1	:	return vertex_[1];
			case 2	:	return vertex_[2];
			default	:	throw Exception::IndexOverflow(__FILE__,__LINE__,i,2);
		}
	}


	template <class T>
	void TTriangle<T>::remove(TTriangleEdge<T>* edge)
		throw()
	{
		for (Position i = 0; i < 3; i++)
		{
			if (edge_[i] == edge)
			{
				edge_[i] = NULL;
			}
		}
	}


	template <typename T>
	TTrianglePoint<T>* TTriangle<T>::thirdPoint(TTrianglePoint<T>* p1, TTrianglePoint<T>* p2)
		throw()
	{
		if ((*vertex_[0] == *p1) || (*vertex_[0] == *p2))
		{
			if ((*vertex_[1] == *p1) || (*vertex_[1] == *p2))
			{
				return vertex_[2];
			}
			else
			{
				return vertex_[1];
			}
		}
		else
		{
			return vertex_[0];
		}
	}


	template <class T>
	TTriangleEdge<T>* TTriangle<T>::getOppositeEdge(TTrianglePoint<T>* point)
		throw()
	{
		for (Position i = 0; i < 3; i++)
		{
			if ((edge_[i]->vertex_[0] != point) && (edge_[i]->vertex_[1] != point))
			{
				return edge_[i];
			}
		}
		return NULL;
	}





}	// namespace BALL


#endif	// BALL_STRUCTURE_TRIANGLE_H


