// $Id: extendedProperty.C,v 1.4 2000/06/18 16:35:19 hekl Exp $

#include <BALL/MOLVIEW/KERNEL/extendedProperty.h>


namespace BALL
{

	namespace MOLVIEW
	{

		ExtendedPropertyManager::ExtendedPropertyManager
			()
				:
				PropertyManager()
		{
		}

		ExtendedPropertyManager::ExtendedPropertyManager
			(const ExtendedPropertyManager &__rExtendedPropertyManager,
			 bool deep)
				:
				PropertyManager(__rExtendedPropertyManager, deep)
		{
		}

		ExtendedPropertyManager::~ExtendedPropertyManager
			()
		{
		}

		void 
		ExtendedPropertyManager::setProperty
			(Property __Property)
		{
			if (__Property == GeometricObject::PROPERTY__OBJECT_TRANSPARENT 
					|| __Property == GeometricObject::PROPERTY__OBJECT_OPAQUE)
			{
				clearProperty(GeometricObject::PROPERTY__OBJECT_TRANSPARENT);
				clearProperty(GeometricObject::PROPERTY__OBJECT_OPAQUE);
			}
			else if (__Property == GeometricObject::PROPERTY__OBJECT_VISIBLE 
							 || __Property == GeometricObject::PROPERTY__OBJECT_HIDDEN)
			{
				clearProperty(GeometricObject::PROPERTY__OBJECT_VISIBLE);
				clearProperty(GeometricObject::PROPERTY__OBJECT_HIDDEN);
			}
			else if (__Property == GeometricObject::PROPERTY__OBJECT_OPENED 
							 || __Property == GeometricObject::PROPERTY__OBJECT_CLOSED)
			{
				clearProperty(GeometricObject::PROPERTY__OBJECT_OPENED);
				clearProperty(GeometricObject::PROPERTY__OBJECT_CLOSED);
			}
			else if (__Property >= GeometricObject::PROPERTY__DRAWING_PRECISION_LOW
							 && __Property <= GeometricObject::PROPERTY__DRAWING_PRECISION_USER_DEFINED)
			{
				clearProperty(GeometricObject::PROPERTY__DRAWING_PRECISION_LOW);
				clearProperty(GeometricObject::PROPERTY__DRAWING_PRECISION_MEDIUM);
				clearProperty(GeometricObject::PROPERTY__DRAWING_PRECISION_HIGH);
				clearProperty(GeometricObject::PROPERTY__DRAWING_PRECISION_ULTRA);
				clearProperty(GeometricObject::PROPERTY__DRAWING_PRECISION_USER_DEFINED);
			}
			else if (__Property >= GeometricObject::PROPERTY__DRAWING_MODE_DOTS 
							 && __Property <= GeometricObject::PROPERTY__DRAWING_MODE_SOLID)
			{
				clearProperty(GeometricObject::PROPERTY__DRAWING_MODE_DOTS);
				clearProperty(GeometricObject::PROPERTY__DRAWING_MODE_WIREFRAME);
				clearProperty(GeometricObject::PROPERTY__DRAWING_MODE_SOLID);
			}
			else if (__Property >= GeometricObject::PROPERTY__MODEL_VDW
							 && __Property <= GeometricObject::PROPERTY__MODEL_LINES)
			{
				clearProperty(GeometricObject::PROPERTY__MODEL_VDW);
				clearProperty(GeometricObject::PROPERTY__MODEL_DOTS);
				clearProperty(GeometricObject::PROPERTY__MODEL_STARS);
				clearProperty(GeometricObject::PROPERTY__MODEL_BALL_AND_STICK);
				clearProperty(GeometricObject::PROPERTY__MODEL_LINES);
			}

			PropertyManager::setProperty(__Property);
		}


#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/MOLVIEW/KERNEL/extendedProperty.iC>
#		endif

	} // namespace MOLVIEW

} // namespace BALL
