#include<BALL/SEQUENCE/sequenceIterator.h>

#include <BALL/SEQUENCE/sequence.h>



namespace BALL{			

			/**
			 *Default Constructor
			 */
		SequenceIterator::SequenceIterator():sequence(),counter(0),character(){	}

			/**
			 *Detailed Constructor
			 *@param seq the sequence through which we want to iterate
			 */
		SequenceIterator::SequenceIterator(Sequence& seq):counter(0),character(){
			//TODO initialize chracter with the right one!
			*sequence=seq;

			//set the characters origin to the seq
			character.setOrigin(sequence);

			//initialize the character with the first chracter of the Sequence
/*
			if (RTTI::isKindOf<Protein>(sequence->getOrigin())) {

				//cast origin to a protein
				Protein* protein = RTTI::castTo<Protein>(sequence->getOrigin());

				//retrieve the first residue
				Residue* res = protein->getResidue(0);

				//retrieve the residue's name and cast it to OneLetterCode
				char name = Peptides::OneLetterCode(res->getID());

				//set the sequence character char
				character.setChar(name);
			}
			else {

				if (sequence->getOrigin()->hasAncestor()) {

					if (RTTI::isKindOf<Protein>(sequence->getOrigin()->getFirstChild())) {

						//cast the first child of origin to a protein
						Protein* protein = RTTI::castTo<Protein>(sequence->getOrigin()->getFirstChild());

						//retrieve the first residue
						Residue* res = protein->getResidue(0);

						//retrieve the residue's name and cast it to OneLetterCode
						char name = Peptides::OneLetterCode(res->getID());

						//set the sequence character char
						character.setChar(name);
					}

				}

			}
*/		
		}

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
				*sequence=seq;
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
			int SequenceIterator::getCounter(){
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
		//	SequenceIterator& SequenceIterator::next();

			/**
			*@return the current Iterator setted to the last character
			*/
		//	SequenceIterator& SequenceIterator::last();

			/**
			*operator==
			*/
		//	bool SequenceIterator::operator == (SequenceIterator& it);

			/**
			*operator !=
			**/
		//	bool SequenceIterator::operator != (SequenceIterator& it);
			
			
}//namespace BALL
