// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// 
#include <BALL/QSAR/regressionValidation.h>
#include <BALL/QSAR/statistics.h>
#include <BALL/QSAR/regressionModel.h>
#include <gsl/gsl_rng.h>
#include <newmatio.h>


using namespace BALL::QSAR;


RegressionValidation::RegressionValidation(RegressionModel* m) : Validation(m)
{
	ssR_=0; ssE_=0; ssT_=0; Q2_=-1; F_cv_=-1; R2_=-1; std_err_=-1; F_regr_=-1; max_error_=-1;
	regr_model_=m;
	predQualFetcher_ = &RegressionValidation::getQ2;
	fitQualFetcher_ = &RegressionValidation::getR2;
}

RegressionValidation::~RegressionValidation()
{
}

void RegressionValidation::setQ2(double d)
{
	Q2_ = d;
}

void RegressionValidation::crossValidation(int k, bool restore)
{
	crossValidation(k,NULL,restore);
}


void RegressionValidation::crossValidation(int k, vector<Matrix>* results, bool restore)
{
	if(model_->data->descriptor_matrix_.size()==0 || model_->data->Y_.size()==0)
	{
		throw Exception::InconsistentUsage(__FILE__,__LINE__,"Data must be fetched from input-files by QSARData before cross-validation can be done!");
	}
	
	Matrix desc_backup;
	Matrix res_backup;
	Matrix y_backup;
	if(restore)
	{
		desc_backup=model_->descriptor_matrix_; // save matrices in order in restore them after cross-validation
		res_backup=regr_model_->training_result_;
		y_backup=model_->Y_;
	}
	
	int lines=model_->data->descriptor_matrix_[0].size();
	int col=model_->data->descriptor_matrix_.size();
	if(!model_->descriptor_IDs_.empty())
	{
		col=model_->descriptor_IDs_.size();
	}
	Q2_=0; ssE_=0; ssR_=0; F_cv_=0;

	// test k times
	for(int i=0; i<k; i++)
	{	
		int test_size= (lines+i)/k;
		int training_size=lines-test_size;
		model_->Y_.ReSize(training_size,model_->data->Y_.size());
		model_->descriptor_matrix_.ReSize(training_size,col); 
		test_substances_.resize(test_size);
		test_Y_.ReSize(test_size,model_->data->Y_.size());
		

		int train_line=0;  // no of line in descriptor_matrix_ of model_
		int test_line=0;
		
		//copy data to training and test data set
		for(int line=0; line<lines; line++)
		{
			if((line+1+i)%k==0)
			{
				setTestLine(test_line,line);
				test_line++;
			}
			else
			{
				setTrainingLine(train_line,line);
				train_line++;
			}
			
		}
		// test Model with model_->predict() for each line of test-data
		model_->train();
		if(results!=NULL){ results->push_back(*regr_model_->getTrainingResult());}
		testAllSubstances(0);	  // do not transform cross-validation test-data again...
		Q2_ += 1-(ssE_/(ssE_+ssR_));
	}
	Q2_ = Q2_/k;
//	F_cv_ = gsl_cdf_fdist_P((Q2_*(lines-col-1))/((1-Q2_)*col), col, lines-col-1);
	
	std_err_ = std_err_ / ((k-1)*lines);
	
	if(restore)
	{
		model_->descriptor_matrix_=desc_backup;   // prevent confusion of cross-validation coefficients with coefficients
		regr_model_->training_result_=res_backup; // derived from training with complete input data
		model_->Y_=y_backup;
	}
}


