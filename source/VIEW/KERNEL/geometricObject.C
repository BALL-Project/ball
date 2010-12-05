// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/KERNEL/geometricObject.h>
#include <BALL/VIEW/DATATYPE/colorExtensions.h>

using std::endl;
using std::ostream;
using std::istream;

namespace BALL
{
	namespace VIEW
	{

		GeometricObject::GeometricObject()
			: color_(),
				composite_(0)
		{
		}

		GeometricObject::GeometricObject(const GeometricObject& geometric_object)
			: color_(geometric_object.color_),
				composite_(geometric_object.composite_)
		{
		}

		GeometricObject::~GeometricObject()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.info() << "Destructing object " << this << " of class " << RTTI::getName<GeometricObject>() << endl;
			#endif 
		}

		void GeometricObject::clear()
		{
			color_ = ColorRGBA();
			composite_ = 0;
		}

		void GeometricObject::set(const GeometricObject& geometric_object)
		{
			color_ = geometric_object.color_;
			composite_ = geometric_object.composite_;
		}

		GeometricObject& GeometricObject::operator = (const GeometricObject& geometric_object)
		{
			set(geometric_object);
			return *this;
		}

		void GeometricObject::swap(GeometricObject& geometric_object)
		{
			const Composite* composite = geometric_object.composite_;
			geometric_object.composite_ = composite_;
			composite_ = composite;
		}

		void GeometricObject::dump(ostream& s, Size depth) const
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			BALL_DUMP_DEPTH(s,depth);
			s << "composite 		 : " << composite_ << endl;

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		void GeometricObject::getColors(HashSet<String>& color_set)
		{
			String c;

			MultiColorExtension* mce = dynamic_cast<MultiColorExtension*>(this);
			if (mce != 0)
			{
				const vector<ColorRGBA>& cs = mce->getColors();
				for (Position p = 0; p < cs.size(); p++)
				{
					cs[p].get(c);
					color_set.insert(c);
				}

				return;
			}

			ColorExtension2* ce2 = dynamic_cast<ColorExtension2*>(this);
			if (ce2 != 0)
			{
				ce2->getColor2().get(c);
				color_set.insert(c);
			}

			getColor().get(c);
			color_set.insert(c);
		}
			
	} // namespace VIEW
} // namespace BALL
