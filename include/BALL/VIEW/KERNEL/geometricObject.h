// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: geometricObject.h,v 1.25 2003/02/25 13:28:23 sturm Exp $

#ifndef BALL_VIEW_KERNEL_GEOMETRICOBJECT_H
#define BALL_VIEW_KERNEL_GEOMETRICOBJECT_H

#ifndef BALL_CONCEPT_COMPOSITE_H
#	include <BALL/CONCEPT/composite.h>
#endif

#ifndef BALL_CONCEPT_PROPERTY_H
#	include <BALL/CONCEPT/property.h>
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
    /** \ingroup ViewKernelGeometricPrimitives
     *  @{
     */
		/** GeometricObject class.
				The class GeometricObject is the most important base class for a
				geometric primitive.  All geometric primitives must be derived from
				this class.  GeometricObject is derived from  \link Composite Composite \endlink .
				Therefore all geometric primitive can be appended to other
				composites. With the help of this class the graphical
				representation can be easily added to the information (stored as
				composites) which should be visualized. This class contains only
				the general information like selection color and properties a
				geometric object can have. All other methods concerning drawing are
				in the base class  \link GLObject GLObject \endlink .  The class geometricObject
				handles the different properties (see predefined properties), the
				selected color, the name and the typename of a geometric object.
				<b>Definition:</b> BALL/VIEW/KERNEL/geometricObject.h
		*/
		class GeometricObject
			: public Composite,
				public PropertyManager
		{
			public:

			/** @name Deep cloning facility.
			*/
			//@{
			BALL_CREATE_DEEP(GeometricObject)
			//@}

			/** @name Enumerations.
			*/
			//@{

			/** Anonymous enumeration.
					Unnamed enumeration of all non-categorized constants.
					Up to now there are 32 predefined properties.
					There are the following properties available:
			*/
			enum
			{
				/// number of the maximal used properties
				MAX_NUMBER_OF_PROPERTIES = 32,

				/// number of the minimal used property
				PROPERTY_MIN             = -MAX_NUMBER_OF_PROPERTIES + 1,

				/// number of the maximal used property
				PROPERTY_MAX             = MAX_NUMBER_OF_PROPERTIES - 1
			};

			/** Predefined properties.
					Enumeration of all properties that are used by the VIEW kernel.
					This properties define the look of a geometric object. There are three
					sub groups of the properties.
					Not all properties are necessary for geometric objects. Important properties
					are marked with a (*). These properties are a must that the geometric object
					is properly rendered.
			*/
			enum Property
			{
				// general properties
				/// the object is visible only in the static render mode (Default). 
				/// This property turns off the dynamic property. (*)
				PROPERTY__OBJECT_STATIC                    = 0,     // Default

				/// the object is visible only in the dynamic render mode. This property turns off the static property. (*)
				PROPERTY__OBJECT_DYNAMIC,                   

				/// the object is always before any other objects
				PROPERTY__OBJECT_ALWAYS_FRONT,               

				/// the object is transparent. This property turns off the opaque property. (*)
				PROPERTY__OBJECT_TRANSPARENT,               

				/// the object is opaque. This property turns off the transparent property. (*)
				PROPERTY__OBJECT_OPAQUE,                    

				/// the object is visible. This property turns off the hidden property. (*)
				PROPERTY__OBJECT_VISIBLE,                   

				/// the object is hidden. This property turns off the visible property. (*)
				PROPERTY__OBJECT_HIDDEN,                    

				/// the object has not a closed surface (eg. a tube without caps is open). 
				/// This property turns off the close property.
				PROPERTY__OBJECT_OPENED,                    

				/// the object has a closed surface (eg. a tube with caps is closed). This property turns off the opened property.
				PROPERTY__OBJECT_CLOSED,                    
				
				// DrawingMode Properties
				/// the object is drawn as a dots model. This property turns off the other drawing properties. (*)
				PROPERTY__DRAWING_MODE_DOTS                = 20,

				/// the object is drawn as a wireframe model. This property turns off the other drawing properties. (*)
				PROPERTY__DRAWING_MODE_WIREFRAME,

				/// the object is drawn as a solid model (Default). This property turns off the other drawing properties. (*)
				PROPERTY__DRAWING_MODE_SOLID,    // Default
				
				// DrawingPrecision Properties
				/// the object is drawn with low resolution. This property turns off the other resolution properties. (*)
				PROPERTY__DRAWING_PRECISION_LOW            = 30,

				/// the object is drawn with medium resolution. This property turns off the other resolution properties. (*)
				PROPERTY__DRAWING_PRECISION_MEDIUM,

				/// the object is drawn with high resolution (Default). This property turns off the other resolution properties. (*)
				PROPERTY__DRAWING_PRECISION_HIGH,  // Default

				NUMBER_OF_PROPERTIES
			};
		
			//@}
			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new geometricObject.
					The state of {\em *this} geometricObject is:
					\begin{itemize}
					  \item selected color is set to yellow (1.0, 1.0, 0.0, 1.0)
						\item name is set to "unknown"
						\item typename is set to "GeometricObject"
						\item properties are set to:
						\begin{itemize}
						  \item PROPERTY__OBJECT_STATIC
						  \item PROPERTY__OBJECT_OPAQUE
						  \item PROPERTY__OBJECT_VISIBLE
						  \item PROPERTY__OBJECT_CLOSED
						  \item PROPERTY__DRAWING_MODE_SOLID
						  \item PROPERTY__DRAWING_PRECISION_HIGH
						\end{itemize}
					\end{itemize}
					@return      GeometricObject new constructed geometricObject
					@see         ColorRGBA
					@see         Composite
					@see         PropertyManager
			*/
			GeometricObject()
				throw();
				
			/** Copy constructor.
					Construct new geometricObject by copying the geometricObject {\em object}.
					The copy is either deep (default) or shallow.
					The state of {\em *this} geometricObject is initialized to the state of the geometricObject {\em object}.
					@param       object the geometricObject to be copied (cloned)
					@param       deep make a deep (=<tt>true</tt>) or shallow (=<tt>false</tt>) copy of {\em object}
					@return      GeometricObject new constructed geometricObject cloned from {\em object}
					@see         ColorRGBA
					@see         Composite
					@see         PropertyManager
			*/
			GeometricObject(const GeometricObject& object, bool deep = true)
				throw();

			//@}
			/** @name Destructors */
			//@{

			/** Destructor.
					Default destruction of {\em *this} geometricObject.
					Calls  \link destroy destroy \endlink .
					@see         destroy
			*/
			virtual ~GeometricObject()
				throw();

			/** Explicit default initialization.
					Set the state of {\em *this} geometricObject to the default values.
					The state of {\em *this} geometricObject is:
					\begin{itemize}
					  \item selected color is set to yellow (1.0, 1.0, 0.0, 1.0)
						\item name is set to "unknown"
						\item properties are set to:
						\begin{itemize}
						  \item PROPERTY__OBJECT_STATIC
						  \item PROPERTY__OBJECT_OPAQUE
						  \item PROPERTY__OBJECT_VISIBLE
						  \item PROPERTY__OBJECT_CLOSED
						  \item PROPERTY__DRAWING_MODE_SOLID
						  \item PROPERTY__DRAWING_PRECISION_HIGH
						\end{itemize}
					\end{itemize}
					Calls  \link Composite::clear Composite::clear \endlink .
					Calls  \link PropertyManager::clear PropertyManager::clear \endlink .
			*/
			virtual void clear()
				throw();

			/** Explicit destructor.
					Destroy {\em *this} geometricObject.
					Calls  \link Composite::destroy Composite::destroy \endlink .
					Calls  \link PropertyManager::destroy PropertyManager::destroy \endlink .
					@see         Composite::destroy
					@see         PropertyManager::destroy
			*/
			virtual void destroy()
				throw();
							
			//@}
			/**	@name	Assignment methods
			*/
			//@{

			/** Assignment with cloning facility.
					Assign the geometricObject {\em object} to {\em *this} geometricObject.
					The assignment is either deep (default) or shallow.
					The state of {\em *this} geometricObject is initialized to the state of 
					the geometricObject {\em object}. \par
					@param       object the geometricObject to be copied (cloned)
					@param       deep make a deep (=<tt>true</tt>) or shallow (=<tt>false</tt>) copy of {\em object}
					@see         GeometricObject
			*/
			void set(const GeometricObject& object, bool deep = true)
				throw();

			/** Assignment operator.
					Assign the geometricObject {\em object} to {\em *this} geometricObject.
					The assignment is deep.
					Calls  \link set set \endlink .
					The state of {\em *this} geometricObject is initialized to the state 
					of the geometricObject {\em object}. \par
					@param       object the geometricObject to be copied (cloned)
					@return      GeometricObject& {\em *this} geometricObject
					@see         set
			*/
			GeometricObject& operator = (const GeometricObject& object)
				throw();

			/** Copying with cloning facility.
					Copy {\em *this} geometricObject to the geometricObject {\em object}.
					The assignment is either deep (default) or shallow.
					Calls  \link set set \endlink .
					The state of the geometricObject {\em object} is initialized to the
					state of {\em *this} geometricObject. \par
					@param       object the geometricObject to be assigned to
					@see         set
			*/
			void get(GeometricObject& object, bool deep = true) const
				throw();

			/** Swapping of geometricObjects.
					Swap the states of {\em *this} geometricObject with the geometricObject
					{\em object}.
					@param       object the geometricObject being swapped with {\em *this} geometricObject 
					@see         GeometricObject
			*/
			void swap(GeometricObject& object)
				throw();
							
			//@}
			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{

			/** Change the property of {\em *this} geometricObject to the property
					represented by the parameter {\em property}. 
					See predefined properties for allowed	properties.
					This method overrides the method {\em setProperty} from the class
					 \link PropertyManager PropertyManager \endlink . There are certain properties that cannot be
					set together (e.g. the model properties, the precision properties, etc.).
					If such a property is set all previously set properties of the same
					group are cleared (e.g. if the property DRAWING_PRECISION_LOW was set
					and the the property DRAWING_PRECISION_HIGH is set the former property
					will be cleared).
					@param       property the new property of {\em *this} geometricObject
			*/
			void setProperty(Property property)
				throw();

			/** Change the name of {\em *this} geometricObject.
					Change the name of {\em *this} geometricObject to the name
					represented by the parameter {\em name}.
					@param       name the new name of {\em *this} geometricObject
					@see         getName
					@see         hasName
			*/
			void setName(const String& name)
				throw();

			/** Mutable inspection of the name of {\em *this} geometricObject.
					Access the mutual reference of the name of {\em *this} geometricObject.
					@return      String& mutable reference to the name of {\em *this} geometricObject
					@see         setName
					@see         hasName
			*/
			String& getName()
				throw();

			/** Non-mutable inspection of the name of {\em *this} geometricObject.
					For further information see  \link getName getName \endlink .
			*/
			const String& getName() const
				throw();

			/** Test if {\em *this} geometricObject has a name.
					Test if the name of {\em *this} is not empty.
					@return  bool -	<tt>true</tt> if the name of {\em *this} is not empty, <tt>false</tt> otherwise
					@see     setName
					@see     getName
			*/
			bool hasName() const
				throw();

			/** Return the type name of {\em *this} geometricObject.
					Virtually overrideable method for specifying the type name of {\em *this} geometricObject.
					This method is used by the  \link Control Control \endlink  class to identify the geometricObjects.
					If the derived primitive geometric object should have a type name displayed by the
					control class, this method should be overridden.
					@return  String the type name of {\em *this} geometricObject
					@see     Control
			*/
			virtual String getTypeName() const
				throw();
							
			//@}
			/**	@name	debuggers and diagnostics
			*/
			//@{

			/** Internal state and consistency self-validation.
					Initiate self-validation of the internal state and data structure consistencies
					of {\em *this} geometricObject.
					If the internal state of {\em *this} geometricObject is correct (self-validated) and 
					consistent <tt>true</tt> is returned, <tt>false</tt> otherwise. 
					Calls {Composite::isValid}.
					Calls {PropertyManager::isValid}.
					@return			bool <tt>true</tt> if the internal state of {\em *this} geometricObject is correct
					@see        Composite::isValid
					@see        PropertyManager::isValid
			*/
			virtual bool isValid() const
				throw();

			/** Internal value dump.
					Dump the current state of {\em *this} geometricObject to 
					the output ostream {\em s} with dumping depth {\em depth}.
					@param   s output stream where to output the state of {\em *this} geometricObject
					@param   depth the dumping depth
					@see     ColorRGBA::dump
					@see     Composite::dump
					@see     PropertyManager::dump
			*/
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const
				throw();

			//@}

			protected:

			/*_ @name Internal protected members.
			*/
			//@{
			
			/*_ Export method.
					This method handles the export of {\em *this} geometricObject into another format (eg. POVRAY, VRML).\\
				  {\bf Note:} Not yet implemented.
					@return    bool {\tt true} if successful,	{\tt false} otherwise
			*/
			virtual bool extract()
				throw();

			/*_ Inspection of the drawing mode and drawing precision.
					Access the drawing mode and the drawing precision as integers. The properties
					will be converted into integers. See predefined properties for further
					information converning allowed properties and integer range.
					This method is provided for convience.
					@return   mode the drawing mode. Converted from property to int.
					@return   precision the drawing precision. Converted from property to int.
			*/
			void getDrawingModeAndPrecision(unsigned int& mode, unsigned int& precision) const
				throw();
			//@}

			private:

			void clear_()
				throw();
		
			/* name of the geometric object */
			String name_;
		};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/KERNEL/geometricObject.iC>
#		endif
  /** @} */
	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_KERNEL_GEOMETIRCOBJECT_H
