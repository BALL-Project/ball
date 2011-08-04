// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/CONCEPT/bidirectionalIterator.h>
#include <vector>
///////////////////////////

using namespace BALL;
using namespace std;

typedef Index VectorIteratorPosition;

template <typename DataType>
class VectorIteratorTraits
{
	public:

	BALL_CREATE_DEEP(VectorIteratorTraits)

	VectorIteratorTraits()
		throw()
		: bound_(0),
			position_(0)
	{
	}
	
	VectorIteratorTraits(const vector<DataType>& container)
		throw()
		: bound_((vector<DataType>*)& container),
			position_(0)
	{
	}
	
	VectorIteratorTraits(const VectorIteratorTraits& traits, bool /* deep */ = true)
		throw()
		: bound_(traits.bound_),
			position_(traits.position_)
	{
	}
	
	const VectorIteratorTraits& operator = (const VectorIteratorTraits& traits)
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

	const VectorIteratorPosition& getPosition() const
		throw(Exception::InvalidIterator)
	{
		if (bound_ == 0)
		{
			throw(Exception::InvalidIterator(__FILE__, __LINE__));
		}
		return position_;
	}

	bool operator == (const VectorIteratorTraits& traits) const
		throw(Exception::InvalidIterator)
	{
		if (bound_ == 0)
		{
			throw(Exception::InvalidIterator(__FILE__, __LINE__));
		}
		return (position_ == traits.position_);
	}

	bool operator != (const VectorIteratorTraits& traits) const
		throw(Exception::InvalidIterator)
	{
		if (bound_ == 0)
		{
			throw(Exception::InvalidIterator(__FILE__, __LINE__));
		}
		return (position_ != traits.position_);
	}
	
	bool operator < (const VectorIteratorTraits& traits) const
		throw(Exception::InvalidIterator)
	{
		if (bound_ == 0)
		{
			throw(Exception::InvalidIterator(__FILE__, __LINE__));
		}
		return (position_ < traits.position_);
	}
	
	bool operator > (const VectorIteratorTraits& traits) const
		throw(Exception::InvalidIterator)
	{
		if (bound_ == 0)
		{
			throw(Exception::InvalidIterator(__FILE__, __LINE__));
		}
		return (position_ > traits.position_);
	}

	Distance getDistance(const VectorIteratorTraits& traits) const
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
		return (bound_ != 0 && position_ >= 0 && position_ < (VectorIteratorPosition)bound_->size());
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
		throw()
	{
		return (position_ == 0) && (bound_ != 0);
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
		throw()
	{
		return ((position_ >= (VectorIteratorPosition)bound_->size()) && (bound_ != 0));
	}

	DataType& getData()
		throw(Exception::InvalidIterator)
	{
		if (bound_ == 0)
		{
			throw(Exception::InvalidIterator(__FILE__, __LINE__));
		}
		if (position_ >= (VectorIteratorPosition)bound_->size())
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
		if (position_ >= (VectorIteratorPosition)bound_->size())
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
		return (position_ == (VectorIteratorPosition)(bound_->size() - 1));
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
	
		if (position_ + distance > (VectorIteratorPosition)(bound_->size()))
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
		
		if (index >= (VectorIteratorPosition)bound_->size())
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
	VectorIteratorPosition  position_;
};

typedef ConstBidirectionalIterator<vector<float>, float, VectorIteratorPosition, VectorIteratorTraits<float> > MyIterator;
typedef reverse_iterator<ConstBidirectionalIterator<vector<float>, float, VectorIteratorPosition, VectorIteratorTraits<float> > > MyReverseIterator;

START_TEST(ConstBidirectionalIterator)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

std::vector<float> v(8);
v[0] = 0.12;
v[1] = 1.23;
v[2] = 2.34;
v[3] = 3.45;
v[4] = 4.56;
v[5] = 5.67;
v[6] = 6.78;
v[7] = 7.89;

MyIterator* m_ptr = 0;
CHECK(ConstBidirectionalIterator() throw())
	m_ptr = new MyIterator;
	TEST_NOT_EQUAL(m_ptr, 0)
RESULT

CHECK(~ConstBidirectionalIterator() throw())
	delete m_ptr;
RESULT

CHECK(ConstBidirectionalIterator operator ++ (int) throw(Exception::Precondition))
  // ???
RESULT

CHECK(ConstBidirectionalIterator operator -- (int) throw(Exception::Precondition))
  // ???
RESULT

CHECK(ConstBidirectionalIterator& operator ++ () throw(Exception::Precondition))
  // ???
RESULT

CHECK(ConstBidirectionalIterator& operator -- () throw(Exception::Precondition))
  // ???
RESULT

CHECK(ConstBidirectionalIterator(const ConstBidirectionalIterator& iterator) throw())
  // ???
RESULT

CHECK(bool isEnd() const throw())
  // ???
RESULT

CHECK(bool isRBegin() const throw())
  // ???
RESULT

CHECK(bool isREnd() const throw())
  // ???
RESULT

CHECK(static ConstBidirectionalIterator begin(const Container& container) throw(Exception::Precondition))
	MyIterator m = MyIterator::begin(v);
	TEST_EQUAL(m.isValid(), true)
	ABORT_IF(!m.isValid())
	TEST_EQUAL(*m, v[0])
RESULT

CHECK(bool isBegin() const throw())
	MyIterator m1;
	TEST_EQUAL(m1.isBegin(), false)
	MyIterator m2 = MyIterator::begin(v);
	TEST_EQUAL(m2.isBegin(), true)
RESULT

CHECK(static ConstBidirectionalIterator end(const Container& container) throw(Exception::Precondition))
  // ???
RESULT

CHECK(static ConstBidirectionalIterator rbegin(const Container& container) throw(Exception::Precondition))
  // ???
RESULT

CHECK(static ConstBidirectionalIterator rend(const Container& container) throw(Exception::Precondition))
  // ???
RESULT

CHECK(void toBegin() throw(Exception::Precondition))
  // ???
RESULT

CHECK(void toEnd() throw(Exception::Precondition))
  // ???
RESULT

CHECK(void toRBegin() throw(Exception::Precondition))
  // ???
RESULT

CHECK(void toREnd() throw(Exception::Precondition))
  // ???
RESULT

END_TEST