void RegressionValidation::testAllSubstances(bool transform)
{	
	ssE_=0; ssR_=0; std_err_=0; ssT_=0;
	
	RowVector mean_Y(test_Y_.Ncols()); // mean of each activity
	//RowVector sum_of_squares(test_Y_.Ncols());
	for(int i=1; i<=test_Y_.Ncols();i++)
	{
		mean_Y(i)=Statistics::getMean(test_Y_, i);
		//sum_of_squares(i)=Statistics::sq(test_Y_,i,mean_Y(i));
	}
	//ssT_ =  sum_of_squares.Sum();
	
	for(unsigned int i=0; i<test_substances_.size();i++)
	{       
		RowVector rv=model_->predict(test_substances_[i],transform);
		double error = 0;
		for(int k=1; k<=test_Y_.Ncols();k++)
		{
			error+=pow(test_Y_(i+1,k)-rv(k),2);
			ssR_+=pow(mean_Y(k)-rv(k),2);
		}
		if (error>max_error_)
		{
			max_error_=error;
		}
		ssE_+=error;
		
// 		if(model_->type_=="GP")
// 		{
// 			std_err_ += model_->calculateStdErr();
// 		}
	}
/*	if(model_->type_=="GP")
	{
		std_err_ = std_err_/test_substances_.size();
	}*/	

}


void RegressionValidation::testInputData(bool transform)
{ 
	if(model_->data->descriptor_matrix_.size()==0)
	{
		throw Exception::InconsistentUsage(__FILE__,__LINE__,"Data must be fetched from input-files by QSARData before a model_'s fit to it can be evaluated!");
	}
	
	if(model_->type_!="ALL" && model_->type_!="SVR" && model_->type_!="KNN") // do not check dimensions of traning results for ALL, since ALL does no training
	{
		//unsigned int des = model_->descriptor_IDs_.size();
		//unsigned int data_cols = model_->data->descriptor_matrix_.size();
		unsigned int res_rows = regr_model_->training_result_.Nrows();
		//unsigned int desmat_cols = model_->descriptor_matrix_.Ncols();
		
		if(res_rows==0)
		{
			throw Exception::InconsistentUsage(__FILE__,__LINE__,"Model must be trained before its fit to the input data can be evaluated!");
		}
		int test_col = model_->descriptor_IDs_.size(); // no of columns of X^T X (linear model)
		int kernel_test_col = model_->descriptor_matrix_.Nrows();  // no of columns of X X^T (nonlinear model)
		if(test_col==0)
		{
			test_col=model_->data->getNoDescriptors();
		}
		
		if(test_col!=res_rows && kernel_test_col!=res_rows)
		{
			throw Exception::InconsistentUsage(__FILE__,__LINE__,"Model must be trained before its fit to the input data can be evaluated!");
		}
				 		 		 
// 		else if( des==0 && data_cols!=res_rows && data_cols!=desmat_cols)
// 		{
// 			throw Exception::InconsistentUsage(__FILE__,__LINE__,"Model must be trained with data containing the same number of features as the test data set!");
// 		}
// 		else if(des!=0 && res_rows!=des && data_cols!=desmat_cols)
// 		{cout<<data_cols<<" "<<res_rows<<" "<<desmat_cols<<endl;cout.flush();
// 			throw Exception::InconsistentUsage(__FILE__,__LINE__,"Model must be trained with data containing the same number of features as the test data set!");
// 		}
	}
	
	R2_=0; ssE_=0; ssR_=0; std_err_=0; F_regr_=0;
	int lines=model_->data->descriptor_matrix_[0].size();
	test_substances_.resize(lines);
	test_Y_.ReSize(lines,model_->data->Y_.size());
	
	bool back_transform=0;
	if(transform && model_->data->descriptor_transformations_.size()!=0)
	{
		// if test data is to be transformed according to centering of training data, BUT has already been centered itself
		back_transform = 1;
	}

	for(int i=0; i<lines; i++)
	{
		setTestLine(i,i,back_transform);
	}
	testAllSubstances(transform);
	R2_= 1-(ssE_/(ssE_+ssR_));
	
	int col=model_->data->descriptor_matrix_.size();
	if(!model_->descriptor_IDs_.empty())
	{
		col=model_->descriptor_IDs_.size();
	}
	//F_regr_ = gsl_cdf_fdist_P((R2_*(lines-col-1))/((1-R2_)*col), col, lines-col-1);
//	F_regr_ = gsl_cdf_fdist_P( (ssR_*(lines-col-1))/(ssE_*col), col,lines-col-1 );
}


