// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: secondaryStructureProcessor.C,v 1.4 2004/02/23 17:58:35 oliver Exp $

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

	void SecondaryStructureProcessor::compute()
	{
		Size size = HBonds_.size();
	
		sheet.resize(size);
		for (Size i=0; i<size; i++)
		{
			sheet[i]='-';
		}
		bridge1  = sheet;
		bridge2  = sheet;
		Fiveturn = sheet;
		Fourturn = sheet;
		Threeturn = sheet;
		summary   = sheet;

		/****************************************
		 *  in the first place we search turns
		 ****************************************/

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
						if( Fourturn[i]  == '<')
						{
							Fourturn[i]   = 'X';
						}else
						{
							Fourturn[i]   = '>';
						}
						//	cout << " nach 1.Zeichen" << i << endl; 
						//position 2,3,4
						for (int j=1; j<4; j++)
						{
							if( Fourturn[i+j]  == '<')
							{
								Fourturn[i+j]  = '<';
							}else
							{
								Fourturn[i+j]  = '4';
							}
						}

						//	cout << " -->nach mittleren Zeichen" << i<<" tes Residue "<< endl;
						// Fourturn[i+1] = '4';
						// Fourturn[i+2] = '4';
						// Fourturn[i+3] = '4';

						//last position has always to be <
						Fourturn[i+4] = '<';
						// cout << " -->nach letztem Zeichen " << i << endl; 
					}

					//---------- 3 turns
					else if (HBonds_[i][k]==(int)(i+3))
					{
						//first sign
						if( Threeturn[i]  == '<')
						{
							Threeturn[i]   = 'X';
						}else
						{
							Threeturn[i]   = '>';
						}
						//position 2,3
						for (int j=1; j<3; j++)
						{
							if( Threeturn[i+j]  == '<')
							{
								Threeturn[i+j]  = '<';
							}else
							{
								Threeturn[i+j]  = '3';
							}
						}

						//last position has always to be <
						Threeturn[i+3] = '<';

						//			 	Threeturn[i]   = '>';
						//				Threeturn[i+1] = '3';
						//				Threeturn[i+2] = '3';
						//				Threeturn[i+3] = '<';
					} 

					//---------- 5 turns
					else if (HBonds_[i][k]==(int)(i+5))
					{
						//first sign
						if( Fiveturn[i]  == '<')
						{
							Fiveturn[i]   = 'X';
						}else
						{
							Fiveturn[i]   = '>';
						}
						//position 2,3,4,5
						for (int j=1; j<5; j++)
						{
							if( Fiveturn[i+j]  == '<')
							{
								Fiveturn[i+j]  = '<';
							}else
							{
								Fiveturn[i+j]  = '5';
							}
						}

						//last position has always to be <
						Fiveturn[i+5] = '<';

						//				Fiveturn[i]   = '>';
						//				Fiveturn[i+1] = '5';
						//				Fiveturn[i+2] = '5';
						//				Fiveturn[i+3] = '5';
						//				Fiveturn[i+4] = '5';
						//				Fiveturn[i+5] = '<';
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
							if(bridge1[i+1]=='-')
							{
								bridge1[i+1] = parallel;
							}
							else
							{
								bridge2[i+1]= parallel;
							}
							if(bridge1[k]=='-')
							{
								bridge1[k] = parallel;
							}
							else
							{
								bridge2[k]= parallel;
							}

							lastpattern     = PARALLEL;
							found_a_pattern = true;

							break;
						}
					}	 //forend parallelbridge


					// actually we have HBP(i,k)
					// do we habe HBP(k, i) ? => antiparallel bridge(i,k)
					for (Size s = 0; (s < HBonds_[k].size()); s++)
					{
						if(HBonds_[k][s]== (int)i) 
						{
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

								if(bridge1[i]=='-')
								{
									bridge1[i] = antiparallel;
								}
								else
								{
									bridge2[i]= antiparallel;
								}
								if(bridge1[k]=='-')
								{
									bridge1[k] = antiparallel;
								}
								else
								{
									bridge2[k]= antiparallel;
								}

								lastpattern     = ANTIPARALLEL;
								found_a_pattern = true;

								break;
							}
						}
					}//forend antiparallelbridge1

					// actually we have HBP(i,k)
					// do we have HBP(k-2, i+2) ? => antiparallel bridge(i+1,k-1)
					if(((k-2)>=0) && ((i+2) <=size))
					{
						for (Size s = 0;(s < HBonds_[k-2].size()); s++)
						{

							if (    (HBonds_[k-2][s] == (int)(i+2))
									&& ((int)(i+1) != (k-1)))
							{

								// make sure that we didn't already see this bridge
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


		//now we construct the sujmary string


		/*
			 vector<char> sheet(size, '-');
			 vector<char> bridge1(size, '-');
			 vector<char> bridge2(size, '-');
			 vector<char> Fiveturn(size, '-');
			 vector<char> Fourturn(size, '-');
			 vector<char> Threeturn(size, '-');
			 vector<char> summary(size, '-');
		 */

		for(Size i = 0; i < size;i++)
		{
			if (Fourturn[i]!='-')
			{
				summary[i]= 'H';
			}
			else if ((bridge1[i] != '-') || (bridge2[i] != '-'))
			{
				if ((bridge1[i-1] != bridge1[i]) 
						&& (bridge1[i] != bridge1[i+1])
						&& (bridge2[i-1] != bridge1[i]) 
						&& bridge2[i+1] != bridge1[i])
				{
					summary[i]='B';
				}	
				else if((bridge2[i-1] != bridge2[i]) 
								&& (bridge2[i] != bridge2[i+1])
								&& (bridge2[i] != bridge1[i-1]) 
								&& (bridge2[i] != bridge1[i+1]))
				{
					summary[i] = 'B';
				}	
				else
				{
					summary[i] = 'E';
				}
			}
			else if	(Threeturn[i] != '-')
			{
				summary[i] = 'G';
			}	
			else if (Fiveturn[i] != '-')
			{
				summary[i]= 'I';
			}

		}
	/*
		// ?????
		//!!!T wenn die Helix zu klein ist
		for(Size i= 0; i<size;i++)
		{
		} 
	*/

		String s1, s2, s3, s4, s5, s6;
		for (Size i=0; i<size; i++)
		{
			s1 += summary[i];
			s2 += Threeturn[i];
			s3 += Fourturn[i];
			s4 += Fiveturn[i];
			s5 += bridge1[i];
			s6 += bridge2[i];
		}
/*
		std::cout << "summary"<< s1 << std::endl;
		std::cout << "Threeturn" << s2 << std::endl;
		std::cout << "Fourturn"<< s3 << std::endl;
		std::cout << "Fiveturn" << s4 << std::endl;
		std::cout << "Bridge1" << s5 << std::endl;
		std::cout << "Bridge2" << s6 << std::endl;
	*/
	}

	Processor::Result SecondaryStructureProcessor::operator() (Composite &composite)
	{
		if (!RTTI::isKindOf<Chain>(composite))
		{
			return Processor::CONTINUE;
		}

		Chain* p = RTTI::castTo<Chain>(composite);
		HBondProcessor hbp;
		p->apply(hbp);
		HBonds_ = hbp.getHBondPairs();
		ResidueIterator ri = p->beginResidue();
		 
		if (!(+ri))
		{
			return Processor::CONTINUE;
		}
		compute();

		// Splice into the Protein
		SecondaryStructure* ss = 0;
		char last_struct = 'X';
		Position resnum=0;

		vector<SecondaryStructure*> new_ss;
		vector<SecondaryStructure*> new_parent;
		vector<Residue*> 						residues;
		for (;+ri;++ri)
		{
			if (summary[resnum] != last_struct)
			{
				if (last_struct != 'L' || (summary[resnum] != 'G' && summary[resnum] != '-'))
				{
					ss = new SecondaryStructure;
					new_ss.push_back(ss);
				}
			}

			
			// first determine the type of this residue
			switch (summary[resnum])
			{
				case 'H':
				case 'G':
				case 'I':
					// Assign all helices as type HELIX
					ss->setType(SecondaryStructure::HELIX);
					break;
				
				case 'E':
				case 'B':
					// Assign all strands/extended as type STRAND
					ss->setType(SecondaryStructure::STRAND);
					break;

				case 'T':
					// Assign all turns to type TURN
					ss->setType(SecondaryStructure::TURN);
					break;
				
				default:
					// Default: loop region
					ss->setType(SecondaryStructure::COIL);
			}
			last_struct = summary[resnum];

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
		for (;+ssit; ++ssit)
		{
			to_remove.push_back(&*ssit);
		}

		for (Position i = 0; i < to_remove.size(); i++)
		{
			p->remove(*to_remove[i]);
		}

		// ------ insert new SecondaryStructures ----------
		for (Position i = 0; i < new_ss.size(); i++)
		{
			p->insert(*new_ss[i]);
		}
	
		return Processor::CONTINUE;
	}

} //Namespace BALL
