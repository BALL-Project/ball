#ifndef BALL_SECONDARY_STRUCTURE_PROCESSOR_H
#define BALL_SECONDARY_STRUCTURE_PROCESSOR_H

#ifndef  BALL_COMMON_H
#include <BALL/common.h>
#endif

#ifndef VECTOR
# include <vector>
#endif

#ifndef BALL_CONCEPT_COMPOSITE_H
# include <BALL/CONCEPT/composite.h>
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
