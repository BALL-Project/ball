#ifndef BALL_SEQUENCE_ALIGNALGORITHM_H
#define BALL_SEQUENCE_ALIGNALGORITHM_H

/////////////////////////////////////////////////////////////////////////
#ifndef BALL_DATATYPE_STRING_H
	#include <BALL/DATATYPE/string.h>
#endif


#ifndef BALL_SEQUENCE_ALIGNMENT_H
	#include <BALL/SEQUENCE/alignment.h>
#endif

#ifndef BALL_SEQUENCE_SCORINGFUNCTION_H
	#include <BALL/SEQUENCE/scoringFunction.h>
#endif
///////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////

typedef BALL_SIZE_TYPE Position;

	class BALL_EXPORT alignAlgorithm
{
	public:
		///////////////////////////////////Constructors and Deconstructor///////////////////////////////////////////////

		/**
		 *Default Constructor
		 */
		alignAlgorithm();

		/**
		 *Detailed Constructor
		 *@param alignment the alignment which is to be aligned
		 *@param scoring the Scoringfunction which should be used
		 **/
		alignAlgorithm(BALL::Alignment& alignment, BALL::ScoringFunction& scoring);

		/**
		 *CopyConstructor
		 */
		alignmentAlgorithm(AlignmentAlgorithm& align_algo);

		/**
		 *Destructor
		 */
		~alignmentAlgorithm();

		//////////////////////////////////////////////////////Getter and Setter///////////////////////////////////////

		/**
		 *sets the alignment to a given alignment
		 *@param alignment the alignment to be set
		 */
		void setAlignment(BALL::Alignment& alignmen);

		/**
		 *@return the alignment which the Algorithm shall align
		 */
		BALL::Alignment& getAlignment();

		/**
		 *@param scoring_function the scoring_function to be set
		 */
		void setScoringFunction(ScoringFunction& scoring_function);


		/**
		 *@return the current ScoringFunction
		 */
		ScoringFunction& getScoringFunction();

		//////////////////////////////////////////////////////////Misc///////////////////////////////////////////////

		/**
		 *aligns the sequences contained in the alignment
		 */
		void align();

		/**
		 *aligns the sequences contained in the alignment starting at a specific Position
		 *@param 
		 */
		alignFromSpecificPosition(Position pos);


	private:
		BALL::Alignment alignment;
		ScoringFunction scroingfunction;
};


#endif // BALL_SEQUENCE_ALIGNALGORITHM_H
