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
								//	:name_(""),origin_(0)
				{ 
								name_= "";
								origin_ = 0;
				}
				/**
				 *Detailed Constructor
				 *@param name_ the name_ of the Sequence
				 *@param origin_ the composite from which the sequence originates
				 */
				Sequence::Sequence(String& name, AtomContainer* origin)
								:name_(name),
								origin_(origin)
				{  }

				/**
				 *Copy Constructor
				 */
				Sequence::Sequence(Sequence& seq)
				{
								name_ = seq.getName();
								origin_ =  seq.getOrigin();
				}

				/**
				 *Deconstrcutor
				 **/
				Sequence::~Sequence()
				{		}




				/**
				 * @param name_ the new name_ to be set
				 */
				void Sequence::setName(const BALL::String& name)
				{
								name_= name;
				}

				/**
				 *@return the name_ of the sequence
				 */
				String& Sequence::getName()
				{
								return name_;
				}


				/**
				 *@param origin_ the origin which is to be setted
				 */
				void Sequence::setOrigin(AtomContainer* origin)
				{
								origin_=origin;
				}

				/**
				 *@return the origin_ of the Sequence
				 */
				AtomContainer* Sequence::getOrigin()
				{
								return origin_;
				}	

				/**
				 *operator==
				 */
				bool Sequence::operator== (Sequence& seq)
				{
								return ((name_==seq.getName()) && (origin_==seq.getOrigin()));
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
								name_ = seq.getName();
								origin_ = seq.getOrigin();

								return *this;
				}



				/**
				 *@return a SequenceIterator which points to the first character of the Sequence
				 */
				SequenceIterator Sequence::begin()
				{
								//Initialize the new Iterator
								SequenceIterator it;
								it.setSequence(*this);

								if (RTTI::isKindOf<Protein>(*origin_)) 
								{
												//create new Sequencecharacter
												SequenceCharacter c;
												c.setOrigin(this);
												Protein* p= RTTI::castTo<Protein>(*origin_);

												if (p)
												{	
																c.setChar(Peptides::OneLetterCode(p->getResidue(0)->getName()));

																it.setChar(c);

															return it;
												}
								}

								throw BALL::Exception::InvalidArgument(__FILE__,__LINE__,"origin_ is no Protein");
								//			cout<<"leaving SequenceBegin() now"<< endl;

				}				

				/**
				 *@return a SequenceIterator which points to the end character of the Sequence
				 */
				SequenceIterator Sequence::end(){

								//initialize the new Iterator
								SequenceIterator it;

								//set the the sequence
								it.setSequence(*this);


								if (RTTI::isKindOf<Protein>(*origin_)) {

												//cast origin_ to a protein
												Protein* protein = RTTI::castTo<Protein>(*origin_);

												int num_of_res = protein->countResidues();

												//set the counter to the number of residues in the protein
												it.setCounter(num_of_res);

												//to ensure comparing with another one  gives right decision we have to initiali
												SequenceCharacter c;
												c.setOrigin(this);
												it.setChar(c);

								}
								//cout<<"returning now"<<endl;

								return it;
				}





				/**
				 *@return String which contains the Sequence
				 **/
				String Sequence::getStringSequence(){

//TODO when implemented all Iterator functionalities change!!!

								String* nseq = new String();

								SequenceIterator it,e;
								e = end();

								for(it = begin(); it != e; it.next())
								{

												String* tmp = new String(it.getCharacter().getChar());

												*nseq += *tmp;

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
												seq.conservativeResize(seq.rows()+1);
												//retrieve the current Sequencecharacter for each Step
												SequenceCharacter c = it.getCharacter();
												//store it in the Array
												seq(seq.rows()-1,0) = c;


								}

								return seq;
				}

}//name_space BALL
