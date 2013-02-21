// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/SEQUENCE/sequenceCharacter.h>
#include <BALL/SEQUENCE/sequence.h>

namespace BALL
{

	/////////////////////////////////////////// Constructors and Destructor ////////////////////////////////////////////////////

	/**
	 *Default Constructor
	 */
	SequenceCharacter::SequenceCharacter()
			:t_(type::CHAR)
	{
		origin_=0;
		character_=0;
		res_=0;
	}


	/**
	 *Detailed Constructor
	 *@param character_ the character of which comprises the new SequenceCharacter
	 *@param origin_ the sequence from which the character_ originates
	 *@param type tells whether the character_ is a GAP a nucleic acid or an amino acid
	 */
	SequenceCharacter::SequenceCharacter(char c, Sequence* ori, type ty, Residue* residue)
			:t_(ty),
			origin_(ori),
			character_(c),
			res_(residue)
	{  }


	////////////////////////////////////////////////////// Getter and Setter ///////////////////////////////////////////////////

	/**
	 *sets the type of the Character and if type is GAP char will be automatically setted to '-'
	 *@param type the new type to be set
	 *@throws BALL::InvalidArgument if character_ is a GAP and type is to be setted to CHAR
	 */
	void SequenceCharacter::setType(const SequenceCharacter::type ty)
	{
		//if type is to be setted to GAP the character_ will also be set to GAP
		if (ty == type::GAP)
		{
			character_ = '-';
			res_ = 0;
		}

		//if type is to be setted to CHAR and character_ is a GAP throw Exception
		if ( (character_ == '-') && (ty == type::CHAR ) )
		{
			throw Exception::InvalidArgument(__FILE__,__LINE__, "Don't set the type to Char manually just set the Char you want to change, the res_t will be done automatically");
		}

		//change the type
		t_=ty;
	}

	/**
	 *returns the type of the Character
	 *@return the type of the SequenceCharacter
	 */
	SequenceCharacter::type SequenceCharacter::getType() const
	{
		return t_;
	}

	/**
	 *@param character_ the character to be set
	 @throws InvalisArgumentException if Character does not repres_ent a valid AminoAcid or Nucleotide OneLetterCode
	 */
	void SequenceCharacter::setChar(const char c)
	{
		switch (c)
		{
			case '-':
			{
				t_ = type::GAP;
				character_ = c;
				res_= 0;
				return;
			}

			case 'B':
			case 'J':
			case 'O':
			case 'X':
			case 'Z':
			{
				throw Exception::InvalidArgument(__FILE__,__LINE__,"character_ is not a valid OneLetterCode");
			}
		}

		character_ = c;
		t_ = type::CHAR;
	}

	/**
	 *@return the stored character_
	 */
	char SequenceCharacter::getChar() const
	{
		return character_;
	}

	/**
	 *sets the Origin of the Character
	 *@param origin_ the origin to be set
	 */
	void SequenceCharacter::setOrigin(Sequence* ori)
	{
		origin_ = ori;
	}

	/**
	 *@return the origin_ of the character_
	 */
	Sequence* SequenceCharacter::getOrigin() const
	{
		return origin_;
	}


	void SequenceCharacter::setResidue(Residue* residue)
	{
		res_ = residue;
	}

	Residue* SequenceCharacter::getResidue() const
	{
		return res_;
	}



	//////////////////////////////////////////////////////////////////////////////// Operators ///////////////////////////////////////////////////
	/**
	 *operator ==
	 */
	bool SequenceCharacter::operator== (const SequenceCharacter& c)
	{
		return ( (character_== c.getChar()) && (origin_ == c.getOrigin()) && (t_ == c.getType()) );
	}

	/**
	 *operator !=
	 **/
	bool SequenceCharacter::operator!= (const SequenceCharacter& c)
	{
		return !(operator==(c));
	}

	SequenceCharacter& SequenceCharacter::operator = (const SequenceCharacter& c)
	{
		origin_ = (c.getOrigin());
		t_ = c.getType();
		character_=c.getChar();
		res_ = c.res_;

		return *this;
	}


	//////////////////////////////////////////////////////////////////// Misc //////////////////////////////////////////////////////////

	/**
	 *@return true if the Character is a Gap
	 * false else
	 */
	bool SequenceCharacter::isGap()
	{
		return t_ == type::GAP;
	}

	void SequenceCharacter::reset()
	{
		origin_    = NULL;
		character_ = 0;

		//TODO: what happens to the type? 
	}


}//namespace BALL

