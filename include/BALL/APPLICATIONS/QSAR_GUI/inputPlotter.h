#ifndef INPUT_PLOTTER
#define INPUT_PLOTTER

#include  <BALL/APPLICATIONS/QSAR_GUI/plotter.h>
#include  <BALL/APPLICATIONS/QSAR_GUI/inputDataItem.h>


namespace BALL
{
	namespace VIEW
	{		
		class InputPlotter : public Plotter
		{
			public:
				InputPlotter(InputDataItem* item);
				
				
		
			private:
				InputDataItem* input_item_;
				QSARData* data_;
				
				void plot();
				
				void plotActivity();
			
	
		};
	}
}

#endif //INPUT_PLOTTER

