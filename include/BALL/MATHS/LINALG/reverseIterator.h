#ifndef BALL_LINALG_REVERSEITERATOR_H
#define BALL_LINALG_REVERSEITERATOR_H

#ifndef BALL_CONCEPT_RANDOMACCESSITERATOR_H
#include <BALL/CONCEPT/randomAccessIterator.h>
#endif

#include <BALL/MATHS/LINALG/linalgException.h>

namespace BALL 
{
	  ///
	  typedef int Distance;
	  ///
	  typedef int Index;

	/** @name	Generic Constant reverse Random Access Iterator
		\ingroup  ConceptsIterators
	*/
	//@{
	template <typename Container, typename DataType, typename Position, typename Traits>
	class ConstReverseIterator
		: public ConstRandomAccessIterator<Container, DataType, Position, Traits>
	{
		public:

    /** @name Typedefs
     */
    //@{

    ///
    typedef std::random_access_iterator_tag iterator_category;
    // convenience typedef
    typedef ConstRandomAccessIterator<Container, DataType, Position, Traits> Base;
    //@}

		/**	@name	Constructors and Destructors
		*/
		//@{

		///
		ConstReverseIterator() {}
	
		///
		ConstReverseIterator(const ConstReverseIterator& iterator)
			:	Base(iterator)
		{
		}

		///
		~ConstReverseIterator() {}
		//@}



		/**	@name Assignment
		*/
		//@{
		
		/** Advance iterator a given distance.
		 *  @exception Exception::InvalidIterator if the iterator is invalid
		 */
		ConstReverseIterator& operator += (Distance distance);

		/** Advance iterator a given distance in reverse direction.
		 *  @exception Exception::InvalidIterator if the iterator is invalid
		 */
		ConstReverseIterator& operator -= (Distance distance);

		/** Return an iterator.
		 *  It points to the element with the given distance from the element of this iterator.
		 *  @exception Exception::InvalidIterator if the iterator is invalid
		 */
		ConstReverseIterator operator + (Distance distance) const;

		/** Return an iterator.
		 *  It points to the element with the given distance in reverse direction
		 *  from the element of this iterator.
		 *  @exception Exception::InvalidIterator if the iterator is invalid
		 */
		ConstReverseIterator operator - (Distance distance) const;

		/** Return an iterator.
		 *  It points to the element with the given distance in reverse direction
		 *  from the element of this iterator.
		 *  @exception Exception::InvalidIterator if the iterator is invalid
		 *  @exception Exception::IncompatibleIterators if this iterator and the argument are incompatible
		 */
		Distance operator - (const ConstReverseIterator& iterator) const;

		/** Return a ConstReverseIterator for a given container. 
		 *  It points to the first element.
		 *  @exception Exception::Precondition
		 */
		static ConstReverseIterator begin(const Container& container);

		/** Return a Const ReverseIterator for a given container.
		 *  It points behind the last element of the container.
		 *  @exception Exception::Precondition
		 */
		static ConstReverseIterator end(const Container& container);

		/** Return a Const ReverseIterator for a given container.
		 *  It points at the last element of the container.
		 *  @exception Exception::Precondition
		 */
		static ConstReverseIterator rbegin(const Container& container);

		/** Return a Const ReverseIterator for a given container.
		 *  It points before the first element.
		 *  @exception Exception::Precondition
		 */
		static ConstReverseIterator rend(const Container& container);

		/** Increment operator
		 *  @exception Exception::InvalidIterator if the iterator is invalid
		 */
		ConstReverseIterator& operator ++ ();

		/** Postfix increment operator
		 *  @exception Exception::InvalidIterator if the iterator is invalid
		 */
		ConstReverseIterator operator ++ (int);

		/** Decrement operator
		 *  @exception Exception::SingularIterator if the iterator is singular
		 */
		ConstReverseIterator& operator -- ();

		/** Postfix decrement operator
		 *  @exception Exception::SingularIterator if the iterator is singular
		 */
		ConstReverseIterator operator -- (int);


		//@}

		/**	@name Predicates
		*/
		//@{

		///
		bool operator + () const { return Base::getTraits().isValid(); }

		///
		bool operator - () const { return !Base::getTraits().isValid(); }

    /** Lesser operator.
		 *	Returns true if both iterators have the same container
		 *  and the element of this iterator resides before the elemen of the given iterator.
		 *  @exception InvalidIterator if one of the iterators is not valid
		 *  @exception IncompatibleIterators if the iterators show to different containers.
		 */
		bool operator < (const ConstReverseIterator& iterator) const;

    /** Lesser or equal operator.
		 *  Returns true if both iterators have the same container
		 *  and the element of this iterator resides before the elemen of the given iterator
		 *  or both iterators point to the same element.
		 *  @exception InvalidIterator if one of the iterators is not valid
		 *  @exception IncompatibleIterators if the iterators show to different containers.
		 */
		bool operator <= (const ConstReverseIterator& iterator) const;

    /** Greater or equal operator.
		 *  Returns true if both iterators have the same container
		 *  and the element of this iterator resides after the elemen of the given iterator
		 *  or both iterators point to the same element.
		 *  @exception InvalidIterator if one of the iterators is not valid
		 *  @exception IncompatibleIterators if the iterators show to different containers.
		 */
		bool operator >= (const ConstReverseIterator& iterator) const;

    /** Greater operator.
		 *  Returns true if both iterators have the same container
		 *  and the element of this iterator resides after the elemen of the given iterator.
		 *  @exception InvalidIterator if one of the iterators is not valid
		 *  @exception IncompatibleIterators if the iterators show to different containers.
		 */
		bool operator > (const ConstReverseIterator& iterator) const;

		//@}

			

		/**	@name Iterator dereferencing
		*/
		//@{
		/** Random access operator.
		 *  @exception Exception::InvalidIterator if the iterator is invalid
		 */
		const DataType& operator [] (Index index) const;
		//@}

		protected:

		ConstReverseIterator(const Container& container)
			:	Base(container)
		{
		}
	};
	//@}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstReverseIterator<Container, DataType, Position, Traits>& 
		ConstReverseIterator<Container, DataType, Position, Traits>::operator ++ ()
	{
		  if (!Base::getTraits().isValid())
		  {
		    Exception::InvalidIterator e;
		    throw(e);
		  }
		Base::getTraits().backward();
		return *this;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstReverseIterator<Container, DataType, Position, Traits> 
		ConstReverseIterator<Container, DataType, Position, Traits>::operator ++ (int)
	{
		  if (!Base::getTraits().isValid())
		  {
		    Exception::InvalidIterator e;
		    throw(e);
		  }
		ConstReverseIterator iterator(*this);
		++(*this);
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstReverseIterator<Container, DataType, Position, Traits>& 
		ConstReverseIterator<Container, DataType, Position, Traits>::operator -- ()
	{	
		  if (Base::getTraits().isSingular())
		  {
		    Exception::SingularIterator e;
		    throw(e);
		  }
		Base::getTraits().forward();
		return *this;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstReverseIterator<Container, DataType, Position, Traits> 
		ConstReverseIterator<Container, DataType, Position, Traits>::operator -- (int)
	{	
		  if (Base::getTraits().isSingular())
		  {
		    Exception::SingularIterator e;
		    throw(e);
		  }
		ConstReverseIterator iterator(*this);
		--(*this);
		return iterator;
	}
	
	/** Return an incremented iterator.
	 *  It points to the element with the given distance from the element 
	 *  of the given iterator.
	 *  @exception Exception::InvalidIterator if the iterator is invalid
	 */
	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstReverseIterator<Container, DataType, Position, Traits> operator + 
		(Distance distance, const ConstReverseIterator<Container, DataType, Position, Traits>& iterator) 
	{
		ConstReverseIterator<Container, DataType, Position, Traits> tmp_iterator(iterator);
		return (tmp_iterator += distance);
	}

	/** Return the distance between two iterators.
	 *  It points to the element with the given distance in reverse direction
	 *  of the given iterator.
	 *  @exception Exception::InvalidIterator if the iterator is invalid
	 *  @exception Exception::IncompatibleIterators if this iterator and the argument are incompatible
	 */
	template <typename Container, typename DataType, typename Position, typename Traits>
	Distance ConstReverseIterator<Container, DataType, Position, Traits>::operator -
		(const ConstReverseIterator<Container, DataType, Position, Traits>& b) const
	{
		if (!Base::getTraits().isValid())
		{
			Exception::InvalidIterator e;
			throw e;
		}
		if (!b.getTraits().isValid())
		{
			Exception::InvalidIterator e;
			throw e;
		}
		if (Base::getTraits().getContainer() != b.getTraits().getContainer())
		{
			Exception::IncompatibleIterators e;
			throw e;
		}
		return Base::getTraits().getDistance(b.getTraits());
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstReverseIterator<Container, DataType, Position, Traits>&
		ConstReverseIterator<Container, DataType, Position, Traits>::operator += (Distance distance)
	{
		if (!Base::getTraits().isValid())
		{
			Exception::InvalidIterator e;
			throw e;
		}
		if (distance < (Distance)0)
		{
			return (*this -= -distance);
		}
		Base::getTraits().backward(distance);
		return *this;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstReverseIterator<Container, DataType, Position, Traits>&
		ConstReverseIterator<Container, DataType, Position, Traits>::operator -= (Distance distance)
	{
		if (Base::getTraits().isSingular())
		{
			Exception::InvalidIterator e;
			throw e;
		}
		if (distance < (Distance)0)
		{
			return (*this += -distance);
		}
		if (Base::getTraits().isREnd() == true)
		{
			Base::getTraits().toBegin();
			Base::getTraits().forward(distance - 1);
		}
		else 
		{
			Base::getTraits().forward(distance);
		}
		return *this;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstReverseIterator<Container, DataType, Position, Traits>
		ConstReverseIterator<Container, DataType, Position, Traits>::operator + (Distance distance) const
	{
		ConstReverseIterator iterator(*this);
		return (iterator += distance);
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstReverseIterator<Container, DataType, Position, Traits> 
		ConstReverseIterator<Container, DataType, Position, Traits>::operator - (Distance distance) const
	{
		ConstReverseIterator iterator(*this);
		return (iterator -= distance);
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool ConstReverseIterator<Container, DataType, Position, Traits>::operator < 
		(const ConstReverseIterator& iterator) const
	{
		if (!Base::getTraits().isValid())
		{
			Exception::InvalidIterator e;
			throw e;
		}
		if (!iterator.isValid())
		{
			Exception::InvalidIterator e;
			throw e;				
		}
		if (Base::getTraits().getContainer() != iterator.getContainer())
		{
			Exception::IncompatibleIterators e;
			throw e;
		}

		return !(Base::getTraits().operator < (iterator.getTraits()));
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool ConstReverseIterator<Container, DataType, Position, Traits>::operator <= 
		(const ConstReverseIterator& iterator) const
	{
		if (!Base::getTraits().isValid())
		{
			Exception::InvalidIterator e;
			throw e;	
		}
		if (!iterator.isValid())
		{
			Exception::InvalidIterator e;
			throw e;	
		}
		if (Base::getTraits().getContainer() != iterator.getContainer())
		{
			Exception::IncompatibleIterators e;
			throw e;
		}
		return (Base::getTraits().operator > (iterator.getTraits()));
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool ConstReverseIterator<Container, DataType, Position, Traits>::operator >= 
		(const ConstReverseIterator& iterator) const
	{
		if (!Base::getTraits().isValid())
		{
			Exception::InvalidIterator e;
			throw e;
		}
		if (!iterator.isValid())
		{
			Exception::InvalidIterator e;
			throw e;
		}
		if (Base::getTraits().getContainer() != iterator.getContainer())
		{
			Exception::IncompatibleIterators e;
			throw e;
		}

		return (Base::getTraits().operator < (iterator.getTraits()));
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool ConstReverseIterator<Container, DataType, Position, Traits>::operator > 
		(const ConstReverseIterator& iterator) const
	{
		if (!Base::getTraits().isValid())
		{
			Exception::InvalidIterator e;
			throw e;
		}
		if (!iterator.isValid())
		{
			Exception::InvalidIterator e;
			throw e;
		}
		if (Base::getTraits().getContainer() != iterator.getContainer())
		{
			Exception::IncompatibleIterators e;
			throw e;
		}

		return !(Base::getTraits().operator > (iterator.getTraits()));
	}

	
	template <typename Container, typename DataType, typename Position, typename Traits>
	const DataType& ConstReverseIterator<Container, DataType, Position, Traits>::operator [] (Index index) const
	{
		if (!Base::getTraits().isValid())
		{
			Exception::InvalidIterator e;
			throw e;
		}

		return Base::getTraits().getData(index);
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstReverseIterator<Container, DataType, Position, Traits> 
		ConstReverseIterator<Container, DataType, Position, Traits>::begin(const Container& container)
	{
		ConstReverseIterator iterator(container);
		iterator.toRBegin();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstReverseIterator<Container, DataType, Position, Traits> 
		ConstReverseIterator<Container, DataType, Position, Traits>::end(const Container& container)
	{
		ConstReverseIterator iterator(container);
		iterator.toREnd();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstReverseIterator<Container, DataType, Position, Traits> 
		ConstReverseIterator<Container, DataType, Position, Traits>::rbegin(const Container& container)
	{
		ConstReverseIterator iterator(container);
		iterator.toBegin();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstReverseIterator<Container, DataType, Position, Traits> 
		ConstReverseIterator<Container, DataType, Position, Traits>::rend(const Container& container)
	{
		ConstReverseIterator iterator(container);
		iterator.toEnd();
		return iterator;
	}

	/**	@name	Generic Random Access Iterator Classes.
	 	 \ingroup  ConceptsIterators
	*/
	//@{

	/**	Random Access Iterator Class
	*/
	template <typename Container, typename DataType, typename Position, typename Traits>
	class ReverseIterator
		: public ConstReverseIterator<Container, DataType, Position, Traits>
	{
		public:

		/** @name Typedefs
		 */
		//@{
		///
	        typedef DataType& reference;
		///
		typedef DataType* pointer;
		///
		typedef ConstReverseIterator<Container, DataType, Position, Traits> Base;
		//@}

		/**	@name Constructors and Destructors
		*/
		//@{

		///
		ReverseIterator() {}
	
		///
		ReverseIterator(const ReverseIterator& iterator)
			:	Base(iterator)
		{
		}

		///
		~ReverseIterator() {}
		//@}

		/**	@name Dereferenciation
		*/
		//@{
		/** Random access operator.
		 *  @exception Exception::InvalidIterator if the iterator is invalid
		 */
		reference operator [] (Index index) const { return const_cast<reference>(Base::getTraits().getData(index)); }
		///
		reference operator * () const { return const_cast<reference>(Base::getTraits().getData()); }
		///
		pointer operator -> () const { return const_cast<pointer>(&Base::getTraits().getData()); }
		//@}

		/**	@name Assignment
		*/
		//@{
		/** Return a ReverseIterator for a given container.
		 *  It points at the first element.
		 *  @exception Exception::InvalidIterator if the iterator is invalid
		 */
		static ReverseIterator begin(const Container& container);

		/** Return a ReverseIterator for a given container.
		 *  It points behind the last element of the container.
		 *  @exception Exception::InvalidIterator if the iterator is invalid
		 */
		static ReverseIterator end(const Container& container);

		/** Return a ReverseIterator for a given container.
		 *  It points at the last element of the container.
		 *  @exception Exception::InvalidIterator if the iterator is invalid
		 */
		static ReverseIterator rbegin(const Container& container);

		/** Return a ReverseIterator for a given container.
		 *  It points before the first element.
		 *  @exception Exception::InvalidIterator if the iterator is invalid
		 */
		static ReverseIterator rend(const Container& container);
		//@}

		protected:

		ReverseIterator(const Container& container)
			:	Base(container)
		{
		}

	};
	//@}
	
	template <typename Container, typename DataType, typename Position, typename Traits>
	ReverseIterator<Container, DataType, Position, Traits> 
		ReverseIterator<Container, DataType, Position, Traits>::begin(const Container& container)
	{
		ReverseIterator iterator(container);
		iterator.toRBegin();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ReverseIterator<Container, DataType, Position, Traits> 
		ReverseIterator<Container, DataType, Position, Traits>::end(const Container& container)
	{
		ReverseIterator iterator(container);
		iterator.toREnd();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ReverseIterator<Container, DataType, Position, Traits> 
		ReverseIterator<Container, DataType, Position, Traits>::rbegin(const Container& container)
	{
		ReverseIterator iterator(container);
		iterator.toBegin();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ReverseIterator<Container, DataType, Position, Traits> 
		ReverseIterator<Container, DataType, Position, Traits>::rend(const Container& container)
	{
		ReverseIterator iterator(container);
		iterator.toEnd();
		return iterator;
	}

} // namespace BALL 

#endif // BALL_KERNEL_REVERSEITERATOR_H
