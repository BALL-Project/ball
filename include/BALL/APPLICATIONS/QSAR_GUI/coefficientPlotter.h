#ifndef COEFF_PLOTTER
#define COEFF_PLOTTER

#include  <BALL/APPLICATIONS/QSAR_GUI/plotter.h>
#include  <BALL/APPLICATIONS/QSAR_GUI/modelItem.h>


namespace BALL
{
	namespace VIEW
	{
		class CoefficientPlotter : public Plotter
		{
			public:
				CoefficientPlotter(ModelItem* model_item);
		
			
			private:
				ModelItem* model_item_;
				
				void plot(bool zoom);
		
			
			
		};
	}
}

#endif //COEFF_PLOTTER


