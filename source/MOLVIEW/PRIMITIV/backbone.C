// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: backbone.C,v 1.10 2003/06/06 10:41:00 amoll Exp $

#include <BALL/MOLVIEW/PRIMITIV/backbone.h>

using namespace std;

namespace BALL
{

	namespace MOLVIEW
	{

		Backbone::Backbone()
			throw()
			:	GeometricObject()
		{
		}

		Backbone::Backbone(const Backbone& backbone, bool deep)
			throw()
			:	GeometricObject(backbone, deep)
		{
		}

		Backbone::Backbone(const GeometricObject& geometric_object)
			throw()
			:	GeometricObject(geometric_object)
		{
		}

		Backbone::~Backbone()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
						 << " of class " << RTTI::getName<Backbone>() << endl;
			#endif 

			destroy();
		}

		void Backbone::clear()
			throw()
		{
			GeometricObject::clear();
		}

		void Backbone::destroy()
			throw()
		{ 
			GeometricObject::destroy();
		}

		void Backbone::set(const Backbone& backbone, bool deep)
			throw()
		{
			GeometricObject::set(backbone, deep);
		}

		const Backbone& Backbone::operator = (const Backbone& backbone)
			throw()
		{
			set(backbone);

			return *this;
		}

		void Backbone::get(Backbone& backbone, bool deep) const
			throw()
		{
			backbone.set(*this, deep);
		}

		void Backbone::swap(Backbone& backbone)
			throw()
		{
			GeometricObject::swap(backbone);
		}

		String Backbone::getTypeName() const
			throw()
		{
			return String("Backbone");
		}

		bool Backbone::isValid() const
			throw()
		{
			return GeometricObject::isValid();
		}

		void Backbone::dump(ostream& s, Size depth) const
				throw()
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			GeometricObject::dump(s, depth + 1);

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		bool Backbone::extract()
			throw()
		{
			return true;  
		}

		Sphere* Backbone::createSphere_()
		{
			return new Sphere();
		}
		
		Tube* Backbone::createTube_()
		{
			return new Tube();
		}

		Line* Backbone::createLine_()
		{
			return new Line();
		}

	} // namespace MOLVIEW
} // namespace BALL
