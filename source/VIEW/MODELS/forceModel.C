// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: forceModel.C,v 1.11.18.1 2007/03/25 22:02:31 oliver Exp $
//

#include <BALL/VIEW/MODELS/forceModel.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/VIEW/PRIMITIVES/mesh.h>
#include <BALL/VIEW/KERNEL/common.h>

using namespace std;

namespace BALL
{
	namespace VIEW
	{

		ForceModel::ForceModel()
			: ModelProcessor(),
				scaling_(1.1),
				max_length_(10),
				base_size_(0.2),
				offset_(0.2)
		{
		}

		ForceModel::ForceModel(const ForceModel& model)
			: ModelProcessor(model),
				scaling_(model.scaling_),
				max_length_(model.max_length_),
				base_size_(model.base_size_),
				offset_(model.offset_)
		{
		}

		ForceModel::~ForceModel()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.error() << "Destructing object " << this << " of class ForceModel" << std::endl;
			#endif 
		}

		Processor::Result ForceModel::operator() (Composite &composite)
		{
			Atom* atom = dynamic_cast<Atom*>(&composite);
			if (atom == 0) return Processor::CONTINUE;

			Vector3 force = atom->getForce() * 1E15;
			if (Maths::isZero(force.getSquareLength())) return Processor::CONTINUE;
			float forcev = log(force.getLength()) * scaling_; 

			if (forcev < 0) return Processor::CONTINUE;

			if (forcev > max_length_) forcev = max_length_;

			// prevent problems in normalize
			force *= 10000000000.0;
			force.normalize();
			Vector3 forcen = force;
			force *= forcev;

			Mesh* mesh = new Mesh();

			Vector3 start = atom->getPosition() + offset_ * forcen;
			Vector3 n1 = VIEW::getNormal(forcen);
			n1 *= base_size_ / 2.0;
			Vector3 n2 = n1 % forcen;
			n2.normalize();
			n2 *= base_size_ / 2.0;
			n2 *= 0.2;

			vector<Vector3>& vertices = mesh->vertex;
			vector<Vector3>& normals = mesh->normal;
			vector<Mesh::Triangle>& tris = mesh->triangle;
			Mesh::Triangle t;

			// base:
			vertices.push_back(start + n1 - n2);
			vertices.push_back(start + n1 + n2);
			vertices.push_back(start - n1 + n2);
			vertices.push_back(start - n1 - n2);
			normals.push_back(-forcen);
			normals.push_back(-forcen);
			normals.push_back(-forcen);
			normals.push_back(-forcen);
			t.v1 = 0;
			t.v2 = 1;
			t.v3 = 2;
			tris.push_back(t);
			t.v1 = 2;
			t.v2 = 3;
			t.v3 = 0;
			tris.push_back(t);

			Vector3 tip = start + force;

			// sides:
			for (Position p = 0; p < 4; p++)
			{
				Position x = p;
				vertices.push_back(vertices[x]);
				x++;
				if (x == 4) x = 0;
				vertices.push_back(vertices[x]);
				vertices.push_back(tip);
				n1 = (vertices[x] - vertices[p]) % (tip - vertices[x]);
				normals.push_back(n1);
				normals.push_back(n1);
				normals.push_back(n1);
				Index s = vertices.size() - 3;
				t.v1 = s;
				t.v2 = s + 1;
				t.v3 = s + 2;

				tris.push_back(t);
			}

			mesh->setComposite(atom);
			geometric_objects_.push_back(mesh);

			return Processor::CONTINUE;
		}

	} // namespace VIEW
} // namespace BALL
