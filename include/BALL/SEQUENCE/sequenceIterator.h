#ifndef BALL__SEQUENCE_SEQUENCEITERATOR_H
#define BALL__SEQUENCE_SEQUENCEITERATOR_H

/////////////////////////////////////////////////////////////////////////

#ifndef BALL_SEQUENCE_SEQUENCECHARACTER_H
	#include <BALL/SEQUENCE/sequenceCharacter.h>
#endif

//////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////

	class Sequence;

namespace BALL 
{
	class BALL_EXPORT SequenceIterator
	{
		public:

			////////////////////////////////////////Constructors and Destructor//////////////////////////////////

			/**
			 *Default Constructor
			 */
			SequenceIterator();

			/**
			 *Detailed Constructor
			 *@param seq the sequence through which we want to iterate
			 */
			SequenceIterator(Sequence& seq);

			/**
			 *Copy Constructor
			 *@param it the iterator to be copied
			 */
			SequenceIterator(SequenceIterator& it);

			/**
			 *Destructor
			 */
			~SequenceIterator();

			/////////////////////////////////////////////Getter and Setter//////////////////////////////////////////

			/**
			 * @param seq the Sequence to be set
			 */
			void setSequence(const Sequence& seq);

			/**
			 *@return the Sequence over which the Iterator iterates
			 */
			Sequence& getSequence();

			/**
			 *@return the current counter
			 */
			int getCounter();

			/**
			 *@return the SequenceCharacter to which the Iterator points
			 */
			SequenceCharacter& getCharacter();

			//////////////////////////////////////////////Misc//////////////////////////////////////////////////////////

			/**
			 *@return a sequenceIterator pointing to the next Character of the Sequence
			 */
			void next();


		private:
			Sequence sequence;

			int counter;

			SequenceCharacter character;
	};

}//namespace BALL
#endif // BALL_SEQUENCE_SEQUENCEITERATOR_
