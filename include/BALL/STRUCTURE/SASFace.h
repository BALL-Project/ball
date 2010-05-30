// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_STRUCTURE_SASFACE_H
#define BALL_STRUCTURE_SASFACE_H

#ifndef BALL_STRUCTURE_GRAPHFACE_H
#	include <BALL/STRUCTURE/graphFace.h>
#endif

#ifndef BALL_STRUCTURE_SPHERE3_H
#	include <BALL/MATHS/sphere3.h>
#endif

#include <list>

namespace BALL
{

	class SolventAccessibleSurface;
	class SASEdge;
	class SASVertex;
	class TriangulatedSAS;
	class SASTriangulator;

	/** Generic SASFace Class.
	\ingroup Surface			
	*/
	class BALL_EXPORT SASFace	:	public GraphFace< SASVertex,SASEdge,SASFace >
	{
		public:

		/** @name Class friends

					- class SASEdge
					- class SASVertex
					- class SolventAccessibleSurface
					- class TriangulatedSAS
					- class SASTriangulator
				
		*/
		friend class SASEdge;
		friend class SASVertex;
		friend class SolventAccessibleSurface;
		friend class TriangulatedSAS;
		friend class SASTriangulator;

		BALL_CREATE(SASFace)

		/**	@name	Type definitions
		*/
		//@{

		typedef std::list<bool>::iterator OrientationIterator;
		typedef std::list<bool>::const_iterator ConstOrientationIterator;

		//@}
		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
				This method creates a new SASFace object.
		*/
		SASFace()
			;

		/**	Copy constructor.
				Create a new SASFace object from another.
				@param	sasface the SASFace object to be copied
				@param	deep		if deep = false, all pointers are set to NULL	
												(default). Otherwise the new SASFace object is linked	
												to the neighbours of the old SASFace object.
		*/
		SASFace(const SASFace& sasface, bool deep = false)
			;

		/**	Destructor.
				Destructs the SASFace object.
		*/
		virtual ~SASFace()
			;

		//@}
		/**	@name	Assignments
		*/
		//@{

		/**	Assign from another SASFace.
				@param	sasface the SASFace object to assign from
				@param	deep		if deep = false, all pointers are set to NULL	
												(default). Otherwise the new SASFace object is linked	
												to the neighbours of the SASFace object to assign from.
		*/
		void set(const SASFace& sasface, bool deep = false)
			;

		/**	Assign from another SASFace.
				The new SASFace object is linked to the neighbours of the SASFace	
				object to assign from.
				@param	sasface the SASFace object to assign from
		*/
		SASFace& operator = (const SASFace& sasface)
			;

		//@}
		/**	@name	Accessors
		*/
		//@{

		/** Set the sphere the GraphFace lies on.
				@param	sphere	the new sphere
		*/
		void setSphere(const TSphere3<double>& sphere)
			;

		/** Return the sphere the GraphFace lies on.
				@return	TSphere<double>	the sphere the GraphFace lies on
		*/
		TSphere3<double> getSphere() const
			;

		//@}
		/**	@name	Predicates
		*/
		//@{

		/**	Equality operator.
				@return bool	<b>true</b>
		*/
		virtual bool operator == (const SASFace&) const
			;

		/**	Inequality operator.
				@return	bool	<b>false</b>
		*/
		virtual bool operator != (const SASFace&) const
			;

		/**	Similarity operator.
				@return	bool	<b>true</b>
		*/
		virtual bool operator *= (const SASFace&) const
			;

		//@}
		/**	@name	External Iterators
		*/
		//@{

		OrientationIterator beginOrientation()
			;
		ConstOrientationIterator beginOrientation() const
			;
		OrientationIterator endOrientation()
			;
		ConstOrientationIterator endOrientation() const
			;

		//@}

		protected:

		/**	@name	Attributes
		*/
		//@{

		/*_	A list of booleans to indicate the orientation of each sasedge.
		*/
		std::list<bool> orientation_;
		/*_ The sphere on which the face lies.
		*/
		TSphere3<double> sphere_;

		//@}
	};

	/**	@name	Storers
	*/
	//@{

	/**	Output- Operator
	*/
	BALL_EXPORT std::ostream& operator << (std::ostream& s, const SASFace& sasface);

	//@}

} // namespace BALL

#endif // BALL_STRUCTURE_SASFACE_H
