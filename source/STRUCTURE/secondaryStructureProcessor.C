// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: secondaryStructureProcessor.C,v 1.6 2004/03/19 15:02:53 anne Exp $
//

#include <BALL/STRUCTURE/secondaryStructureProcessor.h>
#include <BALL/STRUCTURE/HBondProcessor.h>

#include <BALL/KERNEL/secondaryStructure.h>           
#include <BALL/KERNEL/protein.h>
#include <BALL/KERNEL/chain.h>
#include <BALL/KERNEL/bond.h>
#include <math.h>

using namespace std;

namespace BALL
{
	SecondaryStructureProcessor::SecondaryStructureProcessor()
	{
	}

	 /***************************************
	 * find the Secondary Structures
	 * **************************************/
	void SecondaryStructureProcessor::compute()
	{
		 /*********************************
		 *   initialize the summary strings			
		 **********************************/

		// note that HBonds_.size() is not the number of HBonds but
		// rather the number of residues in the system.
	  Size size = HBonds_.size();
	
		sheet.resize(size);
		for (Size i=0; i<size; i++)
		{
			sheet[i]='-';
		}
		bridge1   = sheet;
		bridge2  	= sheet;
		Fiveturn  = sheet;
		Fourturn  = sheet;
		Threeturn = sheet;
		summary   = sheet;

	  /************************************************
	  *  in the first place we search and insert turns
		*************************************************/

		//over all residues

		for(Size i=0; i<size; i++)
		{
			/*
			for (Size j=0; j<HBonds_[i].size(); j++)
				cout << "Schleife nr " << i << " HBP " << HBonds_[i][j] << " ";
			cout << std::endl;
			*/

			if(HBonds_[i].size() != 0 )
			{
				// over all HBondpartners
				for(Size k=0; k<HBonds_[i].size(); k++)
				{	
					//---------- 4 turns
					if (HBonds_[i][k]==(int)(i+4))
					{
						//first sign
						if( (Fourturn[i]  == '<') || (Fourturn[i] == 'X'))
						{
							Fourturn[i]   = 'X';
						}else
						{
							Fourturn[i]   = '>';
						}
						//position 2,3,4
						for (int j=1; j<4; j++)
						{
							if(Fourturn[i+j]=='-')
							{
								Fourturn[i+j]='4';
							}
						}

						//last position has always to be <
						Fourturn[i+4] = '<';
					}

					//---------- 3 turns
					else if (HBonds_[i][k]==(int)(i+3))
					{
						//first sign
						if( (Threeturn[i]  == '<') || (Threeturn[i] == 'X'))
						{
							Threeturn[i]   = 'X';
						}else
						{
							Threeturn[i]   = '>';
						}
						//position 2,3
						for (int j=1; j<3; j++)
						{
							if(Threeturn[i+j]=='-')
							{
								Threeturn[i+j]='3';
							}
						}

						//last position has always to be <
						Threeturn[i+3] = '<';
					} 

					//---------- 5 turns
					else if (HBonds_[i][k]==(int)(i+5))
					{
						//first sign
						if( (Fiveturn[i]  == '<') || (Fiveturn[i] == 'X'))
						{
							Fiveturn[i]   = 'X';
						}else
						{
							Fiveturn[i]   = '>';
						}
						//position 2,3
						for (int j=1; j<5; j++)
						{
							if(Fiveturn[i+j]=='-')
							{
								Fiveturn[i+j]='5';
							}
						}

						//last position has always to be <
						Fiveturn[i+5] = '<';

					} //else if turns

				}//	for(int k=0; k<HBonds_[i].size(); k++)

			}//	if(HBonds_[i].size() != 0 )

		}//for all residues



		//	/****************************************
		//	 * secondly  we search bridges
		//	 ****************************************/


		char parallel = 'a'-1;
		char antiparallel = 'A'-1;
		enum pattern { PARALLEL, ANTIPARALLEL, DEFAULT } lastpattern;

		//over all residues
		lastpattern = DEFAULT;
		bool found_a_pattern = false;

		for(Size i=0; i<size; i++)
		{		
			found_a_pattern = false;

			if(HBonds_[i].size() != 0 )
			{
				// over all HBondpartners
				for(Size n=0; n<HBonds_[i].size(); n++)
				{		
					int k = HBonds_[i][n];

					// actually we have HBP(i,k)
					// do we have HBP(k, i+2) ? => parallel bridge(i+1,k) 			

					for (Size s = 0; (s < HBonds_[k].size()); s++)
					{
						if(HBonds_[k][s]== (int)(i+2)) 
						{
							if (lastpattern != PARALLEL)
							{
								parallel+=1;
								if(parallel == 'z'+1) parallel='a';
							}

							//insert
							if(bridge1[i+1] == '-')
							{
								bridge1[i+1] = parallel;
							}
							else
							{
								bridge2[i+1] = parallel;
							}
							if(bridge1[k] == '-')
							{
								bridge1[k] = parallel;
							}
							else
							{
								bridge2[k] = parallel;
							}

							lastpattern     = PARALLEL;
							found_a_pattern = true;

							break;
						}
					}	 //forend parallelbridge


					// currently we have HBP(i,k)
					// do we have HBP(k, i) ? => antiparallel bridge(i,k)
					for (Size s = 0; (s < HBonds_[k].size()); s++)
					{
						if(HBonds_[k][s]== (int)i) 
						{
		    		  // we aren't allowed to overwrite antiparallel bridges found before!
							// remember: we have two equal cases: 
							// 							first : HBP(k,i) && HBP(i,k)  
							// 					and second: HBP(i,k) && HBP(k,i)  						
							if(   ((bridge1[i]==bridge1[k])&&(bridge1[i]!='-')) 
									||((bridge1[i]==bridge2[k])&&(bridge1[i]!='-')) 
									||((bridge2[i]==bridge1[k])&&(bridge2[i]!='-'))
									||((bridge2[i]==bridge2[k])&&(bridge2[i]!='-'))
									||(k==(int)i)
								)
							{ 
							}
							else
							{
								//insert

								if (lastpattern != ANTIPARALLEL)
								{
									antiparallel+=1;
									if(antiparallel == 'Z'+1) antiparallel='A';
								}

								if(bridge1[i] == '-')
								{
									bridge1[i] = antiparallel;
								}
								else
								{
									bridge2[i] = antiparallel;
								}
								if(bridge1[k] == '-')
								{
									bridge1[k] = antiparallel;
								}
								else
								{
									bridge2[k] = antiparallel;
								}

								lastpattern     = ANTIPARALLEL;
								found_a_pattern = true;

								break;
							}
						}
					}//forend antiparallelbridge1

					// currently we have HBP(i,k)
					// do we have HBP(k-2, i+2) ? => antiparallel bridge(i+1,k-1)
					if(((k-2)>=0) && ((i+2) <=size))
					{
						for (Size s = 0;(s < HBonds_[k-2].size()); s++)
						{

							if (    (HBonds_[k-2][s] == (int)(i+2))
									&& ((int)(i+1) != (k-1)))
							{

								// make sure that we have not already seen this bridge
								if (  ((bridge1[i+1]==bridge1[k-1]) && (bridge1[i+1]!='-'))
										||((bridge1[i+1]==bridge2[k-1]) && (bridge1[i+1]!='-'))
										||((bridge2[i+1]==bridge1[k-1]) && (bridge2[i+1]!='-'))
										||((bridge2[i+1]==bridge2[k-1]) && (bridge2[i+1]!='-')) )
								{
									continue;
								}

								//insert
								if (lastpattern != ANTIPARALLEL)
								{
									antiparallel+=1;
									if(antiparallel == 'Z'+1) antiparallel='A';
								}

								if(bridge1[i+1]=='-')
								{
									bridge1[i+1] = antiparallel;
								}
								else
								{
									bridge2[i+1]= antiparallel;
								}
								if(bridge1[k-1]=='-')
								{
									bridge1[k-1] = antiparallel;
								}
								else
								{
									bridge2[k-1]= antiparallel;
								}

								lastpattern     = ANTIPARALLEL;
								found_a_pattern = true;

								break;
							}
						}
					}	// if (k==...)

					if (!found_a_pattern) lastpattern = DEFAULT;
				}//	for(int k=0; k<HBonds_[i].size(); k++)

			}//	if(HBonds_[i].size() != 0 )

		}//for all residues

		String s1(&(summary[0]), 0, summary.size());
		String s2(&(Threeturn[0]), 0, Threeturn.size());
		String s3(&(Fourturn[0]), 0, Fourturn.size());
		String s4(&(Fiveturn[0]), 0, Fiveturn.size());
		String s5(&(bridge1[0]), 0, bridge1.size());
		String s6(&(bridge2[0]), 0, bridge2.size());
		
/**		std::cout << "-----------turns and Bridges----------------------------------------" << std::endl;
		std::cout << "summary   "<< s1 << std::endl;
		std::cout << "Threeturn " << s2 << std::endl;
		std::cout << "Fourturn  "<< s3 << std::endl;
		std::cout << "Fiveturn  " << s4 << std::endl;
		std::cout << "Bridge1   " << s5 << std::endl;
		std::cout << "Bridge2   " << s6 << std::endl;
*/	
		
		
		// /***********************************
		//  * now we repair the irregularities
		// 	***********************************/
		//  two overlapping minimal helices(consecutive n-turns)
		//  offset by tow or three residues are joined into one 
		//  helix
		//  therefor we just insert the missing n-turns
 	 
		// we start with the 3-turn
		for(Size i= 0; i<size; i++)
		{
			// we have to proof the length : offset by two or three 
			// residues and the lenght of the turn itself 
			// offset two residues 			
			int length = 5;
			int turn = 3;
			if((   (i+2+length) < size)  
				  && (   hasPrefix(">>", i, Threeturn)
				 			|| hasPrefix("XX", i, Threeturn)
				 			|| hasPrefix(">X", i, Threeturn)
				 			|| hasPrefix("X>", i, Threeturn) 
						 )
					&& (   hasPrefix(">>", i+3, Threeturn)
				 			|| hasPrefix("XX", i+3, Threeturn)
				 			|| hasPrefix(">X", i+3, Threeturn)
				 			|| hasPrefix("X>", i+3, Threeturn) 
						 )
				)
			{
				insert_turn(turn, i+2);
			 //	insert_turn(turn, i+3);	
			}
			
			// offset three turns	
			if((   (i+3+length) < size)  
				  && (   hasPrefix(">>", i, Threeturn)
				 			|| hasPrefix("XX", i, Threeturn)
				 			|| hasPrefix(">X", i, Threeturn)
				 			|| hasPrefix("X>", i, Threeturn) 
						 )
					&& (   hasPrefix(">>", i+4, Threeturn)
				 			|| hasPrefix("XX", i+4, Threeturn)
				 			|| hasPrefix(">X", i+4, Threeturn)
				 			|| hasPrefix("X>", i+4, Threeturn) 
						 )
				)
			{
				insert_turn(turn, i+2);
			 	insert_turn(turn, i+3);
		//		insert_turn(turn, i+4);	
			}			
		}
	
		
		// -------------- 4-turn -----------------------
		for(Size i= 0; i<size; i++)
		{
			// we have to proof the length : offset by two or three 
			// residues and the lenght of the turn itself (here 6) 
			// offset two residues
			int length = 6;
			int turn = 4;			
			if((   (i+2+length) < size)  
				  && (   hasPrefix(">>", i, Fourturn)
				 			|| hasPrefix("XX", i, Fourturn)
				 			|| hasPrefix(">X", i, Fourturn)
				 			|| hasPrefix("X>", i, Fourturn) 
						 )
					&& (   hasPrefix(">>", i+3, Fourturn)
				 			|| hasPrefix("XX", i+3, Fourturn)
				 			|| hasPrefix(">X", i+3, Fourturn)
				 			|| hasPrefix("X>", i+3, Fourturn) 
						 )
				)
			{
				insert_turn(turn, i+2);
			 	insert_turn(turn, i+3);	
			}		
			// offset three turns	
			if((   (i+3+length) < size)  
				  && (   hasPrefix(">>", i, Fourturn)
				 			|| hasPrefix("XX", i, Fourturn)
				 			|| hasPrefix(">X", i, Fourturn)
				 			|| hasPrefix("X>", i, Fourturn) 
						 )
					&& (   hasPrefix(">>", i+4, Fourturn)
				 			|| hasPrefix("XX", i+4, Fourturn)
				 			|| hasPrefix(">X", i+4, Fourturn)
				 			|| hasPrefix("X>", i+4, Fourturn) 
						 )
				)
			{
				insert_turn(turn, i+2);
			 	insert_turn(turn, i+3);
				insert_turn(turn, i+4);	
			}			
		}
		
		
		// -------------- 5-turn -----------------------
		
		for(Size i= 0; i<size; i++)
		{
			// we have to proof the length : offset by two or three 
			// residues and the lenght of the turn itself (here 7) 
			// offset two residues
			int length = 7;
			int turn = 5;			
			if((   (i+2+length) < size)  
				  && (   hasPrefix(">>", i, Fiveturn)
				 			|| hasPrefix("XX", i, Fiveturn)
				 			|| hasPrefix(">X", i, Fiveturn)
				 			|| hasPrefix("X>", i, Fiveturn) 
						 )
					&& (   hasPrefix(">>", i+3, Fiveturn)
				 			|| hasPrefix("XX", i+3, Fiveturn)
				 			|| hasPrefix(">X", i+3, Fiveturn)
				 			|| hasPrefix("X>", i+3, Fiveturn) 
						 )
				)
			{
				insert_turn(turn, i+2);
			 	insert_turn(turn, i+3);	
			}		
			// offset three turns	
			if((   (i+3+length) < size)  
				  && (   hasPrefix(">>", i, Fiveturn)
				 			|| hasPrefix("XX", i, Fiveturn)
				 			|| hasPrefix(">X", i, Fiveturn)
				 			|| hasPrefix("X>", i, Fiveturn) 
						 )
					&& (   hasPrefix(">>", i+4, Fiveturn)
				 			|| hasPrefix("XX", i+4, Fiveturn)
				 			|| hasPrefix(">X", i+4, Fiveturn)
				 			|| hasPrefix("X>", i+4, Fiveturn) 
						 )
				)
			{
				insert_turn(turn, i+2);
			 	insert_turn(turn, i+3);
				insert_turn(turn, i+4);	
			}			
		}	
   
    s1.set(&(summary[0]), 0, summary.size());
		s2.set(&(Threeturn[0]), 0, Threeturn.size());
		s3.set(&(Fourturn[0]), 0, Fourturn.size());
		s4.set(&(Fiveturn[0]), 0, Fiveturn.size());
		s5.set(&(bridge1[0]), 0, bridge1.size());
	  s6.set(&(bridge2[0]), 0, bridge2.size());
		
/**		std::cout << "---------------------repaired turns----------------------------------" << std::endl;
		std::cout << "summary   "<< s1 << std::endl;
		std::cout << "Threeturn " << s2 << std::endl;
		std::cout << "Fourturn  "<< s3 << std::endl;
		std::cout << "Fiveturn  " << s4 << std::endl;
		std::cout << "Bridge1   " << s5 << std::endl;
		std::cout << "Bridge2   " << s6 << std::endl;
*/

		// /*****************************************************
		// * -------- now we construct the summary string ----------
		// * structural overlaps are eleminated by considering
  	// * hierarchy H > B > E > G > I > T
  	// * 		H means 4 Helices, B means single bridges, 
  	// * 		E means extended ladder residues, G means 3 Helices
  	// *		I means 5 Helices and T means single 3-, 4-, or 5-turns 
		// * we start with writing 5 Helices and overwrite graduately 
 		// * the summary string with 3 Helices, extended bridges, 
		// * single bridges and 4 Helices helices	
    // -------------------------------------------------------
		// ****************************************************/


		// --------------------- 5 helices ---------------------- 
		for(Size i= 0; i<size; i++)
		{
			// we initialize the summary string with '-'			
			if(Fiveturn[i] == '-') 
			{
				summary[i]= '-';
			}
			else if(   (hasPrefix(">>", i, Fiveturn))		 
	    	      || (hasPrefix(">X", i, Fiveturn))
				      || (hasPrefix("X>", i, Fiveturn))
				      || (hasPrefix("XX", i, Fiveturn))
			     	 )  
							
			{
				if( (i+5) < size)
				{
					summary[i+1]= 'I';
					summary[i+2]= 'I';
					summary[i+3]= 'I';
					summary[i+4]= 'I';
					summary[i+5]= 'I';
				}	
			}// do we have a helix reduced to less than minimal size?
			else if(   ( ! hasPrefix("II", i, summary)) 
							&& (  (hasPrefix(">5", i, Fiveturn)) 
							)  ) 
			{
				if( (i+5) < size)
				{				
					summary[i+1]= 'T';
					summary[i+2]= 'T';
					summary[i+3]= 'T';
					summary[i+4]= 'T';
					summary[i+5]= 'T';
				}	
			}	
		}	
		

		// -------------------3 helices ------------------------
		for(Size i= 0; i<size; i++)
		{
			if(   (hasPrefix(">>", i, Threeturn))		 
	    	 || (hasPrefix(">X", i, Threeturn))
				 || (hasPrefix("X>", i, Threeturn))
				 || (hasPrefix("XX", i, Threeturn))
				)  
							
			{
				if( (i+3) < size)
				{
					summary[i+1]= 'G';
					summary[i+2]= 'G';
					summary[i+3]= 'G';
				}	
			}// do we have a helix reduced to less than minimal size?
			 // we have to consider, that we do not overwrite 
			else if(hasPrefix(">3", i, Threeturn))  
			{
				if( (i+3) < size)
				{
					for(Size j=1; j<3;j++)
					{ 
						if(  (summary[i+j]!= 'G') 
							 &&(summary[i+j]!= 'I') 
							)
						{	
							summary[i+j]= 'T';	
						}
					}
				}	
			}	
		}	


		// ---------------- Extended Bridges and Single Bridges --------------
		// according to the paper:
		// 		single bridges are ladders of length one -> B, 
		// 		all other ladder residues -> E
		// we assume that there is a mistake in the paper: E has a higher priority than B
		// first we generate the sheet-line and than summarize it in the summary line
		for(Size i= 0; i<size; i++)
		{
			if(bridge1[i]!='-')
			{
				int j=0;			
				for(j=0; ((i+j)<size) && (bridge1[i+j]!='-'); j++)
				{
				}
				if((j==1)&&(summary[i]!='E')) //single bridge
				{
					summary[i]='B';
				}else //extended bridge
				{
					for(int n=0; n<j;n++)
					{
						summary[i]='E';
					}
				}							
				i=i+j;				
			}				
    }
		for(Size i= 0; i<size; i++)
		{
			if(bridge2[i]!='-')
			{
				int j=0;			
				for(j=0; ((i+j)<size) && (bridge2[i+j]!='-'); j++)
				{
				}
				if((j==1)&&(summary[i]!='E')) //single bridge
				{
					summary[i]='B';
				}else //extended bridge
				{
					for(int n=0; n<j;n++)
					{
						summary[i]='E';
					}
				}							
				i=i+j;				
			}				
    }
		
						
		// ---------------- 4 helices ---------------------
		for(Size i= 0; i<size; i++)
		{
			if(   (hasPrefix(">>", i, Fourturn))		 
	    	 || (hasPrefix(">X", i, Fourturn))
				 || (hasPrefix("X>", i, Fourturn))
				 || (hasPrefix("XX", i, Fourturn))
				)  
							
			{
				if( (i+4) < size)
				{
					summary[i+1]= 'H';
					summary[i+2]= 'H';
					summary[i+3]= 'H';
				  summary[i+4]= 'H';
				}	
			}// do we have a helix reduced to less than minimal size?
			 // we have to consider, that we do not overwrite 
			else if(hasPrefix(">4", i, Fourturn))  
			{
							
				if( (i+4) < size)
				{
					for(Size j=1; j<4;j++)
					{ 
						if(  ( summary[i+j]!= 'G')
							 &&(!summary[i+j]!= 'H')
							 &&(!summary[i+j]!= 'I')
							 &&(!summary[i+j]!= 'E')
							 &&(!summary[i+j]!= 'B')
							)
						{	
							summary[i+j]= 'T';	
						}
					}
				}	
			}	
		}	
	  s1.set(&(summary[0]), 0, summary.size());
		s2.set(&(Threeturn[0]), 0, Threeturn.size());
		s3.set(&(Fourturn[0]), 0, Fourturn.size());
		s4.set(&(Fiveturn[0]), 0, Fiveturn.size());
		s5.set(&(bridge1[0]), 0, bridge1.size());
		s6.set(&(bridge2[0]), 0, bridge2.size());
		
/**		std::cout << "----------------------------------summary string-----" << std::endl;
		std::cout << "summary   "<< s1 << std::endl;
		std::cout << "Threeturn " << s2 << std::endl;
		std::cout << "Fourturn  "<< s3 << std::endl;
		std::cout << "Fiveturn  " << s4 << std::endl;
		std::cout << "Bridge1   " << s5 << std::endl;
		std::cout << "Bridge2   " << s6 << std::endl;
*/
		
	}
	
