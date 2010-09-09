// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: mesh.C,v 1.6.16.1 2007-03-25 22:02:34 oliver Exp $

#include <BALL/VIEW/PRIMITIVES/mesh.h>
#include <BALL/SYSTEM/file.h>
#include <BALL/SYSTEM/binaryFileAdaptor.h>

using namespace std;

namespace BALL
{
	namespace VIEW
	{

		Mesh::Mesh()
			:	GeometricObject(),
				Surface(),
				MultiColorExtension()
		{
		}

		Mesh::Mesh(const Mesh& mesh)
			:	GeometricObject(mesh),
				Surface(mesh),
				MultiColorExtension(mesh)
		{
		}

		Mesh::~Mesh()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.error() << "Destructing object " << (void *)this 
										<< " of class " << RTTI::getName<Mesh>() << endl;
			#endif 
		}

		void Mesh::clear()
		{
			GeometricObject::clear();
			MultiColorExtension::clear();
		}

		void Mesh::set(const Mesh& mesh)
		{
			GeometricObject::set(mesh);
			MultiColorExtension::set(mesh);
		}

		const Mesh& Mesh::operator = (const Mesh& mesh)
		{
			set(mesh);
			return *this;
		}

		void Mesh::swap(Mesh& mesh)
		{
			GeometricObject::swap(mesh);
//   			Surface::swap(mesh);
			MultiColorExtension::swap(mesh);
		}

		bool Mesh::isValid() const
		{
			return (GeometricObject::isValid());
		}

		void Mesh::dump(ostream& s, Size depth) const
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			GeometricObject::dump(s, depth + 1);
			MultiColorExtension::dump(s, depth + 1);
			
			BALL_DUMP_STREAM_SUFFIX(s);
		}

		void Mesh::getVertices(vector<Vector3>& vertices) const
		{
			vertices.reserve(vertices.size() + vertex.size());
			for (Position p = 0; p < vertex.size(); p++)
			{
				vertices.push_back(vertex[p]);
			}
		}

		bool Mesh::binaryWrite(const String& filename)
		{
			BinaryFileAdaptor<float> float_ad;
			BinaryFileAdaptor<Size>   size_ad;
			BinaryFileAdaptor<char>   char_ad;
			try
			{
				File outfile(filename, std::ios::out|std::ios::binary);
				outfile << size_ad << vertex.size();
				outfile << size_ad << triangle.size();
				outfile << size_ad << colors.size();
				for (Position p = 0; p < vertex.size(); p++)
				{
					outfile << float_ad << vertex[p].x;
					outfile << float_ad << vertex[p].y;
					outfile << float_ad << vertex[p].z;
				}
				for (Position p = 0; p < normal.size(); p++)
				{
					outfile << float_ad << normal[p].x;
					outfile << float_ad << normal[p].y;
					outfile << float_ad << normal[p].z;
				}
				for (Position p = 0; p < triangle.size(); p++)
				{
					outfile << size_ad << triangle[p].v1;
					outfile << size_ad << triangle[p].v2;
					outfile << size_ad << triangle[p].v3;
				}
				for (Position p = 0; p < colors.size(); p++)
				{
					outfile << char_ad << (char)colors[p].getRed();
					outfile << char_ad << (char)colors[p].getGreen();
					outfile << char_ad << (char)colors[p].getBlue();
					outfile << char_ad << (char)colors[p].getAlpha();
				}
				outfile.close();
				return true;
			}
			catch(...)
			{
			}

			return false;
		}

		bool Mesh::binaryRead(const String& filename)
		{
			clear();

			BinaryFileAdaptor<float> float_ad;
			BinaryFileAdaptor<Size>   size_ad;
			BinaryFileAdaptor<char>   char_ad;
			try
			{
				File infile(filename, std::ios::in|std::ios::binary);
				Size vs, ts, cs;
				infile >> size_ad >> vs;
				infile >> size_ad >> ts;
				infile >> size_ad >> cs;
				vertex.resize(vs);
				triangle.resize(ts);
				colors.resize(cs);
				normal.resize(vs);
				for (Position p = 0; p < vertex.size(); p++)
				{
					infile >> float_ad >> vertex[p].x;
					infile >> float_ad >> vertex[p].y;
					infile >> float_ad >> vertex[p].z;
				}
				for (Position p = 0; p < normal.size(); p++)
				{
					infile >> float_ad >> normal[p].x;
					infile >> float_ad >> normal[p].y;
					infile >> float_ad >> normal[p].z;
				}
				for (Position p = 0; p < triangle.size(); p++)
				{
					infile >> size_ad >> triangle[p].v1;
					infile >> size_ad >> triangle[p].v2;
					infile >> size_ad >> triangle[p].v3;
				}
				char r,g,b,a;
				for (Position p = 0; p < colors.size(); p++)
				{
					infile >> char_ad >> r;
					infile >> char_ad >> g;
					infile >> char_ad >> b;
					infile >> char_ad >> a;
					colors[p].setRed(r);
					colors[p].setGreen(g);
					colors[p].setBlue(b);
					colors[p].setAlpha(a);
				}
				infile.close();
				return true;
			}
			catch(...)
			{
			}

			return false;
		}


	} // namespace VIEW
} // namespace BALL
