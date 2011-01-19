#ifndef BALL_LINALG_DIAGONALITERATOR_H
#define BALL_LINALG_DIAGONALITERATOR_H

#ifndef BALL_LINALG_MATRIX_IH
# include <BALL/MATHS/LINALG/matrix.ih>
#endif

#ifndef BALL_LINALG_VECTOR_IH
# include <BALL/MATHS/vector.ih>
#endif

#ifndef BALL_CONCEPT_RANDOMACCESSITERATOR_H
#include <BALL/CONCEPT/randomAccessIterator.h>
#endif

namespace BALL {

	// forward declaration
	template <class valuetype, class mtraits>
	class Matrix;

	
	template <class valuetype, class mtraits=StandardTraits>
	class DiagonalIteratorTraits
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

			virtual ~DiagonalIteratorTraits()
			{
			}

			DiagonalIteratorTraits()
				:	bound_(0),
					position_(0),
					vector_(0)
			{
			}
			
			DiagonalIteratorTraits(const Matrix<valuetype, mtraits>& matrix)
				:	bound_(const_cast<Matrix<valuetype, mtraits>*>(&matrix)),
					position_(0),
					vector_(bound_->m_)
			{
			}
			
			DiagonalIteratorTraits(const DiagonalIteratorTraits& traits)
				:	bound_(traits.bound_),
					position_(traits.position_),
					vector_(bound_->m_)
			{
			}
			
			DiagonalIteratorTraits& operator = (const DiagonalIteratorTraits& traits)
			{
				bound_ = traits.bound_;
				position_ = traits.position_;
				vector_ = traits.vector_;
		
				return *this;
			}

			Matrix<valuetype, mtraits>* getContainer()
			{
				return bound_;
			}
			
			const Matrix<valuetype, mtraits>* getContainer() const
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

			bool operator == (const DiagonalIteratorTraits& traits) const
			{
			  return (position_ == traits.position_);
			}

			bool operator != (const DiagonalIteratorTraits& traits) const
			{
				return (position_ != traits.position_);
			}
				
			bool operator < (const DiagonalIteratorTraits& traits) const
			{
			  if (bound_->row_major_)
			  {
			    return (((position_ / bound_->m_) > (traits.position_ / traits.bound_->m_)) &&
				    ((position_ % bound_->m_) < (traits.position_ % traits.bound_->m_)));
			  }
			  else
			  {
			    return (((position_ % bound_->n_) > (traits.position_ % traits.bound_->n_)) &&
				    ((position_ / bound_->n_) < (traits.position_ / traits.bound_->n_)));
			  }
			}

