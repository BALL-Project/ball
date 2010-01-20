/* predictionResultDialog.h
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

#ifndef PREDDIALOG_H
#define PREDDIALOG_H

#include <QtGui/QDialog>
#include <QtGui/QTableWidget>
#include <BALL/APPLICATIONS/QuEasyViz/predictionItem.h>


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
				const list<Vector<double> >* results_;
			
				QString file_name_;

				/** the compund names*/
				const vector<string>* compound_names_;	
				
				PredictionItem* pred_item_;
				
				QTableWidget* table_;		
		};			
	}
}

#endif
