// $Id: RandomAccessIterator_test.C,v 1.4 2001/06/30 15:38:40 amoll Exp $

#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/CONCEPT/randomAccessIterator.h>
#include <vector>
///////////////////////////

using namespace BALL;
using namespace std;

typedef Index VectorIteratorPosition_;

template <typename DataType>
class VectorIteratorTraits_
{
	public:

	BALL_CREATE_DEEP(VectorIteratorTraits_)

	VectorIteratorTraits_()
		throw()
		: bound_(0),
			position_(0)
	{
	}
	
	VectorIteratorTraits_(const vector<DataType>& container)
		throw()
		: bound_((vector<DataType>*)& container),
			position_(0)
	{
	}
	
	VectorIteratorTraits_(const VectorIteratorTraits_& traits, bool /* deep */ = true)
		throw()
		: bound_(traits.bound_),
			position_(traits.position_)
	{
	}
	
	const VectorIteratorTraits_& operator = (const VectorIteratorTraits_& traits)
		throw()
	{
		bound_ = traits.bound_;
		position_ = traits.position_;
		return *this;
	}

	vector<DataType>* getContainer()
		throw()
	{
		return bound_;
	}

	const vector<DataType>* getContainer() const
		throw()
	{
		return bound_;
	}

	bool isSingular() const
		throw()
	{
		return (bound_ == 0);
	}

	VectorIteratorPosition_& getPosition()
		throw(Exception::InvalidIterator)
	{
		if (bound_ == 0)
		{
			throw(Exception::InvalidIterator(__FILE__, __LINE__));
		}
		return position_;
	}

	const VectorIteratorPosition_& getPosition() const
		throw(Exception::InvalidIterator)
	{
		if (bound_ == 0)
		{
			throw(Exception::InvalidIterator(__FILE__, __LINE__));
		}
		return position_;
	}

	bool operator == (const VectorIteratorTraits_& traits) const
		throw(Exception::InvalidIterator)
	{
		if (bound_ == 0)
		{
			throw(Exception::InvalidIterator(__FILE__, __LINE__));
		}
		return (position_ == traits.position_);
	}

	bool operator != (const VectorIteratorTraits_& traits) const
		throw(Exception::InvalidIterator)
	{
		if (bound_ == 0)
		{
			throw(Exception::InvalidIterator(__FILE__, __LINE__));
		}
		return (position_ != traits.position_);
	}
	
	bool operator < (const VectorIteratorTraits_& traits) const
		throw(Exception::InvalidIterator)
	{
		if (bound_ == 0)
		{
			throw(Exception::InvalidIterator(__FILE__, __LINE__));
		}
		return (position_ < traits.position_);
	}

	Distance getDistance(const VectorIteratorTraits_& traits) const
		throw(Exception::InvalidIterator)
	{
		if (bound_ == 0)
		{
			throw(Exception::InvalidIterator(__FILE__, __LINE__));
		}
		return (Distance)(position_ - traits.position_);
	}

	bool isValid() const
		throw()
	{
		return (bound_ != 0 && position_ >= 0 && position_ < (VectorIteratorPosition_)bound_->size());
	}

	void invalidate()
		throw()
	{
		bound_ = 0;
		position_ = 0;
	}

	void toBegin()
		throw(Exception::InvalidIterator)
	{
		if (bound_ == 0)
		{
			throw(Exception::InvalidIterator(__FILE__, __LINE__));
		}
		position_ = 0;
	}

	bool isBegin() const
		throw(Exception::InvalidIterator)
	{
		if (bound_ == 0)
		{
			throw(Exception::InvalidIterator(__FILE__, __LINE__));
		}
		return (position_ == 0);
	}

	void toEnd()
		throw(Exception::InvalidIterator)
	{
		if (bound_ == 0)
		{
			throw(Exception::InvalidIterator(__FILE__, __LINE__));
		}
		position_ = bound_->size();
	}

	bool isEnd() const
		throw(Exception::InvalidIterator)
	{
		if (bound_ == 0)
		{
			throw(Exception::InvalidIterator(__FILE__, __LINE__));
		}
		return (position_ >= (VectorIteratorPosition_)bound_->size());
	}

	DataType& getData()
		throw(Exception::InvalidIterator, Exception::IndexOverflow, Exception::IndexUnderflow)
	{
		if (bound_ == 0)
		{
			throw(Exception::InvalidIterator(__FILE__, __LINE__));
		}
		if (position_ >= (VectorIteratorPosition_)bound_->size())
		{
			throw(Exception::IndexOverflow(__FILE__, __LINE__, position_, bound_->size() - 1));
		}
		
		if (position_ < 0)
		{
			throw(Exception::IndexUnderflow(__FILE__, __LINE__, position_, 0));
		}

		return *(&bound_->operator [] (position_));
	}

