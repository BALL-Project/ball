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

	void bindTo(vector<DataType>& container) throw() { bound_ = &container; }

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

	DataType& getData()
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

	DataType& getData(Index index)
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

typedef BaseIterator<vector<float>, float, VectorIteratorPosition_, VectorIteratorTraits_<float> > MyIterator;

START_TEST(BaseIterator)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

MyIterator* m_ptr;


CHECK(BaseIterator() throw())
	m_ptr = new MyIterator;
	TEST_NOT_EQUAL(m_ptr, 0)
	TEST_EQUAL(m_ptr->isValid(), false)
RESULT

CHECK(~BaseIterator() throw())
 delete(m_ptr);
RESULT


vector<float> v;
v.push_back((float)0.1);
v.push_back((float)0.2);
v.push_back((float)0.3);
v.push_back((float)0.4);

MyIterator m;
m.getTraits().bindTo(v);
MyIterator n;

CHECK(BaseIterator& operator = (const BaseIterator<Container, DataType, Position, Traits>& iterator) throw())
  // ???
RESULT

CHECK(BaseIterator(const BaseIterator& iterator) throw())
  // ???
RESULT

CHECK(Traits& getTraits() throw())
  // ???
RESULT

CHECK(const Traits& getTraits() const throw())
	// ???
RESULT

CHECK(bool isSingular() const throw())
	MyIterator i1;
	TEST_EQUAL(i1.isSingular(), true)
	i1.getTraits().bindTo(v);
	TEST_EQUAL(i1.isSingular(), false)
	i1.invalidate();
	TEST_EQUAL(i1.isSingular(), true)
RESULT

CHECK(bool isValid() const throw())
	MyIterator i1;
	TEST_EQUAL(i1.isValid(), false)
	i1.getTraits().bindTo(v);
	TEST_EQUAL(i1.isValid(), true)
	i1.invalidate();
	TEST_EQUAL(i1.isValid(), false)
RESULT

CHECK(bool operator != (const BaseIterator& iterator) const throw())
  // ???
RESULT

CHECK(bool operator + () const throw())
	MyIterator i1;
	TEST_EQUAL(+i1, false)
	i1.getTraits().bindTo(v);
	TEST_EQUAL(+i1, true)
	i1.invalidate();
	TEST_EQUAL(+i1, false)
RESULT

CHECK(bool operator - () const throw())
	MyIterator i1;
	TEST_EQUAL(-i1, true)
	i1.getTraits().bindTo(v);
	TEST_EQUAL(-i1, false)
	i1.invalidate();
	TEST_EQUAL(-i1, true)
RESULT

CHECK(bool operator == (const BaseIterator& iterator) const throw())
  // ???
RESULT

CHECK(const Container* getContainer() const throw())
	MyIterator i1;
	TEST_EQUAL(i1.getContainer(), 0)
	
	MyIterator i2;
	i2.getTraits().bindTo(v);
	TEST_EQUAL(i2.getContainer(), &v)
RESULT

CHECK(operator const Position& () const throw())
  // ???
RESULT

CHECK(pointer operator -> () const throw())
	MyIterator i1;
	i1.getTraits().bindTo(v);
	TEST_EQUAL(*(i1.operator -> ()), v[0])
RESULT

CHECK(reference operator * () const throw())
	MyIterator i1;
	i1.getTraits().bindTo(v);
	TEST_EQUAL(*i1, v[0])
RESULT

CHECK(void invalidate() throw())
	MyIterator i1;
	i1.getTraits().bindTo(v);
	TEST_EQUAL(i1.isValid(), true)
	TEST_EQUAL(i1.getContainer(), &v)
	i1.invalidate();
	TEST_EQUAL(i1.isValid(), false)
	TEST_EQUAL(i1.getContainer(), 0)
RESULT

CHECK(void setTraits(const Traits& traits) throw())
	MyIterator i1;
	i1.getTraits().bindTo(v);
	MyIterator i2;
	TEST_EQUAL(i2.getTraits().getContainer(), 0)
	i2.setTraits(i1.getTraits());
	TEST_EQUAL(i2.getTraits().getContainer(), &v)

	// Make sure we got our own copy!
	i2.invalidate();
	TEST_EQUAL(i2.getTraits().getContainer(), 0)
	TEST_EQUAL(i1.getTraits().getContainer(), &v)
RESULT

CHECK(void swap(BaseIterator& iterator) throw())
	std::vector<float> v1(5);
	std::vector<float> v2(10);	
	MyIterator i1;
	i1.getTraits().bindTo(v1);
	MyIterator i2;
	i2.getTraits().bindTo(v2);
	TEST_EQUAL(i1.getContainer(), &v1)
	TEST_EQUAL(i2.getContainer(), &v2)
	i1.swap(i2);
	TEST_EQUAL(i1.getContainer(), &v2)
	TEST_EQUAL(i2.getContainer(), &v1)	
RESULT

CHECK([EXTRA] STL requirements for input iterator)
 // ????
RESULT

END_TEST
