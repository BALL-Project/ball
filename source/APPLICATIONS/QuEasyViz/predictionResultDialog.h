#ifndef PREDDIALOG_H
#define PREDDIALOG_H

#include <QtWidgets/QDialog>
#include <QtWidgets/QTableWidget>
#include <predictionItem.h>


namespace BALL
{
	namespace VIEW
	{
		/** @class PredictionResultDialog
		* @brief a dialog for displaying the results of the prediction of activity values for all compounds of a dataset
		*
		* @todo
		*/
		class PredictionResultDialog : public QDialog
		{	
			Q_OBJECT

			public:
				/** @name Constructors and Destructors */	

				/** constructor
				* @param item the PredictionItem connected to this dialog
				*/
				PredictionResultDialog(PredictionItem* item);	
			
				/** destructor
				*/
				~PredictionResultDialog();

			protected slots:
				void saveToFile();
			
			private:
				/** @name Private Attributes */
			
				/** the results of the validation */
				const list<Eigen::VectorXd >* results_;
			
				QString file_name_;

				/** the compund names*/
				const vector<string>* compound_names_;	
				
				PredictionItem* pred_item_;
				
				QTableWidget* table_;		
		};			
	}
}

#endif
