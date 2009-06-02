#ifndef BALL_LINALG_ELEMENTROWITERATOR_H
#define BALL_LINALG_ELEMENTROWITERATOR_H

#ifndef BALL_LINALG_MATRIX_iH
# include <BALL/MATHS/LINALG/matrix.ih>
#endif

#ifndef BALL_CONCEPT_RANDOMACCESSITERATOR_H
#include <BALL/CONCEPT/randomAccessIterator.h>
#endif

namespace BALL {

	// forward declaration
	template <class valuetype, class mtraits>
	class Matrix;

	
	template <class valuetype, class mtraits=StandardTraits>
	class ElementRowIteratorTraits
	{
		/**
		*/
		typedef valuetype ValueType;

		/**
		*/
		typedef valuetype* PointerType;

		/**
		*/
		typedef int IteratorPosition;

		/**
		*/
		typedef int Distance;

		/**
		*/
		typedef int Index;

		friend class Matrix<valuetype, mtraits>;
	  public:

			virtual ~ElementRowIteratorTraits()
				throw()
			{
			}

			ElementRowIteratorTraits()
				throw()
				:	bound_(0),
					position_(0)
			{
			}
			
			ElementRowIteratorTraits(const Matrix<valuetype, mtraits>& matrix)	throw()
				:	bound_(const_cast<Matrix<valuetype, mtraits>*>(&matrix)),
					position_(0)
			{
			}
			
			ElementRowIteratorTraits(const ElementRowIteratorTraits& traits)	throw()
				:	bound_(traits.bound_),
					position_(traits.position_)
			{
			}
			
			ElementRowIteratorTraits& operator = (const ElementRowIteratorTraits& traits)	throw()
			{
				bound_ = traits.bound_;
				position_ = traits.position_;
		
				return *this;
			}

			Matrix<valuetype, mtraits>* getContainer()	throw()
			{
				return bound_;
			}
			
			const Matrix<valuetype, mtraits>* getContainer() const	throw()
			{
				return bound_;
			}
			
			bool isSingular() const	throw()
			{
				return (bound_ == 0);
			}
			
			IteratorPosition& getPosition()	throw()
			{
				return position_;
			}

			const IteratorPosition& getPosition() const	throw()
			{
				return position_;
			}

			bool operator == (const ElementRowIteratorTraits& traits) const	throw()
			{
			  return (position_ == traits.position_);
			}

			bool operator != (const ElementRowIteratorTraits& traits) const	throw()
			{
				return (position_ != traits.position_);
			}
				
			bool operator < (const ElementRowIteratorTraits& traits) const throw()
			{
			  return (position_ < traits.position_);
			}

			Distance getDistance(const ElementRowIteratorTraits& traits) const throw()
			{
			  return (Distance)(position_ - traits.position_);
			}
			
			bool isValid() const	throw()
			{
			  return ((bound_ != 0) && (position_ >= 0) && (position_ < (int)bound_->data_.size()));
			}

			void invalidate()	throw()
			{
				bound_ = 0;
				position_ = -1;
			}
			
			void toBegin()	throw()
			{
			  position_ = 0;
			}

			bool isBegin() const	throw()
			{
			  return ( position_ == 0 );
			}

			void toEnd()	throw()
			{
			  position_ = bound_->data_.size();
			}
			
			bool isEnd() const	throw()
			{
			  return ( position_ == (int)bound_->data_.size());
			}
			
			ValueType& getData()	throw()
			{
				return (*bound_)[position_];
			}

			const ValueType& getData() const	throw()
			{
				return (*bound_)[position_];
			}

			void forward()	throw()
			{
			  if (bound_->row_major_)
			  {
			    position_++;
			  }
			  else
			  {
			    if ((uint)(position_+1) == bound_->data_.size())
			    {
			      position_++;
			      return;
			    }
			    position_ += bound_->n_;
			    if ((uint)position_ >= bound_->data_.size())
			      position_ = (position_ % bound_->m_) + 1;
			  }
			}

			friend std::ostream& operator << (std::ostream& s, const ElementRowIteratorTraits& traits)
			  throw()
			{
			  return (s << traits.position_ << ' ');
			}
			
			void dump(std::ostream& s) const
			  throw()
			{
			  s << position_ << std::endl;
			}
			
			void toRBegin()
			  throw()
			{
			  position_ = bound_->data_.size() - 1;
			}
			
			bool isRBegin() const
			  throw()
			{
			  return (position_ == bound_->data_.size() - 1);
			}
			
			void toREnd()
			  throw()
			{
			  position_ = -1;
			}

			bool isREnd() const
			  throw()
			{
			  return (position_ <= -1);
			}
			
			void backward()
			  throw()
			{
			  if (bound_->row_major_)
			  {
			    position_--;
			  }
			  else
			  {
			    if (position_ == 0)
			    {
			      position_--;
			      return;
			    }
			    position_ -= bound_->n_;
			    if (position_ < 0)
			      position_ = (int)(bound_->data_.size()) - 1 + position_;
			  }
			}

			void backward(Distance distance)
			  throw()
			{
			  if (bound_->row_major_)
			  {
			    position_ -= distance;
			  }
			  else
			  {
			    for (int i=0; i<distance; i++)
			    {
			      if (position_ == 0)
			      {
				position_--;
				return;
			      }
			      position_ -= bound_->n_;
			      if (position_ < 0)
				position_ = (int)(bound_->data_.size()) - 1 + position_;
			    }
			  }
			}

			void forward(Distance distance)
			  throw()
			{
			  if (bound_->row_major_)
			  {
			    position_ += distance;
			  }
			  else
			  {
			    for (int i=0; i<distance; i++)
			    {
			      if ((uint)(position_+1) == bound_->data_.size())
			      {
				position_++;
				return;
			      }
			      position_ += bound_->n_;
			      if ((uint)position_ >= bound_->data_.size())
				position_ = (position_ % bound_->m_) + 1;
			    }
			  }
			}
			
			ValueType& getData(Index index) throw()
			{
			  return (*bound_)[index];
			}
			
			const ValueType& getData(Index index) const throw()
			{
			  return (*bound_)[index];
			}
			

			protected:

			Matrix<valuetype, mtraits>*		bound_;
			IteratorPosition		position_;
		};


} // namespace BALL

#endif
