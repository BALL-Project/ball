// $Id: surface.h,v 1.9 2001/06/21 02:32:48 oliver Exp $

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
		TSurface()
			throw();

		///
		TSurface(const TSurface& surface)
			throw();

		///
		virtual ~TSurface()
			throw();
		//@}

		/**	@name	Assignment
		*/
		//@{

		///
		void set(const TSurface& box)
			throw();

		///
		const TSurface& operator = (const TSurface& box)
			throw();

		///
		void get(TSurface& box) const
			throw();

		///
		void clear()
			throw();

		/**	Read from MSMS file.
				Read the contents of the vertex and faces file created by Michael
				Sanners software MSMS.
		*/
		void readMSMSFile(const String& vert_filename, const String& face_filename)
			throw(Exception::FileNotFound);
		//@}

		/**	@name	Accessors
		*/
		//@{

		///
		float getArea() const
			throw();
		//@}

		/**	@name	Predicates
		*/
		//@{

		///
		bool operator == (const TSurface& box) const
			throw();

		///
		bool operator != (const TSurface& box) const
			throw();
		//@}

		/**	@name	Debugging and Diagnostics
		*/
		//@{
		///
		bool isValid() const
			throw();
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

	template <typename T>
	TSurface<T>::TSurface()
		throw()
		:	valid_(false)
	{
	}

	template <typename T>
	TSurface<T>::TSurface(const TSurface<T>& surface)
		throw()
		:	vertex(surface.vertex),
			normal(surface.normal),
			triangle(surface.triangle),
			valid_(surface.valid_)
	{
	}

	template <typename T>
	TSurface<T>::~TSurface()
		throw()
	{
		valid_ = false;
	}

	template <typename T>
	void TSurface<T>::clear()
		throw()
	{
		valid_ = true;
		vertex.clear();
		normal.clear();
		triangle.clear();
	}
	
	template <typename T>
	void TSurface<T>::set(const TSurface<T>& surface)
		throw()
	{
		vertex = surface.vertex;
		normal = surface.normal;
		triangle = surface.triangle;
		valid_ = surface.valid_;
	}

	template <typename T>
	const TSurface<T>& TSurface<T>::operator = (const TSurface<T>& surface)
		throw()
	{
		vertex = surface.vertex;
		normal = surface.normal;
		triangle = surface.triangle;
		valid_ = surface.valid_;
		return *this;
	}
	
	template <typename T>
	void TSurface<T>::get(TSurface<T>& surface) const
		throw()
	{
		surface.vertex = vertex;
		surface.normal = normal;
		surface.triangle = triangle;
		surface.valid_ = valid_;
	}
	
	template <typename T>
	void TSurface<T>::readMSMSFile(const String& vert_filename, const String& face_filename)
		throw(Exception::FileNotFound)
	{
		// delete old contents
		normal.clear();
		vertex.clear();
		triangle.clear();

		std::ifstream file(vert_filename.c_str());
		if (!file)
		{
			throw Exception::FileNotFound(__FILE__, __LINE__, vert_filename);
		}

		// there are two formats: one with three lines of 
		// header and one without
		String line;
		while ((line.countFields() != 9) && file)
		{
			line.getline(file);
		}
		
		String s[6];
		while (file && (line.countFields() == 9))
		{
			// read the vertex coordinates and the normal vector 
			line.split(s, 6);
			vertex.push_back(Vector3(s[0].toFloat(), s[1].toFloat(), s[2].toFloat()));
			normal.push_back(Vector3(s[3].toFloat(), s[4].toFloat(), s[5].toFloat()));
			
			// read the next line
			line.getline(file);
		}
		file.close();
		// workaround for trouble in File
		file.clear();

		// now read the faces file:
		file.open(face_filename.c_str());
		if (!file)
		{
			throw Exception::FileNotFound(__FILE__, __LINE__, face_filename);
		}

		// there are two formats: one with three lines of 
		// header and one without
		while ((line.countFields() != 5) && file)
		{
			line.getline(file);
		}
		
		Triangle t;
		Size number_of_vertices = vertex.size();
		while (file && (line.countFields() == 5))
		{
			// read the vertex indices
			line.split(s, 5);
			t.v1 = (Index)s[0].toInt() - 1;
			t.v2 = (Index)s[1].toInt() - 1;
			t.v3 = (Index)s[2].toInt() - 1;

			// if all three vertex indices are valid, insert the triangle
			if ((t.v1 < (Index)number_of_vertices) && (t.v1 >= 0)
					&& (t.v1 < (Index)number_of_vertices) && (t.v1 >= 0)
					&& (t.v1 < (Index)number_of_vertices) && (t.v1 >= 0))
			{
				triangle.push_back(t);
			}
			
			// read the next line
			line.getline(file);
		}
		file.close();
	}

	template <typename T>
	float TSurface<T>::getArea() const
		throw()
	{
		float area = 0;
		// add the areas of all triangles
		for (Size i = 0; i < triangle.size(); i++)
		{
			//Vector3 v1 = vertex[triangle[i].v1];
			//Vector3 v2 = vertex[triangle[i].v2];
			//Vector3 v3 = vertex[triangle[i].v3];
			
			// projection of v3 onto v1-v2
			//Vector3	v4 = 
			// BAUSTELLE
		}
		
		return area;
	}

	template <typename T>
	bool TSurface<T>::isValid() const
		throw()
	{
		return valid_;
	}

	/**	Default surface type.
	*/
	typedef TSurface<float> Surface;

} // namespace BALL

#endif // BALL_MATHS_SURFACE_H
