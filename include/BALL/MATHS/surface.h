// $Id: surface.h,v 1.3 2000/03/26 21:54:12 oliver Exp $

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

		BALL_CREATE_NODEEP(Surface)

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
		//@}

		/**	@name	Assignment
		*/
		//@{

		///
		void set(const Surface& box);

		///
		Surface& operator = (const Surface& box);

		///
		void get(Surface& box) const;

		///
		void swap(Surface& box);

		/**	Read from MSMS file.
				Read the contents of the vertex and faces file created by Michael
				Sanners MSMS.
		*/
		void readMSMSFile(const String& vert_filename, const String& face_filename);
		//@}

		/**	@name	Accessors
		*/
		//@{

		///
		float getVolume() const;

		///
		float getArea() const;
	
		///	
		Size getNumberOfTriangles() const;
	
		///	
		Size getNumberOfVertices() const;

		//	
		Triangle triangle(Size index) const;

		//	
		Vector3 vertex(Size index) const;

		//	
		Vector3 normal(Size index) const;

		///
		void smoothNormals();
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


		protected:

		bool valid_;

		vector<Vector3>		vertices_;
		vector<Vector3>		normals_;
		vector<Triangle>	triangles_;
	};

} // namespace BALL

#endif // BALL_MATHS_SURFACE_H
