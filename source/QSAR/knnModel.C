#include <BALL/QSAR/knnModel.h>

using namespace std;

namespace BALL
{
	namespace QSAR
	{

		// ALLModel::kw_ will not be used so it can be set to an arbitrary value
		KNNModel::KNNModel(const QSARData& q, int k) 
			: ALLModel(q, 1)
		{
			type_ = "KNN";
			k_ = k;
			default_no_opt_steps_ = 20;
		}

		void KNNModel::calculateWeights(Eigen::MatrixXd& dist, Eigen::VectorXd& w)
		{
			// set first k entries of similarity vector w to 1
			//   and all other entries to 0
			//   == > KNN instead of ALL	
			
			w.resize(dist.cols());
			w.setConstant(1.0);
			
			std::multiset<pair<double, int> > ranking;
			for (int i = 0; i < dist.cols(); i++)
			{
				ranking.insert(make_pair(dist(0, i), i));
			}
			
			std::multiset<pair<double, int> >::iterator r_it = ranking.begin();
			
			for (int i = 0; i < k_ && r_it != ranking.end(); i++, ++r_it)
			{ }			// skip the k nearest neighbors
			
			for (; r_it != ranking.end(); ++r_it)
			{			  
				w(r_it->second) = 0;
			}
		}


		void KNNModel::setParameters(vector<double>& v)
		{
			if (v.size() != 2)
			{
				String c = "Wrong number of model parameters! Needed: 2;";
				c = c+" given: "+String(v.size());
				throw Exception::ModelParameterError(__FILE__, __LINE__, c.c_str());
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
			double best_q2 = 0;
			int best_no = 1;
			for (int i = 1; i <= no_steps && i <= (int)data->getNoSubstances(); i++)
			{
				k_ = i;
				validation->crossValidation(d);

				if (validation->getQ2() > best_q2)
				{
					best_q2 = validation->getQ2();
					best_no = i;
				}
				
				if (validation->getQ2() < best_q2*0.5)
				{
					break;
				}
			}
			k_ = best_no;
			validation->setQ2(best_q2);
			
			return 1;
		}
	}
}
