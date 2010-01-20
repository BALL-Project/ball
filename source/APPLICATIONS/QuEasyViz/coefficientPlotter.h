/* coefficientPlotter.h
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

#ifndef COEFF_PLOTTER
#define COEFF_PLOTTER

#include  <BALL/APPLICATIONS/QuEasyViz/plotter.h>
#include  <BALL/APPLICATIONS/QuEasyViz/modelItem.h>


namespace BALL
{
	namespace VIEW
	{
		class CoefficientPlotter : public Plotter
		{
			public:
				CoefficientPlotter(ModelItem* model_item);
		
			
			private:
				ModelItem* model_item_;
				
				void plot(bool zoom);
		
			
			
		};
	}
}

#endif //COEFF_PLOTTER


