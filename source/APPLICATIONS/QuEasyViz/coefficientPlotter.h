#ifndef COEFF_PLOTTER
#define COEFF_PLOTTER

#include  <plotter.h>
#include  <modelItem.h>


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


