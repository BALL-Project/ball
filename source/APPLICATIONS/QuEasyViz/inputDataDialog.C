/* inputDataDialog.C
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

#include <inputDataDialog.h>

#include <QtGui/QDialogButtonBox>
#include <QtGui/QFileDialog>
#include <QtGui/QLabel>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QPushButton>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtGui/QScrollArea>
#include <QtGui/QTableWidget>
#include <QtGui/QHeaderView>

namespace BALL
{
	namespace VIEW
	{

		InputDataDialog::InputDataDialog(InputDataItem* item)	
		{
			///return if there's no parent
			if (item == NULL)
			{
				return;
			}
			file_name_ = item->name();
			if (item->data())
			{
				compound_names_ = item->data()->getSubstanceNames();
			}
			QDialogButtonBox* buttons = new QDialogButtonBox(QDialogButtonBox::Ok,Qt::Horizontal, this);
			QPushButton* print_button = new QPushButton(tr("Save to File"), buttons);
			QVBoxLayout* mainLayout = new QVBoxLayout();
			QVBoxLayout* resultGroupLayout = new QVBoxLayout();
			QGroupBox* resultGroup = new QGroupBox(tr("Compounds"),this);

			if (compound_names_->size() > 0)
			{
				QStringList labels;
				labels << "Compound";
			
				QTableWidget* table = new QTableWidget(compound_names_->size(), 1, this);	
				table->verticalHeader()->hide();
				table->setHorizontalHeaderLabels (labels);
				table->setAlternatingRowColors(true);
				table->setDragDropMode(QAbstractItemView::NoDragDrop);
				table->setEditTriggers(QAbstractItemView::NoEditTriggers);
				table->horizontalHeader()->setResizeMode(QHeaderView::Stretch); 
			
				for (unsigned int i=0; i< compound_names_->size(); i++)
				{
					QTableWidgetItem* name = new QTableWidgetItem(QString(compound_names_->at(i).c_str()));
					table->setItem(i, 0, name);	
				}
			
				QScrollArea* scrollArea = new QScrollArea(this);
				scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
				scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
				scrollArea->setFrameShape(QFrame::NoFrame);
				scrollArea->setWidget(table);
				scrollArea->setWidgetResizable(true);
			
				resultGroupLayout->addWidget(scrollArea);
				resultGroup->setLayout(resultGroupLayout);
			}
			else
			{
				QLabel* label = new QLabel(tr("No data available, please execute pipeline first."));
				resultGroupLayout->addWidget(label);
				resultGroup->setLayout(resultGroupLayout);
			}
			
			mainLayout->addWidget(resultGroup);
			mainLayout->addWidget(buttons);
			mainLayout->addStretch(1);
			setLayout(mainLayout);	
			setWindowTitle(tr("Descriptors in ") + item->name());

			connect(buttons, SIGNAL(accepted()), this, SLOT(accept()));
			connect(print_button, SIGNAL(clicked()), this, SLOT(saveToFile()));
		}

		InputDataDialog::~InputDataDialog()
		{
		}

		void InputDataDialog::saveToFile()
		{
			QString filename = QFileDialog::getSaveFileName(this, tr("Save File as"), 
					                                            file_name_ + tr("_compounds") + ".txt",
																											tr("text") + " (*.txt)");
			if (filename.isEmpty())
			{
				return;
			} 
			
			QFile file(filename);
			if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
			{
				return;
			}

			QTextStream out(&file);

			for (unsigned int i=0; i< compound_names_->size(); i++)
			{
				out << QString(compound_names_->at(i).c_str()) << "\n";	
			}
		}
	}
}
