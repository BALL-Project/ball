// $Id: enumerator.C,v 1.7 2000/12/19 12:51:05 amoll Exp $

#include <BALL/COMMON/global.h>
#include <BALL/CONCEPT/enumerator.h>


namespace BALL 
{

	EnumeratorIndex::EnumeratorIndex()
		throw()
		: vector<Position>(),
			modulus_(),
			base_multipliers_()
 	{
	}

	EnumeratorIndex::~EnumeratorIndex()
		throw()
	{
	}

	EnumeratorIndex::IncompatibleIndex::IncompatibleIndex
		(const char* /* file */, int /* line */)
		throw()
	{
		// BAUSTELLE
	}


	EnumeratorIndex& EnumeratorIndex::operator ++ ()
		throw(Exception::IndexOverflow)
	{
		Index i;
		bool add_one = true;
		for (i = (Index)size() - 1; (i >= 0) && add_one; i--)
		{
			operator[](i)++;
			add_one = false;
			if (operator[](i) >= modulus_[i])
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
		throw(Exception::IndexUnderflow)
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


	void EnumeratorIndex::set(Position index)
		throw()
	{	
		for (Position i = 0; i < size(); ++i)
		{
			operator[](i) = index / base_multipliers_[i];
			index -= base_multipliers_[i] * operator[](i);
		}
	}


	bool operator == (const EnumeratorIndex& x, const EnumeratorIndex& y)
		throw(EnumeratorIndex::IncompatibleIndex)
	{
		if (x.getModulus() != y.getModulus())
		{
			throw EnumeratorIndex::IncompatibleIndex(__FILE__, __LINE__);
		}

		return (static_cast<const vector<Position>&>(x) == static_cast<const vector<Position>&>(y));
	}

	bool operator != (const EnumeratorIndex& x, const EnumeratorIndex& y)
		throw(EnumeratorIndex::IncompatibleIndex)
	{
		if (x.getModulus() != y.getModulus())
		{
			throw EnumeratorIndex::IncompatibleIndex(__FILE__, __LINE__);
		}

		return (static_cast<const vector<Position>&>(x) != static_cast<const vector<Position>&>(y));
	}

	bool operator < (const EnumeratorIndex& x, const EnumeratorIndex& y)
		throw(EnumeratorIndex::IncompatibleIndex)
	{
		if (x.getModulus() != y.getModulus())
		{
			throw EnumeratorIndex::IncompatibleIndex(__FILE__, __LINE__);
		}

		return (static_cast<const vector<Position>&>(x) < static_cast<const vector<Position>&>(y));
	}

	bool operator > (const EnumeratorIndex& x, const EnumeratorIndex& y)
		throw(EnumeratorIndex::IncompatibleIndex)
	{
		if (x.getModulus() != y.getModulus())
		{
			throw EnumeratorIndex::IncompatibleIndex(__FILE__, __LINE__);
		}

		return (static_cast<const vector<Position>&>(x) > static_cast<const vector<Position>&>(y));
	}

	bool operator <= (const EnumeratorIndex& x, const EnumeratorIndex& y)
		throw(EnumeratorIndex::IncompatibleIndex)
	{
		if (x.getModulus() != y.getModulus())
		{
			throw EnumeratorIndex::IncompatibleIndex(__FILE__, __LINE__);
		}

		return (static_cast<const vector<Position>&>(x) <= static_cast<const vector<Position>&>(y));
	}

	bool operator >= (const EnumeratorIndex& x, const EnumeratorIndex& y)
		throw(EnumeratorIndex::IncompatibleIndex)
	{
		if (x.getModulus() != y.getModulus())
		{
			throw EnumeratorIndex::IncompatibleIndex(__FILE__, __LINE__);
		}

		return (static_cast<const vector<Position>&>(x) >= static_cast<const vector<Position>&>(y));
	}
	

# ifdef BALL_NO_INLINE_FUNCTIONS
#   include <BALL/CONCEPT/enumerator.iC>
# endif

}
