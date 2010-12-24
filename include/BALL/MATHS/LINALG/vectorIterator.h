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
			{
			}

			VectorIteratorTraits()
				:	bound_(0),
					position_(0)
			{
			}
			
			VectorIteratorTraits(const Vector<valuetype>& vector)
				:	bound_(const_cast<Vector<valuetype>*>(&vector)),
					position_(0)
			{
			}
			
			VectorIteratorTraits(const VectorIteratorTraits& traits)
				:	bound_(traits.bound_),
					position_(traits.position_)
			{
			}
			
			VectorIteratorTraits& operator = (const VectorIteratorTraits& traits)
			{
				bound_ = traits.bound_;
				position_ = traits.position_;
		
				return *this;
			}

			Vector<valuetype>* getContainer()
			{
				return bound_;
			}
			
			const Vector<valuetype>* getContainer() const
			{
				return bound_;
			}
			
			bool isSingular() const
			{
				return (bound_ == 0);
			}
			
			IteratorPosition& getPosition()
			{
				return position_;
			}

			const IteratorPosition& getPosition() const
			{
				return position_;
			}

			bool operator == (const VectorIteratorTraits& traits) const
			{
			  return (position_ == traits.position_);
			}

			bool operator != (const VectorIteratorTraits& traits) const
			{
				return (position_ != traits.position_);
			}
				
			bool operator < (const VectorIteratorTraits& traits) const
			{
			  return (position_ < traits.position_);
			}

			Distance getDistance(const VectorIteratorTraits& traits) const
			{
			  return (Distance)(position_ - traits.position_);
			}
			
			bool isValid() const
			{
			  return ((bound_ != 0) && (position_ >= 0) && (position_ < (int)bound_->data_.size()));
			}

			void invalidate()
			{
				bound_ = 0;
				position_ = -1;
			}
			
			void toBegin()
			{
			  position_ = 0;
			}

			bool isBegin() const
			{
			  return ( position_ == 0 );
			}

			void toEnd()
			{
			  position_ = bound_->data_.size();
			}
			
			bool isEnd() const
			{
			  return ( position_ == (int)bound_->data_.size());
			}
			
			ValueType& getData()
			{
				return (*bound_)[position_];
			}

			const ValueType& getData() const
			{
				return (*bound_)[position_];
			}

			void forward()
			{
			  position_++;
			}

			friend std::ostream& operator << (std::ostream& s, const VectorIteratorTraits& traits)
			{
			  return (s << traits.position_ << ' ');
			}
			
			void dump(std::ostream& s) const
			{
			  s << position_ << std::endl;
			}
			
			void toRBegin()
			{
			  position_ = bound_->data_.size() - 1;
			}
			
			bool isRBegin() const
			{
			  return (position_ == bound_->data_.size() - 1);
			}
			
			void toREnd()
			{
			  position_ = -1;
			}

			bool isREnd() const
			{
			  return (position_ <= -1);
			}
			
			void backward()
			{
			  position_--;
			}

			void backward(Distance distance)
			{
			  position_ -= distance;
			}

			void forward(Distance distance)
			{
			  position_ += distance;
			}
			
			ValueType& getData(Index index)
			{
			  return (*bound_)[index];
			}
			
			const ValueType& getData(Index index) const
			{
			  return (*bound_)[index];
			}
			

			protected:

			Vector<valuetype>*		bound_;
			IteratorPosition		position_;
		};


} // namespace BALL

#endif
