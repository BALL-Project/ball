// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// 
#include <BALL/QSAR/classificationValidation.h>
#include <BALL/QSAR/statistics.h>
#include <BALL/QSAR/classificationModel.h>
#include <newmatio.h>

using namespace BALL::QSAR;


ClassificationValidation::ClassificationValidation(ClassificationModel* m) : Validation(m)
{
	clas_model = m; accuracy_=-1; accuracy_cv_=-1; accuracy_input_test_=-1;
	qualCalculation = &ClassificationValidation::calculateAverageAccuracy;
}

void ClassificationValidation::selectStat(int s)
{
	if(s>=0 && s<=4)
	{
		validation_statistic_ = s;
	}
	if(s==0)
	{
		qualCalculation = &ClassificationValidation::calculateAverageAccuracy;
	}
	else if(s==1)
	{
		qualCalculation = &ClassificationValidation::calculateWeightedAccuracy;
	}
	else if(s==2)
	{
		qualCalculation = &ClassificationValidation::calculateOverallAccuracy;
	}
	else if(s==3)
	{
		qualCalculation = &ClassificationValidation::calculateAverageMCC;
	}
	else if(s==4)
	{
		qualCalculation = &ClassificationValidation::calculateOverallMCC;
	}
}

void ClassificationValidation::crossValidation(int k, bool restore)
{
	if(model_->data->descriptor_matrix_.size()==0 || model_->data->Y_.size()==0)
	{
		throw Exception::InconsistentUsage(__FILE__,__LINE__,"Data must be fetched from input-files by QSARData before cross-validation can be done!");
	}
	
	Matrix desc_backup;
	//Matrix res_backup;
	Matrix y_backup;
	if(restore)
	{
		desc_backup=model_->descriptor_matrix_; // save matrices in order in restore them after cross-validation
		//res_backup=clas_model->training_result_;
		y_backup=model_->Y_;
	}
	
	int lines=model_->data->descriptor_matrix_[0].size();
	int col=model_->data->descriptor_matrix_.size();
	if(!model_->descriptor_IDs_.empty())
	{
		col=model_->descriptor_IDs_.size();
	}
	double average_accuracy=0;
	class_results_.ReSize(clas_model->labels_.size());
	class_results_ = 0;
	
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
		testAllSubstances(0);  // do not transform cross-validation test-data again...
		average_accuracy += accuracy_;
	}
	accuracy_cv_ = average_accuracy/k;
	class_results_ = class_results_/k;
	
	if(restore)
	{
		model_->descriptor_matrix_=desc_backup;   // prevent confusion of cross-validation coefficients with coefficients
		model_->Y_=y_backup;
		model_->readTrainingData();
		model_->train();
	}
}


void ClassificationValidation::testAllSubstances(bool transform)
{	
	confusion_matrix_.ReSize(4,clas_model->labels_.size());
	confusion_matrix_=0;
	class_results_.ReSize(clas_model->labels_.size());
	class_results_ = 0;
	
	for(int i=0; i<(int)test_substances_.size();i++) // for all substances in test-data
	{
		RowVector rv=model_->predict(test_substances_[i],transform);
		
		for(int c=1; c<=test_Y_.Ncols();c++) // for all modelled activities
		{			
			int y_ic= static_cast<int>(test_Y_(i+1,c)); 
			int rv_ic = static_cast<int>(rv(c));
			
			for(int k=1;k<=confusion_matrix_.Ncols();k++)   // set TP,FP,TN,FN for all classes
			{				
				if((clas_model->labels_)[k-1]==y_ic)
				{
					if(y_ic==rv_ic)
					{
						confusion_matrix_(1,k)++;  // TP for class k
					}
					else
					{	
						confusion_matrix_(4,k)++; // FN for class k
					}
				}
				else
				{
					if(clas_model->labels_[k-1]!=rv_ic)
					{
						confusion_matrix_(3,k)++;  // TN for class k
					}
					else
					{
						confusion_matrix_(2,k)++; // FP for class k
					}
				}
			}
		}
		
		
	}
	
	(this->*qualCalculation)();
}


void ClassificationValidation::testInputData(bool transform)
{	
	int lines=model_->data->descriptor_matrix_[0].size();
	test_substances_.resize(lines);
	test_Y_.ReSize(lines,model_->data->Y_.size());
	
	class_results_.ReSize(clas_model->labels_.size());
	class_results_ = 0;
	
	bool back_transform=0;
	if(transform && model_->data->descriptor_transformations_.size()>0)
	{
		// if test data is to be transformed according to centering of training data, BUT has already been centered itself
		back_transform=1;
	}
	
	for(int i=0; i<lines; i++)
	{
		setTestLine(i,i,back_transform);
	}
	//cout<<test_Y_<<endl;
	
	testAllSubstances(transform);
	accuracy_input_test_ = accuracy_;
}


