#ifndef FEATURE_PLOTTER
#define FEATURE_PLOTTER

#include  <BALL/APPLICATIONS/QSAR_GUI/plotter.h>
#include  <BALL/APPLICATIONS/QSAR_GUI/modelItem.h>
#include <QtGui/QComboBox>


namespace BALL
{
	namespace VIEW
	{
		class FeaturePlotter : public Plotter
		{
			Q_OBJECT
			
			public:
				FeaturePlotter(ModelItem* model_item);
		
			
			private:
				ModelItem* model_item_;
				
				QComboBox* feature_combobox_;
				
				void plot(bool zoom);
			
				
			private slots:
				void selectedFeatureChanged();
		
			
			
		};
	}
}

#endif //FEATURE_PLOTTER


