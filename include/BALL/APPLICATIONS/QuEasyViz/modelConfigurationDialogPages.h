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

#include <BALL/APPLICATIONS/QuEasyViz/modelConfigurationDialog.h>
#include <BALL/APPLICATIONS/QuEasyViz/connectionManager.h>
#include <BALL/APPLICATIONS/QuEasyViz/mainWindow.h>
#include <BALL/QSAR/sortedList.h>


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
				BALL::QSAR::SortedList<unsigned int>* descriptor_ids_;
				
				QTableWidget* table_;
				QTextEdit* descriptor_explanation_;
				MainWindow* main_window_;
				
			private slots:
				void updateDescriptorExplanation();
				
		};
	}
}

#endif
