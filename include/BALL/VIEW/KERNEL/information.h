// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: information.h,v 1.5 2004/02/18 23:24:06 oliver Exp $
//

#ifndef BALL_VIEW_KERNEL_INFORMATION_H
#define BALL_VIEW_KERNEL_INFORMATION_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_CONCEPT_VISITOR_H
#	include <BALL/CONCEPT/visitor.h>
#endif

#ifndef BALL_VIEW_KERNEL_GEOMETRICOBJECT_H
#	include <BALL/VIEW/KERNEL/geometricObject.h>
#endif

namespace BALL
{
	namespace VIEW
	{
		
		/** This class retrieves information concerning GeometricObject objects. 
				The result is written in the member variables name_ and type_name_.
				\ingroup ViewKernelGeometricPrimitives
		*/
		class Information: public Visitor<GeometricObject>
		{
			public:
			
			/**	@name	Enums
			 */
			//@{
		
			/** Type.
			*/
			enum Type
			{
				/// Unknown type. This value will be set in clear
				TYPE__UNKNOWN              = -1,

				/// Line type. 
				TYPE__LINE                 = 0,

				/// Sphere type. 
				TYPE__SPHERE               = 1,

				/// Tube type. 
				TYPE__TUBE                 = 2,

				/// Point type.
				TYPE__POINT                = 3,

				/// Mesh type.
				TYPE__MESH                 = 4,

				/// Box type.
				TYPE__BOX           			 = 5
			};
			//@}

			/**	@name	Constructors and Destructor
			*/	
			//@{

			/** Default Constructor.
					Calls clear.
			*/
			Information()
				throw();


			/** Destructor.
			*/
			virtual ~Information()
				throw();

			/** Explicit default initialization.
					Resets the state of this information to:
					  -  name_ is set to <tt> "unknown"</tt>
					  -  type_name_ is set to <tt> "unknown type"</tt>
					  -  type is set to <tt> TYPE__UNKNOWN</tt>
					\par
			*/
			virtual void clear()
				throw();

			//@} 
			/**	@name	Accessors: inspectors and mutators */ 
			//@{

			/** Returns the type of the given GeometricObject.
					If the given GeometricObject is of any kind that is defined by the 
					enumeration Type this type will be returned.
					\return  Type the type of the GeometricObject
					\see     Type
					\see     visit
			*/
			Type getType() const
				throw();

			/** Accesses the type name of the given GeometricObject object.
					If the GeometricObject object is of any kind that is defined by the 
					enumeration Type this type will be returned as
					a string.
					\return  String the type name of the given GeometricObject
					\see     visit
			*/
			String getTypeName() const
				throw();
			
			/** Accesses the name of the given GeometricObject object.
					\return  String the name of the given GeometricObject 
					\see     visit
			*/
			String getName() const
				throw();		

			/** Visit method.
					Override this method for extracting the informations.
					Set the name and the type name in the protected member variables name_ and type_name_.
					This method extracts already the name and the type name of a
					given GeometricObject object as defined in the enumeration
					Type.\par
					<b>Note:</b> If this method is overridden always call this method if the new
					implementation does not yield any information on the given 
					GeometricObject.
					\param  object the GeometricObject whose information should be extracted
					\see     getType
					\see     getTypeName
					\see     getName
			*/
			virtual void visit(GeometricObject& object);
			
			protected:
			
			//@}
			/** @name Protected members
			*/
			//@{
		
			/** Extracted name of the GeometricObject.
			*/
			String name_;

			/** Extracted type name of the GeometricObject.
			*/
			String type_name_;
			//@}

  		private:

			void getType_(GeometricObject& object);
			void getTypeName_();
			void getName_(GeometricObject& object);

			Type type_;
		};

#	ifndef BALL_NO_INLINE_FUNCTIONS
#	 include <BALL/VIEW/KERNEL/information.iC>
#	endif

	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_KERNEL_INFORMATION_H
