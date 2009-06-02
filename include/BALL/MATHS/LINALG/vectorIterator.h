#ifndef BALL_LINALG_VECTORITERATOR_H
#define BALL_LINALG_VECTORITERATOR_H

#ifndef BALL_LINALG_VECTOR_IH
# include <BALL/MATHS/LINALG/vector.ih>
#endif

#ifndef BALL_CONCEPT_RANDOMACCESSITERATOR_H
#include <BALL/CONCEPT/randomAccessIterator.h>
#endif

namespace BALL {

        // forward declaration
        template <typename valuetype>
	  class Vector;

	
        template <typename valuetype>
	class VectorIteratorTraits
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

	    friend class Vector<valuetype>;
	  public:

			virtual ~VectorIteratorTraits()
				throw()
			{
			}

			VectorIteratorTraits()
				throw()
				:	bound_(0),
					position_(0)
			{
			}
			
			VectorIteratorTraits(const Vector<valuetype>& vector)	throw()
				:	bound_(const_cast<Vector<valuetype>*>(&vector)),
					position_(0)
			{
			}
			
			VectorIteratorTraits(const VectorIteratorTraits& traits)	throw()
				:	bound_(traits.bound_),
					position_(traits.position_)
			{
			}
			
			VectorIteratorTraits& operator = (const VectorIteratorTraits& traits)	throw()
			{
				bound_ = traits.bound_;
				position_ = traits.position_;
		
				return *this;
			}

			Vector<valuetype>* getContainer()	throw()
			{
				return bound_;
			}
			
			const Vector<valuetype>* getContainer() const	throw()
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

			bool operator == (const VectorIteratorTraits& traits) const	throw()
			{
			  return (position_ == traits.position_);
			}

			bool operator != (const VectorIteratorTraits& traits) const	throw()
			{
				return (position_ != traits.position_);
			}
				
			bool operator < (const VectorIteratorTraits& traits) const throw()
			{
			  return (position_ < traits.position_);
			}

			Distance getDistance(const VectorIteratorTraits& traits) const throw()
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
			  position_++;
			}

			friend std::ostream& operator << (std::ostream& s, const VectorIteratorTraits& traits)
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
			  position_--;
			}

			void backward(Distance distance)
			  throw()
			{
			  position_ -= distance;
			}

			void forward(Distance distance)
			  throw()
			{
			  position_ += distance;
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

			Vector<valuetype>*		bound_;
			IteratorPosition		position_;
		};


} // namespace BALL

#endif
