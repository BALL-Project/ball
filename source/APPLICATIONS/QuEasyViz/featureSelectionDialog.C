/* featureSelectionDialog.C
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

#include <featureSelectionDialog.h>
#include <iostream>
#include <BALL/QSAR/exception.h>
#include <exception.h>
#include <mainWindow.h>
#include <QtGui/QMessageBox>
#include <QtGui/QPushButton>
#include <iterator>

using namespace BALL::QSAR;

namespace BALL
{
	namespace VIEW
	{

		FeatureSelectionDialog::FeatureSelectionDialog(FeatureSelectionItem* fsitem, ModelItem* model):
			fs_item_(fsitem)
		{
			Registry* reg = model->getRegistryEntry()->getRegistry();
			
			QVBoxLayout* main_layout = new QVBoxLayout(this);
			QHBoxLayout* layout1 = new QHBoxLayout;
			QHBoxLayout* layout2 = NULL;
			QDialogButtonBox* buttons = new QDialogButtonBox(QDialogButtonBox::Cancel);
			QPushButton* applyButton = new QPushButton("OK");
			buttons->addButton(applyButton, QDialogButtonBox::ApplyRole);
			checkbox_post_optimization_model_par_ =0;
			checkbox_post_optimization_kernel_par_=0;
			cutoff_ = 0;
			edit_ = new QLineEdit(this);
			edit2_ = NULL;
			
			if((fsitem->getType()>0&&fsitem->getType()<4) || fsitem->getType()==6)
			{
				edit_->setText(String(reg->default_k).c_str());	
				QLabel* klabel = new QLabel("k for k-fold cross validation",this);
				
				QHBoxLayout* cutoff_layout = new QHBoxLayout;
				QLabel* cutoff_label = new QLabel("quality increase cutoff");
				cutoff_ = new QLineEdit(this);
				cutoff_->setText("0.001");
				cutoff_layout->addWidget(cutoff_label);
				cutoff_layout->addWidget(cutoff_);
				
				layout1->addWidget(klabel);
				layout1->addWidget(edit_);
			
				//QHBoxLayout* layout2 = new QHBoxLayout();
				//layout2->addWidget(checkbox_post_optimization_model_par_);
				//layout2->addWidget(checkbox_post_optimization_kernel_par_);
				
				main_layout->addLayout(layout1);
				main_layout->addLayout(cutoff_layout);
				statistic_box_ = NULL;
				
				// let user select validation statistic
				QHBoxLayout* layout3 = new QHBoxLayout;
				QLabel* label3 = new QLabel("quality statistic");
				statistic_box_ = new QComboBox;
				
				const std::map<unsigned int,String>* statistics = model->getRegistryEntry()->getStatistics();
				for(std::map<unsigned int,String>::const_iterator it=statistics->begin(); it!=statistics->end(); ++it)
				{
					statistic_box_->addItem(it->second.c_str(),it->first);
				}

				if(statistics->size()>2) statistic_box_->setCurrentIndex(2); // use overall ACC as default
				layout3->addWidget(label3);layout3->addWidget(statistic_box_);
				main_layout->addLayout(layout3);
				
			}
			else   // no validation statistics for removal of colineal features
			{
				double cor = reg->default_correlation_cutoff;
				edit_->setText(String(cor).c_str());
				QLabel* label = NULL;
				if(fsitem->getType()==0) label = new QLabel("max correlation between features");
				else if(fsitem->getType()==4)
				{
					label = new QLabel("min correlation with response");
					edit_->setText(String(0.15).c_str());			
				}
				else if(fsitem->getType()==5) 
				{
					QLabel* description = new QLabel("Remove each feature whose absolut coefficient value\nis smaller than d times its standard deviation.");
					label = new QLabel("d = ",this);
					edit_->setText("2");
					main_layout->addWidget(description);
					edit2_ = new QLineEdit(this);
					QString t;
					t.setNum(model->view()->data_scene->main_window->registry()->default_no_boostrap_samples);
					edit2_->setText(t);
					QLabel* label2 = new QLabel("number of bootstrap samples",this);
					layout2 = new QHBoxLayout;
					layout2->addWidget(label2);
					layout2->addWidget(edit2_);
				}
			
				layout1->addWidget(label);
				layout1->addWidget(edit_);		
				main_layout->addLayout(layout1);
				if(layout2!=NULL) main_layout->addLayout(layout2);
			}
			
			checkbox_post_optimization_model_par_ = new QCheckBox("optimize model parameters afterwards", this);
			if(!model->optimize_model_parameters || model->k_fold<2)
			{
				checkbox_post_optimization_model_par_->setEnabled(0);
			}
			checkbox_post_optimization_kernel_par_ = new QCheckBox("optimize kernel parameters afterwards", this);
			if(!model->optimize_kernel_parameters || model->k_fold<2)
			{
				checkbox_post_optimization_kernel_par_->setEnabled(0);
			}
			main_layout->addWidget(checkbox_post_optimization_model_par_);
			main_layout->addWidget(checkbox_post_optimization_kernel_par_);
			
			main_layout->addWidget(buttons);
			setLayout(main_layout);
			setWindowTitle(fs_item_->name());

			connect(applyButton, SIGNAL(clicked()), this, SLOT(accept()));
			connect(buttons, SIGNAL(rejected()), this, SLOT(reject()));
		}


		FeatureSelectionDialog::FeatureSelectionDialog():
			edit_(NULL)
		{
			post_optimization_model_par_ = 0;
			post_optimization_kernel_par_ = 0;
		}

		FeatureSelectionDialog::~FeatureSelectionDialog()
		{
 			delete edit_;
 			delete checkbox_post_optimization_model_par_;
			delete checkbox_post_optimization_kernel_par_;
		}	

		void FeatureSelectionDialog::applyInput()
		{
			bool ok = 0;
			post_optimization_model_par_ = checkbox_post_optimization_model_par_->isChecked();
			post_optimization_kernel_par_ = checkbox_post_optimization_kernel_par_->isChecked();
			fs_item_->post_optimization_model_par_ = post_optimization_model_par_;
			fs_item_->post_optimization_kernel_par_ = post_optimization_kernel_par_;
			
			if((fs_item_->getType()>0&&fs_item_->getType()<4)||fs_item_->getType()==6) // no validation statistics for removal of colineal features
			{
				k_ =  edit_->text().toInt(&ok);
				fs_item_->setK(k_);
				
				statistic_ = -1;
				if(statistic_box_!=NULL)
				{
					statistic_ = statistic_box_->currentIndex();
				}
				if(cutoff_!=NULL)
				{
					fs_item_->setQualityIncreaseCutoff(cutoff_->text().toDouble(&ok));
				}
			}
			else
			{
				fs_item_->cor_threshold_=edit_->text().toDouble(&ok);
				statistic_ = -1;
				if(fs_item_->getType()==5) fs_item_->setK(edit2_->text().toInt(&ok));
				else k_ = 0;
			}
		}

		int FeatureSelectionDialog::k()
		{
			return k_;
		}
	}
}