// $Id: surface.h,v 1.6 2000/10/10 19:37:11 oliver Exp $

#ifndef BALL_MATHS_SURFACE_H
#define BALL_MATHS_SURFACE_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector3.h>
#endif

namespace BALL 
{

	/**	Generic Three-dimensional Surface class.
			This class describes a three-dimensional triangulated surface. 
			\\
			{\bf Definition:} \URL{BALL/MATHS/surface.h}
			\\
	*/
	template <typename T>
	class TSurface
	{
		public:

		BALL_CREATE(TSurface)

		/**	@name	Type Definitions
		*/
		//@{

		/**
		*/
		struct Triangle
		{
			Index	v1;
			Index	v2;
			Index	v3;
		};
		//@}

		/**	@name	Constructors and Destructors
		*/
		//@{

		///
		TSurface();

		///
		TSurface(const TSurface& surface);

		///
		virtual ~TSurface();

		///
		void clear();

		///
		void destroy();
		//@}

		/**	@name	Assignment
		*/
		//@{

		///
		void set(const TSurface& box);

		///
		const TSurface& operator = (const TSurface& box);

		///
		void get(TSurface& box) const;

		/**	Read from MSMS file.
				Read the contents of the vertex and faces file created by Michael
				Sanners software MSMS.
		*/
		void readMSMSFile(const String& vert_filename, const String& face_filename);
		//@}

		/**	@name	Accessors
		*/
		//@{

		///
		float getArea() const;
		//@}

		/**	@name	Predicates
		*/
		//@{

		///
		bool operator == (const TSurface& box) const;

		///
		bool operator != (const TSurface& box) const;
		//@}

		/**	@name	Debugging and Diagnostics
		*/
		//@{
		///
		bool isValid() const;
		//@}

		/**	@name	Attributes
		*/
		//@{

		/// the vertices
		vector<TVector3<T> >		vertex;

		/// the normals for each vertex
		vector<TVector3<T> >		normal;

		/// the triangles
		vector<Triangle>				triangle;
		//@}

		protected:

		bool valid_;
	};

	/**	Default surface type.
	*/
	typedef TSurface<float> Surface;

} // namespace BALL

#endif // BALL_MATHS_SURFACE_H
