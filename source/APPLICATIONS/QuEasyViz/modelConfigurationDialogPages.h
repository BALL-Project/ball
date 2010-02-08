/* modelConfigurationDialogPages.h
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

#ifndef PAGES_H
#define PAGES_H

#include <QtGui/QWidget>
#include <QtGui/QPushButton>
#include <QtGui/QMessageBox>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QGroupBox>
#include <QtGui/QTextEdit>
#include <QtGui/QTableWidget>

#include <modelConfigurationDialog.h>
#include <connectionManager.h>
#include <mainWindow.h>

#include <set>

namespace BALL
{
	namespace VIEW
	{
		class ModelConfigurationDialog;
		class ConnectionManager;

		/** @class ModelParameterPage
		* @brief a dialog page
		*
		* @todo
		*/
		class ModelParameterPage : public QWidget
		{
			
			public:
				ModelParameterPage(ModelConfigurationDialog* parent);	
				std::vector<QLineEdit*> edits_;
				QLineEdit* edit_;
		};
		

		/** @class KernelParameterPage
		* @brief a dialog page
		*
		* @todo
		*/
		class KernelParameterPage : public QWidget
		{
			Q_OBJECT
			public:
				KernelParameterPage(ModelConfigurationDialog* parent);
				KernelParameterPage();
				QComboBox* kernel_combo_;
				QLineEdit* kernel_param_edit1_;
				QLineEdit* kernel_param_edit2_;
				QLineEdit* kernel_string_edit1_ ;
				QLineEdit* kernel_string_edit2_;
				QLabel* function_label_;

			public slots:
				void showKernelFunction(int id);
				void showExtensions(int id);

			signals:
				void hasIndividualKernel(bool);
				void hasKernelParameter1(bool);
				void hasKernelParameter2(bool);
	
			private:
				QWidget* kernelParameter1Extension_;
				QWidget* kernelParameter2Extension_;
				QWidget* individualKernelExtension_;
				QSAR::Registry* registry_;
				ModelConfigurationDialog* dialog_;
		};
		
		/** @class OptimizePage
		* @brief a dialog page
		*
		* @todo
		*/
		class OptimizePage : public QWidget
		{
			Q_OBJECT
					
			public:
				OptimizePage(ModelConfigurationDialog* parent);
				OptimizePage();
				QCheckBox* enable_checkbox;
				std::vector<QLineEdit*> edits_;
				std::vector<QLabel*> labels_;
				QLineEdit* edit_;
				QLabel* label_;
				QLineEdit* k_edit_;
				//QLineEdit* param_k_edit_;
				QLineEdit* search_edit1_;
				QLineEdit* search_edit2_;
				QLineEdit* search_edit3_;
				//QLineEdit* kernel_k_edit_;
				QLineEdit* parameter1_edit_;
				QLineEdit* parameter2_edit_;
				
			private:
				/** used to activate/deactivate everthing if checkbox is clicked */
				list<QGroupBox*> groupboxes;
				
			private slots:
				void enableCheckboxChange();
		};

		/** @class ModelPropertiesPage
		* @brief a dialog page
		*
		* @todo
		*/
		class ModelPropertiesPage : public QWidget
		{	
			public:
				ModelPropertiesPage(ModelConfigurationDialog* parent);	

		};

		/** @class ConnectionsPage
		* @brief a dialog page
		*
		* @todo
		*/
		class ConnectionsPage : public QWidget
		{	
			public:
				ConnectionsPage(ModelConfigurationDialog* parent);	

			private:
				ModelConfigurationDialog* parent_;
				ConnectionManager* connection_manager_;

		};

		
		/** @class DataPage
		* @brief a dialog page
		*
		* @todo
		*/
		class DataPage : public QWidget
		{
			Q_OBJECT
			
			public:
				DataPage(ModelConfigurationDialog* parent);
			
			private:
				const vector<string>* descriptor_names_;
				std::multiset<unsigned int>* descriptor_ids_;
				
				QTableWidget* table_;
				QTextEdit* descriptor_explanation_;
				MainWindow* main_window_;
				
			private slots:
				void updateDescriptorExplanation();
				
		};
	}
}

#endif
