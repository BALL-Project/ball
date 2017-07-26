#include <BALL/VIEW/MODELS/colorByGridProcessor.h>

#include <BALL/VIEW/PRIMITIVES/mesh.h>
#include <BALL/VIEW/PRIMITIVES/quadMesh.h>
#include <BALL/VIEW/PRIMITIVES/multiLine.h>

#include <BALL/VIEW/WIDGETS/gridColorWidget.h>

namespace BALL
{
	namespace VIEW
	{
		 ColorByGridProcessor::ColorByGridProcessor(const RegularData3D* grid, const GridColorWidget* widget)
			: values_(),
			  objects_(),
			  min_value_(-1.0f),
			  mid_value_(0.0f),
			  max_value_(1.0f),
			  normalization_enabled_(false),
			  min_min_color_(),
			  min_color_(),
			  mid_color_(),
			  max_color_(),
			  max_max_color_(),
			  cm_(),
			  grid_(grid),
			  num_levels_(0)
		{
			if(widget)
			{
				setColors(
					widget->getMinMinColor(),
					widget->getMinColor(),
					widget->getMidColor(),
					widget->getMaxColor(),
					widget->getMaxMaxColor()
				);

				setBreakpoints(widget->getMinValue(), widget->getMidValue(), widget->getMaxValue());
				setNumLevels(widget->getNumLevels());
				setNormalizationEnabled(widget->isNormalizationEnabled());
			}
		}

		bool ColorByGridProcessor::start()
		{
			if(!ColorProcessor::start())
			{
				return false;
			}

			if((min_value_ >= mid_value_) || (mid_value_ >= max_value_))
			{
				return false;
			}

			ColorRGBA list[3];
			list[0] = min_color_;
			list[1] = mid_color_;
			list[2] = max_color_;

			cm_.setBaseColors(list, 3);
			cm_.setMinMaxColors(min_min_color_, max_max_color_);
			cm_.setAlphaBlending(true);
			cm_.setNumberOfColors(static_cast<Size>(num_levels_));
			cm_.setRange(min_value_, max_value_);

			std::vector<Vector4> interpolation_points(3);
			interpolation_points[0] = Vector4(0.f);
			interpolation_points[1] = Vector4( (mid_value_ - min_value_) / (max_value_ - min_value_));
			interpolation_points[2] = Vector4(1.f);

			cm_.setInterpolationBoundaries(interpolation_points);
			cm_.createMap();

			values_.clear();

			return true;
		}

		Processor::Result ColorByGridProcessor::operator() (GeometricObject*& object)
		{
			objects_.push_back(object);

			try
			{
				vector<Vector3>* vertices = nullptr;

				Mesh* mesh = dynamic_cast<Mesh*>(object);
				if (mesh) vertices = &mesh->vertex;

				QuadMesh* qmesh = dynamic_cast<QuadMesh*>(object);
				if (qmesh) vertices = &qmesh->vertex;

				MultiLine* line = dynamic_cast<MultiLine*>(object);
				if (line) vertices = &line->vertices;

				if (!vertices) return Processor::CONTINUE;

				values_.reserve(values_.size() + vertices->size());
				for (Position i = 0; i < vertices->size(); i++)
				{
					values_.push_back(grid_->getInterpolatedValue((*vertices)[i]));
				}
			}
			catch (Exception::OutOfGrid&)
			{
				Log.warn() << "Warning! There is a point contained in the surface that is not inside "
				           << "the grid!" << std::endl;
			}

			return Processor::CONTINUE;
		}

		bool ColorByGridProcessor::finish()
		{
			if(!ColorProcessor::finish())
			{
				return false;
			}

			if (normalization_enabled_)
			{
				calculateHistogramEqualization(values_, values_);
			}

			size_t p = 0;

			for(std::list<GeometricObject*>::iterator it = objects_.begin(); it != objects_.end(); ++it)
			{
				vector<ColorRGBA>* colors = nullptr;
				vector<Vector3>*   vertices = nullptr;

				Mesh* mesh = dynamic_cast<Mesh*>(*it);
				if (mesh)
				{
					colors = &mesh->colors;
					vertices = &mesh->vertex;
				}

				QuadMesh* qmesh = dynamic_cast<QuadMesh*>(*it);
				if (qmesh)
				{
					colors = &qmesh->colors;
					vertices = &qmesh->vertex;
				}

				MultiLine* line = dynamic_cast<MultiLine*>(*it);
				if (line)
				{
					colors = &line->colors;
					vertices = &line->vertices;
				}

				if (!colors) continue;

				colors->resize(vertices->size());
				for (Position i = 0; (i < colors->size()) && (p < values_.size()); ++i, ++p)
				{
					(*colors)[i].set(cm_.map(values_[p]));
				}
			}

			objects_.clear();
			values_.clear();

			return true;
		}

		void ColorByGridProcessor::setNormalizationEnabled(bool enabled)
		{
			normalization_enabled_ = enabled;
		}

		void ColorByGridProcessor::setColors(const ColorRGBA& min_min, const ColorRGBA& min, const ColorRGBA& mid,
		                                     const ColorRGBA& max, const ColorRGBA& max_max)
		{
			min_min_color_ = min_min;
			min_color_     = min;
			mid_color_     = mid;
			max_color_     = max;
			max_max_color_ = max_max;
		}

		void ColorByGridProcessor::setBreakpoints(float min_value, float mid_value, float max_value)
		{
			min_value_ = min_value;
			mid_value_ = mid_value;
			max_value_ = max_value;
		}

		void ColorByGridProcessor::setNumLevels(int num_levels)
		{
			num_levels_ = num_levels;
		}
	}
}
