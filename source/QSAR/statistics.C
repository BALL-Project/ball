// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// 

#include <BALL/QSAR/statistics.h>
using namespace BALL::QSAR;




void Statistics::scaling(vector<vector<double> >& m)
{
	for (unsigned int i=0; i<m.size(); i++)
	{
		scaling(m[i]);
	}
}


void Statistics::scaling(vector<double>& v)
{
	double std=sqrt(getVariance(v));
	if (std==0) {return;} // if all values==0, do not change anything
	for(unsigned int i=0; i<v.size(); i++)
	{
		v[i]/=std;
	}
}


void Statistics::centering(vector<vector<double> >& m)
{
	for (unsigned int i=0; i<m.size(); i++)
	{
		centering(m[i]);
	}
}


void Statistics::centering(vector<double>& v)
{
	double mean=getMean(v);
	double std=sqrt(getVariance(v,mean));
	if (std==0) {return;} // if all values==0, do not change anything
	for(unsigned int i=0; i<v.size(); i++)
	{
		v[i]=(v[i]-mean)/std;
	}
}


void Statistics::centering(vector<double>& v, double& mean, double& std)
{
	mean=getMean(v);
	std=sqrt(getVariance(v,mean));
	if (std==0) {return;} // if all values==0, do not change anything
	for(unsigned int i=0; i<v.size(); i++)
	{
		v[i]=(v[i]-mean)/std;
	}
}


double Statistics::getVariance(const vector<double>& v, double mean)
{
	if (mean==-1) {	mean=getMean(v); }
	double sum_of_squares=0;
	for(unsigned int i=0; i<v.size(); i++)
	{
		sum_of_squares+=(v[i]-mean)*(v[i]-mean);
	}
	return sum_of_squares/(v.size()-1);
}


double Statistics::getMean(const vector<double>& v)
{
	double sum=0;
	for(unsigned int i=0; i<v.size(); i++)
	{
		sum+=v[i];
	}
	return sum/v.size();
}

//--------------------------

void Statistics::centering(Matrix& m)
{
	for (int i=1; i<=m.Ncols(); i++)
	{
		centering(m, i);
	}
}


void Statistics::centering(Matrix& m, int col)
{
	double mean=getMean(m, col);
	double std=sqrt(getVariance(m, col, mean));
	if (std==0) {return;} // if all values==0, do not change anything
	for(int i=1; i<=m.Nrows(); i++)
	{
		m(i,col)=(m(i,col)-mean)/std;
	}
}

double Statistics::getMean(const Matrix& m, int col)
{
	double sum=0;
	for(int i=1; i<=m.Nrows(); i++)
	{
		sum+=m(i,col);
	}
	return sum/m.Nrows();
}

double Statistics::getVariance(const Matrix& m, int col, double mean)
{
	if (mean==-1) {	mean=getMean(m,col); }
	double sum_of_squares=0;
	for(int i=1; i<=m.Nrows(); i++)
	{
		sum_of_squares+=pow(m(i,col)-mean,2);
	}
	return sum_of_squares/(m.Nrows()-1);
}

double Statistics::getStddev(const Matrix& m, int col, double mean)
{
	double d = getVariance(m,col,mean);
	return sqrt(d);
}

double Statistics::getCovariance(const Matrix& m, int col1, int col2, double mean1, double mean2)
{
	if (mean1==-1) {mean1=getMean(m,col1);}
	if (mean2==-1) {mean2=getMean(m,col2);}
	double sum_of_squares=0;
	for(int i=1; i<=m.Nrows(); i++)
	{
		sum_of_squares+=(m(i,col1)-mean1)*(m(i,col2)-mean2);
	}
	return sum_of_squares/(m.Nrows()-1);
}


double Statistics::sq(const Matrix& m, int col, double mean)
{
	if (mean==-1) {	mean=getMean(m,col); }
	double sum_of_squares=0;
	for(int i=1; i<=m.Nrows(); i++)
	{
		sum_of_squares+=pow(m(i,col)-mean,2);
	}
	return sum_of_squares;
}

double Statistics::euclNorm(const ColumnVector& cv)
{
	return sqrt(scalarProduct(cv));
}


double Statistics::scalarProduct(const ColumnVector& cv)
{
	double n=0;
	for(int i=1; i<=cv.Nrows();i++)
	{
		n+=cv(i)*cv(i);
	}
	return n;
}


double Statistics::euclDistance(const ColumnVector& c1, const ColumnVector& c2)
{
	double n=0;
	for(int i=1; i<=c1.Nrows();i++)
	{
		n+=pow((c1(i)-c2(i)),2);
	}
	return sqrt(n);
}

//---------------------------


double Statistics::distance(const Matrix& m, int& row1, int& row2, double& p)
{
	double dist=0;
	for (int j=1; j<=m.Ncols(); j++)
	{
		dist+=m(row1,j)*m(row2,j);
		
	}
		
	int i_p = static_cast <int> (p);
	if(i_p != p) // if a root of dist should be taken, then dist may not be negative
	{
		dist = abs(dist);
	}
	return pow(dist,p);
}


double Statistics::distance(const Matrix& m1, const Matrix& m2, int& row1, int& row2, double& p)
{
	if(m1.Ncols()!=m2.Ncols()) 
	{
		cout << "Matrices must have the same number of columns!!"<<endl; return 0;
	}
	
	double dist=0;
	for (int j=1; j<=m1.Ncols(); j++)
	{
		dist+=m1(row1,j)*m2(row2,j);
	}
		
	int i_p = static_cast <int> (p);
	if(i_p != p) // if a root of dist should be taken, then dist may not be negative
	{
		dist = abs(dist);
	}
	return pow(dist,p);
}


double Statistics::distance(const Matrix& m1, const Matrix& m2, int& row1, int& row2, String& f, String& g)
{
	if(m1.Ncols()!=m2.Ncols()) 
	{
		cout << "Matrices must have the same number of columns!!"<<endl; return 0;
	}
	
	double dist=0;
	for (int j=1; j<=m1.Ncols(); j++)
	{
		String var="";
		var = var+"x1="+String(m1(row1,j))+";x2="+String(m2(row2,j))+";";
	//	cout<<"f= "<<var+f<<endl;
		ParsedFunction<double> pf(var+f);
		dist+=pf(0);
	}	
	String var2="";
	var2 = var2+"sum="+String(dist)+";";
	//cout<<"g= "<<var+g<<endl;
	ParsedFunction<double> pf2(var2+g);
	return pf2(0);
}


double Statistics::euclDistance(const Matrix& m1, const Matrix& m2, int row1, int row2)
{
	if(m1.Ncols()!=m2.Ncols()) 
	{
		cout << "Matrices must have the same number of columns!!"<<endl; return 0;
	}
	
	double dist=0;
	for (int j=1; j<=m1.Ncols(); j++)
	{
		dist+=pow(m1(row1,j)-m2(row2,j),2);
	}	
	return sqrt(dist);
}



