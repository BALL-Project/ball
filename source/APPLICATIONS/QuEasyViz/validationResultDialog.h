/* validationResultDialog.h
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
