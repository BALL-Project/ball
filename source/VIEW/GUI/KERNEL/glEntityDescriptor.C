// $Id: glEntityDescriptor.C,v 1.3 2001/02/11 13:06:31 hekl Exp $

#include <BALL/VIEW/GUI/KERNEL/glEntityDescriptor.h>
#include <BALL/COMMON/exception.h>

using namespace std;

namespace BALL
{

	namespace VIEW
	{

		GLEntityDescriptor::GLEntityDescriptor()
			:	static_display_list_(),
				static_always_front_display_list_(),
				static_wireframe_display_list_(),
				static_wireframe_always_front_display_list_(),
				dynamic_display_list_(),
				dynamic_always_front_display_list_(),
				transparent_display_list_(),
				transparent_always_front_display_list_(),
				update_(true)
		{
		}
			

		GLEntityDescriptor::~GLEntityDescriptor()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this << " of class " 
					<< RTTI::getName<GLEntityDescriptor>() << endl;
			#endif 

			destroy();
		}

		void GLEntityDescriptor::clear()
		{
			static_display_list_.destroy();
			static_always_front_display_list_.destroy();
			static_wireframe_display_list_.destroy();
			static_wireframe_always_front_display_list_.destroy();
			dynamic_display_list_.destroy();
			dynamic_always_front_display_list_.destroy();
			transparent_display_list_.destroy();
			transparent_always_front_display_list_.destroy();

			update_ = true;
		}

		void GLEntityDescriptor::destroy()
		{
		}

		bool GLEntityDescriptor::isValid() const
		{
			return (static_display_list_.isValid()
										&& static_always_front_display_list_.isValid()
										&& static_wireframe_display_list_.isValid()
										&& static_wireframe_always_front_display_list_.isValid()
										&& dynamic_display_list_.isValid()
										&& dynamic_always_front_display_list_.isValid()
										&& transparent_display_list_.isValid()
										&& transparent_always_front_display_list_.isValid());
		}

		void GLEntityDescriptor::dump
			(ostream& s, Size depth) const
		{
			BALL_DUMP_STREAM_PREFIX(s);

			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			static_display_list_.dump(s, depth + 1);
			static_always_front_display_list_.dump(s, depth + 1);
			static_wireframe_display_list_.dump(s, depth + 1);
			static_wireframe_always_front_display_list_.dump(s, depth + 1);
			dynamic_display_list_.dump(s, depth + 1);
			dynamic_always_front_display_list_.dump(s, depth + 1);
			transparent_display_list_.dump(s, depth + 1);
			transparent_always_front_display_list_.dump(s, depth + 1);
			
			BALL_DUMP_STREAM_SUFFIX(s);     
		}

		void GLEntityDescriptor::read(istream &/* s */)
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		void GLEntityDescriptor::write(ostream &/*s*/) const
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/GUI/KERNEL/glEntityDescriptor.iC>
#		endif
		
	} // namespace VIEW

} // namespace BALL
