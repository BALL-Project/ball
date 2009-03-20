// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: gridVisualisation.h,v 1.1.4.1 2007-03-25 21:26:08 oliver Exp $
//

#ifndef  BALL_VIEW_PRIMITIV_GRIDSLICE_H
#define  BALL_VIEW_PRIMITIV_GRIDSLICE_H

#ifndef BALL_VIEW_KERNEL_GEOMETRICOBJECT_H
# include <BALL/VIEW/KERNEL/geometricObject.h>
#endif

#ifndef BALL_DATATYPE_REGULARDATA3D_H
# include <BALL/DATATYPE/regularData3D.h>
#endif

namespace BALL
{
	namespace VIEW
	{
		/** GridVisualisation 
				\ingroup ViewPrimitives
		*/
		class BALL_VIEW_EXPORT GridVisualisation
			: public GeometricObject
		{
			public:

			enum Type
			{
				///
				PLANE,

				///
				SLICES,

				///
				DOTS
			};

			BALL_CREATE(GridVisualisation)

			///
			GridVisualisation();

			///
			GridVisualisation(const GridVisualisation& plane);

			/** Destructor
			*/
			virtual ~GridVisualisation(){};

			///
			const GridVisualisation& operator = (const GridVisualisation& plane);
			
			///
			const Vector3& getNormal() const { return normal_;}

			///
			void setNormal(const Vector3& normal) { normal_ = normal;}

			///
			const Vector3& getPoint() const { return point_;}

			///
			void setPoint(const Vector3& v) { point_ = v;}

			///
			void setGrid(const RegularData3D* grid) { grid_ = grid;}

			///
			const RegularData3D* getGrid() const { return grid_;}

			///
			Position getTexture() const { return texture_;}

			///
			void setTexture(Position texture) { texture_ = texture;}

			///
			void setDotSize(Size dot_size) { dot_size_ = dot_size;}

			///
			Size getDotSize() const { return dot_size_;}

			Vector3 origin, x,y,z;
			float max_dim;
			Size slices;
			vector<Vector3> points;
			Type type;
			bool draw_box;

			protected:

			Vector3 normal_;
			Vector3 point_;
			Position texture_;
			const RegularData3D* grid_;
			Size dot_size_;
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_KERNEL_CLIPPING_PLANE_H
