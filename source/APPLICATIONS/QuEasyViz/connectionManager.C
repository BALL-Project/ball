/* connectionManager.C
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

#include <connectionManager.h>
#include <QtGui/QGridLayout>
#include <QtGui/QVBoxLayout>

using namespace BALL::QSAR;

namespace BALL
{
	namespace VIEW
	{

		ConnectionManager::ConnectionManager(ModelConfigurationDialog* parent):
			parent_(parent)
		{
			if (parent_ == NULL)
			{
				return;
			}

			connected_items_ = new QListWidget(this);
			connected_items_->setViewMode(QListView::IconMode);
			connected_items_->setIconSize(QSize(32, 32));
			connected_items_->setMovement(QListView::Static);

			disconnected_items_ = new QListWidget(this);
			disconnected_items_->setViewMode(QListView::IconMode);
			disconnected_items_->setIconSize(QSize(32, 32));
			disconnected_items_->setMovement(QListView::Static);

			connect_button_ = new QPushButton(" <- connect");
			disconnect_button_ = new QPushButton("disconnect ->");

			QVBoxLayout* button_layout = new QVBoxLayout();
			button_layout->addWidget(disconnect_button_);
			button_layout->addWidget(connect_button_);
			QGridLayout* main_layout = new QGridLayout();
			main_layout->addWidget(connected_items_,1,1);
			main_layout->addLayout(button_layout,1,2);
			main_layout->addWidget(disconnected_items_,1,3);

			setLayout(main_layout);	
			
			addItems();
		}


		ConnectionManager::~ConnectionManager()
		{
		}

		void ConnectionManager::addItems()
		{
			///add all items, that are connected to the model item
			ModelItem* model_item = parent_->modelItem();

			foreach(Edge* edge, model_item->inEdges())
			{
				DataItem* item = edge->sourceNode();
				if (qgraphicsitem_cast<InputDataItem *>(item))
				{
					InputDataItem* input_item = qgraphicsitem_cast<InputDataItem *>(item); 
					CMInputItem_* cm_input = new CMInputItem_(input_item);
					connected_items_->addItem(cm_input);
				}	
			} 
			
			foreach(Edge* edge, model_item->outEdges())
			{
				DataItem* item = edge->destNode();

				if (qgraphicsitem_cast<FeatureSelectionItem *>(item))
				{
					FeatureSelectionItem* fs_item = qgraphicsitem_cast<FeatureSelectionItem *>(item); 
					CMFeatureSelectionItem_* cm_input = new CMFeatureSelectionItem_(fs_item);
					connected_items_->addItem(cm_input);
				}
				else if (qgraphicsitem_cast<ValidationItem *>(item))
				{
					ValidationItem* val_item = qgraphicsitem_cast<ValidationItem *>(item); 
					CMValidationItem_* cm_val = new CMValidationItem_(val_item);
					connected_items_->addItem(cm_val);
				}
				else if (qgraphicsitem_cast<PredictionItem *>(item))
				{
					PredictionItem* pred_item = qgraphicsitem_cast<PredictionItem *>(item); 
					CMPredictionItem_* cm_pred = new CMPredictionItem_(pred_item);
					connected_items_->addItem(cm_pred);
				}
			}

			Pipeline<DataItem*> disconnected_items = parent_->modelItem()->view()-> data_scene->main_window->disconnectedItems();
			for (Pipeline<DataItem*>::iterator it = disconnected_items.begin(); it != disconnected_items.end(); it++)
			{
				if (qgraphicsitem_cast<FeatureSelectionItem *>(*it))
				{
					FeatureSelectionItem* fs_item = qgraphicsitem_cast<FeatureSelectionItem *>(*it); 
					CMFeatureSelectionItem_* cm_input = new CMFeatureSelectionItem_(fs_item);
					disconnected_items_->addItem(cm_input);
				}
				else if (qgraphicsitem_cast<ValidationItem *>(*it))
				{
					ValidationItem* val_item = qgraphicsitem_cast<ValidationItem *>(*it); 
					CMValidationItem_* cm_val = new CMValidationItem_(val_item);
					disconnected_items_->addItem(cm_val);
				}
				else if (qgraphicsitem_cast<PredictionItem *>(*it))
				{
					PredictionItem* pred_item = qgraphicsitem_cast<PredictionItem *>(*it); 
					CMPredictionItem_* cm_pred = new CMPredictionItem_(pred_item);
					disconnected_items_->addItem(cm_pred);
				}	
			}
		}

		void ConnectionManager::connectItem(QListWidgetItem* item)
		{
			Q_UNUSED(item);
		}


		void ConnectionManager::disconnectItem(QListWidgetItem* item)
		{
			Q_UNUSED(item);
		}

		//###########################

		ConnectionManager::CMInputItem_::CMInputItem_(InputDataItem* input):
		QListWidgetItem(),
		input_(input)
		{
			setText(input->name());
			setIcon(QIcon(input->pixmap()));
		}

		ConnectionManager::CMInputItem_::~CMInputItem_()
		{
		}

		//###########################

		ConnectionManager::CMFeatureSelectionItem_::CMFeatureSelectionItem_(FeatureSelectionItem* feature_selection):
		QListWidgetItem(),
		feature_selection_(feature_selection)
		{
			setText(feature_selection->name());
			setIcon(QIcon(feature_selection->pixmap()));
		}

		ConnectionManager::CMFeatureSelectionItem_::~CMFeatureSelectionItem_()
		{
		}

		//###########################

		ConnectionManager::CMValidationItem_::CMValidationItem_(ValidationItem* validation):
		QListWidgetItem(),
		validation_(validation)
		{
			setText(validation->name());
			setIcon(QIcon(validation->pixmap()));
		}

		ConnectionManager::CMValidationItem_::~CMValidationItem_()
		{
		}

		//###########################

		ConnectionManager::CMPredictionItem_::CMPredictionItem_(PredictionItem* prediction):
		QListWidgetItem(),
		prediction_(prediction)
		{
			setText(prediction->name());
			setIcon(QIcon(prediction->pixmap()));
		}

		ConnectionManager::CMPredictionItem_::~CMPredictionItem_()
		{
		}
	}
}