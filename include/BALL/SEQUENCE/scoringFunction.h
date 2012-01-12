


#ifndef BALL_SEQUENCE_SCORINGFUNCTION_H
#define BALL_SEQUENCE_SCORINGFUNCTION_H


//////////////////////////////////////////////////////////////////////
#ifndef BALL_DATATYPE_STRING_H
	#include <BALL/DATATYPE/string.h>
#endif

//////////////////////////////////////////////////////////////////////
#ifndef EIGEN_DENSE
	#include<Eigen/Dense>
#endif

//////////////////////////////////////////////////////////////////////


	class BALL_EXPORT ScoringFunction
{
	public:
		///////////////////////////////////////Constructors and Deconstructor//////////////////////////////////////////
			/**
			 *Default Constructor	
		 	*/
			ScoringFunction();

			/**
		 	*Detailed Constructor
		 	*@param scoring_matrix the matrix to compute the score of the alignment
		 	*/
			ScoringFunction(Eigen::Array<double, Eigen::Dynamic, Eigen::Dynamic> scoringMatrix);

			/**
			 *Copy Constructor
			 */
			ScoringFunction(ScoringFunction& scoring_function);

			/**
			 *Destructor
			 */
			~ScoringFunction();

			/////////////////////////////////////////////Getter and Setter///////////////////////////////////////////////

			/**
			 *@return scoring matrix
			 */
			Eigen::Array<double, Eigen::Dynamic, Eigen::Dynamic>	getScoringMatrix();

			/**
			 *@param matrix the scoring matrix to be set
			 */
			void setScoringMatrix(Eigen::Array<double, Eigen::Dynamic, Eigen::Dynamic> matrix);

			///////////////////////////////////////////////////Misc/////////////////////////////////////////////////////////

			/**
			 *computes the score of a given alignment
			 *@param alignment the alignment the score shall be computed for
			 */
			bool computeScore(BALL::Alignment& alignment);

				
	private:
			 Eigen::Array<double, Eigen::Dynamic, Eigen::Dynamic> scoringMatrix_;

};


#endif // BALL_SEQUENCE_SCORINGFUNCTION_H
