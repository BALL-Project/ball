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
				
				/** fetch new results if the model was retrained and update display */
				void update();
		
			
			private:
				ModelItem* model_item_;
				
				QComboBox* feature_combobox_;
				
				QPushButton* delete_feature_button_;
				
				void plot(bool zoom);
				
				map<String,uint> map_names_to_ID_;
			
				
			private slots:
				void selectedFeatureChanged();
				
				void deleteCurrentFeature();
		
			
			
		};
	}
}

#endif //FEATURE_PLOTTER


