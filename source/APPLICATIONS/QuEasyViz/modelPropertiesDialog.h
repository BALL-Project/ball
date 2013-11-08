#ifndef MODELPROPERTIESDIALOG_H
#define MODELPROPERTIESDIALOG_H

#include <BALL/QSAR/registry.h>
#include <BALL/QSAR/Model.h>
#include <vector>

#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QLineEdit>
#include <QtGui/QGridLayout>
#include <QtGui/QLabel>


namespace BALL
{
	namespace VIEW
	{

		class ModelPropertiesDialog : public QDialog
		{
			Q_OBJECT
		
		public:

			/** @name Constructors and Destructors */

			/** constructor */
			ModelPropertiesDialog();

			/** constructor
			* @param entry @todo
			*/
			ModelPropertiesDialog(RegistryEntry* entry);

			/** destructor */
			~ModelPropertiesDialog();

			/** @name Accessors */
			std::vector<double> parameters();
			int k();
			
		
		public slots:
		
			/** @name Public Slots */
			void apply();
			void optimizeParameters();
		
		private:

			/** @name Private Attributes */
			
			/** number of model parameters  */
			int num_of_parameters_;

			RegistryEntry* entry_;
			std::vector<QLineEdit*> edits_;
			QLineEdit* edit_;
			QLineEdit* kedit_;
			std::vector<double> parameters_;
			int k_ ;
		};
	}
}

 #endif
