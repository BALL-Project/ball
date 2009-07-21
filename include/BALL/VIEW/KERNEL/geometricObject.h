// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: geometricObject.h,v 1.39.16.2 2007-03-28 13:51:45 amoll Exp $
//

#ifndef BALL_VIEW_KERNEL_GEOMETRICOBJECT_H
#define BALL_VIEW_KERNEL_GEOMETRICOBJECT_H

#ifndef BALL_VIEW_DATATYPE_COLORRGBA_H
#	include <BALL/VIEW/DATATYPE/colorRGBA.h>
#endif

#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector3.h>
#endif

#ifdef BALL_DATATYPE_HASHSET_H
#	include <BALL/DATATYPE/hashSet.h>
#endif

namespace BALL
{
	class Composite;

	namespace VIEW
	{
		/** GeometricObject class.
				The class GeometricObject is the most important base class for a
				geometric primitive.  All geometric primitives must be derived from
				this class.  
				This class contains only the general information like 
				color, name and the composite pointer.
				\ingroup ViewKernelGeometricPrimitives
		*/
		class BALL_VIEW_EXPORT GeometricObject
		{
			public:

			/** @name cloning facility.
			*/
			//@{
			BALL_CREATE(GeometricObject)

			//@} /**	@name	Constructors and Destructors*/	//@{

			/** Default Constructor.
					The state of this geometricObject is:
					  - selected color is set to yellow (1.0, 1.0, 0.0, 1.0)
						- name is set to "unknown"
						- typename is set to "GeometricObject"
					\par
					\return      GeometricObject new constructed geometricObject
					\see         ColorRGBA
			*/
			GeometricObject();
				
			/** Copy constructor.
					\param       object the geometricObject to be copied (cloned)
			*/
			GeometricObject(const GeometricObject& object);

			/** Destructor.
			*/
			virtual ~GeometricObject();

			/** Explicit default initialization.
					Set the state of this geometricObject to the default values.
					The state of this geometricObject is:
					  - selected color is set to yellow (1.0, 1.0, 0.0, 1.0)
						- name is set to "unknown"
					\par
			*/
			virtual void clear();

			/**	@name	Assignment methods 
			 */ 
			//@{

			/** Assignment 
			*/
			void set(const GeometricObject& object);

			/** Assignment operator.
					Calls set.
			*/
			GeometricObject& operator = (const GeometricObject& object);

			/** Swapping of geometricObjects.
					\param       object the geometricObject being swapped with this geometricObject 
			*/
			void swap(GeometricObject& object);
							
			//@} 
			/**	@name	Accessors: inspectors and mutators 
			*/ 
			//@{

			///
			void setComposite(const Composite* composite)
				{ composite_ = composite;}

			///
			const Composite* getComposite() const
				{ return composite_;}
			
			//@} 
			/**	@name	debuggers and diagnostics */ 
			//@{

			/** Internal state and consistency self-validation.
					Initiate self-validation of the internal state and data structure consistencies
					of this geometricObject.
					If the internal state of this geometricObject is correct (self-validated) and 
					consistent <tt> true</tt> is returned, <tt> false</tt> otherwise. 
					\return			bool <tt> true</tt> if the internal state of this geometricObject is correct
			*/
			virtual bool isValid() const
				{return true;}

			/** Internal value dump.
					Dump the current state of this geometricObject to 
					the output ostream <b> s</b> with dumping depth <b> depth</b>.
					\param   s output stream where to output the state of this geometricObject
					\param   depth the dumping depth
					\see     ColorRGBA::dump
			*/
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const;

			/**
			 */
			void setColor(const ColorRGBA& color)
				{ color_ = color; }

			/**
			 */
			void setColor(const ColorUnit& red, const ColorUnit& green,
									  const ColorUnit& blue, const ColorUnit& alpha = (float)1)
				{ color_.set(red, green, blue, alpha);}

			/**
			*/
			const ColorRGBA& getColor() const
				{ return color_; }

			///
			ColorRGBA& getColor() 
				{ return color_; }

			/**
			*/
			void getColor(ColorUnit& red,  ColorUnit& green,
										ColorUnit& blue, ColorUnit& alpha) const
				{ color_.get(red, green, blue, alpha); }

			/// Insert the colors of this object into the hashset
			void getColors(HashSet<String>& colors);
			
			/// Needed for MSVC
			bool operator == (const GeometricObject& object) const
				{ return this == &object;}

		
			/// Needed for MSVC
			bool operator < (const GeometricObject& object) const
				{ return this < &object;}

			// Method to get all vertices from a geometric object
			virtual void getVertices(std::vector<Vector3>&) const {};

			//@}

			private:

			void clear_();
		
			ColorRGBA color_;

			// the composite, which is represented by this geometric object
			const Composite* composite_;

		};

		///
		typedef std::list<GeometricObject*> GeometricObjectList;

} } // namespaces
#endif // BALL_VIEW_KERNEL_GEOMETIRCOBJECT_H
