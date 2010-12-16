// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_CONCEPT_RANDOMACCESSITERATOR_H
#define BALL_CONCEPT_RANDOMACCESSITERATOR_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_CONCEPT_BIDIRECTIONALITERATOR_H
#	include <BALL/CONCEPT/bidirectionalIterator.h>
#endif

namespace BALL 
{

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
		BALL_INLINE ConstRandomAccessIterator()  {}
	
		///
		BALL_INLINE ConstRandomAccessIterator(const ConstRandomAccessIterator& iterator) 
			:	Base(iterator)
		{
		}

		///
		BALL_INLINE ~ConstRandomAccessIterator()  {}
		//@}



		/**	@name Assignment
		*/
		//@{
		
		/** Advance iterator a given distance.
	 	 *	\throws Exception::InvalidIterator
		 */
		ConstRandomAccessIterator& operator += (Distance distance);

		/** Advance iterator a given distance in reverse direction.
		 *	\throws Exception::InvalidIterator
		 */
		ConstRandomAccessIterator& operator -= (Distance distance);

		/** Return an iterator.
		 *  It points to the element with the given distance from the element of this iterator.
		 *	\throws Exception::InvalidIterator
		 */
		ConstRandomAccessIterator operator + (Distance distance) const;

		/** Return an iterator.
		 *  It points to the element with the given distance in reverse direction
		 *  from the element of this iterator.
		 *	\throws Exception::InvalidIterator
		 */
		ConstRandomAccessIterator operator - (Distance distance) const;

		/** Return an iterator.
		 *  It points to the element with the given distance in reverse direction
		 *  from the element of this iterator.
		 *  \throws Exception::InvalidIterator
		 *  \throws Exception::IncompatibleIterators
		 */
		Distance operator - (const ConstRandomAccessIterator& iterator) const;

		/** Return a ConstRandomAccessIterator for a given container. 
		 *  It points to the first element.
		 *	\throws Exception::InvalidIterator
		 */
		static ConstRandomAccessIterator begin(const Container& container);

		/** Return a Const RandomAccessIterator for a given container.
		 *  It points behind the last element of the container.
		 *  \throws Exception::InvalidIterator
		 */
		static ConstRandomAccessIterator end(const Container& container);

		/** Return a Const RandomAccessIterator for a given container.
		 *  It points at the last element of the container.
		 *	\throws Exception::InvalidIterator
		 */
		static ConstRandomAccessIterator rbegin(const Container& container);

		/** Return a Const RandomAccessIterator for a given container.
		 *  It points before the first element.
		 *	\throws Exception::InvalidIterator
		 */
		static ConstRandomAccessIterator rend(const Container& container);

		//@}

		/**	@name Predicates
		*/
		//@{

		///
		BALL_INLINE bool operator + () const  { return Base::getTraits().isValid(); }

		///
		BALL_INLINE bool operator - () const  { return !Base::getTraits().isValid(); }

    /** Lesser operator.
		 *	Returns true if both iterators have the same container
		 *  and the element of this iterator resides before the elemen of the given iterator.
		 *  @exception InvalidIterator if one of the iterators is not valid
		 *  @exception IncompatibleIterators if the iterators show to different containers.
		 */
		bool operator < (const ConstRandomAccessIterator& iterator) const;

    /** Lesser or equal operator.
		 *  Returns true if both iterators have the same container
		 *  and the element of this iterator resides before the elemen of the given iterator
		 *  or both iterators point to the same element.
		 *  @exception InvalidIterator if one of the iterators is not valid
		 *  @exception IncompatibleIterators if the iterators show to different containers.
		 */
		bool operator <= (const ConstRandomAccessIterator& iterator) const;

    /** Greater or equal operator.
		 *  Returns true if both iterators have the same container
		 *  and the element of this iterator resides after the elemen of the given iterator
		 *  or both iterators point to the same element.
		 *  @exception InvalidIterator if one of the iterators is not valid
		 *  @exception IncompatibleIterators if the iterators show to different containers.
		 */
		bool operator >= (const ConstRandomAccessIterator& iterator) const;

    /** Greater operator.
		 *  Returns true if both iterators have the same container
		 *  and the element of this iterator resides after the elemen of the given iterator.
		 *  @exception InvalidIterator if one of the iterators is not valid
		 *  @exception IncompatibleIterators if the iterators show to different containers.
		 */
		bool operator > (const ConstRandomAccessIterator& iterator) const;

		//@}

			

