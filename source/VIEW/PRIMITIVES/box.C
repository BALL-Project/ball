// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: box.C,v 1.4.28.1 2007-03-25 22:02:33 oliver Exp $

#include <BALL/VIEW/PRIMITIVES/box.h>

using namespace std;

namespace BALL
{
	namespace VIEW
	{

		Box::Box()
			:	GeometricObject(),
				Box3()
		{
		}

		Box::Box(const Box& box)
			:	GeometricObject(box),
				Box3(box)
		{
		}

		Box::Box(const Vector3& point, 
				const Vector3& right_vector,
				const Vector3& height_vector,
				float depth)
			: GeometricObject(),
				Box3(point, right_vector, height_vector, depth)
		{
		}

		Box::Box(const Vector3& point, 
				const Vector3& right_vector,
				const Vector3& height_vector,
				const Vector3& depth_vector)
			: GeometricObject(),
				Box3(point, right_vector, height_vector, depth_vector)
		{}
		
		Box::~Box()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.error << "Destructing object " << (void *)this 
									<< " of class " << RTTI::getName<Box>() << std::endl;
			#endif 
		}

		void Box::clear()
		{
			GeometricObject::clear();
			Box3::clear();
		}

		void Box::set(const Box& box)
		{
			GeometricObject::set(box);
			Box3::set(box);
		}

		const Box& Box::operator = (const Box& box)
		{
			set(box);
			return *this;
		}

		bool Box::isValid() const
		{
			return (GeometricObject::isValid() && 
							        	 Box3::isValid());
		}

		void Box::dump(ostream& s, Size depth) const
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			GeometricObject::dump(s, depth + 1);
			Box3::dump(s, depth + 1);

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		void Box::getVertices(vector<Vector3>& vertices) const
		{
			vertices.push_back(point_);
			vertices.push_back(point_ + right_vector_ * width_ + height_vector_ * height_ + depth_vector_ * depth_);
		}


	} // namespace VIEW
} // namespace BALL
