// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: SESFace.h,v 1.27 2004/02/23 17:26:05 anhi Exp $
//

#ifndef BALL_STRUCTURE_SESFACE_H
#define BALL_STRUCTURE_SESFACE_H

#ifndef BALL_STRUCTURE_GRAPHFACE_H
#	include <BALL/STRUCTURE/graphFace.h>
#endif

#ifndef BALL_STRUCTURE_RSEDGE_H
#	include <BALL/STRUCTURE/RSEdge.h>
#endif

#ifndef BALL_STRUCTURE_RSFACE_H
#	include <BALL/STRUCTURE/RSFace.h>
#endif

#ifndef BALL_STRUCTURE_RSVERTEX_H
#	include <BALL/STRUCTURE/RSVertex.h>
#endif

namespace BALL
{
	class SESEdge;
	class SESVertex;
	class SolventExcludedSurface;
	class TriangulatedSES;
	class SESComputer;
	class SESSingularityCleaner;
	class SESTriangulator;

	/** Generic SESFace Class.
	\ingroup Surface		
	*/
	class SESFace	:	public GraphFace< SESVertex,SESEdge,SESFace >
	{
		public:

		/** @name Class friends

					- class SESEdge
					- class SESVertex
					- class SolventExcludedSurface
					- class SESComputer
					- class TSESSingularityCleaner
					- class TriangulatedSES
					- class SESTriangulator
				
		*/
		friend class SESEdge;
		friend class SESVertex;
		friend class SolventExcludedSurface;
		friend class SESComputer;
		friend class SESSingularityCleaner;
		friend class TriangulatedSES;
		friend class SESTriangulator;

		BALL_CREATE(SESFace)

		/**	@name	Enums
		*/
		//@{

		/** type of SESFace:
				<tt>0</tt> spheric reentrant face
				<tt>1</tt> toric reentrant face
				<tt>2</tt> contact face
				<tt>3</tt> singular toric reentrant face
		*/
		enum Type
		{
			TYPE_SPHERIC  = 0,
			TYPE_TORIC = 1,
			TYPE_CONTACT = 2,
			TYPE_TORIC_SINGULAR = 3
		};

		//@}
		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
				This method creates a new SESFace object.
		*/
		SESFace()
			throw();

		/**	Copy constructor.
				Create a new SESFace object from another.
				@param	sesface the SESFace object to be copied
				@param	deep		if deep = false, all pointers are set to NULL	
												(default). Otherwise the new SESFace object is linked	
												to the neighbours of the old SESFace object.
		*/
		SESFace(const SESFace& sesface, bool deep = false)
			throw();

		/**	Detailled constructor.
				Create a new SESFace object
				@param	type			assigned the type
				@param	rsvertex	assigned to the rsvertex
				@param	rsedge		assigned to the rsedge
				@param	rsface		assigned to the rsface
		*/
		SESFace
				(Type				type,
				 RSVertex*	rsvertex,
				 RSEdge*		rsedge,
				 RSFace*		rsface)
			throw();

		/**	Destructor.
				Destructs the SESFace object.
		*/
		virtual ~SESFace()
			throw();

		//@}
		/**	@name	Assignments
		*/
		//@{

		/**	Assign from another SESFace.
				@param	sesface the SESFace object to assign from
				@param	deep		if deep = false, all pointers are set to NULL	
												(default). Otherwise the SESFace object is linked to	
												the neighbours of the SESFace object to assign from.
		*/
		void set(const SESFace& sesface, bool deep = false)
			throw();

		/**	Assign from another SESFace.
				The SESFace object is linked to the neighbours of the SESFace object	
				to assign from.
				@param	sesface the SESFace object to assign from
		*/
		SESFace& operator = (const SESFace& sesface)
			throw();

		/** Set the type of the SESFace.
				@param	type	the new type
		*/
		void setType(Type type)
			throw();

		/** Get the type of the SESFace
				@return	SESFace::Type	the type of the SESFace
		*/
		Type getType() const
			throw();

		/** Set the corresponding RSVertex of the SESFace.
				@param	rsvertex	a pointer to the new RSVertex
		*/
		void setRSVertex(RSVertex* rsvertex)
			throw();

		/** Get the corresponding RSVertex of the SESFace.
				@return	RSVertex*	a pointer to the corresponding RSVertex
		*/
		RSVertex* getRSVertex() const
			throw();

		/** Set the corresponding RSEdge of the SESFace.
				@param	rsedge	a pointer to the new RSFace
		*/
		void setRSEdge(RSEdge* rsedge)
			throw();

		/** Get the corresponding RSEdge of the SESFace.
				@return	RSEdge*	a pointer to the corresponding RSEdge
		*/
		RSEdge* getRSEdge() const
			throw();

		/** Set the corresponding RSFace of the SESFace.
				@param	rsface	a pointer to the new RSFace
		*/
		void setRSFace(RSFace* rsface)
			throw();

		/** Get the corresponding RSFace of the SESFace.
				@return	RSFace*	a pointer to the corresponding RSFace
		*/
		RSFace* getRSFace() const
			throw();

		//@}
		/**	@name	Accessors
		*/
		//@{

		/** Normalize the SESFace.
		*/
		void normalize(bool singular)
			throw();

		//@}
		/**	@name	Predicates
		*/
		//@{

		/**	Equality operator.
				@return bool	<b>true</b>
		*/
		virtual bool operator == (const SESFace&) const
			throw();

		/**	Inequality operator.
				@return	bool	<b>false</b>
		*/
		virtual bool operator != (const SESFace&) const
			throw();

		/**	Similarity operator.
				@return	bool	<b>true</b>
		*/
		virtual bool operator *= (const SESFace&) const
			throw();

		/** Test whether two faces are neighboured.
				Two faces are neighboured if they have a common edge.
				@param	face	the face to be tested
				@return	bool	<b>true</b> if the faces are neighboured,	
											<b>false</b> otherwise
		*/
		bool isNeighbouredTo(SESFace* face) const
			throw();

		/**	Test whether a toric face is free ore not.
				@return	bool	<b>true</b> if the face is toric and free,	
											<b>false</b> otherwise
		*/
		bool isFree() const
			throw();

		/**	Test whether a face has singular edges.
				@return	bool	<b>true</b> if the face has singular edges,	
											<b>false</b> otherwise
		*/
		bool hasSingularEdges() const
			throw();

		//@}

		private:

		void normalizeNonSingularToricFace_()
			throw();

		void normalizeSingularToricFace_()
			throw();

		void findTriangle_
				(bool first,
				 SESEdge*& edge0,
				 SESEdge*& edge1,
				 SESEdge*& edge2,
				 SESVertex*& vertex0,
				 SESVertex*& vertex1,
				 SESVertex*& vertex2)
			throw();

		protected:

		/**	@name	Attributes
		*/
		//@{

		/*_	The type of the face.
		*/
		Type type_;
		/*_	A pointer to the corresponding RSVertex
				(if type_ is TYPE_CONTACT).
		*/
		RSVertex* rsvertex_;
		/*_	A pointer to the corresponding RSEdge
				(if type_ is TYPE_TORIC or TYPE_TORIC_SINGULAR).
		*/
		RSEdge* rsedge_;
		/*_	A pointer to the corresponding RSFace
				(if type_ is TYPE_SPHERIC).
		*/
		RSFace* rsface_;

		//@}
	};

	/**	@name	Storers
	*/
	//@{

	/**	Output- Operator
	*/
	std::ostream& operator << (std::ostream& s, const SESFace& sesface);

	//@}

   
} // namespace BALL

#endif // BALL_STRUCTURE_SESFACE_H
