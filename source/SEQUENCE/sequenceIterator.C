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
											: counter_(0),
                        character_()
		{	
			sequence_ = NULL;
		}

			/**
			 *Detailed Constructor
			 *@param seq the sequence through which we want to iterate
			 */
		SequenceIterator::SequenceIterator(Sequence& seq)
				: counter_(0),
          character_()
		{ 
			sequence_= &seq;	
			character_.setOrigin(sequence_);

		}

			/**
			 *Copy Constructor
			 *@param it the iterator to be copied
			 */
		SequenceIterator::SequenceIterator(const SequenceIterator& it)
		{
				sequence_ = &(it.getSequence());
				counter_ = it.getCounter();
				character_ = it.getCharacter();
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
				sequence_ = &seq;
  		}


			void SequenceIterator::setChar(SequenceCharacter& c)
			{
					if ( (c.getOrigin()) != sequence_) 
					{
							throw BALL::Exception::InvalidArgument(__FILE__,__LINE__,"origins don't match");
					}

					character_ = c;
			}
		
			/**
			 *@return the Sequence over which the Iterator iterates
			 */
			Sequence& SequenceIterator::getSequence() const
			{
				return *sequence_;
			}

			/**
			 *@return the current counter_
			 */
			unsigned int SequenceIterator::getCounter() const
			{
				return counter_;
			}

			/**
			 *@return the SequenceCharacter to which the Iterator points
			 */
			SequenceCharacter SequenceIterator::getCharacter() const
			{
				return character_;
			}


			void SequenceIterator::setCounter(const unsigned int i)
			{
				counter_ = i;
			}


			////////////////////////////////////////////// Operators //////////////////////////////////////////////////////////

			/**
			 *@return a sequenceIterator pointing to the next Character of the Sequence
			 */
			void SequenceIterator::next()
			{
					
					AtomContainer* ori = sequence_->getOrigin();

					if (RTTI::isKindOf<Protein>(*ori)) 
					{
							//cast origin to a protein
							Protein* protein = RTTI::castTo<Protein>(*ori);

							//increase counter_
							counter_++;

							//check whether we are already at the end	
							if(counter_ > protein->countResidues())
							{			
									throw BALL::Exception::IndexOverflow(__FILE__,__LINE__,counter_, protein->countResidues());
							}

							//check whether we are at the last character
							if(counter_ == protein->countResidues()) 
							{		
									//create new empty SequenceCharacter
									character_.reset();

									//set its origin to the current sequence
									character_.setOrigin(sequence_);
				
								}
								else
								{
									//retrieve the residue
									Residue* res = protein->getResidue(counter_);
							
									//retrieve the residue's name and cast it to OneLetterCode
									char name = Peptides::OneLetterCode(res->getName());

									//set the SequenceCharacter
									character_.setChar(name);
									character_.setResidue(res);
							}
					}
					
			}


	void SequenceIterator::previous()
	{

		AtomContainer* ori = sequence_->getOrigin();

					if (RTTI::isKindOf<Protein>(*ori)) 
					{
							//cast origin to a protein
							Protein* protein = RTTI::castTo<Protein>(*ori);

							if(counter_==0)
							{
								throw BALL::Exception::IndexUnderflow(__FILE__,__LINE__,counter_,0);
							}

							//decrease counter_
							counter_--;

							//retrieve the residue
							Residue* res = protein->getResidue(counter_);
							
							//retrieve the residue's name and cast it to OneLetterCode
							char name = Peptides::OneLetterCode(res->getName());

							//set the SequenceCharacter
							character_.setChar(name);	
							character_.setResidue(res);

					}

				}

	void SequenceIterator::reset()
	{
		sequence_ = NULL;
		counter_ = 0;
		character_.reset();
	}

bool SequenceIterator::first()
{

AtomContainer* ori = sequence_->getOrigin();
	if (RTTI::isKindOf<Protein>(*ori)) 
			{

					Protein* p= RTTI::castTo<Protein>(*ori);
					if (p)
					{		character_.setOrigin(sequence_);
							character_.setResidue(p->getResidue(0));
							character_.setChar(Peptides::OneLetterCode(character_.getResidue()->getName()));
						counter_ = 0;
						return true;
					}
						}
	return false;
	
}

	bool SequenceIterator::last()
	{
	AtomContainer* ori = sequence_->getOrigin();
		if (RTTI::isKindOf<Protein>(*ori)) 
		{
					Protein* p= RTTI::castTo<Protein>(*ori);
					if (p)
					{		
							counter_ = p->countResidues() -1;
							character_.setOrigin(sequence_);
							character_.setResidue(p->getResidue(counter_));
							character_.setChar(Peptides::OneLetterCode(character_.getResidue()->getName()));
							return true;
					} 
		}
return false;
}

				bool SequenceIterator::operator == (const SequenceIterator& it) const
			{
				return ((sequence_ == it.sequence_) && (counter_ == it.getCounter()) && ( it.getCharacter() == character_));
			}

			bool SequenceIterator::operator != (const SequenceIterator& it) const
			{
				return !(operator== (it));
			}

	SequenceIterator& SequenceIterator::operator = (const SequenceIterator& it){

		sequence_ = it.sequence_;
		counter_ = it.counter_;
		character_ = it.character_;
	return *this;

	}

			SequenceCharacter& SequenceIterator::operator * (){
				return character_;
		}

