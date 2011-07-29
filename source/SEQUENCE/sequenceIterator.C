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
											: counter(0),character()
		{	
				sequence = new Sequence();
			}

			/**
			 *Detailed Constructor
			 *@param seq the sequence through which we want to iterate
			 */
		SequenceIterator::SequenceIterator(Sequence& seq)
				:counter(0),character()
		{ *sequence=seq;	}

			/**
			 *Copy Constructor
			 *@param it the iterator to be copied
			 */
		SequenceIterator::SequenceIterator(SequenceIterator& it){
				*sequence = it.getSequence();
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
			void SequenceIterator::setSequence(Sequence& seq){
								*(sequence) = seq;
	
			}


			void SequenceIterator::setChar(SequenceCharacter& c)
			{
				if ( *(c.getOrigin()) != *sequence) 
				{
				throw BALL::Exception::InvalidArgument(__FILE__,__LINE__,"origins don't match");
				}
				character = c;
			}
			/**
			 *@return the Sequence over which the Iterator iterates
			 */
			Sequence& SequenceIterator::getSequence(){
				return *sequence;
			}

			/**
			 *@return the current counter
			 */
			unsigned int SequenceIterator::getCounter(){
				return counter;
			}

			/**
			 *@return the SequenceCharacter to which the Iterator points
			 */
			SequenceCharacter& SequenceIterator::getCharacter(){
					return character;
				}

			//////////////////////////////////////////////Misc//////////////////////////////////////////////////////////

			/**
			 *@return a sequenceIterator pointing to the next Character of the Sequence
			 */
			SequenceIterator SequenceIterator::next()
			{
							//get the residue at position counter
							AtomContainer& ori = *(sequence->getOrigin());

							if (RTTI::isKindOf<Protein>(ori)) {

											//cast origin to a protein
											Protein* protein = RTTI::castTo<Protein>(ori);

											//retrieve the residue and increase counter
											Residue* res = protein->getResidue(counter++);

											//retrieve the residue's name and cast it to OneLetterCode
											char name = Peptides::OneLetterCode(res->getID());

											//set the sequence character char
											character.setChar(name);
							}
						return *this;
			}

			
			/**
			*@return the current Iterator setted to the end character
			*/
			SequenceIterator SequenceIterator::end()
			{
							cout<<"inside Iteratorend()"<<endl;	
				//get the residue at position counter
					AtomContainer& ori = *(sequence->getOrigin());
						cout<<"test whether it is a protein"<<endl;

						if (RTTI::isKindOf<Protein>(ori)) {
								cout<<"in if statement ->cast to protein"<<endl;
								//cast origin to a protein
								Protein* protein = RTTI::castTo<Protein>(ori);
								cout<<"increase counter"<<endl;
								//set the counter to the numver of residues in the protein +1
								counter = protein->countResidues() + 1;
						}
					cout<<"returning now"<<endl;
				return *this;
			}

			/**
			*operator==
			*/
			bool SequenceIterator::operator == (SequenceIterator& it)
			{
				return ((*sequence == it.getSequence()) && (counter == it.getCounter()) && ( it.getCharacter() == character) );
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
