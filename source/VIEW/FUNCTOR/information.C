// $Id: information.C,v 1.3 2001/05/13 14:28:35 hekl Exp $

#include <BALL/VIEW/FUNCTOR/information.h>

using namespace std;

namespace BALL
{

	namespace VIEW
	{

		Information::Information()
			throw()
			:	Visitor<Composite>()
		{
			clear();
		}

		Information::~Information()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<Information>() << endl;
			#endif 

			destroy();
		}

		void Information::clear()
			throw()
		{
				name_ = "unkown";
				type_name_ = "unkown type";
				type_ = TYPE__UNKNOWN;
		}

		void Information::destroy()
			throw()
		{
		}

		void Information::visit(Composite& composite)
			throw()
		{
			getType_(composite);
			getTypeName_(composite);
			getName_(composite);
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
