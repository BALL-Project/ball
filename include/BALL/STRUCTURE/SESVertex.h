// $Id: SESVertex.h,v 1.6 2001/02/23 02:40:45 amoll Exp $

#ifndef BALL_STRUCTURE_SESVERTEX_H
#define BALL_STRUCTURE_SESVERTEX_H

#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector3.h>
#endif

#ifndef BALL_MATHS_SPHERE3_H
#	include <BALL/MATHS/sphere3.h>
#endif

#include <list.h>

namespace BALL
{

	/** Generic SESVertex Class.	\\
      {\bf Definition:} \URL{BALL/STRUCTURE/SESVertex.h}  \\
	*/
	template <class T>
	class TSESVertex
	{
		public:

		BALL_CREATE(TSESVertex)

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
				This method creates a new SESVertex object. All components are
				initialized to {\tt (T)0}.
		*/
		TSESVertex()
			: p(), n(), atom(0), index(-1)
		{
		}

		/**	Copy constructor.
				Create a new SESVertex object from another.
				@param sesvertex the SESVertex object to be copied
				@param bool ignored - just for interface consistency
		*/
		TSESVertex(const TSESVertex<T>& sesvertex,bool /* deep */ = true)
			: p(sesvertex.p), n(sesvertex.n),
				atom(sesvertex.atom), index(sesvertex.index)
		{
		}

		/**	Detailled constructor.
				Create a new SESVertex object from two vector3 and two int.
				@param	point assigned to the point representet by the vertex
				@param	norm assigned to the normal vector
				@param	a assigned to the index of the nearest atom
				@param	i assigned to the index of this vertex
		*/
		TSESVertex(const TVector3<T>& point, const TVector3<T>& normal, int a, int i)
			: p(point), n(normal), atom(a), index(i)
		{
		}

		/**	Destructor.
				Destructs the RSVertex object. As there are no dynamic
				data structures, nothing happens.
		*/
		virtual ~TSESVertex()
		{
		}

		//@}
		/**	@name	Assignment
		*/
		//@{

		/**	Swap the contents of two SESVertex.
				@param	sesvertex the SESVertex to swap contents with
		*/
		void swap(TSESVertex<T>& sesvertex)
		{
			TVector3<T> temp_point(p);
			p = sesvertex.p;
			sesvertex.p = temp_point;

			temp_point = n;
			n = sesvertex.n;
			sesvertex.n = temp_point;

			int temp = atom;
			atom = sesvertex.atom;
			sesvertex.atom = temp;
		}

		/**	Assign from another SESVertex.
				@param sesvertex	the SESVertex object to assign from
				@param deep ignored
		*/
		void set(const TSESVertex& sesvertex, bool /* deep */ = true)
		{
			p = sesvertex.p;
			n = sesvertex.n;
			atom = sesvertex.atom;
			index = sesvertex.index;
		}

		/**	Assign to two vectors and an index.
				@param	point assigned to the point representet by the vertex
				@param	norm assigned to the normal vector
				@param	a assigned to the index of the nearest atom
				@param	i assigned to the index of this vertex
		*/
		void set(const TVector3<T> point, const TVector3<T>& norm, const int& a, const int& i)
		{
			p = point; n = norm; atom = a; index = i;
		}

		//@}
		/**	@name	Predicates
		*/
		//@{

		//@}
		/**	@name	Attributes
		*/
		//@{

		/**	p.
				The coordinates of the vertex.
		*/
		TVector3<T> p;

		/**	n.
				The normal vector of the vertex.
		*/
		TVector3<T> n;

		/**	atom.
				The index of the closest atom.
		*/
		int atom;

		/**	index.
		*/
		int index;

		//@}
	};

	/**	@name	Storers
	*/
	//@{

	// BAUSTELLE : Input-Operator
	

	/**	Output- Operator
	*/
	template <typename T>
	std::ostream& operator << (std::ostream& s, const TSESVertex<T>& sesvertex)
	{
		return (s << "SESVERTEX" << sesvertex.index << "(" << sesvertex.p << ' ' << sesvertex.n << ' '
														 << sesvertex.atom << ")");
	}

	//@}

	/**	The Default SESVertex Type.
			If double precision is not needed, {\tt SESVertex<float>} should
			be used. It is predefined as {\tt SESVertex} for convenience.
	*/
	typedef TSESVertex<float> SESVertex;

} // namespace BALL

#endif // BALL_SES_SESVERTEX_H
