/* predictionResultDialog.C
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


#include <predictionResultDialog.h>
#include <mainWindow.h>

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
#include <QtGui/QHeaderView>

using namespace BALL::QSAR;

namespace BALL
{
	namespace VIEW
	{

		PredictionResultDialog::PredictionResultDialog(PredictionItem* item)	
		{
			//return if there's no parent
			if (item == NULL)
			{
				return;
			}
			pred_item_ = item;

			QDialogButtonBox* buttons = new QDialogButtonBox(QDialogButtonBox::Ok,Qt::Horizontal, this);
			QPushButton* print_button = new QPushButton("Save to File", buttons);

			QVBoxLayout* mainLayout = new QVBoxLayout();
			QVBoxLayout* resultGroupLayout = new QVBoxLayout();

			QGroupBox* resultGroup = new QGroupBox(tr("Predicted Activity Values"),this);

			file_name_ = item->name();
			results_ = item->results();

			QStringList labels;
			labels << "Compound";
			
			bool show_expected=0;
			const QSARData* test_data = 0;
			if(item->getTestData()==0 || results_->size()==0) return; // no prediction=>nothing to be displayed

			test_data = item->getTestData();
			Size no_y=test_data->getNoResponseVariables();
			if(no_y>=1)
			{
				show_expected=1;
				if(no_y==1) labels<<"Prediction"<<"Expected";
				else
				{
					for(uint i=0; i<no_y;i++)
					{
						String p = "Prediction"+String(i);
						String e = "Expected"+String(i);
						labels<<p.c_str()<<e.c_str();
					}
				}
			}
			else
			{
				for(Size act=0; act<results_->front().getSize(); act++)
				{
					String p = "Prediction"+String(act);
					labels<<p.c_str();
				}
			}
			
			compound_names_ = test_data->getSubstanceNames();
			Size no_columns = 1;
			if(no_y>=1) no_columns+=2*no_y;
			else no_columns+=results_->front().getSize();
			table_ = new QTableWidget(results_->size(), no_columns, this);
			table_->verticalHeader()->hide();
			table_->setHorizontalHeaderLabels (labels);
			table_->setAlternatingRowColors(true);					
			table_->setDragDropMode(QAbstractItemView::NoDragDrop);		
			table_->setEditTriggers(QAbstractItemView::NoEditTriggers);

			if(results_->size()==compound_names_->size())
			{
				int i = 0;
				for (list<Vector<double> >::const_iterator it = results_->begin(); it != results_->end(); it++)
				{
					QTableWidgetItem* name = new QTableWidgetItem(QString(compound_names_->at(i).c_str()));
					table_->setItem(i, 0, name);
					vector<double>* e = 0;
					if(show_expected) e = test_data->getActivity(i);
					
					for(uint act=0; act<it->getSize(); act++)
					{
						QTableWidgetItem* pred = new QTableWidgetItem(QString((((String)(*it)(1+act)).c_str())));
						table_->setItem(i, 1+(1+show_expected)*act, pred);
						
						if(show_expected)
						{
							QTableWidgetItem* expected = new QTableWidgetItem(QString(((String((*e)[act])).c_str())));
							table_->setItem(i, 2+2*act, expected);
						}
					}
					
					delete e;
					i++;
				}
			}

			QScrollArea* scrollArea = new QScrollArea(this);
			scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
			scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
			scrollArea->setFrameShape(QFrame::NoFrame);
			scrollArea->setWidget(table_);
			scrollArea->setWidgetResizable(true);

			resultGroupLayout->addWidget(scrollArea);
			resultGroup->setLayout(resultGroupLayout);
			
			mainLayout->addWidget(resultGroup);
			mainLayout->addWidget(buttons);
			setLayout(mainLayout);	
			setWindowTitle("Predicted Activity Values for " + item->name());
			table_->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
			
			uint width = 0;
			for(int i=0; i<table_->columnCount();i++)
			{
				width+=table_->columnWidth(i);
			}
			width+=65;
			uint mainWindow_width = item->view()->data_scene->main_window->size().width();
			if(width<mainWindow_width) resize(width,450);
			else resize(mainWindow_width,450);
			table_->setSortingEnabled(1);

			connect(buttons, SIGNAL(accepted()), this, SLOT(accept()));
			connect(print_button, SIGNAL(clicked()), this, SLOT(saveToFile()));
		}


		PredictionResultDialog::~PredictionResultDialog()
		{
		}


		void PredictionResultDialog::saveToFile()
		{
			QString filename = QFileDialog::getSaveFileName(this, tr("Save File as"),file_name_ +"_prediction_results.txt",tr("text (*.txt)"));
			QFile file(filename);
			if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
			{
				return;
			}
			ofstream out(file.fileName().toStdString().c_str());
			
			uint no_rows=table_->rowCount();
			uint no_cols=table_->columnCount();
			bool use_selection = (table_->selectedItems().size()>1);

			list<int> selected_columns;
			list<int> selected_rows;
			list<int>::iterator col_it;
			list<int>::iterator row_it;
			if(use_selection) 
			{
				// find selected columns 
				QList<QTableWidgetSelectionRange> ranges = table_->selectedRanges();
				for(int i=0; i<(int)no_cols; i++)
				{
					for(QList<QTableWidgetSelectionRange>::iterator it=ranges.begin();it!=ranges.end();it++)
					{
						if(i<=it->rightColumn() && i>=it->leftColumn())
						{
							selected_columns.push_back(i);
							break;
						}
					}
				}
				col_it=selected_columns.begin();
				
				// find selected rows 
				for(int i=0; i<(int)no_rows; i++)
				{
					for(QList<QTableWidgetSelectionRange>::iterator it=ranges.begin();it!=ranges.end();it++)
					{
						if(i<=it->bottomRow() && i>=it->topRow())
						{
							selected_rows.push_back(i);
							break;
						}
					}
				}
				row_it=selected_rows.begin();
			}
			
			// print table-header
			for(int i=0; i<(int)no_cols; i++)
			{
				bool write=0;
				if(use_selection)
				{
					if(col_it!=selected_columns.end() && *col_it==i) 
					{
						write=1;
						col_it++;
					}			
				}
				if(!use_selection || write) out<<table_->horizontalHeaderItem(i)->text().toStdString()<<"\t";
			
			}	
			out<<endl;
			
			if(use_selection) col_it=selected_columns.begin();
			
			// print data
			for(int i=0; i<(int)no_rows; i++)
			{
				bool wrote_item=0;
				for(int j=0; j<(int)no_cols; j++)
				{
					if(!use_selection || table_->item(i,j)->isSelected())
					{
						out<<table_->item(i,j)->text().toStdString()<<"\t";
						wrote_item=1;
					}
					else if(col_it!=selected_columns.end() && *col_it==j && row_it!=selected_rows.end() && *row_it==i)
					{
						out<<"\t";
						col_it++;
					}
				}
				if(wrote_item) 
				{
					out<<endl;
					if(use_selection) 
					{
						row_it++;
						col_it=selected_columns.begin();
					}
				}
			}
		}
	}
}