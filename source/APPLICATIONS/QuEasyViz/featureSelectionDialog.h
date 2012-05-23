#ifndef FSDIALOG_H
#define FSDIALOG_H

#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QComboBox>

#include <featureSelectionItem.h>

namespace BALL
{
	namespace VIEW
	{

		/** @class FeatureSelectionDialog
		* @brief a dialog
		*
		* @todo
		*/
		class FeatureSelectionDialog : public QDialog
		{
			Q_OBJECT
		
			public:
				/** @name Constructors and Destructors
				*/
				/** constructor */
				FeatureSelectionDialog();
				
				/**constructor 
				* @param fsitem FeatureSelectionItem for which the dialog reads in user input
				*/
				FeatureSelectionDialog(FeatureSelectionItem* fsitem, ModelItem* model);

				/** destructor */
				~FeatureSelectionDialog();	
		
				
				/** @name Accessors*/

				/** returns k for k-cross-validation*/
				int k();
				
				int getValidationStatistic() {return statistic_;};
	
			public slots:	
				void applyInput();
	
			private:
			
				/** @name Private Attributes*/
				QLineEdit* edit_;
				QLineEdit* edit2_;
				QLineEdit* cutoff_;
				QCheckBox* checkbox_post_optimization_model_par_;
				QCheckBox* checkbox_post_optimization_kernel_par_;
				int k_;
				double cor_threshold_;
				bool post_optimization_model_par_;
				bool post_optimization_kernel_par_;
				FeatureSelectionItem* fs_item_;
				
				QComboBox* statistic_box_;
				int statistic_;

				
				friend class FeatureSelectionItem;
		};
	}
}

 #endif