	void SecondaryStructureProcessor::insert_turn(int turn, int position)
	{
		bool correct = true;			
		vector<char>* n_turn=0;			
		if(turn == 3)
		{ 
			n_turn = &Threeturn;
		}else if(turn == 4)
		{ 
			n_turn = &Fourturn;
		}else if(turn == 5)
		{
			n_turn = &Fiveturn;
		}else 
		{
			correct = false;//TODO: Gute Fehlermeldung!! return;
		}				
		
		if(correct)
		{				
			//first sign
			if( ((*n_turn)[position]  == '<') || ((*n_turn)[position] == 'X'))
			{
				(*n_turn)[position]   = 'X';
			}else
			{
				(*n_turn)[position]   = '>';
			}
			//position in between
			for (int j=1; j<turn; j++)
			{
				if((*n_turn)[position+j]=='-')
				{
					String s(turn);
					(*n_turn)[position+j]= s[0];
				}
			}
			//last position has always to be <
			(*n_turn)[position+turn] = '<';
		}			
	}
	
	bool SecondaryStructureProcessor::hasPrefix(const String& pattern, Size i, const vector<char>& target)
	{
		bool ret = false;			
		String temp(&(target[0]), i, target.size()-i-1);
		ret = temp.hasPrefix(pattern);
		return ret;
	}
				
/**********************************************
 *   determine the new Secondary Structure and
 *   replace the old one with the new one
 ************************************************/
	Processor::Result SecondaryStructureProcessor::operator() (Composite &composite)
	{
		if (!RTTI::isKindOf<Chain>(composite))
		{
			return Processor::CONTINUE;
		}

		Chain* p = RTTI::castTo<Chain>(composite);
		HBondProcessor hbp;
		p->apply(hbp);       // find all posible HBonds
		HBonds_ = hbp.getHBondPairs();
		ResidueIterator ri = p->beginResidue();
		 
		if (!(+ri))
		{
			return Processor::CONTINUE;
		}
		
		// locate the Secondary Structures
		compute();

		//----------associate new Secondary Structures (SS) for each residue -----
	  // - summarize equal residues in one new SS (new_ss)
		// - push for each residue the new SS into new_parent  
		// - push all residues into residues
		
		SecondaryStructure* ss = 0;
		char last_struct = 'X';
		Position resnum=0;
 
		vector<SecondaryStructure*> new_ss;
		vector<SecondaryStructure*> new_parent;
		vector<Residue*> 						residues;
		for (;+ri;++ri)
		{
		 // !!!! attention: resnum is the real "index"
			if (summary[resnum] != last_struct)
			{
				if (last_struct != 'L' || (summary[resnum] != 'G' && summary[resnum] != '-'))
				{
					ss = new SecondaryStructure;
					new_ss.push_back(ss);
				}
			}

			// first determine the type of this residue
			if (summary[resnum] == 'H') 			// Alpha - HELIX
			{
				// TODO: what about other helices???
				ss->setType(SecondaryStructure::HELIX);
				last_struct = 'H';
			}
			else if (summary[resnum] == 'E') 	// Beta - STRAND
			{
				ss->setType(SecondaryStructure::STRAND);
				last_struct = 'E';
			}
			else 															// LOOP
			{
				ss->setType(SecondaryStructure::COIL);
				last_struct = 'L';
			}

			new_parent.push_back(ss);
			residues.push_back(&*ri);
			
			resnum++;
		}
		
		// ------ insert Residues to new SS ---------------
		for (Position i = 0; i < residues.size(); i++)
		{
			new_parent[i]->insert(*residues[i]);
		}
	
		// ------ remove old SecondaryStructures ----------
		vector<SecondaryStructure*> to_remove;
		SecondaryStructureIterator ssit = p->beginSecondaryStructure();
		for (; +ssit; ++ssit)
		{
			to_remove.push_back(&*ssit);
		}

		for (Position i = 0; i < to_remove.size(); i++)
		{
			delete to_remove[i];
		}

		BALL_POSTCONDITION_EXCEPTION(p->countSecondaryStructures() == 0, 
		 "SecondaryStructureProcessor did not remove all old secondary structures!")
		
		// ------ insert new SecondaryStructures ----------
		for (Position i = 0; i < new_ss.size(); i++)
		{
			p->insert(*new_ss[i]);
		}
	
		BALL_POSTCONDITION_EXCEPTION(p->countSecondaryStructures() == new_ss.size(),
		 "SecondaryStructureProcessor did not add all new secondary structures!")

		return Processor::CONTINUE;
	}

} // namespace BALL


