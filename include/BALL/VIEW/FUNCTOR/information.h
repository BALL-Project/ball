// $Id: information.h,v 1.6 2001/07/15 18:48:52 oliver Exp $

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
		
		/** Information class.
				{\bf Framework:} BALL/VIEW/FUNCTOR\\
				{\bf Definition:} \URL{BALL/VIEW/FUNCTOR/information.h}\\
				{\bf Concept:} visitor design pattern\\ \\
				The class Information is a base class for information objects.
				This class retrieves information concerning
				\Ref{Composite} objects. Override the \Ref{visit} method to
				extract the needed information from the given \Ref{Composite} object and
				write the result in the member variables \Ref{name_} and
				\Ref{type_name_}.
				In this implementation the types and type names of \Ref{Type} will be already
				recognized.
				This class is used by the class \Ref{Control} to retrieve informations
				about the shown \Ref{Composite} objects.
				@see     Control
				@memo    Information class (BALL VIEW functor framework)
				@author  $Author: oliver $
				@version $Revision: 1.6 $
				@date    $Date: 2001/07/15 18:48:52 $
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
				/// Unknown type. This value will be set in \Ref{clear}
				TYPE__UNKNOWN              = -1,

				/// Line type. This value will be set if the given \Ref{Composite} object is of kind \Ref{Line}
				TYPE__LINE                 = 0,

				/// Sphere type. This value will be set if the given \Ref{Composite} object is of kind \Ref{Sphere}
				TYPE__SPHERE               = 1,

				/// Tube type. This value will be set if the given \Ref{Composite} object is of kind \Ref{Tube}
				TYPE__TUBE                 = 2,

				/// Point type. This value will be set if the given \Ref{Composite} object is of kind \Ref{Point}
				TYPE__POINT                = 3,

				/// Mesh type. This value will be set if the given \Ref{Composite} object is of kind \Ref{Mesh}
				TYPE__MESH                 = 4,

				/// Simple box type. This value will be set if the given \Ref{Composite} object is of kind \Ref{SimpleBox}
				TYPE__SIMPLE_BOX           = 5,

				/// Geometric object type. This value will be set if the given \Ref{Composite} object is of kind \Ref{GeometricObject}
				TYPE__GEOMETRIC_OBJECT     = 20,

				/// Composite type. This value will be set if the given \Ref{Composite} object is non of the above
				TYPE__COMPOSITE            = 21
			};
			//@}

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Constructs new information.
					Calls \Ref{clear}.
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
					Calls \Ref{destroy}.
					@see         destroy
			*/
			virtual ~Information()
				throw();

			/** Explicit default initialization.
					Resets the state of {\em *this} information to:
					\begin{itemize}
					  \item  name_ is set to {\tt "unknown"}
					  \item  type_name_ is set to {\tt "unknown type"}
					  \item  type is set to {\tt TYPE__UNKNOWN}
					\end{itemize}
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
					If the given \Ref{Composite} is of any kind that is defined by the 
					enumeration \Ref{Type} this type will be returned.
					@return  Type the type of the \Ref{Composite}
					@see     Type
					@see     visit
			*/
			Type getType() const
				throw();

			/** Returns the type name of the given composite.
					Accesses the type name of the given \Ref{Composite} object.
					If the composite object is of any kind that is defined by the 
					enumeration \Ref{Type} this type will be returned as
					a string. If a \Ref{Composite} is of kind \Ref{GeometricObject}
					than the string defined by the \Ref{GeometricObject::getTypeName}
					method is returned.
					@return  String the type name of the given \Ref{Composite} object
					@see     visit
			*/
			String getTypeName() const
				throw();
			
			/** Returns the name of the given composite.
					Accesses the name of the given \Ref{Composite} object.
					If a \Ref{Composite} is of kind \Ref{GeometricObject}
					than the string defined by the \Ref{GeometricObject::getName}
					method is returned. If a \Ref{Composite} is not of kind 
					\Ref{GeometricObject} than the string {\tt "<Composite>"} is returned.
					@return  String the name of the given \Ref{Composite} object
					@see     visit
			*/
			String getName() const
				throw();		

			/** Visit method.
					Override this method for extracting the informations.
					Set the name and the type name in the protected member variables
					\Ref{name_} and \Ref{type_name_}.
					This method extracts already the name and the type name of a
					given \Ref{Composite} object as defined in the enumeration
					\Ref{Type}.\\
					{\bf Note:} If this method is overridden always call this method
					if the new
					implementation does not yield any information on the given 
					\Ref{Composite} object.
					@param  composite the \Ref{Composite} object whose information should be extracted
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

	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_FUNCTOR_INFORMATION_H