	void forward()
		throw(Exception::InvalidIterator, Exception::IndexOverflow)
	{
		if (bound_ == 0)
		{
			throw(Exception::InvalidIterator(__FILE__, __LINE__));
		}

		//cout << "@" << position_ <<"@"<< bound_->size() << endl;
		if (position_ >= (Index) bound_->size())
		{
			throw(Exception::InvalidIterator(__FILE__, __LINE__));
			//throw(Exception::IndexOverflow(__FILE__, __LINE__, position_ + 1, bound_->size() - 1));
		}
		++position_;
	}

	friend std::ostream& operator << (std::ostream& s, const VectorIteratorTraits_& traits)
		throw(Exception::InvalidIterator)
	{
		if (traits.bound_ == 0)
		{
			throw(Exception::InvalidIterator(__FILE__, __LINE__));
		}
		return (s << traits.position_ << ' ');
	}

	void dump(std::ostream& s) const
		throw(Exception::InvalidIterator)
	{
		if (bound_ == 0)
		{
			throw(Exception::InvalidIterator(__FILE__, __LINE__));
		}
		s << position_ << std::endl;
	}

	void toRBegin()
		throw(Exception::InvalidIterator)
	{
		if (bound_ == 0)
		{
			throw(Exception::InvalidIterator(__FILE__, __LINE__));
		}
		position_ = bound_->size() - 1;
	}

	bool isRBegin() const
		throw(Exception::InvalidIterator)
	{
		if (bound_ == 0)
		{
			throw(Exception::InvalidIterator(__FILE__, __LINE__));
		}
		return (position_ == (VectorIteratorPosition_)(bound_->size() - 1));
	}
	
	void toREnd()
		throw(Exception::InvalidIterator)
	{
		if (bound_ == 0)
		{
			throw(Exception::InvalidIterator(__FILE__, __LINE__));
		}
		position_ = -1;
	}

	bool isREnd() const
		throw(Exception::InvalidIterator)
	{
		if (bound_ == 0)
		{
			throw(Exception::InvalidIterator(__FILE__, __LINE__));
		}
		return (position_ <= -1);
	}
	
	void backward()
		throw(Exception::InvalidIterator)
	{
		if (bound_ == 0)
		{
			throw(Exception::InvalidIterator(__FILE__, __LINE__));
		}

		if (position_ < 0)
		{
			throw(Exception::InvalidIterator(__FILE__, __LINE__));
		}
		
		--position_;
	}

	void backward(Distance distance)
		throw(Exception::InvalidIterator)
	{
		if (bound_ == 0)
		{
			throw(Exception::InvalidIterator(__FILE__, __LINE__));
		}

		if (position_ - distance < -1)
		{
			throw(Exception::InvalidIterator(__FILE__, __LINE__));
		}
		
		position_ -= distance;
	}

	void forward(Distance distance)
		throw(Exception::InvalidIterator)
	{
		if (bound_ == 0)
		{
			throw(Exception::InvalidIterator(__FILE__, __LINE__));
		}
	
		if (position_ + distance > (VectorIteratorPosition_)(bound_->size()))
		{
			throw(Exception::InvalidIterator(__FILE__, __LINE__));
		}
		
		position_ += distance;
	}

	DataType& getData(Index index)
		throw(Exception::InvalidIterator, Exception::IndexOverflow, Exception::IndexUnderflow)
	{
		if (bound_ == 0)
		{
			throw(Exception::InvalidIterator(__FILE__, __LINE__));
		}
		
		if (index >= (VectorIteratorPosition_)bound_->size())
		{
			throw(Exception::IndexOverflow(__FILE__, __LINE__, index, bound_->size() - 1));
		}
		
		if (index < 0)
		{
			throw(Exception::IndexUnderflow(__FILE__, __LINE__, index, 0));
		}


		return *(bound_->operator [] (index));
	}

	private:

	vector<DataType>*    		 bound_;
	VectorIteratorPosition_  position_;

};


class MyIterator
	: public RandomAccessIterator<vector<float>, float, VectorIteratorPosition_, VectorIteratorTraits_<float> >
{
  public:

	MyIterator()
	{
		RandomAccessIterator<vector<float>, float, VectorIteratorPosition_, VectorIteratorTraits_<float> >();
	}

	MyIterator(const vector<float>& v)
		: RandomAccessIterator<vector<float>, float, VectorIteratorPosition_, VectorIteratorTraits_<float> >(v)
	{
	}
};


