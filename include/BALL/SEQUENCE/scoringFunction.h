#ifndef BALL_SEQUENCE_SCORINGFUNCTION_H
#define BALL_SEQUENCE_SCORINGFUNCTION_H


//////////////////////////////////////////////////////////////////////
#ifndef BALL_DATATYPE_STRING_H
	#include <BALL/DATATYPE/string.h>
#endif

#ifndef BALL_MATHS_MATRIX44_H
	#include <BALL/MATHS/matrix44.h>
#endif
//////////////////////////////////////////////////////////////////////

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
		ScoringFunction(TMatrix4x4<double> scoringMatrix;

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
				getScoringMatrix();

				/**
				 *@param matrix the scoring matrix to be set
				 */
				setScoringMatrix();

				///////////////////////////////////////////////////Misc/////////////////////////////////////////////////////////

				/**
				 *computes the score of a given alignment
				 *@param alignment the alignment the score shall be computed for
				 */
				computeScore(BALL::Alignment& alignment);

				/**
				 * reads a scoring Function from a given File
				 *@param file the file to be read
				 */
				void read(BALL::File file);

				/**writes a scoringFunction to a given file
				 *@param file the file to which the scoring function is written
				 */
				void write(BALL::File file);

	private:
				BALL::TMatrix4x4<double> scoringMatrix;

};


#endif // BALL_SEQUENCE_SCORINGFUNCTION_H