			Distance getDistance(const DiagonalIteratorTraits& traits) const
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
			  if (bound_->row_major_)
			  {
			    position_ = bound_->data_.size() - bound_->m_;
			  }
			  else
			  {
			    position_ = (bound_->n_ - 1);
			  }
			}

			bool isBegin() const
			{
			  if (bound_->row_major_)
			  {
			    return (position_ == (int)(bound_->data_.size() - bound_->m_));
			  }
			  else
			  {
			    return (position_ == (int)(bound_->n_ - 1));
			  }
			}

			void toEnd()
			{
			  if (bound_->row_major_)
			  {
			    position_ = bound_->m_;
			  }
			  else
			  {
			    position_ = bound_->data_.size();
			  }
			}
			
			bool isEnd() const
			{
			  if (bound_->row_major_)
			  {
			    return (position_ == (int)bound_->m_);
			  }
			  else
			  {
			    return (position_ == (int)bound_->data_.size());
			  }
			}
			
			Vector<valuetype>& getData()
			{

			  if (bound_->row_major_)
			  {
			    // calculate the size of the current diagonalvector
			    uint vector_size = std::min(bound_->n_-(position_ / bound_->m_), bound_->m_-(position_ % bound_->m_));
			    vector_.resize(vector_size);
			    uint i = 0;
			    for (uint j = 0; j < vector_size; j++)
			    {
			      vector_[j]=(*bound_)[position_+i];
			      i+=bound_->m_+1;
			    }
			  }
			  else
			  {
			    // calculate the size of the current diagonalvector
			    uint vector_size = std::min(bound_->n_-(position_ % bound_->n_), bound_->m_-(position_ / bound_->n_));
			    vector_.resize(vector_size);
			    uint j = 0;
			    for (uint i = 0; position_+i < bound_->data_.size(); i+=bound_->n_)
			    {
			      vector_[j++]=(*bound_)[position_+i];
			      i++;
			    }
			  }

			  return vector_;
			}

			const Vector<valuetype>& getData() const
			{
			  if (bound_->row_major_)
			  {

			    // calculate the size of the current diagonalvector
			    uint vector_size = std::min(bound_->n_-(position_ / bound_->m_), bound_->m_-(position_ % bound_->m_));
			    vector_.resize(vector_size);
			    uint i = 0;
			    for (uint j = 0; j < vector_size; j++)
			    {
			      vector_[j]=(*bound_)[position_+i];
			      i+=bound_->m_+1;
			    }
			  }
			  else
			  {
			    // calculate the size of the current diagonalvector
			    uint vector_size = std::min(bound_->n_-(position_ % bound_->n_), bound_->m_-(position_ / bound_->n_));
			    vector_.resize(vector_size);
			    uint j = 0;
			    for (uint i = 0; position_+i < bound_->data_.size(); i+=bound_->n_)
			    {
			      vector_[j++]=(*bound_)[position_+i];
			      i++;
			    }
			  }

			  return vector_;

			}

			void forward()
			{
			  int i,j;
			  if (bound_->row_major_)
			  {
			    i = position_ / bound_->m_;
			    j = position_ % bound_->m_;
			  }
			  else
			  {
			    i = position_ % bound_->n_;
			    j = position_ / bound_->n_;
			  }

			  if (i != 0)
			  {
			    i--;
			  } else
			  {
			    j++;
			  }
			  if (bound_->row_major_)
			  {
			    position_ = j + bound_->m_*i;
			  }
			  else
			  {
			    position_ = i + bound_->n_*j;
			  }
			}

			friend std::ostream& operator << (std::ostream& s, const DiagonalIteratorTraits& traits)
			{
			  return (s << traits.position_ << ' ');
			}
			
			void dump(std::ostream& s) const
			{
			  s << position_ << std::endl;
			}
			
			void toRBegin()
			{
			  if (bound_->row_major_)
			  {
			    position_ = (bound_->m_ - 1);
			  }
			  else
			  {
			    position_ = bound_->data_.size() - bound_->n_;
			  }
			}
			
			bool isRBegin() const
			{
			  if (bound_->row_major_)
			  {
			    return (position_ == (int)(bound_->m_ - 1));
			  }
			  else
			  {
			    return (position_ == (int)(bound_->data_.size() - bound_->n_));
			  }
			}
			
			void toREnd()
			{
			  if (bound_->row_major_)
			  {
			    position_ = bound_->data_.size();
			  }
			  else
			  {
			    position_ = bound_->n_;
			  }
			}

			bool isREnd() const
			{
			  if (bound_->row_major_)
			  {
			    return (position_ == (int)bound_->data_.size());
			  }
			  else
			  {
			    return  (position_ == (int)bound_->n_);
			  }
			}
			
			void backward()
			{

			  int i,j;
			  if (bound_->row_major_)
			  {
			    if (position_ == (int)bound_->m_)
			    {
			      position_--;
			      return;
			    }
			    i = position_ / bound_->m_;
			    j = position_ % bound_->m_;
			  }
			  else
			  {
			    i = position_ % bound_->n_;
			    j = position_ / bound_->n_;
			  }
			  if (j != 0)
			  {
			    j--;
			  } else
			  {
			    i++;
			  }
			  if (bound_->row_major_)
			  {
			    position_ = j + bound_->m_*i;
			  }
			  else
			  {
			    position_ = i + bound_->n_*j;
			  }

			}

			void backward(Distance distance)
			{
			  int i,j;
			  if (bound_->row_major_)
			  {
			    if (position_ == (int)bound_->m_)
			    {
			      position_--;
			      distance--;
			    }
			    i = position_ / bound_->m_;
			    j = position_ % bound_->m_;
			  }
			  else
			  {
			    i = position_ % bound_->n_;
			    j = position_ / bound_->n_;
			  }

			  if (j-distance >= 0)
			  {
			    j-=distance;
			  } else
			  {
			    j = 0;
			    i += (distance - j);
			  }
			  if (bound_->row_major_)
			  {
			    position_ = j + bound_->m_*i;
			  }
			  else
			  {
			    position_ = i + bound_->n_*j;
			  }
			}

			void forward(Distance distance)
			{
			  
			  int i,j;
			  if (bound_->row_major_)
			  {
			    i = position_ / bound_->m_;
			    j = position_ % bound_->m_;
			  }
			  else
			  {
			    i = position_ % bound_->n_;
			    j = position_ / bound_->n_;
			  }

			  if (i-distance >= 0)
			  {
			    i-=distance;
			  } else
			  {
			    i = 0;
			    j += (distance - i);
			  }
			  if (bound_->row_major_)
			  {
			    position_ = j + bound_->m_*i;
			  }
			  else
			  {
			    position_ = i + bound_->n_*j;
			  }
			}
			
			Vector<valuetype>& getData(Index index)
			{
			  int i = bound_->n_ - 1;
			  int j = 0;
			  int position;

			  if (i-index >= 0)
			  {
			    i-=index;
			  } else
			  {
			    i = 0;
			    j += (index - i);
			  }
			  if (bound_->row_major_)
			  {
			    position = j + bound_->m_*i;
			  }
			  else
			  {
			    position = i + bound_->n_*j;
			  }
			  if (bound_->row_major_)
			  {
			    // calculate the size of the current diagonalvector
			    uint vector_size = std::min(bound_->n_-(position / bound_->m_), bound_->m_-(position % bound_->m_));
			    vector_.resize(vector_size);
			    uint i = 0;
			    for (uint j = 0; j < vector_size; j++)
			    {
			      vector_[j]=(*bound_)[position+i];
			      i+=bound_->m_+1;
			    }
			  }
			  else
			  {
			    // calculate the size of the current diagonalvector
			    uint vector_size = std::min(bound_->n_-(position % bound_->n_), bound_->m_-(position / bound_->n_));
			    vector_.resize(vector_size);
			    uint j = 0;
			    for (uint i = 0; i < bound_->data_.size(); i+=bound_->n_)
			    {
			      vector_[j++]=(*bound_)[position+i];
			      i++;
			    }
			  }
			  
			  return vector_;
			}
			
			const Vector<valuetype>& getData(Index index) const
			{
			  int i = bound_->n_ - 1;
			  int j = 0;
			  int position;

			  if (i-index >= 0)
			  {
			    i-=index;
			  } else
			  {
			    i = 0;
			    j += (index - i);
			  }
			  if (bound_->row_major_)
			  {
			    position = j + bound_->m_*i;
			  }
			  else
			  {
			    position = i + bound_->n_*j;
			  }
			  if (bound_->row_major_)
			  {
			    // calculate the size of the current diagonalvector
			    uint vector_size = std::min(bound_->n_-(position / bound_->m_), bound_->m_-(position % bound_->m_));
			    vector_.resize(vector_size);
			    uint i = 0;
			    for (uint j = 0; j < vector_size; j++)
			    {
			      vector_[j]=(*bound_)[position+i];
			      i+=bound_->m_+1;
			    }
			  }
			  else
			  {
			    // calculate the size of the current diagonalvector
			    uint vector_size = std::min(bound_->n_-(position % bound_->n_), bound_->m_-(position / bound_->n_));
			    vector_.resize(vector_size);
			    uint j = 0;
			    for (uint i = 0; i < bound_->data_.size(); i+=bound_->n_)
			    {
			      vector_[j++]=(*bound_)[position+i];
			      i++;
			    }
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
