// $Id: glEntityDescriptor.h,v 1.3 2000/09/23 14:15:03 hekl Exp $

#ifndef BALL_VIEW_GUI_KERNEL_GLENTITYDESCRIPTOR_H
#define BALL_VIEW_GUI_KERNEL_GLENTITYDESCRIPTOR_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_VIEW_GUI_KERNEL_GLDISPLAYLIST_H
#	include <BALL/VIEW/GUI/KERNEL/glDisplayList.h>
#endif


namespace BALL
{
	
	namespace VIEW
	{

		class GLEntityDescriptor
		{
			public:

			BALL_CREATE_DEEP(GLEntityDescriptor)

			/**	@name	 Constructors and Destructors
			*/
			//@{

			GLEntityDescriptor();
			
			GLEntityDescriptor
				(const GLEntityDescriptor& descriptor, bool deep = true);

			virtual ~GLEntityDescriptor();

			virtual void clear();

			virtual void destroy();
			//@}

			/**	@name	Assignment
			*/
			//@{
			//@}

			/**	@name	Accessors
			*/
			//@{
			GLDisplayList* getStaticDisplayList();
			const GLDisplayList* getStaticDisplayList() const;

			GLDisplayList* getStaticAlwaysFrontDisplayList();
			const GLDisplayList* getStaticAlwaysFrontDisplayList() const;

			GLDisplayList* getStaticWireframeDisplayList();
			const GLDisplayList* getStaticWireframeDisplayList() const;

			GLDisplayList* getStaticWireframeAlwaysFrontDisplayList();
			const GLDisplayList* getStaticWireframeAlwaysFrontDisplayList() const;

			GLDisplayList* getDynamicDisplayList();
			const GLDisplayList* getDynamicDisplayList() const;

			GLDisplayList* getDynamicAlwaysFrontDisplayList();
			const GLDisplayList* getDynamicAlwaysFrontDisplayList() const;

			GLDisplayList* getTransparentDisplayList();
			const GLDisplayList* getTransparentDisplayList() const;

			GLDisplayList* getTransparentAlwaysFrontDisplayList();
			const GLDisplayList* getTransparentAlwaysFrontDisplayList() const;

			void update();

			void cancelUpdate();
 			//@}

			/**	@name	Predicates
			*/
			//@{
			bool needUpdate() const;
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
				

			private:
			
			GLDisplayList static_display_list_;
			GLDisplayList static_always_front_display_list_;
			GLDisplayList static_wireframe_display_list_;
			GLDisplayList static_wireframe_always_front_display_list_;
			GLDisplayList dynamic_display_list_;
			GLDisplayList dynamic_always_front_display_list_;
			GLDisplayList transparent_display_list_;
			GLDisplayList transparent_always_front_display_list_;

			bool update_;
		};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/GUI/KERNEL/glEntityDescriptor.iC>
#		endif 

	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_GUI_KERNEL_GLENTITYDESCRIPTOR_H
