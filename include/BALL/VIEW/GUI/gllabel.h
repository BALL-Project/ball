// $Id: gllabel.h,v 1.3 2000/09/23 14:15:05 hekl Exp $

#ifndef BALL_VIEW_GUI_PRIMITIV_GLLABEL_H
#define BALL_VIEW_GUI_PRIMITIV_GLLABEL_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_VIEW_GUI_KERNEL_GLOBJECT_H
#	include <BALL/VIEW/GUI/KERNEL/glObject.h>
#endif

#ifndef BALL_VIEW_PRIMITIV_LABEL_H
#	include <BALL/VIEW/PRIMITIV/label.h>
#endif

#include <qpainter.h>
#include <qfont.h>
#include <qbitmap.h>
#include <qimage.h>

namespace BALL
{
	
	namespace VIEW
	{

		/**
		*/
		class GLLabel
			: public Label,
				public GLObject
		{
			public:

			/**	@name	Constructors and Destructors
			*/	
			//@{

			GLLabel();

			GLLabel(const GLLabel& Label, bool deep = true);

			GLLabel(const GeometricObject& geometric_object);

			virtual ~GLLabel();

			virtual void clear();

			virtual void destroy();

			void setText(const String text);

			const QFont& getFont() const;

			void setFont(const QFont& font);
			//@}


			protected:

			virtual bool draw(bool with_names = false);

			virtual bool extract();

  		
		  private:

			void generateBitmap_();

			String getOldText_() const;

			const QFont& getOldFont_() const;

			QFont actual_font_;
			QFont old_font_;

			String old_text_;

			int width_;
			int height_;

			GLubyte* text_array_;
		};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/GUI/PRIMITIV/gllabel.iC>
#		endif
		
	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_GUI_PRIMITIV_GLLABEL_H
