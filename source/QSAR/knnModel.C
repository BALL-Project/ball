#include <BALL/QSAR/knnModel.h>


using namespace BALL::QSAR;


KNNModel::KNNModel(const QSARData& q, int k, double kw) : ALLModel(q,kw)
{
	type_ = "KNN";
	k_ = k;
}



void KNNModel::calculateWeights(Matrix& dist, RowVector& w)
{
	w.ReSize(dist.Ncols());
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
	if(v.size()!=3)
	{
		String c = "Wrong number of model parameters! Needed: 3;";
		c = c+" given: "+String(v.size());
		throw Exception::ModelParameterError(__FILE__,__LINE__,c.c_str());
	}
	kw_ = v[0];
	lambda_ = v[1];
	k_ = (int) v[2];
}

vector<double> KNNModel::getParameters() const
{
	vector<double> d;
	d.push_back(kw_);
	d.push_back(lambda_);
	d.push_back(k_);
	return d;
}

bool KNNModel::optimizeParameters(int d)
{
	double best_q2=0;
	int best_no=1;
	for(unsigned int i=1; i<=data->getNoSubstances() ;i++)
	{
		k_=i;
		validation->crossValidation(d);

		if(validation->getQ2()>best_q2)
		{
			best_q2=validation->getQ2();
			best_no=i;
		}
// 		else
// 		{
// 			break;
// 		}
	}
	k_=best_no;
	validation->setQ2(best_q2);
	
	return 1;
}