START_TEST(class_name, "$Id: RandomAccessIterator_test.C,v 1.4 2001/06/30 15:38:40 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
{
MyIterator* m;

CHECK(RandomAccessIterator())
	m = new MyIterator;
	TEST_NOT_EQUAL(m, 0)
	TEST_EQUAL(m->isValid(), false)
RESULT

CHECK(~RandomAccessIterator())
 delete(m);
RESULT
}
vector<float> v;
v.push_back((float)0.1);
v.push_back((float)0.2);
v.push_back((float)0.3);
v.push_back((float)0.4);

CHECK(RandomAccessIterator(const Container& container))
	MyIterator m(v);
	TEST_EQUAL(m.isValid(), true)
	TEST_REAL_EQUAL(*m, 0.1)
RESULT

MyIterator m(v);
MyIterator n;

CHECK(Cpy cstr)
	MyIterator m2(m);
	TEST_REAL_EQUAL(*m2, 0.1)
RESULT

CHECK(Detailed cstr)
	//BAUSTELLE
RESULT

CHECK(operator +)
	TEST_EQUAL(+m, true)
	TEST_EQUAL(+n, false)
RESULT

CHECK(operator -)
	TEST_EQUAL(-m, false)
	TEST_EQUAL(-n, true)
RESULT

CHECK(toBegin)
	++m;
	TEST_REAL_EQUAL(*m, 0.2)
	m.toBegin();
	TEST_REAL_EQUAL(*m, 0.1)	

	TEST_EXCEPTION(Exception::InvalidIterator, n.toBegin())
RESULT

CHECK(isBegin)
	TEST_EQUAL(m.isBegin(), true)
	++m;
	TEST_EQUAL(m.isBegin(), false)
	TEST_EXCEPTION(Exception::InvalidIterator, n.isBegin())
RESULT

CHECK(toEnd)
	TEST_REAL_EQUAL(*m, 0.2)
	m.toEnd();
	TEST_EXCEPTION(Exception::InvalidIterator, *m)
	--m;
	TEST_REAL_EQUAL(*m, 0.4)	

	TEST_EXCEPTION(Exception::InvalidIterator, n.toEnd())
RESULT

CHECK(isEnd)
	m.toEnd();
	TEST_EQUAL(m.isEnd(), true)
	--m;
	TEST_EQUAL(m.isEnd(), false)
	TEST_EXCEPTION(Exception::InvalidIterator, n.isEnd())
RESULT

CHECK(toRBegin())
	m.toRBegin();
	TEST_REAL_EQUAL(*m, 0.4)

	TEST_EXCEPTION(Exception::InvalidIterator, n.toRBegin())
RESULT

CHECK(isRBegin)
	TEST_EQUAL(m.isRBegin(), true)
	m.toEnd();
	TEST_EQUAL(m.isRBegin(), false)

	TEST_EXCEPTION(Exception::InvalidIterator, n.isRBegin())
RESULT

CHECK(toREnd)
	m.toREnd();
	//BAUSTELLE
	TEST_EXCEPTION(Exception::InvalidIterator, *m)
	++m;
	TEST_REAL_EQUAL(*m, 0.1)
	
	TEST_EXCEPTION(Exception::InvalidIterator, n.toREnd())
RESULT

CHECK(isREnd)
	m.toREnd();
	TEST_EQUAL(m.isREnd(), true)
	m.toBegin();
	m++;
	TEST_EQUAL(m.isREnd(), false)
RESULT

CHECK(operator ++)
	m.toBegin();
	TEST_REAL_EQUAL(*m, 0.1)
	++m;
	TEST_REAL_EQUAL(*m, 0.2)

	m.toEnd();
	TEST_EXCEPTION(Exception::InvalidIterator, ++m)

	TEST_EXCEPTION(Exception::InvalidIterator, ++n)
RESULT

CHECK(operator ++ POSTFIX)
	m.toBegin();
	TEST_REAL_EQUAL(*m, 0.1)
	m++;
	TEST_REAL_EQUAL(*m, 0.2)

	m.toEnd();
	TEST_EXCEPTION(Exception::InvalidIterator, m++)

	TEST_EXCEPTION(Exception::InvalidIterator, n++)
RESULT


CHECK(operator --)
	m.toEnd();
	--m;
	TEST_REAL_EQUAL(*m, 0.4)

	m.toREnd();
	TEST_EXCEPTION(Exception::InvalidIterator, --m)

	TEST_EXCEPTION(Exception::InvalidIterator, --n)
RESULT


CHECK(operator -- POSTFIX)
	m.toEnd();
	m--;
	TEST_REAL_EQUAL(*m, 0.4)

	m.toREnd();
	TEST_EXCEPTION(Exception::InvalidIterator, m--)

	TEST_EXCEPTION(Exception::InvalidIterator, n--)
RESULT


CHECK(operator +=(distance))
	m.toBegin();
	m+=2;
	TEST_REAL_EQUAL(*m, 0.3)
	m+=2;
	TEST_EQUAL(m.isEnd(), true)
	TEST_EXCEPTION(Exception::InvalidIterator, m+=1)

	TEST_EXCEPTION(Exception::InvalidIterator, n+=1)
RESULT


CHECK(operator -=(distance))
	m.toEnd();
	m-=2;
	TEST_REAL_EQUAL(*m, 0.3)
	m-=3;
	TEST_EQUAL(m.isREnd(), true)
	TEST_EXCEPTION(Exception::InvalidIterator, m-=1)

	TEST_EXCEPTION(Exception::InvalidIterator, n-=1)
RESULT



END_TEST
