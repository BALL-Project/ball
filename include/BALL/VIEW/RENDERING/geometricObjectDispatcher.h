#ifndef BALL_VIEW_RENDERING_GEOMETRICOBJECTDISPATCHER_H
#define BALL_VIEW_RENDERING_GEOMETRICOBJECTDISPATCHER_H

#include <BALL/COMMON/global.h>

namespace BALL
{
	namespace VIEW
	{
		class Line;
		class MultiLine;
		class Tube;
		class Mesh;
		class Label;
		class Point;
		class Sphere;
		class Disc;
		class Box;
		class SimpleBox;
		class TwoColoredLine;
		class TwoColoredTube;
		class ClippingPlane;
		class GridVisualisation;
		class QuadMesh;
		class GeometricObject;

		class BALL_VIEW_EXPORT GeometricObjectDispatcher
		{
			protected:
				virtual void render_(const GeometricObject*);

				virtual void renderSphere_           (const Sphere&           );
				virtual void renderTwoColoredTube_   (const TwoColoredTube&   );
				virtual void renderMesh_             (const Mesh&             );
				virtual void renderTwoColoredLine_   (const TwoColoredLine&   );
				virtual void renderPoint_            (const Point&            );
				virtual void renderDisc_             (const Disc&             );
				virtual void renderLine_             (const Line&             );
				virtual void renderTube_             (const Tube&             );
				virtual void renderBox_              (const Box&              );
				virtual void renderSimpleBox_        (const SimpleBox&        );
				virtual void renderLabel_            (const Label&            );
				virtual void renderMultiLine_        (const MultiLine&        );
				virtual void renderGridVisualisation_(const GridVisualisation&);
				virtual void renderQuadMesh_         (const QuadMesh&         );
		};
	}
}

#endif //BALL_VIEW_RENDERING_GEOMETRICOBJECTDISPATCHER_H