/************************************
   komplizierte, aber laufzeitfreundlichere Version evtl. nicht korrekt, 
	 da sich Konstellationen nicht reproduzieren lassen! 

 	// first we edit the 5 helices 
		for(Size i= 0; i<size;i++)
		{
			// we initialize the summary string with '-'			
			if(Fiveturn[i] == '-') 
			{
				summary[i]= '-';
			}
			if(   (hasPrefix("->>", i, Fiveturn)) 
	    	 || (hasPrefix("<>>", i, Fiveturn)) )
			{
				if( (i+5) < size)
				{
					summary[i+1]= 'I';
					summary[i+2]= 'I';
					summary[i+3]= 'I';
					summary[i+4]= 'I';
					summary[i+5]= 'I';
					i=i+2;
				}	
			}// do we have a helix reduced to less than minimal size?
			else if( (hasPrefix("-<5", i, Fiveturn)) || (hasPrefix("X>", i, Fiveturn)) ) 
			{
				if( (i+5) < size)
				{				
					summary[i]= '-';
					summary[i+1]= 'T';
					summary[i+2]= 'T';
					summary[i+3]= 'T';
					summary[i+4]= 'T';
					summary[i+5]= 'T';
				  i=i+2;
				}	
			}	//do we have overlapping minimal helices?
			else if(hasPrefix(" ", i, Fiveturn)) 
			{
			}

						
		} 

		// now we edit the 3 Helices	
		
		// now we edit the single Bridges and the extended bridges
		
		// finally we edit the 4 helices
		// we have to ensure that we didn't overwrite symbols 
		// with 'T' in summary string 
		for(Size i=0; i<size; i++)
		{
			if(   (hasPrefix("->>", i, Fourturn)) 
	       || (hasPrefix("<>>", i, Fourturn)) )
			{
				if( (i+4) < size)
				{
					summary[i+1]= 'H';
					summary[i+2]= 'H';
					summary[i+3]= 'H';
					summary[i+4]= 'H';
					i=i+2;
				}
			}// do we have a helix reduced to less than minimal size?
			else if( (hasPrefix("-<4", i, Fourturn)) || (hasPrefix("X>", i, Fourturn)) ) 
			{
				if( ((i+4) < size))
				{				
					for(int j=1; j<5; j++)
					{
						if(summary[i+j]=='-')
							summary[i+j]= 'T';
					}				
				  i=i+2;
				}	
			}
				//do we have overlapping minimal helices?
			else	if( helix_before_ && (hasPrefix("XX", i, Fourturn)) || hasPrefix(">X", i, Fourturn))
			{
				if(i+4 < size)
				{
					summary[i+1]= 'H';
					summary[i+2]= 'H';
					summary[i+3]= 'H';
					summary[i+4]= 'H';
					i=i+2;
				} 
			}	
			//other symbols are 4 and <, which belong to a startsymbol ->> or <>> and
			//therefor have not to be considered
		}// end o for
		
		String s1, s2, s3, s4, s5, s6;
		for (Size i=0; i<size; i++)
		{
			s1+=summary[i];
			s2+=Threeturn[i];
			s3+=Fourturn[i];
			s4+=Fiveturn[i];
			s5+=bridge1[i];
			s6+=bridge2[i];
		}
		std::cout << "summary"<< s1 << std::endl;
		std::cout << "Threeturn" << s2 << std::endl;
		std::cout << "Fourturn"<< s3 << std::endl;
		std::cout << "Fiveturn" << s4 << std::endl;
		std::cout << "Bridge1" << s5 << std::endl;
		std::cout << "Bridge2" << s6 << std::endl;
	************************************************/


