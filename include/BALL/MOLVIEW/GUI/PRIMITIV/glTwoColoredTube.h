// $Id: glTwoColoredTube.h,v 1.3 2000/12/22 19:12:14 amoll Exp $

#ifndef BALL_MOLVIEW_GUI_PRIMITIV_GLTWOCOLOREDTUBE_H
#define BALL_MOLVIEW_GUI_PRIMITIV_GLTWOCOLOREDTUBE_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_VIEW_GUI_KERNEL_GLOBJECT_H
#	include <BALL/VIEW/GUI/KERNEL/glObject.h>
#endif

#ifndef BALL_MOLVIEW_PRIMITIV_TWOCOLOREDTUBE_H
#	include <BALL/MOLVIEW/PRIMITIV/twoColoredTube.h>
#endif


namespace BALL
{

	namespace MOLVIEW
	{
		
		using VIEW::GLObject;
		
		/**
		*/
		class GLTwoColoredTube
			: public TwoColoredTube,
				public GLObject
		{
			public:

			/**	@name Constructors and Destructors
			*/
			//@{

			GLTwoColoredTube();

			GLTwoColoredTube(const GLTwoColoredTube& GL_two_colored_tube, bool deep = true);

			GLTwoColoredTube(const VIEW::GeometricObject& geometric_object);

			virtual ~GLTwoColoredTube()
				throw();

			virtual void clear()
				throw();

			virtual void destroy()
				throw();
			//@}



			protected:

			virtual bool draw(bool with_names = false);

			virtual bool drawUserDefined();

			virtual bool extract()
				throw();

			
			private:

			void draw_
				(const Vector3& vertex_1, const Vector3& vertex_2,
				 const Vector3& rotation_axis, const Real angle);
		};

#			ifndef BALL_NO_INLINE_FUNCTIONS
#				include <BALL/MOLVIEW/GUI/PRIMITIV/glTwoColoredTube.iC>
#			endif

	} // namespace MOLVIEW

} // namespace BALL
			
#endif // BALL_MOLVIEW_GUI_PRIMITIV_GLTWOCOLOREDTUBE_H
