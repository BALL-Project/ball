/////////////////////////////////////////////

#include<BALL/FORMAT/fastaFile.h>
#include <BALL/KERNEL/residueIterator.h>
namespace BALL{

	///////////////////////////////////////////// Constructor und Deconstructor //////////////////////////////////////////////////////////////
			
			FastaFile::FastaFile()

			{
			}

			
			FastaFile::FastaFile(const BALL::String& filename,BALL::File::OpenMode open_mode, bool trim_whitespaces):LineBasedFile::LineBasedFile( filename, open_mode, trim_whitespaces){
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
			
			
			/**
			const FastaFile& FastaFile::operator = (const FastaFile& f){
				return LineBasedFile::operator=(f);
			}
			*/










			///////////////////////////////////////////////////////////// Reading and Writing /////////////////////////////////////////


		
			 /**
                        *reads a FastaFile into a protein
			@throws InvalidArgument if a file holding more than one sequence is to be read
                        */
			void FastaFile::read(Protein& protein){
				//counter

				int i=0;
				//read the first line
				if (! this->readLine()) throw Exception::InvalidArgument(__FILE__,__LINE__, "could not read the current Line");


				//extract the current line
				String& current_str_l = this->getLine();

				//iterate over all lines of the file

				while(this->gotoLine(i) && this->readLine()) {


					//extract current line of the Sequence
					current_str_l = this->getLine();

					//extract the first character of the current line to check whether it is a comment or a headline

					char c= current_str_l.toChar();

					if ((c =='>') || (c == ';'))
					{
						//check whether currentline is the first line, else there is more than one Sequence in the FastaFile and we can not read it into a protein
						//TODO extraction of the ID
						if(i==0){
							//Substring& ID = current_str_l.getSubstring(2);
							//protein.setID 
						}else{
							throw BALL::Exception::InvalidArgument(__FILE__,__LINE__, "The file holds more than one sequence for a protein. Either split the file or load it into a System");
						}
					}

					else{


						//iterate through the string to get the single characters
						for (unsigned int j=0; j<current_str_l.length(); j++)						
						{	
							String tmp;

							char check = current_str_l[j];

							//check whether the next character is a valid AS and if it is change it to three letter code						
							if(Peptides::IsOneLetterCode(check)) {
								tmp= Peptides::ThreeLetterCode(check);

							}else{ 
								throw Exception::InvalidArgument(__FILE__,__LINE__, "There is no OneLetterCode in the FastaFile");
							}  

							//for every Amino Acid create a new Residue named with the name of the Amino Acid
							Residue* r= new Residue(tmp);

							//set the Property to Amino Acid
							r->setProperty(Residue::Property::PROPERTY__AMINO_ACID);


							//append Residue to the Protein	

							protein.append(*r); 	



						}

					}

					//increment the counter
					++i;

				}
			}
			

			/**
                        * reads a Fastafile into a Molecule
                        */
			void FastaFile::read(Molecule& molecule){

				//counter
				int i=0;

				//read the first line
				if (! this->readLine()) throw Exception::InvalidArgument(__FILE__,__LINE__, "could not read the current Line");

				//extract the first line
				String& current_str_l =this->getLine();

				//iterate over all Lines of the file
				while(this->gotoLine(i) && this->readLine()) {

					//extract current line of the Sequence
					current_str_l = this->getLine();

					//extract first character of current line to check whether the line conatins a comment							
					char c= current_str_l.toChar();

					//check whether it is a headline or a comment

					if (( c =='>') || (c == ';'))
					{ 
						//Check whether there is more than one protein in the file
						if(i!=0) throw BALL::Exception::InvalidArgument(__FILE__,__LINE__, "The file holds more than one sequence for a molecule. Either split the file or load it into a system.");

					}else{			

						//iterate through the string to get the single characters
						for (unsigned int j=1; j<=current_str_l.length(); j++)
						{	

							//for every Amino Acid create a new Residue named with the name of the Amino Acid
							Residue* r= new Residue(current_str_l[j]);

							//append Residue to the molecule		
							molecule.append(*r); 	

						}

					}

					//increment the counter
					++i;

				}


			}


			/**
                        *reads a FastaFile into a System
                        */
			//TODO check whether it is a protein or a nucleic acid
			void FastaFile::read(System& system){


				//counter
				int i=0;

				//read the first line
				if (! this->readLine()) throw Exception::InvalidArgument(__FILE__,__LINE__, "could not read the current Line");

				//extract the first line
				String& current_str_l=this->getLine();

				//iterate over all Lines of the File
				while(this->gotoLine(i) && this->readLine()) {


					//extract current line of the Sequence
					current_str_l = this->getLine();

					//check whether the current line is a > then start a new Protein
					char c = current_str_l.toChar();

					Protein* pp;

					//check whether it is a headline or a comment
					if (( c =='>') || (c == ';'))
					{ 
						pp= new Protein;	
						system.append(*pp);

					}else{			
						//iterate through the string to get the single characters
						for (unsigned int j=0; j<current_str_l.length(); j++)
						{	

							String tmp;

							char check = current_str_l[j];

							//check whether the next character is a valid AS and if it is change it to three letter code						
							if(Peptides::IsOneLetterCode(check)) {
								tmp= Peptides::ThreeLetterCode(check);

							}else{ 
								throw Exception::InvalidArgument(__FILE__,__LINE__, "There is no OneLetterCode in the FastaFile");
							}  

							//for every Amino Acid create a new Residue named with the name of the Amino Acid
							Residue* r= new Residue(tmp);

							//set the Property to Amino Acid
							r->setProperty(Residue::Property::PROPERTY__AMINO_ACID);


							//append Residue to the Protein	
							pp->append(*r); 	

						}

					}

					//increment the counter
					++i;

				}


			}


		


		void FastaFile::read(Alignment& align){
					
				//read the file into a System
				System* s= new System();
				
				read(*s);
		//TODO 				//align.read(*s);

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
