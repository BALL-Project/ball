#ifndef FEATURE_PLOTTER
#define FEATURE_PLOTTER

#include  <BALL/APPLICATIONS/QSAR_GUI/plotter.h>
#include  <BALL/APPLICATIONS/QSAR_GUI/modelItem.h>


namespace BALL
{
	namespace VIEW
	{
		class FeaturePlotter : public Plotter
		{
			public:
				FeaturePlotter(ModelItem* model_item);
		
			
			private:
				ModelItem* model_item_;
				
				void plot(bool zoom);
		
			
			
		};
	}
}

#endif //FEATURE_PLOTTER


