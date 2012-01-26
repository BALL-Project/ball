#include<BALL/SEQUENCE/alignAlgorithm.h>
#ifdef BALL_HAS_SEQAN
	#include <seqan/align.h>
	#include <seqan/score.h>
	#include <seqan/graph_align.h>
	#include <seqan/graph_msa.h>
#endif


namespace BALL {
				AlignAlgorithm::AlignAlgorithm()
					//			alignment()
				{   }


				/*AlignAlgorithm::AlignAlgorithm(BALL::Alignment& alignment)
						:
												alignment(alignment)
				{    }

				AlignAlgorithm::AlignAlgorithm(AlignAlgorithm& align_algo)
				{
								alignment=align_algo.alignment;
				}
*/
				AlignAlgorithm::~AlignAlgorithm()
				{   }


	/*			void AlignAlgorithm::setAlignment(BALL::Alignment& alignment)
				{
								alignment=alignment;
				}

				BALL::Alignment& AlignAlgorithm::getAlignment()
				{
								return alignment;
				}
*/

				bool AlignAlgorithm::align(BALL::Alignment& alignment)
				{
								if(alignment.rows()<2)
								{
												return false;
								}

					/*			typedef seqan::String<seqan::AminoAcid> SSequence; //sequence type
								typedef seqan::Align<SSequence, seqan::ArrayGaps> SAlign; //align type
								typedef seqan::Row<SAlign>::Type SRow; //gapped sequence type
								typedef seqan::Iterator<SRow>::Type        TIterator; // Itersator for Seqan Alignment

								//Define Seqan alignment
								SAlign align;

								//resize align to number od sequences in alignment
								seqan::resize(seqan::rows(align), alignment.rows());

								//convert Sequences to SeqanSequences
								for (unsigned int i=0; i< alignment.rows(); i++)
								{
												//for every row in the alignment create a new Seqan Sequence
												SSequence seqa_seq;

												for(unsigned int j=0; j< alignment.cols(); j++)
												{
																if (!alignment.getSeqChar(i,j).isGap())
																{
																				//append character stored in SequenceCharacter at current position in the alignment to Seqan Sequence
																				seqan::appendValue(seqa_seq, alignment.getSeqChar(i,j).getChar());
																}
																else
																{
																				//retrieve gapped Sequence
																				SRow& row = seqan::row(align,i);
																				//insert Gap at given Position
																				seqan::insertGap(row,j);
																}
												}
												//put sequence into seqan-alignment
												seqan::assignSource(row(align,i),seqa_seq);
								}

								//compute actual alignment

							double score = 0;
								//decide whether to compute pairwise or multiple sequence alignments
								if(alignment.rows()==2)
								{
												//pairwise alignment with Smith- and Waterman Algorithm
												//TODO decide on good scoringfunction!
												score = seqan::globalAlignment(align,seqan::Score<double>(1,-1,-1,-1), seqan::NeedlemanWunsch());
												alignment.setScore(score);
								}
								else
								{
												//compute global multiple alignment
												//TODO decide on scoringFunction
												//int score =	
												seqan::globalMsaAlignment(align, seqan::Blosum62(-1, -11));
								//				alignment.setScore(score);
								}
							
								//find the longest row in alignment
							 unsigned int aliLength = 0;
							for(unsigned int i=0 ; i<seqan::length(seqan::rows(align)); i++)
							{
								if(seqan::length(row(align, i)) > aliLength)
									{
										aliLength = seqan::length(row(align,i));
									}
							}

						//initialize new alignmentMatrix, to store the newly computed alignment
						AlignmentMatrix newAlign;
						//resize newAlign to make room for alignment
						newAlign.resize(seqan::length(rows(align)), aliLength);
	

				//Now shift seqan::alignment into newAlign,
				//we need to know the old residue pointers, therefore we also need a counter at which position we are in the "old" alignment

				//iterate over all rows of seqan::alignment
				for(unsigned int i = 0; i < seqan::length(rows(align)); ++i)
    		{
					//to retrieve all seqan::gaps (even the ones at the beginning/end of a row) we have to use the function iter
			
					//set seqan::row-iterator to start position
        	TIterator it = seqan::iter(row(align,i), 0);
	
					//set seqan::end-iterator to end position
        	TIterator itEnd = seqan::iter(row(align,i), aliLength);

					//denotes positon in the current seqan::row and also the position in the newly created alignment
        	unsigned int new_pos = 0;
        	
					//counter for old alignment
					unsigned int old_pos = 0;

					//iterate over current row
        	while (it != itEnd)
        	{
            if(seqan::isGap(it))
						{
										//Check whether in old alignment is also a Gap at this position
										if(alignment.getSeqChar(i,old_pos).isGap())
										{
														//insert Gap into new Alignment
														newAlign(i,new_pos)=alignment.getSeqChar(i,old_pos);

														//increase old_pos
														++old_pos;
										}
										else
										{
														//insert new Gap into new Alignment
														newAlign(i,new_pos) = *(new SequenceCharacter('-', alignment.getAlignmentMatrix()(i,0).getOrigin(), SequenceCharacter::type::GAP, 0));
										}

						}

						else
						{
										//retrieve character from seqan::alignment
										char c = seqan::getValue(row(align,i),new_pos);				
										//retrieve old_sequence character from alignment and set it to new Position in new_align
										SequenceCharacter seq_c = alignment.getSeqChar(i,old_pos);
										if(c==seq_c.getChar())
										{
														newAlign(i,new_pos) = seq_c;
										}
										else
										{
															return false;
													//	throw Exception::InvalidArgument(__FILE__,__LINE__, "Error while shifting seqan::alignment back into BALL::alignment");
										}

										//set old_pos to the next character in old alignment

										//Check whether there are Gaps in old Alignment, if so just ignore them and increase old_pos
										while(alignment.getSeqChar(i,old_pos).isGap())
										{
														++old_pos;
										}
										//increase old_pos
										++old_pos;
						}

						//increase iterator
            ++it;
						//increase new_pos
            ++new_pos;
        	}
    	}
  		
			//set alignment to new alignment
				alignment.setAlignmentMatrix(newAlign);

				//now that we are done also set the score
				alignment.setScore(score);
*/
				//set aligned
				alignment.setAligned(true);

				return true;
				}


