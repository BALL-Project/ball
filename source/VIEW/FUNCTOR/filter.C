// $Id: filter.C,v 1.4 2001/07/15 18:50:08 oliver Exp $

#include <BALL/VIEW/FUNCTOR/filter.h>

using namespace std;

namespace BALL
{

	namespace VIEW
	{

		Filter::Filter()
				throw()
			:	Visitor<Composite>(),
				object_passed_(false)
		{
		}

		Filter::~Filter()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<Filter>() << endl;
			#endif 

			destroy();
		}

		void Filter::clear()
			throw()
		{
				object_passed_ = false;
		}

		void Filter::destroy()
			throw()
		{
		}

		void Filter::visit(Composite& composite)
		{
			object_passed_ = RTTI::isKindOf<GeometricObject>(composite);
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/FUNCTOR/filter.iC>
#		endif

	} // namespace VIEW

} // namespace BALL
