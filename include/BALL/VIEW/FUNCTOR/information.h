// $Id: information.h,v 1.3 2000/12/12 16:15:40 oliver Exp $

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
		
		/**
		*/
		class Information
			: public Visitor<Composite>
		{
			public:
			
			/**	@name	Enums
			 */
			//@{
			enum Type
			{
				TYPE__UNKNOWN              = -1,
				TYPE__LINE                 = 0,
				TYPE__SPHERE               = 1,
				TYPE__TUBE                 = 2,
				TYPE__POINT                = 3,
				TYPE__MESH                 = 4,
				TYPE__SIMPLE_BOX           = 5,
				TYPE__GEOMETRIC_OBJECT     = 20,
				TYPE__COMPOSITE            = 21
			};
			//@}

			/**	@name	Type definitions
			*/
			//@{
				
			//@}

			/**	@name	Constructors and Destructors
			*/
			//@{

			Information();

			Information
				(const Information& info, bool deep = true);

			virtual ~Information()
				throw();

			virtual void clear()
				throw();

			virtual void destroy()
				throw();
			//@}

			/**	@name	Asignment
			*/
			//@{

			void set
				(const Information& info, bool deep = true);

			Information&  operator =
				(const Information& info);

			void get
				(Information&  info, bool deep = true) const;

			void swap
				(Information&  info);
			//@}

			
			/**	@name	Accessors
			*/
			//@{

			Type getType() const;

			String getTypeName() const;
			
			String getName() const;		

			virtual void visit(Composite& composite);
			//@}
			

			/**	@name	Debugging and Diagnostics
			*/
			//@{

			virtual bool isValid() const;

			virtual void dump
				(std::ostream& s = std::cout, Size depth = 0) const
				throw();
			//@}

			protected:
			
			String name_;
			String type_name_;
			
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
