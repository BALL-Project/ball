/* inputPlotter.h
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

#ifndef INPUT_PLOTTER
#define INPUT_PLOTTER

#include  <plotter.h>
#include  <inputDataItem.h>


namespace BALL
{
	namespace VIEW
	{		
		class InputPlotter : public Plotter
		{
			Q_OBJECT
			
			public:
				InputPlotter(InputDataItem* item);
				
				~InputPlotter();
				
		
			protected slots:
				
				void sortChangeState();
				
			private:
				InputDataItem* input_item_;
				QSARData* data_;
				
				QCheckBox* sort_checkbox_;
				
				bool sort_;
				
				void plot(bool zoom);
				
				void plotActivity(bool zoom);
				
				void plotSortedActivity(bool zoom);
			
	
		};
	}
}

#endif //INPUT_PLOTTER

