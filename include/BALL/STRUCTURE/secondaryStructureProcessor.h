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

			String sheet;
		//	vector<char> bridge1;
		//	vector<char> bridge2;
			String Fiveturn;
			String Fourturn;
			String Threeturn;
			String summary;

		private:
			/** SecondaryStructureBool_ : matrix to save the existence of a SecondaryStructure
			*/
			//matrix to save the possible HBondPairs
			vector< vector<int> > HBonds_;
			//vector to save the bridges
			vector< vector <int> > posbridges_; 
			void insert_turn(int turn, int position);
      void change_all_X_to_Y(char X, char Y, String& target);			
			
	}; //class SecondaryStructureProcessor

}//namesspace BALL

#endif