				bool AlignAlgorithm::alignFromSpecificPosition(Alignment& alignment, Position position)
				{

					//CHECK whether Position is valid
					if( position >= alignment.cols()-1)
					{
						return false;
					}

					if(alignment.rows()<2)
					{
						return false;
					}
/*
					typedef seqan::String<seqan::AminoAcid> SSequence; //sequence type
					typedef seqan::Align<SSequence, seqan::ArrayGaps> SAlign; //align type
					typedef seqan::Row<SAlign>::Type SRow; //gapped sequence type
					typedef seqan::Iterator<SRow>::Type        TIterator; // Itersator for Seqan Alignment

					//Define Seqan alignment
					SAlign align;

					//resize align to number of sequences in alignment
					seqan::resize(seqan::rows(align), alignment.rows());

					//convert Sequences to SeqanSequences
					for (unsigned int i=0; i< alignment.rows(); i++)
					{
						//for every row in the alignment create a new Seqan Sequence
						SSequence seqa_seq;

						for(unsigned int j=position; j< alignment.cols(); j++)
						{
							if (!alignment.getSeqChar(i,j).isGap())
							{
								//append character stored in SequenceCharacter at current position in the alignment to Seqan Sequence
								seqan::appendValue(seqa_seq, alignment.getSeqChar(i,j).getChar());
							}
							else
							{
								//retrieve gapped Sequence
								SRow& row = seqan::row(align,i);
								//insert Gap at given Position
								seqan::insertGap(row,j);
							}
						}
						//put sequence into seqan-alignment
						seqan::assignSource(row(align,i),seqa_seq);
					}


					//compute actual alignment

					int score = 0;
					//decide whether to compute pairwise or multiple sequence alignments
					if(alignment.rows()==2)
					{
						//pairwise alignment with Smith- and Waterman Algorithm
						//TODO decide on good scoringfunction!
						score = seqan::globalAlignment(align,seqan::Score<int>(1,-1,-1,-1), seqan::NeedlemanWunsch());
						//alignment.setScore(score);
						cout<<"Score is: "<<score<<endl;
					}
					else
					{
						//compute global multiple alignment
						//TODO decide on scoringFunction
						//int score =	
						seqan::globalMsaAlignment(align, seqan::Blosum62(-1, -11));
						//				alignment.setScore(score);
					}

					//find the longest row in alignment
					unsigned int aliLength = 0;
					for(unsigned int i=0 ; i<seqan::length(seqan::rows(align)); i++)
					{
						if(seqan::length(row(align, i)) > aliLength)
						{
							aliLength = seqan::length(row(align,i));
						}
					}

					//initialize new alignmentMatrix, to store the newly computed alignment
					AlignmentMatrix newAlign;
					//resize newAlign to make room for alignment
					newAlign.resize(seqan::length(rows(align)), aliLength);

					//shift all Rows and Columns up to Position into newAlign
					for(int i=0; i< alignment.rows(); i++)
					{
						for (unsigned int j=0; j< position; j++)
						{
							newAlign(i,j) = alignment.getSeqChar(i,j);
						}
					}

					//Now shift seqan::alignment into newAlign beginning at position,
					//we need to know the old residue pointers, therefore we also need a counter at which position we are in the "old" alignment

					//iterate over all rows of seqan::alignment
					for(unsigned int i = 0; i < seqan::length(rows(align)); ++i)
					{
						//to retrieve all seqan::gaps (even the ones at the beginning/end of a row) we have to use the function iter

						//set seqan::row-iterator to start position
						TIterator it = seqan::iter(row(align,i), position);

						//set seqan::end-iterator to end position
						TIterator itEnd = seqan::iter(row(align,i), aliLength);

						//denotes positon in the current seqan::row and also the position in the newly created alignment
						unsigned int new_pos = position;

						//counter for old alignment
						unsigned int old_pos = position;

						//iterate over current row
						while (it != itEnd)
						{
							if(seqan::isGap(it))
							{
								//Check whether in old alignment is also a Gap at this position
								if(alignment.getSeqChar(i,old_pos).isGap())
								{
									//insert Gap into new Alignment
									newAlign(i,new_pos)=alignment.getSeqChar(i,old_pos);

									//increase old_pos
									++old_pos;
								}
								else
								{
									//insert new Gap into new Alignment
									newAlign(i,new_pos) = *(new SequenceCharacter('-', alignment.getAlignmentMatrix()(i,0).getOrigin(), SequenceCharacter::type::GAP, 0));
								}

							}

							else
							{
								//retrieve character from seqan::alignment
								char c = seqan::getValue(row(align,i),new_pos);				
								//retrieve old_sequence character from alignment and set it to new Position in new_align
								SequenceCharacter seq_c = alignment.getSeqChar(i,old_pos);
								if(c==seq_c.getChar())
								{
									newAlign(i,new_pos) = seq_c;
								}
								else
								{
									return false;
									//	throw Exception::InvalidArgument(__FILE__,__LINE__, "Error while shifting seqan::alignment back into BALL::alignment");
								}

								//set old_pos to the next character in old alignment

								//Check whether there are Gaps in old Alignment, if so just ignore them and increase old_pos
								while(alignment.getSeqChar(i,old_pos).isGap())
								{
									++old_pos;
								}
								//increase old_pos
								++old_pos;
							}

							//increase iterator
							++it;
							//increase new_pos
							++new_pos;
						}
					}

					//set alignment to new alignment
					alignment.setAlignmentMatrix(newAlign);

					//now that we are done also set the score
					alignment.setScore(score);
					*/
					//set aligned
					alignment.setAligned(true);

					return true;
				}

			
}//namespace BALL
