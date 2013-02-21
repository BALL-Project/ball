#ifndef BALL_SEQUENCE_ALIGNALGORITHM_H
#define BALL_SEQUENCE_ALIGNALGORITHM_H

#ifndef BALL_DATATYPE_STRING_H
#include <BALL/DATATYPE/string.h>
#endif

#ifndef BALL_SEQUENCE_ALIGNMENT_H
#include <BALL/SEQUENCE/alignment.h>
#endif

namespace BALL
{

	typedef BALL_SIZE_TYPE Position;

	class BALL_EXPORT AlignAlgorithm
	{
		public:

			/**
			 *Default Constructor
			 */
			AlignAlgorithm();

			/**
			 *Destructor
			 */
			~AlignAlgorithm();

			/**
			 *aligns the sequences contained in the alignment
			 */
			bool align(BALL::Alignment& alignment);

			/**
			 *aligns the sequences contained in the alignment starting at a specific Position
			 *@param
			 */
			bool alignFromSpecificPosition(Alignment& alignment, Position pos);


	};

}//namespace BALL
#endif // BALL_SEQUENCE_ALIGNALGORITHM_H