void ClassificationValidation::bootstrap(int k, bool restore)
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
		//res_backup=clas_model->training_result_;
		y_backup=model_->Y_;
	}

	class_results_.ReSize(clas_model->labels_.size());
	class_results_ = 0;
	accuracy_cv_=0;
	int N = model_->data->descriptor_matrix_[0].size();
	int no_descriptors=model_->data->descriptor_matrix_.size();
	if(!model_->descriptor_IDs_.empty())
	{
		no_descriptors=model_->descriptor_IDs_.size();
	}

	gsl_rng * r = gsl_rng_alloc (gsl_rng_ranlxd2);
	PreciseTime pt;
	gsl_rng_set(r,pt.now().getSeconds());
	
	double overall_fit=0;
	double overall_pred=0;
	RowVector class_results_pred; 
	class_results_pred.ReSize(clas_model->labels_.size()); class_results_pred=0;
	RowVector class_results_fit; 
	class_results_fit.ReSize(clas_model->labels_.size()); class_results_fit=0;

	for(int i=0; i<k; i++) // create and evaluate k bootstrap samples
	{
		//gsl_rng_set(r,i);
		vector<int> sample_substances(N,0); // numbers of occurences of substances within this sample
		
		class_results_ = 0;
	
		/// create training matrix and train the model_
		model_->descriptor_matrix_.ReSize(N,no_descriptors);
		model_->Y_.ReSize(N,model_->data->Y_.size());
		for(int j=0; j<N;j++)
		{
			//int pos = rand()%N;
			int pos = gsl_rng_uniform_int(r,N);
			setTrainingLine(j,pos);
			sample_substances[pos]++;
		}
		model_->train();
	
		
		/// find size of test data set
		int test_size=0;
		for(int j=0; j<N;j++)
		{
			if(sample_substances[j]==1) 
			{
				continue;
			}
			test_size++; 
		}
		test_substances_.resize(test_size);
		test_Y_.ReSize(test_size,model_->data->Y_.size());
		
	
		/// create test data set and calculate accuracy_ of prediction
		int test_line=0;
		for(int j=0; j<N;j++) 
		{
			if(sample_substances[j]==0) 
			{	
				setTestLine(test_line,j);
				test_line++;
			}
		}
		testAllSubstances(0);
		overall_pred+=accuracy_;
		class_results_pred+=class_results_;		
	
		class_results_ = 0; // clear pred. result before adding training fit result!!
		
		/// create test data set and calculate accuracy_ of fit to training data	
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
		overall_fit+=accuracy_;
		class_results_fit+=class_results_;
	}
	
	overall_pred = overall_pred/k;
	overall_fit = overall_fit/k;
	class_results_pred = class_results_pred/k;
	class_results_fit = class_results_fit/k;
	
	accuracy_cv_ = 0.632*overall_pred + 0.368*overall_fit;
	class_results_ = 0.632*class_results_pred + 0.368*class_results_fit;
	
	gsl_rng_free(r);
	if(restore)
	{
		model_->descriptor_matrix_=desc_backup;   // prevent confusion of cross-validation coefficients with coefficients
		model_->Y_=y_backup;
		model_->train();
	}
}


Matrix ClassificationValidation::yRandomizationTest(int runs, int k)
{
	Matrix y_backup=model_->Y_;
	Matrix desc_backup=model_->descriptor_matrix_;
	//Matrix res_backup=clas_model->training_result_;
	vector<vector<double> > dataY_backup=model_->data->Y_;
				
	//vector<double> c(2,-1);
	//vector<vector<double> > results(runs,2);
	Matrix results(runs,2);
	results=-1;
	class_results_.ReSize(clas_model->labels_.size());
	class_results_ = 0;

	for(int i=0; i<runs;i++)
	{
		yRand(); // randomize all columns of Y_
		crossValidation(k,0);
		testInputData(0);
		//results[i][0]=accuracy_input_test_;
		//results[i][1]=accuracy_cv_;
		results(i+1,1)=accuracy_input_test_;
		results(i+1,2)=accuracy_cv_;
	}
	
	class_results_ = class_results_/runs;
	
	model_->Y_=y_backup;
	model_->descriptor_matrix_=desc_backup;
	//clas_model->training_result_=res_backup;
	QSARData* data = const_cast <QSARData*> (model_->data);
	data->Y_=dataY_backup;
	model_->train();
	
	return results;
}



void ClassificationValidation::calculateOverallAccuracy()
{		
	// do NOT calculate accuracy seperately for each class!
	int TP = 0;
	for(int j=1;j<=confusion_matrix_.Ncols();j++)
	{
		TP += (int)confusion_matrix_(1,j);
	}
	int N = 0; // number of predictions
	for(int j=1;j<=confusion_matrix_.Nrows();j++)
	{
		N += (int)confusion_matrix_(j,1);
	}
	accuracy_ = ((double)TP) / N;
	
// 	cout<<confusion_matrix_;
// 	cout<<"overall ACC="<<accuracy_<<endl;
}
		


