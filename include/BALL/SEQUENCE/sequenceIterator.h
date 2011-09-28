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


	class BALL_EXPORT SequenceIterator //: RandomAccessIterator
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

			/////////////////////////////////// Operators //////////////////////////////////////////////

			/**
			 *operator==
			 */
			bool operator == (SequenceIterator& it);


			/**
			* assignment operator
			*/
			SequenceIterator& operator = (const SequenceIterator& it);

			/**
			* dereference operator
			*@ return the current SequenceCharacter
			*/
			SequenceCharacter& operator * ();

			/**
			* operator
			*/
//		SequenceIterator* operator -> ();
	
			/**
			* operator++ increments the Iterator
			*@return the incremented Iterator
			*@throws Exception if reaching the end
			*/
	SequenceIterator& operator ++ ();

		/**
		* postfix - operator ++
		* increments iterator after returning it
		*@return unchanged Iterator
		*/
SequenceIterator operator++ (int);//Postfix
//Integer operator ++(int) {
  //   Integer tmp = *this;
     /* hier was immer postfix++ mit *this machen soll*/
    // return tmp;
//}


	/**
	* operator-- decrements the SequenceIterator
	*prefix
	*@return incremented Iterator
	*/
SequenceIterator& operator -- ();

	/**
	* postfix operator --
	*decrements SequenceIterator after returning
	*@ return unchanged Iterator
	*/
SequenceIterator operator --(int); //Postfix

	/**
	*operator +
	* increments the SequenceIterator by n
	*@return incremented SequenceIterator
	*/
SequenceIterator operator + (int  n);

//n + a

	/**
	*operator -
	* decrements the SequenceIterator by n
	*@return decremented SequenceIterator
 	*/
SequenceIterator operator - (int n);

	/**
	* assignment operator
			*/
//SequenceIterator& operator - (SequenceIterator& it); TODO Das macht doch keine Sinn!

	/**
	* comparison operator
	*/

bool operator < (const SequenceIterator& it);

	/**
	* comparison operator
	*/
bool operator > (const SequenceIterator& it);

	/**
	* comparison operator
	*/
bool operator <= (const SequenceIterator& it);

	/**
	* comparison operator
	*/
bool operator >= (const SequenceIterator& it);

	/**
	* assignment and increment operator
	*/
SequenceIterator& operator += (int i);

	/**
	* assignment and decrement operator
	*/
SequenceIterator& operator -= (int i);

	/**
	* access whats inside the ith SequenceIterator
	*/
//SequenceCharacter& operator[] (int i); 


		/**
		 *operator !=
		 **/
		bool operator != (SequenceIterator& it);



			/**
			 *@return a sequenceIterator pointing to the next Character of the Sequence
			 */
			SequenceIterator& next();

		/**
		*@return the previous SequenceIterator
		*/
		SequenceIterator& previous();	
			
	
		private:
			Sequence* sequence;

			unsigned int counter;
			
			SequenceCharacter character;
	};

}//namespace BALL
#endif // BALL_SEQUENCE_SEQUENCEITERATOR_
