// $Id: SESVertex.h,v 1.10 2001/12/08 17:02:01 strobel Exp $

#ifndef BALL_STRUCTURE_SESVERTEX_H
#define BALL_STRUCTURE_SESVERTEX_H

#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector3.h>
#endif

#ifndef BALL_MATHS_SPHERE3_H
#	include <BALL/MATHS/sphere3.h>
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
	class TSESEdge;

	template <typename T>
	class TSESFace;

	template <typename T>
	class TTriangulatedSES;

	/** Generic SESVertex Class.	
      {\bf Definition:} \URL{BALL/STRUCTURE/SESVertex.h}  
	*/
	template <typename T>
	class TSESVertex	:	public GraphVertex< TSESEdge<T>,TSESFace<T> >
	{
		public:

		/** @name Class friends
				\begin{itemize}
					\item class GraphEdge< TRSVertex<T>,TRSFace<T> >
					\item class GraphFace< TRSVertex<T>,TRSEdge<T> >
					\item class GraphVertex< TRSEdge<T>,TRSFace<T> >
					\item class TSolventExcludedSurface<T>
					\item class TSESEdge<T>
					\item class TSESFace<T>
					\item class TTriangulatedSES<T>
				\end{itemize}
		*/
		friend class GraphEdge< TSESVertex<T>,TSESFace<T> >;
		friend class GraphFace< TSESVertex<T>,TSESEdge<T> >;
		friend class GraphVertex< TSESEdge<T>,TSESFace<T> >;
		friend class TSolventExcludedSurface<T>;
		friend class TSESEdge<T>;
		friend class TSESFace<T>;
		friend class TTriangulatedSES<T>;

		BALL_CREATE(TSESVertex)

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
				This method creates a new SESVertex object.
		*/
		TSESVertex()
			throw();

		/**	Copy constructor.
				Create a new SESVertex object from another.
				@param	sesvertex	the SESVertex object to be copied
				@param	bool			ignored - just for interface consistency
		*/
		TSESVertex(const TSESVertex<T>& sesvertex, bool = false)
			throw();

		/**	Detailled constructor.
				Create a new SESVertex object from a lot of nice objects.
				@param	point		assigned to the point representet by the vertex
				@param	normal	assigned to the normal vector of the vertex
				@param	atom		assigned to the index of the closest atom
				@param	index		assigned to the index of the vertex
		*/
		TSESVertex(const TVector3<T>& point,
				const TVector3<T>& normal,
				Index atom,
				Index index)
			throw();

		/**	Destructor.
				Destructs the RSVertex object.
		*/
		virtual ~TSESVertex()
			throw();

		//@}
		/**	@name	Assignment
		*/
		//@{

		/**	Swap the contents of two SESVertices.
				@param	sesvertex	the SESVertex to swap contents with
		*/
		//void swap(TSESVertex<T>& sesvertex)
		//	throw();

		/**	Assign from another SESVertex.
				@param	sesvertex	the SESVertex object to assign from
				@param	bool			ignored
		*/
		virtual void set(const TSESVertex<T>& sesvertex, bool = false)
			throw();

		/**	Assign to two vectors and two indices.
				@param	point		assigned to the point represented by the vertex
				@param	normal	assigned to the normal vector of th vertex
				@param	atom		assigned to the index of the closest atom
				@param	index		assigned to the index of the vertex
		*/
		virtual void set(const TVector3<T> point,
				const TVector3<T>& normal,
				Index atom,
				Index index)
			throw();

		/**	Set the point represented by the SESVertex.
				@param	point	the new point
		*/
		void setPoint(const TVector3<T>& point)
			throw();

		/**	Return the point represented by the SESVertex.
				@return	TVector3<T>	the point represented by the vertex
		*/
		TVector3<T> getPoint() const
			throw();

		/**	Set the normal vector of the SESVertex.
				@param	point	the new normal vector
		*/
		void setNormal(const TVector3<T>& normal)
			throw(Exception::DivisionByZero);

		/**	Return the normal vector of the SESVertex.
				@return	TVector3<T>	the normal vector of the vertex
		*/
		TVector3<T> getNormal() const
			throw();

		/**	Set the index of the closest atom of the SESVertex.
				@param	atom	the index of the closest atom
		*/
		void setAtom(Index atom)
			throw();

		/**	Return the index of the closest atom.
				@return	Index	the index of the closest atom
		*/
		Index getAtom() const
			throw();

		//@}
		/**	@name	Predicates
		*/
		//@{

		//protected:

		//@}
		/**	@name	Attributes
		*/
		//@{

		/*_	The coordinates of the vertex.
		*/
		TVector3<T> point_;
		/*_	The normal vector of the vertex.
		*/
		TVector3<T> normal_;
		/*_	The index of the closest atom.
		*/
		Index atom_;

		//@}
	};

	/**	@name	Storers
	*/
	//@{

	/**	Output- Operator
	*/
	template <typename T>
	std::ostream& operator << (std::ostream& s, const TSESVertex<T>& sesvertex)
	{
		return (s << "SESVERTEX" << sesvertex.getIndex() << "("
														 << sesvertex.getPoint() << ' '
														 << sesvertex.getNormal() << ' '
														 << sesvertex.getAtom() << ")");
	}


	/**	The Default SESVertex Type.
			If double precision is not needed, {\tt SESVertex<float>} should
			be used. It is predefined as {\tt SESVertex} for convenience.
	*/
	typedef TSESVertex<float> SESVertex;

	//@}



	template <typename T>
	TSESVertex<T>::TSESVertex()
		throw()
		: GraphVertex< TSESEdge<T>,TSESFace<T> >(),
			point_(),
			normal_(),
			atom_(0)
	{
	}


	template <typename T>
	TSESVertex<T>::TSESVertex(const TSESVertex<T>& sesvertex, bool)
		throw()
		: GraphVertex< TSESEdge<T>,TSESFace<T> >(sesvertex,false),
			point_(sesvertex.point_),
			normal_(sesvertex.normal_),
			atom_(sesvertex.atom_)
	{
	}


	template <typename T>
	TSESVertex<T>::TSESVertex(const TVector3<T>& point,
			const TVector3<T>& normal,
			Index atom,
			Index index)
		throw()
		: GraphVertex< TSESEdge<T>,TSESFace<T> >(),
			point_(point),
			normal_(normal),
			atom_(atom)
	{
		index_ = index;
	}


	template <typename T>
	TSESVertex<T>::~TSESVertex()
		throw()
	{
	}


	/*template <typename T>
	void TSESVertex<T>::swap(TSESVertex<T>& sesvertex)
		throw()
	{
		TVector3<T> temp_point(point_);
		point_ = sesvertex.point_;
		sesvertex.point_ = temp_point;

		temp_point = normal_;
		normal_ = sesvertex.normal_;
		sesvertex.normal_ = temp_point;

		Index temp = atom_;
		atom_ = sesvertex.atom_;
		sesvertex.atom_ = temp;

		temp = index_;
		index_ = sesvertex.index_;
		sesvertex.index_ = temp;
	}*/


	template <typename T>
	void TSESVertex<T>::set(const TSESVertex<T>& sesvertex, bool)
		throw()
	{
		point_ = sesvertex.point_;
		normal_ = sesvertex.normal_;
		atom_ = sesvertex.atom_;
		index_ = sesvertex.index_;
	}


	template <typename T>
	void TSESVertex<T>::set(const TVector3<T> point,
			const TVector3<T>& normal,
			Index atom,
			Index index)
		throw()
	{
		point_ = point;
		normal_ = normal;
		atom_ = atom;
		index_ = index;
	}


	template <typename T>
	void TSESVertex<T>::setPoint(const TVector3<T>& point)
		throw()
	{
		point_ = point;
	}


	template <typename T>
	TVector3<T> TSESVertex<T>::getPoint() const
		throw()
	{
		return point_;
	}


	template <typename T>
	void TSESVertex<T>::setNormal(const TVector3<T>& normal)
		throw(Exception::DivisionByZero)
	{
		normal_ = normal;
		normal_.normalize();
	}


	template <typename T>
	TVector3<T> TSESVertex<T>::getNormal() const
		throw()
	{
		return normal_;
	}


	template <typename T>
	void TSESVertex<T>::setAtom(Index atom)
		throw()
	{
		atom_ = atom;
	}


	template <typename T>
	Index TSESVertex<T>::getAtom() const
		throw()
	{
		return atom_;
	}






} // namespace BALL

#endif // BALL_SES_SESVERTEX_H
