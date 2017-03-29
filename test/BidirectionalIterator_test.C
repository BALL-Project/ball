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

typedef Index VectorIteratorPosition_;

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

	const VectorIteratorPosition_& getPosition() const
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

typedef BidirectionalIterator<vector<float>, float, VectorIteratorPosition_, VectorIteratorTraits<float> > MyIterator;
typedef reverse_iterator<BidirectionalIterator<vector<float>, float, VectorIteratorPosition_, VectorIteratorTraits<float> > > MyReverseIterator;

START_TEST(BidirectionalIterator)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

MyIterator* m_ptr = 0;

CHECK(BidirectionalIterator() throw())
	m_ptr = new MyIterator;
	TEST_NOT_EQUAL(m_ptr, 0)
RESULT

CHECK(~BidirectionalIterator() throw())
	delete m_ptr;
RESULT

CHECK(BidirectionalIterator operator ++ (int) throw(Exception::Precondition))
  // ???
RESULT

CHECK(BidirectionalIterator operator -- (int) throw(Exception::Precondition))
  // ???
RESULT

CHECK(BidirectionalIterator& operator ++ () throw(Exception::Precondition))
  // ???
RESULT

CHECK(BidirectionalIterator& operator -- () throw(Exception::Precondition))
  // ???
RESULT

CHECK(BidirectionalIterator(const BidirectionalIterator& iterator) throw())
  // ???
RESULT

CHECK(pointer operator -> () const throw())
  // ???
RESULT

CHECK(reference operator * () const throw())
  // ???
RESULT

CHECK(static BidirectionalIterator begin(const Container& container) throw(Exception::Precondition))
  // ???
RESULT

CHECK(static BidirectionalIterator end(const Container& container) throw(Exception::Precondition))
  // ???
RESULT

CHECK(static BidirectionalIterator rbegin(const Container& container) throw(Exception::Precondition))
  // ???
RESULT

CHECK(static BidirectionalIterator rend(const Container& container) throw(Exception::Precondition))
  // ???
RESULT

END_TEST
