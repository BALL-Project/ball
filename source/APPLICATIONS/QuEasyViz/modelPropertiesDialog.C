/* TRANSLATOR BALL::QSAR

		Necessary for lupdate.
*/


/* modelPropertiesDialog.C
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

#include <modelPropertiesDialog.h>
#include <QtGui/QPushButton>
#include <QtGui/QMessageBox>
#include <QtGui/QCheckBox>

#include <iostream>

using namespace BALL::VIEW;

ModelPropertiesDialog::ModelPropertiesDialog(RegistryEntry* entry)
	: entry_(entry)
{
	num_of_parameters_ = entry_->parameterNames.size();
	QGridLayout *layout = new QGridLayout;

	QDialogButtonBox* Buttons = new QDialogButtonBox(QDialogButtonBox::Ok |QDialogButtonBox::Cancel);

	QPushButton* applyButton = new QPushButton(tr("Apply"));
	QPushButton* optimizeButton = new QPushButton(tr("Optimize Parameters"));
	optimizeButton->setEnabled(false);

	Buttons->addButton(applyButton, QDialogButtonBox::ApplyRole);
	Buttons->addButton(optimizeButton, QDialogButtonBox::ActionRole);

	QLabel* klabel = new QLabel(tr("k-fold cross validation"));
	kedit_ = new QLineEdit(); 
	kedit_->setEnabled(false);

	for (int i=0; i < num_of_parameters_; i++)
	{
		QLabel* label = new QLabel;	
		label->setText(QString(entry_->parameterNames[i].c_str()));
		edit_ = new QLineEdit();
		edits_.push_back(edit_);
		layout->addWidget(label, i,1);
		layout->addWidget(edit_, i ,2);
	}

	layout->addWidget(Buttons,num_of_parameters_,3);
	layout->addWidget(kedit_, num_of_parameters_,2);
	layout->addWidget(klabel, num_of_parameters_,1);

	this->setLayout(layout);

	std::string ab_name = entry->name_abreviation;
	std::string name = entry->name;
	
	this->setWindowTitle("Model Properties: " + QString(name.c_str()) + " (" + QString(ab_name.c_str()) + ") ");

	connect(Buttons, SIGNAL(accepted()), this, SLOT(accept()));
	connect(Buttons, SIGNAL(rejected()), this, SLOT(reject()));
	connect(applyButton, SIGNAL(clicked()), this, SLOT(apply()));
	connect(optimizeButton, SIGNAL(clicked()), this, SLOT(optimizeParameters()));
	connect(Buttons, SIGNAL(accepted()), this, SLOT(apply()));
}

ModelPropertiesDialog::ModelPropertiesDialog()
{
}

ModelPropertiesDialog::~ModelPropertiesDialog()
{
}

std::vector<double> ModelPropertiesDialog::parameters()
{
	return parameters_;
}

int ModelPropertiesDialog::k()
{
	return k_;
}

void ModelPropertiesDialog::apply()
{	
	parameters_.clear();
	bool ok;
	double num = 0.;
	for (unsigned int i=0; i < edits_.size(); i++)
	{
		num = edits_[i]->text().toDouble(&ok);
		if (ok)
		{	
			parameters_.push_back(num);
		}
		else 
		{	
			QMessageBox::about(this, tr("Error"),tr("Invalid parameter"));
			parameters_.push_back(0);
		}
	}
	int k = kedit_->text().toInt(&ok);
	if (ok)
	{
		k_ = k;
	}
	else
	{
		QMessageBox::about(this, tr("Error"),tr("Invalid k"));
	}
	
}

void ModelPropertiesDialog::optimizeParameters()
{
	
	if(entry_->optimizable)
	{	
		/*
		entry_->optimizeParameters();
	
		parameters_ = entry_->getParameters();
		for (int i=0; i< parameters_.size(); i++)
		{
			edits_[i]->setText(QString::number(parameters_[i]));
			edits_[i]->update();
		}
		*/
		std::cout << "Optimizable" << std::endl;
	}
	else 
	{
		std::cout << "Not optimizable" << std::endl;
	}

}

