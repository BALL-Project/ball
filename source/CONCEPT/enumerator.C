// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: enumerator.C,v 1.14 2003/08/26 09:17:44 oliver Exp $
//

#include <BALL/COMMON/global.h>
#include <BALL/COMMON/exception.h>
#include <BALL/DATATYPE/string.h>
#include <BALL/CONCEPT/enumerator.h>

#include <algorithm>
#include <functional>

namespace BALL 
{

	EnumeratorIndex::IncompatibleIndex::IncompatibleIndex
		(const char* file, int line)
		: Exception::GeneralException(file, line, "IncompatibleIndex", "")
	{
		message_ = "different enumerator moduli occured.";

		Exception::globalHandler.setMessage(message_);
	}


	EnumeratorIndex::EnumeratorIndex()
		: vector<Position>(),
			modulus_(),
			base_multipliers_()
 	{
	}

	EnumeratorIndex::~EnumeratorIndex()
	{
	}


	EnumeratorIndex& EnumeratorIndex::operator ++ ()
	{
		Index i;
		bool add_one = true;
		for (i = (Index) size() - 1; (i >= 0) && add_one; i--)
		{
			operator[](i)++;
			add_one = false;
			if ((operator[](i)) >= modulus_[i])
			{
				operator[](i) = 0;
				add_one = true;
			}
		}

		if (add_one)
		{
			throw Exception::IndexOverflow(__FILE__, __LINE__);
		}

		return *this;
	}


	EnumeratorIndex& EnumeratorIndex::operator -- () 
	{
		Index i;
		bool sub_one = true;
		for (i = (Index)size() - 1; (i >= 0) && sub_one; i--)
		{
			if (operator[](i) == 0)
			{
				operator[](i) = modulus_[i] - 1;
			} 
			else 
			{
				operator[](i)--;
				sub_one = false;
			}
		}

		if (sub_one)
		{
			throw Exception::IndexUnderflow(__FILE__, __LINE__);
		}

		return *this;
	}

	const EnumeratorIndex& EnumeratorIndex::operator = (const EnumeratorIndex& rhs)
	{
		std::vector<Position>::operator = (rhs);
		modulus_ = rhs.modulus_;
		base_multipliers_ = rhs.base_multipliers_;
		
		return *this;
	}

	const EnumeratorIndex& EnumeratorIndex::operator = (Position index)
	{	
		for (Position i = 0; i < std::vector<Position>::size(); ++i)
		{
			Position digit = index / base_multipliers_[i]; 
			if (digit >= modulus_[i])
			{
				throw Exception::IndexOverflow(__FILE__, __LINE__, index);
			}
			std::vector<Position>::operator[](i) = digit;
			index -= base_multipliers_[i] * digit;
		}

		return *this;
	}

	EnumeratorIndex& EnumeratorIndex::operator << (Size modulus)
	{
		// there's no point in counting in the unary system or below
		if (modulus < 2)
		{
			throw Exception::OutOfRange(__FILE__, __LINE__);
		}

		// initialize the new digit
		std::vector<Position>::push_back(0);
	
		// update the modulus array
		modulus_.push_back(modulus);

		// transform the multipliers for each digit (by multiplying
    // the old contents with the new modulus and adding a one
    // for the new digit)
		std::transform(base_multipliers_.begin(), base_multipliers_.end(),
									 base_multipliers_.begin(),
									 std::bind2nd(std::multiplies<Position>(), modulus));
		base_multipliers_.push_back(1);		
		
		return *this;
	}

	bool EnumeratorIndex::operator == (const EnumeratorIndex& rhs) const
	{
		return (modulus_ == rhs.modulus_) && (static_cast<const vector<Position>&>(*this) == static_cast<const vector<Position>&>(rhs));
	}

	bool EnumeratorIndex::operator != (const EnumeratorIndex& rhs) const
	{
		return (modulus_ != rhs.modulus_) || (static_cast<const vector<Position>&>(*this) != static_cast<const vector<Position>&>(rhs));
	}

	bool EnumeratorIndex::operator < (const EnumeratorIndex& rhs) const
	{
		if (modulus_ != rhs.modulus_)
		{
			throw EnumeratorIndex::IncompatibleIndex(__FILE__, __LINE__);
		}

		return (static_cast<const vector<Position>&>(*this) < static_cast<const vector<Position>&>(rhs));
	}

	bool EnumeratorIndex::operator > (const EnumeratorIndex& rhs) const
	{
		if (modulus_ != rhs.modulus_)
		{
			throw EnumeratorIndex::IncompatibleIndex(__FILE__, __LINE__);
		}

		return (static_cast<const vector<Position>&>(*this) > static_cast<const vector<Position>&>(rhs));
	}

	bool EnumeratorIndex::operator <= (const EnumeratorIndex& rhs) const
	{
		if (modulus_  != rhs.modulus_)
		{
			throw EnumeratorIndex::IncompatibleIndex(__FILE__, __LINE__);
		}

		return (static_cast<const vector<Position>&>(*this) <= static_cast<const vector<Position>&>(rhs));
	}

	bool EnumeratorIndex::operator >= (const EnumeratorIndex& rhs) const
	{
		if (modulus_ != rhs.modulus_)
		{
			throw EnumeratorIndex::IncompatibleIndex(__FILE__, __LINE__);
		}

		return (static_cast<const vector<Position>&>(*this) >= static_cast<const vector<Position>&>(rhs));
	}
	

# ifdef BALL_NO_INLINE_FUNCTIONS
#   include <BALL/CONCEPT/enumerator.iC>
# endif

}
