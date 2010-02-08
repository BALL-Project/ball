/* modelItem.C
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

#include <modelItem.h>
#include <QtGui/QDrag>
#include <QtCore/QMimeData>
#include <mainWindow.h>
#include <BALL/QSAR/exception.h>
#include <exception.h>
#include <QtGui/QMessageBox>
#include <QtGui/QFileDialog>
#include <BALL/QSAR/configIO.h>
#include <coefficientPlotter.h>
#include <bayesPlotter.h>
#include <featurePlotter.h>
#include <componentPlotter.h>

using namespace BALL::QSAR;
using namespace BALL::Exception;
using namespace BALL::QSAR::Exception;

namespace BALL
{
	namespace VIEW
	{

		ModelItem::ModelItem(RegistryEntry* entry,  DataItemView* miv):
			DataItem(miv),
			kernel_function_type(0),
			kernel_parameter1(0.),
			kernel_parameter2(0.),
			optimize_model_parameters(false),
			optimize_kernel_parameters(false),
			grid_search_stepwidth(0.0),
			grid_search_steps(0),
			grid_search_recursions(0),
			k_fold(0),
			input_(NULL),
			model_(NULL), 
			entry_(entry),
			save_attribute_(true)
		{
			no_training_ = 0;
			setName(QString(entry_->name_abreviation.c_str()));
			init();
		}

		ModelItem::ModelItem(InputDataItem* inputdata, RegistryEntry* entry, DataItemView* miv):
			DataItem(miv),	
			kernel_function_type(0),
			kernel_parameter1(0),
			kernel_parameter2(0),
			optimize_model_parameters(false),
			optimize_kernel_parameters(false),
			grid_search_stepwidth(0.0),
			grid_search_steps(0),
			grid_search_recursions(0),
			k_fold(0),
			input_(inputdata),
			entry_(entry),
			save_attribute_(true)
		{
			no_training_ = 0;
			
			if(!entry_->kernel)
			{
				if (input_ != NULL)
				{
					model_ = (*entry_->create)(*input_->data());
				}
				else
				{
					QSARData q;
					model_ = (*entry_->create)(q);
				}
			}
			else
			{
				throw BALL::Exception::GeneralException(__FILE__,__LINE__,"Model creation error","Kernel type and kernel parameters must be specified for creation of a kernel based model!");
			}
			setName(QString(entry_->name_abreviation.c_str()));
			init();
			
		}

		ModelItem::ModelItem(InputDataItem* inputdata, RegistryEntry* entry, int kernelType, double parameter1, double parameter2, DataItemView* miv): 
			DataItem(miv),
			kernel_function_type(kernelType),
			kernel_parameter1(parameter1),
			kernel_parameter2(parameter2),
			optimize_model_parameters(false),
			optimize_kernel_parameters(false),
			grid_search_stepwidth(0.0),
			grid_search_steps(0),
			grid_search_recursions(0),
			k_fold(0),
			input_(inputdata),
			entry_(entry),
			save_attribute_(true)
		{
			no_training_ = 0;
			
			if(entry_->kernel && kernelType < 4)
			{
				if (input_ != NULL)
				{
					model_ = entry_->createKernel1(*input_->data(), kernel_function_type, kernel_parameter1, kernel_parameter2);
				}
				else
				{
					QSARData q;
					model_ = entry_->createKernel1(q, kernel_function_type, kernel_parameter1, kernel_parameter2);
				}
			}
			else
			{
				if(kernelType<4)
				throw BALL::Exception::GeneralException(__FILE__,__LINE__,"KernelModel creation error","The desired model is no KernelModel!");
				else throw BALL::Exception::GeneralException(__FILE__,__LINE__,"KernelModel creation error","Given kernel-type unknown!");
			}
			setName(QString(entry_->name_abreviation.c_str()));
			init();
		}

		ModelItem::ModelItem(InputDataItem* inputdata, RegistryEntry* entry, String s1, String s2,DataItemView* miv):
			DataItem(miv),
			kernel_function_type(4),
			kernel_parameter1(0.),
			kernel_parameter2(0.),
			optimize_model_parameters(false),
			optimize_kernel_parameters(false),
			grid_search_stepwidth(0.0),
			grid_search_steps(0),
			grid_search_recursions(0),
			k_fold(0),
			input_(inputdata),
			entry_(entry),
			save_attribute_(true)
		{
			no_training_ = 0;
			
			if(entry_->kernel)
			{
				if (input_ != NULL)
				{
				model_ = entry_->createKernel2(*input_->data(),s1, s2);
				}
				else 
				{
					QSARData q;
					model_ = entry_->createKernel2(q,s1, s2);
				}
			}
			else
			{
				throw BALL::Exception::GeneralException(__FILE__,__LINE__,"KernelModel creation error","The desired model is no KernelModel!");
			}
			
			setName(QString(entry_->name_abreviation.c_str()));
			init();
		}

		ModelItem::ModelItem(ModelItem& item):
		DataItem(item.view_)
		{
			result_color_ = item.result_color_;
			view_ = item.view_;
			name_ = item.name_;
			input_ = NULL;
			entry_ = item.entry_;
			
			// if the new ModelItem is created for a feature selection, the following parameters are ignored (since this model will be trained by the FeatureSelectionItem)
			// if the new ModelItem is created for a Trainings-Partition, the parameters are used to optimize model- and kernel-parameter is the same way as the source model
			kernel_function_type = item.kernel_function_type;
			kernel_parameter1 = item.kernel_parameter1;
			kernel_parameter2 = item.kernel_parameter2;
			model_parameters = item.model_parameters;
			optimize_model_parameters = item.optimize_model_parameters;
			optimize_kernel_parameters = item.optimize_kernel_parameters;
			grid_search_stepwidth = item.grid_search_stepwidth;
			grid_search_steps = item.grid_search_steps;
			grid_search_recursions = item.grid_search_recursions;
			k_fold = item.k_fold;
			
			no_training_ = 0;
			save_attribute_ = item.save_attribute_;
			//prediction_input_edges_ = item.prediction_input_edges_;

			QSARData q;

			if(!entry_->kernel)
			{
				model_ = (*entry_->create)(q);
			}
			else if(entry_->kernel && kernel_function_type < 4)
			{
				model_ = entry_->createKernel1(q, kernel_function_type, kernel_parameter1, kernel_parameter2);
			}
			else
			{
				Kernel* kernel = ((KernelModel*)item.model())->kernel;
				model_ = entry_->createKernel2(q,kernel->equation1,kernel->equation2);
			}

			if (item.model_ != NULL)
			{
				*model_ = *item.model_; // copies descriptor-IDs and parameters
				model_->deleteDescriptorIDs(); // just to make sure that no descriptors are accidentally reused when copying pipelines ...
			}
			init();
		}



		ModelItem::ModelItem(String& configfile_section, std::map<String, DataItem*>& filenames_map, list<pair<double,double> >* item_positions, DataItemView* view)
			: DataItem(view)
		{
			istringstream input;
			input.str(configfile_section);
			ModelConfiguration conf = ConfigIO::readModelConfiguration(&input);
			
			map<String,DataItem*>::iterator it = filenames_map.find(conf.data_file);
			if(it==filenames_map.end())
			{
				cout<<"\""<<conf.data_file<<"\" not found!"<<endl;
				throw BALL::Exception::GeneralException(__FILE__,__LINE__,"Model reading error","InputDataItem for a model does not exist!");
			}
			input_ =  (InputDataItem*) it->second;
			Registry* reg = view_->data_scene->main_window->registry();
			entry_ = reg->getEntry(conf.model_no);
			if(!entry_->kernel)
			{
				model_ = (entry_->create)(*input_->data());
			}
			else
			{
				model_ = (entry_->createKernel1)(*input_->data(),conf.kernel_type,conf.kernel_par1,conf.kernel_par2);
			}
			
			kernel_function_type = conf.kernel_type;
			kernel_parameter1 = conf.kernel_par1;
			kernel_parameter2 = conf.kernel_par2;
			model_parameters = conf.model_parameters;
			optimize_model_parameters = conf.optimize_model_parameters;
			grid_search_stepwidth = conf.grid_search_stepwidth;
			grid_search_steps = conf.grid_search_steps;
			grid_search_recursions = conf.grid_search_recursions;
			k_fold = conf.k_fold;
			no_training_ = conf.no_training;
			if(grid_search_stepwidth>0 && grid_search_steps>0) optimize_kernel_parameters=1;
			else optimize_kernel_parameters=0;
			
			model_->setParameters(conf.model_parameters);
			init();
			
			view_->data_scene->addItem(this);
			addToPipeline();
			if(item_positions!=0 && item_positions->size()>0)
			{
				pair<double,double> pos = item_positions->front();
				item_positions->pop_front();
				setPos(pos.first,pos.second);
			}
			
			Edge* edge;
			if(conf.descriptor_source_model!="")
			{
				map<String,DataItem*>::iterator it = filenames_map.find(conf.descriptor_source_model);
				if(it==filenames_map.end())
				{
					cout<<"\""<<conf.descriptor_source_model<<"\" not found!"<<endl;
					throw BALL::Exception::GeneralException(__FILE__,__LINE__,"Model reading error","ModelItem from which the descriptor IDs are to be taken can not be found!");
					return;
				}
				descriptor_source_model_ = (ModelItem*) it->second;
				edge = new Edge(descriptor_source_model_, this);
			}
			else edge = new Edge(input_, this);
			
			view_->data_scene->addItem(edge);
			save_attribute_ = 1;
			setName(QString(entry_->name_abreviation.c_str()));
			
			filenames_map.insert(make_pair(conf.output,this));
			setSavedAs(conf.output.c_str());
		}

		void ModelItem::init()
		{
			plotter_ = NULL;
			feature_plotter_ = NULL;
			latent_variable_plotter_ = NULL;
			loading_plotter_ = NULL;
			descriptor_source_model_ = NULL;
			result_color_ = QColor(160,172,182);
			setPixmap();
			createActions();
		}



		ModelItem::~ModelItem()
		{
			if (view_->name == "view")
			{
				//if the item was connected to others, delete it from its respective pipeline
				if (!removeDisconnectedItem())
				{
					removeFromPipeline();
				}
			}
		}

		ModelItem* ModelItem::createModel(InputDataItem* inputdata)
		{
			ModelItem* item = new ModelItem(inputdata, entry_, view_);
			return item;
		}

		ModelItem* ModelItem::createModel(InputDataItem* inputdata, int kernelType, double parameter1, double parameter2)
		{
			ModelItem* item = new ModelItem(inputdata, entry_, kernelType, parameter1, parameter2,  view_);

			item->grid_search_stepwidth = grid_search_stepwidth;
			item->grid_search_steps = grid_search_steps;
			item->grid_search_recursions = grid_search_recursions;
			item->k_fold = k_fold;

			return item;
		}
		ModelItem* ModelItem::createModel(InputDataItem* inputdata, String s1, String s2)
		{
			ModelItem* item = new ModelItem(inputdata, entry_, s1, s2,  view_);
			item->grid_search_stepwidth = grid_search_stepwidth;
			item->grid_search_steps = grid_search_steps;
			item->grid_search_recursions = grid_search_recursions;
			item->k_fold = k_fold;

			return item;
		}

		void ModelItem::setModel(Model* model)
		{
			model_ = model;
		}

		void  ModelItem::setEntry(RegistryEntry* entry)
		{
			entry_ = entry;
		}

		RegistryEntry* ModelItem::getRegistryEntry()
		{
			return entry_;
		}

		Model* ModelItem::model()
		{	
			return model_;
		}

		InputDataItem* ModelItem::inputDataItem()
		{
			return input_;
		}

		void ModelItem::setInput(InputDataItem* new_input)
		{
			input_ = new_input;
		}


		bool ModelItem::execute()
		{
			if(isDone()) return 0; // do nothing twice...
			
			if(input_==0)
			{
				throw BALL::Exception::GeneralException(__FILE__,__LINE__,"Model training error","ModelItem is not connected to an InputItem!");
			}
			if(input_->data()==0)
			{
				throw BALL::Exception::GeneralException(__FILE__,__LINE__,"Model training error","Data has not been read by InputItem!");
			}
			
			model_->setDataSource(input_->data());
			if(descriptor_source_model_!=NULL)
			{
				cout<<"copied "<<descriptor_source_model_->model()->getDescriptorIDs()->size()<<" descriptor IDs"<<endl;
				model_->copyDescriptorIDs(*descriptor_source_model_->model());
			}
			
			if (!no_training_)
			{
				optimizeModelParameters();
				optimizeKernelParameters();
			}
			
			model_->readTrainingData();
			
			if(!no_training_)
			{
				model_->train();
			}
			
			setResultString((int)model_->getDescriptorNames()->size());
			if(feature_plotter_!=0) feature_plotter_->update();
			done_ = 1; //ready!
			return 1;
		}


		void ModelItem::optimizeModelParameters()
		{
			if(optimize_model_parameters && k_fold>=2)
			{
				cout<<"optimizing model parameters..."<<endl;
				model_->optimizeParameters(k_fold);	
			}
		}


		void ModelItem::optimizeKernelParameters()
		{
			if(optimize_kernel_parameters && k_fold>=2)
			{
				cout<<"optimizing kernel parameters..."<<endl;
				/// search locally around current kernel parameters
				KernelModel* km = (KernelModel*)model_;
				km->kernel->gridSearch(grid_search_stepwidth, grid_search_steps,grid_search_recursions,k_fold);
			}	
		}


		bool ModelItem::isDone()
		{
			if(done_) return 1;
			
			// If this ModelItem is the output of a feature selection, FeatureSelectionItem::execute() will set this->done_ to true.
			// Thus if this ModelItem is the output of a feature selection and the pipeline has just been reloaded (done_==0), just check in order to set the correct pixmap in this case.	
			bool only_fs=1;
			for(set<Edge*>::iterator it=in_edge_list_.begin();it!=in_edge_list_.end();it++)
			{
				if((*it)->sourceNode()->type()!=FeatureSelectionItem::Type)
				{
					only_fs=0;
					break;
				}
			}
			if(only_fs)
			{
				enableTraining(); // sets pixmap
				if(!(*in_edge_list_.begin())->sourceNode()->isDone()) return 1; 
				else return 0;
			}
			
			// if this model is the last child of a pipeline train it in order to be able to analyze the final model or make predictions
			if(out_edge_list_.size()==0)
			{
				enableTraining();
				return 0;
			}
			
			// if this item's outgoing edges are connected to FeatureSelectionItems only, save time by not training this model
			for(set<Edge*>::iterator it=out_edge_list_.begin();it!=out_edge_list_.end();it++)
			{
				// if insignificant features are to be removed (FS-type 5), the model has to be trained first
				if((*it)->destNode()->type()!=FeatureSelectionItem::Type || ((FeatureSelectionItem*)(*it)->destNode())->getType()==5)
				{
					enableTraining();
					return 0;
				}
			}
			
			// If this ModelItem is connected only to FeatureSelectionItem (!=type 5),
			// then make sure that this model is NOT trained but return 0, so that execute() will 
			// read the training data
			disableTraining();
			return 0;
		}

		void ModelItem::disableTraining()
		{
			if(!no_training_) // do only if training has not yet been disabled...
			{
				no_training_ = 1;
				setPixmap();
			}
		}

		void ModelItem::enableTraining()
		{
			if(no_training_) // do only if training has been disabled...
			{
				no_training_ = 0;
				setPixmap();
			}
		}

		void ModelItem::setPixmap()
		{
			QPixmap pm;
			String dir = view_->data_scene->main_window->getImageDirectory();
			if(!no_training_)
			{
				if (entry_->kernel)
				{
					pm = QPixmap((dir+"kernel_model.png").c_str()).scaled(QSize(width(), height()), Qt::KeepAspectRatio,Qt::FastTransformation );
				}
				else 
				{
					pm = QPixmap((dir+"model.png").c_str()).scaled(QSize(width(), height()), Qt::KeepAspectRatio,Qt::FastTransformation );
				}
			}
			else
			{	
				if (entry_->kernel)
				{
					pm = QPixmap((dir+"kernel_model_deactivated.png").c_str()).scaled(QSize(width(), height()), Qt::KeepAspectRatio,Qt::FastTransformation );
				}
				else 
				{
					pm = QPixmap((dir+"model_deactivated.png").c_str()).scaled(QSize(width(), height()), Qt::KeepAspectRatio,Qt::FastTransformation );
				}
			}
			QGraphicsPixmapItem::setPixmap(pm);
		}

		void  ModelItem::setSaveAttribute(bool save)
		{
			save_attribute_ = save;
		}

		bool  ModelItem::saveAttribute()
		{
			return save_attribute_;
		}

		void ModelItem::setInputDataItem(InputDataItem* item)
		{
			input_ = item;
			model_->setDataSource(item->data());
		}

		void ModelItem::addPredictionInputEdge(Edge* edge)
		{
			prediction_input_edges_.insert(edge);
			if (in_edge_list_.find(edge)!=in_edge_list_.end())
			{
				in_edge_list_.erase(in_edge_list_.find(edge));
			}
		}

		void ModelItem::deletePredictionInputEdge(Edge* edge)
		{
			if (prediction_input_edges_.find(edge)!=prediction_input_edges_.end())
			{
				prediction_input_edges_.erase(prediction_input_edges_.find(edge));
			}
			delete edge;
		}


		void ModelItem::createActions()
		{
			String dir = view_->data_scene->main_window->getImageDirectory();
			
			QAction* save_action = new QAction(QIcon((dir+"save_desktop.png").c_str()),tr("Save model"), this);
			connect(save_action, SIGNAL(triggered()), this, SLOT(saveModel()));
			context_menu_actions_.push_back(save_action);

			QAction* load_action = new QAction(QIcon((dir+"save_desktop.png").c_str()),tr("Load model"), this);
			connect(load_action, SIGNAL(triggered()), this, SLOT(loadModel()));
			context_menu_actions_.push_back(load_action);

			QAction* properties_action = new QAction(QIcon((dir+"save_desktop.png").c_str()),tr("Show Properties"), this);
			connect(properties_action, SIGNAL(triggered()), this, SLOT(showProperties()));
			context_menu_actions_.push_back(properties_action);
			
			if(entry_->regression) 
			{
				QAction* plot_features_action = new QAction("plot coefficients",this);
				connect(plot_features_action,SIGNAL(triggered()),this,SLOT(showPlotter()));
				context_menu_actions_.push_back(plot_features_action);
			}
			else if(model_!=NULL&&(*model_->getType()=="snB"||*model_->getType()=="nB"))
			{
				QAction* plot_features_action = new QAction("plot training result",this);
				connect(plot_features_action,SIGNAL(triggered()),this,SLOT(showPlotter()));
				context_menu_actions_.push_back(plot_features_action);
			}
			if(entry_->regression) 
			{
				QAction* plot_features_action = new QAction("plot features",this);
				connect(plot_features_action,SIGNAL(triggered()),this,SLOT(showFeaturePlotter()));
				context_menu_actions_.push_back(plot_features_action);
			}
			if(entry_->latent_variables)
			{
				QAction* plot_components_action = new QAction("plot loadings",this);
				connect(plot_components_action,SIGNAL(triggered()),this,SLOT(showLoadingPlotter()));
				context_menu_actions_.push_back(plot_components_action);
			}
			if(entry_->latent_variables)
			{
				QAction* plot_components_action = new QAction("plot components",this);
				connect(plot_components_action,SIGNAL(triggered()),this,SLOT(showLatentVariablePlotter()));
				context_menu_actions_.push_back(plot_components_action);
			}
				
		}


		// SLOT
		void ModelItem::saveModel()
		{
			try
			{
				if(done_)
				{
					model_->setDataSource(input_->data());
					QString filename = QFileDialog::getSaveFileName(view_, tr("Save File as"),"",tr("text (*.txt)"));
					model_->saveToFile(filename.toStdString());
				}
			}
			catch(GeneralException e)
			{	
				QMessageBox::about(view_, tr("Error"),e.getMessage());
			}	
		}

		void ModelItem::saveToFile(String file)
		{
			try
			{
				if(done_)
				{
					model_->setDataSource(input_->data()); // neccessary when saving a pipeline directly after restoring it if it includes nested validations
					model_->saveToFile(file);
				}
			}
			catch(GeneralException e)
			{	
				QMessageBox::about(view_, tr("Error"),e.getMessage());
			}
		}

		void ModelItem::loadModel()
		{
			QString filename = QFileDialog::getOpenFileName(view_, tr("Open Model"),"",tr("models (*.mod)"));
			if (!filename.isEmpty())
			{
				try
				{
					model_->readFromFile(filename.toStdString());
				}
				catch(WrongDataType e)
				{
					QMessageBox::warning(view_,"Error",e.getMessage());
					return;
				}
			}
			
			setResultString((int)model_->getDescriptorNames()->size());
			done_ = 1;
		}

		void ModelItem::loadFromFile(String file)
		{
			try
			{
				model_->readFromFile(file);
			}
			catch(WrongDataType e)
			{
				QMessageBox::warning(view_,"Error",e.getMessage());
				return;
			}
			
			if(!save_attribute_) // if this model was created by a feature selection, also set FeatureSelectionItem::done_ to true
			{
				for(set<Edge*>::iterator it=in_edge_list_.begin(); it!=in_edge_list_.end();it++)
				{
					if((*it)->sourceNode()->type()==FeatureSelectionItem::Type)
					{
						(*it)->sourceNode()->setDone(1);
					}
				}
			}
			
			setResultString((int)model_->getDescriptorNames()->size());
			done_ = 1;
		}

		void ModelItem::showProperties()
		{
			ModelConfigurationDialog modelConfigurationDialog(this, view_->data_scene->main_window);
			modelConfigurationDialog.exec();
		}


		void ModelItem::writeConfigSection(ofstream& out)
		{
			String parameter_string;
			String tmp;
			for (uint i = 0; i < model_parameters.size(); i++)
			{
				parameter_string += " "+ String(model_parameters[i]);
			}
			
			bool hasKernel = getRegistryEntry()->kernel;
			
			out << "[ModelCreator]" << "\n";
			if(isDone()) out << "done = "<<1<<endl;
			out << "data_file = "<< inputDataItem()->savedAs().toStdString() << "\n";
			if(descriptor_source_model_!=NULL) 
			{
				out<<"descriptor_source_model = " <<descriptor_source_model_->savedAs().toStdString()<<endl;
			}
			out << "model_no = "<< view_->data_scene->main_window->reg_->getModelNo(getRegistryEntry()->name_abreviation) << "\n";
			out << "model_parameters = "<< parameter_string << "\n";
			
			isDone(); // set no_training_ appropriately
			if(no_training_) out<<"no_training = 1\n";
			
			if (hasKernel)
			{
				out << "kernel_type = "<< kernel_function_type <<"\n";
			
				if (kernel_function_type != 4)
				{
					out << "kernel_par1 = "<< kernel_parameter1 << "\n";
					if (kernel_function_type == 3)
					{
						out << "kernel_par2 = " << kernel_parameter2 << "\n";
					}
				}
				out << "grid_search_steps = "<< grid_search_steps << "\n";
				out << "grid_search_stepwidth = "<< grid_search_stepwidth <<"\n";
				out << "grid_search_recursions = "<< grid_search_recursions << "\n";
			}
			
			out << "optimize_model_parameters = "<< optimize_model_parameters << "\n";
			
			if (optimize_model_parameters || optimize_kernel_parameters)
			{
				out << "k_fold = "<< k_fold <<  "\n";
			}
			out << "output = "<< savedAs().toStdString() << "\n\n";
		}

		void ModelItem::removeFromPipeline()
		{
			view_->data_scene->main_window->model_pipeline_.erase(this);
			view_->data_scene->main_window->all_items_pipeline_.erase(this);
		}

		void ModelItem::addToPipeline()
		{
			view_->data_scene->main_window->model_pipeline_.insert(this);
			view_->data_scene->main_window->all_items_pipeline_.insert(this);
		}

		// SLOT
		void ModelItem::showPlotter()
		{
			if(model_==NULL) return;
			
			// for the moment, we can only plot regression coefficients...
			if(entry_->regression && ((RegressionModel*)model_)->getTrainingResult()->Ncols()!=0)
			{
				if(plotter_ == NULL)
				{
					plotter_=new CoefficientPlotter(this);
				}
				plotter_->show();
				plotter_->raise();
				
			}
			else if((*model_->getType()=="snB"||*model_->getType()=="nB") && ((BayesModel*)model_)->isTrained())
			{
				if(plotter_ == NULL)
				{
					plotter_=new BayesPlotter(this);
				}
				plotter_->show();
				plotter_->raise();
			}
		}


		BALL::String ModelItem::getMouseOverText()
		{
			String message="";
			bool b=(view_->name=="view");
			
			if(!b)
			{
				stringstream s; s.str(entry_->name);
				for(int i=0;s;i++) // insert line-break after every second word
				{
					String word; s >> word;
					if(word=="") break;
					if(i>0) message+="\n";
					message+=word+" ";
					word="";
					s >> word;
					message+=word;
				}
			}
			else message=entry_->name;
			
			if(!b) return message;
			
			if(no_training_)
			{
				message+="\n  training deactivated";
				return message;
			}
			
			bool fs=0;
			bool fs_opt_par=0;
			bool fs_opt_ker=0;
			if(in_edge_list_.size()!=0&&(*in_edge_list_.begin())->sourceNode()->type()==FeatureSelectionItem::Type) fs=1;
			if(fs)
			{
				FeatureSelectionItem* fs_item = (FeatureSelectionItem*) (*in_edge_list_.begin())->sourceNode();
				if(fs_item->getPostFSModelParOpt()) fs_opt_par=1;
				if(fs_item->getPostFSKernelParOpt()) fs_opt_ker=1;
			}
			
			if(entry_->parameterNames.size()>0)
			{
				message+="\n  ";
				const vector<double>& par = model_->getParameters();
				std::multiset<int>& opt_par = entry_->optimizableParameters;
				std::multiset<int>::iterator it = opt_par.begin();
				for(uint i=0; i<entry_->parameterNames.size()&&i<par.size(); i++)
				{
					message+=entry_->parameterNames[i];
					if(!done_&&
							((!fs&&optimize_model_parameters&&it!=opt_par.end()&&*it==(int)i)
					 || (fs&&fs_opt_par&&it!=opt_par.end()&&*it==(int)i)))
					{
						message+=" will be optimized by cross validation";
						it++;
					}
					else 
					{
						String t(par[i]);
						int index = t.find_last_not_of("0");
						if(index!=(int)string::npos)
						{
							if(index+1<=(int)t.size()&&t[index]!='.') index++;
							message+="="+t.substr(0,index);
						}
						else message+=t;
					}
					if(i<par.size()-1) message+="\n";
				}
			}
			if(entry_->kernel)
			{
				message+="\n  ";
				KernelModel* km = (KernelModel*) model_;
				if(km->kernel->type==1) message+="polyn. kernel, ";
				else if(km->kernel->type==2) message+="RBF kernel, ";
				else if(km->kernel->type==3) message+="sigm. kernel, ";
				if(done_||(!fs&&!optimize_kernel_parameters)||(fs&&!fs_opt_ker))
				{
					if(km->kernel->type==1) message+="degree="+String(km->kernel->par1);
					else if(km->kernel->type==2) message+="gamma="+String(km->kernel->par1);
					else if(km->kernel->type==3) message+="c="+String(km->kernel->par1)+", d="+String(km->kernel->par2);
				}	
				else
				{
					if(km->kernel->type==1) message+="degree ";
					else if(km->kernel->type==2) message+="gamma ";
					else if(km->kernel->type==3) message+="c and d ";
					message+="will be optimized by grid search";
				}	
			}
			return message;	
		}


		void ModelItem::change()
		{
			DataItem::change();
		//	delete feature_plotter_; 
			delete plotter_; 
			delete latent_variable_plotter_;
			delete loading_plotter_;
		//	feature_plotter_=NULL;
			plotter_=NULL;
			latent_variable_plotter_=NULL;
			loading_plotter_=NULL;
		}


		// SLOT
		void ModelItem::showFeaturePlotter()
		{
			if(model_==NULL || !isDone()) return;
			
			if(entry_->regression)
			{
				if(feature_plotter_ == NULL)
				{
					feature_plotter_=new FeaturePlotter(this);
				}
				feature_plotter_->show();
			}
		}


		// SLOT
		void ModelItem::showLatentVariablePlotter()
		{
			if(model_==NULL) return;
			
			if(entry_->regression && ((RegressionModel*)model_)->getTrainingResult()->Ncols()!=0)
			{
				if(latent_variable_plotter_ == NULL)
				{
					latent_variable_plotter_=new ComponentPlotter(this,0);
				}
				latent_variable_plotter_->show();
			}
		}

		// SLOT
		void ModelItem::showLoadingPlotter()
		{
			if(model_==NULL) return;
			
			if(entry_->regression && ((RegressionModel*)model_)->getTrainingResult()->Ncols()!=0)
			{
				if(loading_plotter_ == NULL)
				{
					loading_plotter_=new ComponentPlotter(this,1);
				}
				loading_plotter_->show();
			}
		}
	}
}