void RegressionValidation::calculateCoefficientStddev(int k, bool b)
{
	if(model_->data->descriptor_matrix_.size()==0 || model_->data->Y_.size()==0)
	{
		throw Exception::InconsistentUsage(__FILE__,__LINE__,"Data must be fetched from input-files by QSARData before standart errors of coefficients can be calculated!");
	}
	Matrix desc_backup=model_->descriptor_matrix_; // save matrices in order in restore them after cross-validation
	Matrix res_backup=regr_model_->training_result_;
	Matrix y_backup=model_->Y_;
	
	
	int no_activities=model_->data->Y_.size();
	vector<Matrix>* results = new vector<Matrix>;
	int no_descriptors=model_->data->descriptor_matrix_.size();
	if(!model_->descriptor_IDs_.empty())
	{
		no_descriptors=model_->descriptor_IDs_.size();
	}
	coefficient_stddev_.ReSize(no_descriptors, no_activities);
	
	if(b==1)
	{
		bootstrap(k,results,0);
	}
	else
	{
		crossValidation(k,results,0);
	}
	
	for(int c=1; c<=no_activities; c++) // for all modelled activities
	{
		for(int m=1;m<=no_descriptors; m++) // for all descriptors
		{			
			double mean_mc=0;
			double sumsquares_mc=0;
			
			for(int i=0; i<k;i++) // for all training results
			{
				mean_mc+=(*results)[i](m,c);
				sumsquares_mc+=pow((*results)[i](m,c),2);
			}
			mean_mc /= k;
			
			// calculate standard deviation of coefficient
			// = sqrt(1/k * \sum_{i=1}^k (x_i \^bar x)^2)
			// <=> sqrt(1/k (\sum_{i=1}^k x_i - k*\bar x^2))
			coefficient_stddev_(m,c)= sqrt(abs(sumsquares_mc-k*pow(mean_mc,2))/(k-1));
		}
	}
			


	delete results;
	model_->descriptor_matrix_=desc_backup;   // prevent confusion of cross-validation coefficients with coefficients
	regr_model_->training_result_=res_backup; // derived from training with complete input data
	model_->Y_=y_backup;
}


void RegressionValidation::bootstrap(int k, bool restore)
{ 
	bootstrap(k, NULL, restore);
}



