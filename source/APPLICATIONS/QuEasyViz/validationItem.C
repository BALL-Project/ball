/* validationItem.C
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

#include <validationItem.h>
#include <exception.h>
#include <mainWindow.h>
#include <validationResultDialog.h>
#include <BALL/QSAR/configIO.h>

#include <QtGui/QDrag>
#include <QtCore/QMimeData>

#include <BALL/MATHS/LINALG/matrix.h>
#include <BALL/VIEW/KERNEL/iconLoader.h>

using namespace BALL::QSAR;
using namespace BALL::Exception;

namespace BALL
{
	namespace VIEW
	{

		ValidationItem::ValidationItem(int type, DataItemView* view):
			DataItem(view),
			k_(0),
			num_of_samples_(0),
			num_of_runs_(0),
			model_item_(NULL),
			q2_(0.0),
			r2_(0.0)
		{
			nested_val_item_ = NULL;
			type_ = type;
			partitioner_ = NULL;
			validation_statistic_ = 0;
			init();	
		}

		ValidationItem::ValidationItem(ValidationItem& item):
		DataItem(item.view_)
		{
			type_ = item.type_;
			k_ = item.k_;
			num_of_samples_ = item.num_of_samples_;
			num_of_runs_ = item.num_of_runs_;
			model_item_ = item.model_item_;
			q2_ = -1;
			r2_ = -1;
			nested_val_item_ = NULL;
			partitioner_ = NULL;
			validation_statistic_ = 0;
			init();	
		}

		ValidationItem::~ValidationItem()
		{
			if (view_->name == "view")
			{
				//if the item was connected to others, delete it from its respective pipeline
				if (!removeDisconnectedItem())
				{
					removeFromPipeline();
				}
			}
			if(nested_val_item_!=NULL && view_->data_scene->main_window->val_pipeline_.contains(nested_val_item_))
			{
				nested_val_item_->external_validations_.remove(this);
				
				// one fold validation (this item) is being deleted, so that the average of the remaining predictive qualities has to calculated anew ...
				nested_val_item_->change();
			}
			delete plotter_;
		}


		ValidationItem::ValidationItem(String& configfile_section, std::map<String, DataItem*>& filenames_map, list<pair<double,double> >* item_positions, DataItemView* view)
			: DataItem(view)
		{
			result_color_ = QColor(205,225,205);
			type_ = -1;
			istringstream input;
			input.str(configfile_section);
			ValidationConfiguration conf = ConfigIO::readValidationConfiguration(&input);

			int no = conf.external_predictions.size();
			for(int i=0; i<no; i++)
			{
				String file_i = conf.external_predictions[i];
				map<String,DataItem*>::iterator it = filenames_map.find(file_i);
				if(it==filenames_map.end())
				{
					cout<<file_i<<" can not be found!!"<<endl;
					throw BALL::Exception::GeneralException(__FILE__,__LINE__,"ValidationItem reading error","PredictionItem of a nested cross validation fold could not be found!");
				}
				PredictionItem* pred_i = (PredictionItem*) it->second;
				addExternalFoldValidation(pred_i);
				
				if(i==0) // all folds of ONE validationItem for nested validation come from 
					// ONE PartitioningItem
				{
					Edge* edge = pred_i->dottedEdge();
					if(edge!=NULL)
					{
						if(edge->sourceNode()->inEdges().size()>0)
						{						
							DataItem* tmp = (*edge->sourceNode()->inEdges().begin())->sourceNode();
							if(tmp->type()==PartitioningItem::Type)
							{
								setPartitioner((PartitioningItem*)tmp);
							}
						}
					}
				}
			}
			
			
			// conf.data is not used since the inputItem connected to the modelItem is used for obtaining input data
			
			k_ = conf.k_folds;
			num_of_samples_ = conf.bootstrap_samples;
			num_of_runs_ = conf.no_of_permutation_tests;
			validation_statistic_ = conf.statistic;
			type_ = conf.val_type;
			
			map<String,DataItem*>::iterator it = filenames_map.find(conf.model);
			if(it==filenames_map.end())
			{
				throw BALL::Exception::GeneralException(__FILE__,__LINE__,"ValidationItem reading error","ModelItem for which the validation should be done can not be found!");
			}
			model_item_ = (ModelItem*) it->second;
			
			view_->data_scene->addItem(this);
			addToPipeline();
			if(item_positions!=0 && item_positions->size()>0)
			{
				pair<double,double> pos = item_positions->front();
				item_positions->pop_front();
				setPos(pos.first,pos.second);
			}
			
			Edge* edge = new Edge(model_item_, this);
			view_->data_scene->addItem(edge);

			setSavedAs(conf.output.c_str());
			
			/// if not defined in config-section explicitly, find type of validation to be done:
			if(type_==-1)
			{
				if(k_<2 && conf.external_predictions.size()>0) type_ = 5;
				else
				{
					if(k_<=0) type_ = 1;
					else if(num_of_samples_<=0) type_ = 2;
					else type_ = 3;
					if(num_of_runs_>0) type_ = 4;
				}
			}
			init();	
		}


		void ValidationItem::init()
		{
			result_color_ = QColor(205,225,205);
			initName();
			setPixmap(findPixmap("validation.png"));
			createActions();
			coeff_stderr_ratio_ = 0;
			plotter_ = NULL;
		}


		void ValidationItem::initName()
		{
			name_ = view_->data_scene->main_window->registry()->getValidationName(type_).c_str();
		}

		void ValidationItem::createActions()
		{
			if(type_>2 && type_!=7) // for other types, redoing the validation makes no sense, since that would always yield the same result
			{	
				QAction* redo_action = new QAction("Redo", this);
				connect(redo_action, SIGNAL(triggered()), this, SLOT(changeSlot()));	
				context_menu_actions_.push_back(redo_action);
			}
			QAction* show_values = new QAction("Show predictions", this);
			connect(show_values, SIGNAL(triggered()), this, SLOT(showPredictionDialog()));
			context_menu_actions_.push_back(show_values);
			
			if(type_==5)
			{
				QAction* show_plotter = new QAction(QIcon(""),tr("Plot all predictions"), this);
				connect(show_plotter, SIGNAL(triggered()), this, SLOT(showPlotter()));
				context_menu_actions_.push_back(show_plotter);
			}
		}


		void ValidationItem::showPlotter()
		{
			if(plotter_ == NULL)
			{
				plotter_=new PredictionPlotter(this);
			}
			plotter_->show();
		}

		void ValidationItem::changeSlot()
		{
			change();
			if(partitioner_!=NULL) partitioner_->change();
			view_->data_scene->update();
		}

		void ValidationItem::change()
		{
			DataItem::change();
			delete plotter_; 
			plotter_=NULL;
		}

		void ValidationItem::setPartitioner(PartitioningItem* partitioner)
		{
			partitioner_ = partitioner;
		}

		// overloaded by PredictionItem 
		void ValidationItem::setValidationInput()
		{
			model_item_->model()->setDataSource(model_item_->inputDataItem()->data());
		}


		bool ValidationItem::execute()
		{
			if (model_item_ == NULL)
			{
				throw InvalidValidationItem(__FILE__,__LINE__);
			}
			
			if(done_) return 0; // do nothing twice...
			
			setValidationInput();

			if(validation_statistic_>=0)
			{
				model_item_->model()->model_val->selectStat(validation_statistic_);
			}
			switch(type_)
			{
				case 1:	
					model_item_->model()->model_val->testInputData();
					break;
				case 2:
					model_item_->model()->model_val->crossValidation(k_);
					break;
				case 3:	
					model_item_->model()->model_val->bootstrap(num_of_samples_);
					break;
				case 4:	
					result_of_rand_test_ = model_item_->model()->model_val->yRandomizationTest(num_of_runs_, k_);		
					break;
				case 5: 
					q2_ = 0;
					for(list<ValidationItem*>::iterator it=external_validations_.begin(); it!=external_validations_.end(); it++)
					{
						// if one prediction is not yet ready, stop calc. average quality
						if(!(*it)->isDone()) return 0;
						q2_ += (*it)->getQ2();
					}
					q2_ /= external_validations_.size(); // average Q^2 obtained from nested cross validation			
					break; 
				
				case 6:
					if(!model_item_->getRegistryEntry()->regression)
					{
						throw BALL::Exception::GeneralException(__FILE__,__LINE__,"validation error","coefficient stddev can only be calculated for regression models!");
					}		
					((RegressionModel*)model_item_->model())->validation->calculateCoefficientStdErrors(num_of_samples_,1);
					break;	
					
				case 7: 
					model_item_->model()->model_val->testInputData(1);
					break;
				
				default:
					throw InvalidValidationItem(__FILE__,__LINE__);
			}
			
			processResults();
			done_ = 1;
			return 1;
		}


		void ValidationItem::processResults()
		{
			if(type_<5 || type_==7)
			{
				r2_ = model_item_->model()->model_val->getFitRes();
				q2_ = model_item_->model()->model_val->getCVRes();
			}

			if(type_==1||type_==7) setResultString(r2_);
			else if(type_==6)
			{
				coeff_stddev_ = *((RegressionModel*)model_item_->model())->validation->getCoefficientStdErrors();
				const Matrix<double>* training_result = ((RegressionModel*)model_item_->model())->getTrainingResult();
				
				// calculate&display average stddev
				double mean_stddev=0;
				Size rows=coeff_stddev_.getRowCount();
				Size cols=coeff_stddev_.getColumnCount();

				double epsilon = std::numeric_limits<double>::epsilon();

				if(training_result->getColumnCount()==cols && training_result->getRowCount()==rows)
				{
					for(uint i=1; i<=rows;i++) // for each feature
					{
						for(uint j=1; j<=cols;j++) // for each activity
						{
							double t_ij = (*training_result)(i,j);
							double s_ij = coeff_stddev_(i,j);
							
							if(abs(t_ij) > epsilon && abs(s_ij) > epsilon)
							{
								cout<<s_ij/t_ij;
								mean_stddev += abs(s_ij/t_ij);
							}
							cout<<endl;
						}
					}
					coeff_stderr_ratio_ = mean_stddev/(rows*cols);
					setResultString(coeff_stderr_ratio_);
				}
			}
			else if(type_==4)
			{
				double mean_q2=0;
				for(uint i=1;i<=result_of_rand_test_.getRowCount();i++)
				{
					mean_q2+=result_of_rand_test_(i,2);
				}
				mean_q2/=result_of_rand_test_.getRowCount();
				setResultString(mean_q2);
			}
			else setResultString(q2_);
		}

			
		BALL::String ValidationItem::getStatName()
		{
			String name="";
			if(validation_statistic_==-1) return name;
			
			return model_item_->getRegistryEntry()->getStatName(validation_statistic_);
		}

		void ValidationItem::setModelItem(ModelItem* model_item)
		{
			model_item_ = model_item;
		}

		int ValidationItem::getValidationType()
		{
			return type_;
		}

		void ValidationItem::setValFraction(double frac)
		{
			val_fraction_ = frac;
		}

		double ValidationItem::getValFraction()
		{
			return val_fraction_;	
		}

		void ValidationItem::setK(int k)
		{
			k_ = k;
		}
		void ValidationItem::setNumOfRuns(int num)
		{
			num_of_runs_ = num;
		}

		void ValidationItem::setNumOfSamples(int num)
		{
			num_of_samples_ = num;
		}

		void ValidationItem::setNumOfNCVFolds(int num)
		{
			n_of_ncv_folds_ = num;
		}

		ModelItem* ValidationItem::modelItem()
		{
			return model_item_;
		}

		double ValidationItem::getQ2()
		{
			return q2_;
		}

		double ValidationItem::getR2()
		{
			return r2_;
		}

		int ValidationItem::numOfRuns()
		{
			return num_of_runs_;
		}

		int ValidationItem::numOfSamples()
		{
			return num_of_samples_;
		}

		int ValidationItem::numOfNCVFolds()
		{
			return  n_of_ncv_folds_;
		}

		int ValidationItem::k()
		{
			return k_;
		}

		BALL::Matrix<double>* ValidationItem::resultOfRandTest()
		{
			return &result_of_rand_test_;
		}

		// SLOT
		void ValidationItem::showPredictionDialog()
		{
			ValidationResultDialog validationResultDialog(this);
			validationResultDialog.exec();
		}

		void ValidationItem::writeConfigSection(ofstream& out)
		{
			out << "[Validator]" << "\n";
			out << "model_file = "<< modelItem()->savedAs().toStdString() << "\n";
			out << "data_file = "<< modelItem()->inputDataItem()->savedAs().toStdString() << "\n";
			out << "validation_type = "<<type_<<endl;
			
			// for nested cross validation save the names of the prediction output-files in order to be able to restore the pipeline later
			if(external_validations_.size()>0)
			{
				out<<"external_validation_predictions = ";
				for(list<ValidationItem*>::iterator it=external_validations_.begin(); it!=external_validations_.end(); it++)
				{
					out<<(*it)->savedAs().toStdString()<<" ";
				}
				out<<endl;
			}
			int s = getValidationStatistic();
			if(s>=0)
			{
				String stat = modelItem()->getRegistryEntry()->getStatName(s);
				if(!model_item_->getRegistryEntry()->regression) out<< "classification_statistic = "<<stat.c_str()<<endl;
				else out<< "regression_statistic = "<<stat.c_str()<<endl;
			}
			
			if(type_>0) out << "k_fold = "<< k() <<  "\n";
			if(num_of_samples_>0) out << "bootstrap_samples = "<< num_of_samples_ << "\n";
			if(num_of_runs_>0) out << "no_of_permutation_tests = " <<  num_of_runs_ << "\n";
			out << "output = " << savedAs().toStdString() << "\n\n";	
		}


		void ValidationItem::addToPipeline()
		{
			view_->data_scene->main_window->val_pipeline_.insert(this);
			view_->data_scene->main_window->all_items_pipeline_.insert(this);
		}

		void ValidationItem::removeFromPipeline()
		{
			view_->data_scene->main_window->val_pipeline_.erase(this);
			view_->data_scene->main_window->all_items_pipeline_.erase(this);
		}

		void ValidationItem::addExternalFoldValidation(ValidationItem* item)
		{
			external_validations_.push_back(item);
			item->nested_val_item_ = this;
		}

		int ValidationItem::getNoExternalFolds()
		{
			return external_validations_.size();
		}

		BALL::String ValidationItem::getMouseOverText()
		{
			String message="";
			if(view_->name!="view") return message;
			
			if(type_!=6)
			{
				message+="using ";
				message += modelItem()->getRegistryEntry()->getStatName(validation_statistic_);
			}
			else if(done_ && type_==6)
			{
				message+="average ratio between stddev\nand coeff: "+String(coeff_stderr_ratio_);
			}
			return message;				
		}


		void ValidationItem::saveToFile(String filename)
		{
			if(model_item_->getRegistryEntry()->regression)
			{
				((RegressionModel*)model_item_->model())->validation->saveToFile(filename,r2_,q2_,coeff_stddev_,result_of_rand_test_);
			}
			else
			{
				((ClassificationModel*)model_item_->model())->validation->saveToFile(filename,r2_,q2_);
			}
		}

		void ValidationItem::loadFromFile(String filename)
		{
			try
			{
				model_item_->model()->model_val->readFromFile(filename);
			}
			catch(BALL::Exception::GeneralException e)
			{
				QMessageBox::warning(view_,"Error",e.getMessage());
				return;
			}
			r2_ = model_item_->model()->model_val->getFitRes();
			q2_ = model_item_->model()->model_val->getCVRes();
			result_of_rand_test_ = model_item_->model()->model_val->getYRandResults();
			
			if(r2_!=-1 || q2_!=-1) // if some result was loaded
			{
				processResults();
				done_=1;
			}
		}
	}
}
