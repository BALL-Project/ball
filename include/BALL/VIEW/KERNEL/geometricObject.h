// $Id: geometricObject.h,v 1.8 2000/08/30 19:58:27 oliver Exp $

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
				public PropertyManager
		{
			friend class GLObject;
			friend class Scene;

			public:
			
			BALL_CREATE_DEEP(GeometricObject)
			
			/**	@name	Enums
			*/
			//@{
			enum Property
			{
				// Object Properties
				PROPERTY__OBJECT_STATIC                    = 0,     // Default
				PROPERTY__OBJECT_DYNAMIC,                   
				PROPERTY__OBJECT_ALWAYS_FRONT,               
				PROPERTY__OBJECT_TRANSPARENT,               
				PROPERTY__OBJECT_OPAQUE,                    
				PROPERTY__OBJECT_VISIBLE,                   
				PROPERTY__OBJECT_HIDDEN,                    
				PROPERTY__OBJECT_OPENED,                    
				PROPERTY__OBJECT_CLOSED,                    
				
				// DrawingMode Properties
				PROPERTY__DRAWING_MODE_DOTS                = 20,
				PROPERTY__DRAWING_MODE_WIREFRAME,
				PROPERTY__DRAWING_MODE_SOLID,    // Default
				
				// DrawingPrecision Properties
				PROPERTY__DRAWING_PRECISION_LOW            = 30,
				PROPERTY__DRAWING_PRECISION_MEDIUM,
				PROPERTY__DRAWING_PRECISION_HIGH,  // Default
				PROPERTY__DRAWING_PRECISION_ULTRA,
				PROPERTY__DRAWING_PRECISION_USER_DEFINED,
				PROPERTY__DRAWING_PRECISION_FIRST_VALUE,
				PROPERTY__DRAWING_PRECISION_SECOND_VALUE,
				PROPERTY__DRAWING_PRECISION_THIRD_VALUE,
				
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

			bool hasName() const;

			virtual String getTypeName() const;
			//@}
		
			/**	@name	Debugging and Diagnostics
			*/
			//@{
			virtual bool isValid() const;

			virtual void dump
				(std::ostream& s = std::cout, Size depth = 0) const;
			//@}

			/**	@name	Storers
			*/
			//@{

			virtual void read(std::istream& s);

			virtual void write(std::ostream& s) const;
			//@}

			
			protected:

			virtual bool extract();

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
