// $Id: gllabel.C,v 1.5 2001/05/13 14:28:37 hekl Exp $

#include <BALL/VIEW/GUI/PRIMITIV/gllabel.h>
#include <GL/gl.h>

#include <qdialog.h>

using namespace std;

namespace BALL
{

	namespace VIEW
	{

		GLLabel::GLLabel()
			throw()
			:	Label(),
				GLObject(),
				actual_font_("helvetica"),
				old_font_(),
				width_(0),
				height_(0),
				text_array_(0)
		{
			setProperty(PROPERTY__DRAWING_MODE_WIREFRAME);
		}

		GLLabel::GLLabel(const GLLabel& label, bool deep)
			throw()
			:	Label(label, deep),
				GLObject(label),
				actual_font_(label.actual_font_),
				old_font_(label.old_font_),
				width_(label.width_),
				height_(label.height_)
		{
			if (text_array_ != 0)
			{
				delete [] text_array_;

				text_array_ = 0;
			}

			setProperty(PROPERTY__DRAWING_MODE_WIREFRAME);
		}

		GLLabel::GLLabel(const GeometricObject& geometric_object)
			throw()
			:	Label(geometric_object),
				GLObject(),
				actual_font_("helvetica"),
				old_font_(),
				width_(0),
				height_(0),
				text_array_(0)
		{
			setProperty(PROPERTY__DRAWING_MODE_WIREFRAME);
		}

		GLLabel::~GLLabel()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<GLLabel>() << endl;
			#endif 

			destroy();
		}

		void GLLabel::clear()
			throw()
		{
			Label::clear();
			GLObject::clear();

			actual_font_.setFamily("helvetica");

			width_ = 0;
			height_ = 0;

			if (text_array_ != 0)
			{
				delete [] text_array_;

				text_array_ = 0;
			}
		}

		void GLLabel::destroy()
			throw()
		{
			Label::destroy();
			GLObject::destroy();

			width_ = 0;
			height_ = 0;

			if (text_array_ != 0)
			{
				delete [] text_array_;

				text_array_ = 0;
			}
		}

		void GLLabel::set(const GLLabel& label, bool deep)
			throw()
		{
			Label::set(label, deep);
			GLObject::set(label);

			actual_font_ = label.actual_font_;
		}

		const GLLabel& GLLabel::operator = (const GLLabel& label)
			throw()
		{
			set(label);
			return *this;
		}

		void GLLabel::get(GLLabel& label, bool deep) const
			throw()
		{
			label.set(*this, deep);
		}

		void GLLabel::swap(GLLabel& label)
			throw()
		{
			Label::swap(label);
			GLObject::swap(label);

			QFont temp = actual_font_;
			actual_font_ = label.actual_font_;
			label.actual_font_ = actual_font_;
		}

		bool GLLabel::draw(bool with_names)
			throw()
		{
			if (hasProperty(GeometricObject::PROPERTY__OBJECT_HIDDEN) == true)
			{
				return true;
			}

			if (isSelected() == false)
			{
				glColor4ub((unsigned char)getColor().getRed(),
									 (unsigned char)getColor().getGreen(),
									 (unsigned char)getColor().getBlue(),
									 (unsigned char)getColor().getAlpha());
			}
			else
			{
				glColor4ub((unsigned char)getSelectedColor().getRed(),
									 (unsigned char)getSelectedColor().getGreen(),
									 (unsigned char)getSelectedColor().getBlue(),
									 (unsigned char)getSelectedColor().getAlpha());
			}

			if (with_names)
			{
				glLoadName(getGLPrimitiveManager()->getName(*this));
			}

			glPushMatrix();

			// build bitmap
			generateBitmap_();

			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

			glRasterPos3f((GLfloat)getVertex().x, 
										(GLfloat)getVertex().y, 
										(GLfloat)getVertex().z);

			// draw bitmap
			glBitmap(width_, height_, 0, height_/2.0, 0, 0, text_array_);

			glPopMatrix();

			return true;
		}

		bool GLLabel::extract()
			throw()
		{
			return Label::extract();
		}

		void GLLabel::generateBitmap_()
		{
			if (old_font_ != actual_font_
					|| getText() != old_text_
					|| text_array_ == 0)
			{
				QColor c2(0,0,0);
				QColor c1(255,255,255);
				int border = 2;
				
				QPainter p;
				QPixmap pm(1,1,1);
				p.begin(&pm);
				  p.setFont( actual_font_ );
				  QRect r = p.fontMetrics().boundingRect(getText().c_str());
				p.end();
				pm.resize(r.size() + QSize(border * 2, border * 2));
				
				p.begin(&pm);
				  pm.fill(c1);
				  p.setPen(c2);
				  p.drawText(-r.x() + border, -r.y() + border, getText().c_str());
				p.end();
						
				// convert to image (acces to single pixel is allowed here)
				QImage image = pm.convertToImage();
				
				width_ = image.width();
				int width = (width_ + 7) / 8;
				height_ = image.height();
				
				// convert to opengl usable bitmap
				int array_size = width * height_;
				
				if (text_array_ == 0)
				{
					text_array_ = new GLubyte[array_size]();
				}
				else
				{
					delete [] text_array_;
					text_array_ = new GLubyte[array_size]();
				}
				
				// clear char array
				for (int i = 0; i < array_size; ++i)
				{
					*(text_array_ + i) = 0;
				}
				
				// copy image to char array
				int offset = (height_-1)*width;
				
				for (int i = 0; i < height_; ++i, offset -= width)
				{
					for (int j = 0; j < width_; ++j)
					{
						if (image.pixel(j,i) != 0)
						{
							*(text_array_ + (j>>3) + offset) |= (128 >> (j&7));
						}
					}
				}

				// save old values 
				old_font_ = actual_font_;
				old_text_ = getText();
			}
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/GUI/PRIMITIV/gllabel.iC>
#		endif

	} // namespace VIEW

} // namespace BALL
