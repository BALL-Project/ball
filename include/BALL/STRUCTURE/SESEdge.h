// $Id: SESEdge.h,v 1.11 2001/12/08 17:03:43 strobel Exp $

#ifndef BALL_STRUCTURE_SESEDGE_H
#define BALL_STRUCTURE_SESEDGE_H

#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector3.h>
#endif

#ifndef BALL_MATHS_CIRCLE3_H
#	include <BALL/MATHS/circle3.h>
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
	class TSolventExcludedSurface;

	template <typename T>
	class TSESFace;

	template <typename T>
	class TSESVertex;

	template <typename T>
	class TRSEdge;

	template <typename T>
	class TTriangulatedSES;

	/** Generic SESEdge Class.
			{\bf Definition:} \URL{BALL/STRUCTURE/SESEdge.h} 
	*/
	template <class T>
	class TSESEdge	:	public GraphEdge< TSESVertex<T>,TSESFace<T> >
	{
		public:

		/** @name Class friends
				\begin{itemize}
					\item class GraphEdge< TRSVertex<T>,TRSFace<T> >
					\item class GraphFace< TRSVertex<T>,TRSEdge<T> >
					\item class GraphVertex< TRSEdge<T>,TRSFace<T> >
					\item class TSolventExcludedSurface<T>
					\item class TSESFace<T>
					\item class TSESVertex<T>
					\item class TTriangulatedSES<T>
				\end{itemize}
		*/
		friend class GraphEdge< TSESVertex<T>,TSESFace<T> >;
		friend class GraphFace< TSESVertex<T>,TSESEdge<T> >;
		friend class GraphVertex< TSESEdge<T>,TSESFace<T> >;
		friend class TSolventExcludedSurface<T>;
		friend class TSESFace<T>;
		friend class TSESVertex<T>;
		friend class TTriangulatedSES<T>;

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
				This method creates a new SESEdge object.
		*/
		TSESEdge()
			throw();

		/**	Copy constructor.
				Create a new SESEdge object from another.
				@param	sesedge	the SESEdge object to be copied
				@param	deep		if deep = false, all pointers are set to NULL (default). Otherwise the new
												SESEdge object is linked to the neighbours of the old SESEdge object.
		*/
		TSESEdge(const TSESEdge<T>& sesedge, bool deep = false)
			throw();

		/**	Detailled constructor.
				Create a new SESEdge object from a lot of nice objects.
				@param	vertex0	assigned to the first vertex
				@param	vertex1	assigned to the second vertex
				@param	face0		assigned to the first face
				@param	face1		assigned to the second face
				@param	circle	assigned to the circle
				@param	rsedge	assigned to the RSEdge
				@param	type		assigned to the type of the SESEdge
				@param	index		assigned to the index
		*/
		TSESEdge(TSESVertex<T>* vertex0,
				TSESVertex<T>* vertex1,
				TSESFace<T>* face0,
				TSESFace<T>* face1,
				const TCircle3<T>& circle,
				TRSEdge<T>* rsedge,
				Type type,
				Index index)
			throw();

		/**	Destructor.
				Destructs the SESEdge object.
		*/
		virtual ~TSESEdge()
			throw();

		//@}
		/**	@name	Assignment
		*/
		//@{

		//@}
		/**	@name	Accessors
		*/
		//@{

		/** Set the circle on wich the SESEdge lies.
				@param	circle	the new circle
		*/
		void setCircle(const TCircle3<T>& center)
			throw();

		/** Return the circle on wich the SESEdge lies.
				@return	TCircle3<T>	the circle of The SESEdge
		*/
		TCircle3<T> getCircle() const
			throw();

		/** Set the corresponding RSEdge.
				@param	rsedge	a pointer to the new RSEdge
		*/
		void setRSEdge(TRSEdge<T>* rsedge)
			throw();

		/** Return the corresponding RSEdge.
				@return	TRSEdge<T>*	a pointer to th correwsponding RSEdge
		*/
		TRSEdge<T>* getRSEdge() const
			throw();


		/** Set the type of the SESEdge.
				@param	type	the new type of the SESEdge
		*/
		void setType(Type typ)
			throw();

		/** Get the type of the SESEdge.
				@return	Type	the type of the SESEdge
		*/
		Type getType() const
			throw();

		//@}
		/**	@name	Predicates
		*/
		//@{

		/**	similar.
				@return	bool	{\bf true} if all vertices are representing the same	
											point modulo order, {\bf false} otherwise
		*/
		bool similar(const TSESEdge<T>& sesedge) const
			throw();

		/** isFree.
				@return	bool	{\bf true} if the RSEdge of the SESEdge is free,	
											{\bf false} otherwise
		*/
		bool isFree() const
			throw();

		//@}
		
		protected:
		
		/**	@name	Attributes
		*/
		//@{

		/*_ The circle on which the SESEdge lies.
		*/
		TCircle3<T> circle_;
		/*_ A pointer to the corresponding RSEdge.
		*/
		TRSEdge<T>* rsedge_;
		/*_ The type of the SESEdge.
		*/
		//int type;
		Type type_;

		//@}
	};

	/**	@name	Storers
	*/
	//@{

	/*	Input- Operator
	*/
	template <typename T>
	std::istream& operator >> (std::istream& s, TSESEdge<T>& sesedge)
	{
		throw Exception::NotImplemented(__FILE__, __LINE__);
	}

	/**	Output- Operator
	*/
	template <typename T>
	std::ostream& operator << (std::ostream& s, const TSESEdge<T>& sesedge)
	{
		return (s << "SESEDGE" << sesedge.getIndex() << "(["
							<< ((sesedge.getVertex(0) == NULL) ?
											-2 : sesedge.getVertex(0)->getIndex()) << ' '
							<< ((sesedge.getVertex(1) == NULL) ?
											-2 : sesedge.getVertex(1)->getIndex()) << "] ["
							<< ((sesedge.getFace(0) == NULL) ?
											-2 : sesedge.getFace(0)->getIndex()) << ' '
							<< ((sesedge.getFace(1) == NULL) ?
											-2 : sesedge.getFace(1)->getIndex()) << "] "
							<< sesedge.getCircle() << ' '
							<< ((sesedge.getRSEdge() == NULL) ?
											-2 : sesedge.getRSEdge()->getIndex())
							<< ((sesedge.getType() == TSESEdge<T>::TYPE_CONCAVE) ?
											" cocave)" :
											((sesedge.getType() == TSESEdge<T>::TYPE_CONVEX) ?
														" convex)" : " singular)"))
						);
	}
	//@}


	/**	The Default SESEdge Type.
			If double precision is not needed, {\tt SESEdge<float>} should
			be used. It is predefined as {\tt SESEdge} for convenience.
	*/
	typedef TSESEdge<float> SESEdge;


	template <typename T>
	TSESEdge<T>::TSESEdge()
		throw()
		: GraphEdge< TSESVertex<T>,TSESFace<T> >(),
			circle_(),
			rsedge_(NULL),
			type_(TSESEdge<T>::TYPE_CONCAVE)
	{
	}


	template <typename T>
	TSESEdge<T>::TSESEdge(const TSESEdge<T>& sesedge, bool deep)
		throw()
		: GraphEdge< TSESVertex<T>,TSESFace<T> >(sesedge,deep),
			circle_(sesedge.circle_),
			rsedge_(NULL),
			type_(sesedge.type_)
	{
		if (deep)
		{
			rsedge_ = sesedge.rsedge_;
		}
	}


	template <typename T>
	TSESEdge<T>::TSESEdge(TSESVertex<T>* vertex0,
			TSESVertex<T>* vertex1,
			TSESFace<T>* face0,
			TSESFace<T>* face1,
			const TCircle3<T>& circle,
			TRSEdge<T>* rsedge,
			Type type,
			Index index)
		throw()
		: GraphEdge< TSESVertex<T>,TSESFace<T> >(vertex0,vertex1,face0,face1,index),
			circle_(circle),
			rsedge_(rsedge),
			type_(type)
	{
	}


	template <typename T>
	TSESEdge<T>::~TSESEdge()
		throw()
	{
	}


	template <typename T>
	void TSESEdge<T>::setCircle(const TCircle3<T>& center)
		throw()
	{
		circle_ = circle;
	}


	template <typename T>
	TCircle3<T> TSESEdge<T>::getCircle() const
		throw()
	{
		return circle_;
	}


	template <typename T>
	void TSESEdge<T>::setRSEdge(TRSEdge<T>* rsedge)
		throw()
	{
		rsedge_ = rsedge;
	}


	template <typename T>
	TRSEdge<T>* TSESEdge<T>::getRSEdge() const
		throw()
	{
		return rsedge_;
	}


	template <typename T>
	void TSESEdge<T>::setType(TSESEdge<T>::Type typ)
		throw()
	{
		type_ = typ;
	}


	template <typename T>
	TSESEdge<T>::Type TSESEdge<T>::getType() const
		throw()
	{
		return type_;
	}


	template <typename T>
	bool TSESEdge<T>::similar(const TSESEdge<T>& sesedge) const
		throw()
	{
		if ((vertex0_->p != sesedge.vertex0_->p) &&
				(vertex0_->p != sesedge.vertex1_->p)		)
		{
			return false;
		}
		if ((vertex1_->p != sesedge.vertex0_->p) &&
				(vertex1_->p != sesedge.vertex1_->p)		)
		{
			return false;
		}
		return true;
	}


	template <typename T>
	bool TSESEdge<T>::isFree() const
		throw()
	{
		if (rsedge_ == NULL)
		{
			return false;
		}
		else
		{
			return rsedge_->isFree();
		}
	}




} // namespace BALL

#endif // BALL_STRUCTURE_SESEDGE_H
