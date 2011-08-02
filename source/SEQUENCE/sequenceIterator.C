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


			//////////////////////////////////////////////Misc//////////////////////////////////////////////////////////

			/**
			 *@return a sequenceIterator pointing to the next Character of the Sequence
			 */
			SequenceIterator SequenceIterator::next()
			{
					//get the residue at position counter
					AtomContainer* ori = sequence->getOrigin();

					if (RTTI::isKindOf<Protein>(*ori)) 
					{
							//cast origin to a protein

							Protein* protein = RTTI::castTo<Protein>(*ori);
							
							if(counter >= protein->countResidues())
							{
								counter= protein->countResidues()+1;
								
								return *this;
							}

							//retrieve the residue and increase counter
							Residue* res = protein->getResidue(counter++);
							
							//retrieve the residue's name and cast it to OneLetterCode
							char name = Peptides::OneLetterCode(res->getName());

							//set the sequence character char
							character.setChar(name);
							}
						return *this;
			}

		
			
	
			
			/**
			*operator==
			*/
			bool SequenceIterator::operator == (SequenceIterator& it)
			{
				return ((sequence == it.sequence) && (counter == it.getCounter()) && ( it.getCharacter() == character));
			}


	

			/**
			*operator !=
			**/
			bool SequenceIterator::operator != (SequenceIterator& it)
			{
				return !(operator== (it));
			}

//

	}//namespace BALL
