#ifndef BALL_VIEW_MODELS_COLORBYGRIDPROCESSOR_H
#define BALL_VIEW_MODELS_COLORBYGRIDPROCESSOR_H

#include <BALL/DATATYPE/regularData3D.h>

#include <BALL/VIEW/MODELS/colorProcessor.h>
#include <BALL/VIEW/DATATYPE/colorMap.h>

namespace BALL
{
	namespace VIEW
	{
		class GridColorWidget;

		class BALL_VIEW_EXPORT ColorByGridProcessor : public ColorProcessor
		{
			public:
				ColorByGridProcessor(const RegularData3D* grid, const GridColorWidget* widget = 0);

				virtual bool start();
				virtual bool finish();
				virtual Processor::Result operator() (GeometricObject*& object);

				void setNormalizationEnabled(bool enabled);

				void setColors(const ColorRGBA& min_min, const ColorRGBA& min,
				               const ColorRGBA& mid,     const ColorRGBA& max, const ColorRGBA& max_max);

				void setBreakpoints(float min_value, float mid_value, float max_value);

				void setNumLevels(int num_levels);

			protected:
				vector<float> values_;
				list<GeometricObject*> objects_;

				float min_value_;
				float mid_value_;
				float max_value_;

				bool normalization_enabled_;

				ColorRGBA min_min_color_;
				ColorRGBA min_color_;
				ColorRGBA mid_color_;
				ColorRGBA max_color_;
				ColorRGBA max_max_color_;

				ColorMap cm_;
				const RegularData3D* grid_;
				int num_levels_;
		};
	}
}

#endif //BALL_VIEW_MODELS_COLORBYGRIDPROCESSOR_H
