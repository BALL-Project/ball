// $Id: backbone.C,v 1.3 2001/01/26 01:37:14 amoll Exp $

#include <BALL/MOLVIEW/PRIMITIV/backbone.h>

using namespace std;

namespace BALL
{

	namespace MOLVIEW
	{

		Backbone::Backbone()
			:	GeometricObject(),
				spline_array_(0),
				size_of_spline_array_(0)
		{
		}

		Backbone::Backbone(const Backbone& backbone, bool deep)
			:	GeometricObject(backbone, deep),
				spline_array_(0),
				size_of_spline_array_(0)
		{
		}

		Backbone::Backbone(const GeometricObject& geometric_object)
			:	GeometricObject(geometric_object),
				spline_array_(0),
				size_of_spline_array_(0)
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
		{
			GeometricObject::set(backbone, deep);
		}

		Backbone& Backbone::operator = (const Backbone& backbone)
		{
			set(backbone);

			return *this;
		}

		void Backbone::get(Backbone& backbone, bool deep) const
		{
			backbone.set(*this, deep);
		}

		void Backbone::swap(Backbone& backbone)
		{
			GeometricObject::swap(backbone);
		}

		// creates the backbone representation from the given atom and 
		// atom colors list
		void Backbone::createBackbone()
		{
			// atom list empty => finished
			if (atoms_.size() == 0)
			{
				return;
			}

			spline_array_ = new SplinePoint[atoms_.size()];

			// Memory allocation failed
			if (spline_array_ == 0)
			{
				return;
			}

			size_of_spline_array_ = atoms_.size();
			have_start_point_ = false;

			// initialized spline_array_
			initSplineArray_();

			calculateTangentialVectors_();
			
			createSplinePath_();

			delete [] spline_array_;
			size_of_spline_array_ = 0;
		}

		String Backbone::getTypeName() const
		{
			return String("Backbone");
		}

		bool Backbone::isValid() const
			throw()
		{
			return (GeometricObject::isValid();
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

		void Backbone::read(istream & /* s */)
			throw()
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		void Backbone::write(ostream & /* s */) const
			throw()
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
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

		// init the spline array with both the positions from the atom list
		// and the colors from the color list
		void Backbone::initSplineArray_()
		{
			List<Atom*>::Iterator it_atoms;
			int index = 0;

			// for all used atoms
			for (it_atoms = atoms_.begin();
					 it_atoms != atoms_.end(); ++it_atoms)
			{
				spline_array_[index++].setVector((*it_atoms)->getPosition());
			}
			
			List<ColorRGBA>::Iterator it_color;
			index = 0;

			ColorRGBA color(255,255,255);

			// for all used colors
			for (it_color = atom_colors_.begin();
					 it_color != atom_colors_.end(); ++it_color)
			{
				color = *it_color;
				spline_array_[index++].setColor(color);
			}

			for (;index < size_of_spline_array_; ++index)
			{
				spline_array_[index].setColor(color);				
			}		
		}
		
		// calculates to every splinepoint the tangential vector
		void Backbone::calculateTangentialVectors_()
		{
			// first and last spline point have tangential vectors (0,0,0)
			// so lets forget about the first and the last spline point
			
			int index = 1;

			for (; index < size_of_spline_array_ - 1; ++index)
			{
				Vector3 tangent;

				tangent.x = 0.8 * (spline_array_[index + 1].getVector().x 
													 - spline_array_[index - 1].getVector().x);

				tangent.y = 0.8 * (spline_array_[index + 1].getVector().y 
													 - spline_array_[index - 1].getVector().y);

				tangent.z = 0.8 * (spline_array_[index + 1].getVector().z 
													 - spline_array_[index - 1].getVector().z);

				spline_array_[index].setTangentialVector(tangent);
			}
		}
		
		// computes the actual spline path through the given support points
		// in the splinepoint array
		void Backbone::createSplinePath_()
		{
			int index;
			
			// creates the spline
			for (index = 0; index < size_of_spline_array_ - 1; ++index)
			{
				createSplineSegment_(spline_array_[index], spline_array_[index + 1]);
			}
		}

		// create a spline segment between two spline points a and b
		void Backbone::createSplineSegment_(const SplinePoint &a, const SplinePoint &b)
		{
			int max_step = 6;
			double time = 0.0;
			double step = (double)1 / (double)max_step;

			for (int index = 0; index <= max_step; ++index, time += step)
			{
				double t_2 = time * time;
				double t_3 = t_2 * time;
				double m2_t_3 = 2.0 * t_3;
				double m3_t_2 = 3.0 * t_2;

				double h1 = m2_t_3 - m3_t_2 + 1.0;
				double h2 = - m2_t_3 + m3_t_2;
				double h3 = t_3 - 2.0 * t_2 + time;
				double h4 = t_3 - t_2;

				double x1_old, y1_old, z1_old;
				double x2_old, y2_old, z2_old;
				double t1x_old, t1y_old, t1z_old;
				double t2x_old, t2y_old, t2z_old;

				x1_old = a.getVector().x;
				y1_old = a.getVector().y;
				z1_old = a.getVector().z;

				x2_old = b.getVector().x;
				y2_old = b.getVector().y;
				z2_old = b.getVector().z;

				t1x_old = a.getTangentialVector().x;
				t1y_old = a.getTangentialVector().y;
				t1z_old = a.getTangentialVector().z;

				t2x_old = b.getTangentialVector().x;
				t2y_old = b.getTangentialVector().y;
				t2z_old = b.getTangentialVector().z;

				Vector3 new_vector;

				new_vector.x = (h1 * x1_old) + (h2 * x2_old) + (h3 * t1x_old) + (h4 * t2x_old);
				new_vector.y = (h1 * y1_old) + (h2 * y2_old) + (h3 * t1y_old) + (h4 * t2y_old);
				new_vector.z = (h1 * z1_old) + (h2 * z2_old) + (h3 * t1z_old) + (h4 * t2z_old);

				// build the objects
				buildGraphicalRepresentation_
					(new_vector, ((index <= max_step/2) ? a.getColor() : b.getColor()));
			}
		}

		// builds a graphical representation to this point
		void Backbone::buildGraphicalRepresentation_(const Vector3 &point, const ColorRGBA &color)
		{
			if (have_start_point_)
			{
				if (point == last_point_)
				{
					return;
				}
				
				// build tube connection to the last point
				Tube *tube = createTube_();
				
				if (!tube)
				{
					// throw ...
				}
				
				tube->PropertyManager::set(*this);
				tube->setRadius(0.4);
				tube->setVertex1(last_point_);
				tube->setVertex2(point);
				tube->setColor(color);
				
				this->Composite::appendChild(*tube);

				// build line (dynamic mode) connection to the last point
				Line *line = createLine_();
				
				if (!line)
				{
					// throw ...
				}
				
				line->PropertyManager::set(*this);
				line->setProperty(GeometricObject::PROPERTY__OBJECT_DYNAMIC);
				line->setVertex1(last_point_);
				line->setVertex2(point);
				line->setColor(color);
				
				this->Composite::appendChild(*line);
			}
			else
			{
				have_start_point_ = true;
			}

			last_point_ = point;

			// create sphere for the point
			Sphere *sphere = createSphere_();
			
			if (!sphere)
			{
				// throw ...
			}
			
			sphere->PropertyManager::set(*this);
			sphere->setRadius(0.4);
			sphere->setVertex(point);
			sphere->setColor(color);
			
			this->Composite::appendChild(*sphere);
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/MOLVIEW/PRIMITIV/backbone.iC>
#		endif

	} // namespace MOLVIEW

} // namespace BALL
