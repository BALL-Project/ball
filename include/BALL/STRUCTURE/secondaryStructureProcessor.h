#ifndef BALL_SECONDARY_STRUCTURE_PROCESSOR_H
#define BALL_SECONDARY_STRUCTURE_PROCESSOR_H


#ifndef  BALL_COMMON_H
#include <BALL/common.h>
#endif

#ifndef BALL_KERNEL_PROTEIN_H
#include <BALL/KERNEL/protein.h>
#endif

#ifndef  BALL_KERNEL_CHAIN_H
#include <BALL/KERNEL/chain.h>
#endif

#ifndef  BALL_KERNEL_BOND_H
#include <BALL/KERNEL/bond.h>
#endif

#ifndef MATH_H
#include <math.h>
#endif

#ifndef BALL_STRUCTURE_RESIDUECHECKER_H
#include <BALL/STRUCTURE/residueChecker.h>
#endif

#ifndef BALL_STRUCTURE_FRAGMENTDB_H
#include <BALL/STRUCTURE/fragmentDB.h>
#endif

#ifndef BALL_MATHS_MATRIX44_H
#include <BALL/MATHS/matrix44.h> //4x4 Matrix
#endif

#ifndef BALL_MATHS_VECTOR3_H
#include <BALL/MATHS/vector3.h>  //TVector3
#endif

#ifndef BALL_MATHS_ANGLE_H
#include <BALL/MATHS/angle.h>     //TAngle
#endif

#ifndef VECTOR
#include <vector>
#endif

#ifndef BALL_DATATYPE_HASHGRID_H
#include <BALL/DATATYPE/hashGrid.h>
#endif

#ifndef BALL_MATHS_VECTOR4_H
#include <BALL/MATHS/vector4.h>
#endif

#ifndef  BALL_STRUCTURE_GEOMETRICPROPERTIES_H
#include <BALL/STRUCTURE/geometricProperties.h>
#endif

#ifndef   BALL_KERNEL_SECONDARYSTRUCTURE_H
#include <BALL/KERNEL/secondaryStructure.h>           
#endif

#ifndef  BALL_CONCEPT_PROPERTY_H
#include <BALL/CONCEPT/property.h>
#endif

namespace BALL
{
  
  /** This class represents processor
   */
  class SecondaryStructureProcessor :public UnaryProcessor<Composite>
	{

		public:
			/** Default constructor
			*/
			SecondaryStructureProcessor();

			/** Detailed constructor maybe it is helpful to use other criteria for detecting hbonds
			*/
			//SecondaryStructureProcessor();

			/** Destructor
			*/
			~ SecondaryStructureProcessor();


			/** start
			*/

			/** finish computes all hbonds of the composit
			*/
			bool finish();

			/** Compute the secondary structure
			 */
			void compute();
			
			/** operator()
			*/
			Processor::Result operator() (Composite &composite);

			vector<char> sheet;
			vector<char> bridge1;
			vector<char> bridge2;
			vector<char> Fiveturn;
			vector<char> Fourturn;
			vector<char> Threeturn;
			vector<char> summary;

	private:
			/** SecondaryStructureBool_ : matrix to save the existence of a SecondaryStructure
			*/
			vector< vector<int> > HBonds_;

	}; //class SecondaryStructureProcessor

}//namesspace BALL


#endif
