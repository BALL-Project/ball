// $Id: extendedProperty.h,v 1.4 2000/12/12 16:16:57 oliver Exp $

#ifndef BALL_MOLVIEW_KERNEL_EXTENDEDPROPERTY_H
#define BALL_MOLVIEW_KERNEL_EXTENDEDPROPERTY_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_CONCEPT_PROPERTY_H
#	include <BALL/CONCEPT/property.h>
#endif

#ifndef BALL_MOLVIEW_COMMON_COMMON_H
#	include <BALL/MOLVIEW/COMMON/common.h>
#endif

namespace BALL
{

	namespace MOLVIEW
	{

		/**
		*/
		class ExtendedPropertyManager
			: public PropertyManager
		{
			public:

			/**	@name	Constructors and Destructors
			*/
			//@{

			ExtendedPropertyManager();

			ExtendedPropertyManager
				(const ExtendedPropertyManager& extended_property_manager, bool deep = true);

			virtual ~ExtendedPropertyManager()
				throw();
			//@}

			/**	@name	Accessors
			*/
			//@{

			void setProperty(Property property);
			//@}
		};

#			ifndef BALL_NO_INLINE_FUNCTIONS
#				include <BALL/MOLVIEW/KERNEL/extendedProperty.iC>
#			endif


	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_KERNEL_EXTENDEDPROPERTY_H 
