// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include<BALL/SEQUENCE/alignment.h>
#include<BALL/SEQUENCE/alignAlgorithm.h>
#include<BALL/SEQUENCE/scoringFunction.h>

namespace BALL
{

	Alignment::Alignment()
			: alignment_(),
			score_(0),
			is_aligned_(false)
	{ }

	Alignment::Alignment(const AlignmentMatrix & alignmentmatrix, const double score, const bool is_aligned)
			: alignment_(alignmentmatrix),
			score_(score),
			is_aligned_(is_aligned)
	{ }

	double Alignment::getScore() const
	{
		return score_;
	}

	AlignmentMatrix Alignment::getAlignmentMatrix() const
	{
		return alignment_;
	}

	bool Alignment::isAligned() const
	{
		return is_aligned_;
	}

	SequenceCharacter Alignment::getSeqChar(unsigned int row, unsigned int column) const
	{
		if ((row > alignment_.rows())|| column > alignment_.cols())
		{
			throw BALL::Exception::IndexOverflow(__FILE__,__LINE__);
		}
		return alignment_(row,column);
	}

	String Alignment::getStringSequence(unsigned int  row) const
	{
		String sequence;

		for (int i=0; i<alignment_.cols(); i++)
		{
			sequence+=alignment_(row,i).getChar();
		}
		return sequence;
	}

	unsigned int Alignment::cols() const
	{
		return alignment_.cols();
	}

	unsigned int Alignment::rows() const
	{
		return alignment_.rows();
	}

	void Alignment::setScore(double score)
	{
		score_=score;
	}

	bool Alignment::empty() const
	{
		return ((alignment_.rows()== 0) || (alignment_.cols()== 0));
	}

	void Alignment::setAligned(bool aligned)
	{
		is_aligned_=aligned;
	}

	void Alignment::setAlignmentMatrix(AlignmentMatrix& align_matrix)
	{
		alignment_=align_matrix;
	}

	bool Alignment::insertGap(Sequence& seq, int pos)
	{
		//pointer to the sequence stored in the alignment
		Sequence* ori = 0;

		//denotes the row in which the sequence is found
		int row = 0;

		//search for the sequence in the alignment
		while ((ori == 0) && (row < alignment_.rows()))
		{
			if (seq == *(alignment_(row,0).getOrigin()))
			{
				ori = alignment_(row,0).getOrigin();
			}
			else
			{
				row++;
			}
		}

		//if sequence was not found return false
		if (ori == 0 )
		{
			return false;
		}

		//check whether we have to resize the matrix or not
		if (alignment_(row,alignment_.cols()-1).getType()!=SequenceCharacter::type::GAP)
		{
			//store old number of columns
			int old_size = alignment_.cols();

			//resize matrix to add a column at the end
			alignment_.conservativeResize(Eigen::NoChange,alignment_.cols()+1);

			//iterate over all newly created columns and fill them with Gaps
			for (int i=0; i<alignment_.rows(); i++)
			{
				for (int j=old_size; j<alignment_.cols(); j++)
				{
					alignment_(i,j)= *(new SequenceCharacter('-', alignment_(i,0).getOrigin(), SequenceCharacter::type::GAP, 0));

				}

			}
		}

		//iterate over the columns to change the Sequence and insert the Gap
		for (int j=alignment_.cols()-1; j> pos ; j--)
		{
			alignment_(row,j)=alignment_(row,j-1);
		}

		//insert Gap at right position
		alignment_(row,pos) = *(new SequenceCharacter( '-', &seq , SequenceCharacter::type::GAP, 0 ));

		return true;
	}


