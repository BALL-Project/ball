// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: SASEdge.h,v 1.2 2002/12/12 09:48:54 oliver Exp $

#ifndef BALL_STRUCTURE_SASEDGE_H
#define BALL_STRUCTURE_SASEDGE_H

#ifndef BALL_MATHS_CIRCLE3_H
#	include <BALL/MATHS/circle3.h>
#endif

#ifndef BALL_MATHS_ANGLE_H
#	include <BALL/MATHS/angle.h>
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
	class TSASFace;

	template <typename T>
	class TSASVertex;

	template <typename T>
	class TRSEdge;

	template <typename T>
	class TTriangulatedSAS;

	/** Generic SASEdge Class.
			{\bf Definition:} \URL{BALL/STRUCTURE/SASEdge.h}
	*/
	template <class T>
	class TSASEdge	:	public GraphEdge< TSASVertex<T>,TSASFace<T> >
	{
		public:

		/** @name Class friends
				\begin{itemize}
					\item class GraphEdge< TRSVertex<T>,TRSFace<T> >
					\item class GraphFace< TRSVertex<T>,TRSEdge<T> >
					\item class GraphVertex< TRSEdge<T>,TRSFace<T> >
					\item class TSolventAccessibleSurface<T>
					\item class TSASFace<T>
					\item class TSASVertex<T>
					\item class TTriangulatedSAS<T>
				\end{itemize}
		*/
		friend class GraphEdge< TSASVertex<T>,TSASFace<T> >;
		friend class GraphFace< TSASVertex<T>,TSASEdge<T> >;
		friend class GraphVertex< TSASEdge<T>,TSASFace<T> >;
		friend class TSolventAccessibleSurface<T>;
		friend class TSASFace<T>;
		friend class TSASVertex<T>;
		friend class TTriangulatedSAS<T>;

		BALL_CREATE(TSASEdge)

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
				This method creates a new SASEdge object.
		*/
		TSASEdge()
			throw();

		/**	Copy constructor.
				Create a new SASEdge object from another.
				@param	sasedge	the SASEdge object to be copied
				@param	deep		if deep = false, all pointers are set to NULL (default). Otherwise the new
												SASEdge object is linked to the neighbours of the old SASEdge object.
		*/
		TSASEdge(const TSASEdge<T>& sasedge, bool deep = false)
			throw();

		/**	Detailled constructor.
				Create a new SASEdge object from a lot of nice objects.
				@param	vertex0	assigned to the first vertex
				@param	vertex1	assigned to the second vertex
				@param	face0		assigned to the first face
				@param	face1		assigned to the second face
				@param	circle	assigned to the circle
				@param	index		assigned to the index
		*/
		TSASEdge(TSASVertex<T>* vertex0,
				TSASVertex<T>* vertex1,
				TSASFace<T>* face0,
				TSASFace<T>* face1,
				const TCircle3<T>& circle,
				Index index)
			throw();

		/**	Destructor.
				Destructs the SASEdge object.
		*/
		virtual ~TSASEdge()
			throw();

		//@}
		/**	@name	Assignment
		*/
		//@{

		/** Set the circle on wich the SASEdge lies.
				@param	circle	the new circle
		*/
		void setCircle(const TCircle3<T>& center)
			throw();

		/** Return the circle on wich the SASEdge lies.
				@return	TCircle3<T>	the circle of the SASEdge
		*/
		TCircle3<T> getCircle() const
			throw();


		//@}
		/**	@name	Accessors
		*/
		//@{

		//@}
		/**	@name	Predicates
		*/
		//@{

		/** isFree.
				@return	bool	{\bf true} if the RSEdge of the SASEdge is free,	
											{\bf false} otherwise
		*/
		bool isFree() const
			throw();

		//@}
		
		protected:
		
		/**	@name	Attributes
		*/
		//@{

		/*_ The circle on which the SASEdge lies.
		*/
		TCircle3<T> circle_;
		/*_ The angle of the SASEdge.
		*/
		TAngle<T> angle_;

		//@}
	};

	/**	@name	Storers
	*/
	//@{

	/*	Input- Operator
	*/
	template <typename T>
	std::istream& operator >> (std::istream& s, TSASEdge<T>& sasedge)
	{
		throw Exception::NotImplemented(__FILE__, __LINE__);
	}

	/**	Output- Operator
	*/
	template <typename T>
	std::ostream& operator << (std::ostream& s, const TSASEdge<T>& sasedge)
	{
		return (s << "SASEDGE" << sasedge.getIndex() << "(["
							<< ((sasedge.getVertex(0) == NULL) ?
											-2 : sasedge.getVertex(0)->getIndex()) << ' '
							<< ((sasedge.getVertex(1) == NULL) ?
											-2 : sasedge.getVertex(1)->getIndex()) << "] ["
							<< ((sasedge.getFace(0) == NULL) ?
											-2 : sasedge.getFace(0)->getIndex()) << ' '
							<< ((sasedge.getFace(1) == NULL) ?
											-2 : sasedge.getFace(1)->getIndex()) << "] "
							<< sasedge.getCircle()
						);
	}
	//@}


	/**	The Default SASEdge Type.
			If double precision is not needed, {\tt SASEdge<float>} should
			be used. It is predefined as {\tt SASEdge} for convenience.
	*/
	typedef TSASEdge<float> SASEdge;


	template <typename T>
	TSASEdge<T>::TSASEdge()
		throw()
		: GraphEdge< TSASVertex<T>,TSASFace<T> >(),
			circle_()
	{
	}


	template <typename T>
	TSASEdge<T>::TSASEdge(const TSASEdge<T>& sasedge, bool deep)
		throw()
		: GraphEdge< TSASVertex<T>,TSASFace<T> >(sasedge,deep),
			circle_(sasedge.circle_)
	{
	}


	template <typename T>
	TSASEdge<T>::TSASEdge(TSASVertex<T>* vertex0,
			TSASVertex<T>* vertex1,
			TSASFace<T>* face0,
			TSASFace<T>* face1,
			const TCircle3<T>& circle,
			Index index)
		throw()
		: GraphEdge< TSASVertex<T>,TSASFace<T> >(vertex0,vertex1,face0,face1,index),
			circle_(circle)
	{
	}


	template <typename T>
	TSASEdge<T>::~TSASEdge()
		throw()
	{
	}


	template <typename T>
	void TSASEdge<T>::setCircle(const TCircle3<T>& circle)
		throw()
	{
		circle_ = circle;
	}


	template <typename T>
	TCircle3<T> TSASEdge<T>::getCircle() const
		throw()
	{
		return circle_;
	}


	template <typename T>
	bool TSASEdge<T>::isFree() const
		throw()
	{
		return (vertex_[0] == NULL);
	}




} // namespace BALL

#endif // BALL_STRUCTURE_SASEDGE_H
