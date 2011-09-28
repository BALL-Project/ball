// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//


#include<BALL/SEQUENCE/sequenceIterator.h>

#include <BALL/SEQUENCE/sequence.h>



namespace BALL{			

			/**
			 *Default Constructor
			 */
		SequenceIterator::SequenceIterator()
											: counter(0),
                        character()
		{	
			sequence = NULL;
		}

			/**
			 *Detailed Constructor
			 *@param seq the sequence through which we want to iterate
			 */
		SequenceIterator::SequenceIterator(Sequence& seq)
				: counter(0),
          character()
		{ 
			sequence=&seq;	
			character.setOrigin(sequence);

		}

			/**
			 *Copy Constructor
			 *@param it the iterator to be copied
			 */
		SequenceIterator::SequenceIterator(SequenceIterator& it)
		{
				sequence = &(it.getSequence());
				counter = it.getCounter();
				character= it.getCharacter();
			}


	

			/**
			 *Destructor
			 */
		SequenceIterator::~SequenceIterator(){}

			/////////////////////////////////////////////Getter and Setter//////////////////////////////////////////

			/**
			 * @param seq the Sequence to be set
			 */
			void SequenceIterator::setSequence(Sequence& seq)
			{
				sequence = &seq;
  		}


			void SequenceIterator::setChar(SequenceCharacter& c)
			{
					if ( (c.getOrigin()) != sequence) 
					{
							throw BALL::Exception::InvalidArgument(__FILE__,__LINE__,"origins don't match");
					}

					character = c;
			}
		
			/**
			 *@return the Sequence over which the Iterator iterates
			 */
			Sequence& SequenceIterator::getSequence()
			{
				return *sequence;
			}

			/**
			 *@return the current counter
			 */
			unsigned int SequenceIterator::getCounter()
			{
				return counter;
			}

			/**
			 *@return the SequenceCharacter to which the Iterator points
			 */
			SequenceCharacter& SequenceIterator::getCharacter()
			{
				return character;
			}


			void SequenceIterator::setCounter(unsigned int i)
			{
				counter = i;
			}


			////////////////////////////////////////////// Operators //////////////////////////////////////////////////////////

			/**
			 *@return a sequenceIterator pointing to the next Character of the Sequence
			 */
			SequenceIterator& SequenceIterator::next()
			{
					
					AtomContainer* ori = sequence->getOrigin();

					if (RTTI::isKindOf<Protein>(*ori)) 
					{
							//cast origin to a protein
							Protein* protein = RTTI::castTo<Protein>(*ori);

							//increase counter
							counter++;

							//check whether we are already at the end	
							if(counter > protein->countResidues())
							{			
									throw BALL::Exception::IndexOverflow(__FILE__,__LINE__,counter, protein->countResidues());
							}

							//check whether we are at the last character
							if(counter == protein->countResidues()) 
							{		
									//create new empty SequenceCharacter
									SequenceCharacter c;

									//set its origin to the current sequence
									c.setOrigin(sequence);

									//add it to the current Iterator
									character=c;
				
									return *this;
							}

							//retrieve the residue
							Residue* res = protein->getResidue(counter);
							
							//retrieve the residue's name and cast it to OneLetterCode
							char name = Peptides::OneLetterCode(res->getName());

							//set the SequenceCharacter
							character.setChar(name);
					}

					return *this;
			}

	SequenceIterator& SequenceIterator::previous()
	{

		AtomContainer* ori = sequence->getOrigin();

					if (RTTI::isKindOf<Protein>(*ori)) 
					{
							//cast origin to a protein
							Protein* protein = RTTI::castTo<Protein>(*ori);

							if(counter==0)
							{
								throw BALL::Exception::IndexUnderflow(__FILE__,__LINE__,counter,0);
							}

							//decrease counter
							counter--;

							//retrieve the residue
							Residue* res = protein->getResidue(counter);
							
							//retrieve the residue's name and cast it to OneLetterCode
							char name = Peptides::OneLetterCode(res->getName());

							//set the SequenceCharacter
							character.setChar(name);
					}

					return *this;
			}

	

				bool SequenceIterator::operator == (SequenceIterator& it)
			{
				return ((sequence == it.sequence) && (counter == it.getCounter()) && ( it.getCharacter() == character));
			}

			bool SequenceIterator::operator != (SequenceIterator& it)
			{
				return !(operator== (it));
			}

	SequenceIterator& SequenceIterator::operator = (const SequenceIterator& it){

		sequence = it.sequence;
		counter = it.counter;
		character=it.character;
	return *this;

	}

			SequenceCharacter& SequenceIterator::operator * (){
				return character;
		}

//		SequenceIterator* operator -> ();
	
	SequenceIterator& SequenceIterator::operator ++ ()
	{
//TODO check index!
		return next();
	}

