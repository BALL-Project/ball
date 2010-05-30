// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

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

	class Triangle;
	class TrianglePoint;
	class TriangulatedSurface;
	class TriangulatedSphere;
	class TriangulatedSES;
	class TriangulatedSAS;
	class SESTriangulator;
	class SASTriangulator;


	/** Generic TriangleEdge Class.
			\ingroup Surface
	*/
	class BALL_EXPORT TriangleEdge
			:	public GraphEdge< TrianglePoint,TriangleEdge,Triangle >
	{
		
		public:

		/** @name Class friends

					- class Triangle
					- class TriangleEdge
					- class TriangulatedSurface
					- class TriangulatedSphere
					- class TriangulatedSES
					- class TriangulatedSAS
					- class SESTriangulator
					- class SASTriangulator
				
		*/
		friend class Triangle;
		friend class TrianglePoint;
		friend class TriangulatedSurface;
		friend class TriangulatedSphere;
		friend class TriangulatedSES;
		friend class TriangulatedSAS;
		friend class SESTriangulator;
		friend class SASTriangulator;

		BALL_CREATE(TriangleEdge)

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
				This method creates a new TriangleEdge object.
		*/
		TriangleEdge();

		/** 
		 * Detailed constructor.
		 * Creates a new TriangleEdge object connecting the passed 
		 * TrianglePoint instances.
		 */ 
		TriangleEdge(TrianglePoint* v1, TrianglePoint* v2);

		/**	Copy constructor.
				Create a new TriangleEdge object from another.
				@param	edge	the TriangleEdge object to be copied
				@param	deep	if deep = false, all pointers are set to NULL (default).	
											Otherwise the new TriangleEdge object is linked to the	
											neighbours of the old TriangleEdge object.
		*/
		TriangleEdge(const TriangleEdge& edge, bool deep = false);

		/**	Destructor.
				Destructs the TriangleEdge object.
		*/
		virtual ~TriangleEdge();
		//@}

		/**	@name	Accessors
		*/
		//@{

		/** Set one of the two points of the TriangleEdge.
				@param	i			the first point is changed if i = 0, the second otherwise
				@param	point	a pointer to the new point
		*/
		void setPoint(Position i, TrianglePoint* point);

		/** Return one of the two points of the TriangleEdge.
				@param	i
				@return	TrianglePoint*	a pointer to the first point if i = 0,	
																		a pointer to the second point otherwise
		*/
		TrianglePoint* getPoint(Position i) const;

		/** Set one of the two triangles of the TriangleEdge.
				@param	i					change the first triangle, if i = 0, the second	
													otherwise
				@param	triangle	a pointer to the new triangle
		*/
		void setTriangle(Position i, Triangle* triangle);

		/** Return one of the two triangles of the TriangleEdge.
				@param	i
				@return	Triangle*	a pointer to the first triangle if i = 0,	
															a pointer to the second triangle otherwise
		*/
		Triangle* getTriangle(Position i) const;

		//@}

		/**	@name	Predicates
		*/
		//@{

		/** Equality operator
				@return	bool	<b>true</b> if the vertices are similar,	
											<b>false</b> otherwise.
		*/
		virtual bool operator == (const TriangleEdge&) const;

		/** Inequality operator
				@return	bool	<b>false</b> if the vertices are similar,	
											<b>true</b> otherwise.
		*/
		virtual bool operator != (const TriangleEdge&) const;

		/** Similarity operator
				@return	bool	<b>true</b>
		*/
		virtual bool operator *= (const TriangleEdge&) const;

		//@}

	};


	/**	@name	Storers
	*/
	//@{

	/**	Output- Operator
	*/
	BALL_EXPORT std::ostream& operator << (std::ostream& s, const TriangleEdge& edge);

	//@}


}	// namespace BALL


#endif	// BALL_STRUCTURE_TRIANGLEEDGE_H
