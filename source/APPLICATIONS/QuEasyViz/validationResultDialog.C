/* TRANSLATOR BALL::QSAR

		Necessary for lupdate.
*/

/* validationResultDialog.C
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

#include <validationResultDialog.h>

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

		ValidationResultDialog::ValidationResultDialog(ValidationItem* item)
		{
			//return if there's no parent
			if (item == NULL)
			{
				return;
			}

			QDialogButtonBox* buttons = new QDialogButtonBox(QDialogButtonBox::Ok,Qt::Horizontal, this);
			QPushButton* print_button = new QPushButton("Save to File", buttons);

			QVBoxLayout* mainLayout = new QVBoxLayout();
			QGroupBox* resultGroup = new QGroupBox(tr("Validation Results"),this);
			QGridLayout* layout = new QGridLayout();
			QVBoxLayout* resultGroupLayout = new QVBoxLayout();

			int type = item->getValidationType();

			if (type < 4 || type==5)
			{
				String train_fit ="R^2";
				String pred_qual="Q^2";
				String n = item->modelItem()->getRegistryEntry()->getStatName(item->getValidationStatistic());
				
				if(n!="") 
				{
					train_fit = n+"<br>on training data";
					pred_qual = "predictive<br>"+n;			
				}
				
				QLabel* rlabel = new QLabel(train_fit.c_str(),this);
				layout->addWidget(rlabel, 0,1);
				QString tmp;
				QLabel* qlabel=NULL;
				
				if(type<3)
				{
					qlabel = new QLabel(tmp.setNum(item->k())+ " fold "+pred_qual.c_str(),this);
				}
				else if(type==3) // boostrap
				{
					qlabel = new QLabel(QString(pred_qual.c_str())+" of "+tmp.setNum(item->numOfSamples())+ "\nbootstrap samples",this);
				}
				else if(type==5)
				{
					qlabel = new QLabel(tmp.setNum(item->getNoExternalFolds())+ " fold nested "+pred_qual.c_str(),this);
				}
				
				layout->addWidget(qlabel, 0,2);

				QLabel* rvaluelabel = new QLabel(QString(((String)(item->getR2())).c_str()),this);
				layout->addWidget(rvaluelabel,1,1);
				QLabel* qvaluelabel = new QLabel(QString(((String)(item->getQ2())).c_str()),this);
				layout->addWidget(qvaluelabel,1,2);

				resultGroup->setLayout(layout);
			}	
			else if (type == 4)
			{
				QString value;
				float num;

				QStringList labels;
				labels << "# Tests" << "R^2" << "Q^2";

				QTableWidget* table = new QTableWidget(item->resultOfRandTest()->Nrows(), 3, this);	
				table->verticalHeader()->hide();
				table->setHorizontalHeaderLabels (labels);
				table->setAlternatingRowColors(true);
				table->setDragDropMode(QAbstractItemView::NoDragDrop);
				table->setEditTriggers(QAbstractItemView::NoEditTriggers);
				table->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
				table->horizontalHeader()->setResizeMode(2,QHeaderView::Stretch); 
			
				for (int i = 1; i <= item->resultOfRandTest()->Nrows();i++)
				{
					value.setNum(i);
					QTableWidgetItem* num_of_Test = new QTableWidgetItem(value);
					table->setItem(i-1, 0, num_of_Test);

					num = (*(item->resultOfRandTest()))(i,item->resultOfRandTest()->Ncols()-1);
					value.setNum(num);
					QTableWidgetItem* r = new QTableWidgetItem(value);
					table->setItem(i-1, 1, r);

					num = (*(item->resultOfRandTest()))(i,item->resultOfRandTest()->Ncols());
					value.setNum(num);
					QTableWidgetItem* q = new QTableWidgetItem(value);
					table->setItem(i-1, 2, q);
				}

				QScrollArea* scrollArea = new QScrollArea(this);
				scrollArea->setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
				scrollArea->setVerticalScrollBarPolicy ( Qt::ScrollBarAsNeeded );
				scrollArea->setFrameShape(QFrame::NoFrame);
				scrollArea->setWidget(table);
				scrollArea->setWidgetResizable(true);
			
				resultGroupLayout->addWidget(scrollArea);
				resultGroup->setLayout(resultGroupLayout);
			}
			mainLayout->addWidget(resultGroup);
			mainLayout->addWidget(buttons);
			mainLayout->addStretch(1);
			setLayout(mainLayout);	
			setWindowTitle("Validation Results for " + item->name());

			connect(buttons, SIGNAL(accepted()), this, SLOT(accept()));
			connect(print_button, SIGNAL(clicked()), this, SLOT(saveToFile()));
		}

		ValidationResultDialog::ValidationResultDialog()
		{
		}

		ValidationResultDialog::~ValidationResultDialog()
		{
		}

		void ValidationResultDialog::saveToFile()
		{
			QString filename = QFileDialog::getSaveFileName(this, tr("Save File as"),"results.txt",tr("text (*.txt)"));

			QFile file(filename);
			if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
			{
         			return;
			}

			QTextStream out(&file);
		}
	}
}
