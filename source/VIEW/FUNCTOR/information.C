// $Id: information.C,v 1.1 2000/05/04 17:22:07 hekl Exp $

#include <BALL/VIEW/FUNCTOR/information.h>

using namespace std;

namespace BALL
{

	namespace VIEW
	{

		Information::Information()
			:	Visitor<Composite>()
		{
		}

		Information::Information
			(const Information& info, bool /* deep */)
			:	Visitor<Composite>(info),
				name_("unkown"),
				type_name_("unkown type"),
				type_(TYPE__UNKNOWN)
		{
		}

		Information::~Information()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<Information>() << endl;
			#endif 

			destroy();
		}

		void Information::clear()
		{
				name_ = "unkown";
				type_name_ = "unkown type";
				type_ = TYPE__UNKNOWN;
		}

		void Information::destroy()
		{
			clear();
		}

		void Information::set
			(const Information& info,
			 bool /* deep */)
		{
			name_ = info.name_;
			type_name_ = info.type_name_;
			type_ = info.type_;
		}

		Information& 	Information::operator =
			(const Information& info)
		{
			set(info);

			return *this;
		}

		void Information::get
			(Information& info, bool deep) const
		{
			info.set(*this, deep);
		}

		void Information::swap
			(Information& info)
		{
			name_.swap(info.name_);
			type_name_.swap(info.type_name_);

			Type temp = type_;
			type_ = info.type_;
			info.type_ = temp;
		}

		void Information::visit(Composite& composite)
		{
			getType_(composite);
			getTypeName_(composite);
			getName_(composite);
		}

		bool Information::isValid() const
		{
			return true;
		}

		void Information::dump
			(ostream& s, Size depth) const
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			BALL_DUMP_DEPTH(s, depth);
			s << "name: " << name_ << endl;
					
			BALL_DUMP_DEPTH(s, depth);
			s << "type_name: " << type_name_ << endl;
					
			BALL_DUMP_STREAM_SUFFIX(s);
		}

	  void Information::getType_(Composite& composite)
    {
			Information::Type type = TYPE__UNKNOWN;
			
			if (RTTI::isKindOf<Line>(composite))
			{
				type = TYPE__LINE;
			}	
			else if (RTTI::isKindOf<Sphere>(composite))
			{
				type = TYPE__SPHERE;
			}
			else if (RTTI::isKindOf<Tube>(composite))
			{
				type = TYPE__TUBE;
			}
			else if (RTTI::isKindOf<Point>(composite))
			{
				type = TYPE__POINT;
			}	
			else if (RTTI::isKindOf<Mesh>(composite))
			{
				type = TYPE__MESH;
			}	
			else if (RTTI::isKindOf<SimpleBox>(composite))
			{
				type = TYPE__SIMPLE_BOX;
			}	
			else if (RTTI::isKindOf<GeometricObject>(composite))
			{
				type = TYPE__GEOMETRIC_OBJECT;
			}	
			else
			{
				type = TYPE__COMPOSITE;
			}	

			type_ = type;
    }

	  void Information::getTypeName_(Composite& composite)
    {
			String temp;
			
			switch(type_)
			{
			  case TYPE__LINE:
					temp = "Line";
					break;

			  case TYPE__SPHERE:
					temp = "Sphere";
					break;
					
			  case TYPE__TUBE:
					temp = "Tube";
					break;
					
			  case TYPE__POINT:
					temp = "Point";
					break;
					
			  case TYPE__MESH:
					temp = "Mesh";
					break;
					
			  case TYPE__SIMPLE_BOX:
					temp = "SimpleBox";
					break;
					
  			case TYPE__GEOMETRIC_OBJECT:
					{
						// every GeometricObject has a type name
						// (default set to: GeometricObject)
						// new objects can write their type in this variable
						GeometricObject* object = RTTI::castTo<GeometricObject>(composite);

						temp = object->getTypeName();
					}
					break;
					
			  case TYPE__COMPOSITE:
					temp = "Composite";
					break;
					
  			default:
					temp = "unknown";
					break;
			}

			type_name_ = temp;
    }

	  void Information::getName_(Composite& composite)
    {
			String temp = "UNKNOWN";
			
			switch(type_)
			{
			  case TYPE__LINE:
			  case TYPE__SPHERE:
			  case TYPE__TUBE:
			  case TYPE__POINT:
			  case TYPE__MESH:
			  case TYPE__SIMPLE_BOX:
			  case TYPE__GEOMETRIC_OBJECT:
				{
					GeometricObject* object = RTTI::castTo<GeometricObject>(composite);
					temp = object->getName();
				}
				break;
				
			  case TYPE__COMPOSITE:
				{
					temp = "";
				}
				break;
				
			  default:
				  break;
			}
			
			// empty string
			if (temp == "")
			{
				temp = "<";
				temp += type_name_;
				temp += ">";		
			}
			
			name_ = temp;
    }

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/FUNCTOR/information.iC>
#		endif

	} // namespace VIEW

} // namespace BALL
