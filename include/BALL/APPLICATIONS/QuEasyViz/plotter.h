/* plotter.h
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

#ifndef PLOTTER_H
#define PLOTTER_H

#include <qwt_plot.h>
#include <BALL/APPLICATIONS/QuEasyViz/dataItem.h>
#include <list>
#include <QString>
#include <QtGui/QDialog>
#include <QMouseEvent>
#include <QPushButton>
#include <QtGui/QDialogButtonBox>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QComboBox>

#include <qwt_symbol.h>
#include <qwt_plot_zoomer.h>


namespace BALL
{
	namespace VIEW
	{
		class Plotter : public QFrame
		{	
			Q_OBJECT		
			
			public: 
				
				/** the symbol to be used for each data point within the plots */
				QwtSymbol data_symbol;
				
				/** the font to to be used for each data point within the plots */
				QFont data_label_font;
				
				/** determines how each label is aligned with respect to its data point */
				Qt::Alignment data_label_alignment;
				
				/** the symbol to be used for each data point within the plots during printing or saving to file.\n
				By default it is equal to data_symbol */
				QwtSymbol print_data_symbol;
				
				/** determines whether or not labels for each data point are to be drawn */
				bool show_data_labels;
				
				/** prints the current contents of the plot to a file */
				void printToFile(QString& file);
				
				~Plotter();
				
				/** Call this (overloaded) method every new results are available within the connected item */
				virtual void update() {};

				
				
			protected slots:
				
				void labelsChangeState();
				
				void save();
				
				void print();
				
				virtual void activityChange();
				
			
			
			protected:
				/** constructor */
				Plotter(DataItem* item);
				
				/** (re)plots all data. \n
				Must be implemented by derived classes. */
				virtual void plot(bool zoom=1) = 0;
				
				QColor generateColor(int no_colors, int current_no);
				
				DataItem* item_;
				
				QwtPlot* qwt_plot_;
				
				/** contains a name for each plotted data point.\n
				Can be used e.g. for mouse-over effects... */
				std::list<QString> names_;
				
				QPushButton* okButton_;
				QCheckBox* show_labels_;
				QPushButton* saveButton_;
				QPushButton* printButton_;
				QHBoxLayout* buttonsLayout_;
				QVBoxLayout* main_layout_;
				QComboBox* activity_combobox_;
				
				QwtPlotZoomer* zoomer_;
				
				int selected_activity_;	
		};
	}
}

#endif // PLOTTER_H
