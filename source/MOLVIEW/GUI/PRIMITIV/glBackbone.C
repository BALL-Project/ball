// $Id: glBackbone.C,v 1.3 2001/02/04 16:19:04 hekl Exp $

#include <BALL/MOLVIEW/GUI/PRIMITIV/glBackbone.h>
#include <GL/gl.h>


namespace BALL
{

	namespace MOLVIEW
	{

		GLBackbone::GLBackbone()
			:	Backbone(),
				GLObject()
		{
		}

		GLBackbone::GLBackbone(const GLBackbone& GL_backbone, bool deep)
			:	Backbone(GL_backbone, deep),
				GLObject(GL_backbone)
		{
		}

		GLBackbone::GLBackbone(const GeometricObject& geometric_object)
			:	Backbone(geometric_object),
				GLObject()
		{
		}

		GLBackbone::~GLBackbone()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<GLBackbone>() << endl;
			#endif 

			destroy();
		}

		void GLBackbone::clear()
			throw()
		{
			Backbone::clear();
			GLObject::clear();
		}

		void GLBackbone::destroy()
			throw()
		{
			Backbone::destroy();
			GLObject::destroy();
		}

		bool GLBackbone::draw(bool /* with_names */)
		{
			return true;
		}

		bool GLBackbone::drawUserDefined()
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		bool GLBackbone::extract()
			throw()
		{
			return Backbone::extract();
		}

		Sphere* GLBackbone::createSphere_()
		{
			return new GLSphere();
		}

		Tube* GLBackbone::createTube_()
		{
			return new GLTube();
		}

		Line* GLBackbone::createLine_()
		{
			return new GLLine();
		}
		
#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/MOLVIEW/GUI/PRIMITIV/glBackbone.iC>
#		endif

	} // namespace MOLVIEW

} // namespace BALL
