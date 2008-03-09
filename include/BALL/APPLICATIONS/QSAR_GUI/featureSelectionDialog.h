#ifndef FSDIALOG_H
#define FSDIALOG_H

#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QLineEdit>
#include <QtGui/QCheckBox>
#include <QtGui/QGridLayout>
#include <QtGui/QLabel>

#include <BALL/APPLICATIONS/QSAR_GUI/featureSelectionItem.h>

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
				FeatureSelectionDialog(FeatureSelectionItem* fsitem);

				/** destructor */
				~FeatureSelectionDialog();	
		
				
				/** @name Accessors*/

				/** returns k for k-cross-validation*/
				int k();
			
				/** */
				bool optimize();	
	
			public slots:	
				void applyInput();
	
			private:
			
				/** @name Private Attributes*/
				QLineEdit* k_edit_;
				QCheckBox* optimize_parameters_;
				int k_;
				bool optimize_;
				FeatureSelectionItem* fs_item_;
		};
	}
}

 #endif

