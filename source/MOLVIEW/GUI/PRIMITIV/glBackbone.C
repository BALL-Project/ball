// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: glBackbone.C,v 1.4.2.1 2003/01/07 13:21:32 anker Exp $

#include <BALL/MOLVIEW/GUI/PRIMITIV/glBackbone.h>

#include <qgl.h>

namespace BALL
{

	namespace MOLVIEW
	{

		GLBackbone::GLBackbone()
			throw()
			:	Backbone(),
				GLObject()
		{
		}

		GLBackbone::GLBackbone(const GLBackbone& GL_backbone, bool deep)
			throw()
			:	Backbone(GL_backbone, deep),
				GLObject(GL_backbone)
		{
		}

		GLBackbone::GLBackbone(const GeometricObject& geometric_object)
			throw()
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

		void GLBackbone::set(const GLBackbone& glbackbone, bool deep)
			throw()
		{
			Backbone::set(glbackbone, deep);
			GLObject::set(glbackbone);
		}

		const GLBackbone& GLBackbone::operator = (const GLBackbone& glbackbone)
			throw()
		{
			set(glbackbone);

			return *this;
		}

		void GLBackbone::get(GLBackbone& glbackbone, bool deep) const
			throw()
		{
			glbackbone.set(*this, deep);
		}

		void GLBackbone::swap(GLBackbone& glbackbone)
			throw()
		{
			Backbone::swap(glbackbone);
			GLObject::swap(glbackbone);
		}

		bool GLBackbone::draw(bool /* with_names */)
			throw()
		{
			return true;
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
		
	} // namespace MOLVIEW

} // namespace BALL
