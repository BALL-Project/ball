// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: information.h,v 1.12 2003/03/14 11:50:10 sturm Exp $

#ifndef BALL_VIEW_FUNCTOR_INFORMATION_H
#define BALL_VIEW_FUNCTOR_INFORMATION_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_CONCEPT_VISITOR_H
#	include <BALL/CONCEPT/visitor.h>
#endif

#ifndef BALL_CONCEPT_COMPOSITE_H
#	include <BALL/CONCEPT/composite.h>
#endif

#ifndef BALL_VIEW_KERNEL_GEOMETRICOBJECT_H
#	include <BALL/VIEW/KERNEL/geometricObject.h>
#endif

#ifndef BALL_VIEW_PRIMITIV_LINE_H
#	include <BALL/VIEW/PRIMITIV/line.h>
#endif

#ifndef BALL_VIEW_PRIMITIV_SPHERE_H
#	include <BALL/VIEW/PRIMITIV/sphere.h>
#endif

#ifndef BALL_VIEW_PRIMITIV_TUBE_H
#	include <BALL/VIEW/PRIMITIV/tube.h>
#endif

#ifndef BALL_VIEW_PRIMITIV_POINT_H
#	include <BALL/VIEW/PRIMITIV/point.h>
#endif

#ifndef BALL_VIEW_PRIMITIV_MESH_H
#	include <BALL/VIEW/PRIMITIV/mesh.h>
#endif

#ifndef BALL_VIEW_PRIMITIV_SIMPLEBOX_H
#	include <BALL/VIEW/PRIMITIV/simpleBox.h>
#endif


  
namespace BALL
{

	namespace VIEW
	{
    /**  \addtogroup  ViewFunctors
     *  @{
     */		
		/** Information class.
				The class Information is a base class for information objects.
				This class retrieves information concerning
				 \link Composite Composite \endlink  objects. Override the  \link visit visit \endlink  method to
				extract the needed information from the given  \link Composite Composite \endlink  object and
				write the result in the member variables  \link name_ name_ \endlink  and
				 \link type_name_ type_name_ \endlink .
				In this implementation the types and type names of  \link Type Type \endlink  will be already
				recognized.
				This class is used by the class  \link Control Control \endlink  to retrieve informations
				about the shown  \link Composite Composite \endlink  objects.  \par
				
				@see     Control
		*/
		class Information: public Visitor<Composite>
		{
			public:
			
			/**	@name	Enums
			 */
			//@{
			/** Type.
					This types are possible.
			*/
			enum Type
			{
				/// Unknown type. This value will be set in  \link clear clear \endlink 
				TYPE__UNKNOWN              = -1,

				/// Line type. This value will be set if the given  \link Composite Composite \endlink  object is of kind  \link Line Line \endlink 
				TYPE__LINE                 = 0,

				/// Sphere type. This value will be set if the given  \link Composite Composite \endlink  object is of kind  \link Sphere Sphere \endlink 
				TYPE__SPHERE               = 1,

				/// Tube type. This value will be set if the given  \link Composite Composite \endlink  object is of kind  \link Tube Tube \endlink 
				TYPE__TUBE                 = 2,

				/// Point type. This value will be set if the given  \link Composite Composite \endlink  object is of kind  \link Point Point \endlink 
				TYPE__POINT                = 3,

				/// Mesh type. This value will be set if the given  \link Composite Composite \endlink  object is of kind  \link Mesh Mesh \endlink 
				TYPE__MESH                 = 4,

				/// Simple box type. This value will be set if the given  \link Composite Composite \endlink  object is of kind  \link SimpleBox SimpleBox \endlink 
				TYPE__SIMPLE_BOX           = 5,

				/// Geometric object type. This value will be set if the given  \link Composite Composite \endlink  object is of kind  \link GeometricObject GeometricObject \endlink 
				TYPE__GEOMETRIC_OBJECT     = 20,

				/// Composite type. This value will be set if the given  \link Composite Composite \endlink  object is non of the above
				TYPE__COMPOSITE            = 21
			};
			//@}

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Constructs new information.
					Calls  \link clear clear \endlink .
					@return      Information new constructed information
					@see         clear
			*/
			Information()
				throw();

			//@}

			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} information.
					Calls  \link destroy destroy \endlink .
					@see         destroy
			*/
			virtual ~Information()
				throw();

			/** Explicit default initialization.
					Resets the state of {\em *this} information to:

					  -  name_ is set to <tt>"unknown"</tt>
					  -  type_name_ is set to <tt>"unknown type"</tt>
					  -  type is set to <tt>TYPE__UNKNOWN</tt>
					
			*/
			virtual void clear()
				throw();

			/** Explicit destructor.
					Empty for further purpose.
					@see  ~Information
			*/
			virtual void destroy()
				throw();
			//@}

			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{

			/** Returns the type of the given composite.
					If the given  \link Composite Composite \endlink  is of any kind that is defined by the 
					enumeration  \link Type Type \endlink  this type will be returned.
					@return  Type the type of the  \link Composite Composite \endlink 
					@see     Type
					@see     visit
			*/
			Type getType() const
				throw();

			/** Returns the type name of the given composite.
					Accesses the type name of the given  \link Composite Composite \endlink  object.
					If the composite object is of any kind that is defined by the 
					enumeration  \link Type Type \endlink  this type will be returned as
					a string. If a  \link Composite Composite \endlink  is of kind  \link GeometricObject GeometricObject \endlink 
					than the string defined by the  \link GeometricObject::getTypeName GeometricObject::getTypeName \endlink 
					method is returned.
					@return  String the type name of the given  \link Composite Composite \endlink  object
					@see     visit
			*/
			String getTypeName() const
				throw();
			
			/** Returns the name of the given composite.
					Accesses the name of the given  \link Composite Composite \endlink  object.
					If a  \link Composite Composite \endlink  is of kind  \link GeometricObject GeometricObject \endlink 
					than the string defined by the  \link GeometricObject::getName GeometricObject::getName \endlink 
					method is returned. If a  \link Composite Composite \endlink  is not of kind 
					 \link GeometricObject GeometricObject \endlink  than the string <tt>"<Composite>"</tt> is returned.
					@return  String the name of the given  \link Composite Composite \endlink  object
					@see     visit
			*/
			String getName() const
				throw();		

			/** Visit method.
					Override this method for extracting the informations.
					Set the name and the type name in the protected member variables
					 \link name_ name_ \endlink  and  \link type_name_ type_name_ \endlink .
					This method extracts already the name and the type name of a
					given  \link Composite Composite \endlink  object as defined in the enumeration
					 \link Type Type \endlink . \par
					<b>Note:</b> If this method is overridden always call this method
					if the new
					implementation does not yield any information on the given 
					 \link Composite Composite \endlink  object.
					@param  composite the  \link Composite Composite \endlink  object whose information should be extracted
					@see     getType
					@see     getTypeName
					@see     getName
			*/
			virtual void visit(Composite& composite);
			//@}
			
			protected:
			
			/** @name Protected members
			*/
			//@{
			/** Extracted name of the composite.
					In this member the extracted name of the composite is stored.
			*/
			String name_;

			/** Extracted type name of the composite.
					In this member the extracted type name of the composite is stored.
			*/
			String type_name_;
			//@}

  		private:

			void getType_(Composite& composite);
			void getTypeName_(Composite& composite);
			void getName_(Composite& composite);

			Type type_;
		};

			
#			ifndef BALL_NO_INLINE_FUNCTIONS
#				include <BALL/VIEW/FUNCTOR/information.iC>
#			endif
  /** @} */
	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_FUNCTOR_INFORMATION_H
