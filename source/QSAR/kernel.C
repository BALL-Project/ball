//  -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
//

#include <BALL/QSAR/kernel.h>
#include <newmatio.h>

using namespace BALL::QSAR;


/*Kernel(RegressionModel* m, int k_type, double p1, double p2=1)
{
	if(k_type==4)
	{
		throw Exception::KernelParameterError(__FILE__,__LINE__, "Equations must be specified in order to create a kernel with an individual kernel function!"); 
	}

	if(k_type==5)
	{
		throw Exception::KernelParameterError(__FILE__,__LINE__, "A linear model_ or a RowVector must be given in order to create a kernel with a weighted distance function!");
	}
	regr_model=m;
	par1=p1;
	par2=p2;
	f="";
	g="";
	type=k_type;
}*/	
	
	

	
Kernel::Kernel(Model* m, int k_type, double p1, double p2)
{
	if(k_type==4)
	{
		throw Exception::KernelParameterError(__FILE__,__LINE__, "Equations must be specified in order to create a kernel with an individual kernel function!"); 
	}

	if(k_type==5)
	{
		throw Exception::KernelParameterError(__FILE__,__LINE__, "A linear model_ or a RowVector must be given in order to create a kernel with a weighted distance function!");
	}
	model_=m;
	par1=p1;
	par2=p2;
	equation1="";
	equation2="";
	type=k_type;
}


Kernel::Kernel(Model* m, String s1, String s2) 
{
	type = 4;
	model_=m;
	par1=2;
	equation1=s1;
	equation2=s2;
}


Kernel::Kernel(Model* m, RowVector& w)
{
	type = 5;
	model_=m;
	par1=2;
	weights_=w;
}


Kernel::Kernel(Model* m, const LinearModel& lm, int column)
{
	model_=m;
	par1=2;
	type = 5;
	
	const Matrix* w = lm.getTrainingResult();
	if(w->Nrows()==0)
	{
		throw Exception::KernelParameterError(__FILE__,__LINE__, "linear model_ must be trained before its weights_ can be used for a weighted kernel function!");
	}
	RowVector v(w->Nrows());
	for(int i=1; i<=w->Nrows();i++)
	{
		v(i)=(*w)(i,column);
	}
	weights_=v;

	m->copyDescriptorIDs(lm);	
}

Kernel::~Kernel()
{
}

void Kernel::calculateKernelMatrix(Matrix& input, Matrix& output)
{
	if(type==1)
	{
		calculateKernelMatrix1(input, output);
	}
	else if(type==2)
	{
		calculateKernelMatrix2(input, output);
	}
	else if(type==3)
	{
		calculateKernelMatrix3(input, output);
	}
	else if(type==4)
	{
		calculateKernelMatrix4(input, output);
	}
	else if(type==5 && weights_.Ncols()!=0)
	{
		calculateWeightedKernelMatrix(input, output); 
		if(*model_->getType()=="GP"){return;}
	}
	
	// center Matrix output
 	IdentityMatrix I(output.Ncols());
 	ColumnVector iv(output.Ncols());
 	iv=1;
 	double d=(double)1/output.Ncols();
 	output = (I-d*iv*iv.t())*output*(I-d*iv*iv.t());
}


void Kernel::calculateKernelMatrix(Matrix& K, Matrix& m1, Matrix& m2, Matrix& output)
{
	if(type==1)
	{
		calculateKernelMatrix1(m1,m2,output);
	}
	else if(type==2)
	{
		calculateKernelMatrix2(m1,m2,output);
	}
	else if(type==3)
	{
		calculateKernelMatrix3(m1,m2,output);
	}
	else if(type==4)
	{
		calculateKernelMatrix4(m1,m2,output);
	}
	else if(type==5 && weights_.Ncols()!=0)
	{
		calculateWeightedKernelMatrix(m1, m2, output); 
		if(*model_->getType()=="GP"){return;}
	}
	
	// center Matrix output
	IdentityMatrix I(output.Ncols()); //dim: nxn
 	ColumnVector iv(m2.Nrows()); //dim: nx1
 	iv=1;
	ColumnVector ivt(output.Nrows());
 	ivt=1;
 	double d=(double)1/output.Ncols();
 	output = (output-d*ivt*(iv.t()*K))*(I-d*iv*iv.t());
}


