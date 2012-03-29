// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//



#include <BALL/SEQUENCE/homologyModeller.h>

#include <BALL/SEQUENCE/alignment.h>
#include <BALL/KERNEL/protein.h>
#include <BALL/STRUCTURE/structureMapper.h>
#include <BALL/STRUCTURE/atomBijection.h>
#include<BALL/STRUCTURE/RMSDMinimizer.h>

#include <BALL/MATHS/matrix44.h>



#ifndef BALL_STRUCTURE_PEPTIDES_H
#include <BALL/STRUCTURE/peptides.h>
#endif
#include <BALL/KERNEL/residue.h>
#include <BALL/STRUCTURE/atomBijection.h>

namespace BALL
{

				HomologyModeller::HomologyModeller()
				{   }

				HomologyModeller::HomologyModeller(const HomologyModeller& modeller)
				{   }


				HomologyModeller::~HomologyModeller()
				{   }


				bool HomologyModeller::createModel(Alignment& alignment, Protein& templ)
				{
								if (alignment.empty())
								{
												return false;
								}

								if(! alignment.isAligned())
								{
												return false;
								}

								vector<AtomBijection> bijections;

								//iterate through every sequence in the alignment
								for (unsigned int al_row =0; al_row< alignment.rows(); al_row++)	
								{
												bijections.push_back( *(new AtomBijection()));

												//iterate through every character in the current sequence
												for(unsigned int al_char =0; al_char <alignment.cols(); al_char++)
												{
																if(Peptides::ThreeLetterCode(alignment.getSeqChar(al_row, al_char).getChar()) == templ.getResidue(al_char)->getName())
																{

																				//TODO Chains ignored at the moment!!
																				//iterate over all Atoms of the Residue 
																				AtomIterator prot_it, tmpl_it = templ.getResidue(al_char)->beginAtom();
																				prot_it = alignment.getSeqChar(al_row, al_char).getResidue()->beginAtom();


																				for (; +tmpl_it && +prot_it; ++tmpl_it, ++prot_it)
																				{
																								//map template atom onto new Atom
																								bijections.at(al_row).push_back(AtomBijection::AtomPair(&*tmpl_it, &*prot_it));

																				}
																}
												}
								}

								if(bijections.empty())
								{
												return false;
								}

								//iterate over all Atombijections and transform the Protein
								for (vector<AtomBijection>::iterator it = bicetions.begin(); it++)
								{
												RMSDMinimizer minimizer;

												//Note: at the moment RMSD is just thrown away
												RMSDMinimizer::Result result = minimizer.computeTransformation(bijections.at(i));

												TMatrix4x4<float> transformation = result.first;

												//perform a transformation for each Atom of the bijection
												for (AtomBijection::iterator it = bijections.at(i).begin(); it !=bijections.at(i).end() ; ++it)
												{
																TransformationProcessor *transformer = new TransformationProcessor(transformation);

																transformer->operator() ( *(it->second));
												}


								}
								return true;	
				}

}//namespace BALL
