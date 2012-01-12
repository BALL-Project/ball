#ifndef BALL_SEQUENCE_ALIGNALGORITHM_H
#define BALL_SEQUENCE_ALIGNALGORITHM_H

/////////////////////////////////////////////////////////////////////////
#ifndef BALL_DATATYPE_STRING_H
	#include <BALL/DATATYPE/string.h>
#endif


#ifndef BALL_SEQUENCE_ALIGNMENT_H
	#include <BALL/SEQUENCE/alignment.h>
#endif

///////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////
namespace BALL{

	typedef BALL_SIZE_TYPE Position;

	class BALL_EXPORT AlignAlgorithm
{
	public:
		///////////////////////////////////Constructors and Deconstructor///////////////////////////////////////////////

		/**
		 *Default Constructor
		 */
		AlignAlgorithm();

		/**
		 *Detailed Constructor
		 *@param alignment the alignment which is to be aligned
		 *@param scoring the Scoringfunction which should be used
		 **/
	//	AlignAlgorithm(BALL::Alignment& alignment);

		/**
		 *CopyConstructor
		 */
	//	AlignAlgorithm(AlignAlgorithm& align_algo);

		/**
		 *Destructor
		 */
		~AlignAlgorithm();

		//////////////////////////////////////////////////////Getter and Setter///////////////////////////////////////

		/**
		 *sets the alignment to a given alignment
		 *@param alignment the alignment to be set
		 */
		//void setAlignment(BALL::Alignment& alignment);

		/**
		 *@return the alignment which the Algorithm shall align
		 */
	//	BALL::Alignment& getAlignment();

			//////////////////////////////////////////////////////////Misc///////////////////////////////////////////////

		/**
		 *aligns the sequences contained in the alignment
		 */
		bool align(BALL::Alignment& alignment);

		/**
		 *aligns the sequences contained in the alignment starting at a specific Position
		 *@param 
		 */
		bool alignFromSpecificPosition(Alignment& alignment, Position pos);


//	private:
	//	BALL::Alignment alignment_;
	};

}//namespace BALL
#endif // BALL_SEQUENCE_ALIGNALGORITHM_H