	bool Alignment::insertGap(int row, int column)
	{
		//check the given indices
		if (row >= alignment_.rows() || column > alignment_.cols())
		{
			return false;
		}

		//check whether we have to resize the matrix or not
		if (alignment_(row,alignment_.cols()-1).getType()!=SequenceCharacter::type::GAP)
		{
			//store old number of columns
			int old_size = alignment_.cols();

			//resize matrix to add a column at the end
			alignment_.conservativeResize(Eigen::NoChange,alignment_.cols()+1);

			//iterate over all newly created columns and fill them with Gaps
			for (int i=0; i<alignment_.rows(); i++)
			{
				for (int j=old_size; j<alignment_.cols(); j++)
				{
					alignment_(i,j)= *(new SequenceCharacter('-', alignment_(i,0).getOrigin(), SequenceCharacter::type::GAP, 0));
				}
			}
		}

		//iterate over columns of alignment to make room for the new character
		for (int j=alignment_.cols()-1; j>column ; j--)
		{
			alignment_(row,j)=alignment_(row,j-1);
		}

		//insert GAP at the right position
		alignment_(row,column) = *(new SequenceCharacter('-',alignment_(row,column).getOrigin(), SequenceCharacter::type::GAP, new Residue ()));
		return true;
	}


	bool Alignment::deleteGap(Sequence& seq, int pos)
	{
		//iterate over all rows to detect the Sequence
		for (int i=0; i<alignment_.rows(); i++)
		{
			Sequence* ori = alignment_(i,0).getOrigin();
			if (seq == *ori)
			{
				//check whether there is really a Gap at the given Position
				if ( alignment_(i,pos).getType() != SequenceCharacter::type::GAP )
				{
					return false;
				}

				//iterate over the columns to change the Sequence and delete the Gap
				for (int j= pos; j < alignment_.cols()-1; j++)
				{
					alignment_(i,j) = alignment_(i,j+1);
				}

				//if there is already a Gap at the end of the sequence return
				if (alignment_(i,alignment_.cols()-1).getType() == SequenceCharacter::GAP)
				{
					return true;
				}

				//if there isn't a Gap, insert one and return
				alignment_(i,alignment_.cols()-1) = *(new SequenceCharacter( '-', &seq , SequenceCharacter::type::GAP,0 ));
				return true;
			}
		}

		//if the sequence wasn't found return false
		return false;
	}


	bool Alignment::deleteGap(int row, int column)
	{
		//check the given indices
		if (row >= alignment_.rows() || column > alignment_.cols())
		{
			return false;
		}

		//check whether there is a Gap at the given Position
		if (alignment_(row,column).getType() != SequenceCharacter::type::GAP )
		{
			return false;
		}

		//iterate over columns of alignment shifting the remaining characters to the previous position
		for (int j=column; j < alignment_.cols()-1 ; j++)
		{
			alignment_(row,j) = alignment_(row,j+1);
		}

		//if there is already a Gap at the end of the sequence return
		if (alignment_(row,alignment_.cols()-1).getType() == SequenceCharacter::GAP)
		{
			return true;
		}

		//if there isn't a Gap, insert one and return
		alignment_(row,alignment_.cols()-1) = *(new SequenceCharacter( '-', alignment_(row,1).getOrigin() , SequenceCharacter::type::GAP, 0 ));

		return true;
	}


	void Alignment::addSequence(Sequence& seq)
	{
		//retrieve the sequence represented by SequenceCharacters
		Eigen::Array<SequenceCharacter, 1, Eigen::Dynamic> real_seq = seq.getArraySequence();

		//make room for the new sequence
		//store the number of cols before resizing the matrix
		int old_size = alignment_.cols();

		//Check whether we need to add some columns to the alignment, because the sequence is longer than all others in the alignment
		if (alignment_.cols()<real_seq.cols())
		{
			//resize matrix (add one row, add as many columns as needed)
			alignment_.conservativeResize(alignment_.rows()+1, real_seq.cols());

			//copy sequence to last row
			for (int i=0; i<alignment_.cols(); i++)
			{
				alignment_(alignment_.rows()-1, i ) = real_seq(0,i);
			}

			//iterate over all rows except the last one (since there we added the new Sequence) and initialize the empty columns with gaps
			for (int i=0; i< alignment_.rows()-1; i++)
			{
				for (int j= old_size; j< alignment_.cols(); j++)
				{
					alignment_(i,j)= *(new SequenceCharacter('-', alignment_(i,0).getOrigin(), SequenceCharacter::type::GAP, 0));
				}
			}
		}
		//sequence is either of eqal or smaller length than the ones in the alignment
		else
		{
			alignment_.conservativeResize(alignment_.rows()+1, Eigen::NoChange);

			for (int i=0; i<alignment_.cols(); i++)
			{
				//copy sequence
				if (i<real_seq.cols())
				{
					alignment_(alignment_.rows()-1, i ) = real_seq(0,i);
				}
				else
				{
					//initialize empty columns with gaps
					alignment_(alignment_.rows()-1, i) = * (new SequenceCharacter('-', alignment_(alignment_.rows()-1,0).getOrigin(), SequenceCharacter::type::GAP, 0));
				}
			}
		}
	}


