#include <BALL/VIEW/RENDERING/geometricObjectDispatcher.h>

#include <BALL/COMMON/rtti.h>

#include <BALL/VIEW/PRIMITIVES/label.h>
#include <BALL/VIEW/PRIMITIVES/line.h>
#include <BALL/VIEW/PRIMITIVES/mesh.h>
#include <BALL/VIEW/PRIMITIVES/point.h>
#include <BALL/VIEW/PRIMITIVES/box.h>
#include <BALL/VIEW/PRIMITIVES/simpleBox.h>
#include <BALL/VIEW/PRIMITIVES/sphere.h>
#include <BALL/VIEW/PRIMITIVES/tube.h>
#include <BALL/VIEW/PRIMITIVES/disc.h>
#include <BALL/VIEW/PRIMITIVES/twoColoredLine.h>
#include <BALL/VIEW/PRIMITIVES/twoColoredTube.h>
#include <BALL/VIEW/PRIMITIVES/mesh.h>
#include <BALL/VIEW/PRIMITIVES/quadMesh.h>
#include <BALL/VIEW/PRIMITIVES/multiLine.h>
#include <BALL/VIEW/PRIMITIVES/gridVisualisation.h>

#include <QtCore/QCoreApplication>

namespace BALL
{
	namespace VIEW
	{
		void GeometricObjectDispatcher::render_(const GeometricObject* object)
		{
			// most used geometric objects first
			if      (RTTI::isKindOf<Sphere>(object))            renderSphere_           (*(const Sphere*)            object);
			else if (RTTI::isKindOf<TwoColoredTube>(object))    renderTwoColoredTube_   (*(const TwoColoredTube*)    object);
			else if (RTTI::isKindOf<Mesh>(object))              renderMesh_             (*(const Mesh*)              object);
			else if (RTTI::isKindOf<TwoColoredLine>(object))    renderTwoColoredLine_   (*(const TwoColoredLine*)    object);
			else if (RTTI::isKindOf<Point>(object))             renderPoint_            (*(const Point*)             object);
			else if (RTTI::isKindOf<Disc>(object))              renderDisc_             (*(const Disc*)              object);
			else if (RTTI::isKindOf<Line>(object))              renderLine_             (*(const Line*)              object);
			else if (RTTI::isKindOf<Tube>(object))              renderTube_             (*(const Tube*)              object);
			else if (RTTI::isKindOf<Box>(object))               renderBox_              (*(const Box*)               object);
			else if (RTTI::isKindOf<SimpleBox>(object))         renderSimpleBox_        (*(const SimpleBox*)         object);
			else if (RTTI::isKindOf<Label>(object))             renderLabel_            (*(const Label*)             object);
			else if (RTTI::isKindOf<MultiLine>(object))         renderMultiLine_        (*(const MultiLine*)         object);
			else if (RTTI::isKindOf<GridVisualisation>(object)) renderGridVisualisation_(*(const GridVisualisation*) object);
			else if (RTTI::isKindOf<QuadMesh>(object))          renderQuadMesh_         (*(const QuadMesh*)          object);
			// ... add more types of GeometricObjects here
			else
			{
				// unknown type of GeometricObject
				Log.error()
					<< QCoreApplication::instance()->translate("BALL::VIEW::Renderer", "unknown type of GeometricObject in" ).toStdString()
					<< " Renderer::render_: "
					<< typeid(object).name() << "  " << object << std::endl;
				return;
			}
		}

		// Render a text label
		void GeometricObjectDispatcher::renderLabel_(const Label&)
		{
			Log.error() << "renderLabel_ not implemented." << std::endl;
		}

		/// Render a line
		void GeometricObjectDispatcher::renderLine_(const Line&)
		{
			Log.error() << "renderLine_ not implemented." << std::endl;
		}

		/// Render an illuminated line
		void GeometricObjectDispatcher::renderMultiLine_(const MultiLine&)
		{
			Log.error() << "renderMultiLine_ not implemented." << std::endl;
		}

		/// Render a surface mesh
		void GeometricObjectDispatcher::renderMesh_(const Mesh&)
		{
			Log.error() << "renderMesh_ not implemented." << std::endl;
		}

		/// Render a single point
		void GeometricObjectDispatcher::renderPoint_(const Point&)
		{
			Log.error() << "renderPoint_ not implemented." << std::endl;
		}

		/// Render a box
		void GeometricObjectDispatcher::renderBox_(const Box&)
		{
			Log.error() << "renderBox_ not implemented." << std::endl;
		}

		/// Render a simple box (parallel to the axes)
		void GeometricObjectDispatcher::renderSimpleBox_(const SimpleBox&)
		{
			Log.error() << "renderSimpleBox_ not implemented." << std::endl;
		}

		/// Render a sphere
		void GeometricObjectDispatcher::renderSphere_(const Sphere&)
		{
			Log.error() << "renderSphere_ not implemented." << std::endl;
		}

		/// Render a disc
		void GeometricObjectDispatcher::renderDisc_(const Disc&)
		{
			Log.error() << "renderDisc_ not implemented." << std::endl;
		}

		/// Render a tube
		void GeometricObjectDispatcher::renderTube_(const Tube&)
		{
			Log.error() << "renderTube_ not implemented." << std::endl;
		}

		/// Render a line with two colors
		void GeometricObjectDispatcher::renderTwoColoredLine_(const TwoColoredLine&)
		{
			Log.error() << "renderTwoColoredLine_ not implemented." << std::endl;
		}

		/// Render a tube with two colors
		void GeometricObjectDispatcher::renderTwoColoredTube_(const TwoColoredTube&)
		{
			Log.error() << "renderTwoColoredTube_ not implemented." << std::endl;
		}

		/// Render a grid
		void GeometricObjectDispatcher::renderGridVisualisation_(const GridVisualisation&)
		{
			Log.error() << "renderGridVisualisation_ not implemented." << std::endl;
		}

		/// Render a quad mesh
		void GeometricObjectDispatcher::renderQuadMesh_(const QuadMesh&)
		{
			Log.error() << "renderQuadMesh_ not implemented." << std::endl;
		}
	}
}
