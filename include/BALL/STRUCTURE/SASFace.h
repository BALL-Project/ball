// $Id: SASFace.h,v 1.2 2002/12/12 09:48:54 oliver Exp $

#ifndef BALL_STRUCTURE_SASFACE_H
#define BALL_STRUCTURE_SASFACE_H

#ifndef BALL_MATHS_VECTOR3_H
//#	include <BALL/MATHS/vector3.h>
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
	class TSASVertex;

	template <typename T>
	class TTriangulatedSAS;

	/** Generic SASFace Class.
			{\bf Definition:} \URL{BALL/STRUCTURE/SASFace.h}
	*/
	template <typename T>
	class TSASFace	:	public GraphFace< TSASVertex<T>,TSASEdge<T> >
	{
		public:

		/** @name Class friends
				\begin{itemize}
					\item class GraphEdge< TSASVertex<T>,TSASFace<T> >
					\item class GraphFace< TSASVertex<T>,TSASEdge<T> >
					\item class GraphVertex< TSASEdge<T>,TSASFace<T> >
					\item class TSolventAccessibleSurface<T>
					\item class TSASEdge<T>
					\item class TSASVertex<T>
					\item class TTriangulatedSAS<T>
				\end{itemize}
		*/
		friend class GraphEdge< TSASVertex<T>,TSASFace<T> >;
		friend class GraphFace< TSASVertex<T>,TSASEdge<T> >;
		friend class GraphVertex< TSASEdge<T>,TSASFace<T> >;
		friend class TSolventAccessibleSurface<T>;
		friend class TSASEdge<T>;
		friend class TSASVertex<T>;
		friend class TTriangulatedSAS<T>;

		BALL_CREATE(TSASFace)

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
				This method creates a new SASFace object.
		*/
		TSASFace()
			throw();

		/**	Copy constructor.
				Create a new SASFace object from another.
				@param	sasface the SASFace object to be copied
				@param	deep		if deep = false, all pointers are set to NULL (default). Otherwise the new
												SASFace object is linked to the neighbours of the old SASFace object.
		*/
		TSASFace(const TSASFace<T>& sasface, bool deep = false)
			throw();

		/**	Detailled constructor.
				Create a new SASFace object
				@param	vertex			assigned to the list of vertices
				@param	edge				assigned to the list of edges
				@param	orientation	assigned to the list of orientations
				@param	index				assigned to the index
		*/
		TSASFace(const ::std::list<TSASVertex<T>*>& vertex,
				const ::std::list<TSASEdge<T>*> edge,
				const ::std::list<bool> orientation,
				Index index)
			throw();

		/**	Destructor.
				Destructs the SASFace object.
		*/
		virtual ~TSASFace()
			throw();

		//@}
		/**	@name	Assignments
		*/
		//@{

		//@}
		/**	@name	Accessors
		*/
		//@{

		//@}
		/**	@name	Predicates
		*/
		//@{

		//@}

		protected:

		/**	@name	Attributes
		*/
		//@{

		/*_	A list of booleans to indicate the orientation of each sasedge.
		*/
		::std::list<bool> orientation_;
		/*_ The sphere on which the face lies.
		*/
		TSphere3<T> sphere_;

		//@}
	};

	/**	@name	Storers
	*/
	//@{

	/**	Output- Operator
	*/
	template <typename T>
	std::ostream& operator << (std::ostream& s, const TSASFace<T>& sasface)
	{
		s << "SASFACE" << sasface.getIndex() << "(";
		std::list<TSASVertex<T>*> vertices(sasface.getVertices());
		typename std::list<TSASVertex<T>*>::iterator v = vertices.begin();
		while (v != vertices.end())
		{
			s << (*v)->getIndex() << ' ';
			v++;
		}
		s << "] [";
		std::list<TSASEdge<T>*> edges(sasface.getEdges());
		typename std::list<TSASEdge<T>*>::iterator e = edges.begin();
		while (e != edges.end())
		{
			s << (*e)->getIndex() << ' ';
			e++;
		}
		//s << "] [";
		//for (Position i = 0; i < sasface.orientation_.size(); i++)
		//{
		//	s << sasface.orientation_[i] << ' ';
		//}
		s << "])";
		return s;
	}

	//@}

	/**	The Default SASFace Type.
			If double precision is not needed, {\tt SASFace<float>} should
			be used. It is predefined as {\tt SASFace} for convenience.
	*/
	typedef TSASFace<float> SASFace;


	template <class T>
	TSASFace<T>::TSASFace()
		throw()
		: GraphFace< TSASVertex<T>,TSASEdge<T> >(),
			orientation_()
	{
	}


	template <class T>
	TSASFace<T>::TSASFace(const TSASFace<T>& sasface, bool deep)
		throw()
		: GraphFace< TSASVertex<T>,TSASEdge<T> >(sasface,deep),
			orientation_()
	{
		if (deep)
		{
			orientation_ = sasface.orientation_;
		}
	}


	template <class T>
	TSASFace<T>::TSASFace(const ::std::list<TSASVertex<T>*>& vertex,
			const ::std::list<TSASEdge<T>*> edge,
			const ::std::list<bool> orientation,
			Index index)
		throw()
		: GraphFace< TSASVertex<T>,TSASEdge<T> >(vertex,edge,index),
			orientation_(orientation)
	{
	}


	template <class T>
	TSASFace<T>::~TSASFace()
		throw()
	{
	}





} // namespace BALL

#endif // BALL_STRUCTURE_SASFACE_H
