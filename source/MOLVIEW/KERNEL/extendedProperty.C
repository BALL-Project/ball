// $Id: extendedProperty.C,v 1.6 2001/05/13 15:02:41 hekl Exp $

#include <BALL/MOLVIEW/KERNEL/extendedProperty.h>


namespace BALL
{

	namespace MOLVIEW
	{

		ExtendedPropertyManager::ExtendedPropertyManager
			()
			throw()
				:
				PropertyManager()
		{
		}

		ExtendedPropertyManager::ExtendedPropertyManager
			(const ExtendedPropertyManager &__rExtendedPropertyManager,
			 bool deep)
			throw()
				:
				PropertyManager(__rExtendedPropertyManager, deep)
		{
		}

		ExtendedPropertyManager::~ExtendedPropertyManager()
			throw()
		{
		}

		void ExtendedPropertyManager::setProperty
			(int property)
			throw()
		{
			if (property == GeometricObject::PROPERTY__OBJECT_TRANSPARENT 
					|| property == GeometricObject::PROPERTY__OBJECT_OPAQUE)
			{
				clearProperty(GeometricObject::PROPERTY__OBJECT_TRANSPARENT);
				clearProperty(GeometricObject::PROPERTY__OBJECT_OPAQUE);
			}
			else if (property == GeometricObject::PROPERTY__OBJECT_VISIBLE 
							 || property == GeometricObject::PROPERTY__OBJECT_HIDDEN)
			{
				clearProperty(GeometricObject::PROPERTY__OBJECT_VISIBLE);
				clearProperty(GeometricObject::PROPERTY__OBJECT_HIDDEN);
			}
			else if (property == GeometricObject::PROPERTY__OBJECT_OPENED 
							 || property == GeometricObject::PROPERTY__OBJECT_CLOSED)
			{
				clearProperty(GeometricObject::PROPERTY__OBJECT_OPENED);
				clearProperty(GeometricObject::PROPERTY__OBJECT_CLOSED);
			}
			else if (property >= GeometricObject::PROPERTY__DRAWING_PRECISION_LOW
							 && property <= GeometricObject::PROPERTY__DRAWING_PRECISION_ULTRA)
			{
				clearProperty(GeometricObject::PROPERTY__DRAWING_PRECISION_LOW);
				clearProperty(GeometricObject::PROPERTY__DRAWING_PRECISION_MEDIUM);
				clearProperty(GeometricObject::PROPERTY__DRAWING_PRECISION_HIGH);
				clearProperty(GeometricObject::PROPERTY__DRAWING_PRECISION_ULTRA);
			}
			else if (property >= GeometricObject::PROPERTY__DRAWING_MODE_DOTS 
							 && property <= GeometricObject::PROPERTY__DRAWING_MODE_SOLID)
			{
				clearProperty(GeometricObject::PROPERTY__DRAWING_MODE_DOTS);
				clearProperty(GeometricObject::PROPERTY__DRAWING_MODE_WIREFRAME);
				clearProperty(GeometricObject::PROPERTY__DRAWING_MODE_SOLID);
			}
			else if (property >= PROPERTY__MODEL_VDW
							 && property <= PROPERTY__MODEL_LINES)
			{
				clearProperty(PROPERTY__MODEL_VDW);
				clearProperty(PROPERTY__MODEL_DOTS);
				clearProperty(PROPERTY__MODEL_BALL_AND_STICK);
				clearProperty(PROPERTY__MODEL_LINES);
			}

			PropertyManager::setProperty(property);
		}


#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/MOLVIEW/KERNEL/extendedProperty.iC>
#		endif

	} // namespace MOLVIEW

} // namespace BALL