void RegressionValidation::bootstrap1(int k, vector<Matrix>* results, bool restore)
{
	if(model_->data->descriptor_matrix_.size()==0 || model_->data->Y_.size()==0)
	{
		throw Exception::InconsistentUsage(__FILE__,__LINE__,"Data must be fetched from input-files by QSARData before bootstrapping can be done!");
	}
	Matrix desc_backup;
	Matrix res_backup;
	Matrix y_backup;
	if(restore)
	{
		desc_backup=model_->descriptor_matrix_; // save matrices in order in restore them after cross-validation
		res_backup=regr_model_->training_result_;
		y_backup=model_->Y_;
	}
	
	
	Q2_=0; max_error_=0; double r2=0;
	int N = model_->data->descriptor_matrix_[0].size();
	int no_descriptors=model_->data->descriptor_matrix_.size();
	if(!model_->descriptor_IDs_.empty())
	{
		no_descriptors=model_->descriptor_IDs_.size();
	}
	vector<double> errors(N,0); // errors for each substance as sum over all samples
	vector<int> no_samples(N,0); // number of samples for each substance, that do NOT contain this substance
	gsl_rng * r = gsl_rng_alloc (gsl_rng_ranlxd2);
	
	for(int i=0; i<k; i++) // create and evaluate k bootstrap samples
	{
		//gsl_rng_set(r,i);
		vector<bool> sample_substances(N,0); // IDs of substances of this sample
		
		model_->descriptor_matrix_.ReSize(N,no_descriptors);
		model_->Y_.ReSize(N,model_->data->Y_.size());
		for(int j=0; j<N;j++)  // create training matrix
		{
			//int pos = rand()%N;
			int pos = gsl_rng_uniform_int(r,N);
			setTrainingLine(j,pos);
			sample_substances[pos]=1;
		}
		int test_size=0;
		for(int j=0; j<N;j++)  // find size of test data set
		{
			if(sample_substances[j]==1) 
			{
				continue;
			}
			test_size++;
		}
		test_substances_.resize(test_size);
		test_Y_.ReSize(test_size,model_->data->Y_.size());
		vector<int> test_IDs(test_size,0); // IDs of the test-substances
		int test_line=0;
		for(int j=0; j<N;j++)  // create test data set
		{
			if(sample_substances[j]==0) 
			{	
				setTestLine(test_line,j);
				test_IDs[test_line]=j;
				test_line++;
			}
		}
		model_->train();
		if(results!=NULL){ results->push_back(*regr_model_->getTrainingResult());}
		
		RowVector mean_Y(test_Y_.Ncols()); // mean of each activity
		for(int i=1; i<=test_Y_.Ncols();i++)
		{
			mean_Y(i)=Statistics::getMean(test_Y_, i);
		}
		for(unsigned int i=0; i<test_substances_.size();i++)  // predict activiti(es) of each test-substance
		{
			RowVector rv=model_->predict(test_substances_[i],0);
			double e = ((test_Y_.Row(i+1)-rv)*(test_Y_.Row(i+1)-rv).t()).AsScalar();
			if (e>max_error_)
			{
				max_error_=e;
			}
			
			double r = ((mean_Y-rv)*(mean_Y-rv).t()).AsScalar();
			errors[test_IDs[i]] += 1-(e/(e+r));
			no_samples[test_IDs[i]]++;
		}
		
		test_substances_.resize(N);
		test_Y_.ReSize(N,model_->data->Y_.size());
		test_IDs.resize(N,0);
		for(int j=0; j<N;j++)  // create test data set for calculation of R^2
		{
			if(sample_substances[j]==1) 
			{	
				setTestLine(test_line,j);
				test_IDs[test_line]=j;
				test_line++;
			}
		}
		testAllSubstances(0);
		r2 += 1-(ssE_/(ssE_+ssR_));
	}
	
	int no=0;
	for(int i=0; i<N;i++)
	{
		if(no_samples[i]==0) 
		{
			continue;
		}
		Q2_ += errors[i]/no_samples[i];
		//cout << errors[i] <<"  "<<no_samples[i]<<endl;
		no++;
	}
	Q2_ = Q2_/no;
	
	r2 = r2/k;
	Q2_ = 0.632*Q2_ + 0.368*r2;
	
	gsl_rng_free(r);
	if(restore)
	{
		model_->descriptor_matrix_=desc_backup;   // prevent confusion of cross-validation coefficients with coefficients
		regr_model_->training_result_=res_backup; // derived from training with complete input data
		model_->Y_=y_backup;
	}
}


