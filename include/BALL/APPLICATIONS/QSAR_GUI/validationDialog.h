
#ifndef VALDIALOG_H
#define VALDIALOG_H

#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QLineEdit>
#include <QtGui/QGridLayout>
#include <QtGui/QLabel>

#include <BALL/APPLICATIONS/QSAR_GUI/validationItem.h>

namespace BALL
{
	namespace VIEW
	{

		/** @class ValidationDialog
		* @brief a dialog 
		*
		* @todo
		*/
		class ValidationDialog : public QDialog
		{
			Q_OBJECT
		
			public:
				/** @name Constructors and Destructors
				*/
				/** standard constructor */
				ValidationDialog();

				ValidationDialog(ValidationItem* val_item);

				/** standard destructor */
				~ValidationDialog();	

				/** @name Accessors */		

				/** returns the k value */
				int k();	
	
			public slots:	
				/** @name public slots */
		
				/** reads in the k value given by the user */
				void applyInput();
	
			private:
			
				/** @name Attributes
				*/
		
				ValidationItem* val_item_;

				QLineEdit* k_edit_;

				/** the k value given by the user which is needed for k fold cross validation*/
				int k_;
				
				QLineEdit* n_of_samples_edit_;
				QLineEdit* n_of_runs_edit_;
				int n_of_runs_;
				int n_of_samples_;
		};
	}
}

 #endif

