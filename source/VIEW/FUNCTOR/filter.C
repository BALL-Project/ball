// $Id: filter.C,v 1.1 2000/05/16 21:08:59 hekl Exp $

#include <BALL/VIEW/FUNCTOR/filter.h>

using namespace std;

namespace BALL
{

	namespace VIEW
	{

		Filter::Filter()
			:	Visitor<Composite>(),
				object_passed_(false)
		{
		}

		Filter::~Filter()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<Filter>() << endl;
			#endif 

			destroy();
		}

		void Filter::clear()
		{
				object_passed_ = false;
		}

		void Filter::destroy()
		{
			clear();
		}

		void Filter::visit(Composite& composite)
		{
			object_passed_ = RTTI::isKindOf<GeometricObject>(composite);
		}

		bool Filter::isValid() const
		{
			return true;
		}

		void Filter::dump
			(ostream& s, Size depth) const
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			s << "object passed: " << ((object_passed_ == true) ? "yes": "no") << endl;

			BALL_DUMP_STREAM_SUFFIX(s);
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/FUNCTOR/filter.iC>
#		endif

	} // namespace VIEW

} // namespace BALL
