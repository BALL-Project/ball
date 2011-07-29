// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//


#include<BALL/SEQUENCE/sequence.h>

#include<BALL/SEQUENCE/sequenceIterator.h>

#include <BALL/SEQUENCE/sequenceCharacter.h>

namespace BALL{


			/**
			 *Default Constructor
			 */
			Sequence::Sequence()
				:name(),origin()
			{  }
			/**
			 *Detailed Constructor
			 *@param name the name of the Sequence
			 *@param origin the composite from which the sequence originates
			 */
			Sequence::Sequence(String& seq_name, AtomContainer* seq_origin):name(seq_name),origin(seq_origin)
			{  }

			/**
			 *Copy Constructor
			 */
			Sequence::Sequence(Sequence& seq){
				name = seq.getName();
				origin =  seq.getOrigin();
				}

			/**
			 *Deconstrcutor
			 **/
			Sequence::~Sequence(){

			}

			


			/**
			 * @param name the new name to be set
			  */
			void Sequence::setName(const BALL::String& seq_name){
				name= seq_name;
			}

			/**
			 *@return the name of the sequence
			 */
			String& Sequence::getName(){
				return name;
			}


			/**
			 *@param origin the origin which is to be setted
			 */
			void Sequence::setOrigin(AtomContainer* seq_origin)
			{
			origin=seq_origin;
			}

			/**
			 *@return the origin of the Sequence
			 */
			AtomContainer* Sequence::getOrigin(){
				return origin;
			}	

			/**
			*operator==
			*/
			bool Sequence::operator== (Sequence& seq){
				return ((name==seq.getName()) && (origin==seq.getOrigin()));
			}

			/**
			*operator!=
			*/
			bool Sequence::operator!= (Sequence& seq){
				return ! (operator==(seq));
			}
			
			/**
			*assignment operator
			*/
			Sequence& Sequence::operator= (Sequence& seq){
				name = seq.getName();
				origin = seq.getOrigin();

				return *this;
			}



			/**
			 *@return a SequenceIterator which points to the first character of the Sequence
			 */
			SequenceIterator Sequence::begin(){

				//Initialize the new Iterator
					SequenceIterator *it = new SequenceIterator();
					it->setSequence(*this);

				if (RTTI::isKindOf<Protein>(*origin)) 
				{

					//create new Sequencecharacter
					SequenceCharacter c;
					c.setOrigin(this);

					Protein* p= RTTI::castTo<Protein>(*origin);

					c.setChar(Peptides::OneLetterCode(p->getResidue(0)->getName()));


					it->setChar(c);

					return *it;
				}
				
				throw BALL::Exception::InvalidArgument(__FILE__,__LINE__,"origin is no Protein");
					
			}				

			/**
			 *@return a SequenceIterator which points to the end character of the Sequence
			 */
			SequenceIterator Sequence::end(){
				
				cout<<"inside Sequence-end() now"<<endl;
				//initialize the new Iterator
				SequenceIterator* it= new SequenceIterator();
				
				//set the the sequence
				it->setSequence(*this);
				
				cout<<"returning now"<<endl;
				//go to to the end chracter and return the iterator
			
				*it = it->end();
	
				return *it;
				
			}

			/**
			 *@return String which contains the Sequence
			 **/
			String Sequence::getStringSequence(){
			
			cout<< "inside get StringSequence now..."<<endl;

			String* nseq = new String();
			cout<< "created a new String ->entering loop now"<<endl;

			SequenceIterator it;
				 it = begin();

			cout<<"created Sequenceiterator.."<<endl;		

			SequenceIterator e;
				e = end();
				cout<<"created end-it now.."<<endl;

			while(it != e){
			//for(SequenceIterator it = begin(); it != end(); it.next()){

			cout<< "inside the loop now " <<endl;	

				String* tmp = new String(it.getCharacter().getChar());

				cout<< "append now the sequence "<< endl;
				*nseq += *tmp;
				
				cout <<"the current seq is: "<< *nseq <<endl;
				cout<< "increasing the iterator "<<endl; 
				it=it.next();
			}

				cout<< "this is the sequence returned: "<< *nseq << endl;
				return *nseq;
			}

	Eigen::Array<SequenceCharacter,Eigen::Dynamic,1> Sequence::getArraySequence(){

		//initialize the array
		Eigen::Array<SequenceCharacter, Eigen::Dynamic, 1> seq;
			
		SequenceIterator it,e;
		e = end(); 

		for(it = begin(); it != e; it.next())
			{
			//make room for the new Character
			seq.conservativeResize(seq.rows());

			//retrieve the current Sequencecharacter for each Step
			SequenceCharacter c = it.getCharacter();

			//store it in the Array
			seq(seq.rows()-1) = c;
			}
		return seq;
	}

}//namespace BALL
