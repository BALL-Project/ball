// $Id: geometricObject.h,v 1.1 1999/08/26 07:53:14 oliver Exp $

#ifndef BALL_VIEW_KERNEL_GEOMETRICOBJECT_H
#define BALL_VIEW_KERNEL_GEOMETRICOBJECT_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_CONCEPT_COMPOSITE_H
#	include <BALL/CONCEPT/composite.h>
#endif

#ifndef BALL_CONCEPT_PROPERTY_H
#	include <BALL/CONCEPT/property.h>
#endif

#ifndef BALL_CONCEPT_SELECTABLE_H
#	include <BALL/CONCEPT/selectable.h>
#endif

#ifndef BALL_DATATYPE_STRING_H
#	include <BALL/DATATYPE/string.h>
#endif

#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector3.h>
#endif

#ifndef BALL_VIEW_DATATYPE_COLOR_H
#	include <BALL/VIEW/DATATYPE/color.h>
#endif


#define BALL_VIEW_PRINT_PROPERTY(property) \
	(hasProperty(property) ? "yes" : "no")

namespace BALL
{

	namespace VIEW
	{

		/* forward */
		class GLObject;
		class Scene;

		/**
		*/
		class GeometricObject
			: public Composite,
				public PropertyManager,
				public Selectable
		{
			friend class GLObject;
			friend class Scene;

			public:
			
			/**	@name	Enums
			*/
			//@{
			enum Property
			{
				// Object Properties
				PROPERTY__OBJECT_STATIC                    = 0,     // Default
				PROPERTY__OBJECT_DYNAMIC                   = 1,
				PROPERTY__OBJECT_TRANSPARENT               = 2,
				PROPERTY__OBJECT_OPAQUE                    = 3,
				PROPERTY__OBJECT_VISIBLE                   = 4,
				PROPERTY__OBJECT_HIDDEN                    = 5,
				PROPERTY__OBJECT_OPENED                    = 6,
				PROPERTY__OBJECT_CLOSED                    = 7,
				
				// DrawingMode Properties
				PROPERTY__DRAWING_MODE_DOTS                = 8,
				PROPERTY__DRAWING_MODE_WIREFRAME           = 9,
				PROPERTY__DRAWING_MODE_SOLID               = 10, // Default
				
				// DrawingPrecision Properties
				PROPERTY__DRAWING_PRECISION_LOW            = 11,
				PROPERTY__DRAWING_PRECISION_MEDIUM         = 12,
				PROPERTY__DRAWING_PRECISION_HIGH           = 13,  // Default
				PROPERTY__DRAWING_PRECISION_ULTRA          = 14,
				PROPERTY__DRAWING_PRECISION_USER_DEFINED   = 15,
				PROPERTY__DRAWING_PRECISION_FIRST_VALUE    = 16,
				PROPERTY__DRAWING_PRECISION_SECOND_VALUE   = 17,
				PROPERTY__DRAWING_PRECISION_THIRD_VALUE    = 18,
				
				NUMBER_OF_PROPERTIES
			};
		
			enum
			{
				MAX_NUMBER_OF_PROPERTIES = 32,
				PROPERTY_MIN             = -MAX_NUMBER_OF_PROPERTIES + 1,
				PROPERTY_MAX             = MAX_NUMBER_OF_PROPERTIES - 1
			};

			//@}

			/**	@name	Constructors and Destructors
			*/	
			//@{

			GeometricObject();
				
			GeometricObject(const GeometricObject& object, bool deep = true);

			virtual ~GeometricObject();

			virtual void clear();

			virtual void destroy();
			//@}

			/**	@name	Assignment
			*/
			//@{

			void set
				(const GeometricObject& object, bool deep = true);

			GeometricObject& operator = (const GeometricObject& object);

			void get(GeometricObject& object, bool deep = true) const;

			void swap(GeometricObject& object);
			//@}

			/**	@name	Accessors
			*/
			//@{

			void setProperty(Property property);

			void setSelectedColor(const ColorRGBA& color);
			
			void setSelectedColor
				(const ColorUnit& red, const ColorUnit& green,
				 const ColorUnit& blue, const ColorUnit& alpha = 255);
			
			const ColorRGBA& getSelectedColor() const;

			void getSelectedColor(ColorRGBA& color) const;

			void getSelectedColor
				(ColorUnit& red, ColorUnit& green,
				 ColorUnit& blue, ColorUnit& alpha) const;

			void getSelectedColor
				(ColorUnit& red, ColorUnit& green, ColorUnit& blue) const;

		
			void setName(const String& name);

			String& getName();

			const String& getName() const;
			//@}
		
			/**	@name	Debugging and Diagnostics
			*/
			//@{
			virtual bool isValid() const;

			virtual void dump
				(ostream& s = cout, unsigned long depth = 0) const;
			//@}

			/**	@name	Storers
			*/
			//@{

			virtual void read(istream& s);

			virtual void write(ostream& s) const;
			//@}

			
			protected:

			virtual bool export();

			void getDrawingModeAndPrecision
				(unsigned int& mode, unsigned int& precision) const;


			private:

			void clear_();
		
			/* color of the selected object */
			ColorRGBA selected_color_;

			/* name of the geometric object */
			String name_;
		};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/KERNEL/geometricObject.iC>
#		endif
		
	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_KERNEL_GEOMETIRCOBJECT_H
