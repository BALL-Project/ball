// $Id: backbone.h,v 1.2 2001/01/08 17:29:39 anker Exp $

#ifndef BALL_MOLVIEW_PRIMITIV_BACKBONE_H
#define BALL_MOLVIEW_PRIMITIV_BACKBONE_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_DATATYPE_LIST_H
#	include <BALL/DATATYPE/list.h>
#endif

#ifndef BALL_KERNEL_ATOM_H
#	include <BALL/KERNEL/atom.h>
#endif

#ifndef BALL_VIEW_DATATYPE_COLOR_H
#	include <BALL/VIEW/DATATYPE/color.h>
#endif

#ifndef BALL_VIEW_KERNEL_GEOMETRICOBJECT_H
#include <BALL/VIEW/KERNEL/geometricObject.h>
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

using namespace BALL;
using namespace BALL::VIEW;

namespace BALL
{
	
	namespace MOLVIEW
	{

		/**
		*/
		class Backbone
			: public VIEW::GeometricObject
		{
			public:

			///	@name	Constructors and Destructors
			//@{

			///
			Backbone();

			///
			Backbone(const Backbone& backbone, bool deep = true);

			///
			Backbone(const GeometricObject& geometric_object);

			///
			virtual ~Backbone()
				throw();

			///
			virtual void clear()
				throw();

			///
			virtual void destroy()
				throw();
			//@}

			
			///	@name	Assignment
			//@{

			///
			void set(const Backbone& backbone, bool deep = true);

			///
			Backbone& operator = (const Backbone& backbone);

			///
			void get(Backbone& backbone, bool deep = true) const;

			///
			void swap(Backbone& backbone);

			//@}


			///	@name	Accessors
			//@{

			///
			void setAtomList(const List<Atom*> &atoms);

			///
			void setAtomColorList(const List<ColorRGBA> &atom_colors);

			///
			void createBackbone();

			///
			virtual String getTypeName() const;

			//@}


			/// @name Predicates
			//@{
			//@}


			///	@name	Debugging and Diagnostics
			//@{

			///
			virtual bool isValid() const
				throw();

			///
			virtual void dump
				(std::ostream&  s = std::cout, Size depth = 0) const
				throw();

			//@}


			///	@name	Storers
			//@{

			///
			virtual void read(std::istream&  s)
				throw();

			///
			virtual void write(std::ostream& s) const
				throw();

			//@}


			protected:
			
		  private:

			//_
			class SplinePoint
			{
			  public:

				SplinePoint() {}
				~SplinePoint() {}

				void setVector(const Vector3 &point);
				const Vector3 &getVector() const;

				void setTangentialVector(const Vector3 &tangent);
				const Vector3 &getTangentialVector() const;
				
				void setColor(const ColorRGBA &color);
				const ColorRGBA &getColor() const;

			  private:

				Vector3 point_;
				Vector3 tangent_;
				ColorRGBA color_;
			};


			//_
			virtual Sphere* createSphere_();

			//_
			virtual Tube* createTube_();

			//_
			virtual Line* createLine_();

			//_ init the spline array with both the positions from the atom list
			//_ and the colors from the color list
			void initSplineArray_();

			//_ calculates to every splinepoint the tangential vector
			void calculateTangentialVectors_();
			
			//_ computes the actual spline path through the given support points
			//_ in the splinepoint array
			void createSplinePath_();

			//_ create a spline segment between two spline points a and b
			void createSplineSegment_(const SplinePoint &a, const SplinePoint &b);

			//_ builds a graphical representation to this point with color
			void buildGraphicalRepresentation_(const Vector3 &point, const ColorRGBA &color);


			//_
			List<Atom*> atoms_;

			//_
			List<ColorRGBA> atom_colors_;

			//_
			SplinePoint *spline_array_;

			//_
			int size_of_spline_array_;

			//_
			bool have_start_point_;

			//_
			Vector3 last_point_;
		};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/MOLVIEW/PRIMITIV/backbone.iC>
#		endif
		
	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_PRIMITIV_BACKBONE_H
