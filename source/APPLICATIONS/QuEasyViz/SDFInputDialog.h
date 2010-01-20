/* SDFInputDialog.h
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

#ifndef SDFInputDialog_H
#define SDFInputDialog_H

#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QLineEdit>
#include <QtGui/QCheckBox>
#include <QtGui/QGridLayout>
#include <QtGui/QLabel>

#include <BALL/APPLICATIONS/QuEasyViz/SDFInputDataItem.h>

namespace BALL
{
	namespace VIEW
	{
		/** @class SDFInputDialog
		* @brief a dialog
		*
		* @todo
		*/
		class SDFInputDialog : public QDialog
		{
		
			Q_OBJECT
		public:
			/** @name Constructors and Destructors
			*/
			SDFInputDialog(SDFInputDataItem* item);
			SDFInputDialog();
			~SDFInputDialog();
		
			/** @name Accessors
			*/
		
			/** returns the activity numbers
			*/ 
			SortedList<int> numbers();
		
			bool centerDescriptorValues();
			bool centerResponseValues();
			bool inputOk();
		
		public slots:
			void getNumbers();
			void classNamesChange();

		private:
		
			/** @name Attributes
			*/
			QLineEdit* activity_edit_;
			QCheckBox* sd_descriptors_checkbox_;
			QCheckBox* center_descriptor_values_;
			QCheckBox* center_response_values_;
			QCheckBox* class_names_checkbox_;
			SortedList<int> numbers_;
			bool input_ok_;
			SDFInputDataItem* input_item_;
			vector<String>* property_names_;

		};
	}
}

 #endif
