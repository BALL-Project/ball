// $Id: glObjectCollector.C,v 1.1 2000/09/23 13:28:32 hekl Exp $

#include <BALL/VIEW/GUI/FUNCTOR/glObjectCollector.h>

using namespace std;

namespace BALL
{

	namespace VIEW
	{

		GLObjectCollector::GLObjectCollector()
			:	UnaryProcessor<Composite>(),
				composite_(0),
				static_list_(),
				static_always_front_list_(),
				static_wireframe_list_(),
				static_wireframe_always_front_list_(),
				dynamic_list_(),
				dynamic_always_front_list_(),
				transparent_list_(),
				transparent_always_front_list_()
		{
		}

		GLObjectCollector::GLObjectCollector
			(const GLObjectCollector& GL_object_collector, bool /* deep */)
			:	UnaryProcessor<Composite>(GL_object_collector),
				composite_(0),
				static_list_(),
				static_always_front_list_(),
				static_wireframe_list_(),
				static_wireframe_always_front_list_(),
				dynamic_list_(),
				dynamic_always_front_list_(),
				transparent_list_(),
				transparent_always_front_list_()
		{
		}

		GLObjectCollector::~GLObjectCollector()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<GLObjectCollector>() << endl;
			#endif 

			destroy();
		}

		void GLObjectCollector::clear()
		{
			static_list_.clear();
			static_always_front_list_.clear();
			static_wireframe_list_.clear();
			static_wireframe_always_front_list_.clear();
			dynamic_list_.clear();
			dynamic_always_front_list_.clear();
			transparent_list_.clear();
			transparent_always_front_list_.clear();
		}

		void GLObjectCollector::destroy()
		{
			composite_ = 0;

			clear();
		}

		void GLObjectCollector::set
			(const GLObjectCollector&  /* GL_object_collector */, bool /* deep */)
		{
			clear();
		}

		GLObjectCollector& GLObjectCollector::operator =
			(const GLObjectCollector& GL_object_collector)
		{
			set(GL_object_collector);

			return *this;
		}

		void GLObjectCollector::get
			(GLObjectCollector& GL_object_collector, bool deep) const
		{
			GL_object_collector.set(*this, deep);
		}

		void GLObjectCollector::swap(GLObjectCollector&  /* GL_object_collector */)
		{
		}

		bool GLObjectCollector::start()
		{
			#ifdef BALL_VIEW_DEBUG_PROCESSORS
				cout << "start collect process ..." << endl;
			#endif

			clear();

			return true;
		}
				
		bool GLObjectCollector::finish()
		{
			#ifdef BALL_VIEW_DEBUG_PROCESSORS
				cerr << "finished collect process ..." << endl;
				cerr << "              static wireframe: " << static_wireframe_list_.size() << endl;
				cerr << "static wireframe always front: " << static_wireframe_always_front_list_.size() << endl;
				cerr << "                        static: " << static_list_.size() << endl;
				cerr << "          static always front: " << static_always_front_list_.size() << endl;
				cerr << "                       dynamic: " << dynamic_list_.size() << endl;
				cerr << "         dynamic always front: " << dynamic_always_front_list_.size() << endl;
				cerr << "                   transparent: " << transparent_list_.size() << endl;
				cerr << "     transparent always front: " << transparent_always_front_list_.size() << endl;
			#endif

			return true;
		}
				
