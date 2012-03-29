// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//


/////////////////////////////////////////////

#include<BALL/FORMAT/fastaFile.h>
#include <BALL/KERNEL/residueIterator.h>
#include <BALL/STRUCTURE/peptideBuilder.h>
#include <BALL/STRUCTURE/fragmentDB.h>

namespace BALL
{

	///////////////////////////////////////////// Constructor und Deconstructor //////////////////////////////////////////////////////////////
			
			FastaFile::FastaFile()
			{
			}

			
			FastaFile::FastaFile(const BALL::String& filename,BALL::File::OpenMode open_mode, bool trim_whitespaces)
									: LineBasedFile::LineBasedFile(filename, open_mode, trim_whitespaces){
			}

		
			/**
			 *Destructor
			 */
			FastaFile::~FastaFile(){}


			/////////////////////////////////////////////////// Operators ///////////////////////////////////////////7
			bool FastaFile::operator == (const FastaFile& f){
			return LineBasedFile::operator==(f);
			}  
			
			bool FastaFile::operator != (const FastaFile& f){
			return LineBasedFile::operator != (f);
			}
			
		
			///////////////////////////////////////////////////////////// Reading and Writing /////////////////////////////////////////


		
			 /**
       *reads a FastaFile into a protein
			 *@throws InvalidArgument if a file holding more than one sequence is to be read
       */
			void FastaFile::read(Protein& protein)
			{
							//counter to denote lines
							int i=0;

							//read the first line
							if (!readLine()) throw Exception::InvalidArgument(__FILE__,__LINE__, "could not read the current Line");

							//extract the current line
							String& current_str_l = getLine();

							//String to store the Sequence
							String sequence;

							//String to store the name of the protein
							String name;

							//iterate over all lines of the file
							while(gotoLine(i) && readLine()) 
							{
											//extract current line of the Sequence
											current_str_l = this->getLine();

											//extract the first character of the current line to check whether it is a comment or a headline
											char c= current_str_l.toChar();

											if ((c =='>') || (c == ';'))
											{
															//check whether currentline is the first line, else there is more than one Sequence in the FastaFile and we can not read it into a protein

															if(i==0)
															{
																			//retrieve the name of the protein. It starts right after '>' and ends before the first whitespace
																			unsigned int counter =  0;
																			while(counter < current_str_l.length() )
																			{
																							if(current_str_l(counter) == ' ' || current_str_l(counter) == '\t' )
																							{ 
																											break;
																							}
																							else
																							{
																											name += current_str_l(counter);
																											counter++;
																							}
																			}
															}
															else
															{
																			throw BALL::Exception::InvalidArgument(__FILE__,__LINE__, "The file holds more than one sequence for a protein. Either split the file or load it into a System");
															}
											}

											else
											{

															//iterate through the string to get the single characters
															for (unsigned int j=0; j<current_str_l.length(); j++)						
															{	

																			char check = current_str_l[j];

																			//check whether the next character is a valid AS and if it is change it to three letter code						
																			if(Peptides::IsOneLetterCode(check)) 
																			{
																							sequence += check/*Peptides::ThreeLetterCode(check)*/;

																			}
																			else
																			{ 
																							throw Exception::InvalidArgument(__FILE__,__LINE__, "There is no OneLetterCode in the FastaFile");
																			}  

															}

											}

											//increment the counter
											++i;

							}

							//Build Peptide with Peptidebuilder

							// we need to define the peptid's AA sequence as a vector of descriptors
							vector<BALL::Peptides::AminoAcidDescriptor> descriptor_seq;

							for(unsigned int i=0; i< sequence.length(); i++)
							{
											// each aminoacid is represented by a descriptor
											BALL::Peptides::AminoAcidDescriptor* aad = new BALL::Peptides::AminoAcidDescriptor;


											// define a glycin
											aad->setAminoAcidType(Peptides::ThreeLetterCode(sequence.c_str() [i]));
											//aad->setPhi(Angle(-47., false));
											//aad->setPsi(Angle(-77., false));

											// add it to the sequence
											descriptor_seq.push_back(*aad);
							}

							// create a builder using the sequence
							BALL::Peptides::PeptideBuilder* pb = new BALL::Peptides::PeptideBuilder(descriptor_seq);

							// "link" the fragment db for adding missing information
							FragmentDB fdb("");
							pb->setFragmentDB(&fdb);

							// now build the peptide 
							Protein* prot = pb->construct();

							// give sensible names
							pb->setChainName("Chain A");
							pb->setProteinName(name);

							protein = *prot;	

			}
			

			