	bool Alignment::deleteSequence(Sequence& seq)
	{
		// pointer to the Sequence that is stored in the alignment
		Sequence* ori = 0;

		//denote the row where the Sequence in the alignment has been found
		int row = 0;

		//search for the sequence
		while ( (ori == 0) && (row< alignment_.rows()))
		{
			if (seq == *(alignment_(row,0).getOrigin()))
			{
				ori = alignment_(row, 0).getOrigin();
			}
			else
			{
				row++;
			}
		}

		//If sequence was not found in the alignment return false
		if (ori == 0)
		{
			return false;
		}

		//Copy all lines to the previous one starting at the position of the sequence to be deleted
		for (int i=row; i<alignment_.rows()-1; i++)
		{
			alignment_(i)=alignment_(i+1);
		}

		//resize matrix
		alignment_.conservativeResize(alignment_.rows()-1, Eigen::NoChange);

		return true;
	}


	bool Alignment::deleteSequence(int row)
	{
		if (row >= alignment_.rows())
		{
			return false;
		}

		//just copy the current line into the previous line
		for (int i=row; i<alignment_.rows()-1; i++)
		{
			alignment_(i)=alignment_(i+1);
		}

		//resize the alignment
		alignment_.conservativeResize(alignment_.rows()-1,Eigen::NoChange);

		return true;
	}


	bool Alignment::insertSeqChar(Sequence& seq, SequenceCharacter& c, int pos)
	{
		//Check whether SequenceCharacter has same orign as the seq where it is to be inserted
		if ( *(c.getOrigin()) != seq)
		{
			return false;
		}

		// pointer to the Sequence that is stored in the alignment
		Sequence* ori = 0;

		//denote the row where the Sequence in the alignment has been found
		int row = 0;

		//search for the sequence
		while ( (ori == 0) && (row< alignment_.rows()))
		{
			if (seq == *(alignment_(row,0).getOrigin()))
			{
				ori = alignment_(row, 0).getOrigin();
			}
			else
			{
				row++;
			}
		}

		// If sequence was not found in the alignment return false
		if (ori == 0)
		{
			return false;
		}
		else
		{
			//check whether we have to make room for the new Char or whether there is a Gap
			if (alignment_(row,pos).getType() == SequenceCharacter::type::GAP)
			{
				alignment_(row,pos) = c;
				return true;
			}

			//check whether we have to resize the matrix
			if (alignment_ (row,alignment_.cols()-1).getType() != SequenceCharacter::type::GAP)
			{
				int old_size= alignment_.cols();

				alignment_.conservativeResize(Eigen::NoChange, alignment_.cols()+1);

				//iterate over all rows and initialize the empty columns with gaps
				for (int i=0; i< alignment_.rows(); i++)
				{
					for (int j= old_size; j< alignment_.cols(); j++)
					{
						alignment_(i,j)= *(new SequenceCharacter('-', alignment_(i,0).getOrigin(), SequenceCharacter::type::GAP, 0));
					}
				}
			}

			//shift every character just one position further right
			for (int j=alignment_.cols()-1; j>pos; j--)
			{
				alignment_(row,j)= alignment_(row,j-1);
			}

			//set the seqchar
			alignment_(row,pos)= c;
			return true;
		}
	}


