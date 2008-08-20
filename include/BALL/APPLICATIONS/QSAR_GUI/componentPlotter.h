#ifndef COMP_PLOTTER
#define COMP_PLOTTER

#include  <BALL/APPLICATIONS/QSAR_GUI/plotter.h>
#include  <BALL/APPLICATIONS/QSAR_GUI/modelItem.h>
#include <QtGui/QComboBox>


namespace BALL
{
	namespace VIEW
	{
		class ComponentPlotter : public Plotter
		{
			Q_OBJECT
			
			public:
				ComponentPlotter(ModelItem* model_item);
		
			
			private:
				void plot();
				
				const Matrix* component_matrix_;
				
				ModelItem* model_item_;
				
				void calculateComponents();
				
				double min_response_value_;
				
				double max_response_value_;
				
				QComboBox* component_one_combobox_;
				QComboBox* component_two_combobox_;
				
			private slots:
				void selectedCompChanged();
				
		};
	}
}

#endif //COMP_PLOTTER



