/////////////////////////////////////////////

#include<BALL/SEQUENCE/fastaFile.h>

namespace BALL{

	///////////////////////////////////////////// Constructor und Deconstructor //////////////////////////////////////////////////////////////
			
			FASTAFile::FASTAFile()

			{
			}

			
			FASTAFile::FASTAFile(const BALL::String& filename,BALL::File::OpenMode open_mode, bool trim_whitespaces):LineBasedFile::LineBasedFile( filename, open_mode, trim_whitespaces){
			}

		
			/**
			 *Destructor
			 */
			FASTAFile::~FASTAFile(){}


			/////////////////////////////////////////////////// Operators ///////////////////////////////////////////7
			bool FASTAFile::operator == (const FASTAFile& f){
			return LineBasedFile::operator==(f);
			}  
			
			bool FASTAFile::operator != (const FASTAFile& f){
			return LineBasedFile::operator != (f);
			}
			
			
			/**
			const FASTAFile& FASTAFile::operator = (const FASTAFile& f){
				return LineBasedFile::operator=(f);
			}
			*/










			///////////////////////////////////////////////////////////// Reading and Writing /////////////////////////////////////////

			/**
			*reads a FastaFile into a protein
			*/
			Protein& operator>>(FASTAFile file,Protein& protein){
				file.read(protein);
				return protein;
			}
	
			/**
			* reads a Fastafile into a Molecule
			*/
			Molecule& operator>>(FASTAFile file, Molecule& molecule){
				file.read(molecule);
				return molecule;	
				}

			/**
			*reads a FastaFile into a System
			*/
			System& operator>> (FASTAFile file, System& system){
			file.read(system);
				return system;
				}

			 /**
                        *reads a FastaFile into a protein
			@throws InvalidArgument if a file holding more than one sequence is to be read
                        */
			void FASTAFile::read(Protein& protein){
				//counter
				int i=0;
				
				String& current_str_l = (*this).getLine();
				
		
				while((*this).gotoLine(i)) {

					//extract current line of the Sequence
					char* currentl;
							
					current_str_l = (*this).getLine();
	
					Residue* r;

					//check whether it is a headline or a comment
					char c= current_str_l.toChar();


					if ((c =='>') || (c == ';'))
					{
						//check whether currentline is the first line, else there is more than one Sequence in the FastaFile and we can not read it into a protein
						//TODO extraction if the ID
						if(i==0){
							//Substring& ID = current_str_l.getSubstring(2);
							//protein.setID 
							}else{
							throw BALL::Exception::InvalidArgument(__FILE__,__LINE__, "The file holds more than one sequence for a protein. Either split the file or load it into a System");
						}
					}

					else{			

						//convert current_str_l to a C style String	
						current_str_l.set(currentl);

						//iterate through the string to get the single characters
						//TODO check whether set gives a terminating null character!!!!!!!!!!!!
						while(*currentl)
						{	
							String* name = new String(*currentl);
							
							//for every Amino Acid create a new Residue named with the name of the Amino Acid
							r= new Residue(*name);

							//set the Property to Amino Acid
							r->setProperty(Residue::Property::PROPERTY__AMINO_ACID);

							//append Residue to the Protein		
							protein.append(*r); 	

							//got to the next character				
							currentl++;	

						}

					}
					
					//increment the counter
					++i;

				}
		
			}

			/**
                        * reads a Fastafile into a Molecule
                        */
			void FASTAFile::read(Molecule& molecule){
					//counter

				int i=0;

				String& current_str_l =(*this).getLine();
				
				while((*this).gotoLine(i)) {

					//extract current line of the Sequence
					current_str_l = (*this).getLine();

					char* currentl;

					Residue* r;
					
					char c= current_str_l.toChar();

					//check whether it is a headline or a comment

					if (( c =='>') || (c == ';'))
					{ 
					//Check whether there is more than one protein in the file
					if(i!=0) throw BALL::Exception::InvalidArgument(__FILE__,__LINE__, "The file holds more than one sequence for a melcule. Either split the file or load it into a system.");

					}else{			

						//convert current_str_l to a C style String	
						current_str_l.set(currentl);

						//iterate through the string to get the single characters
						//TODO check whether set gives a terminating null character!!!!!!!!!!!!
						while(*currentl)
						{	
							String* name= new String(*currentl);
							//for every Amino Acid create a new Residue named with the name of the Amino Acid
							r= new Residue(*name);

							//append Residue to the molecule		
							molecule.append(*r); 	

							//got to the next character				
							currentl++;	

						}

					}
					
					//increment the counter
					++i;

				}


			}

			/**
                        *reads a FastaFile into a System
                        */
			void FASTAFile::read(System& system){
				//counter
				int i=0;
				
				String& current_str_l=(*this).getLine();

				while((*this).gotoLine(i)) {

					//extract current line of the Sequence
					current_str_l = (*this).getLine();

					char* currentl;

					Residue* r;

					Protein* pp;

					char c = current_str_l.toChar();

					//check whether it is a headline or a comment

					if (( c =='>') || (c == ';'))
					{ 
						pp= new Protein();
						system.append(*pp);

					}else{			

						//convert current_str_l to a C style String	
						current_str_l.set(currentl);

						//iterate through the string to get the single characters
						//TODO check whether set gives a terminating null character!!!!!!!!!!!!
						while(*currentl)
						{	
							//for every Amino Acid create a new Residue named with the name of the Amino Acid
							r= new Residue(*(new String(*currentl)));

							//append Residue to the Protein		
							(*pp).append(*r); 	

							//got to the next character				
							currentl++;	

						}

					}

					//increment the counter
					++i;

				}


			}






			/**
			* writes a Protein into a Fastafile
			*/
			void FASTAFile::write(Protein& protein){

			//retrieve the Sequence of the Protein
			String seq = Peptides::GetSequence(protein);
			
			getFileStream() << "> " << protein.getID() << endl << seq << endl;
			
			}
			

			/**
			*writes a Molecule into a Fastafile
			*/
			void FASTAFile::write(Molecule& molecule)
			{
			
			}
			
			/**
			*writes a System into a Fastafile
			*/
			void FASTAFile::write(System& system){

			//	for (Molecule::MoleculeIterator m_it = S.beginMolecule(); +m_it; ++m_it)
			//	{
			//		if (RTTI::isKindOf<Protein>(*(m_it)))
			//		{

				for(ProteinIterator it = system.beginProtein(); +it; ++it)
					{
						// get the protein's sequence
						
						String seq= Peptides::GetSequence(*it);
						
						getFileStream()<< "> " << it->getID() <<endl << seq << endl;
					}
				}
			

			////////////////////////////////////////////////// Misc ////////////////////////////////////////////////////

			/**
			 *clear method
			 */
			void FASTAFile::clear(){
			LineBasedFile::clear();
			}
			

			

}//namespace BALL
