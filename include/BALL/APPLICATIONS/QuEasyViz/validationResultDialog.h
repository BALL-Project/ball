#ifndef VALRESDIALOG_H
#define VALRESDIALOG_H

#include <QtGui/QDialog>
#include <BALL/APPLICATIONS/QuEasyViz/validationItem.h>

namespace BALL
{
	namespace VIEW
	{
		/** @class ValidationResultDialog
		* @brief A simple dialog for displaying the results of a model validation 
		* @todo
		*/
		class ValidationResultDialog : public QDialog
		{	
			Q_OBJECT

			public:
				/** @name Constructors and Destructors */	

				/** constructor
				* @param item pointer to the ValidationItem connected to this dialog
				*/
				ValidationResultDialog(ValidationItem* item);	
			
				/** standard constructor
				*/
				ValidationResultDialog();
			
				/** destructor
				*/
				~ValidationResultDialog();

			protected slots:
				/** saves the result of the validation to a text file */
				void saveToFile();
		};			
	}
}

#endif
