/* TRANSLATOR BALL::QSAR

		Necessary for lupdate.
*/

/* modelConfigurationDialogPages.C
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

#include <modelConfigurationDialogPages.h>
#include <QtGui/QLabel>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QLineEdit>
#include <QtGui/QHeaderView>
#include <BALL/VIEW/KERNEL/iconLoader.h>

using namespace BALL::QSAR;

namespace BALL
{
	namespace VIEW
	{

		ModelParameterPage::ModelParameterPage(ModelConfigurationDialog *parent)
		{
			///return if there's no parent
			if (parent == NULL)
			{
				return;
			}

			/// group the model parameters
			QGroupBox *configGroup = new QGroupBox(tr("Model Parameters"), this);
			QGridLayout *layout = new QGridLayout();

			///create labels and line edits for every model parameter and print out the parameter name and the default value (read from the registry)
			for (unsigned int i=0; i < parent->entry()->parameterNames.size(); i++)
			{
				ostringstream value;
				value.precision(3);
				value << parent->entry()->parameterDefaults[i];
				QLabel* label = new QLabel(QString(parent->entry()->parameterNames[i].c_str()));
				edit_ = new QLineEdit(this);
				edit_->setText(QString(value.str().c_str()));
				edit_->setMinimumSize(50,15);
				edit_->setMaximumSize(80,30);
				edits_.push_back(edit_);
				layout->addWidget(label, i,1);
				layout->addWidget(edit_, i ,2);
			}

			/// mark which parameters are optimizable
			if (parent->isOptimizable)
			{
				std::multiset<int>::iterator o_it = parent->entry()->optimizableParameters.begin();
				
				for(; o_it != parent->entry()->optimizableParameters.end(); ++o_it)
				{
					QLabel* label = new QLabel(QString("*"), this);
					layout->addWidget(label,*o_it, 3);
				}
				QLabel* label = new QLabel("*: can be automatically optimized\n    by cross validation", this);
				layout->addWidget(label,parent->entry()->parameterNames.size()+1,1,1,-1);
			}

			configGroup->setLayout(layout);
			QVBoxLayout *mainLayout = new QVBoxLayout();
			mainLayout->addWidget(configGroup);
			mainLayout->addStretch(1);
			setLayout(mainLayout);	

		}

		KernelParameterPage::KernelParameterPage(ModelConfigurationDialog* parent)
		 {
			///return if there's no parent
			if (parent == NULL)
			{
				return;
			}
			
			dialog_ = parent;	
			registry_ = parent->parent->registry();

			///---------------------CONFIG GROUP----------------------------------

			/// group the kernel parameters
			QGroupBox *configGroup = new QGroupBox(tr("Kernel Parameters"), this);


			///create a combo box with the different available kernel function types
			QLabel *kernelLabel = new QLabel(tr("Kernel function:"), this);

			///don't mess with the combo box, the order should kept fixed, otherwise ModelConfigurationDialog::applyKernelParameters() and isIndividualKernel(int id) will not work correctly!!!!
			kernel_combo_ = new QComboBox(this);
			kernel_combo_->addItem(tr("polynomial"));
			kernel_combo_->addItem(tr("radial basis"));
			kernel_combo_->addItem(tr("sigmoid"));
			kernel_combo_->addItem(tr("individual"));

			QHBoxLayout* kernelParamLayout = new QHBoxLayout();
			kernelParamLayout->addWidget(kernelLabel);
			kernelParamLayout->addWidget(kernel_combo_);

			//-------------------------------------------------------

			///label and line edit for kernel parameter 1
			QLabel* kernelParamLabel1 = new QLabel(tr("Parameter 1:"), this);
			kernel_param_edit1_ = new QLineEdit(this);
			kernel_param_edit1_->setText(QString(((String)(registry_->default_kernel_par1)).c_str()));

			//QLabel* param = new QLabel(QString(((String)(parent->model()->model()->getParameters()[i])).c_str()));

			QHBoxLayout *param1Layout = new QHBoxLayout;
			param1Layout->addWidget(kernelParamLabel1);
			param1Layout->addWidget(kernel_param_edit1_);

			kernelParameter1Extension_ = new QWidget;
			kernelParameter1Extension_->setLayout(param1Layout);
			//-------------------------------------------------------

			///label and line edit for kernel parameter 2 (if available with the chosen kernel type)
			QLabel* kernelParamLabel2 = new QLabel(tr("Parameter 2:"), this);
			kernel_param_edit2_ = new QLineEdit(this);
			kernel_param_edit2_->setText(QString(((String)(registry_->default_kernel_par2)).c_str()));

			QHBoxLayout *param2Layout = new QHBoxLayout();
			param2Layout->addWidget(kernelParamLabel2);
			param2Layout->addWidget(kernel_param_edit2_);

			kernelParameter2Extension_ = new QWidget(this);
			kernelParameter2Extension_->setLayout(param2Layout);

			//-------------------------------------------------------

			///labels and line edits for individual kernel strings
			QLabel* indKernelParamLabel1 = new QLabel(tr("String 1:"), this);
			QLabel* indKernelParamLabel2 = new QLabel(tr("String 2:"), this);
			kernel_string_edit1_ = new QLineEdit(this);
			kernel_string_edit2_ = new QLineEdit(this);

			//-------------------------------------------------------

			QGridLayout* individualKernelLayout = new QGridLayout();
			individualKernelLayout->addWidget(indKernelParamLabel1,1,1);
			individualKernelLayout->addWidget(indKernelParamLabel2,2,1);
			individualKernelLayout->addWidget(kernel_string_edit1_,1,2);
			individualKernelLayout->addWidget(kernel_string_edit2_,2,2);
			
			individualKernelExtension_ = new QWidget(this);
			individualKernelExtension_->setLayout(individualKernelLayout);

			//-------------------------------------------------------

			///add all of the parameter layouts to the group box for the kernel parameters
			QVBoxLayout *configLayout = new QVBoxLayout();
			configLayout->addLayout(kernelParamLayout);
			configLayout->addWidget(kernelParameter1Extension_);
			configLayout->addWidget(kernelParameter2Extension_);
			configLayout->addWidget(individualKernelExtension_);
			configGroup->setLayout(configLayout);

			///---------------------FUNCTION GROUP----------------------------------
			
			QGroupBox* functionGroup = new QGroupBox(tr("Kernel Function"), this);
			QLabel* functionLabel1 = new QLabel(tr("Kernel Function:"), this);
			function_label_ = new QLabel();

			QHBoxLayout *functionLayout = new QHBoxLayout();
			functionLayout->addWidget(functionLabel1);
			functionLayout->addWidget(function_label_);
			functionGroup->setLayout(functionLayout);
			
			//-------------------------------------------------------

			///add all group boxes to the main layout of the page
			QVBoxLayout *mainLayout = new QVBoxLayout();
			mainLayout->addWidget(configGroup);
			mainLayout->addWidget(functionGroup);
			mainLayout->addStretch(1);
			setLayout(mainLayout);	

			///set visibility modes for the different extensions
			kernelParameter1Extension_->show();
			kernelParameter2Extension_->hide();
			individualKernelExtension_->hide();

			///connect all needed signals with the corresponding slots
			connect(kernel_combo_, SIGNAL(activated(int)), this, SLOT(showExtensions(int)));
			connect(kernel_combo_, SIGNAL(activated(int)), this, SLOT(showKernelFunction(int)));
			connect(this, SIGNAL(hasIndividualKernel(bool)), individualKernelExtension_, SLOT(setVisible(bool)));
			connect(this, SIGNAL(hasKernelParameter1(bool)), kernelParameter1Extension_, SLOT(setVisible(bool)));
			connect(this, SIGNAL(hasKernelParameter2(bool)), kernelParameter2Extension_, SLOT(setVisible(bool)));
			showKernelFunction(0); // when opening dialog, show first function
		 }

		void KernelParameterPage::showExtensions(int id)
		{
			if (id < 2)
			{
				emit hasKernelParameter1(true);
				emit hasKernelParameter2(false);
				emit hasIndividualKernel(false);
			}
			else if (id == 2)
			{	
				emit hasKernelParameter1(true);
				emit hasKernelParameter2(true);
				emit hasIndividualKernel(false);
			}
			else if(id == 3)
			{
				emit hasKernelParameter1(false);
				emit hasKernelParameter2(false);
				emit hasIndividualKernel(true);
			}
		}

		QPixmap KernelParameterPage::findPixmap(const String& name)
		{
			QIcon const& icon  = IconLoader::instance().getIcon(String("actions/")+name);
			QList<QSize> sizes = icon.availableSizes();

			QPixmap result = icon.pixmap(sizes.last());

			return result;
		}

		void KernelParameterPage::showKernelFunction(int id)
		{
			QString function_string = "";
			switch (id) 
			{
				case 0: 
					function_string = "Funktion1";
					function_label_->setPixmap(findPixmap("polynomial_kernel"));
					break;
				case 1:
					function_string = "Funktion2";
					function_label_->setPixmap(findPixmap("radial_basis_kernel"));
					kernel_param_edit1_->setText(QString(((String)(registry_->default_rbf_par)).c_str()));
					break;
				case 2: 
					function_string = "Funktion3";
					function_label_->setPixmap(findPixmap("sigmoid_kernel"));
					break;
				case 3:
					function_string = "Funktion4";
					function_label_->setPixmap(QPixmap());
					break;
			}
			
			if(id!=1) kernel_param_edit1_->setText(QString(((String)(registry_->default_kernel_par1)).c_str()));
		}

		OptimizePage::OptimizePage(ModelConfigurationDialog* parent)
		 {	
			///return if there's no parent
			if (parent == NULL)
			{
				return;
			}
			
			QVBoxLayout* mainLayout = new QVBoxLayout();
			Registry* reg = parent->parent->registry();

			if (parent->isOptimizable || parent->entryHasKernel)
			{
				QGroupBox* kGroup = new QGroupBox(tr("k"),this);
				groupboxes.push_back(kGroup);
				
				enable_checkbox = new QCheckBox("enable optimizations",this);
				mainLayout->addWidget(enable_checkbox);
				connect(enable_checkbox,SIGNAL(clicked()),this,SLOT(enableCheckboxChange()));
				
				QHBoxLayout* k_layout = new QHBoxLayout();
				QLabel* label = new QLabel("k for k-fold cross-validation",this);
				k_edit_ = new QLineEdit(this); k_edit_->setMinimumWidth(50);
				k_edit_->setText(parent->defaultValueToQString(reg->default_k));
				k_layout->addWidget(label);
				k_layout->addWidget(k_edit_);
				kGroup->setLayout(k_layout);	
				mainLayout->addWidget(kGroup);
			}
			
			if (parent->isOptimizable)
			{
				QGroupBox *modelConfigGroup = new QGroupBox(tr("Model Parameters to be optimized:"),this);
				groupboxes.push_back(modelConfigGroup);
				QGridLayout* layout1 = new QGridLayout;
			
				std::multiset<int>::iterator o_it = parent->entry()->optimizableParameters.end();
				int i = 0;

				for (; o_it != parent->entry()->optimizableParameters.end(); ++o_it)
				{
					QLabel* label = new QLabel(this);
					label->setText(QString(parent->entry()->parameterNames[*o_it].c_str()));
					label_ = new QLabel(this);
					labels_.push_back(label_);
					layout1->addWidget(label, i+2,1);
					layout1->addWidget(label_, i+2 ,2);
					i++;
				}		

				modelConfigGroup->setLayout(layout1);
			
				QVBoxLayout *modelLayout = new QVBoxLayout();
				modelLayout->addWidget(modelConfigGroup);
				modelLayout->addStretch(1);
				
				mainLayout->addLayout(modelLayout);
			}

			if (parent->entryHasKernel)	
			{		
				QGroupBox* kernelConfigGroup = new QGroupBox(tr("Optimize Kernel Parameters"),this);
				groupboxes.push_back(kernelConfigGroup);
			
				QGridLayout *layout2 = new QGridLayout();
				QLabel* search_label1 = new QLabel(tr("number of steps for grid search:"),this);
				QLabel* search_label2 = new QLabel(tr("width of grid search:"),this);
				QLabel* search_label3 = new QLabel(tr("number of recursion steps:"),this);
		// 		QLabel* parameter1_label = new QLabel(tr("start value for parameter 1"),this);
		// 		QLabel* parameter2_label = new QLabel(tr("start value for parameter 2"),this);
				search_edit1_ = new QLineEdit(this); search_edit1_->setMinimumWidth(60);
				search_edit1_->setText(parent->defaultValueToQString(reg->default_gridsearch_steps));
				search_edit2_ = new QLineEdit(this);
				search_edit2_->setText(parent->defaultValueToQString(reg->default_gridsearch_stepwidth,2));
				search_edit3_ = new QLineEdit(this);
				search_edit3_->setText(parent->defaultValueToQString(reg->default_gridsearch_recursion));
		// 		parameter1_edit_ = new QLineEdit(this);
		// 		parameter1_edit_->setText(parent->defaultValueToQString(reg->default_gridsearch_par1_start,2));
		// 		parameter2_edit_ = new QLineEdit(this);
		// 		parameter2_edit_->setText(parent->defaultValueToQString(reg->default_gridsearch_par2_start,2));
				
				layout2->addWidget(search_label1,1,1);
				layout2->addWidget(search_label2,2,1);
				layout2->addWidget(search_label3,3,1);
				layout2->addWidget(search_edit1_,1,2);
				layout2->addWidget(search_edit2_,2,2);
				layout2->addWidget(search_edit3_,3,2);
		// 		layout2->addWidget(parameter1_label,5,1);
		// 		layout2->addWidget(parameter2_label,6,1);
		// 		layout2->addWidget(parameter1_edit_,5,2);
		// 		layout2->addWidget(parameter2_edit_,6,2);
				
				QVBoxLayout *kernelLayout = new QVBoxLayout();
				kernelConfigGroup->setLayout(layout2);
				kernelLayout->addWidget(kernelConfigGroup);
				kernelLayout->addStretch(1);
			
				mainLayout->addLayout(kernelLayout);
			}
			setLayout(mainLayout);	
			enableCheckboxChange();
		 }
		 
		 
		void OptimizePage::enableCheckboxChange()
		{
			bool enable=enable_checkbox->isChecked();
			
			for(list<QGroupBox*>::iterator it=groupboxes.begin();it!=groupboxes.end();it++)
			{
				(*it)->setEnabled(enable);
			}
		}


		ModelPropertiesPage::ModelPropertiesPage(ModelConfigurationDialog* parent)
		{
			///return if there's no parent
			if (parent == NULL)
			{
				return;
			}
			QVBoxLayout *mainLayout = new QVBoxLayout();

			QGroupBox *paramGroup = new QGroupBox(tr("Model Parameters"),this);
			QGridLayout *layout = new QGridLayout();

			for (unsigned int i=0; i < parent->entry()->parameterNames.size(); i++)
			{
				QLabel* label = new QLabel(QString(parent->entry()->parameterNames[i].c_str()),this);
				layout->addWidget(label, i,1);
				QLabel* param = new QLabel(QString(((String)(parent->modelItem()->model()->getParameters()[i])).c_str()),this);
				layout->addWidget(param, i,2);
			}
			paramGroup->setLayout(layout);
			mainLayout->addWidget(paramGroup);

			if (parent->entryHasKernel)
			{
				QGroupBox *kernelGroup = new QGroupBox(tr("Kernel Parameters"),this);
				QGridLayout *layout2 = new QGridLayout();
			
				KernelModel* kernel_model = (KernelModel*)(parent->modelItem()->model());
				int type = kernel_model->kernel->type;
				double param1 =  kernel_model->kernel->par1;
				double param2 = kernel_model->kernel->par2;
				QString type_string;

				switch (type) 
				{
					case 1:
						type_string = "polynomial kernel";
						break;
					case 2:
						type_string = "radial basis function kernel";
						break;
					case 3:
						type_string = "sigmoid kernel";
						break;
					case 4: 
						type_string = "individual kernel-function";
						break;
					default: 
						type_string = "";
				}

				QLabel* label = new QLabel(type_string,this);
				QLabel* label2 = new QLabel("kernel function type: ", this);
				QLabel* label3 = new QLabel("kernel parameter 1: ", this);
				QLabel* label4 = new QLabel("kernel parameter 2: ", this);
				QLabel* label5 = new QLabel(QString(((String)param1).c_str()));
				QLabel* label6 = new QLabel(QString(((String)param2).c_str()));
				layout2->addWidget(label2,1,1);
				layout2->addWidget(label, 1,2);
				layout2->addWidget(label3, 2,1);
				layout2->addWidget(label4, 3,1);
				layout2->addWidget(label5, 2,2);
				layout2->addWidget(label6, 3,2);

				/*
				for (unsigned int i=0; i < parent->entry()->parameterNames.size(); i++)
				{
					
					QLabel* label = new QLabel(QString(parent->entry()->parameterNames[i].c_str()));
					layout2->addWidget(label, i,1);
				
					//QLabel* param = new QLabel(QString(((String)(parent->modelItem()->model()->getParameters()[i])).c_str()));
					//layout2->addWidget(param, i,2);
				}
				*/

				kernelGroup->setLayout(layout2);
				mainLayout->addWidget(kernelGroup);
			}
			mainLayout->addStretch(1);
			setLayout(mainLayout);	
		}


		ConnectionsPage::ConnectionsPage(ModelConfigurationDialog* parent)
		{
			///return if there's no parent
			if (parent == NULL)
			{
				return;
			}

			connection_manager_ = new ConnectionManager(parent);
			QVBoxLayout *mainLayout = new QVBoxLayout();

			mainLayout->addWidget(connection_manager_);

			mainLayout->addStretch(1);
			setLayout(mainLayout);	
		}


		// SLOT
		void DataPage::updateDescriptorExplanation()
		{
			//descriptor_explanation_->setText(table_->currentItem()->text());
			const String* expl = main_window_->getDescriptorExplanation(table_->currentItem()->text().toStdString());
			
			if(expl!=0)
			{
				descriptor_explanation_->setText(expl->c_str());
				descriptor_explanation_->setMaximumHeight(50);
			}
			else 
			{
				descriptor_explanation_->setText("");
				descriptor_explanation_->setMaximumHeight(0);
			}
		}


		DataPage::DataPage(ModelConfigurationDialog* parent)
		{
			///return if there's no parent
			if (parent == NULL)
			{
				return;
			}
			main_window_ = parent->parent;
			
			QVBoxLayout* mainLayout = new QVBoxLayout();
			QVBoxLayout* resultGroupLayout = new QVBoxLayout();

			QGroupBox* resultGroup = new QGroupBox(tr("Descriptors"),this);

			descriptor_names_ = parent->modelItem()->model()->getDescriptorNames();
			descriptor_ids_ = parent->modelItem()->model()->getDescriptorIDs();

			QStringList labels;
			labels << "Descriptor name";

			table_ = new QTableWidget(descriptor_names_->size(), 1, this);	
			table_->verticalHeader()->hide();
			table_->setHorizontalHeaderLabels (labels);
			table_->setAlternatingRowColors(true);
			table_->setDragDropMode(QAbstractItemView::NoDragDrop);
			table_->setEditTriggers(QAbstractItemView::NoEditTriggers);	
			table_->horizontalHeader()->setResizeMode(QHeaderView::Stretch);

			for (unsigned int i=0; i< descriptor_names_->size(); i++)
			{
				QTableWidgetItem* name = new QTableWidgetItem(QString(descriptor_names_->at(i).c_str()));
				table_->setItem(i, 0, name);	
			}

			QScrollArea* scrollArea = new QScrollArea(this);
			scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
			scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
			scrollArea->setFrameShape(QFrame::NoFrame);
			scrollArea->setWidget(table_);
			scrollArea->setWidgetResizable(true);
			
			descriptor_explanation_ = new QTextEdit(this);
			descriptor_explanation_->setReadOnly(1);
			descriptor_explanation_->setMaximumHeight(0);
			connect(table_,SIGNAL(currentCellChanged(int,int,int,int)),this,SLOT(updateDescriptorExplanation()));

			resultGroupLayout->addWidget(scrollArea);
			resultGroupLayout->addWidget(descriptor_explanation_);
			resultGroup->setLayout(resultGroupLayout);

			QString tmp;
			QLabel* num_of_descriptors;
			
			if (descriptor_ids_->size() == 0)
			{
				num_of_descriptors = new QLabel("There are " + tmp.setNum(descriptor_names_->size()) + " descriptors in this model");
			}
			else
			{
				num_of_descriptors = new QLabel("There are " + tmp.setNum(descriptor_ids_->size()) + " descriptors in this model");
			}
			
			mainLayout->addWidget(num_of_descriptors);
			mainLayout->addWidget(resultGroup);
			setLayout(mainLayout);	
		}
	}
}
