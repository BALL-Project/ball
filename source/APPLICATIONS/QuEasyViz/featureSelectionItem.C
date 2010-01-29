/* featureSelectionItem.C
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

#include <BALL/QSAR/exception.h>

#include <exception.h>
#include <featureSelectionItem.h>
#include <mainWindow.h>
#include <BALL/QSAR/configIO.h>

#include <QtGui/QDrag>
#include <QtCore/QMimeData>

using namespace BALL::QSAR;

namespace BALL
{
	namespace VIEW
	{

		FeatureSelectionItem::FeatureSelectionItem(int type, DataItemView* miv):
			DataItem(miv)
		{
			String dir = view_->data_scene->main_window->getImageDirectory();
			setPixmap(QPixmap((dir+"feature_selection.png").c_str()).scaled(QSize(width(), height()), Qt::KeepAspectRatio,Qt::FastTransformation ));

			feature_selection_ = NULL;
			validation_statistic_ = 0;
			done_= 0;
			k_ = 0;
			cor_threshold_ = 0;
			quality_increase_cutoff_ = -1;
			opt_ = 0;
			post_optimization_model_par_ = 0;
			post_optimization_kernel_par_ = 0;

			type_ = type;
			name_ = view_->data_scene->main_window->registry()->getFeatureSelectionName(type_).c_str();
		}

		FeatureSelectionItem::~FeatureSelectionItem()
		{	
			if (view_ && view_->name == "view")
			{
				//if the item was connected to others, delete it from its respective pipeline
				if (!removeDisconnectedItem())
				{
					removeFromPipeline();
				}
			}
		}

		FeatureSelectionItem::FeatureSelectionItem(FeatureSelectionItem& fs_item):
		DataItem(fs_item.view_)
		{
			type_ = fs_item.type_;
			k_ = fs_item.k_;
			opt_ = fs_item.opt_;
			model_item_ = NULL;
			name_ = fs_item.name_;
			setPixmap(fs_item.pixmap());
			validation_statistic_ = fs_item.validation_statistic_;
			done_ = 0;
			result_ = "";
			quality_increase_cutoff_ = fs_item.quality_increase_cutoff_;
			cor_threshold_ = fs_item.cor_threshold_;
			feature_selection_ = NULL;
			post_optimization_model_par_ = fs_item.post_optimization_model_par_;
			post_optimization_kernel_par_ = fs_item.post_optimization_kernel_par_;
		}


		FeatureSelectionItem::FeatureSelectionItem(String& configfile_section, std::map<String, DataItem*>& filenames_map, list<pair<double,double> >* item_positions, DataItemView* view)
			: DataItem(view)
		{
			istringstream input;
			input.str(configfile_section);
			FeatureSelectionConfiguration conf = ConfigIO::readFeatureSelectionConfiguration(&input);
			
			map<String,DataItem*>::iterator it = filenames_map.find(conf.model);
			if(it==filenames_map.end())
			{
				String m = "ModelItem \"";
				m += conf.model+"\" to which the feature selection should be applied can not be found!";
				throw BALL::Exception::GeneralException(__FILE__,__LINE__,"Feature selection reading error",m.c_str());
			}
			setInputModelItem((ModelItem*) it->second);	
			model_item_ = new ModelItem(*input_model_item_);
			model_item_->setInputDataItem(input_model_item_->inputDataItem());
			
			// set feature selection parameters
			type_ = conf.feat_type;
			validation_statistic_ = conf.statistic;
			quality_increase_cutoff_ = conf.quality_increase_cutoff;
			cor_threshold_ = conf.cor_threshold;
			k_ = conf.k_fold;
			name_ = conf.selection_name.c_str();
			opt_ = conf.opt;
			addToPipeline();
			
			// model-/kernel-parameter optimization parameters
			post_optimization_model_par_ = conf.opt_model_after_fs;
			post_optimization_kernel_par_ = conf.opt_kernel_after_fs;
			model_item_->k_fold = conf.opt_k_fold;
			model_item_->grid_search_steps = conf.grid_search_steps;
			model_item_->grid_search_stepwidth = conf.grid_search_stepwidth;
			model_item_->grid_search_recursions = conf.grid_search_recursions;
			
			// configure output model-item and add it to scene
			model_item_->setSaveAttribute(false);
			model_item_->setSavedAs(conf.output.c_str());
			view_->data_scene->addItem(model_item_);
			model_item_->addToPipeline();
			
			feature_selection_ = new FeatureSelection(*model_item_->model());
			if(quality_increase_cutoff_!=-1)
			{
				feature_selection_->setQualityIncreaseCutoff(quality_increase_cutoff_);
			}
			
			view_->data_scene->addItem(this);
			if(item_positions!=0 && item_positions->size()>0)
			{
				pair<double,double> pos = item_positions->front();
				item_positions->pop_front();
				setPos(pos.first,pos.second);
				if(item_positions->size()>0)
				{
					pair<double,double> pos = item_positions->front();
					item_positions->pop_front();
					model_item_->setPos(pos.first,pos.second);
				}
			}		
			Edge* edge1 = new Edge(input_model_item_, this);
			Edge* edge2 = new Edge(this, model_item_);
			view_->data_scene->addItem(edge1);
			view_->data_scene->addItem(edge2);
			
			setSavedAs(conf.output.c_str());
			filenames_map.insert(make_pair(conf.output,model_item_));
			String dir = view_->data_scene->main_window->getImageDirectory();
			setPixmap(QPixmap((dir+"feature_selection.png").c_str()).scaled(QSize(width(), height()), Qt::KeepAspectRatio,Qt::FastTransformation ));
			
			done_ = 0;
		}

		bool FeatureSelectionItem::execute()
		{
			if (model_item_ == NULL)
			{
				throw BALL::Exception::InvalidFeatureSelectionItem(__FILE__,__LINE__);
			}
			
			if(done_) return 0; // do nothing twice...
			
			/// set input data source of this model and copy the descriptor IDs from the previous model. The latter is important if there are several successive feature selections to be done
			model_item_->setInputDataItem(input_model_item_->inputDataItem());
			model_item_->model()->copyDescriptorIDs(*input_model_item_->model());
			
			delete feature_selection_;
			feature_selection_ = new FeatureSelection(*(model_item_->model()));
			if(validation_statistic_>=0)
			{
				feature_selection_->selectStat(validation_statistic_);
			}
			
			feature_selection_->setQualityIncreaseCutoff(quality_increase_cutoff_);
			
			switch(type_)
			{
				case 0:
					feature_selection_->removeHighlyCorrelatedFeatures(cor_threshold_);
					break;		
				case 1:	
					feature_selection_->forwardSelection(k_, opt_);
					break;
				case 2:
					feature_selection_->backwardSelection(k_, opt_);
					break;
				case 3:					
					feature_selection_->stepwiseSelection(k_, opt_);
					break;
				case 4:					
					feature_selection_->removeLowResponseCorrelation(cor_threshold_);
					break;
					
				case 5:
				{
					LinearModel* lm = ((LinearModel*)model_item_->model());
					LinearModel* in_lm = ((LinearModel*)input_model_item_->model());
					lm = in_lm; // copy training result !
					lm->validation->calculateCoefficientStdErrors(k_,1);
					//lm->validation->setCoefficientStddev(in_lm->validation->getCoefficientStddev());
					//cout<<*lm->validation->getCoefficientStdErrors()<<endl;
					feature_selection_->implicitSelection(*lm,1,cor_threshold_);
					break;
				}
				
				case 6:		
					feature_selection_->twinScan(k_,opt_);
					break;
					
				default:
					throw BALL::Exception::InvalidFeatureSelectionItem(__FILE__,__LINE__);
					break;
			}

			if(post_optimization_model_par_) model_item_->optimizeModelParameters();
			if(post_optimization_kernel_par_) model_item_->optimizeKernelParameters();

			model_item_->model()->readTrainingData();
			model_item_->model()->train();
			model_item_->setResultString((int)model_item_->model()->getDescriptorIDs()->size());

			model_item_->setDone(1); // set done_ to 1, so that model will be saved to file when exporting the pipeline
			done_ = 1; // ready!
			return 1;
		}


		void FeatureSelectionItem::change()
		{
			if(model_item_ && input_model_item_)
			{
				model_item_->setInputDataItem(input_model_item_->inputDataItem());
			}
			DataItem::change();
		}


		int FeatureSelectionItem::numberOfFeatures()
		{
			return model_item_->model()->getDescriptorIDs()->size();
		}


		int FeatureSelectionItem::getType()
		{
			return type_;
		}

		FeatureSelection* FeatureSelectionItem::featureSelection()
		{
			return feature_selection_;
		}

		void FeatureSelectionItem::setK(int k)
		{
			k_ = k;
		}

		int FeatureSelectionItem::k()
		{
			return k_;
		}

		void FeatureSelectionItem::setOpt(bool opt)
		{
			opt_ = opt;
		}

		void FeatureSelectionItem::setModelItem(ModelItem* model_item)
		{
			model_item_ = model_item;
		}

		void FeatureSelectionItem::setInputModelItem(ModelItem* model_item)
		{
			input_model_item_ = model_item;	
		}

		bool FeatureSelectionItem::opt()
		{
			return opt_;
		}

		ModelItem* FeatureSelectionItem::modelItem()
		{
			return model_item_;
		}

		ModelItem* FeatureSelectionItem::inputModelItem()
		{
			return input_model_item_;
		}


		void FeatureSelectionItem::writeConfigSection(ofstream& out)
		{
			out << "[FeatureSelector]" << "\n";
			if(done_) out<<"done = "<<done_<<endl;
			out << "model_file = "<< inputModelItem()->savedAs().toStdString() << "\n";
			out << "data_file = "<< inputModelItem()->inputDataItem()->savedAs().toStdString() << "\n";
			out << "feature_selection_type = "<< getType() <<  "\n";
			if(getType()>0 && getType()!=4)
			{
				int s = getValidationStatistic();
				if(s>=0)
				{
					String stat = modelItem()->getRegistryEntry()->getStatName(s);
					if(!model_item_->getRegistryEntry()->regression) out<< "classification_statistic = "<<stat.c_str()<<endl;
					else out<< "regression_statistic = "<<stat.c_str()<<endl;
				}
				out << "k_fold = "<< k() <<  "\n";
				out<<"quality_increase_cutoff = "<<quality_increase_cutoff_<<"\n";
			}
			if(type_==0||type_==4||type_==5)
			{
				out<<"cor_threshold = "<<getCorThreshold()<<endl;
			}
			
			if(opt_) out << "optimize_parameters = " << opt() << "\n";
			
			bool b=0;
			if(post_optimization_model_par_) 
			{
				out << "opt_model_par_after_fs = "<< 1 << "\n";
				out << "opt_k_fold = "<<model_item_->k_fold<< "\n";
				b=1;
			}
			if(post_optimization_kernel_par_)
			{
				out << "opt_kernel_par_after_fs = "<< 1 << "\n";
				out << "grid_search_steps = "<<model_item_->grid_search_steps << "\n";
				out << "grid_search_recursions = "<<model_item_->grid_search_recursions << "\n";
				out << "grid_search_stepwidth = " <<model_item_->grid_search_stepwidth << "\n";
				if(!b) out << "opt_k_fold = "<<model_item_->k_fold << "\n";
			}
			out << "output = " << modelItem()->savedAs().toStdString() << "\n";
			out << "\n";
		}

		void FeatureSelectionItem::addToPipeline()
		{
			view_->data_scene->main_window->fs_pipeline_.insert(this);
			view_->data_scene->main_window->all_items_pipeline_.insert(this);
		}

		void FeatureSelectionItem::removeFromPipeline()
		{
			view_->data_scene->main_window->fs_pipeline_.erase(this);
			view_->data_scene->main_window->all_items_pipeline_.erase(this);
		}

		void FeatureSelectionItem::setQualityIncreaseCutoff(double cutoff)
		{
			quality_increase_cutoff_ = cutoff;
		}

		BALL::String FeatureSelectionItem::getMouseOverText()
		{
			String message="";
			if(view_->name!="view") return message;
			
			if(type_==0) message="maximal desired correlation\nbetween features="+valueToString(cor_threshold_);
				
 			else if(type_<4||type_==6)
			{
		// 			if(type_==1) message="forward feature selection\n";
		// 			if(type_==3) message="backward feature selection\n";
		// 			if(type_==2) message="stepwise feature selection\n";
				message="using "+valueToString(k_)+"-fold cross validation\n";
				message+="  and ";
				String stat = modelItem()->getRegistryEntry()->getStatName(validation_statistic_);
				message+=stat+"\n";	
				message+="quality increase cutoff="+valueToString(quality_increase_cutoff_);
			}
			else if(type_==4) message="minimal correlation between\neach feature and response="+valueToString(cor_threshold_);
			else if(type_==5) message="Remove each feature whose absolut coefficient value\nis smaller than d times its standard deviation\nd="+valueToString(cor_threshold_);
			return message;		
		}
	}
}