		/**
		* postfix - operator ++
		* increments iterator after returning it
		*@return unchanged Iterator
		*/
SequenceIterator SequenceIterator::operator++ (int)
{ 
//TODO check index!
	//Postfix increment
		SequenceIterator tmp = *this;
		next();
		return tmp;
}


	/**
	* operator-- decrements the SequenceIterator
	*prefix
	*@return incremented Iterator
	*/
SequenceIterator& SequenceIterator::operator -- (){

						return previous();

}

	/**
	* postfix operator --
	*decrements SequenceIterator after returning
	*@ return unchanged Iterator
	*/
SequenceIterator SequenceIterator::operator --(int)
{
//Postfix decrementation
	SequenceIterator tmp = *this;
	previous();
	return tmp;
}




	/**
	* assignment operator
			*/
//SequenceIterator& operator - (SequenceIterator& it); TODO Das macht doch keine Sinn!

	/**
	* comparison operator
	*/

bool SequenceIterator::operator < (const SequenceIterator& it)
{
	if (sequence != it.sequence)
	{
		throw BALL::Exception::IncompatibleIterators(__FILE__,__LINE__);
	}

	if(counter < it.counter) 
	{
		return true;
	}

	return false;
}

	/**
	* comparison operator
	*/
bool SequenceIterator::operator > (const SequenceIterator& it)
{	
	if (sequence != it.sequence)
	{
		throw BALL::Exception::IncompatibleIterators(__FILE__,__LINE__);
	}

	if(counter > it.counter) 
	{
	return true;
	}

	return false;
}


	/**
	* comparison operator
	*/
bool SequenceIterator::operator <= (const SequenceIterator& it)
{	
	if (sequence != it.sequence)
	{
		throw BALL::Exception::IncompatibleIterators(__FILE__,__LINE__);
	}

if(counter <= it.counter) 
{
return true;
}
return false;
}


	/**
	* comparison operator
	*/
bool SequenceIterator::operator >= (const SequenceIterator& it)
{
	if (sequence != it.sequence)
	{
		throw BALL::Exception::IncompatibleIterators(__FILE__,__LINE__);
	}
if(counter >= it.counter)
{
return true;
}
return false;}

	/**
	*operator +
	* increments the SequenceIterator by n
	*@return incremented SequenceIterator
	*/
SequenceIterator SequenceIterator::operator + (int  n)
{
	return SequenceIterator( *this ) += n;
}


//n + a

	/**
	*operator -
	* decrements the SequenceIterator by n
	*@return decremented SequenceIterator
 	*/
SequenceIterator SequenceIterator::operator - (int n)
{
	return SequenceIterator(*this) -= n;
	}

	/**
	* assignment and increment operator
	*/
SequenceIterator& SequenceIterator::operator += (int n){

AtomContainer* ori = sequence->getOrigin();

					if (RTTI::isKindOf<Protein>(*ori)) 
					{
							//cast origin to a protein
							Protein* protein = RTTI::castTo<Protein>(*ori);

							//increase counter
							counter+= n;

							//check whether we are already at the end	
							if(counter > protein->countResidues())
							{			
									throw BALL::Exception::IndexOverflow(__FILE__,__LINE__,counter, protein->countResidues());
							}

							//check whether we are at the last character
							if(counter == protein->countResidues()) 
							{		
									//create new empty SequenceCharacter
									SequenceCharacter c;

									//set its origin to the current sequence
									c.setOrigin(sequence);

									//add it to the current Iterator
									character=c;
				
									return *this;
							}

							//retrieve the residue
							Residue* res = protein->getResidue(counter);
							
							//retrieve the residue's name and cast it to OneLetterCode
							char name = Peptides::OneLetterCode(res->getName());

							//set the SequenceCharacter
							character.setChar(name);
					}

 
	return *this;}


	/**
	* assignment and decrement operator
	*/
SequenceIterator& SequenceIterator::operator -= (int n)
{
	AtomContainer* ori = sequence->getOrigin();

					if (RTTI::isKindOf<Protein>(*ori)) 
					{
							//cast origin to a protein
							Protein* protein = RTTI::castTo<Protein>(*ori);

							if( (counter==0) || (counter < n))
							{
								throw BALL::Exception::IndexUnderflow(__FILE__,__LINE__,counter,0);
							}

							//decrease counter
							counter-=n;

							//retrieve the residue
							Residue* res = protein->getResidue(counter);
							
							//retrieve the residue's name and cast it to OneLetterCode
							char name = Peptides::OneLetterCode(res->getName());

							//set the SequenceCharacter
							character.setChar(name);
					}

					return *this;
}


	/**
	* access whats inside the ith SequenceIterator
	*/
//SequenceCharacter& SequenceIterator::operator[] (int n)
//{
	//return new SequenceCharacter();
//}




//

	}//namespace BALL
