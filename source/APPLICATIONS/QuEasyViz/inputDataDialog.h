#ifndef DATADIALOG_H
#define DATADIALOG_H

#include <QtWidgets/QDialog>

#include <inputDataItem.h>

namespace BALL
{
	namespace VIEW
	{
		/** @class InputDataDialog
		* @brief 
		*
		* @todo
		*/
		class InputDataDialog : public QDialog
		{	
			Q_OBJECT

			public:
				/** @name Constructors and Destructors */	

				/** constructor
				* @param item the InputDataItem connected to this dialog
				*/
				InputDataDialog(InputDataItem* item);	
			
				/** destructor
				*/
				~InputDataDialog();

			protected slots:
				void saveToFile();

			protected:
				/** the compund names*/
				QString file_name_;
				const vector<string>* compound_names_;
			
		};			
	}
}

#endif
