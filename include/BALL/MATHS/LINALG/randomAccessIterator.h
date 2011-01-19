#ifndef BALL_LINALG_RANDOMACCESSITERATOR_H
#define BALL_LINALG_RANDOMACCESSITERATOR_H

#ifndef BALL_CONCEPT_BIDIRECTIONALITERATOR_H
#include <BALL/CONCEPT/bidirectionalIterator.h>
#endif

namespace BALL 
{
	  ///
	  typedef int Distance;
	  ///
	  typedef int Index;

	/** @name	Generic Constant Random Access Iterator
		\ingroup  ConceptsIterators
	*/
	//@{
	template <typename Container, typename DataType, typename Position, typename Traits>
	class ConstRandomAccessIterator
		: public ConstBidirectionalIterator<Container, DataType, Position, Traits>
	{
		public:

    /** @name Typedefs
     */
    //@{

    ///
    typedef std::random_access_iterator_tag iterator_category;
    // convenience typedef
    typedef ConstBidirectionalIterator<Container, DataType, Position, Traits> Base;
    //@}

		/**	@name	Constructors and Destructors
		*/
		//@{

		///
		ConstRandomAccessIterator() {}
	
		///
		ConstRandomAccessIterator(const ConstRandomAccessIterator& iterator)
			:	Base(iterator)
		{
		}

		///
		~ConstRandomAccessIterator() {}
		//@}



		/**	@name Assignment
		*/
		//@{
		
		/** Advance iterator a given distance.
		 *  @exception Exception::InvalidIterator if the iterator is invalid
		 */
		ConstRandomAccessIterator& operator += (Distance distance);

		/** Advance iterator a given distance in reverse direction.
		 *  @exception Exception::InvalidIterator if the iterator is invalid
		 */
		ConstRandomAccessIterator& operator -= (Distance distance);

		/** Return an iterator.
		 *  It points to the element with the given distance from the element of this iterator.
		 *  @exception Exception::InvalidIterator if the iterator is invalid
		 */
		ConstRandomAccessIterator operator + (Distance distance) const;

		/** Return an iterator.
		 *  It points to the element with the given distance in reverse direction
		 *  from the element of this iterator.
		 *  @exception Exception::InvalidIterator if the iterator is invalid
		 */
		ConstRandomAccessIterator operator - (Distance distance) const;

		/** Return an iterator.
		 *  It points to the element with the given distance in reverse direction
		 *  from the element of this iterator.
		 *  @exception Exception::InvalidIterator if the iterator is invalid
		 *  @exception Exception::IncompatibleIterators if this iterator and the argument are incompatible
		 */
		Distance operator - (const ConstRandomAccessIterator& iterator) const;

		/** Return a ConstRandomAccessIterator for a given container. 
		 *  It points to the first element.
		 *  @excepttion Exception::Precondition
		 */
		static ConstRandomAccessIterator begin(const Container& container);

		/** Return a Const RandomAccessIterator for a given container.
		 *  It points behind the last element of the container.
		 *  @excepttion Exception::Precondition
		 */
		static ConstRandomAccessIterator end(const Container& container);

		/** Return a Const RandomAccessIterator for a given container.
		 *  It points at the last element of the container.
		 *  @excepttion Exception::Precondition
		 */
		static ConstRandomAccessIterator rbegin(const Container& container);

		/** Return a Const RandomAccessIterator for a given container.
		 *  It points before the first element.
		 *  @excepttion Exception::Precondition
		 */
		static ConstRandomAccessIterator rend(const Container& container);

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
		 *  @exception IncompatibleIterators if the iterators point into different containers.
		 */
		bool operator < (const ConstRandomAccessIterator& iterator) const;

    /** Lesser or equal operator.
		 *  Returns true if both iterators have the same container
		 *  and the element of this iterator resides before the elemen of the given iterator
		 *  or both iterators point to the same element.
		 *  @exception InvalidIterator if one of the iterators is not valid
		 *  @exception IncompatibleIterators if the iterators point into different containers.
		 */
		bool operator <= (const ConstRandomAccessIterator& iterator) const;

    /** Greater or equal operator.
		 *  Returns true if both iterators have the same container
		 *  and the element of this iterator resides after the elemen of the given iterator
		 *  or both iterators point to the same element.
		 *  @exception InvalidIterator if one of the iterators is not valid
		 *  @exception IncompatibleIterators if the iterators point into different containers.
		 */
		bool operator >= (const ConstRandomAccessIterator& iterator) const;

    /** Greater operator.
		 *  Returns true if both iterators have the same container
		 *  and the element of this iterator resides after the elemen of the given iterator.
		 *  @exception InvalidIterator if one of the iterators is not valid
		 *  @exception IncompatibleIterators if the iterators point into different containers.
		 */
		bool operator > (const ConstRandomAccessIterator& iterator) const;

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

		ConstRandomAccessIterator(const Container& container)
			:	Base(container)
		{
		}
	};
	//@}
	