void RegressionValidation::bootstrap(int k, vector<Matrix>* results, bool restore)
{
	if(model_->data->descriptor_matrix_.size()==0 || model_->data->Y_.size()==0)
	{
		throw Exception::InconsistentUsage(__FILE__,__LINE__,"Data must be fetched from input-files by QSARData before bootstrapping can be done!");
	}
	Matrix desc_backup;
	Matrix res_backup;
	Matrix y_backup;
	if(restore)
	{
		desc_backup=model_->descriptor_matrix_; // save matrices in order in restore them after cross-validation
		res_backup=regr_model_->training_result_;
		y_backup=model_->Y_;
	}
	
	
	Q2_=0; double r2=0; max_error_=0;
	int N = model_->data->descriptor_matrix_[0].size();
	int no_descriptors=model_->data->descriptor_matrix_.size();
	if(!model_->descriptor_IDs_.empty())
	{
		no_descriptors=model_->descriptor_IDs_.size();
	}
	
	gsl_rng * r = gsl_rng_alloc (gsl_rng_ranlxd2);
	PreciseTime pt;
	gsl_rng_set(r,pt.now().getSeconds());
	
	for(int i=0; i<k; i++) // create and evaluate k bootstrap samples
	{
		vector<int> sample_substances(N,0); // numbers of occurences of substances within this sample
		
		/// create training matrix and train the model_
		model_->descriptor_matrix_.ReSize(N,no_descriptors);
		model_->Y_.ReSize(N,model_->data->Y_.size());
		for(int j=0; j<N;j++)
		{
			int pos = gsl_rng_uniform_int(r,N);
			setTrainingLine(j,pos);
			sample_substances[pos]++;
		}
		model_->train(); // train the model_ on current bootstrap sample
		
		
		/// find size of test data set
		int test_size=0;
		for(int j=0; j<N;j++) 
		{
			if(sample_substances[j]>0) 
			{
				continue;
			}
			test_size++;
		}
		test_substances_.resize(test_size);
		test_Y_.ReSize(test_size,model_->data->Y_.size());
		
		
		/// create test data set and calculate Q^2
		int test_line=0;
		for(int j=0; j<N;j++)
		{
			if(sample_substances[j]==0) 
			{	
				setTestLine(test_line,j);
				test_line++;
			}
		}
		if(results!=NULL){ results->push_back(*regr_model_->getTrainingResult());}
		testAllSubstances(0);
		Q2_ += 1-(ssE_/(ssE_+ssR_));
		
		/// create test data set and calculate R^2
		test_substances_.resize(N);
		test_Y_.ReSize(N,model_->data->Y_.size());
		test_line=0;
		for(int j=0; j<N;j++)  
		{
			while(sample_substances[j]>0) // insert substance as often as it occurs in the training data set 
			{	
				setTestLine(test_line,j);
				test_line++;
				sample_substances[j]--;
			}
		}
		testAllSubstances(0);
		r2 += 1-(ssE_/(ssE_+ssR_));
	}
	
	Q2_ = Q2_/k;
	r2 = r2/k;
	
	Q2_ = 0.632*Q2_ + 0.368*r2;
		
	gsl_rng_free(r);
	if(restore)
	{
		model_->descriptor_matrix_=desc_backup;   // prevent confusion of cross-validation coefficients with coefficients
		regr_model_->training_result_=res_backup; // derived from training with complete input data
		model_->Y_=y_backup;
	}
}


Matrix RegressionValidation::yRandomizationTest(int runs, int k)
{
	if(model_->data->descriptor_matrix_.size()==0 || model_->data->Y_.size()==0)
	{
		throw Exception::InconsistentUsage(__FILE__,__LINE__,"Data must be fetched from input-files by QSARData object before response permutation tests can be done!");
	}	
	
	Matrix y_backup=model_->Y_;
	Matrix desc_backup=model_->descriptor_matrix_;
	Matrix res_backup=regr_model_->training_result_;
	vector<vector<double> > dataY_backup=model_->data->Y_;
				
	Matrix results(runs,2);
	results=-1;

	for(int i=0; i<runs;i++)
	{
		yRand(); // randomize all columns of Y_
		crossValidation(k,NULL,0);
		model_->readTrainingData();
		model_->train();
		testInputData(0);	
		results(i+1,1)=R2_;
		results(i+1,2)=Q2_;
	}	
	
	model_->Y_=y_backup;
	model_->descriptor_matrix_=desc_backup;
	regr_model_->training_result_=res_backup;
	QSARData* data = const_cast <QSARData*> (model_->data);
	data->Y_=dataY_backup;
	
	return results;
}



double RegressionValidation::getQ2()
{
	return Q2_;
}


double RegressionValidation::getFcv()
{
	return F_cv_;
}


double RegressionValidation::getFregr()
{
	return F_regr_;
}


double RegressionValidation::getMaxError()
{ 
	return max_error_;
}


double RegressionValidation::getR2()
{
	return R2_;
}


double RegressionValidation::getCVRes()
{
	//return getQ2();
	return (this->*predQualFetcher_)();
}


double RegressionValidation::getFitRes()
{
	return (this->*fitQualFetcher_)();
}


