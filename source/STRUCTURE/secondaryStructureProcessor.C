// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/STRUCTURE/secondaryStructureProcessor.h>
#include <BALL/STRUCTURE/HBondProcessor.h>

#include <BALL/KERNEL/secondaryStructure.h>           
#include <BALL/KERNEL/protein.h>
#include <BALL/KERNEL/chain.h>
#include <BALL/KERNEL/bond.h>
#include <cmath>

using namespace std;

namespace BALL
{
	SecondaryStructureProcessor::SecondaryStructureProcessor()
	{
	}

	bool SecondaryStructureProcessor::testString_(const String& s, Size offset, Size offset_2)
	{
		return testString2_(s, offset) &&
					 testString2_(s, offset + offset_2);
	}


	bool SecondaryStructureProcessor::testString2_(const String& s, Size offset)
	{
		if (s.size() < offset + 2) return false;

		const Substring s1 = s.getSubstring(offset, 2);
		return (s1 == ">>" ||
						s1 == "XX" ||
						s1 == ">X" ||
						s1 == "X>");
	}

	bool SecondaryStructureProcessor::testString3_(const String& s, Size offset, char x)
	{
		if (s.size() < offset + 2) return false;

		const Substring s1 = s.getSubstring(offset, 2);

		if (s1 == "><" || s1 == "X<") return true;

		return (s1[1] == x && 
				    (s1[0] == '>' || s1[0] == 'X'));
	}


