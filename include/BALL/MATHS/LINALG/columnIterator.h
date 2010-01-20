#ifndef BALL_LINALG_COLUMNITERATOR_H
#define BALL_LINALG_COLUMNITERATOR_H

#ifndef BALL_LINALG_MATRIX_IH
# include <BALL/MATHS/LINALG/matrix.ih>
#endif

#ifndef BALL_LINALG__VECTOR_IH
# include <BALL/MATHS/LINALG/vector.ih>
#endif

#ifndef BALL_CONCEPT_RANDOMACCESSITERATOR_H
#include <BALL/CONCEPT/randomAccessIterator.h>
#endif

namespace BALL {

	// forward declaration
	template <class valuetype, class mtraits>
	class Matrix;
	
	template <class valuetype, class mtraits=StandardTraits>
	class ColumnIteratorTraits
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

			virtual ~ColumnIteratorTraits()
				throw()
			{
			}

			ColumnIteratorTraits()
				throw()
				:	bound_(0),
					position_(0),
					vector_(0)
			{
			}
			
			ColumnIteratorTraits(const Matrix<valuetype, mtraits>& matrix)	throw()
				:	bound_(const_cast<Matrix<valuetype, mtraits>*>(&matrix)),
					position_(0),
					vector_(bound_->n_)
			{
			}
			
			ColumnIteratorTraits(const ColumnIteratorTraits& traits)	throw()
				:	bound_(traits.bound_),
					position_(traits.position_),
					vector_(bound_->n_)
			{
			}
			
			ColumnIteratorTraits& operator = (const ColumnIteratorTraits& traits)	throw()
			{
				bound_ = traits.bound_;
				position_ = traits.position_;
				vector_ = traits.vector_;
		
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

			bool operator == (const ColumnIteratorTraits& traits) const	throw()
			{
			  return (position_ == traits.position_);
			}

			bool operator != (const ColumnIteratorTraits& traits) const	throw()
			{
				return (position_ != traits.position_);
			}
				
			bool operator < (const ColumnIteratorTraits& traits) const throw()
			{
			  return (position_ < traits.position_);
			}

			Distance getDistance(const ColumnIteratorTraits& traits) const throw()
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
			
			Vector<valuetype>& getData()	throw()
			{

			  if (!bound_->row_major_)
			  {
			    for (uint i = 0; i < bound_->n_; i++)
			    {
			      vector_[i]=&(*bound_)[position_+i];
			    }
			  }
			  else
			  {
			    uint j = 0;
			    for (uint i = 0; i < bound_->data_.size(); i+=bound_->m_)
			    {
			      vector_[j++]=&(*bound_)[position_+i];
			    }
			  }

			  return vector_;
			}

			const Vector<valuetype>& getData() const	throw()
			{

			  if (!bound_->row_major_)
			  {
			    for (uint i = 0; i < bound_->n_; i++)
			    {
			      vector_[i]=(*bound_)[position_+i];
			    }
			  }
			  else
			  {
			    uint j = 0;
			    for (uint i = 0; i < bound_->data_.size(); i+=bound_->m_)
			    {
			      vector_[j++]=(*bound_)[position_+i];
			    }
			  }
			  return vector_;

			}

			void forward()	throw()
			{
			  if (!bound_->row_major_)
			  {
			    position_ += bound_->n_;
			  }
			  else
			  {
			    position_++;
			    if (position_ == (int)bound_->m_)
			      position_ = bound_->data_.size();
			  }
			}

			friend std::ostream& operator << (std::ostream& s, const ColumnIteratorTraits& traits)
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
			  if (!bound_->row_major_)
			  {
			    if (position_ == 0)
			      position_--;
			    else
			      position_ -= bound_->n_;
			  }
			  else
			  {
			    if (position_ == (int)bound_->data_.size())
			      position_ = bound_->m_;
			    position_--;
			  }
			}

			void backward(Distance distance)
			  throw()
			{			  
			  if (!bound_->row_major_)
			  {
			    if (position_-(distance * (int)bound_->n_) < 0)
			    {
			      position_ = -1;
			      return;
			    }
			    position_ -= (distance * (int)bound_->n_);
			    
			  }
			  else
			  {
			    if (position_ == (int)bound_->data_.size())
			      position_ = bound_->m_;
			    if (position_-distance < 0)
			    {
			      position_ = -1;
			      return;
			    }
			    position_ -= distance;
			  }
			}

			void forward(Distance distance)
			  throw()
			{
			  
			  if (!bound_->row_major_)
			  {
			    if (position_+(distance * bound_->n_) > bound_->data_.size())
			    {
			      position_ = bound_->data_.size();
			      return;
			    }
			    position_ += (distance * bound_->n_);
			  }
			  else
			  {
			    position_ += distance;
			    if (position_ >= (int)bound_->m_)
			      position_ = bound_->data_.size();
			  }
			}
			
			Vector<valuetype>& getData(Index index) throw()
			{

			  if (!bound_->row_major_)
			  {
			    for (uint i = 0; i < bound_->n_; i++)
			      vector_[i]=(*bound_)[index+i];
			  }
			  else
			  {
			    for (uint i = 0; i < bound_->n_; i+=bound_->m_)
			      vector_[i]=(*bound_)[index+i];
			  }

			  return vector_;
			}
			
			const Vector<valuetype>& getData(Index index) const throw()
			{

			  if (!bound_->row_major_)
			  {
			    for (uint i = 0; i < bound_->n_; i++)
			      vector_[i]=(*bound_)[index+i];
			  }
			  else
			  {
			    for (uint i = 0; i < bound_->n_; i+=bound_->m_)
			      vector_[i]=(*bound_)[index+i];
			  }

			  return vector_;
			}
			

			protected:

			Matrix<valuetype, mtraits>*		bound_;
			IteratorPosition		position_;
			mutable Vector<valuetype>           vector_;
		};


} // namespace BALL

#endif
