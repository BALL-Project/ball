// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: information.C,v 1.6 2003/08/26 09:18:35 oliver Exp $

#include <BALL/VIEW/FUNCTOR/information.h>
#include <BALL/VIEW/PRIMITIV/line.h>
#include <BALL/VIEW/PRIMITIV/sphere.h>
#include <BALL/VIEW/PRIMITIV/tube.h>
#include <BALL/VIEW/PRIMITIV/point.h>
#include <BALL/VIEW/PRIMITIV/mesh.h>
#include <BALL/VIEW/PRIMITIV/box.h>

namespace BALL
{
	namespace VIEW
	{

		Information::Information()
			throw()
			:	Visitor<GeometricObject>()
		{
			clear();
		}

		Information::~Information()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.info() << "Destructing object " << (void *)this 
									 << " of class " << RTTI::getName<Information>() << std::endl;
			#endif 
		}

		void Information::clear()
			throw()
		{
			name_ = "unkown";
			type_name_ = "unkown type";
			type_ = TYPE__UNKNOWN;
		}

		void Information::visit(GeometricObject& object)
		{
			getType_(object);
			getTypeName_();
			getName_(object);
		}

	  void Information::getType_(GeometricObject& object)
    {
			Information::Type type = TYPE__UNKNOWN;
			
			if (RTTI::isKindOf<Line>(object))
			{
				type = TYPE__LINE;
			}	
			else if (RTTI::isKindOf<Sphere>(object))
			{
				type = TYPE__SPHERE;
			}
			else if (RTTI::isKindOf<Tube>(object))
			{
				type = TYPE__TUBE;
			}
			else if (RTTI::isKindOf<Point>(object))
			{
				type = TYPE__POINT;
			}	
			else if (RTTI::isKindOf<Mesh>(object))
			{
				type = TYPE__MESH;
			}	
			else if (RTTI::isKindOf<Box>(object))
			{
				type = TYPE__BOX;
			}	

			type_ = type;
    }

	  void Information::getTypeName_()
    {
			switch(type_)
			{
			  case TYPE__LINE:
					type_name_ = "Line";
					break;

			  case TYPE__SPHERE:
					type_name_ = "Sphere";
					break;
					
			  case TYPE__TUBE:
					type_name_ = "Tube";
					break;
					
			  case TYPE__POINT:
					type_name_ = "Point";
					break;
					
			  case TYPE__MESH:
					type_name_ = "Mesh";
					break;
					
			  case TYPE__BOX:
					type_name_ = "Box";
					break;

  			default:
					type_name_ = "unknown GeometricObject";
					break;
			}
    }

	  void Information::getName_(GeometricObject& object)
    {
			String temp = object.getName();
			
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
