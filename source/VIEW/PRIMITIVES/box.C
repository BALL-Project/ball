// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: box.C,v 1.2 2003/08/26 15:32:47 amoll Exp $

#include <BALL/VIEW/PRIMITIVES/box.h>

using namespace std;

namespace BALL
{
	namespace VIEW
	{

		Box::Box()
			throw()
			:	GeometricObject(),
				Box3()
		{
		}

		Box::Box(const Box& box, bool deep)
			throw()
			:	GeometricObject(box, deep),
				Box3(box)
		{
		}

		Box::Box(const Vector3& point, 
				const Vector3& right_vector,
				const Vector3& height_vector,
				float depth)
			throw()
			: GeometricObject(),
				Box3(point, right_vector, height_vector, depth)
		{}


		Box::~Box()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.error << "Destructing object " << (void *)this 
									<< " of class " << RTTI::getName<Box>() << std::endl;
			#endif 
		}

		void Box::clear()
			throw()
		{
			GeometricObject::clear();
			Box3::clear();
		}

		void Box::set(const Box& box, bool deep)
			throw()
		{
			GeometricObject::set(box, deep);
			Box3::set(box);
		}

		const Box& Box::operator = (const Box& box)
			throw()
		{
			set(box);
			return *this;
		}

		void Box::get(Box& box, bool deep) const
			throw()
		{
			box.set(*this, deep);
		}

		bool Box::isValid() const
			throw()
		{
			return (GeometricObject::isValid() && 
							        	 Box3::isValid());
		}

		void Box::dump(ostream& s, Size depth) const
			throw()
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			GeometricObject::dump(s, depth + 1);
			Box3::dump(s, depth + 1);

			BALL_DUMP_STREAM_SUFFIX(s);
		}

	} // namespace VIEW
} // namespace BALL
