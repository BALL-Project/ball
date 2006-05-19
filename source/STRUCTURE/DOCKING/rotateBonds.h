// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: rotateBonds.h,v 1.1.2.1 2006/05/19 14:18:33 leonhardt Exp $
// search for rotable bonds and rotate for values between 0 and 1 for each rotable bond

#ifndef _ROTATE_BONDS_
#define _ROTATE_BONDS_

#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/DATATYPE/hashSet.h>
#include <BALL/KERNEL/protein.h>

#include <vector>



namespace BALL
{
  class RotateBond;
  
  class RotateBonds
    {
    public:
      //CREATE(RotateBonds)
      
      RotateBonds();
      
      //void build(System& sys); 
      
      RotateBonds(Molecule& mol);
		
      /** copy constructor
       */
      RotateBonds(const RotateBonds& rb);
      
      /** assignment operator
       */
      RotateBonds& operator=(const RotateBonds& rb);
      
      /** default destructor
       */
      ~RotateBonds();
      
      /** rotate all rotable bonds according to vec
       */
      void rotate(std::vector<double> vec);
      
      /** returns number of rotable bonds
       */
      int numberBonds();
      
    private:
      
      /** vector to store all rotable bonds
       */
      std::vector<RotateBond*> bonds_;
      
      /** pointer to molecule
       */
      Molecule* mol_;
      
            
      void build(BALL::System& sys);
            
    };
}



#endif
