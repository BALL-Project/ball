/* validationDialog.h
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


#ifndef VALDIALOG_H
#define VALDIALOG_H

#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QLineEdit>
#include <QtGui/QGridLayout>
#include <QtGui/QLabel>
#include <QtGui/QComboBox>

#include <BALL/APPLICATIONS/QuEasyViz/validationItem.h>

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

				ValidationDialog(ValidationItem* val_item, ModelItem* model);

				/** standard destructor */
				~ValidationDialog();	

				/** @name Accessors */		

				/** returns the k value */
				int k();	
				
				int getValidationStatistic() {return statistic_;};
	
				
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
				
				QComboBox* statistic_box_;
				int statistic_;
				
				QLineEdit* n_of_samples_edit_;
				QLineEdit* n_of_runs_edit_;
				QLineEdit* n_of_ncv_folds_edit_;
				QLineEdit* val_fraction_edit_;
				int n_of_runs_;
				int n_of_samples_;
				int n_of_ncv_folds_;
				double val_fraction_;
		};
	}
}

 #endif

