#ifndef PRED_PLOTTER
#define PRED_PLOTTER

#include  <BALL/APPLICATIONS/QSAR_GUI/plotter.h>
#include  <BALL/APPLICATIONS/QSAR_GUI/predictionItem.h>


namespace BALL
{
	namespace VIEW
	{
		class PredictionPlotter : public Plotter
		{
			public:
				PredictionPlotter(PredictionItem* item);
				
				void plotObservedVsExpected(bool zoom);
				
				void plotObserved(bool zoom);
		
			
			private:
				PredictionItem* pred_item_;
				QSARData* data_;
				
				void plot(bool zoom);
			
			
			
			
			
		};
	}
}

#endif //PRED_PLOTTER

