// $Id: surface.C,v 1.3 1999/12/28 18:15:29 oliver Exp $

#include <BALL/MATHS/surface.h>

using namespace std;

namespace BALL
{

	Surface::Surface()
		:	valid_(false)
	{
	}

	Surface::Surface(const Surface& /* surface */, bool /* deep */)
	{
		// BAUSTELLE
	}

	Surface::~Surface()
	{
	}

	Size Surface::getNumberOfVertices() const
	{
		return vertices_.size();
	}
	
	Size Surface::getNumberOfTriangles() const
	{
		return triangles_.size();
	}

	void Surface::smoothNormals()
	{
		if (valid_ == false)
		{
			return;
		}

		Size i;
		// clear all normals
		for (i = 0; i < normals_.size(); i++)
		{
			normals_[i].set(0.0);
		}

		// iterate over all triangles, calculate their normals
		// and add them to the vertices` normals
		for (i = 0; i < triangles_.size(); i++)
		{
			Vector3 v1 = vertices_[triangles_[i].v1];
			Vector3 v2 = vertices_[triangles_[i].v2];
			Vector3 v3 = vertices_[triangles_[i].v3];
			Vector3 normal = (v2 - v1) % (v3 - v1);
			normal.normalize();
			normals_[triangles_[i].v1] += normal;
			normals_[triangles_[i].v2] += normal;
			normals_[triangles_[i].v3] += normal;
		}

		// iterate over all normals and normalize them
		for (i = 0; i < normals_.size(); ++i)
		{
			normals_[i].normalize();
		}
	}
	

	void Surface::readMSMSFile(const String& vert_filename, const String& face_filename)
	{
		// delete old contents
		normals_.clear();
		vertices_.clear();
		triangles_.clear();

		ifstream	file(vert_filename.c_str());
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
			vertices_.push_back(Vector3(s[0].toFloat(), s[1].toFloat(), s[2].toFloat()));
			normals_.push_back(Vector3(s[3].toFloat(), s[4].toFloat(), s[5].toFloat()));
			
			// read the next line
			line.getline(file);
		}
		file.close();

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
		Size number_of_vertices = vertices_.size();
		while (file && (line.countFields() == 5))
		{
			Log.info() << "reading line " << line << endl;
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
				triangles_.push_back(t);
			}
			
			// read the next line
			line.getline(file);
		}
		file.close();
	}

	float Surface::getVolume() const
	{
		// BAUSTELLE
		return 0;
	}

	float Surface::getArea() const
	{
		float area = 0;
		// add the areas of all triangles
		for (Size i = 0; i < triangles_.size(); i++)
		{
			//Vector3 v1 = vertices_[triangles_[i].v1];
			//Vector3 v2 = vertices_[triangles_[i].v2];
			//Vector3 v3 = vertices_[triangles_[i].v3];
			
			// projection of v3 onto v1-v2
			//Vector3	v4 = 
			// BAUSTELLE
		}
		
		return area;
	}

	Surface::Triangle Surface::triangle(Size index) const
	{
		return triangles_[index];
	}

	Vector3 Surface::vertex(Size index) const
	{
		return vertices_[index];
	}

	Vector3 Surface::normal(Size index) const
	{
		return normals_[index];
	}

} // namespace BALL