void RegressionValidation::selectStat(int s)
{
	if(s==0)
	{
		validation_statistic_ = 0;
		predQualFetcher_ = &RegressionValidation::getQ2;
		fitQualFetcher_ = &RegressionValidation::getR2;
	}
	else if(s==1)
	{
		validation_statistic_ = 1;
		fitQualFetcher_ = &RegressionValidation::getFregr;
		predQualFetcher_ = &RegressionValidation::getFcv;
	}
}
				
				
void RegressionValidation::setCVRes(double d)
{
	setQ2(d);
}



const Matrix* RegressionValidation::getCoefficientStddev()
{
	return &coefficient_stddev_;
}


void RegressionValidation::setCoefficientStddev(const Matrix* stddev)
{
	coefficient_stddev_ = *stddev;	
}


/*
void RegressionValidation::crossValidation(int k)
{
	int step_width=model_->data->descriptor_matrix_[0].size()/k;
	int rest=model_->data->descriptor_matrix_[0].size()%k;
	int lines=model_->data->descriptor_matrix_[0].size();
	int col=model_->data->descriptor_matrix_.size();
	if(!model_->descriptor_IDs_.empty())
	{
		col=model_->descriptor_IDs_.size();
	}
	Q2_=0; ssE_=0; ssR_=0;

	
	// test k times; one time for each block as test-data
	for(int i=0; i<k; i++)
	{	//cout <<"i="<<i<<"------------"<<endl;		
	
		if(i!=k-1){
			model_->Y_.ReSize((k-1)*step_width+rest,model_->data->Y_.size()); 
			model_->descriptor_matrix_.ReSize((k-1)*step_width+rest,col); 
			test_substances_.resize(step_width);
			test_Y_.ReSize(step_width,model_->data->Y_.size());
			
		}
		else
		{
			model_->Y_.ReSize((k-1)*step_width,model_->data->Y_.size()); 
			model_->descriptor_matrix_.ReSize((k-1)*step_width,col);
			test_substances_.resize(step_width+rest);
			test_Y_.ReSize(step_width+rest,model_->data->Y_.size());
		}
	
// 		cout << model_->Y_.Nrows()<<" "<<model_->Y_.Ncols()<<endl;
// 		cout << model_->descriptor_matrix_.Nrows()<<" "<<model_->descriptor_matrix_.Ncols()<<endl;
// 		cout<<test_substances_.size()<<endl;
// 		cout<<test_Y_[0].size()<<endl;
		
		int current_line=0; // no of line in original data
		int train_line=0;  // no of line in descriptor_matrix_ of model_
		int test_line=0;

		// copy each of the k blocks ).element(1,1)
		// block i = test-data; other blocks=training data 
		for(int block=0; block<k;block++)
		{	
			int end_of_block=(block+1)*step_width-1;
			
			// last blocks contains all remaining lines
			if (block==k-1)
			{
				end_of_block=lines-1;
			}
	
			 // copy training data 
			if(block!=i) 
			{	//cout << "train-data: lines "<<current_line<<"-"<<end_of_block<<endl;
				
				// copy all lines of training data
				for (;current_line<=end_of_block;current_line++) 
				{ 
					setTrainingLine(train_line,current_line);
					train_line++;
					//cout <<"\tcopying training data:"<<current_line<<endl;
				}
			}
 			// copy test data
			else
			{	//cout << "test-data: lines "<<current_line<<"-"<<end_of_block<<endl;
				
				// copy all lines of test data
				for (;current_line<=end_of_block;current_line++) 
				{	
					setTestLine(test_line,current_line);
					test_line++;
					//cout <<"\tcopying test data: "<<current_line<<endl;
				}
			}
		}
		// test Model with model_->predict() for each line of test-data
		model_->train();
		testAllSubstances();
		Q2_ += 1-(ssE_/(ssE_+ssR_));
	}
	Q2_ = Q2_/k;
	std_err_ = std_err_ / ((k-1)*lines);
}
*/
