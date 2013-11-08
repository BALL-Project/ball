#ifndef COMP_PLOTTER
#define COMP_PLOTTER

#include  <plotter.h>
#include  <modelItem.h>
#include <QtGui/QComboBox>
#include <BALL/QSAR/latentVariableModel.h>


namespace BALL
{
	namespace VIEW
	{
		class ComponentPlotter : public Plotter
		{
			Q_OBJECT
			
			public:
				ComponentPlotter(ModelItem* model_item, bool plot_loadings);
		
			
			private:
				void plot(bool zoom);
				
				const Eigen::MatrixXd* component_matrix_;
				
				ModelItem* model_item_;
				
				void calculateComponents();
				
				double min_response_value_;
				
				double max_response_value_;
				
				/** should the loadings be plotted or the latent variables? */
				bool plot_loadings_;
				
				QComboBox* component_one_combobox_;
				QComboBox* component_two_combobox_;
				
			private slots:
				void selectedCompChanged();
				
			protected slots:
				/** overloads Plotter::activityChange(), in order to make sure to recalculate min and max of the response variable in case of a latent-variable-plotter */
				void activityChange();
				
		};
	}
}

#endif //COMP_PLOTTER



