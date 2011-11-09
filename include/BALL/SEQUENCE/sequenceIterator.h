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

#ifndef BALL_KERNEL_ATOMCONTAINER_H
#include <BALL/KERNEL/atomContainer.h>
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
			SequenceIterator(const SequenceIterator& it);


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
			Sequence& getSequence() const;

			/**
			 *@return the current counter
			 */
			unsigned int getCounter() const;

			void setCounter( const unsigned int i);

			/**
			 *@return the SequenceCharacter to which the Iterator points
			 */
			SequenceCharacter getCharacter() const;

			/**
			*sets the Character 
			*@param c the character to be set
			*/
			void setChar(SequenceCharacter& c);

			/////////////////////////////////// Operators //////////////////////////////////////////////

			/**
			 *operator==
			 */
			bool operator == (const SequenceIterator& it) const;


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
		* postfix - operator ++
		* increments iterator after returning it
		*@return unchanged Iterator
		*/

	SequenceIterator operator ++ ();

	/**
			* operator++ increments the Iterator
			*@return the incremented Iterator
			*@throws Exception if reaching the end
			*/

		SequenceIterator& operator++ (int);


	/**
	* postfix operator --
	*decrements SequenceIterator after returning
	*@ return unchanged Iterator
	*/
SequenceIterator operator -- ();
	/**
	* operator-- decrements the SequenceIterator
	*prefix
	*@return incremented Iterator
	*/

SequenceIterator& operator --(int); 
	/**
	*operator +
	* increments the SequenceIterator by n
	*@return incremented SequenceIterator
	*/
SequenceIterator operator + (const unsigned int  n);

//n + a

	/**
	*operator -
	* decrements the SequenceIterator by n
	*@return decremented SequenceIterator
 	*/
SequenceIterator operator - (const unsigned int n);

	/**
	* assignment operator
			*/
//SequenceIterator& operator - (SequenceIterator& it); TODO Das macht doch keine Sinn!

	/**
	* comparison operator
	*/

bool operator < (const SequenceIterator& it) const;

	/**
	* comparison operator
	*/
bool operator > (const SequenceIterator& it) const;

	/**
	* comparison operator
	*/
bool operator <= (const SequenceIterator& it) const;

	/**
	* comparison operator
	*/
bool operator >= (const SequenceIterator& it) const;

	/**
	* assignment and increment operator
	*/
SequenceIterator& operator += (const unsigned int i);

	/**
	* assignment and decrement operator
	*/
SequenceIterator& operator -= (const unsigned int i);

	/**
	* access whats inside the ith SequenceIterator
	*/
//SequenceCharacter& operator[] (int i); 


		/**
		 *operator !=
		 **/
		bool operator != (const SequenceIterator& it) const;



			/**
			 *@return a sequenceIterator pointing to the next Character of the Sequence
			 */
			void next();

		/**
		*@return the previous SequenceIterator
		*/
		void previous();	
		
		/**
		*resets the current iterator to default
		*/	
		void reset();

		/**
		* sets the Iterator to the first element
		*@return true, if setting was ok
		*/
		bool first();

		/** 
		*sets the Iterator to the last element	
		*@return true, if setting was ok
		*/
		bool last();

	
		private:
			Sequence* sequence_;

			unsigned int counter_;
			
			SequenceCharacter character_;
	};

}//namespace BALL
#endif // BALL_SEQUENCE_SEQUENCEITERATOR_