			/**
       *reads a FastaFile into a System
       */
			//TODO check whether it is a protein or a nucleic acid
			void FastaFile::read(System& system)
			{
							//counter to denote the lines
							int i=0;

							//counter to denote the protein
							int p_ct=0;

							//read the first line
							if (! readLine()) 
							{
											throw Exception::InvalidArgument(__FILE__,__LINE__, "could not read the current Line");
							}

							//extract the first line
							String& current_str_l= getLine();


							//vector to store the Sequence
							vector<String> sequences;

							//String to store the name of the protein
							vector<String> names;


							//String to store the sequence
							String sequence;

							//iterate over all Lines of the File
							while(gotoLine(i) && readLine()) 
							{
											//extract current line of the Sequence
											current_str_l = this->getLine();

											//check whether the current line is a > then start a new Protein
											char c = current_str_l.toChar();

											//check whether it is a headline or a comment
											if (( c =='>') || (c == ';'))
											{
															//retrieve the name of the protein. It starts right after '>' and ends before the first whitespace
															unsigned int counter =  0;
															String name;
															while(counter < current_str_l.length() )
															{
																			if(current_str_l(counter) == ' ' || current_str_l(counter) == '\t' )
																			{ 
																							break;
																			}
																			else
																			{
																							name += current_str_l(counter);
																							counter++;
																			}
															}
															//insert name
															names.push_back(name);

															//add last sequence to sequences vector if it is not the very first sequence
															if(p_ct > 0)
															{
																			sequences.push_back(sequence);
															}

															//increment counter
															p_ct++;

															//reset Sequence
															sequence="";

											}
											else
											{			
															String tmp;

															//iterate through the string to get the single characters
															for (unsigned int j=0; j<current_str_l.length(); j++)
															{	

																			char check = current_str_l[j];

																			//check whether the next character is a valid AS and if it is change it to three letter code						
																			if(Peptides::IsOneLetterCode(check)) 
																			{
																							tmp += check;
																			}
																			else
																			{ 
																							throw Exception::InvalidArgument(__FILE__,__LINE__, "No OneLetterCode in the FastaFile");
																			}  

															}
								
												sequence +=tmp;

											}

											//increment the counter
											++i;

							}


							//add the lastly retrieved sequence
							sequences.push_back(sequence);

							//************ Build Peptide with Peptidebuilder *********************************

							if(sequences.size() != names.size())
							{
											throw Exception::InvalidArgument(__FILE__,__LINE__, "Not the same number of names and sequences in the FastaFile!");
							}
							//iterate over all proteins of the fastafile
							for (unsigned int i=0; i<sequences.size(); i++)
							{
											// we need to define the peptid's AA sequence as a vector of descriptors
											vector<BALL::Peptides::AminoAcidDescriptor> descriptor_seq;

											for(unsigned int j=0; j<sequences.at(i).length(); j++)
											{
															// each aminoacid is represented by a descriptor
															BALL::Peptides::AminoAcidDescriptor* aad = new BALL::Peptides::AminoAcidDescriptor;


															// define a glycin
															aad->setAminoAcidType(Peptides::ThreeLetterCode(sequences.at(i).c_str() [j]));
															//aad->setPhi(Angle(-47., false));
															//aad->setPsi(Angle(-77., false));

															// add it to the sequence
															descriptor_seq.push_back(*aad);
											}
											// create a builder using the sequence
											BALL::Peptides::PeptideBuilder* pb = new BALL::Peptides::PeptideBuilder(descriptor_seq);

											// "link" the fragment db for adding missing information
											FragmentDB fdb("");
											pb->setFragmentDB(&fdb);

											// now build the peptide 
											Protein* prot = pb->construct();

											// give sensible names
											pb->setChainName("Chain A");
											pb->setProteinName(names.at(i));

											system.append(*prot);	
							}


			}


		


	










			/**
			* writes a Protein into a Fastafile
			*/
		void FastaFile::write(const Protein& protein){

						ResidueConstIterator resit = protein.beginResidue();
										String seq;
						for (; +resit ; ++resit)
						{
										if (resit->isAminoAcid())
										{
														seq += Peptides::OneLetterCode(resit->getName());
										} 
						}

						getFileStream() << "> " << protein.getName() << endl << seq << endl;

		}
			

			/**
			*writes a Molecule into a Fastafile
			*/
			//void FastaFile::write(Molecule& molecule)
			//{
			
			//}
			
			/**
			*writes a System into a Fastafile
			*/
			void FastaFile::write(const System& system){

			//	for (Molecule::MoleculeIterator m_it = S.beginMolecule(); +m_it; ++m_it)
			//	{
			//		if (RTTI::isKindOf<Protein>(*(m_it)))
			//		{

				for(ProteinConstIterator it = system.beginProtein(); +it; ++it)
					{
						// get the protein's sequence
						
						String seq= Peptides::GetSequence(*it);
						
						getFileStream()<< "> " << it->getName() <<endl << seq << endl;
					}
				}
			

			////////////////////////////////////////////////// Misc ////////////////////////////////////////////////////

			/**
			 *clear method
			 */
			void FastaFile::clear(){
			LineBasedFile::clear();
			}
			

			
#	ifdef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/FORMAT/fastaFile.iC>
#	endif


}//namespace BALL
