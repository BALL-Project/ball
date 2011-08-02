// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//


#ifndef BALL__SEQUENCE_SEQUENCEITERATOR_H
#define BALL__SEQUENCE_SEQUENCEITERATOR_H

/////////////////////////////////////////////////////////////////////////

#ifndef BALL_SEQUENCE_SEQUENCECHARACTER_H
	#include <BALL/SEQUENCE/sequenceCharacter.h>
#endif

#ifndef BALL_KERNEL_PROTEIN_H
#include <BALL/KERNEL/protein.h>
#endif

#ifndef BALL_STRUCTURE_PEPTIDES_H
#include <BALL/STRUCTURE/peptides.h>
#endif


//////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////


namespace BALL 
{
	class Sequence;


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
			void setSequence(Sequence& seq);

			/**
			 *@return the Sequence over which the Iterator iterates
			 */
			Sequence& getSequence();

			/**
			 *@return the current counter
			 */
			unsigned int getCounter();

			void setCounter( unsigned int i);

			/**
			 *@return the SequenceCharacter to which the Iterator points
			 */
			SequenceCharacter& getCharacter();

			/**
			*sets the Character 
			*@param c the character to be set
			*/
			void setChar(SequenceCharacter& c);

			//////////////////////////////////////////////Misc//////////////////////////////////////////////////////////

			/**
			 *@return a sequenceIterator pointing to the next Character of the Sequence
			 */
			SequenceIterator next();

			
			/**
			 *operator==
			 */
			bool operator == (SequenceIterator& it);




			/**
			 *operator !=
			 **/
			bool operator != (SequenceIterator& it);

	
		private:
			Sequence* sequence;

			unsigned int counter;
			
			SequenceCharacter character;
	};

}//namespace BALL
#endif // BALL_SEQUENCE_SEQUENCEITERATOR_
