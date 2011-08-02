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
			//	:name(""),origin(0)
			{ 
				name= "";
				origin = 0;
			 }
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
			Sequence::Sequence(Sequence& seq)
			{
				name = seq.getName();
				origin =  seq.getOrigin();
			}

			/**
			 *Deconstrcutor
			 **/
			Sequence::~Sequence()
			{		}

			


			/**
			 * @param name the new name to be set
			  */
			void Sequence::setName(const BALL::String& seq_name)
			{
				name= seq_name;
			}

			/**
			 *@return the name of the sequence
			 */
			String& Sequence::getName()
			{
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
			AtomContainer* Sequence::getOrigin()
			{
				return origin;
			}	

			/**
			*operator==
			*/
			bool Sequence::operator== (Sequence& seq)
			{
				return ((name==seq.getName()) && (origin==seq.getOrigin()));
			}

			/**
			*operator!=
			*/
			bool Sequence::operator!= (Sequence& seq)
			{
				return ! (operator==(seq));
			}
			
			/**
			*assignment operator
			*/
			Sequence& Sequence::operator= (Sequence& seq)
			{
		if (!origin) 
		{
cout<<"origin unintialised"<<endl;
}
else {
cout << "Doch nicht null" <<endl;
}

if(name == "") {
	cout<<"name empty"<<endl;
}
cout << "this.name=" <<  name << endl;

if(seq.name =="") cout <<"seq.name empty"<<endl;
cout<<"the name of the right sequence is: << "<< name<<endl;
				name = seq.getName();
				cout<<"setted the name"<<endl;
				origin = seq.getOrigin();
cout<<"setted origin"<<endl;
				return *this;
		}



			/**
			 *@return a SequenceIterator which points to the first character of the Sequence
			 */
			SequenceIterator Sequence::begin()
			{
//	cout<<"inside SequenceBegin() now"<< endl;
				//Initialize the new Iterator
					SequenceIterator it;
					it.setSequence(*this);

				if (RTTI::isKindOf<Protein>(*origin)) 
				{
					//create new Sequencecharacter
					SequenceCharacter c;
					c.setOrigin(this);
					Protein* p= RTTI::castTo<Protein>(*origin);
				
					if (p)
					{	
						c.setChar(Peptides::OneLetterCode(p->getResidue(0)->getName()));
				
					it.setChar(c);

					it.setCounter(1);

//	cout<<"leaving SequenceBegin() now"<< endl;

					
					return it;
					}
				}
				
				throw BALL::Exception::InvalidArgument(__FILE__,__LINE__,"origin is no Protein");
//			cout<<"leaving SequenceBegin() now"<< endl;

			}				

			/**
			 *@return a SequenceIterator which points to the end character of the Sequence
			 */
			SequenceIterator Sequence::end(){
				
//cout<<"inside Sequence-end() now"<<endl;

						//initialize the new Iterator
						SequenceIterator it;
				
						//set the the sequence
						it.setSequence(*this);
				
			
						if (RTTI::isKindOf<Protein>(*origin)) {

							//cast origin to a protein
							Protein* protein = RTTI::castTo<Protein>(*origin);

							int num_of_res = protein->countResidues();

							//set the counter to the number of residues in the protein
							it.setCounter(num_of_res+1);

							SequenceCharacter c;
							c.setOrigin(this);
							c.setChar(Peptides::OneLetterCode(protein->getResidue(num_of_res-1)->getName()));
				
							it.setChar(c);

						}
//cout<<"returning now"<<endl;

						return it;
			}





			/**
			 *@return String which contains the Sequence
			 **/
			String Sequence::getStringSequence(){
			
			String* nseq = new String();
			
			SequenceIterator it;  
			 it = begin();

			SequenceIterator e;
				e = end();
	
			while(it != e){
			//for(SequenceIterator it = begin(); it != end(); it.next()){
	
			String* tmp = new String(it.getCharacter().getChar());

				*nseq += *tmp;

				it=it.next();
			}
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
