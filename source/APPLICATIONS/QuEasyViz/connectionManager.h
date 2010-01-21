/* connectionManager.h
 * 
 * Copyright (C) 2009 Marcel Schumann
 * 
 * This file is part of QuEasy -- A Toolbox for Automated QSAR Model
 * Construction and Validation.
 * QuEasy is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 * 
 * QuEasy is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CONMANAGER_H
#define CONMANAGER_H

#include<QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QListWidgetItem>

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

