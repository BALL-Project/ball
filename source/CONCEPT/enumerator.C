// $Id: enumerator.C,v 1.3 2000/06/16 08:33:25 oliver Exp $

#include <BALL/COMMON/global.h>
#include <BALL/CONCEPT/enumerator.h>


namespace BALL 
{

	EnumeratorIndex::IncompatibleIndex::IncompatibleIndex(const char* file, int line)
	{
		// BAUSTELLE
		Log.error() << "BAEH!" << endl;
	}


	EnumeratorIndex& EnumeratorIndex::operator ++ ()
	{
		Index i;
		bool add_one = true;
		for (i = size() - 1; (i >= 0) && add_one; i--)
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
	{
		Index i;
		bool sub_one = true;
		for (i = size() - 1; (i >= 0) && sub_one; i--)
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
	{	
		for (Position i = 0; i < size(); ++i)
		{
			operator[](i) = index / base_multipliers_[i];
			index -= base_multipliers_[i] * operator[](i);
		}
	}


	bool operator == (const EnumeratorIndex& x, const EnumeratorIndex& y)
	{
		if (x.getModulus() != y.getModulus())
		{
			throw EnumeratorIndex::IncompatibleIndex(__FILE__, __LINE__);
		}

		return (static_cast<const vector<Position>&>(x) == static_cast<const vector<Position>&>(y));
	}

	bool operator != (const EnumeratorIndex& x, const EnumeratorIndex& y)
	{
		if (x.getModulus() != y.getModulus())
		{
			throw EnumeratorIndex::IncompatibleIndex(__FILE__, __LINE__);
		}

		return (static_cast<const vector<Position>&>(x) != static_cast<const vector<Position>&>(y));
	}

	bool operator < (const EnumeratorIndex& x, const EnumeratorIndex& y)
	{
		if (x.getModulus() != y.getModulus())
		{
			throw EnumeratorIndex::IncompatibleIndex(__FILE__, __LINE__);
		}

		return (static_cast<const vector<Position>&>(x) < static_cast<const vector<Position>&>(y));
	}

	bool operator > (const EnumeratorIndex& x, const EnumeratorIndex& y)
	{
		if (x.getModulus() != y.getModulus())
		{
			throw EnumeratorIndex::IncompatibleIndex(__FILE__, __LINE__);
		}

		return (static_cast<const vector<Position>&>(x) > static_cast<const vector<Position>&>(y));
	}

	bool operator <= (const EnumeratorIndex& x, const EnumeratorIndex& y)
	{
		if (x.getModulus() != y.getModulus())
		{
			throw EnumeratorIndex::IncompatibleIndex(__FILE__, __LINE__);
		}

		return (static_cast<const vector<Position>&>(x) <= static_cast<const vector<Position>&>(y));
	}

	bool operator >= (const EnumeratorIndex& x, const EnumeratorIndex& y)
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
