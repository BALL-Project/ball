// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

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
	
	bool operator > (const VectorIteratorTraits_& traits) const
		throw(Exception::InvalidIterator)
	{
		if (bound_ == 0)
		{
			throw(Exception::InvalidIterator(__FILE__, __LINE__));
		}
		return (position_ > traits.position_);
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
		position_ = (Size)bound_->size();
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

	const DataType& getData() const
		throw(Exception::InvalidIterator)
	{
		if (bound_ == 0)
		{
			throw(Exception::InvalidIterator(__FILE__, __LINE__));
		}
		if (position_ >= (VectorIteratorPosition_)bound_->size())
		{
			throw(Exception::InvalidIterator(__FILE__, __LINE__));
		}
		
		if (position_ < 0)
		{
			throw(Exception::InvalidIterator(__FILE__, __LINE__));
		}

		return *(&bound_->operator [] (position_));
	}

	void forward()
		throw(Exception::InvalidIterator)
	{
		if (bound_ == 0)
		{
			throw(Exception::InvalidIterator(__FILE__, __LINE__));
		}

		if (position_ >= (Index) bound_->size())
		{
			throw(Exception::InvalidIterator(__FILE__, __LINE__));
		}
		++position_;
	}

	void toRBegin()
		throw(Exception::InvalidIterator)
	{
		if (bound_ == 0)
		{
			throw(Exception::InvalidIterator(__FILE__, __LINE__));
		}
		position_ = (Position)(bound_->size() - 1);
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

	const DataType& getData(Index index) const
		throw(Exception::InvalidIterator)
	{
		if (bound_ == 0)
		{
			throw(Exception::InvalidIterator(__FILE__, __LINE__));
		}
		
		if (index >= (VectorIteratorPosition_)bound_->size())
		{
			throw(Exception::InvalidIterator(__FILE__, __LINE__));
		}
		
		if (index < 0)
		{
			throw(Exception::InvalidIterator(__FILE__, __LINE__));
		}

		return bound_->operator [] (index);
	}

	private:

	vector<DataType>*    		 bound_;
	VectorIteratorPosition_  position_;
};

typedef ConstRandomAccessIterator<vector<float>, float, VectorIteratorPosition_, VectorIteratorTraits_<float> > MyIterator;
typedef reverse_iterator<ConstRandomAccessIterator<vector<float>, float, VectorIteratorPosition_, VectorIteratorTraits_<float> > > MyReverseIterator;

START_TEST(ConstRandomAccessIterator)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

MyIterator* m_ptr;

CHECK(ConstRandomAccessIterator() throw())
	m_ptr = new MyIterator;
	TEST_NOT_EQUAL(m_ptr, 0)
	TEST_EQUAL(m_ptr->isValid(), false)
RESULT

CHECK(~ConstRandomAccessIterator() throw())
 delete(m_ptr);
RESULT

vector<float> v;
v.push_back((float)0.1);
v.push_back((float)0.2);
v.push_back((float)0.3);
v.push_back((float)0.4);

MyIterator m(MyIterator::begin(v));
MyIterator n;

CHECK(ConstRandomAccessIterator(const ConstRandomAccessIterator& iterator) throw())
	MyIterator m2(m);
	TEST_REAL_EQUAL(*m2, 0.1)
RESULT

CHECK(bool operator + () const throw())
	TEST_EQUAL(+m, true)
	TEST_EQUAL(+n, false)
RESULT

CHECK(bool operator - () const throw())
	TEST_EQUAL(-m, false)
	TEST_EQUAL(-n, true)
RESULT

CHECK(ConstRandomAccessIterator& operator += (Distance distance) throw(Exception::InvalidIterator))
	m.toBegin();
	m+=2;
	TEST_REAL_EQUAL(*m, 0.3)
	m+=2;
	TEST_EQUAL(m.isEnd(), true)
	TEST_EXCEPTION(Exception::InvalidIterator, m += 1)
	TEST_EXCEPTION(Exception::InvalidIterator, n += 1)

	MyIterator my;
	TEST_EXCEPTION(Exception::InvalidIterator, my += 1)
RESULT


CHECK(ConstRandomAccessIterator& operator -= (Distance distance) throw(Exception::InvalidIterator))
	m.toEnd();
	m-=2;
	TEST_REAL_EQUAL(*m, 0.3)
	m-=3;
	TEST_EQUAL(m.isREnd(), true)
	TEST_EXCEPTION(Exception::InvalidIterator, m-=1)
	TEST_EXCEPTION(Exception::InvalidIterator, n-=1)

	MyIterator my;
	TEST_EXCEPTION(Exception::InvalidIterator, my -= 1)
RESULT


CHECK(ConstRandomAccessIterator operator + (Distance distance) const throw(Exception::InvalidIterator))
	MyIterator p;
	m.toBegin();
	p = m + 2;
	TEST_REAL_EQUAL(*p, 0.3)
	p = m + 4;
	TEST_EQUAL(p.isEnd(), true)
	TEST_EXCEPTION(Exception::InvalidIterator, p + 1)
	TEST_EXCEPTION(Exception::InvalidIterator, n + 1)

	MyIterator my;
	TEST_EXCEPTION(Exception::InvalidIterator, my + 1)
RESULT


CHECK(ConstRandomAccessIterator operator - (Distance distance) const throw(Exception::InvalidIterator))
	MyIterator p;
	m.toEnd();
	p = m - 2;
	TEST_REAL_EQUAL(*p, 0.3)
	p = m - 5;
	TEST_EQUAL(p.isREnd(), true)
	TEST_EXCEPTION(Exception::InvalidIterator, p - 1)
	TEST_EXCEPTION(Exception::InvalidIterator, n - 1)

	MyIterator my;
	TEST_EXCEPTION(Exception::InvalidIterator, my - 1)
RESULT

vector<float> v1;
v1.push_back((float)99.9);

CHECK(Distance operator - (const ConstRandomAccessIterator& iterator) const throw(Exception::InvalidIterator, Exception::IncompatibleIterators))
	TEST_EXCEPTION(Exception::InvalidIterator, m - n)
	TEST_EXCEPTION(Exception::InvalidIterator, n - m)
	MyIterator m1(MyIterator::begin(v1));
	m.toBegin();
	TEST_EXCEPTION(Exception::IncompatibleIterators, m - m1)
	m1 = MyIterator::end(v);
	m1--;
	TEST_EQUAL(m - m1, -3)
	TEST_EQUAL(m1 - m, 3)

	MyIterator my1;
	MyIterator my2;
	TEST_EXCEPTION(Exception::InvalidIterator, my1 - my2)
RESULT


CHECK(bool operator < (const ConstRandomAccessIterator& iterator) const throw(Exception::InvalidIterator, Exception::IncompatibleIterators))
	MyIterator m1(MyIterator::begin(v1));
	m1.toBegin();
	m.toBegin();
	TEST_EXCEPTION(Exception::IncompatibleIterators, m1 < m)
	TEST_EXCEPTION(Exception::IncompatibleIterators, m < m1)
	TEST_EXCEPTION(Exception::InvalidIterator, n < m)
	
	m1 = MyIterator(MyIterator::begin(v));
	m.toBegin();
	TEST_REAL_EQUAL(*m1, 0.1)
	TEST_REAL_EQUAL(*m, 0.1)
	TEST_EQUAL(m1 < m, false)
	m++; 
	TEST_EQUAL(m < m1, false)
	TEST_EQUAL(m1 < m, true)
RESULT


CHECK(bool operator <= (const ConstRandomAccessIterator& iterator) const throw(Exception::InvalidIterator, Exception::IncompatibleIterators))
	MyIterator m1(MyIterator::begin(v1));
	m.toBegin();
	TEST_EXCEPTION(Exception::IncompatibleIterators, m1 <= m)
	TEST_EXCEPTION(Exception::IncompatibleIterators, m <= m1)
	TEST_EXCEPTION(Exception::InvalidIterator, n <= m)
	
	m1 = MyIterator(MyIterator::begin(v));
	m.toBegin();
	TEST_EQUAL(m1 <= m, true)
	TEST_EQUAL(m <= m1, true)
	m++; 
	
	TEST_REAL_EQUAL(*m, 0.2)
	TEST_REAL_EQUAL(*m1, 0.1)

	TEST_EQUAL(m <= m1, false)
	TEST_EQUAL(m1 <= m, true)
RESULT


CHECK(bool operator > (const ConstRandomAccessIterator& iterator) const throw(Exception::InvalidIterator, Exception::IncompatibleIterators))
	MyIterator m1(MyIterator::begin(v1));
	m.toBegin();
	TEST_EXCEPTION(Exception::IncompatibleIterators, m1 > m)
	TEST_EXCEPTION(Exception::IncompatibleIterators, m > m1)
	TEST_EXCEPTION(Exception::InvalidIterator, n > m)
	
	m1 = MyIterator(MyIterator::begin(v));
	m.toBegin();
	TEST_EQUAL(m > m1, false)
	m++; 
	TEST_EQUAL(m1 > m, false)
	TEST_EQUAL(m > m1, true)
RESULT


CHECK(bool operator >= (const ConstRandomAccessIterator& iterator) const throw(Exception::InvalidIterator, Exception::IncompatibleIterators))
	MyIterator m1(MyIterator::begin(v1));
	m.toBegin();
	TEST_EXCEPTION(Exception::IncompatibleIterators, m1 >= m)
	TEST_EXCEPTION(Exception::IncompatibleIterators, m >= m1)
	TEST_EXCEPTION(Exception::InvalidIterator, n >= m)
	
	m1 = MyIterator(MyIterator::begin(v));
	m.toBegin();
	TEST_EQUAL(m1 >= m, true)
	TEST_EQUAL(m >= m1, true)
	m++; 
	TEST_EQUAL(m1 >= m, false)
	TEST_EQUAL(m >= m1, true)
RESULT


CHECK(const DataType& operator [] (Index index) const throw(Exception::InvalidIterator))
	TEST_REAL_EQUAL(m[2], (float) 0.3)
	TEST_EXCEPTION(Exception::InvalidIterator, m[6])
	MyIterator m2;
	TEST_EXCEPTION(Exception::InvalidIterator, m2[2])
RESULT


CHECK(static ConstRandomAccessIterator begin(const Container& container) throw(Exception::InvalidIterator))
	TEST_REAL_EQUAL(*MyIterator::begin(v), 0.1)
RESULT


CHECK(static ConstRandomAccessIterator end(const Container& container) throw(Exception::InvalidIterator))
	MyIterator m1(MyIterator::end(v));
	m1--;
	TEST_REAL_EQUAL(*m1, 0.4)
RESULT


CHECK(static ConstRandomAccessIterator rbegin(const Container& container) throw(Exception::InvalidIterator))
	MyIterator it(MyIterator::rbegin(v));
	TEST_EQUAL(it.isValid(), true)
	TEST_REAL_EQUAL(*it, 0.4)
RESULT


CHECK(static ConstRandomAccessIterator rend(const Container& container) throw(Exception::InvalidIterator))
  MyIterator m1(MyIterator::rend(v));
	TEST_EQUAL(m1.isValid(), false)
RESULT

END_TEST

