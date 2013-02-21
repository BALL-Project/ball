// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/COMMON/exception.h>

#include <BALL/SEQUENCE/sequence.h>
#include <BALL/SEQUENCE/sequenceIterator.h>
#include <BALL/SEQUENCE/sequenceCharacter.h>

#include <BALL/STRUCTURE/peptideBuilder.h>
#include <BALL/STRUCTURE/fragmentDB.h>

namespace BALL
{

	// Default Constructor
	Sequence::Sequence()
	{
		name_= "";
		origin_ = 0;
	}

	// Detailed Constructor
	Sequence::Sequence(const String& name, AtomContainer* origin)
	 : name_(name),
		 origin_(origin)
	{
	}

	// Copy Constructor
	Sequence::Sequence(const Sequence& seq)
	{
		name_ = seq.getName();
		origin_ =  seq.getOrigin();
	}

	Sequence::Sequence(const String& name, const String& sequence_text)
	{
		name_ = name;

		/// we build a dummy peptide with Peptidebuilder


		// we need to define the peptid's AA sequence as a vector of descriptors
		vector<Peptides::AminoAcidDescriptor> descriptor_seq;

		for (unsigned int i=0; i < sequence_text.length(); i++)
		{
			char current_seq_char = sequence_text.c_str() [i];
			// handle gaps, .i.e. "?" and "UNK" are excluded
			if (   (current_seq_char != '-')
					&& (current_seq_char != '?'))
			{
				// each aminoacid is represented by a descriptor
				Peptides::AminoAcidDescriptor* aad = new Peptides::AminoAcidDescriptor;
				aad->setAminoAcidType(Peptides::ThreeLetterCode(current_seq_char));
				descriptor_seq.push_back(*aad);
			}
		}

		BALL::Peptides::PeptideBuilder* pb = new BALL::Peptides::PeptideBuilder(descriptor_seq);

		// "link" the fragment db for adding missing information
		FragmentDB fdb("");
		pb->setFragmentDB(&fdb);

		// give sensible names
		pb->setChainName("Chain A");
		pb->setProteinName(name);

		// now build a peptide
		origin_ = pb->construct();

	}

	// Destructor
	Sequence::~Sequence()
	{
	}

	void Sequence::setName(const BALL::String& name)
	{
		name_= name;
	}

	String Sequence::getName() const
	{
		return name_;
	}

	void Sequence::setOrigin(AtomContainer* origin)
	{
		origin_=origin;
	}

	AtomContainer* Sequence::getOrigin() const
	{
		return origin_;
	}

	bool Sequence::operator== (const Sequence& seq)
	{
		return ((name_==seq.getName()) && (origin_==seq.getOrigin()));
	}

	bool Sequence::operator!= (const Sequence& seq)
	{
		return !(operator==(seq));
	}

	Sequence& Sequence::operator= (const Sequence& seq)
	{
		name_   = seq.getName();
		origin_ = seq.getOrigin();

		return *this;
	}

	SequenceIterator Sequence::begin()
	{
		// Initialize the new Iterator
		SequenceIterator it;
		it.setSequence(*this);

		if (it.first())
		{
			return it;
		}
		return it;
	}

	SequenceIterator Sequence::end()
	{
		// initialize the new Iterator
		SequenceIterator it;

		// set the the sequence
		it.setSequence(*this);

		if (RTTI::isKindOf<Protein>(*origin_))
		{
			// cast origin_ to a protein
			Protein* protein = RTTI::castTo<Protein>(*origin_);

			int num_of_res = protein->countResidues();

			// set the counter to the number of residues in the protein
			it.setCounter(num_of_res);

			// to ensure comparing with another one  gives right decision we have to initiali
			SequenceCharacter c;
			c.setOrigin(this);
			it.setChar(c);
		}
		return it;
	}

	String Sequence::getStringSequence()
	{
		String* nseq = new String();
		SequenceIterator it;

		for (it = begin(); it != end(); it.next())
		{
			String* tmp = new String(it.getCharacter().getChar());

			*nseq += *tmp;
		}
		return *nseq;
	}


	Eigen::Array<SequenceCharacter, 1, Eigen::Dynamic> Sequence::getArraySequence()
	{
		// initialize the array
		Eigen::Array<SequenceCharacter, 1 , Eigen::Dynamic> seq(end().getCounter());

		int index=0;
		for(SequenceIterator it = begin(); it != end(); it.next(), index++)
		{
			// retrieve the current Sequencecharacter for each step
			SequenceCharacter c;
			c = it.getCharacter();

			// store it in the array
			seq(0, index) = c;
		}

		return seq;
	}

}//namespace BALL