//		SequenceIterator* operator -> ();

		/**
		* postfix - operator ++
		* increments iterator after returning it
		*@return unchanged Iterator
		*/
	SequenceIterator SequenceIterator::operator ++ ()
	{//Postfix increment
		SequenceIterator tmp = *this;
		next();
		return tmp;
	}

		/**
		* postfix - operator ++
		* increments iterator after returning it
		*@return unchanged Iterator
		*/
SequenceIterator& SequenceIterator::operator++ (int)
{ 
	next();
	return *this;
}

/**
	* postfix operator --
	*decrements SequenceIterator after returning
	*@ return unchanged Iterator
	*/
	SequenceIterator SequenceIterator::operator -- ()
{

//Postfix decrementation
	SequenceIterator tmp = *this;
	previous();
	return tmp;

	
}

	/**
	* operator-- decrements the SequenceIterator
	*prefix
	*@return incremented Iterator
	*/

SequenceIterator& SequenceIterator::operator --(int)
{	
	previous();
	return *this;

}




	/**
	* assignment operator
			*/
//SequenceIterator& operator - (SequenceIterator& it); TODO Das macht doch keine Sinn!

	/**
	* comparison operator
	*/

bool SequenceIterator::operator < (const SequenceIterator& it) const
{
	if (sequence_ != it.sequence_)
	{
		throw BALL::Exception::IncompatibleIterators(__FILE__,__LINE__);
	}

	if(counter_ < it.counter_) 
	{
		return true;
	}

	return false;
}

	/**
	* comparison operator
	*/
bool SequenceIterator::operator > (const SequenceIterator& it) const
{	
	if (sequence_ != it.sequence_)
	{
		throw BALL::Exception::IncompatibleIterators(__FILE__,__LINE__);
	}

	if(counter_ > it.counter_) 
	{
	return true;
	}

	return false;
}


	/**
	* comparison operator
	*/
bool SequenceIterator::operator <= (const SequenceIterator& it) const
{	
	if (sequence_ != it.sequence_)
	{
		throw BALL::Exception::IncompatibleIterators(__FILE__,__LINE__);
	}

if(counter_ <= it.counter_) 
{
return true;
}
return false;
}


	/**
	* comparison operator
	*/
bool SequenceIterator::operator >= (const SequenceIterator& it) const
{
	if (sequence_ != it.sequence_)
	{
		throw BALL::Exception::IncompatibleIterators(__FILE__,__LINE__);
	}
if(counter_ >= it.counter_)
{
return true;
}
return false;}

	/**
	*operator +
	* increments the SequenceIterator by n
	*@return incremented SequenceIterator
	*/
SequenceIterator SequenceIterator::operator + (const unsigned int  n)
{
	return SequenceIterator( *this ) += n;
}


//n + a

	/**
	*operator -
	* decrements the SequenceIterator by n
	*@return decremented SequenceIterator
 	*/
SequenceIterator SequenceIterator::operator - (const unsigned int n)
{
	return SequenceIterator(*this) -= n;
	}

	/**
	* assignment and increment operator
	*/
SequenceIterator& SequenceIterator::operator += (const unsigned int n){

AtomContainer* ori = sequence_->getOrigin();

					if (RTTI::isKindOf<Protein>(*ori)) 
					{
							//cast origin to a protein
							Protein* protein = RTTI::castTo<Protein>(*ori);

							//increase counter_
							counter_+= n;

							//check whether we are already at the end	
							if(counter_ > protein->countResidues())
							{			
									throw BALL::Exception::IndexOverflow(__FILE__,__LINE__,counter_, protein->countResidues());
							}

							//check whether we are at the last character
							if(counter_ == protein->countResidues()) 
							{		
									character_.reset();

									//set its origin to the current sequence
									character_.setOrigin(sequence_);
											
									return *this;
							}

							//retrieve the residue
							Residue* res = protein->getResidue(counter_);
							
							//retrieve the residue's name and cast it to OneLetterCode
							char name = Peptides::OneLetterCode(res->getName());

							//set the SequenceCharacter
							character_.setChar(name);
							character_.setResidue(res);

					}

 
	return *this;}


	/**
	* assignment and decrement operator
	*/
SequenceIterator& SequenceIterator::operator -= (const unsigned int n)
{
	AtomContainer* ori = sequence_->getOrigin();

					if (RTTI::isKindOf<Protein>(*ori)) 
					{
							//cast origin to a protein
							Protein* protein = RTTI::castTo<Protein>(*ori);

							if( (counter_ == 0) || (counter_ < n))
							{
								throw BALL::Exception::IndexUnderflow(__FILE__,__LINE__,counter_,0);
							}

							//decrease counter_
							counter_-=n;

							//retrieve the residue
							Residue* res = protein->getResidue(counter_);
							
							//retrieve the residue's name and cast it to OneLetterCode
							char name = Peptides::OneLetterCode(res->getName());

							//set the SequenceCharacter
							character_.setChar(name);
							character_.setResidue(res);

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
