#ifndef PRED_PLOTTER
#define PRED_PLOTTER

#include <BALL/APPLICATIONS/QuEasyViz/plotter.h>
#include <BALL/QSAR/QSARData.h>


namespace BALL
{
	namespace VIEW
	{
		class ValidationItem;
		class PredictionItem;
		
		class PredictionPlotter : public Plotter
		{
			public:
				PredictionPlotter(PredictionItem* item);
				
				PredictionPlotter(ValidationItem* item);
				

			protected:
				void plot(bool zoom);
			
			private:
				PredictionItem* pred_item_;
				
				ValidationItem* val_item_;
				
				BALL::QSAR::QSARData* data_;
			
				void plotObservedVsExpected(bool zoom);
				
				void plotObserved(bool zoom);
				
				void plotConfusion(bool zoom);
			
			
		};
	}
}

#endif //PRED_PLOTTER

