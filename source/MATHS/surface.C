// $Id: surface.C,v 1.8 2000/08/03 12:24:52 anker Exp $

#include <BALL/MATHS/surface.h>

using namespace std;

namespace BALL
{

	Surface::Surface()
		:	valid_(false)
	{
	}

	Surface::Surface(const Surface& surface)
		:	vertex(surface.vertex),
			normal(surface.normal),
			triangle(surface.triangle),
			valid_(surface.valid_)
	{
	}

	Surface::~Surface()
	{
		valid_ = false;
	}

	void Surface::clear()
	{
		valid_ = true;
		vertex.clear();
		normal.clear();
		triangle.clear();
	}
	
	void Surface::destroy()
	{
		clear();
	}

	void Surface::set(const Surface& surface)
	{
		vertex = surface.vertex;
		normal = surface.normal;
		triangle = surface.triangle;
		valid_ = surface.valid_;
	}

	const Surface& Surface::operator = (const Surface& surface)
	{
		vertex = surface.vertex;
		normal = surface.normal;
		triangle = surface.triangle;
		valid_ = surface.valid_;
		return *this;
	}
	
	void Surface::get(Surface& surface) const
	{
		surface.vertex = vertex;
		surface.normal = normal;
		surface.triangle = triangle;
		surface.valid_ = valid_;
	}
	
	void Surface::readMSMSFile
		(const String& vert_filename, const String& face_filename)
	{
		// delete old contents
		normal.clear();
		vertex.clear();
		triangle.clear();

		ifstream file(vert_filename.c_str());
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

	float Surface::getArea() const
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

	bool Surface::isValid() const
	{
		return valid_;
	}


} // namespace BALL
