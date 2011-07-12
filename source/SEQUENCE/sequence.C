#include<BALL/SEQUENCE/sequence.h>

#include<BALL/SEQUENCE/sequenceIterator.h>



namespace BALL{


			/**
			 *Default Constructor
			 */
			Sequence::Sequence(){
						}
			/**
			 *Detailed Constructor
			 *@param name the name of the Sequence
			 *@param origin the composite from which the sequence originates
			 */
			Sequence::Sequence(String& seq_name, AtomContainer* seq_origin):name(seq_name),origin(seq_origin){
				}

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
			void Sequence::setName(BALL::String& seq_name){
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
			void Sequence::setOrigin(AtomContainer* seq_origin){
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
				return !operator==(seq);
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
			SequenceIterator& Sequence::begin(){

				//Initialize the new Iterator
				SequenceIterator* it = new SequenceIterator(*this);
				
				return *it;				
			
			}				

			/**
			 *@return a SequenceIterator which points to the last character of the Sequence
			 */
			SequenceIterator& Sequence::end(){
				
				//initialize the new Iterator
				SequenceIterator* it= new SequenceIterator();
				
				//set the the sequence
				it->setSequence(*this);
				
				//go to to the last chracter and return the iterator

				return it->last();
				
			}

			/**
			 *@return String which contains the Sequence
			 **/
			String Sequence::getSequence(){

			String seq = "";

			for(SequenceIterator it = begin(); it != end(); it.next()){
			
				String* tmp = new String(it.getCharacter().getChar());

				seq += *tmp;
			}

				return seq;
			}

}//namespace BALL
