/* modelPropertiesDialog.h
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

#ifndef MODELPROPERTIESDIALOG_H
#define MODELPROPERTIESDIALOG_H

#include <BALL/QSAR/registry.h>
#include <BALL/QSAR/Model.h>
#include <vector>

#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QLineEdit>
#include <QtGui/QGridLayout>
#include <QtGui/QLabel>


namespace BALL
{
	namespace VIEW
	{

		class ModelPropertiesDialog : public QDialog
		{
			Q_OBJECT
		
		public:

			/** @name Constructors and Destructors */

			/** constructor */
			ModelPropertiesDialog();

			/** constructor
			* @param entry @todo
			*/
			ModelPropertiesDialog(RegistryEntry* entry);

			/** destructor */
			~ModelPropertiesDialog();

			/** @name Accessors */
			std::vector<double> parameters();
			int k();
			
		
		public slots:
		
			/** @name Public Slots */
			void apply();
			void optimizeParameters();
		
		private:

			/** @name Private Attributes */
			
			/** number of model parameters  */
			int num_of_parameters_;

			RegistryEntry* entry_;
			std::vector<QLineEdit*> edits_;
			QLineEdit* edit_;
			QLineEdit* kedit_;
			std::vector<double> parameters_;
			int k_ ;
		};
	}
}

 #endif