void ClassificationValidation::calculateAverageAccuracy()
{
	accuracy_=0;
	
	if(confusion_matrix_.Ncols()<2) // for binary classification normal df. of accuracy
	{
		for(int j=1;j<=confusion_matrix_.Ncols();j++) // calculate accuracy_ of all classes
		{	// (TP+TN) / (TP+TN+FN+FP)
			accuracy_ += (confusion_matrix_(1,j)+confusion_matrix_(3,j)) / (confusion_matrix_(1,j)+confusion_matrix_(3,j)+confusion_matrix_(4,j)+confusion_matrix_(2,j));
		}
	}
	else
	{
		for(int j=1;j<=confusion_matrix_.Ncols();j++) // calculate accuracy_ of all classes
		{	// multi-class accuracy := (TP) / (TP+FN+FP)
			double acc = (confusion_matrix_(1,j)) / (confusion_matrix_(1,j)+confusion_matrix_(4,j)+confusion_matrix_(2,j));
			class_results_(j) += acc;
			accuracy_ += acc;
		}
	}
	
	accuracy_ /= confusion_matrix_.Ncols(); // mean accuracy_ of all classes
}


void ClassificationValidation::calculateWeightedAccuracy()
{
	accuracy_=0;
	int no_all=0;
	// get number of substances that were used for training the model_
	for(int i=0; i<(int)clas_model->no_substances_.size();i++)
	{
		no_all += clas_model->no_substances_[i];
	}
	
	if(confusion_matrix_.Ncols()<2) // for binary classification normal df. of accuracy
	{
		for(int j=1;j<=confusion_matrix_.Ncols();j++)
		{	// accuracy_ = (TP+TN) / (TP+TN+FN+FP)
			double acc_j = (confusion_matrix_(1,j)+confusion_matrix_(3,j)) / (confusion_matrix_(1,j)+confusion_matrix_(3,j)+confusion_matrix_(4,j)+confusion_matrix_(2,j));
			
			accuracy_ = accuracy_ + acc_j*(((double)clas_model->no_substances_[j-1])/no_all);
		}
	}
	else
	{
		for(int j=1;j<=confusion_matrix_.Ncols();j++) 
		{	// multi-class accuracy := (TP) / (TP+FN+FP)
			double acc = (confusion_matrix_(1,j)) / (confusion_matrix_(1,j)+confusion_matrix_(4,j)+confusion_matrix_(2,j));
			double acc_weighted = acc*(((double)clas_model->no_substances_[j-1])/no_all);
			class_results_(j) += acc_weighted;
			accuracy_ += acc_weighted;
		}
	}
}



void ClassificationValidation::calculateAverageMCC()
{
	accuracy_=0;
	double MCC=0;
	for(int j=1;j<=confusion_matrix_.Ncols();j++)
	{
		int TP = (int)confusion_matrix_(1,j);
		int FP = (int)confusion_matrix_(2,j);
		int TN = (int)confusion_matrix_(3,j);
		int FN = (int)confusion_matrix_(4,j);
		
		double nom = ((double)TP)*TN-FP*FN; // (often) too big for int...
		double denom = ((double)(TP+FP))*(TP+FN)*(TN+FP)*(TN+FN);
		if(denom!=0) denom = sqrt(denom);
		else denom = 1; 
		
		double d = nom/denom;
		class_results_(j) += d;
		MCC += d;
	}
	accuracy_ = MCC/confusion_matrix_.Ncols();
}
	
	
void ClassificationValidation::calculateOverallMCC()
{
	accuracy_=0;
	int TP = 0; int FP=0; int TN=0; int FN=0;
	for(int j=1;j<=confusion_matrix_.Ncols();j++)
	{
		TP += (int)confusion_matrix_(1,j);
		FP += (int)confusion_matrix_(2,j);
		TN += (int)confusion_matrix_(3,j);
		FN += (int)confusion_matrix_(4,j);
	}
	double nom = ((double)TP)*TN-FP*FN; // (often) too big for int...
	double denom = ((double)(TP+FP))*(TP+FN)*(TN+FP)*(TN+FN);
	if(denom!=0) denom = sqrt(denom);
	else denom = 1; 
	accuracy_= nom/denom;
}



const Matrix* ClassificationValidation::getConfusionMatrix()
{
	return &confusion_matrix_;
}


const RowVector* ClassificationValidation::getClassResults()
{
	return &class_results_;
}


double ClassificationValidation::getAccuracyInputTest()
{
	return accuracy_input_test_;
}


double ClassificationValidation::getAccuracyCV()
{
	return accuracy_cv_;
}


double ClassificationValidation::getCVRes()
{
	return accuracy_cv_;
}
				

				
void ClassificationValidation::setCVRes(double d)
{
	accuracy_cv_=d;
}


double ClassificationValidation::getFitRes()
{
	return accuracy_input_test_;
}