		/**	@name Iterator dereferencing
		*/
		//@{
		/** Random access operator.
		 *  \throws Exception::InvalidIterator
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
	 *  \throws Exception::InvalidIterator
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
	 *  \throws Exception::InvalidIterator
	 */
	template <typename Container, typename DataType, typename Position, typename Traits>
	Distance ConstRandomAccessIterator<Container, DataType, Position, Traits>::operator -
		(const ConstRandomAccessIterator<Container, DataType, Position, Traits>& b) const
	{
		if (!Base::getTraits().isValid())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}
		if (!b.getTraits().isValid())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}
		if (Base::getTraits().getContainer() != b.getTraits().getContainer())
		{
			throw Exception::IncompatibleIterators(__FILE__, __LINE__);
		}
		return Base::getTraits().getDistance(b.getTraits());
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstRandomAccessIterator<Container, DataType, Position, Traits>&
		ConstRandomAccessIterator<Container, DataType, Position, Traits>::operator += (Distance distance)
	{
		if (!Base::getTraits().isValid())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
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
			throw Exception::InvalidIterator(__FILE__, __LINE__);
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
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}
		if (!iterator.isValid())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);				
		}
		if (Base::getTraits().getContainer() != iterator.getContainer())
		{
			throw Exception::IncompatibleIterators(__FILE__, __LINE__);
		}

		return (Base::getTraits().operator < (iterator.getTraits()));
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool ConstRandomAccessIterator<Container, DataType, Position, Traits>::operator <= 
		(const ConstRandomAccessIterator& iterator) const
	{
		if (!Base::getTraits().isValid())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}
		if (!iterator.isValid())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}
		if (Base::getTraits().getContainer() != iterator.getContainer())
		{
			throw Exception::IncompatibleIterators(__FILE__, __LINE__);
		}
		return !(Base::getTraits().operator > (iterator.getTraits()));
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool ConstRandomAccessIterator<Container, DataType, Position, Traits>::operator >= 
		(const ConstRandomAccessIterator& iterator) const
	{
		if (!Base::getTraits().isValid())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}
		if (!iterator.isValid())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}
		if (Base::getTraits().getContainer() != iterator.getContainer())
		{
			throw Exception::IncompatibleIterators(__FILE__, __LINE__);
		}

		return !(Base::getTraits().operator < (iterator.getTraits()));
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool ConstRandomAccessIterator<Container, DataType, Position, Traits>::operator > 
		(const ConstRandomAccessIterator& iterator) const
	{
		if (!Base::getTraits().isValid())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}
		if (!iterator.isValid())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}
		if (Base::getTraits().getContainer() != iterator.getContainer())
		{
			throw Exception::IncompatibleIterators(__FILE__, __LINE__);
		}

		return (Base::getTraits().operator > (iterator.getTraits()));
	}

	
	template <typename Container, typename DataType, typename Position, typename Traits>
	const DataType& ConstRandomAccessIterator<Container, DataType, Position, Traits>::operator [] (Index index) const
	{
		if (!Base::getTraits().isValid())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
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
		BALL_INLINE RandomAccessIterator()  {}
	
		///
		BALL_INLINE RandomAccessIterator(const RandomAccessIterator& iterator) 
			:	Base(iterator)
		{
		}

		///
		BALL_INLINE ~RandomAccessIterator()  {}
		//@}

		/**	@name Dereferenciation
		*/
		//@{
		/**
		 *  \throws Exception::InvalidIterator
		 */
		BALL_INLINE reference operator [] (Index index) const { return const_cast<reference>(Base::getTraits().getData(index)); }
		///
		BALL_INLINE reference operator * () const  { return const_cast<reference>(Base::getTraits().getData()); }
		///
		BALL_INLINE pointer operator -> () const  { return const_cast<pointer>(&Base::getTraits().getData()); }
		//@}

		/**	@name Assignment
		*/
		//@{
		/** Return a RandomAccessIterator for a given container.
		 *  It points at the first element.
		 *	\throws Exception::InvalidIterator
		 */
		static RandomAccessIterator begin(const Container& container);

		/** Return a RandomAccessIterator for a given container.
		 *  It points behind the last element of the container.
		 *  \throws Exception::InvalidIterator
		 */
		static RandomAccessIterator end(const Container& container);

		/** Return a RandomAccessIterator for a given container.
		 *  It points at the last element of the container.
		 *  \throws Exception::InvalidIterator
		 */
		static RandomAccessIterator rbegin(const Container& container);

		/** Return a RandomAccessIterator for a given container.
		 *  It points before the first element.
		 *  \throws Exception::InvalidIterator
		 */
		static RandomAccessIterator rend(const Container& container);
		//@}

		protected:

		BALL_INLINE RandomAccessIterator(const Container& container) 
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

#endif // BALL_CONCEPT_RANDOMACCESSITERATOR_H
