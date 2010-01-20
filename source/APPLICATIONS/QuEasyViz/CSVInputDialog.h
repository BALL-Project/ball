/* CSVInputDialog.h
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

#ifndef CSVINPUTDIALOG_H
#define CSVINPUTDIALOG_H

#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QLineEdit>
#include <QtGui/QCheckBox>
#include <QtGui/QGridLayout>
#include <QtGui/QLabel>
#include <QtGui/QComboBox>

#include <BALL/APPLICATIONS/QuEasyViz/CSVInputDataItem.h>

namespace BALL
{
	namespace VIEW
	{
		/** @class CSVInputDialog
		* @brief a dialog
		*
		* @todo
		*/
		class CSVInputDialog : public QDialog
		{
		
			Q_OBJECT
		public:
			/** @name Constructors and Destructors
			*/	
			CSVInputDialog(CSVInputDataItem* item);
			~CSVInputDialog();
		
			/** @name Accessors
			*/
		
			/** returns the activity numbers
			*/ 
			int numberOfActivities();
			bool xLabels();
			bool yLabels();
			bool inputOk();
		
		public slots:
			void readNumY();
			void classNamesChange();

		private:
		
			/** @name Attributes
			*/
			QComboBox* separator_box_;
			QLineEdit* activity_edit_;
			QCheckBox* x_labels_;
			QCheckBox* y_labels_;
			QCheckBox* center_descriptor_values_;
			QCheckBox* center_response_values_;
			QCheckBox* class_names_checkbox_;
			QGridLayout* layout_;
			QLabel* alabel_;
			QLabel* blabel_;
			int no_y_;
			bool input_ok_;
			CSVInputDataItem* input_item_;
		};
	}
}

 #endif
