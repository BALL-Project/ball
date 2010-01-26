/* knnModel.C
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

#include <BALL/QSAR/knnModel.h>

namespace BALL
{
	namespace QSAR
	{

		// ALLModel::kw_ will not be used so it can be set to an arbitrary value
		KNNModel::KNNModel(const QSARData& q, int k) 
			: ALLModel(q,1)
		{
			type_ = "KNN";
			k_ = k;
			default_no_opt_steps_ = 20;
		}

		void KNNModel::calculateWeights(Matrix<double>& dist, Vector<double>& w)
		{
			// set first k entries of similarity vector w to 1
			//   and all other entries to 0
			//   ==> KNN instead of ALL	
			
			w.resize(dist.getColumnCount());
			w=1;
			
			SortedList<pair<double,int> > ranking;
			for(int i=1; i<=dist.Ncols();i++)
			{
				ranking.insert(make_pair(dist(1,i),i));
			}
			ranking.front();
			for(int i=0; i<k_ && ranking.hasNext(); i++)
			{
				ranking.next();  // skip the k nearest neighbors
			}
			while(ranking.hasNext()) 
			{			  
				w(ranking.next().second) = 0;
			}
		}


		void KNNModel::setParameters(vector<double>& v)
		{
			if(v.size()!=2)
			{
				String c = "Wrong number of model parameters! Needed: 2;";
				c = c+" given: "+String(v.size());
				throw Exception::ModelParameterError(__FILE__,__LINE__,c.c_str());
			}
			k_ = (int) v[0];
			lambda_ = v[1];
		}

		vector<double> KNNModel::getParameters() const
		{
			vector<double> d;
			d.push_back(k_);
			d.push_back(lambda_);
			return d;
		}

		bool KNNModel::optimizeParameters(int d, int no_steps)
		{
			double best_q2=0;
			int best_no=1;
			for(int i=1; i<=no_steps && i<=(int)data->getNoSubstances();i++)
			{
				k_=i;
				validation->crossValidation(d);

				if(validation->getQ2()>best_q2)
				{
					best_q2=validation->getQ2();
					best_no=i;
				}
				
				if(validation->getQ2()<best_q2*0.5)
				{
					break;
				}
			}
			k_=best_no;
			validation->setQ2(best_q2);
			
			return 1;
		}
	}
}