	/** Return an incremented iterator.
	 *  It points to the element with the given distance from the element 
	 *  of the given iterator.
	 *  @exception Exception::InvalidIterator if the iterator is invalid
	 */
	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstRandomAccessIterator<Container, DataType, Position, Traits> operator + 
		(Distance distance, const ConstRandomAccessIterator<Container, DataType, Position, Traits>& iterator) 
	{
		ConstRandomAccessIterator<Container, DataType, Position, Traits> tmp_iterator(iterator);
		return (tmp_iterator += distance);
	}

	/** Return the distance between two iterators.
	 *  It points to the element with the given distance in reverse direction
	 *  of the given iterator.
	 *  @exception Exception::InvalidIterator if the iterator is invalid
	 *  @exception Exception::IncompatibleIterators if this iterator and the argument are incompatible
	 */
	template <typename Container, typename DataType, typename Position, typename Traits>
	Distance ConstRandomAccessIterator<Container, DataType, Position, Traits>::operator -
		(const ConstRandomAccessIterator<Container, DataType, Position, Traits>& b) const
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
	ConstRandomAccessIterator<Container, DataType, Position, Traits>&
		ConstRandomAccessIterator<Container, DataType, Position, Traits>::operator += (Distance distance)
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
		Base::getTraits().forward(distance);
		return *this;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstRandomAccessIterator<Container, DataType, Position, Traits>&
		ConstRandomAccessIterator<Container, DataType, Position, Traits>::operator -= (Distance distance)
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
		if (Base::getTraits().isEnd() == true)
		{
			Base::getTraits().toRBegin();
			Base::getTraits().backward(distance - 1);
		}
		else 
		{
			Base::getTraits().backward(distance);
		}
		return *this;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstRandomAccessIterator<Container, DataType, Position, Traits>
		ConstRandomAccessIterator<Container, DataType, Position, Traits>::operator + (Distance distance) const
	{
		ConstRandomAccessIterator iterator(*this);
		return (iterator += distance);
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstRandomAccessIterator<Container, DataType, Position, Traits> 
		ConstRandomAccessIterator<Container, DataType, Position, Traits>::operator - (Distance distance) const
	{
		ConstRandomAccessIterator iterator(*this);
		return (iterator -= distance);
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool ConstRandomAccessIterator<Container, DataType, Position, Traits>::operator < 
		(const ConstRandomAccessIterator& iterator) const
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
	bool ConstRandomAccessIterator<Container, DataType, Position, Traits>::operator <= 
		(const ConstRandomAccessIterator& iterator) const
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
	bool ConstRandomAccessIterator<Container, DataType, Position, Traits>::operator >= 
		(const ConstRandomAccessIterator& iterator) const
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
	bool ConstRandomAccessIterator<Container, DataType, Position, Traits>::operator > 
		(const ConstRandomAccessIterator& iterator) const
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
	const DataType& ConstRandomAccessIterator<Container, DataType, Position, Traits>::operator [] (Index index) const
	{
		if (!Base::getTraits().isValid())
		{
			Exception::InvalidIterator e;
			throw e;
		}

		return Base::getTraits().getData(index);
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstRandomAccessIterator<Container, DataType, Position, Traits> 
		ConstRandomAccessIterator<Container, DataType, Position, Traits>::begin(const Container& container)
	{
		ConstRandomAccessIterator iterator(container);
		iterator.toBegin();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstRandomAccessIterator<Container, DataType, Position, Traits> 
		ConstRandomAccessIterator<Container, DataType, Position, Traits>::end(const Container& container)
	{
		ConstRandomAccessIterator iterator(container);
		iterator.toEnd();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstRandomAccessIterator<Container, DataType, Position, Traits> 
		ConstRandomAccessIterator<Container, DataType, Position, Traits>::rbegin(const Container& container)
	{
		ConstRandomAccessIterator iterator(container);
		iterator.toRBegin();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstRandomAccessIterator<Container, DataType, Position, Traits> 
		ConstRandomAccessIterator<Container, DataType, Position, Traits>::rend(const Container& container)
	{
		ConstRandomAccessIterator iterator(container);
		iterator.toREnd();
		return iterator;
	}

	/**	@name	Generic Random Access Iterator Classes.
	 	 \ingroup  ConceptsIterators
	*/
	//@{

	/**	Random Access Iterator Class
	*/
	template <typename Container, typename DataType, typename Position, typename Traits>
	class RandomAccessIterator
		: public ConstRandomAccessIterator<Container, DataType, Position, Traits>
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
		typedef ConstRandomAccessIterator<Container, DataType, Position, Traits> Base;
		//@}

		/**	@name Constructors and Destructors
		*/
		//@{

		///
		RandomAccessIterator() {}
	
		///
		RandomAccessIterator(const RandomAccessIterator& iterator)
			:	Base(iterator)
		{
		}

		///
		~RandomAccessIterator() {}
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
		/** Return a RandomAccessIterator for a given container.
		 *  It points at the first element.
		 *  @exception Exception::InvalidIterator if the iterator is invalid
		 */
		static RandomAccessIterator begin(const Container& container);

		/** Return a RandomAccessIterator for a given container.
		 *  It points behind the last element of the container.
		 *  @exception Exception::InvalidIterator if the iterator is invalid
		 */
		static RandomAccessIterator end(const Container& container);

		/** Return a RandomAccessIterator for a given container.
		 *  It points at the last element of the container.
		 *  @exception Exception::InvalidIterator if the iterator is invalid
		 */
		static RandomAccessIterator rbegin(const Container& container);

		/** Return a RandomAccessIterator for a given container.
		 *  It points before the first element.
		 *  @exception Exception::InvalidIterator if the iterator is invalid
		 */
		static RandomAccessIterator rend(const Container& container);
		//@}

		protected:

		RandomAccessIterator(const Container& container)
			:	Base(container)
		{
		}

	};
	//@}
	
	template <typename Container, typename DataType, typename Position, typename Traits>
	RandomAccessIterator<Container, DataType, Position, Traits> 
		RandomAccessIterator<Container, DataType, Position, Traits>::begin(const Container& container)
	{
		RandomAccessIterator iterator(container);
		iterator.toBegin();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	RandomAccessIterator<Container, DataType, Position, Traits> 
		RandomAccessIterator<Container, DataType, Position, Traits>::end(const Container& container)
	{
		RandomAccessIterator iterator(container);
		iterator.toEnd();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	RandomAccessIterator<Container, DataType, Position, Traits> 
		RandomAccessIterator<Container, DataType, Position, Traits>::rbegin(const Container& container)
	{
		RandomAccessIterator iterator(container);
		iterator.toRBegin();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	RandomAccessIterator<Container, DataType, Position, Traits> 
		RandomAccessIterator<Container, DataType, Position, Traits>::rend(const Container& container)
	{
		RandomAccessIterator iterator(container);
		iterator.toREnd();
		return iterator;
	}

} // namespace BALL 

#endif // BALL_KERNEL_RANDOMACCESSITERATOR_H
