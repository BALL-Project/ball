#ifndef CONMANAGER_H
#define CONMANAGER_H

#include<QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QListWidgetItem>

#include <modelConfigurationDialog.h>
#include <inputDataItem.h>
#include <featureSelectionItem.h>
#include <validationItem.h>
#include <predictionItem.h>

namespace BALL
{
	namespace VIEW
	{
		class ModelConfigurationDialog;
		
		class ConnectionManager : public QWidget
		{
			Q_OBJECT
			
			public:
				ConnectionManager(ModelConfigurationDialog* parent);
				~ConnectionManager();
			
			private slots:
				void connectItem(QListWidgetItem* item);
				void disconnectItem(QListWidgetItem* item);
		
			protected:

				void addItems();

				ModelConfigurationDialog* parent_;
				QListWidget* connected_items_;
				QListWidget* disconnected_items_;
				QPushButton* connect_button_;
				QPushButton* disconnect_button_;

				class CMInputItem_ : public QListWidgetItem
				{
					public:
						CMInputItem_(InputDataItem* input);
						~CMInputItem_();

					private:
						InputDataItem* input_;

						enum {ItemType = QListWidgetItem::UserType + 2 };
				};

				class CMFeatureSelectionItem_ : public QListWidgetItem
				{
					public:
						CMFeatureSelectionItem_(FeatureSelectionItem* feature_selection);
						~CMFeatureSelectionItem_();

					private:
						FeatureSelectionItem* feature_selection_;

						enum {ItemType = QListWidgetItem::UserType + 3 };
				};

				class CMValidationItem_ : public QListWidgetItem
				{
					public:
						CMValidationItem_(ValidationItem* validation);
						~CMValidationItem_();

					private:
						ValidationItem* validation_;

						enum {ItemType = QListWidgetItem::UserType + 4 };
				};

				class CMPredictionItem_ : public QListWidgetItem
				{
					public:
						CMPredictionItem_(PredictionItem* prediction);
						~CMPredictionItem_();

					private:
						PredictionItem* prediction_;

						enum {ItemType = QListWidgetItem::UserType + 5 };
				};

		};
	}
}

#endif

