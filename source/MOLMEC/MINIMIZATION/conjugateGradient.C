// $Id: conjugateGradient.C,v 1.3 1999/12/30 17:36:34 pmueller Exp $
// Minimize the potential energy of a system using a nonlinear conjugate 
// gradient method with  line search

#include <BALL/MOLMEC/MINIMIZATION/conjugateGradient.h>
#include <BALL/COMMON/limits.h>

namespace BALL 
  {
  using namespace std; 

  // Set the default values for options of this class 
  // The control factors needed for the line search along a new search direction
  const char *ConjugateGradientMinimizer::Option::LINE_SEARCH_ALPHA = "line_search_alpha";
  const char *ConjugateGradientMinimizer::Option::LINE_SEARCH_BETA = "line_search_beta";
  const char *ConjugateGradientMinimizer::Option::STEP_LENGTH = "step_length"; 

  float ConjugateGradientMinimizer::Default::LINE_SEARCH_ALPHA = 1e-4;
  float ConjugateGradientMinimizer::Default::LINE_SEARCH_BETA = 0.9;   
  float ConjugateGradientMinimizer::Default::STEP_LENGTH = 1; 


  // Default constructor 
  // It does nothing but calling its base class constructor 
  ConjugateGradientMinimizer::ConjugateGradientMinimizer()
		                   : EnergyMinimizer()
     {
     }



   // Constructor initialized with a force field
   ConjugateGradientMinimizer::ConjugateGradientMinimizer(ForceField& force_field)
		: EnergyMinimizer()
      {
      valid_ = setup(force_field);

      if (!valid_)
        {
        Log.level(LogStream::ERROR) << " ConjugateGradientMinimizer setup failed! " << endl;
	}
      }

   // Constructor initialized with a force field and a snapshot manager 
   ConjugateGradientMinimizer::ConjugateGradientMinimizer(ForceField& force_field,
                 SnapShotManager *ssm)
		: EnergyMinimizer()
      {
      valid_ = setup(force_field,ssm);

      if (!valid_)
        {
        Log.level(LogStream::ERROR) << " ConjugateGradientMinimizer setup failed! " << endl;
	}
      }

	
   // Constructor initialized with a force field and a set of options
   ConjugateGradientMinimizer::ConjugateGradientMinimizer(ForceField& force_field, 
                 const Options& new_options) : EnergyMinimizer()
      {
      // The actual work is done in setup 
      valid_ = setup(force_field, new_options);

      if (!valid_)
        {
        Log.level(LogStream::ERROR) << " Setup of conjugate gradient minimizer has failed! " << endl; 
        }
      }

   // Constructor initialized with a force field, a snapshot manager, and a set of options
   ConjugateGradientMinimizer::ConjugateGradientMinimizer(ForceField& force_field, 
                   SnapShotManager *ssm, const Options& new_options)
		                                       : EnergyMinimizer()
      {
      // The actual work is done in setup 
      valid_ = setup(force_field, ssm, new_options);

      if (!valid_)
        {
        Log.level(LogStream::ERROR) << " Setup of conjugate gradient minimizer has failed! " << endl; 
        }
      }


  // The destructor
  ConjugateGradientMinimizer::~ConjugateGradientMinimizer()
  {
  }


  // The copy constructor 
  ConjugateGradientMinimizer::ConjugateGradientMinimizer 
                  (const ConjugateGradientMinimizer& rhs, bool /* deep */)
		       : EnergyMinimizer(rhs)
    {
    step_length_ = rhs.step_length_; 
    no_of_atoms_ = rhs.no_of_atoms_;
    new_gradient_ = rhs.new_gradient_;
    old_gradient_ = rhs.old_gradient_; 
    search_direction_ = rhs.search_direction_; 
    line_search_alpha_ = rhs.line_search_alpha_; 
    line_search_beta_ = rhs.line_search_beta_; 
    inv_search_dir_norm_ = rhs.inv_search_dir_norm_; 
    lambda_opt_ = rhs.lambda_opt_;
    new_gradient_norm_ = rhs.new_gradient_norm_;
    }

  // The assignment operator
  ConjugateGradientMinimizer& ConjugateGradientMinimizer::operator=
        (const ConjugateGradientMinimizer& rhs)
    {
    EnergyMinimizer::operator=(rhs);

    step_length_ = rhs.step_length_; 
    no_of_atoms_ = rhs.no_of_atoms_;
    new_gradient_ = rhs.new_gradient_;
    old_gradient_ = rhs.old_gradient_; 
    search_direction_ = rhs.search_direction_; 
    line_search_alpha_ = rhs.line_search_alpha_; 
    line_search_beta_ = rhs.line_search_beta_; 
    inv_search_dir_norm_ = rhs.inv_search_dir_norm_; 
    lambda_opt_ = rhs.lambda_opt_;
    new_gradient_norm_ = rhs.new_gradient_norm_;

    return *this;
    }
		
  // This method is responsible for doing the specific setup of this class       
  bool ConjugateGradientMinimizer::specificSetup()
    {
    // set the options  to their default values if not already set  
    line_search_alpha_ = options.setDefaultReal(ConjugateGradientMinimizer::Option::LINE_SEARCH_ALPHA, 
                                  ConjugateGradientMinimizer::Default::LINE_SEARCH_ALPHA);
    line_search_beta_ = options.setDefaultReal(ConjugateGradientMinimizer::Option::LINE_SEARCH_BETA, 
                                  ConjugateGradientMinimizer::Default::LINE_SEARCH_BETA);

    step_length_ = options.setDefaultReal(ConjugateGradientMinimizer::Option::STEP_LENGTH,
                                           ConjugateGradientMinimizer::Default::STEP_LENGTH); 

    // determine the number of atoms
    no_of_atoms_ = force_field_->getAtoms().size(); 

    // reserve memory
    new_gradient_.reserve(no_of_atoms_);
    old_gradient_.reserve(no_of_atoms_);
    search_direction_.reserve(no_of_atoms_);

    new_gradient_.resize(no_of_atoms_); 
    old_gradient_.resize(no_of_atoms_); 
    search_direction_.resize(no_of_atoms_); 

    new_gradient_norm_ = 0; 

    return true;
    }

  // Set explicitly the option line_search_alpha_
  void	ConjugateGradientMinimizer::setLineSearchAlpha(float alpha)
    {
    line_search_alpha_ = alpha;
    }

  // Set explicitly the option line_search_beta__
  void	ConjugateGradientMinimizer::setLineSearchBeta(float beta)
    {
    line_search_beta_ = beta;
    }

  // Set explicitly the option step_length_
  void	ConjugateGradientMinimizer::setStepLength(float length)
    {
    step_length_ = length;
    }

  // get  the value of option line_search_alpha_
  float	ConjugateGradientMinimizer::getLineSearchAlpha() const 
    {
    return line_search_alpha_; 
    }

  // get  the value of option line_search_beta__
  float	ConjugateGradientMinimizer::getLineSearchBeta() const 
    {
    return line_search_beta_; 
    }

  // get  the value of option step_length_ 
  float	ConjugateGradientMinimizer::getStepLength() const 
    {
    return step_length_; 
    }


  // This method calculates the current root-mean-square norm of
  // the gradient  in kJ/(mol A)
  BALL_INLINE double ConjugateGradientMinimizer::calculateRMSGradientNorm(vector<Vector3> &gradient)
    {
    // local variables 
    Size i;
    double result; 

    result = 0; 

    for(i = 0; i < no_of_atoms_; i++)
      {
      result += (gradient[i] * gradient[i]); 
      }

    // No scaling with Avogadro/1e13 necessary because gradient is already scaled ... 
    result = sqrt(result / (3 * no_of_atoms_));

    return result; 
    }



  // This method determines a new lambda along the search direction.
  // This is done via interpolation. 
  BALL_INLINE double ConjugateGradientMinimizer::determineNewLambda(double f_0,double f_1, 
                               double dir_grad_0,double dir_grad_1,double lambda_0,double lambda_1)
    {
    // local variables
    double tmp1,a,b,tmp2; 
    double lambda_diff,lambda_diff_2; 
    double lambda_diff_3; 
    double result; 

    // Do a cubic interpolation based on (0,f_0), (1,f_1), (0,dir_grad_0), (1,dir_grad_1)
    // f_0 : energy at left point 
    // f_1 : energy at right point 
    // dir_grad_0 : direction gradient at left point 
    // dir_grad_1 : direction gradient at right point 
    lambda_diff = lambda_1 - lambda_0;

    if(lambda_diff == 0)
      {
      // the intervall is of length 0
      return lambda_0;
      }

    lambda_diff_2 = lambda_diff * lambda_diff;
    lambda_diff_3 = lambda_diff_2 * lambda_diff; 


    tmp1 = f_1 - f_0; 

    a = 3 * (-2 * tmp1 + (dir_grad_0 + dir_grad_1) * lambda_diff) / lambda_diff_3;
    b = (3 * tmp1 - (2 * dir_grad_0 + dir_grad_1) * lambda_diff) / lambda_diff_2; 

    tmp2 = b * b - a * dir_grad_0;

    if(a != 0 && tmp2 > 0)
      {
      result = lambda_0 + (-b + sqrt(tmp2)) / a; 
      }
    else
      {
      // just a quadratic interpolation 
      // Note: b = 0 is then not possible by construction as this would be a linear
      // interpolation 
      result = lambda_0 - dir_grad_0 / (2 * b); 
      }

    return result; 

    } // end of method 'determineNewLambda' 




 
  // This method determines the new search direction. Along this
  // vector we will try to find the next solution 
  // If called with a true value, the current negative gradient is taken as the search direction 
  // Literature: - David Shanno "Conjugate Gradient Methods With Inexact Searches"
  //               Mathematics of Operations Research, Vol. 3, No. 3, August 1978, pp. 244-256
  //             - S. Watowich et. al "A Stable Rapidly Converging Conjugate Gradient Method
  //                                   for Energy Minimization"
  //               Journal of Computational Chemistry, Vol. 9, No. 6, pp. 650-661 (1988)
  BALL_INLINE void ConjugateGradientMinimizer::determineNewSearchDirection(bool return_gradient)
    {
    // local variables
    double factor1,factor2,factor3;
    double C_1,C_2,D_2,D_3,D_5,D_6,D_7,D_8; 
    double condition; 
    
    Size i;

    static bool first_call = true;
    static vector<Vector3> a_i,b_i,p_t,y_t,p_i,y_i; 
    static double D_1,D_4;
    static Size restart_frequency = 1; 

    if(return_gradient == true)
      {
      // take the current gradient as the new search direction
      search_direction_ = new_gradient_;

      if(new_gradient_norm_ != 0)
         inv_search_dir_norm_ = 1.0 / new_gradient_norm_; 
      else
         {
         Log.level(LogStream::ERROR) 
                       << " gradient has length zero -> program is aborted in line "
                       << __LINE__ << "in file " << __FILE__ << endl; 
         exit(0); 
         }

      return; 
      }

    // The new search direction d_new is calculated as follows:
    // d_new      = - b_i + D_8 / D_5 * a_i - (1 + D_6/D_5) * D_8/D_5 - D_7/D_5) * p_i            
    // with
    // Index i = old iteration 
    // a_i = D_1/D_4 * y_i - D_2/D_4 * y_t + (2 D_2/D_1 - D_3/D_4) * p_t
    // b_i = D_1/D_4 * g_neu - C_1/D_4 * y_t + (2 C_1/D_1 - C_2/D_4) * p_t
    // p_i = step_length_ * d_old;
    // p_t = step_length_t * d_t
    // D_1 = p_t * y_t         D_2 = p_t * y_i
    // D_3 = y_t * y_i         C_1 = p_t * grad_new
    // C_2 = y_t * grad_new    D_4 = y_t * y_t
    // D_5 = p_i * y_i         D_6 = a_i * y_i 
    // D_7 = a_i * grad_new    D_8 = p_i * grad_new 
    // y_i = grad_new - grad_old 
    // y_t = grad_new - grad_old  in iteration t
    // d_t = search direction in iteration t
    if(first_call == true)
     {
     restart_frequency = 3 * no_of_atoms_; 

     p_t.resize(no_of_atoms_);
     y_t.resize(no_of_atoms_);
     y_i.resize(no_of_atoms_); 
     p_i.resize(no_of_atoms_);
     a_i.resize(no_of_atoms_);
     b_i.resize(no_of_atoms_);

     first_call = false;

     D_1 = 0; 
     D_4 = 0; 

     // Do some initialisations 
     // Note: when this method is called for the first time, the 
     // minimisation algorithm must have already done one iteration! 

     for(i = 0; i < no_of_atoms_; i++)
       {
       p_t[i] = search_direction_[i] * lambda_opt_ * step_length_; 
       y_t[i] = old_gradient_[i] - new_gradient_[i]; // exchanged order because of sign
 

       D_1 += p_t[i] * y_t[i]; 
       D_4 += y_t[i] * y_t[i]; 
       }

     if(D_4 == 0)
       {
       Log.level(LogStream::ERROR) << " A division by zero will occur. Abort in " << endl 
                                   << " line " << __LINE__ << " file " << __FILE__ << endl; 
       // BAUSTELLE was genau jetzt machen ?
       exit(0); 
       }
     }

    double sum1 = 0; 
    double sum2 = 0; 

    for(i = 0 ; i <  no_of_atoms_; i++)
      {
      y_i[i]= old_gradient_[i] - new_gradient_[i]; // exchanged order because of sign
      p_i[i] = search_direction_[i] * step_length_ * lambda_opt_; 

      sum1 += y_i[i] * y_i[i]; 
      sum2 += p_i[i] * p_i[i]; 
      }

    if(sum1 == 0 || sum2 == 0)
      {
      // take the current gradient as the new search direction
      search_direction_ = new_gradient_;

      if(new_gradient_norm_ != 0)
         inv_search_dir_norm_ = 1.0 / new_gradient_norm_;
      else
         {
         Log.level(LogStream::ERROR)
                       << " gradient has length zero -> program is aborted in li
ne "
                       << __LINE__ << "in file " << __FILE__ << endl;
         exit(0);
         }

      return;

      }


   if((number_of_iteration_ % restart_frequency == 0))
      {
      condition = 0;

      for(i = 0 ; i <  no_of_atoms_; i++)
        {
        condition += new_gradient_[i]  * old_gradient_[i];
        }

      // take the absolute value
      condition = (condition < 0) ? -condition : condition;

      if(condition >= 0.2 * new_gradient_norm_ * new_gradient_norm_)
        {
        D_1 = 0; 
        D_4 = 0; 

        for(i = 0; i < no_of_atoms_; i++)
          {
          p_t[i] = search_direction_[i] * lambda_opt_ * step_length_; 
          y_t[i] = old_gradient_[i] - new_gradient_[i]; // exchanged order because of sign
  
  
          D_1 += p_t[i] * y_t[i]; 
          D_4 += y_t[i] * y_t[i]; 
          }
  
        if(D_4 == 0)
          {
          Log.level(LogStream::ERROR) << " A division by zero will occur. Abort in " << endl 
                                      << " line " << __LINE__ << " file " << __FILE__ << endl; 
          // BAUSTELLE was genau jetzt machen ?
          exit(0); 
          }
  
        } 
      }
  

    // calculate all the auxiliary values

    D_2 = 0;
    for(i = 0 ; i <  no_of_atoms_; i++)
      {
      D_2 += p_t[i] * y_i[i];  
      }

    D_3 = 0;
    for(i = 0 ; i <  no_of_atoms_; i++)
      {
      D_3 += y_t[i] * y_i[i];  
      }

    C_1 = 0;
    for(i = 0 ; i <  no_of_atoms_; i++)
      {
      C_1 += p_t[i] * new_gradient_[i] ;  
      }

    C_1 *= -1; 

    C_2 = 0;
    for(i = 0 ; i <  no_of_atoms_; i++)
      {
      C_2 += y_t[i] * new_gradient_[i] ;  
      }

    C_2 *= -1; 

    factor1 = D_1 / D_4;
    factor2 = D_2/D_4;
    factor3 = (2 * D_2 / D_1 - D_3 / D_4); 

    for(i = 0 ; i <  no_of_atoms_; i++)
      {
      a_i[i] = y_i[i] * factor1 - y_t[i] * factor2 + p_t[i] * factor3; 
      }

    factor1 *= -1; // because we will multiply with new_gradient_, which is
                   // the negative gradient 

    factor2 = C_1 / D_4;
    factor3 = (2 * C_1 / D_1 - C_2 / D_4);

    for(i = 0 ; i <  no_of_atoms_; i++)
      {
      b_i[i] = new_gradient_[i] * factor1 - y_t[i] * factor2 + p_t[i] * factor3; 
      }

    D_5 = 0;
    for(i = 0 ; i <  no_of_atoms_; i++)
      {
      D_5 += p_i[i] * y_i[i] ;  
      }

    if(D_5 == 0)
      {
      Log.level(LogStream::ERROR) << " A division by zero will occur in " << endl 
                                  << " line " << __LINE__ << " file " << __FILE__ << endl; 
      Log.level(LogStream::ERROR) << " Take current gradient as new search direction..."
                                  << endl; 

      // take the current gradient as the new search direction
      search_direction_ = new_gradient_;

      if(new_gradient_norm_ != 0)
         inv_search_dir_norm_ = 1.0 / new_gradient_norm_;
      else
         {
         Log.level(LogStream::ERROR)
                       << " gradient has length zero -> program is aborted in line "
                       << __LINE__ << "in file " << __FILE__ << endl;
         exit(0);
         }

      return;
      }

    D_6 = 0;
    for(i = 0 ; i <  no_of_atoms_; i++)
      {
      D_6 += a_i[i] * y_i[i] ;  
      }

    D_7 = 0;
    for(i = 0 ; i <  no_of_atoms_; i++)
      {
      D_7 += a_i[i] * new_gradient_[i] ;  
      }

    D_7 *= -1; 
    
    D_8 = 0;
    for(i = 0 ; i <  no_of_atoms_; i++)
      {
      D_8 += p_i[i] * new_gradient_[i] ;  
      }

    D_8 *= -1; 



/*
      {
      // something is going wrong. Do not change the search direction
      Log.level(LogStream::ERROR) << " Division by zero when trying to calculate " << endl
                                  << " a new search vector " << endl; 
      return; 
      }
*/

    // set the new search direction
    double search_dir_norm = 0; 

    factor1 = D_8 / D_5;
    factor2 = (1 + D_6 / D_5) * factor1 - D_7 / D_5; 

    for(i = 0; i < no_of_atoms_; i++)
      {
      search_direction_[i] = - b_i[i] + a_i[i] * factor1 - p_i[i] * factor2;
      search_dir_norm += search_direction_[i] * search_direction_[i]; 
      }


    // Calculate the inverse of the search direction length 
    // This is needed later on for the right scaling 
    if(search_dir_norm != 0)
       inv_search_dir_norm_ = 1.0 / sqrt(search_dir_norm); 
    else
       {
       search_direction_ = new_gradient_;

       inv_search_dir_norm_ = 1.0 / new_gradient_norm_; 
       }


    } // end of method 'determineSearchDirection'





  // This method determines a new solution. The optimal step length for the current
  // search direction is found by a line search approach along the search direction. 
  // The method assumes that energies and forces are up to date for
  // the current conformation of the system. 
  // Return value is true if a new and better solution was found.  
  bool ConjugateGradientMinimizer::determineNewSolution()
    {
    // local variables
    bool last_try = false;
    bool searching; 

    double f_0,f_1,dir_grad_0,dir_grad_1,tmp_dir_grad;
    double fabs_dir_grad_0; 
    double tmp_gradient_norm,f_tmp,tmp_grad_norm_2; 
    double lambda_test,lambda_0,lambda_1; 
    double initial_dir_grad,initial_energy; 
    double fabs_initial_dir_grad;
    double factor, best_f;
    double best_lambda = -1;
    double best_step; 

    int failure_counter = 0; 
    const int MAX_FAILURE_COUNTER = 10; 

    Size i; 

    // the atoms of the system 
    static vector<Atom *> &atom_vector = 
                      const_cast<vector<Atom*>&>(force_field_->getAtoms()); 

    // contains the start positions for the line search
    static vector<Vector3 > startposition; 

    // contains a temporary gradient 
    static vector<Vector3 > tmp_gradient; 


    startposition.clear(); 
    tmp_gradient.clear(); 

    // save the start positions  
    for(i = 0; i < no_of_atoms_; i++)
       {
       startposition.push_back(atom_vector[i]->getPosition()); 
       }
   
    // Save the current gradient. It will be used in the next iteration
    // for determining a new search direction 
    old_gradient_ = new_gradient_; 

    // Determine the  norm of the direction gradient 
    // at the start position = dir_grad_0
    // The new_gradient_ must be up-to-date !
    // direction gradient = gradient * direction / length of direction
    dir_grad_0 = 0;

    for(i = 0; i < no_of_atoms_; i++)
      {
      dir_grad_0 += new_gradient_[i] * search_direction_[i]; 
      }

    // Multiply by -1 because new_gradient actually contains the negative gradient 
    dir_grad_0 *=  -1.0 * inv_search_dir_norm_; 
    fabs_dir_grad_0 = fabs(dir_grad_0); 


   
    // Check if the direction gradient is negative. This is a prerequisite. If this is
    // not the case, take the current negative gradient.

    if(dir_grad_0 > 0.0)
      {
      determineNewSearchDirection(true); 

      // Recalculate the direction gradient = gradient * direction / length of direction 
      dir_grad_0 = 0;

      for(i = 0; i < no_of_atoms_; i++)
       {
       dir_grad_0 += new_gradient_[i] * search_direction_[i]; 
       }

      // Multiply by -1 because new_gradient contains the negative gradient 
      dir_grad_0 *=  -1.0 * inv_search_dir_norm_; 
      fabs_dir_grad_0 = fabs(dir_grad_0); 
      }

    // remember the values for the starting point 
    initial_dir_grad = dir_grad_0;
    fabs_initial_dir_grad = fabs_dir_grad_0;

    lambda_0 = 0;
    lambda_1 = 1; 

    // store the values of the initial solution (starting point) 
    // f_0 : energy at start point 
    f_0 = force_field_->getEnergy(); 
    initial_energy = f_0; 


    // Determine a new lambda between lambda_0 and lambda_1 
    searching = true;


    while(searching)
       {
       // we start with lambda = 1 
       lambda_test = 1.0;

       // We first have to bracket a minimum along the vector 'search_direction_'
       // The left end of the start interval is naturally zero and hence ok
       // The  right end must be detected. If a lambda and its resulting function value
       // has a higher energy or a non-negative direction gradient, we have bracketed a minimum.
       // This minimum becomes the right end of the search interval. 
       // Otherwise, we must search a new right end.  
       while(true)
         {
         // Calculate a new solution
         factor = lambda_test * step_length_ * inv_search_dir_norm_; 

         for(i = 0; i < no_of_atoms_; i++)
            {
            atom_vector[i]->setPosition(startposition[i] + search_direction_[i] * factor); 
            }

         // Determine new energy and gradient
         force_field_->updateEnergy();
         force_field_->updateForces();

         force_update_counter_++; 
         energy_update_counter_++; 

         f_1 = force_field_->getEnergy(); 

         // Check if this energy value is a valid floating point number
         if(isnan(f_1))
           {
           Log.level(LogStream::ERROR) << " NaN value obtained when calculating energies. " 
                                       << " There must be some numerical problem. Abort in "
                                       << endl
                                       << "line " << __LINE__ << ", file " << __FILE__ 
                                       << endl; 
           exit(0); 
           }



  
         // determine the gradient at this point 
         tmp_gradient.clear(); 
         tmp_gradient_norm = 0; 

         for(i = 0; i < no_of_atoms_; i++)
            {
            tmp_gradient.push_back(atom_vector[i]->getForce() * Constants::AVOGADRO * 1e-13); 
            tmp_gradient_norm += tmp_gradient[i] * tmp_gradient[i]; 
            }

         tmp_gradient_norm = sqrt(tmp_gradient_norm); 

         // determine the direction gradient at this point 
         dir_grad_1 = 0;
   
         for(i = 0; i < no_of_atoms_; i++)
           {
           dir_grad_1 += tmp_gradient[i] * search_direction_[i]; 
           }

         // because new_gradient_ is actually the negative gradient we must multiply by -1 
         dir_grad_1 *= -1 * inv_search_dir_norm_;  



         // Is this solution better? If so, go on in this direction 
         if(f_1 < f_0  && dir_grad_1 < 0)
           {
           // increase the step length and go on  
           step_length_ *= 2.0;
           }
         else
           {
           // We have bracketed a minimum. 
           // Determine a first tentative lambda for the local minimum along the search vector  
           lambda_test = determineNewLambda(f_0,f_1,dir_grad_0,dir_grad_1,lambda_0,lambda_1); 

           // Test if the tentative lambda from the interpolation polynomial
           // is not too small
           if(lambda_test < 0.1)
             {
             step_length_ *= 0.5;

             // determine a new lambda 
             factor = 1.0 * step_length_ * inv_search_dir_norm_; 

             for(i = 0; i < no_of_atoms_; i++)
                {
                atom_vector[i]->setPosition(startposition[i] + search_direction_[i] * factor); 
                }

             // determine new energy and gradient
             f_1 = force_field_->updateEnergy();
             force_field_->updateForces();
             force_update_counter_++;
             energy_update_counter_++; 

             // determine the gradient at this point 
             tmp_gradient.clear(); 
             tmp_gradient_norm = 0; 

             for(i = 0; i < no_of_atoms_; i++)
                {
                tmp_gradient.push_back(atom_vector[i]->getForce() * Constants::AVOGADRO * 1e-13); 
                tmp_gradient_norm += tmp_gradient[i] * tmp_gradient[i]; 
                }

             tmp_gradient_norm = sqrt(tmp_gradient_norm); 

             // determine the direction gradient at this point 
             dir_grad_1 = 0;
   
             for(i = 0; i < no_of_atoms_; i++)
               {
               dir_grad_1 += tmp_gradient[i] * search_direction_[i]; 
               }

             // because new_gradient_ is actually the negative gradient we must multiply by -1 
             dir_grad_1 *= -1 * inv_search_dir_norm_; 

             lambda_test = determineNewLambda(f_0,f_1,dir_grad_0,dir_grad_1,lambda_0,lambda_1); 
             }

           break;  // the bracketing interval seems to be reasonable. Quit this loop. 
           }

         } // end of while

       best_f = f_0;
       best_step = step_length_;
       best_lambda = 0; 

       bool first_lambda = false; 

       double lambda_first; 
       double f_first, dir_grad_first;


       // We have now determined an interval and a first "optimal" lambda where the 
       // function value becomes minimal.
       // Now check if the corresponding function value is alright. If not, determine
       // a new lambda and so on. 

       while(true)
         {
         // apply the new lambda and get a new tentative solution 
         factor = lambda_test * step_length_ * inv_search_dir_norm_;

         for(i = 0; i < no_of_atoms_; i++)
           {
           atom_vector[i]->setPosition(startposition[i] + search_direction_[i] * factor); 
           }
   
         // Determine the new energy for this tentative solution 
         force_field_->updateEnergy(); 
         energy_update_counter_++; 

         f_tmp = force_field_->getEnergy(); 

         // calculate the current gradient and its direction gradient 
         force_field_->updateForces(); 
         force_update_counter_++; 

         tmp_dir_grad = 0; 
         tmp_grad_norm_2 = 0; 

         for(i = 0; i < no_of_atoms_; i++)
           {
           tmp_gradient[i] = atom_vector[i]->getForce() * Constants::AVOGADRO * 1e-13;
           tmp_grad_norm_2 += tmp_gradient[i] * tmp_gradient[i]; 
           tmp_dir_grad += tmp_gradient[i] * search_direction_[i];
           }

        if(first_lambda == false)
          {
          // this is the first lambda for this interval
          lambda_first = lambda_test;
          f_first = f_tmp;
          dir_grad_first = tmp_dir_grad; 

          first_lambda = true; 
          }

         // multiply by -1 because tmp_gradient is actually the negative gradient 
         tmp_dir_grad *= -1.0 * inv_search_dir_norm_;  

         // Check if the tentative solution is good enough 
         if(f_tmp <= initial_energy + line_search_alpha_
                              * lambda_test  * initial_dir_grad)
            {
            //  If the alpha condition is fulfilled, check the beta  criterion
            //  This test is normally less stringent than the alpha test and normally
            //  passed 
            if(f_tmp < best_f)
              {
              best_f = f_tmp;
              best_step = step_length_;
              best_lambda = lambda_test; 
              }



            if(fabs(tmp_dir_grad) <= line_search_beta_ * fabs_initial_dir_grad)
              {
              // we have found a good solution
              // save the tentative gradient as the new gradient
              new_gradient_ = tmp_gradient;
              new_gradient_norm_ = sqrt(tmp_grad_norm_2); 

              lambda_opt_ = lambda_test; 

              if(lambda_opt_ < 0.01)
                {
                step_length_ *= 0.5; 
                }

              if(lambda_opt_ > 0.9)
                {
                step_length_ *= 1.1; 
                }

              return true; 
              }
            
            }

          // We have encountered a failure -> go on searching if this has not happened too often
          if(last_try)
            {
            if(best_lambda > 0)
              {
              // choose the best solution met in this line search 
              lambda_opt_ = best_lambda;
              step_length_ = best_step; 

              factor = lambda_opt_ * step_length_ * inv_search_dir_norm_; 

              for(i = 0; i < no_of_atoms_; i++)
                {
                atom_vector[i]->setPosition(startposition[i] + search_direction_[i] * factor);
                }

              force_field_->updateEnergy();
              force_field_->updateForces();
              force_update_counter_++;
              energy_update_counter_++; 

              new_gradient_norm_ = 0;

              for(i = 0; i < no_of_atoms_; i++)
                {
                new_gradient_[i] = atom_vector[i]->getForce() *  Constants::AVOGADRO * 1e-13;
                new_gradient_norm_ += new_gradient_[i] * new_gradient_[i];
                }

              new_gradient_norm_ = sqrt(new_gradient_norm_); 
              }
            else
              {
              new_gradient_ = tmp_gradient;
              new_gradient_norm_ = sqrt(tmp_grad_norm_2); 

              lambda_opt_ = lambda_test; 
              }

            // take the current gradient as the new search direction 
            determineNewSearchDirection(true); 

            // take a smaller step length 
            step_length_ = 1;   

            if(lambda_opt_ < 0.001)
              {
              lambda_opt_ = 0.1; 
              }

            return true;
            }


          failure_counter++;

          if(failure_counter <= MAX_FAILURE_COUNTER)
            {
            // go on searching in the left or right sub-interval
            if(tmp_dir_grad > 0 || f_tmp > f_0)
              {
              // the left sub-interval
              f_1 = f_tmp; 
              dir_grad_1 = tmp_dir_grad; 

              lambda_1 = lambda_test; 
              }
            else
              {
              // the right sub-interval
              f_0 = f_tmp;
              dir_grad_0 = tmp_dir_grad;

              lambda_0 = lambda_test; 
              }
            }
          else
            {
            if(failure_counter == MAX_FAILURE_COUNTER + 1)
               {
               // Take the negative initial gradient as the new search direction and reset the step length
               determineNewSearchDirection(true); 

               // reset the step length 
               step_length_ = ConjugateGradientMinimizer::Default::STEP_LENGTH; 
               }
             else
               if(failure_counter == MAX_FAILURE_COUNTER + 2)
                 {
                 // Drastically cut down the step length and give it a last try
                 last_try = true;
                 step_length_ *= 0.1; 
                 }

            }

         // Determine a new lambda_opt for the next try 
         lambda_test = determineNewLambda(f_0,f_1,dir_grad_0,dir_grad_1,lambda_0,lambda_1); 

         // Check if the calculated lambda is reasonable. If it is outside the current
         // search interval, the function behaves very wildly and the step length should
         // be changed and the search starts anew. 

         if(lambda_test > lambda_1 || lambda_test < lambda_0)
           {
           lambda_test = 0.5 * (lambda_0 + lambda_1);
           }

         } // end of while(true)

       } // end of while(searching)   

    // to pacify the compiler...
    return false; 

    } // end of method 'determineNewSolution' 



  // The minimiser optimises the energy of the system 
  // by using a conjugate gradient method. 
  // Return value is true when convergence is achieved 
  bool ConjugateGradientMinimizer::minimize(Size iterations, bool restart)
    {
    // local variables
    Size max_iteration,i; 
    double rms_gradient_norm; 

    // The atoms of the system 
    vector<Atom *> &atom_vector = 
                      const_cast<vector<Atom*>&>(force_field_->getAtoms());   

    bool result = false; 

    float old_energy,new_energy;
    static Size	same_energy_counter = 0; // for how many iterations is the energy unchanged 

    // Check the minimizer  and the force field connected to it 
    if (isValid() == false || force_field_->isValid() == false)
      {
      Log.level(LogStream::ERROR) << "The ConjugateGradientMinimizer is not valid or " 
                                  << endl
                                  << " the force field is not valid!. " << endl; 

      return false;
      }

    // If there are no atoms  in the system, minimisation is easy!
    if(no_of_atoms_ == 0)
      {
      return true;
      }

     // the first direction vector is the current negative gradient,
     // i.e. the vector of all force vectors 
     force_field_->updateEnergy();
     force_field_->updateForces();
     new_energy = force_field_->getEnergy() ; 
     
     energy_update_counter_++;
     force_update_counter_++; 

     new_gradient_norm_ = 0;

int not_selected = 0; 
     for(i = 0; i < no_of_atoms_; i++)
       {
       if(atom_vector[i]->isSelected() == false)
         {
         not_selected++;
         }

       new_gradient_[i]     = atom_vector[i]->getForce() * Constants::AVOGADRO * 1e-13; 
       old_gradient_[i]     = new_gradient_[i];

       new_gradient_norm_ += (new_gradient_[i] * new_gradient_[i]);
       }


     new_gradient_norm_ = sqrt(new_gradient_norm_);

     if(new_gradient_norm_ == 0)
       {
       // the optimum is already reached!
       Log.info() << " Start conformation is already optimal!" << endl; 
       return true;
       }

     // The first search direction is the negative gradient       
     determineNewSearchDirection(true); 

    
     // Calculate the real step length which is in multiples of the initial gradient length 
     // If this is a restart, use the old step length 
     if(restart == false)
       {
       step_length_ =  ConjugateGradientMinimizer::Default::STEP_LENGTH ; 

       force_update_counter_ = 0;
       energy_update_counter_ = 0;
       }



     // If no upper bound for the number of iterations is given, the value stored in 
     // the options will be the limit
     if(iterations == 0)
       {
       max_iteration = number_of_iteration_ + maximal_number_of_iterations_;
       }
     else
       {
       max_iteration = number_of_iteration_ + iterations;
       }

       
     lambda_opt_ = 1.0;   // this variable contains the optimal value for the line 
                          // search from the previous iteration 
 
     // The main loop starts  here

     while(number_of_iteration_++ < max_iteration)
       {
       // check if the current conformation fulfills the convergence condition 
       rms_gradient_norm = calculateRMSGradientNorm(new_gradient_); 

       if(number_of_iteration_ % energy_output_frequency_ == 0)
         {
         Log.info() << "ConjugateGradient Minimizer iteration: " << 
                                                      number_of_iteration_ - 1 << endl;
         Log.info() << "   current RMS-Gradient  kJ/(mol A)  : " << rms_gradient_norm << endl; 
         Log.info() << "   current potential energy kJ/mol   : " << new_energy << endl; 
         }
 
       if(rms_gradient_norm <= max_gradient_ || same_energy_counter >= max_same_energy_)
         {
         // convergence! 
         result = true; 
         break; 
         }
 
       if(number_of_iteration_ % force_field_->getUpdateFrequency() == 0)
         {
         // Update the force field pair lists in regular intervals
         force_field_->update();
         }

       if(snapShot_ptr_ != 0 && number_of_iteration_ % snapshot_frequency_ == 0)
         {
         // Take a snapshot of the current conformation 
         snapShot_ptr_->takeSnapShot(); 
         }

       old_energy = force_field_->getEnergy(); 

       result = determineNewSolution();

       if(result == false)
         {
         break; 
         }

       new_energy = force_field_->getEnergy();

       // check if there is much difference between the previous solution and the
       // new one
       if(fabs(new_energy - old_energy) < energy_difference_bound_)
         {
         // count if there is the same energy between last iteration and
         // this iteration 
         same_energy_counter++; 
         }
       else
         {
         same_energy_counter = (same_energy_counter > 0) ?  same_energy_counter - 1 : 0;
         }

       // Find a new search direction for the next iteration 
       determineNewSearchDirection(); 

       } // end of while-loop

    number_of_iteration_--; // one augmentation too much ...

    Log.info() << endl << "Minimization stopped after " << number_of_iteration_ 
               << " of " << max_iteration 
               << " iterations  " << endl; 
    Log.info() << " RMS gradient kJ/(mol A) : " << calculateRMSGradientNorm(new_gradient_) << endl ; 
    Log.info() << " energy       kJ/ol      : " << new_energy << endl; 

    Log.info() << endl ;
    Log.info() << "No. of force updates     : " << force_update_counter_ << endl;
    Log.info() << "No. of energy updates    : " << energy_update_counter_ << endl;
    Log.info() << endl;



    return result; 

    } // end of method 'minimize' 
	
  } // end of namespace BALL
