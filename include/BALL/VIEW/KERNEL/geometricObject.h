// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: geometricObject.h,v 1.29 2003/08/26 08:05:07 oliver Exp $
//

#ifndef BALL_VIEW_KERNEL_GEOMETRICOBJECT_H
#define BALL_VIEW_KERNEL_GEOMETRICOBJECT_H

#ifndef BALL_VIEW_DATATYPE_COLORRGBA_H
#	include <BALL/VIEW/DATATYPE/colorRGBA.h>
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
		*/
		class GeometricObject
		{
			public:

			/** @name Deep cloning facility.
			*/
			//@{
			BALL_CREATE_DEEP(GeometricObject)

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
			GeometricObject()
				throw();
				
			/** Copy constructor.
					The copy is either deep (default) or shallow.
					\param       object the geometricObject to be copied (cloned)
					\param       deep make a deep (=<tt> true</b>) or shallow (=<tt> false</b>) copy of <b> object</b>
					\return      GeometricObject new constructed geometricObject cloned from <b> object</b>
			*/
			GeometricObject(const GeometricObject& object, bool deep = true)
				throw();

			/** Destructor.
			*/
			virtual ~GeometricObject()
				throw();

			/** Explicit default initialization.
					Set the state of this geometricObject to the default values.
					The state of this geometricObject is:
					  - selected color is set to yellow (1.0, 1.0, 0.0, 1.0)
						- name is set to "unknown"
					\par
			*/
			virtual void clear()
				throw();

			//@} 

			/**	@name	Assignment methods */ 
			//@{

			/** Assignment with cloning facility.
					The assignment is either deep (default) or shallow.
					\param       deep make a deep (=<tt> true</b>) or shallow (=<tt> false</b>) copy of <b> object</b>
			*/
			void set(const GeometricObject& object, bool deep = true)
				throw();

			/** Assignment operator.
					Calls set.
			*/
			GeometricObject& operator = (const GeometricObject& object)
				throw();

			/** Copying with cloning facility.
					Calls set.
					The state of the geometricObject <b> object</b> is initialized to the
					state of this geometricObject.\par
					\param       object the geometricObject to be assigned to
					\see         set
			*/
			void get(GeometricObject& object, bool deep = true) const
				throw();

			/** Swapping of geometricObjects.
					\param       object the geometricObject being swapped with this geometricObject 
			*/
			void swap(GeometricObject& object)
				throw();
							
			//@} /**	@name	Accessors: inspectors and mutators */ //@{

			///
			void setComposite(const Composite* composite)
				throw() { composite_ = composite;}

			///
			const Composite* getComposite() const
				throw() { return composite_;}
			
			/** Change the name of this geometricObject.
					\param       name the new name of this geometricObject
					\see         getName
					\see         hasName
			*/
			void setName(const String& name)
				throw() {name_ = name;}

			/** Mutable inspection of the name of this geometricObject.
					\return      String& mutable reference to the name of this geometricObject
					\see         setName
					\see         hasName
			*/
			String& getName()
				throw() { return name_;}

			/** Non-mutable inspection of the name of this geometricObject.
			*/
			const String& getName() const
				throw() { return name_;}

			/** Test if this geometricObject has a name.
					\return  bool -	<tt> true</tt> if the name of this is not empty, <tt> false</tt> otherwise
					\see     setName
					\see     getName
			*/
			bool hasName() const
				throw() {return !name_.isEmpty();}

			/** Return the type name of this geometricObject.
					Virtually overrideable method for specifying the type name of this geometricObject.
					This method is used by the Control class to identify the geometricObjects.
					If the derived primitive geometric object should have a type name displayed by the
					control class, this method should be overridden.
					\return  String the type name of this geometricObject
					\see     Control
			*/
			virtual String getTypeName() const
				throw() {return "GeometricObject";}
							
			//@} /**	@name	debuggers and diagnostics */ //@{

			/** Internal state and consistency self-validation.
					Initiate self-validation of the internal state and data structure consistencies
					of this geometricObject.
					If the internal state of this geometricObject is correct (self-validated) and 
					consistent <tt> true</tt> is returned, <tt> false</tt> otherwise. 
					\return			bool <tt> true</tt> if the internal state of this geometricObject is correct
			*/
			virtual bool isValid() const
				throw() {return true;}

			/** Internal value dump.
					Dump the current state of this geometricObject to 
					the output ostream <b> s</b> with dumping depth <b> depth</b>.
					\param   s output stream where to output the state of this geometricObject
					\param   depth the dumping depth
					\see     ColorRGBA::dump
			*/
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const
				throw();

			/**
			 */
			void setColor(const ColorRGBA& color)
				throw() { color_ = color; }

			/**
			 */
			void setColor(const ColorUnit& red, const ColorUnit& green,
									  const ColorUnit& blue, const ColorUnit& alpha = (float)1)
				throw() { color_.set(red, green, blue, alpha);}

			/**
			*/
			const ColorRGBA& getColor() const
				throw() { return color_; }

			/**
			*/
			void getColor(ColorUnit& red,  ColorUnit& green,
										ColorUnit& blue, ColorUnit& alpha) const
				throw() { color_.get(red, green, blue, alpha); }

			//@}

			private:

			void clear_()
				throw();
		
			// name of the geometric object
			String name_;

			ColorRGBA color_;

			// the composite, which is represented by this geometric object
			const Composite* composite_;
		};


	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_KERNEL_GEOMETIRCOBJECT_H