void Kernel::gridSearch(double step_width, int steps, int recursions, int k, bool opt, double par1_start, double par2_start)
{
	bool first_rec=1;
	
	// do once + one time for each desired recursion
	for(int i=0; i<=recursions;i++)
	{
		gridSearch(step_width,steps,first_rec,k,par1_start,par1_start,opt);
		step_width = step_width/10;
		steps = 20;
		first_rec = 0;
		par1_start=par1-step_width;
		par2_start=par2+step_width;
	}


}

void Kernel::gridSearch(double step_width, int steps, bool first_rec, int k, double par1_start, double par2_start, bool opt)
{	
	//cout << "doing grid search ... "<<endl;
	
	double best_par1=par1;
	double best_par2=par2;
	model_->model_val->crossValidation(4,0);
	double best_cvres=model_->model_val->getCVRes();
	//cout<<" begin="<<best_cvres<<endl;
	if(par1_start!=0)
	{
		par1=par1_start;
	}
	if(par2_start!=0)
	{
		par2=par2_start;
	}
	
	if(type!=3) // for kernels that use only 1 parameter
	{
		if(type==2 && first_rec)  // exponential decreasing of gamma for RBF kernel
		{
			for(int i=1;i<=steps;i++)
			{
				if(!opt || !model_->optimizeParameters(k))
				{
					model_->model_val->crossValidation(k,0);
				//cout << par1 <<"  "<< model_->model_val->getCVRes()<<endl;	
				}
				if(model_->model_val->getCVRes()>best_cvres)
				{
					best_cvres=model_->model_val->getCVRes();
					best_par1=par1;
				}	
				par1 = par1/2;
			
			//cout << "-"<<(((double) i)/steps)*100<<"%";
			//cout.flush();
			}
			step_width = par1*2-par1;
		}
		else  // stepwise increasing
		{
			for(int i=1;i<=steps;i++)
			{
				//cout <<par1<<endl;
				if(!opt || !model_->optimizeParameters(k))
				{
					model_->model_val->crossValidation(k,0);
					//cout << par1 <<"  "<< model_->model_val->getCVRes()<<endl;
				}	
				if(model_->model_val->getCVRes()>best_cvres)
				{
					best_cvres=model_->model_val->getCVRes();
					best_par1=par1;
				}	
				par1 += step_width;
				
				//cout << "-"<<(((double) i)/steps)*100<<"%";
				//cout.flush();
			}
		}
	}
	else  // for kernels that use 2 parameter (sigmoid kernel)
	{
		double start2 = par2;
		
		for(int i=1;i<=steps;i++)
		{
			par2 = start2;
			
			for(int j=1;j<=steps;j++)
			{
				if(!opt || !model_->optimizeParameters(k))
				{
					model_->model_val->crossValidation(k,0);
					//cout << par1 <<"  "<<par2 <<"  "<<model_->model_val->getCVRes()<<endl;	
				}
				if(model_->model_val->getCVRes()>best_cvres)
				{
					best_cvres=model_->model_val->getCVRes();
					best_par1=par1;
					best_par2=par2;
				}
				par2 -= step_width;
			}	
			par1 += step_width;
			
			//cout << "-"<<(((double) i)/steps)*100<<"%";
			//cout.flush();
		}	
	}
	
	par1 = best_par1;
	par2 = best_par2;
	model_->model_val->setCVRes(best_cvres);

}



//----------------------------- private functions ---------------



void Kernel::calculateKernelMatrix1(Matrix& input, Matrix& output) // polynomial kernel
{
	output.ReSize(input.Nrows(),input.Nrows());
	//output=0;
	for(int i=1; i<=input.Nrows();i++)
	{
		for(int j=i; j<=input.Nrows(); j++)
		{	
			// get distance of the two current rows
			double d=Statistics::distance(input,i,j,par1);
			output(i,j)=d;
			output(j,i)=d;
		}
		
	}
}


void Kernel::calculateKernelMatrix1(Matrix& m1, Matrix& m2, Matrix& output) // polynomial kernel
{
	output.ReSize(m1.Nrows(),m2.Nrows());
	//output=0;
	for(int i=1; i<=m1.Nrows();i++)
	{
		for(int j=1; j<=m2.Nrows(); j++)
		{	
			output(i,j)=Statistics::distance(m1,m2,i,j,par1);
		}	
	}
}