		Processor::Result GLObjectCollector::operator()
			(Composite& composite)
		{
			if (RTTI::isKindOf<GLObject>(composite) == false)
			{
				return Processor::CONTINUE;
			}

			#ifdef BALL_VIEW_DEBUG_PROCESSORS
				cout << "collecting ..." << endl;
			#endif

			GeometricObject *geometric_object = (GeometricObject*)RTTI::castTo<GeometricObject>(composite);

			if (geometric_object == 0)
			{
				return Processor::CONTINUE;
			}

			GLObject* GL_object = (GLObject*)RTTI::castTo<GLObject>(composite);

			#ifdef BALL_VIEW_DEBUG_PROCESSORS
				geometric_object->dump(cout);
				GL_object->dump(cout);
			#endif

			// static objects 
			// (choice: static, static wireframe, transparent, static always front, transparent always front)
			if (geometric_object->hasProperty(GeometricObject::PROPERTY__OBJECT_STATIC))
			{
				if (geometric_object->hasProperty(GeometricObject::PROPERTY__OBJECT_ALWAYS_FRONT)) // always front
				{
					// object transparent
					if (geometric_object->hasProperty(GeometricObject::PROPERTY__OBJECT_TRANSPARENT))
					{
            #ifdef BALL_VIEW_DEBUG_PROCESSORS
						  cout << "collect static-always front transparent" << endl;
  					#endif

  					transparent_always_front_list_.push_back(GL_object);
					}
					else // object not transparent
					{
						// static wireframe
						if (geometric_object->hasProperty(GeometricObject::PROPERTY__DRAWING_MODE_SOLID) == false)
						{
              #ifdef BALL_VIEW_DEBUG_PROCESSORS
					      cout << "collect static wireframe always front" << endl;
					    #endif

								static_wireframe_always_front_list_.push_back(GL_object);
						}
						else
						{
              #ifdef BALL_VIEW_DEBUG_PROCESSORS
						    cout << "collect static always front " << endl;
  					  #endif

							static_always_front_list_.push_back(GL_object);
						}
					}
				}
				else // z-Buffer enabled
				{
					// object transparent
					if (geometric_object->hasProperty(GeometricObject::PROPERTY__OBJECT_TRANSPARENT))
					{
            #ifdef BALL_VIEW_DEBUG_PROCESSORS
						  cout << "collect static transparent" << endl;
  					#endif

  					transparent_list_.push_back(GL_object);
					}
					else // object not transparent
					{
						if (geometric_object->hasProperty(GeometricObject::PROPERTY__DRAWING_MODE_SOLID) == false)
						{
              #ifdef BALL_VIEW_DEBUG_PROCESSORS
					      cout << "collect static wireframe" << endl;
					    #endif

								static_wireframe_list_.push_back(GL_object);
						}
						else
						{
              #ifdef BALL_VIEW_DEBUG_PROCESSORS
			  			  cout << "collect static " << endl;
  			  		#endif

						  static_list_.push_back(GL_object);
						}
					}
				}			
			}

			// dynamic objects
			// (choice: dynamic, dynamic always front)
			if (geometric_object->hasProperty(GeometricObject::PROPERTY__OBJECT_DYNAMIC))
			{
				if (geometric_object->hasProperty(GeometricObject::PROPERTY__OBJECT_ALWAYS_FRONT)) // always front
				{
          #ifdef BALL_VIEW_DEBUG_PROCESSORS
					 cout << "collect dynamic always front " << endl;
  				#endif

  				dynamic_always_front_list_.push_back(GL_object);
				}
				else // z-Buffer enabled
				{
          #ifdef BALL_VIEW_DEBUG_PROCESSORS
					 cout << "collect dynamic " << endl;
  				#endif

  				dynamic_list_.push_back(GL_object);
				}
			}

			return Processor::CONTINUE;
		}

		bool GLObjectCollector::isValid() const
		{
			return true;
		}

		void GLObjectCollector::dump
			(ostream& s, Size depth) const
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			BALL_DUMP_DEPTH(s, depth);
			s << "number of composites in:" << endl;

			BALL_DUMP_DEPTH(s, depth + 1);
			s << "static list: " 
					 << static_list_.size() << endl;

			BALL_DUMP_DEPTH(s, depth + 1);
			s << "static wireframe list: " 
					 << static_wireframe_list_.size() << endl;

			BALL_DUMP_DEPTH(s, depth + 1);
			s << "dynamic list: " 
					 << dynamic_list_.size() << endl;

			BALL_DUMP_DEPTH(s, depth + 1);
			s << "transparent list: " 
					 << transparent_list_.size() << endl;

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		void GLObjectCollector::read(istream & /* s */)
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		void GLObjectCollector::write(ostream & /* s */) const
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/GUI/FUNCTOR/glObjectCollector.iC>
#		endif

	} // namespace VIEW

} // namespace BALL
