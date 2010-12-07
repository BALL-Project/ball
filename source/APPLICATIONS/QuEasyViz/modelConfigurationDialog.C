/* TRANSLATOR BALL::QSAR

		Necessary for lupdate.
*/

/* modelConfigurationDialog.C
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

#include <modelConfigurationDialog.h>
#include <BALL/QSAR/exception.h>
#include <exception.h>
#include <BALL/MATHS/parsedFunction.h>
#include <BALL/VIEW/KERNEL/iconLoader.h>


#include <QtGui/QLabel>
#include <QtGui/QGroupBox>
#include <QtGui/QComboBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QLineEdit>

#include <iostream>

using namespace BALL::QSAR;
using namespace BALL::Exception;
using namespace BALL::QSAR::Exception;

namespace BALL
{
	namespace VIEW
	{

		ModelConfigurationDialog::ModelConfigurationDialog(ModelItem* modelitem, InputDataItem* input, MainWindow* parent):
			parent(parent),
			model_item_(modelitem),
			input_(input),
			param1_is_set_(false),
			param2_is_set_(false)
		{
			entry_ = model_item_->getRegistryEntry();
			entryHasKernel = entry_->kernel;
			entryHasParameters = entry_->parameterNames.size() > 0;
			isOptimizable = entry_->optimizableParameters.size() > 0;
			
			buttons_ = new QDialogButtonBox(QDialogButtonBox::Cancel,Qt::Horizontal, this);
			okButton_ = new QPushButton("Ok", this);
			buttons_->addButton(okButton_, QDialogButtonBox::AcceptRole);
			QHBoxLayout *buttonsLayout = new QHBoxLayout();
			buttonsLayout->addWidget(buttons_);

			connect(buttons_, SIGNAL(rejected()), this, SLOT(reject()));
			connect(okButton_, SIGNAL(clicked()), this, SLOT(createModel()));	

			QVBoxLayout *mainLayout = new QVBoxLayout;

			mainLayout->addSpacing(12);

			contentsWidget = new QListWidget(this);
			contentsWidget->setViewMode(QListView::IconMode);
			contentsWidget->setIconSize(QSize(55, 55));
			contentsWidget->setMovement(QListView::Static);
			contentsWidget->setMinimumWidth(135);
			contentsWidget->setMaximumWidth(135);
			contentsWidget->setSpacing(1);

			pagesWidget = new QStackedWidget(this);
			optimizePage_ = NULL;

			if (entryHasParameters)
			{
				modelPage_ = new ModelParameterPage(this);
				pagesWidget->addWidget(modelPage_);
			}
			if(entryHasKernel)
			{
				kernelPage_ = new KernelParameterPage(this);
				pagesWidget->addWidget(kernelPage_);
			}
			if(isOptimizable || entryHasKernel)
			{
				optimizePage_ = new OptimizePage(this);
				pagesWidget->addWidget(optimizePage_);
			}

			connectionsPage_ = NULL; //new ConnectionsPage(this);
			//pagesWidget->addWidget(connectionsPage_);

			createIcons();

			contentsWidget->setCurrentRow(0);

			QHBoxLayout *horizontalLayout = new QHBoxLayout();
			horizontalLayout->addWidget(contentsWidget);
			horizontalLayout->addWidget(pagesWidget, 1);	

			mainLayout->addLayout(horizontalLayout);
			mainLayout->addLayout(buttonsLayout);
			setLayout(mainLayout);
			
			std::string ab_name = entry_->name_abreviation;
			std::string name = entry_->name;
			this->setWindowTitle("Model Properties: " + QString(name.c_str()) + " (" + QString(ab_name.c_str()) + ") ");

		}


		ModelConfigurationDialog::ModelConfigurationDialog(ModelItem* modelitem, MainWindow* parent):
			parent(parent),
			okButton_(NULL),
			model_item_(modelitem),
			input_(NULL),
			param1_is_set_(false),
			param2_is_set_(false)
		{
			entry_ = model_item_->getRegistryEntry();
			entryHasKernel = entry_->kernel;
			entryHasParameters = entry_->parameterNames.size() > 0;
			isOptimizable = entry_->optimizableParameters.size() > 0;

			///create button box and connect the buttons
			buttons_ = new QDialogButtonBox(QDialogButtonBox::Ok, Qt::Horizontal, this);
			QHBoxLayout *buttonsLayout = new QHBoxLayout();
			buttonsLayout->addWidget(buttons_);
			connect(buttons_, SIGNAL(accepted()), this, SLOT(accept()));
			
			QVBoxLayout *mainLayout = new QVBoxLayout();
		//	mainLayout->addSpacing(12);

			contentsWidget = new QListWidget(this);
			contentsWidget->setViewMode(QListView::IconMode);
			contentsWidget->setIconSize(QSize(55, 55));
			contentsWidget->setMovement(QListView::Static);
			contentsWidget->setMinimumWidth(135);
			contentsWidget->setMaximumWidth(135);
			contentsWidget->setSpacing(1);
			contentsWidget->setFlow(QListView::TopToBottom);
			contentsWidget->setWrapping(false);

			///create widget that displays the pages and add the property page that is needed in this case
			pagesWidget = new QStackedWidget(this);
				
			propertyPage_ = new ModelPropertiesPage(this);
			pagesWidget->addWidget(propertyPage_);

			dataPage_ = new DataPage(this);
			pagesWidget->addWidget(dataPage_);

			connectionsPage_ = NULL ;//new ConnectionsPage(this);
			//pagesWidget->addWidget(connectionsPage_);
			
			optimizePage_ = 0;
			kernelPage_ = 0;
			modelPage_ = 0;
			
			contentsWidget->setCurrentRow(0);

			QHBoxLayout *horizontalLayout = new QHBoxLayout();
			horizontalLayout->addWidget(contentsWidget);
			horizontalLayout->addWidget(pagesWidget, 1);	

			mainLayout->addLayout(horizontalLayout);
			mainLayout->addLayout(buttonsLayout);
			setLayout(mainLayout);
			
			///set window title
			std::string ab_name = entry_->name_abreviation;
			std::string name = entry_->name;
			this->setWindowTitle("Model Properties: " + QString(name.c_str()) + " (" + QString(ab_name.c_str()) + ") ");

			///add button for this page to the ListWidget
			QListWidgetItem *propertyButton = new QListWidgetItem(contentsWidget);
			propertyButton->setIcon(QIcon(IconLoader::instance().getIcon("actions/modelproperties")));
			propertyButton->setText(tr("Model Properties"));
			propertyButton->setTextAlignment(Qt::AlignHCenter);
			propertyButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

			QListWidgetItem *dataButton = new QListWidgetItem(contentsWidget);
			dataButton->setIcon(QIcon(IconLoader::instance().getIcon("actions/sdf_icon")));
			dataButton->setText(tr("Descriptors"));
			dataButton->setTextAlignment(Qt::AlignHCenter);
			dataButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

		// 	QListWidgetItem *connectionsButton = new QListWidgetItem(contentsWidget);
		// 	connectionsButton->setIcon(QIcon("./images/connections"));
		// 	connectionsButton->setText(tr("Edit Connections"));
		// 	connectionsButton->setTextAlignment(Qt::AlignHCenter);
		// 	connectionsButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

			connect(contentsWidget, SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *)),this, SLOT(changePage(QListWidgetItem *, QListWidgetItem*)));	
		}

		ModelConfigurationDialog::ModelConfigurationDialog():
			contentsWidget(NULL),
			pagesWidget(NULL),
			model_item_(NULL),
			input_(NULL),
			entry_(NULL),
			modelPage_(NULL),
			kernelPage_(NULL),
			optimizePage_(NULL),
			connectionsPage_(NULL)
		{
		}

		///destructor
		ModelConfigurationDialog::~ModelConfigurationDialog()
		{
 			delete okButton_;
 			delete buttons_;
 			delete modelPage_;
  			delete kernelPage_;
  			delete optimizePage_;
			delete propertyPage_;
			delete connectionsPage_;
			delete dataPage_;
			delete contentsWidget;
			delete pagesWidget;
		}

		///function for setting up the icon box to the left of the dialog
		void ModelConfigurationDialog::createIcons()
		{			
			///add button for model parameter page
			if (entryHasParameters)
			{
				QListWidgetItem* modelParamButton = new QListWidgetItem(contentsWidget);
				modelParamButton->setIcon(IconLoader::instance().getIcon("actions/modelparameters"));
				modelParamButton->setText(tr("Model Parameters"));
				modelParamButton->setTextAlignment(Qt::AlignHCenter);
				modelParamButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
			}

			///add button for kernel parameter page
			if(entryHasKernel)
			{
				QListWidgetItem* kernelParamButton = new QListWidgetItem(contentsWidget);
				kernelParamButton->setIcon(IconLoader::instance().getIcon("actions/kernelparameters"));
				kernelParamButton->setText(tr("Kernel Parameters"));
				kernelParamButton->setTextAlignment(Qt::AlignHCenter);
				kernelParamButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
			}

			///add button for optimize page
			if(isOptimizable || entryHasKernel)
			{
				QListWidgetItem* optimizeButton = new QListWidgetItem(contentsWidget);
				optimizeButton->setIcon(IconLoader::instance().getIcon("actions/modeloptimization"));
				optimizeButton->setText(tr("Optimize Parameters"));
				optimizeButton->setTextAlignment(Qt::AlignHCenter);
				optimizeButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
			}

		// 	QListWidgetItem* connectionsButton = new QListWidgetItem(contentsWidget);
		// 	connectionsButton->setIcon(QIcon("./images/connections"));
		// 	connectionsButton->setText(tr("Edit Connections"));
		// 	connectionsButton->setTextAlignment(Qt::AlignHCenter);
		// 	connectionsButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

			///allow changing of pages by clicking on the corresponding buttons
			connect(contentsWidget, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)),this, SLOT(changePage(QListWidgetItem*, QListWidgetItem*)));	
		}

		///function for changing the dialog pages by clicking on the corresponding icon in the icon box
		void ModelConfigurationDialog::changePage(QListWidgetItem* current, QListWidgetItem* previous)
		{
			if (!current)
			{
				current = previous;
			}

			pagesWidget->setCurrentIndex(contentsWidget->row(current));	
			
			if (pagesWidget->currentWidget() == propertyPage_)
			{
				return;
			}
		}

		///function for reading the model parameters from the model parameter page
		void ModelConfigurationDialog::applyModelParameters()
		{	
			///read in parameters from the model parameter page
			model_item_->model_parameters.clear();
			bool ok;
			double num = 0.;
			for (uint i=0; i < modelPage_->edits_.size(); i++)
			{
				num = modelPage_->edits_[i]->text().toDouble(&ok);
				if (ok)
				{	
					model_item_->model_parameters.push_back(num);
				}
				else 
				{	
					QMessageBox::about(this, tr("Error"),tr("Invalid model parameter"));
					modelPage_->edits_[i]->setText("Invalid model parameter");
				}
			}	
		}

		///function for reading the kernel parameters from the kernel parameter page
		void ModelConfigurationDialog::applyKernelParameters()
		{
			switch (kernelPage_->kernel_combo_->currentIndex()) 
			{
				case 0:
					model_item_->kernel_function_type = 1;
					break;
				case 1:
					model_item_->kernel_function_type = 2;
					break;
				case 2:
					model_item_->kernel_function_type = 3;
					break;
				case 3: 
					model_item_->kernel_function_type = 4;
					break;
			}

			bool ok;
			double num = 0.;
			
			/// read in kernel parameter 1, which is needed for all kernel functions except the individual kernel function
			if (model_item_->kernel_function_type != 4)
			{
				num = kernelPage_->kernel_param_edit1_->text().toDouble(&ok);
				if (ok)
				{	
					model_item_->kernel_parameter1 = num;
					param1_is_set_ = true;
				}
				else 
				{	
					QMessageBox::about(this, tr("Error"),tr("Invalid kernel parameter"));
					kernelPage_->kernel_param_edit1_->setText("Invalid Parameter");
					param1_is_set_ = false;
				}

				///read in kernel parameter 2, needed for sigmoid kernel
				if (model_item_->kernel_function_type == 3)
				{
					ok = false;
					num = kernelPage_->kernel_param_edit2_->text().toDouble(&ok);
					if (ok)
					{
						model_item_->kernel_parameter2 = num;
						param2_is_set_ = true;
					}
					else 
					{	
						QMessageBox::about(this, tr("Error"),tr("Invalid kernel parameter"));
						kernelPage_->kernel_param_edit2_->setText("Invalid Parameter");
						param2_is_set_ = false;
					}
				}
				return;
			}	

			///read in strings for individual kernel
			else if (model_item_->kernel_function_type == 4)
			{	String string1 = String(kernelPage_->kernel_string_edit1_->text().toStdString());
				String string2 = String(kernelPage_->kernel_string_edit2_->text().toStdString());

				if(evaluateIndividualKernelFunction(string1) && evaluateIndividualKernelFunction(string2))
				{
					individual_kernel_string1_ = kernelPage_->kernel_string_edit1_->text().toStdString();
					individual_kernel_string2_ = kernelPage_->kernel_string_edit2_->text().toStdString();
				}
				else
				{
					QMessageBox::about(this, tr("Error"),tr("Invalid kernel string(s)"));
				}
			}	
		}

		///function for reading in the optimization parameters for both model and kernel parameters
		void ModelConfigurationDialog::applyOptimizedParameters()
		{
			bool ok = false;
			
			// check whether user wants the current model to be optimized.
			if(optimizePage_!=NULL && !optimizePage_->enable_checkbox->isChecked())
			{
				model_item_->optimize_model_parameters=false;
				model_item_->optimize_kernel_parameters=false;
				return;
			}

			if(isOptimizable || entryHasKernel)
			{
				int k = optimizePage_->k_edit_->text().toInt(&ok);
				if (ok)
				{	
					model_item_->k_fold = k;
					if(isOptimizable) model_item_->optimize_model_parameters = true;
					else model_item_->optimize_model_parameters=false;
				}	
			}
			
			///if kernel parameters are optimizable
			if(entryHasKernel)
			{
				///read in step width
				ok = false;
				double tmp1 = optimizePage_->search_edit2_->text().toDouble(&ok);
				if (ok)
				{
					model_item_->grid_search_stepwidth = tmp1;
					model_item_->optimize_kernel_parameters = true;
				}
				else
				{
					model_item_->optimize_kernel_parameters = false;
				}
					
				///read in number of steps			
				ok = false;
				int tmp2 = optimizePage_->search_edit1_->text().toInt(&ok);
				if (ok)
				{	
					model_item_->grid_search_steps = tmp2;
				}
				else
				{
					model_item_->optimize_kernel_parameters = false;
				}

				///read in number of recursions
				ok = false;
				int tmp3 = optimizePage_->search_edit3_->text().toInt(&ok);
				if (ok)
				{
					model_item_->grid_search_recursions = tmp3;
				}
				else
				{
					model_item_->optimize_kernel_parameters = false;
				}

		// 		//read in start value for parameter 1
		// 		ok = false;
		// 		double tmp5 = optimizePage_->parameter1_edit_->text().toDouble(&ok);
		// 		if (ok)
		// 		{
		// 			parameter1_start_ = tmp5;
		// 		}
		// 		else
		// 		{
		// 			model_item_->optimize_kernel_parameters = false;
		// 		}
		// 	
		// 		//read in start value for parameter 2
		// 		ok = false;
		// 		double tmp6 = optimizePage_->parameter2_edit_->text().toDouble(&ok);
		// 		if (ok)
		// 		{
		// 			parameter2_start_ = tmp6;
		// 		}
		// 		else
		// 		{
		// 			model_item_->optimize_kernel_parameters = false;
		// 		}
			}
			else
			{
				model_item_->optimize_kernel_parameters = false;
			}
		}

		///function for creating a model item with the given parameters
		void ModelConfigurationDialog::createModel()
		{
			try
			{	
				///if kernel model
				if (entryHasKernel)
				{
					///read in kernel parameters
					applyKernelParameters();
				
					///if kernel is not individual, create the model with the input file, the specified function type and the parameter(s)
					if (model_item_->kernel_function_type < 4)
					{
						///seems a bit odd, but its necessary
						if (param1_is_set_)
						{
							if (model_item_->kernel_function_type !=3) // for kernels that use only 1 parameter
							{
								model_item_ = model_item_->createModel(input_,model_item_->kernel_function_type, model_item_->kernel_parameter1, model_item_->kernel_parameter2);
							}
							else //sigmoid kernel
							{
								if (param2_is_set_) //ensures that both parameters are set
								{
									model_item_ = model_item_->createModel(input_ , model_item_->kernel_function_type, model_item_->kernel_parameter1, model_item_->kernel_parameter2);
								}	
							}
						}
						else //do nothing, because there are no kernel parameters given
						{
							throw KernelParameterError(__FILE__,__LINE__, "Parameter 1 not set");
						}
					}

					///if kernel is individual, create the model with the input and the two kernel strings
					else if (model_item_->kernel_function_type == 4)
					{
						model_item_ = model_item_->createModel(input_,KernelString1(), KernelString2());
					}
				}	
				
				///if model without kernel
				else
				{
					///create model item with the given input
					model_item_ = model_item_->createModel(input_);
				}

				/// look, if the user wants optimization of the model
				applyOptimizedParameters();

				if (model_item_ != NULL && model_item_->model() != NULL)
				{
					///is model has parameters
					if(entryHasParameters)
					{
						///read in parameters
						applyModelParameters();
						///set the parameters
						model_item_->model()->setParameters(model_item_->model_parameters);
					}
				}

			}	
			///if creation fails
			catch(ModelParameterError)
			{
				QMessageBox::warning(this, tr("Error"),tr("Invalid number of model parameters"));
			}
			catch(KernelParameterError)
			{
				QMessageBox::warning(this, tr("Error"),tr("Invalid number of kernel parameters"));
			}
			catch(...)
			{
				QMessageBox::warning(this, tr("Error"),tr("Model creation failed"));
			}
			
			accept(); // close dialog
		}

		ModelItem* ModelConfigurationDialog::modelItem()
		{
			return model_item_;
		}


		std::string ModelConfigurationDialog::KernelString1()
		{
			return individual_kernel_string1_;
		}

		std::string ModelConfigurationDialog::KernelString2()
		{
			return individual_kernel_string2_;
		}

		RegistryEntry* ModelConfigurationDialog::entry()
		{
			return entry_;
		}

		bool ModelConfigurationDialog::evaluateIndividualKernelFunction(String function)
		{
			try
			{
				ParsedFunction<float> parsed_function(function);
			}
			catch(ParseError)
			{
				return 0;
			}
			return 1;
		}


		QString ModelConfigurationDialog::defaultValueToQString(double& value, int precision)
		{
			ostringstream stream;
			stream.precision(precision);
			stream << value;
			QString text(stream.str().c_str());
			return text;	
		}


		QString ModelConfigurationDialog::defaultValueToQString(int& value)
		{
			ostringstream stream;
			stream.precision(0);
			stream << value;
			QString text(stream.str().c_str());
			return text;	
		}
	}
}
