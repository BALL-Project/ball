// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_STRUCTURE_SASVERTEX_H
#define BALL_STRUCTURE_SASVERTEX_H

#ifndef BALL_STRUCTURE_GRAPHVERTEX_H
#	include <BALL/STRUCTURE/graphVertex.h>
#endif

#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector3.h>
#endif

namespace BALL
{

	class SASEdge;
	class SASFace;
	class SolventAccessibleSurface;
	class TriangulatedSAS;
	class SASTriangulator;

	/** Generic SASVertex Class.	
  \ingroup Surface    
	*/
	class BALL_EXPORT SASVertex	:	public GraphVertex< SASVertex,SASEdge,SASFace >
	{
		public:

		/** @name Class friends
			
					- class SASEdge
					- class SASFace
					- class SolventAccessibleSurface
					- class TriangulatedSAS
					- class SASTriangulator
				
		*/
		friend class SASEdge;
		friend class SASFace;
		friend class SolventAccessibleSurface;
		friend class TriangulatedSAS;
		friend class SASTriangulator;

		BALL_CREATE(SASVertex)

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
				This method creates a new SASVertex object.
		*/
		SASVertex()
			;

		/**	Copy constructor.
				Create a new SASVertex object from another.
				@param	sasvertex	the SASVertex object to be copied
				@param	deep			if deep = false, all pointers are set to NULL		
													(default). Otherwise the new SASVertex object is	
													linked to the neighbours of the old SASVertex object.
		*/
		SASVertex(const SASVertex& sasvertex, bool deep = false)
			;

		/**	Detailled constructor.
				Create a new SASVertex object from some nice objects.
				@param	point		assigned to the point representet by the vertex
				@param	index		assigned to the index of the vertex
		*/
		SASVertex(const TVector3<double>& point,
				Index index)
			;

		/**	Destructor.
				Destructs the RSVertex object.
		*/
		virtual ~SASVertex()
			;

		//@}
		/**	@name	Assignment
		*/
		//@{

		/**	Assign from another SASVertex.
				@param	sasvertex	the SASVertex object to assign from
				@param	deep			if deep = false, all pointers are set to NULL		
													(default). Otherwise the new SASVertex object is	
													linked to the neighbours of the SASVertex object	
													to assign from.
		*/
		void set(const SASVertex& sasvertex, bool deep = false)
			;

		/**	Assign from another SASVertex.
				The new SASVertex object is linked to the neighbours of the SASVertex	
				object to assign from.
				@param	sasvertex	the SASVertex object to assign from
		*/
		SASVertex& operator = (const SASVertex& sasvertex)
			;

		/**	Assign to a TVector3<double> and an Index.
				@param	point		assigned to the point represented by the vertex
				@param	index		assigned to the index of the vertex
		*/
		void set(const TVector3<double> point,
				Index index)
			;

		/**	Set the point represented by the SASVertex.
				@param	point	the new point
		*/
		void setPoint(const TVector3<double>& point)
			;

		/**	Return the point represented by the SASVertex.
				@return	TVector3<double>	the point represented by the vertex
		*/
		TVector3<double> getPoint() const
			;

		//@}
		/**	@name	Predicates
		*/
		//@{

		/**	Equality operator.
				@return	bool	<b>true</b>
		*/
		virtual bool operator == (const SASVertex&) const
			;

		/**	Inequality operator.
				@return	bool	<b>false</b>
		*/
		virtual bool operator != (const SASVertex&) const
			;

		/**	Similarity operator.
				@return	bool	<b>true</b> if the points the SASVertices lie on are	
											equal, <b>false</b> otherwise
		*/
		virtual bool operator *= (const SASVertex&) const
			;

		//@}

		protected:

		/*_	@name	Attributes
		*/
		//@{

		/*_	The coordinates of the vertex.
		*/
		TVector3<double> point_;

		//@}
	};

	/**	@name	Storers
	*/
	//@{

	/**	Output- Operator
	*/
	BALL_EXPORT std::ostream& operator << (std::ostream& s, const SASVertex& sasvertex);

	//@}



} // namespace BALL

#endif // BALL_STRUCTURE_SASVERTEX_H