void Kernel::calculateKernelMatrix2(Matrix& input, Matrix& output) // radial basis function kernel
{
	output.ReSize(input.Nrows(),input.Nrows());
	for(int i=1; i<=input.Nrows();i++)
	{
		for(int j=i; j<=input.Nrows(); j++)
		{	
			// get distance of the two current rows
			double d=exp(-par1*Statistics::euclDistance(input,input,i,j));
			output(i,j)=d;
			output(j,i)=d;
		}
	}		
}		
		
		
void Kernel::calculateKernelMatrix2(Matrix& m1, Matrix& m2, Matrix& output) // radial basis function kernel
{
	output.ReSize(m1.Nrows(),m2.Nrows());
	for(int i=1; i<=m1.Nrows();i++)
	{
		for(int j=1; j<=m2.Nrows(); j++)
		{	
			output(i,j)=exp(-par1*Statistics::euclDistance(m1,m2,i,j));
		}	
	}
}		
		
		
void Kernel::calculateKernelMatrix3(Matrix& input, Matrix& output) // sigmoid kernel
{
	output.ReSize(input.Nrows(),input.Nrows());
	for(int i=1; i<=input.Nrows();i++)
	{
		for(int j=i; j<=input.Nrows(); j++)
		{	
			double p=1;
			double d=tanh(par1*Statistics::distance(input,input,i,j,p)+par2);
			output(i,j)=d;
			output(j,i)=d;
		}
	}
}		
		
		
void Kernel::calculateKernelMatrix3(Matrix& m1, Matrix& m2, Matrix& output) // sigmoid kernel
{
	output.ReSize(m1.Nrows(),m2.Nrows());
	for(int i=1; i<=m1.Nrows();i++)
	{
		for(int j=1; j<=m2.Nrows(); j++)
		{	
			double p=1;
			output(i,j)=tanh(par1*Statistics::distance(m1,m2,i,j,p)+par2);
		}	
	}
}		
		
		
void Kernel::calculateKernelMatrix4(Matrix& input, Matrix& output) // individual kernel
{
	output.ReSize(input.Nrows(),input.Nrows());
	for(int i=1; i<=input.Nrows();i++)
	{
		for(int j=i; j<=input.Nrows(); j++)
		{	
			double d=Statistics::distance(input,input,i,j,equation1,equation2);
			output(i,j)=d;
			output(j,i)=d;
		}
	}
}		
		
		
void Kernel::calculateKernelMatrix4(Matrix& m1, Matrix& m2, Matrix& output) // individual kernel
{
	output.ReSize(m1.Nrows(),m2.Nrows());
	for(int i=1; i<=m1.Nrows();i++)
	{
		for(int j=1; j<=m2.Nrows(); j++)
		{	
			output(i,j)=Statistics::distance(m1,m2,i,j,equation1,equation2);
		}	
	}
}		


void Kernel::calculateWeightedKernelMatrix(Matrix& input, Matrix& output)
{
	if (input.Ncols()!=weights_.Ncols())
	{
		throw Exception::KernelParameterError(__FILE__,__LINE__, "Kernel.weights_ has wrong size! One weight for each column of the given matrix is needed in order to be able to calculate a weighted distance matrix!");
	}
	
	output.ReSize(input.Nrows(),input.Nrows());
	output=0;
	// for all substances, calculate their weighted cross-product
	for(int i=1; i<=input.Nrows();i++)
	{
		for(int j=i; j<=input.Nrows();j++)
		{
			// get distance of the two current rows
			for(int c=1;c<=input.Ncols();c++) 
			{
				output(i,j)+=weights_(c)*pow(input(i,c)-input(j,c),par1);
			}
			output(j,i)=output(i,j);
		}
	}
}


void Kernel::calculateWeightedKernelMatrix(Matrix& m1, Matrix& m2, Matrix& output)
{
	output.ReSize(m1.Nrows(),m2.Nrows());
	output=0;
	// for all substances, calculate their weighted cross-product
	for(int i=1; i<=m1.Nrows();i++)
	{
		for(int j=1; j<=m2.Nrows();j++)
		{
			// get distance of the two current rows
			for(int c=1;c<=m1.Ncols();c++) 
			{
				output(i,j)+=weights_(c)*pow(m1(i,c)-m2(j,c),par1);
			}
		}
	}
}



