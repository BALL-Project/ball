#include "quaternionGene.h"
#include <iostream>
#include <BALL/MATHS/randomNumberGenerator.h>

using namespace std;


namespace BALL
{
  
  QuaternionGene::QuaternionGene()
  {
    double pi = 3.14159; 
    
    double x = rng_.randomDouble(0.0,1.0);
    
    double o = sqrt(1 - x);
    double b = sqrt(x);
    double c = 2 * pi * rng_.randomDouble(0.0,1.0);
    double d = 2 * pi * rng_.randomDouble(0.0,1.0);
    
    quat_.i = cos(d)*b;
    quat_.j = sin(c)*o;
    quat_.k = cos(c)*o; 
    quat_.angle = sin(d)*b;
    
  }
  
  QuaternionGene::~QuaternionGene()
  {
  }
  
  GenericGene* QuaternionGene::mate(GenericGene* gg)
  {
    if (QuaternionGene* qg = dynamic_cast<QuaternionGene*>(gg))
      {
	if ((quat_.i * qg->quat_.i + quat_.j * qg->quat_.j + quat_.k * qg->quat_.k + quat_.angle * qg->quat_.angle)<0)
	  {
	    qg->quat_.i = -qg->quat_.i;
	    qg->quat_.j = -qg->quat_.j;
	    qg->quat_.k = -qg->quat_.k;
	    qg->quat_.angle = -qg->quat_.angle;
	  }
	
	QuaternionGene* result = new QuaternionGene;
	
	double i = qg->quat_.i - quat_.i;
	double j = qg->quat_.j - quat_.j;
	double k = qg->quat_.k - quat_.k;
	double angle = qg->quat_.angle - quat_.angle;
	
	result->quat_.i = quat_.i - 0.5*i + 2 * i * rng_.randomDouble(0.0,1.0);
	result->quat_.j = quat_.j - 0.5*j + 2 * j * rng_.randomDouble(0.0,1.0);
	result->quat_.k = quat_.k - 0.5*k + 2 * k * rng_.randomDouble(0.0,1.0);
	result->quat_.angle = quat_.angle - 0.5*angle + 2 * angle * rng_.randomDouble(0.0,1.0);
	
	double t = sqrt(result->quat_.i*result->quat_.i + result->quat_.j*result->quat_.j + 
			result->quat_.k*result->quat_.k +result->quat_.angle*result->quat_.angle);
	
	result->quat_.i /= t;
	result->quat_.j /= t;	
	result->quat_.k /= t;
	result->quat_.angle /= t;
	
	return result;
	
      }
    else
      {
	cerr << "mating failed" << endl;
	exit(1);
      }
  }      
  
  
  void QuaternionGene::mutate()
  {
  }
  
  
  Quaternion QuaternionGene::getValue()
  {
    return quat_;
  }
  
  
  GenericGene* QuaternionGene::clone()
  {
    return new QuaternionGene(*this);
  }
}
