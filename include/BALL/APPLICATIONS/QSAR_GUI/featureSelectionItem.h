#ifndef FEATURESELECTIONITEM_H
#define FEATURESELECTIONITEM_H

#include <BALL/APPLICATIONS/QSAR_GUI/dataItem.h>
#include <BALL/APPLICATIONS/QSAR_GUI/modelItem.h>
#include <BALL/QSAR/featureSelection.h>

namespace BALL
{
	namespace VIEW
	{
		/**
 		* graphical representation of a feature selection
 		*/

		/** @class FeatureSelectionItem
		* @brief class for the representation of a feature selection
		*
		* @todo
		*/
		class FeatureSelectionItem : public DataItem
		{
			public:
				/** @name Constructors and Destructors*/
				FeatureSelectionItem(int type, DataItemView* miv);
				~FeatureSelectionItem();
			
				FeatureSelectionItem(FeatureSelectionItem& fs_item); 

				void connectWithModelItem();

				FeatureSelection* featureSelection();
				int getType();
				void setK(int k);
				bool opt();
				void setOpt(bool opt);
				void setModelItem(ModelItem* model_item);
				void setInputModelItem(ModelItem* model_item);
				int numberOfFeatures();
				ModelItem* modelItem();
				ModelItem* inputModelItem();	
				
				int setValidationStatistic(int s) {validation_statistic_=s;};

				int k();

				enum { Type = UserType + 4 };
				int type() const { return Type; }

		
			protected:
				void mousePressEvent(QGraphicsSceneMouseEvent *event);
		
			private:
			
				FeatureSelection* feature_selection_;
				ModelItem* model_item_;
				ModelItem* input_model_item_;
				int type_;
				int k_;
				bool opt_;
				int validation_statistic_;
				
				
				friend class DataItemScene;
				friend class FeatureSelectionDialog;
		};
	}
}

#endif
