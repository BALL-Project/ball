#include <BALL/COMMON/global.h>
#include <BALL/QSAR/Model.h>
#include <BALL/QSAR/QSARData.h>

namespace BALL
{
	namespace QSAR
	{
		/** A class for automatic creation of the most appropriate QSAR model.\n
		All available model-types are therefore evaluated using nested cross-validation and several successive feature selection steps. */

		class BALL_EXPORT AutomaticModelCreator
		{
			public:

				AutomaticModelCreator(const QSARData* data);

				/** Start the search for most appropriate QSAR model for the current data. \n
				All available model-types are therefore evaluated using nested cross-validation and several successive feature selection steps. \n
				The model that obtained the highest nested prediction quality (>min_quality_) will be returned by this function. \n
				If no model achieved a quality higher than min_quality_, 0 will be returned. */
				Model* generateModel();

				void setData(const QSARData* data);

				void setMinQuality(double min_quality);

			private:

				void optimizeParameters(Model* model);

				void selectFeatures(Model* model);

				const QSARData* data_;

				double min_quality_;
		};

	}
}

