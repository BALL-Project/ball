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
			Q_OBJECT
			
			public:
				InputPlotter(InputDataItem* item);
				
				~InputPlotter();
				
		
			protected slots:
				
				void sortChangeState();
				
			private:
				InputDataItem* input_item_;
				QSARData* data_;
				
				QCheckBox* sort_checkbox_;
				
				bool sort_;
				
				void plot();
				
				void plotActivity();
				
				void plotSortedActivity();
			
	
		};
	}
}

#endif //INPUT_PLOTTER