	 /***************************************
	 * find the Secondary Structures
	 * **************************************/
	void SecondaryStructureProcessor::compute_()
	{
		 /*********************************
		 *   initialize the summary strings			
		 **********************************/
		// note that HBonds_.size() is not the number of HBonds but
		// rather the number of residues in the system.
	  Size size = HBonds_.size();

		sheet_.resize(size);
		for (Size i=0; i<size; i++)
		{
			sheet_[i]='-';
		}
		fiveturn_  = sheet_;
		fourturn_  = sheet_;
		threeturn_ = sheet_;
		summary_   = sheet_;

	  /************************************************
	  *  in the first place we search and insert turns
		*************************************************/

		//over all residues

		for (Size i=0; i<size; i++)
		{
			if (HBonds_[i].size() != 0 )
			{
				// over all HBondpartners
				for (Size k=0; k<HBonds_[i].size(); k++)
				{	
					//---------- 4 turns
					if (HBonds_[i][k] == (i+4))
					{
						//first sign
						if ( (fourturn_[i]  == '<') || (fourturn_[i] == 'X'))
						{
							fourturn_[i]   = 'X';
						}
						else
						{
							fourturn_[i]   = '>';
						}
						//position 2,3,4
						for (int j=1; j<4; j++)
						{
							if (fourturn_[i+j]=='-')
							{
								fourturn_[i+j]='4';
							}
						}

						//last position has always to be <
						fourturn_[i+4] = '<';
					}

					//---------- 3 turns
					else if (HBonds_[i][k] == (i + 3))
					{
						//first sign
						if ( (threeturn_[i]  == '<') || (threeturn_[i] == 'X'))
						{
							threeturn_[i]   = 'X';
						}
						else
						{
							threeturn_[i]   = '>';
						}
						//position 2,3
						for (int j=1; j<3; j++)
						{
							if (threeturn_[i+j]=='-')
							{
								threeturn_[i+j]='3';
							}
						}

						//last position has always to be <
						threeturn_[i+3] = '<';
					} 

					//---------- 5 turns
					else if (HBonds_[i][k] == (i + 5))
					{
						//first sign
						if( (fiveturn_[i]  == '<') || (fiveturn_[i] == 'X'))
						{
							fiveturn_[i]   = 'X';
						}
						else
						{
							fiveturn_[i]   = '>';
						}
						//position 2,3
						for (int j=1; j<5; j++)
						{
							if (fiveturn_[i+j]=='-')
							{
								fiveturn_[i+j]='5';
							}
						}

						//last position has always to be <
						fiveturn_[i+5] = '<';

					} //else if turns

				}//	for(int k=0; k<HBonds_[i].size(); k++)

			}//	if(HBonds_[i].size() != 0 )

		}//for all residues



		/****************************************
		 * in the next step, we search bridges
		 ****************************************/
		//initialize posbridges_
		posbridges_.resize(size);
		
		//over all residues
		for (Size current_res=0; current_res<size; current_res++)
		{		
			if (HBonds_[current_res].size() != 0 )
			{
				// over all HBondpartners
				for (Size current_bond=0; current_bond<HBonds_[current_res].size(); current_bond++)
				{		
					int partner = HBonds_[current_res][current_bond];

					// actually we have HBP(i,k) with  i=current_res, k=partner
					// do we have HBP(k, i+2) ? => parallel bridge(i+1,k) 			
					for (Size s = 0; (s < HBonds_[partner].size()); s++)
					{
						if (HBonds_[partner][s]== current_res + 2) 
						{
							//insert
							//NOTE: there might be  more than two bridges for this
							// residue but this should never happen ;-)
							
							posbridges_[current_res+1].push_back(partner);
							posbridges_[partner].push_back(current_res+1);

							sheet_[current_res+1]= '+';
							sheet_[partner]  = '+';
						}
					}	 //forend parallelbridge

					// currently we have HBP(i,k) with i=current_res and k=partner
					// do we have HBP(k, i) ? => antiparallel bridge(i,k)
					for (Size s = 0; (s < HBonds_[partner].size()); s++)
					{
						if (HBonds_[partner][s] == current_res) 
						{
		    		  // we aren't allowed to overwrite antiparallel bridges found before!
							// remember: we have two equal cases: 
							// 							first : HBP(k,i) && HBP(i,k)  
							// 					and second: HBP(i,k) && HBP(k,i)   						
								
							//insert
							//NOTE: there might be  more than two bridges for this residue
							// but this should never happen ;-)
						
							posbridges_[current_res].push_back(partner);
							posbridges_[partner].push_back(current_res);
							sheet_[current_res]= '/';
						  sheet_[partner]  = '/';
						}
					}//forend antiparallelbridge

					// currently we have HBP(i,k) with i=current_res and k=partner
					// do we have HBP(k-2, i+2) ? => antiparallel bridge(i+1,k-1)
					if (((partner-2)>=0) && ((current_res+2) <=size))
					{
						for (Size s = 0; (s < HBonds_[partner-2].size()); s++)
						{

							if ((HBonds_[partner-2][s] == (current_res + 2))
									&& ((current_res + 1) != (Size)(partner - 1)))
							{
								//insert
							  //NOTE: there might be  more than two bridges for this residue
							  // but this should never happen ;-)
						
								posbridges_[current_res+1].push_back(partner-1);
								posbridges_[partner-1].push_back(current_res+1);
								sheet_[current_res+1]= '/';
						  	sheet_[partner-1]  = '/';
							}
						}
					}	// if (partner-2>=...)
				}//	for(int k=0; k<HBonds_[i].size(); k++)
			}//	if(HBonds_[i].size() != 0 )
		}//for all residues

		
		//
		//  now we search ladders!
		//
	  // ladder: set of one or more consecutive bridges of identical type
		// or bulge-linked ladders: two ladders or bridges of the same type 
		//                          connected by at most one extra residue 
		//                          on one strand and at most four extra 
		//                          residues on the other strand
		
		char parallel = 'a'-1;
		char antiparallel = 'A'-1;
		enum pattern { PARALLEL, ANTIPARALLEL, DEFAULT } last_pattern;
	 	char letter='-';
	 	
		//over all residues
		last_pattern = DEFAULT;
		bool found_a_pattern = false;
		int no_residue = -5;
		int last_parallel_res = no_residue; 
		int last_antiparallel_res = no_residue;
		int last_residue = no_residue;
		
		for (  Size residue = 0; 
			   (residue<sheet_.size()) && (residue<posbridges_.size()); 
				  residue++)
	  { 
			// do we have a bridge?		 	
			if ( (sheet_[residue]!='-'))
			{
				// parallel bridge				
				if (    (sheet_[residue]=='+') 
						|| (  (sheet_[residue]>('a'-1)) 
						    &&(sheet_[residue]<('z'+1))
							 )
					)	
				{
					last_residue = last_parallel_res;
					last_pattern = PARALLEL;
				}
				else // antiparallel bridge
				{
					last_residue = last_antiparallel_res;
					last_pattern = ANTIPARALLEL;
				}				
				
				found_a_pattern = false;
				bool has_bonds_to_the_left = false;
				
				if (last_residue != no_residue)
				{
					for (Size last_part_i=0; last_part_i < posbridges_[last_residue].size(); last_part_i++)
					{
						for (Size curr_part_i=0; curr_part_i < posbridges_[residue].size(); curr_part_i++)
						{				
							int last_part=posbridges_[last_residue][last_part_i];
							int curr_part=posbridges_[residue][curr_part_i];			
						
							// have we already seen this residue by a bridge before?
							if (curr_part < (int)residue)
							{
								has_bonds_to_the_left = true;
								letter = sheet_[curr_part];
							}
							
							// do we have a continuation of a ladder?
							// allowed are bulge-linked ladders 
							//   which consist of two ladders or bridges of the same type 
							//   connected by at most one extra residue on one strand and 
							//   at most four extra residues on the other strand!   
							// too

							const Size r_diff = residue - last_residue;
							const Size p_diff = abs(curr_part - last_part);

							if ((r_diff == 1 && p_diff == 1) ||
									(r_diff < 2 && p_diff < 5) ||
									(r_diff < 5 && p_diff < 2))
							{
								found_a_pattern= true;
							}
						}	//end for
					}//end for	
				}//end if
				

				// NOTE: there is no problem if we have seen the 
				// residue before and set the letter 
				// AND found a pattern and overwrite the letter perhaps! 
				// the sheet_-Loop(see below) will give them a unique letter 
				if (last_residue != no_residue && found_a_pattern)
				{	
					letter=sheet_[last_residue];	
				}
				else 
				{
					if (!has_bonds_to_the_left)
					{
						if (last_pattern == PARALLEL)
						{
							parallel+=1;
							if(parallel == 'z'+1) parallel='a';
							letter = parallel;	
						}
						else 
						{
							antiparallel+=1;
							if (antiparallel == 'Z'+1) antiparallel='A';
							letter = antiparallel;	
						}				
					}
				}

				if (last_pattern == PARALLEL) last_parallel_res = residue;
				else 													last_antiparallel_res = residue;

				// name all residues belonging to this bridge
				// NOTE: in most cases there will be just one entry
				sheet_[residue]=letter;	
				for (Size curr_part=0; curr_part < posbridges_[residue].size(); curr_part++)
				{
					if (posbridges_[residue][curr_part] > (int)residue)
					{ //if we have a bridge to the right, we name the partner
						//	sheet_[posbridges_[residue][curr_part]]=letter;
					}
					//		sheet_[posbridges_[residue][curr_part]]=letter;
				}

			}//if(sheet_[residue]!='-'
		}//for(Size residue = 0 ....		
			
		
		//
		// now we are looking for sheet_s
		//
		// sheet_: set of one or more ladders connected by shared residues
	
		for (Size residue = 0; 
			   residue < sheet_.size() && residue < posbridges_.size(); 
				  residue++)
	  { 
			// do we have a bridge?		 	
			if (sheet_[residue] != '-')
			{
				letter = sheet_[residue];
				for (Size curr_part_i=0; curr_part_i < posbridges_[residue].size(); curr_part_i++)
				{
					int curr_part = posbridges_[residue][curr_part_i];			
					if (sheet_[curr_part]!=letter)
					{
						changeAllXToY_(sheet_[curr_part], letter, sheet_);
					}				
				}				
			}
		}
		
	
		// /***********************************
		//  * now we repair the irregularities
		// 	***********************************/
		//  two overlapping minimal helices(consecutive n-turns)
		//  offset by tow or three residues are joined into one 
		//  helix
		//  therefor we just insert the missing n-turns
 	 
		Size length = 5;
		Size turn = 3;

		// we start with the 3-turn
		for (Size i= 0; i < size; i++)
		{
			// we have to proof the length : offset by two or three 
			// residues and the lenght of the turn itself 
			// offset two residues 			
			if (i + 2 + length < size 				&&
					testString_(threeturn_, i, 3) &&
					fourturn_[i + 2] != '>' 			&& 
					fourturn_[i + 2] !='X'				)
			{
				insertTurn_(turn, i+2);
			}
			
			// offset three turns	
			if (i + 3 + length < size 				&&  
					testString_(threeturn_, i, 4) &&
					fourturn_[i + 2] != '>' 			&& 
					fourturn_[i + 2] != 'X'				&&
					fourturn_[i + 3] != '>' 			&& 
					fourturn_[i + 3] !='X'				)
			{
				insertTurn_(turn, i + 2);
			 	insertTurn_(turn, i + 3);
			}			
		}
	
		
		length = 6;
		turn = 4;			
		// -------------- 4-turn -----------------------
		for(Size i= 0; i<size; i++)
		{
			// we have to proof the length : offset by two or three 
			// residues and the lenght of the turn itself (here 6) 

			// offset two residues
			if (i + 2 + length < size 				&&
					testString_(fourturn_, i, 3) 	&&
					fourturn_[i + 2] != '>'			 	&& 
					fourturn_[i + 2] != 'X'				)
			{
				insertTurn_(turn, i + 2);
			}		

			// offset three turns	
			if (i + 3 + length < size 				&&  
					testString_(fourturn_, i, 4) 	&&
					fourturn_[i + 2] != '>' 			&& 
					fourturn_[i + 2] !='X'				&&
					fourturn_[i + 3] != '>' 			&& 
					fourturn_[i + 3] !='X'  			)
			{
				insertTurn_(turn, i + 2);
			 	insertTurn_(turn, i + 3);		
			}			
		}
		
		
		// -------------- 5-turn -----------------------
		
		length = 7;
		turn = 5;			

		for (Size i = 0; i < size; i++)
		{
			// we have to proof the length : offset by two or three 
			// residues and the lenght of the turn itself (here 7) 

			// offset two residues
			if (i + 2 +length < size 					&& 
					testString_(fiveturn_, i, 3) 	&&
					fourturn_[i + 2] != '>' 			&& 
					fourturn_[i + 2] !='X'				)
			{
				insertTurn_(turn, i+2);
			}		

			// offset three turns	
			if (i + 3 +length < size 					&&  
					testString_(fiveturn_, i, 4) 	&&
					fourturn_[i+2] != '>'					&& 
					fourturn_[i+2] !='X'					&&
					fourturn_[i+3] != '>'					&& 
					fourturn_[i+3] !='X')
			{
				insertTurn_(turn, i+2);
			 	insertTurn_(turn, i+3);
			}			
		}	
   
		
		// /*****************************************************
		// *
		// * -------- now we construct the summary_ string ----------
		// *
		// * structural overlaps are eleminated by considering
  	// * hierarchy H > B > E > G > I > T
  	// * 		H means 4 Helices, B means single bridges, 
  	// * 		E means extended ladder residues, G means 3 Helices
  	// *		I means 5 Helices and T means single 3-, 4-, or 5-turns 
		// * we start with writing 5 Helices and overwrite graduately 
 		// * the summary_ string with 3 Helices, extended bridges, 
		// * single bridges and 4 Helices helices	
    // -------------------------------------------------------
		// ****************************************************/


		// --------------------- 5 helices ---------------------- 
		for (Size i= 0; i<size; i++)
		{
			// we initialize the summary_ string with '-'			
			if(fiveturn_[i] == '-') 
			{
				summary_[i]= '-';
			}
			else if (testString2_(fiveturn_, i))
			{
				if (i+5 < size)
				{
					summary_[i+1]= 'I';
					summary_[i+2]= 'I';
					summary_[i+3]= 'I';
					summary_[i+4]= 'I';
					summary_[i+5]= 'I';
				}	
			}
			// do we have a helix reduced to less than minimal size?
			else 
			{
				String ss = fiveturn_.getSubstring(i).toString();
				if (testString3_(fiveturn_, i, '5'))
				{
					for(int j=1; (j<5) && ((i+j)<summary_.size()) && ((i+j)<fiveturn_.size()) ;j++)
					{			
						if(fiveturn_[i+j]!='I')
						{
							summary_[i+j]= 'T';
						}				
					}	
				}	
			}
		}	
		
		// -------------------3 helices ------------------------
		for(Size i= 0; i<size; i++)
		{
			if (testString2_(threeturn_, i))
			{
				if( (i+3) < size)
				{
					summary_[i+1]= 'G';
					summary_[i+2]= 'G';
					summary_[i+3]= 'G';
				}	
			}// do we have a helix reduced to less than minimal size?
			 // we have to consider, that we do not overwrite 
			else if(testString3_(threeturn_, i, '3'))
			{
				if (i+3 < summary_.size())
				{
					for(Size j=1; j<3;j++)
					{ 
						if(  (summary_[i+j]!= 'G') 
							 &&(summary_[i+j]!= 'I') 
							)
						{	
							summary_[i+j]= 'T';	
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
		// first we generate the sheet_-line and than summarize it in the summary_ line
		for(Size i=0; i< (sheet_.size()); i++)
		{
			if(sheet_[i]!='-')
			{
				letter = sheet_[i];			
				int j=0;			
				for(j=0; ((i+j)<sheet_.size()) && (sheet_[i+j]==letter); j++)
				{
				}
				if((j==1)&&(summary_[i]!='E')) //single bridge
				{
					summary_[i]='B';
				}
				else if(j==0)
				{
				}
				else //extended bridge
				{		
					for(int n=0; n<j; n++)
					{
						summary_[i+n]='E';
					}
				  i=i+j-1;	
				}								
			}				
    }
		
						
		// ---------------- 4 helices ---------------------
		for(Size i= 0; i<size; i++)
		{
			if (testString2_(fourturn_, i))
			{
				if (i+4 < size)
				{
					summary_[i+1]= 'H';
					summary_[i+2]= 'H';
					summary_[i+3]= 'H';
				  summary_[i+4]= 'H';
				}	
			}// do we have a helix reduced to less than minimal size?
			 // we have to consider, that we do not overwrite 
			else if(   (fourturn_.getSubstring(i).toString().hasPrefix(">4"))
						  || (fourturn_.getSubstring(i).toString().hasPrefix("X4"))
						 )  
			{
							
				if (i+4 < size)
				{
					for(Size j=1; j<4;j++)
					{ 
						if(  ( summary_[i+j]!= 'G')
							 &&( summary_[i+j]!= 'H')
							 &&( summary_[i+j]!= 'I')
							 &&( summary_[i+j]!= 'E')
							 &&( summary_[i+j]!= 'B')
							)
						{	
							summary_[i+j]= 'T';	
						}
					}//end for
				}//end if	
			}//end if	
		}//end for
	
		// we should read the summary_ string again in order to substitute
		// single G or I, generated by overwriting GGG or IIIII by HHHH 
		for(Size i=0; i<( summary_.size()); i++)
		{			
			if( (     ((i+2)< summary_.size())
					  && (summary_[i]  !='G')
				    && (summary_[i+1]=='G')
				    && (summary_[i+2]!='G')
				  )
				||(    ((i+2)< summary_.size())
						&& (summary_[i]  !='I')
				    && (summary_[i+1]=='I')
				    && (summary_[i+2]!='I')
				   )
				)
			{
				summary_[i+1]='T';
			}		
			if( (     ((i+3)< summary_.size())
					  && (summary_[i]  !='G')
				    && (summary_[i+1]=='G')
				    && (summary_[i+2]=='G')
				    && (summary_[i+3]!='G')
				  )
				||(    ((i+3)< summary_.size())
    			  && (summary_[i]  !='I')
				    && (summary_[i+1]=='I')
				    && (summary_[i+2]=='I')
				    && (summary_[i+3]!='I')
					)
				)				
			{

				summary_[i+1]='T';
				summary_[i+2]='T';
			}	
			
			if( (     ((i+4)< summary_.size())
    			  &&  (summary_[i]  !='I')
				    &&  (summary_[i+1]=='I')
				    &&  (summary_[i+2]=='I')
				    &&  (summary_[i+3]=='I')
				    &&  (summary_[i+4]!='I')
					)
				)				
			{
				summary_[i+1]='T';
				summary_[i+2]='T';
				summary_[i+3]='T';
			}	
		}
		
	}
	
	void SecondaryStructureProcessor::insertTurn_(int turn, int position)
	{
		bool correct = true;			
		String *n_turn;	
		if(turn == 3)
		{ 
			n_turn = &threeturn_;
		}else if(turn == 4)
		{ 
			n_turn = &fourturn_;
		}else if(turn == 5)
		{
			n_turn = &fiveturn_;
		}else 
		{
			correct = false;//TODO: Error message & return Log.error() << " variable turn was set to a unregular value" << std::endl;
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
			//positions in between
			for (int j=1; j<turn; j++)
			{
				if((*n_turn)[position+j]=='-')
				{
					String s(turn);
					(*n_turn)[position+j]= s[0];
				}
			}
			//last position 
			if(	  ((*n_turn)[position+turn] == '>') 
				 || ((*n_turn)[position+turn] == 'X'))
			{				
				(*n_turn)[position+turn] = 'X';
			}else
			{
				(*n_turn)[position+turn] = '<';
			}
		}			
	}

	
	void SecondaryStructureProcessor::changeAllXToY_(char X, char Y, String& target)
	{
		for(Size i=0; i<target.size(); i++)
		{
			if(target[i]==X)
			{
				target[i]=Y;
			}
		}				
	}
	
	
	char SecondaryStructureProcessor::setSecondaryStructureType_(SecondaryStructure* ss, char type)
	{
		char result;

		switch (type)
		{
			case 'H': ss->setType(SecondaryStructure::HELIX);
								ss->setProperty("HELIX_TYPE", "ALPHA");
								result = 'H';
								break;
			case 'G': ss->setType(SecondaryStructure::HELIX);
								ss->setProperty("HELIX_TYPE", "THREE_TEN");
								result = 'G';
								break;
			case 'I': ss->setType(SecondaryStructure::HELIX);
								ss->setProperty("HELIX_TYPE", "PI");
								result = 'I';
								break;
			case 'E': ss->setType(SecondaryStructure::STRAND);
								result = 'E';
								break;
			default:  ss->setType(SecondaryStructure::COIL);
								result = 'L';
		}

		return result;
	}
	
	/**********************************************
	 *   determine the new Secondary Structure and
	 *   replace the old one with the new one
	 **********************************************/
	Processor::Result SecondaryStructureProcessor::operator() (Composite &composite)
	{
        if (!RTTI::isKindOf<Chain>(&composite))
		{
			return Processor::CONTINUE;
		}

		Chain* p = RTTI::castTo<Chain>(composite);
		HBondProcessor hbp;

		p->apply(hbp);       // find all posible HBonds
		
		HBonds_ = hbp.getBackboneHBondPattern();
		ResidueIterator ri = p->beginResidue();
		 
		if (!(+ri))
		{
			return Processor::CONTINUE;
		}
		
		// locate the Secondary Structures
		compute_();

		//----------associate new Secondary Structures (SS) for each residue -----
	  // - summarize equal residues in one new SS (new_ss)
		// - push for each residue the new SS into new_parent  
		// - push all residues into residues
		
		SecondaryStructure* ss = 0;
		char     last_struct   = 'X';
		Position resnum        = 0;
 
		vector<SecondaryStructure*> new_ss;
		vector<SecondaryStructure*> new_parent;
		vector<Residue*> 						residues;

		if (summary_.size() == 0)
			return Processor::CONTINUE;
		
		for (; +ri; ++ri)
		{
			if (resnum >= summary_.size())
			{
				Log.error() << "A problem occured in " << __FILE__ << " " << __LINE__ << std::endl;
				return Processor::CONTINUE;
			}

			// depending on the last type of secondary structure we have seen,
			// we need to react differently to merge them sensibly
			if (last_struct != summary_[resnum])
			{
				switch (last_struct)
				{
					case 'L': // we are in a loop
						// note that we identify 'real' loops, isolated bridges and turns (-,B,T)
						// and map them all to loops. Thus we need to determine here if the current
						// residue also maps to a loop (we already know that the last residue was one)
						switch (summary_[resnum])
						{
							case '-':
							case 'B':
							case 'T': break; // nothing to see here... please walk on...

							default:
								// the current residue is no loop => build a new SecondaryStructure
								ss = new SecondaryStructure;
								last_struct = setSecondaryStructureType_(ss, summary_[resnum]);
								new_ss.push_back(ss);
						}
						break;
				
					default: // in all other cases, setSecondaryStructure does the hard work
						ss = new SecondaryStructure;
						last_struct = setSecondaryStructureType_(ss, summary_[resnum]);
						new_ss.push_back(ss);
				}
			}

			// in all cases, ss is now the new parent of this residue
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
		// we have to be sure not to delete a SS which has an SS as parent!
		// because it would be deleted twice
		vector<SecondaryStructure*> to_remove;
		SecondaryStructureIterator ssit = p->beginSecondaryStructure();
		for (; +ssit; ++ssit)
		{
			if ((*ssit).getParent() == 0 ||
                    !RTTI::isKindOf<SecondaryStructure>((*ssit).getParent()))
			{
				to_remove.push_back(&*ssit);
			}
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
