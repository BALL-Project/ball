// $Id: surface.h,v 1.5 2000/08/30 19:58:14 oliver Exp $

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

	/**	Three-dimensional Surface object.
			This class describes a three-dimensional triangulated surface. \\
			{\bf Definition:} \URL{BALL/MATHS/surface.h}
			\\
	*/
	class Surface
	{
		public:

		BALL_CREATE(Surface)

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
		Surface();

		///
		Surface(const Surface& surface);
			
		///
		virtual ~Surface();

		///
		void clear();

		///
		void destroy();
		//@}

		/**	@name	Assignment
		*/
		//@{

		///
		void set(const Surface& box);

		///
		const Surface& operator = (const Surface& box);

		///
		void get(Surface& box) const;

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
		bool operator == (const Surface& box) const;

		///
		bool operator != (const Surface& box) const;
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
		vector<Vector3>		vertex;

		/// the normals for each vertex
		vector<Vector3>		normal;

		/// the triangles
		vector<Triangle>	triangle;
		//@}

		protected:

		bool valid_;
	};

} // namespace BALL

#endif // BALL_MATHS_SURFACE_H
