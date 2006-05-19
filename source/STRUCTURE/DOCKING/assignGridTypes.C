#include "assignGridTypes.h"
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/molecule.h>


namespace BALL
{
  
  int gridType(const Atom& at)
  {
    int type = at.getType();
    
    /** set all oxygen types to single type 
     */
    if (at.getTypeName() == "O2")
      type = 41;
    
    /** set all carbon elements
     */
    if (at.getElement().getName() == "Carbon")
      type = 2;
    
    /** set all nitrogen elements
     */		
    if (at.getElement().getName() == "Nitrogen")
      type = 39;


    return type;
  }   

}