	bool Alignment::insertSeqChar(int row, int column, SequenceCharacter& c)
	{
		//check the inidices
		if (row>alignment_.rows() || column > alignment_.cols())
		{
			return false;
		}

		//check if SequenceCharacter has the same origin, as the Sequence it is to be inserted
		if (c.getOrigin() != alignment_(row, column).getOrigin() )
		{
			return false;
		}

		//check whether we have to make room for the new Char or whether there is a Gap
		if (alignment_(row,column).getType() == SequenceCharacter::type::GAP)
		{
			alignment_(row,column) = c;
			return true;
		}

		//check whether we have to resize the matrix
		if (alignment_ (row,alignment_.cols()-1).getType() != SequenceCharacter::type::GAP)
		{
			int old_size= alignment_.cols();

			alignment_.conservativeResize(Eigen::NoChange, alignment_.cols()+1);

			//iterate over all rows and initialize the empty columns with gaps
			for (int i=0; i< alignment_.rows(); i++)
			{
				for (int j= old_size; j< alignment_.cols(); j++)
				{
					alignment_(i,j)= *(new SequenceCharacter('-', alignment_(i,0).getOrigin(), SequenceCharacter::type::GAP, 0));
				}
			}
		}

		//shift every character just one position further right
		for (int j=alignment_.cols()-1; j>column; j--)
		{
			alignment_(row,j)= alignment_(row,j-1);
		}

		//set the seqchar
		alignment_(row,column) = c;
		return true;
	}


	void Alignment::reset()
	{
		//TODO CHECK whether this works!!
		alignment_.resize(0,0);
		score_=0;
		is_aligned_=false;
	}


	Sequence& Alignment::getSequence(int row) const
	{
		//retrieve Character at given row an fetch its origin
		return *(alignment_(row,0).getOrigin());
	}


	void Alignment::read(System& system)
	{
		//TODO what if System does not contain Proteins but Molecules etc...

		for (ProteinConstIterator it = system.beginProtein(); +it; ++it)
		{
			//create new Sequence
			Sequence* s = new Sequence();
			s->setName(it->getName());

			Protein p = *it;
			s->setOrigin( &p);

			//add it to the alignment
			addSequence(*s);
		}
	}


	void Alignment::read(Protein& protein)
	{
		//create a new Sequence
		Sequence* s = new Sequence();
		s->setName(protein.getName());
		s->setOrigin(&protein);

		//add it to the alignment
		addSequence(*s);
	}


	void Alignment::dump (std::ostream& s) const
	{
		s << "The alignment contains " << alignment_.rows()<<" Sequences. These look as follows:"<< endl;

		for (int i=0; i<alignment_.rows(); i++)
		{
			s << "The " << i << "th Sequence is: ";

			for (int j=0; j<alignment_.cols(); j++)
			{
				s << alignment_(i,j).getChar();
			}
			s << endl;
		}

		s << "The score is: " << score_<<endl;

		if (is_aligned_)
		{
			s << "The alignment has been correctly aligned." << endl;
		}
		else
		{
			s << "The alignment has not yet been aligned." << endl;
		}
	}


	bool Alignment::deleteGapColumn(unsigned int pos)
	{
		//CHECK whether there are only Gaps in the Column to be deleted
		for (int i=0; i<alignment_.rows(); i++)
		{
			if (!alignment_(i,pos).isGap())
			{
				return false;
			}
		}

		//iterate over all rows and shift the following characters one position to the left
		for (int i=0; i<alignment_.rows(); i++)
		{
			for (int j=pos; j < alignment_.cols()-1 ; j++)
			{
				alignment_(i,j) = alignment_(i,j+1);
			}
		}

		//resize matrix to delete the last row
		alignment_.conservativeResize(Eigen::NoChange, alignment_.cols()-1);

		return true;
	}


}//namespace BALL
