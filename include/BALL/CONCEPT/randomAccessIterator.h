// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: randomAccessIterator.h,v 1.25 2003/06/19 20:35:21 oliver Exp $ 
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

	/**	Generic Constant Random Access Iterator
			 \par
			
	*/
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
		//@}

		/**	@name	Constructors and Destructors
		*/
		//@{

		/** Default Constructor.
		*/
		ConstRandomAccessIterator()
			throw();
	
		/** Copy Constructor.
		*/
		ConstRandomAccessIterator(const ConstRandomAccessIterator& iterator)
			throw();

		/** Detailed Constructor.
		*/
		ConstRandomAccessIterator(const ConstBidirectionalIterator<Container, DataType, Position, Traits>& iterator)
			throw();
		
		/** Destructor.
		 */
		~ConstRandomAccessIterator()
			throw();

		//@}

    /** Move the iterator to the first item of the container.
		 */
		void toBegin()
			throw(Exception::InvalidIterator);

    /** Check whether the iterator points to first item of the container
		 */
		bool isBegin() const
			throw(Exception::InvalidIterator);

		/** Move the iterator behind the last element of the container.
		 */
		void toEnd()
			throw(Exception::InvalidIterator);

		/** Check whether the iterator points the last element of the container.
		 */
		bool isEnd() const
			throw(Exception::InvalidIterator);

		/** Set the iterator to the item at the end of the container.
		 */
		void toRBegin()
			throw(Exception::InvalidIterator);

		/** Test if iterator points to reverse first element.
		 */
		bool isRBegin() const
			throw(Exception::InvalidIterator);

		/** Set iterator to reverse end.
		 */
		void toREnd()
			throw(Exception::InvalidIterator);

		/** Test if iterator points to reverse end.
		 */
		bool isREnd() const
			throw(Exception::InvalidIterator);

		/** Advance iterator a given distance.
		 */
		ConstRandomAccessIterator& operator += (Distance distance)
			throw(Exception::InvalidIterator);

		/** Advance iterator a given distance in reverse direction.
		 */
		ConstRandomAccessIterator& operator -= (Distance distance)
			throw(Exception::InvalidIterator);

		/** Return an iterator.
		 *  It points to the element with the given distance from the element of this iterator.
		 */
		ConstRandomAccessIterator operator + (Distance distance) const
			throw(Exception::InvalidIterator);

		///
		inline bool operator + () const throw() 
		{ 
			return ConstBidirectionalIterator<Container, DataType, Position, Traits>::operator + (); 
		}

		///
		inline bool operator - () const throw() 
		{ 
			return ConstBidirectionalIterator<Container, DataType, Position, Traits>::operator - (); 
		}

		/** Return an iterator.
		 *  It points to the element with the given distance in reverse direction
		 *  from the element of this iterator.
		 */
		ConstRandomAccessIterator operator - (Distance distance) const
			throw(Exception::InvalidIterator);

		/** Return an iterator.
		 *  It points to the element with the given distance in reverse direction
		 *  from the element of this iterator.
		 */
		Distance operator - (const ConstRandomAccessIterator& iterator) const
			throw(Exception::InvalidIterator, Exception::IncompatibleIterators);

    /** Lesser operator.
		 *	Returns true if both iterators have the same container
		 *  and the element of this iterator resides before the elemen of the given iterator.
		 *  @exception InvalidIterator if one of the iterators is not valid
		 *  @exception IncompatibleIterators if the iterators show to different containers.
		 */
		bool operator < (const ConstRandomAccessIterator& iterator) const
			throw(Exception::InvalidIterator, Exception::IncompatibleIterators);

    /** Lesser or equal operator.
		 *  Returns true if both iterators have the same container
		 *  and the element of this iterator resides before the elemen of the given iterator
		 *  or both iterators point to the same element.
		 *  @exception InvalidIterator if one of the iterators is not valid
		 *  @exception IncompatibleIterators if the iterators show to different containers.
		 */
		bool operator <= (const ConstRandomAccessIterator& iterator) const
			throw(Exception::InvalidIterator, Exception::IncompatibleIterators);

    /** Greater or equal operator.
		 *  Returns true if both iterators have the same container
		 *  and the element of this iterator resides after the elemen of the given iterator
		 *  or both iterators point to the same element.
		 *  @exception InvalidIterator if one of the iterators is not valid
		 *  @exception IncompatibleIterators if the iterators show to different containers.
		 */
		bool operator >= (const ConstRandomAccessIterator& iterator) const
			throw(Exception::InvalidIterator, Exception::IncompatibleIterators);

    /** Greater operator.
		 *  Returns true if both iterators have the same container
		 *  and the element of this iterator resides after the elemen of the given iterator.
		 *  @exception InvalidIterator if one of the iterators is not valid
		 *  @exception IncompatibleIterators if the iterators show to different containers.
		 */
		bool operator > (const ConstRandomAccessIterator& iterator) const
			throw(Exception::InvalidIterator, Exception::IncompatibleIterators);

		/** Random access operator.
		 */
		const DataType& operator [] (Index index) const
			throw(Exception::InvalidIterator);

		/** Return a ConstRandomAccessIterator for a given container. 
		 *  It points at the first element.
		 */
		static ConstRandomAccessIterator begin(const Container& container) 
			throw(Exception::InvalidIterator);

		/** Return a Const RandomAccessIterator for a given container.
		 *  It points behind the last element of the container.
		 */
		static ConstRandomAccessIterator end(const Container& container)
			throw(Exception::InvalidIterator);

		/** Return a Const RandomAccessIterator for a given container.
		 *  It points at the last element of the container.
		 */
		static ConstRandomAccessIterator rbegin(const Container& container)
			throw(Exception::InvalidIterator);

		/** Return a Const RandomAccessIterator for a given container.
		 *  It points before the first element.
		 */
		static ConstRandomAccessIterator rend(const Container& container) 
			throw(Exception::InvalidIterator);

		protected:

		/*_ Constructor.
				The iterator will point at the first element of the container.
		 */
		ConstRandomAccessIterator(const Container& container)
			throw();

	};

	/** Return an incremented iterator.
	 *  It points to the element with the given distance from the element 
	 *  of the given iterator.
	 */
	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstRandomAccessIterator<Container, DataType, Position, Traits> operator + 
		(Distance distance, const ConstRandomAccessIterator<Container, DataType, Position, Traits>& iterator) 
		throw(Exception::InvalidIterator)
	{
		ConstRandomAccessIterator<Container, DataType, Position, Traits> tmp_iterator(iterator);
		return (tmp_iterator += distance);
	}

	/** Return the distance between two iterators.
	 *  It points to the element with the given distance in reverse direction
	 *  of the given iterator.
	 */
	template <typename Container, typename DataType, typename Position, typename Traits>
	Distance ConstRandomAccessIterator<Container, DataType, Position, Traits>::operator -
		(const ConstRandomAccessIterator<Container, DataType, Position, Traits>& b) const
		throw(Exception::InvalidIterator, Exception::IncompatibleIterators)
	{
		if (!getTraits().isValid())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}
		if (!b.getTraits().isValid())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}
		if (getTraits().getContainer() != b.getTraits().getContainer())
		{
			throw Exception::IncompatibleIterators(__FILE__, __LINE__);
		}
		return getTraits().getDistance(b.getTraits());
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstRandomAccessIterator<Container, DataType, Position, Traits>::ConstRandomAccessIterator()
		throw()
		:	ConstBidirectionalIterator<Container, DataType, Position, Traits>()
	{
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstRandomAccessIterator<Container, DataType, Position, Traits>::ConstRandomAccessIterator
		(const ConstRandomAccessIterator& iterator) throw()
		:	ConstBidirectionalIterator<Container, DataType, Position, Traits>(iterator)
	{
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstRandomAccessIterator<Container, DataType, Position, Traits>::ConstRandomAccessIterator
		(const ConstBidirectionalIterator<Container, DataType, Position, Traits>& iterator) throw()
		:	ConstBidirectionalIterator<Container, DataType, Position, Traits>(iterator)
	{
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstRandomAccessIterator<Container, DataType, Position, Traits>::~ConstRandomAccessIterator()
		throw()
	{
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	void ConstRandomAccessIterator<Container, DataType, Position, Traits>::toBegin()
		throw(Exception::InvalidIterator)
	{
		if (ConstBidirectionalIterator<Container, DataType, Position, Traits>::getTraits().isSingular())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}
		ConstBidirectionalIterator<Container, DataType, Position, Traits>::getTraits().toBegin();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool ConstRandomAccessIterator<Container, DataType, Position, Traits>::isBegin() const
		throw(Exception::InvalidIterator)
	{
		if (ConstBidirectionalIterator<Container, DataType, Position, Traits>::getTraits().isSingular())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}
		return ConstBidirectionalIterator<Container, DataType, Position, Traits>::getTraits().isBegin();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	void ConstRandomAccessIterator<Container, DataType, Position, Traits>::toEnd()
		throw(Exception::InvalidIterator)
	{
		if (ConstBidirectionalIterator<Container, DataType, Position, Traits>::getTraits().isSingular())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}
		ConstBidirectionalIterator<Container, DataType, Position, Traits>::getTraits().toEnd();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool ConstRandomAccessIterator<Container, DataType, Position, Traits>::isEnd() const
		throw(Exception::InvalidIterator)
	{
		if (ConstBidirectionalIterator<Container, DataType, Position, Traits>::getTraits().isSingular())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}
		return ConstBidirectionalIterator<Container, DataType, Position, Traits>::getTraits().isEnd();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	void ConstRandomAccessIterator<Container, DataType, Position, Traits>::toRBegin()
		throw(Exception::InvalidIterator)
	{
		if (ConstBidirectionalIterator<Container, DataType, Position, Traits>::getTraits().isSingular())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}
		ConstBidirectionalIterator<Container, DataType, Position, Traits>::getTraits().toRBegin();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool ConstRandomAccessIterator<Container, DataType, Position, Traits>::isRBegin() const
		throw(Exception::InvalidIterator)
	{
		if (ConstBidirectionalIterator<Container, DataType, Position, Traits>::getTraits().isSingular())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}

		return ConstBidirectionalIterator<Container, DataType, Position, Traits>::getTraits().isRBegin();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	void ConstRandomAccessIterator<Container, DataType, Position, Traits>::toREnd()
		throw(Exception::InvalidIterator)
	{
		if (ConstBidirectionalIterator<Container, DataType, Position, Traits>::getTraits().isSingular())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}	

		ConstBidirectionalIterator<Container, DataType, Position, Traits>::getTraits().toREnd();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool ConstRandomAccessIterator<Container, DataType, Position, Traits>::isREnd() const
		throw(Exception::InvalidIterator)
	{
		if (ConstBidirectionalIterator<Container, DataType, Position, Traits>::getTraits().isSingular())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}
		return ConstBidirectionalIterator<Container, DataType, Position, Traits>::getTraits().isREnd();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstRandomAccessIterator<Container, DataType, Position, Traits>&
		ConstRandomAccessIterator<Container, DataType, Position, Traits>::operator += (Distance distance)
		throw(Exception::InvalidIterator)
	{
		if (!ConstBidirectionalIterator<Container, DataType, Position, Traits>::getTraits().isValid())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}
		if (distance < (Distance)0)
		{
			return (*this -= -distance);
		}
		ConstBidirectionalIterator<Container, DataType, Position, Traits>::getTraits().forward(distance);
		return *this;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstRandomAccessIterator<Container, DataType, Position, Traits>&
		ConstRandomAccessIterator<Container, DataType, Position, Traits>::operator -= (Distance distance)
		throw(Exception::InvalidIterator)
	{
		if (ConstBidirectionalIterator<Container, DataType, Position, Traits>::getTraits().isSingular())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}
		if (distance < (Distance)0)
		{
			return (*this += -distance);
		}
		if (ConstBidirectionalIterator<Container, DataType, Position, Traits>::getTraits().isEnd() == true)
		{
			ConstBidirectionalIterator<Container, DataType, Position, Traits>::getTraits().toRBegin();
			ConstBidirectionalIterator<Container, DataType, Position, Traits>::getTraits().backward(distance - 1);
		}
		else 
		{
			ConstBidirectionalIterator<Container, DataType, Position, Traits>::getTraits().backward(distance);
		}
		return *this;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstRandomAccessIterator<Container, DataType, Position, Traits>
		ConstRandomAccessIterator<Container, DataType, Position, Traits>::operator + (Distance distance) const
		throw(Exception::InvalidIterator)
	{
		ConstRandomAccessIterator iterator(*this);
		return (iterator += distance);
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstRandomAccessIterator<Container, DataType, Position, Traits> 
		ConstRandomAccessIterator<Container, DataType, Position, Traits>::operator - (Distance distance) const
		throw(Exception::InvalidIterator)
	{
		ConstRandomAccessIterator iterator(*this);
		return (iterator -= distance);
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool ConstRandomAccessIterator<Container, DataType, Position, Traits>::operator < 
		(const ConstRandomAccessIterator& iterator) const
		throw(Exception::InvalidIterator, Exception::IncompatibleIterators)
	{
		if (!ConstBidirectionalIterator<Container, DataType, Position, Traits>::getTraits().isValid())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}
		if (!iterator.isValid())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);				
		}
		if (ConstBidirectionalIterator<Container, DataType, Position, Traits>::getTraits().getContainer() != iterator.getContainer())
		{
			throw Exception::IncompatibleIterators(__FILE__, __LINE__);
		}

		return (ConstBidirectionalIterator<Container, DataType, Position, Traits>::getTraits().operator < (iterator.getTraits()));
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool ConstRandomAccessIterator<Container, DataType, Position, Traits>::operator <= 
		(const ConstRandomAccessIterator& iterator) const
		throw(Exception::InvalidIterator, Exception::IncompatibleIterators)
	{
		if (!ConstBidirectionalIterator<Container, DataType, Position, Traits>::getTraits().isValid())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}
		if (!iterator.isValid())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}
		if (ConstBidirectionalIterator<Container, DataType, Position, Traits>::getTraits().getContainer() != iterator.getContainer())
		{
			throw Exception::IncompatibleIterators(__FILE__, __LINE__);
		}
		return !(ConstBidirectionalIterator<Container, DataType, Position, Traits>::getTraits().operator > (iterator.getTraits()));
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool ConstRandomAccessIterator<Container, DataType, Position, Traits>::operator >= 
		(const ConstRandomAccessIterator& iterator) const
		throw(Exception::InvalidIterator, Exception::IncompatibleIterators)
	{
		if (!ConstBidirectionalIterator<Container, DataType, Position, Traits>::getTraits().isValid())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}
		if (!iterator.isValid())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}
		if (ConstBidirectionalIterator<Container, DataType, Position, Traits>::getTraits().getContainer() != iterator.getContainer())
		{
			throw Exception::IncompatibleIterators(__FILE__, __LINE__);
		}

		return !(ConstBidirectionalIterator<Container, DataType, Position, Traits>::getTraits().operator < (iterator.getTraits()));
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool ConstRandomAccessIterator<Container, DataType, Position, Traits>::operator > 
		(const ConstRandomAccessIterator& iterator) const
		throw(Exception::InvalidIterator, Exception::IncompatibleIterators)
	{
		if (!ConstBidirectionalIterator<Container, DataType, Position, Traits>::getTraits().isValid())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}
		if (!iterator.isValid())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}
		if (ConstBidirectionalIterator<Container, DataType, Position, Traits>::getTraits().getContainer() != iterator.getContainer())
		{
			throw Exception::IncompatibleIterators(__FILE__, __LINE__);
		}

		return (ConstBidirectionalIterator<Container, DataType, Position, Traits>::getTraits().operator > (iterator.getTraits()));
	}

	
	template <typename Container, typename DataType, typename Position, typename Traits>
	const DataType& ConstRandomAccessIterator<Container, DataType, Position, Traits>::operator [] (Index index) const
		throw(Exception::InvalidIterator)
	{
		if (!ConstBidirectionalIterator<Container, DataType, Position, Traits>::getTraits().isValid())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}

		return ConstBidirectionalIterator<Container, DataType, Position, Traits>::getTraits().getData(index);
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstRandomAccessIterator<Container, DataType, Position, Traits> 
		ConstRandomAccessIterator<Container, DataType, Position, Traits>::begin(const Container& container)
		throw(Exception::InvalidIterator)
	{
		ConstRandomAccessIterator iterator(container);
		iterator.toBegin();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstRandomAccessIterator<Container, DataType, Position, Traits> 
		ConstRandomAccessIterator<Container, DataType, Position, Traits>::end(const Container& container)
		throw(Exception::InvalidIterator)
	{
		ConstRandomAccessIterator iterator(container);
		iterator.toEnd();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstRandomAccessIterator<Container, DataType, Position, Traits> 
		ConstRandomAccessIterator<Container, DataType, Position, Traits>::rbegin(const Container& container)
		throw(Exception::InvalidIterator)
	{
		ConstRandomAccessIterator iterator(container);
		iterator.toRBegin();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstRandomAccessIterator<Container, DataType, Position, Traits> 
		ConstRandomAccessIterator<Container, DataType, Position, Traits>::rend(const Container& container)
		throw(Exception::InvalidIterator)
	{
		ConstRandomAccessIterator iterator(container);
		iterator.toREnd();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstRandomAccessIterator<Container, DataType, Position, Traits>::ConstRandomAccessIterator(const Container& container)
		throw()
		:	ConstBidirectionalIterator<Container, DataType, Position, Traits>(container)
	{
	}

	//@}

	/**	@name	Generic Random Access Iterator Classes.
			 \par
			
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
		//@}
		/**	@name Constructors and Destructors
		*/
		//@{

		/** Default constructor
		*/
		RandomAccessIterator()
			throw();
	
		/** Copy constructor
		*/
		RandomAccessIterator(const RandomAccessIterator& iterator)
			throw();

		/** Destructor
		 */
		~RandomAccessIterator()
			throw();
		//@}

		/** Random access operator.
		 */
		DataType& operator [] (Index index) const
			throw(Exception::InvalidIterator);

		///
		reference operator * () const throw(Exception::InvalidIterator);

		///
		pointer operator -> () const throw(Exception::InvalidIterator);

		/** Return a RandomAccessIterator for a given container.
		 *  It points at the first element.
		 */
		static RandomAccessIterator begin(const Container& container)
			throw(Exception::InvalidIterator);

		/** Return a RandomAccessIterator for a given container.
		 *  It points behind the last element of the container.
		 */
		static RandomAccessIterator end(const Container& container)
			throw(Exception::InvalidIterator);

		/** Return a RandomAccessIterator for a given container.
		 *  It points at the last element of the container.
		 */
		static RandomAccessIterator rbegin(const Container& container)
			throw(Exception::InvalidIterator);

		/** Return a RandomAccessIterator for a given container.
		 *  It points before the first element.
		 */
		static RandomAccessIterator rend(const Container& container)
			throw(Exception::InvalidIterator);

		//@}

		protected:

		/*_ Constructor.
				The iterator will point at the first element of the container.
		 */
		RandomAccessIterator(const Container& container)
			throw();

	};

	template <typename Container, typename DataType, typename Position, typename Traits>
	RandomAccessIterator<Container, DataType, Position, Traits>::RandomAccessIterator()
		throw()
		:	ConstRandomAccessIterator<Container, DataType, Position, Traits>()
	{
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	RandomAccessIterator<Container, DataType, Position, Traits>::RandomAccessIterator
		(const RandomAccessIterator& iterator)
		throw()
		:	ConstRandomAccessIterator<Container, DataType, Position, Traits>(iterator)
	{
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	RandomAccessIterator<Container, DataType, Position, Traits>::~RandomAccessIterator()
		throw()
	{
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	DataType& RandomAccessIterator<Container, DataType, Position, Traits>::operator [] (Index index) const
		throw(Exception::InvalidIterator)
	{
		#ifdef BALL_DEBUG
			if (!RandomAccessIterator<Container, DataType, Position, Traits>::getTraits().isValid())
			{
				throw Exception::InvalidIterator(__FILE__, __LINE__);
			}
		#endif

		return (reference)ConstRandomAccessIterator<Container, DataType, Position, Traits>::getTraits().getData(index);
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	DataType& RandomAccessIterator<Container, DataType, Position, Traits>::operator * () const
		throw(Exception::InvalidIterator)
	{
		#ifdef BALL_DEBUG
			if (!RandomAccessIterator<Container, DataType, Position, Traits>::getTraits().isValid())
			{
				throw Exception::InvalidIterator(__FILE__, __LINE__);
			}
		#endif

		return (reference)ConstRandomAccessIterator<Container, DataType, Position, Traits>::getTraits().getData();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	DataType* RandomAccessIterator<Container, DataType, Position, Traits>::operator -> () const
		throw(Exception::InvalidIterator)
	{
		#ifdef BALL_DEBUG
			if (!RandomAccessIterator<Container, DataType, Position, Traits>::getTraits().isValid())
			{
				throw Exception::InvalidIterator(__FILE__, __LINE__);
			}
		#endif

		return (pointer)&ConstRandomAccessIterator<Container, DataType, Position, Traits>::getTraits().getData();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	RandomAccessIterator<Container, DataType, Position, Traits> 
		RandomAccessIterator<Container, DataType, Position, Traits>::begin(const Container& container)
		throw(Exception::InvalidIterator)
	{
		RandomAccessIterator iterator(container);
		iterator.toBegin();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	RandomAccessIterator<Container, DataType, Position, Traits> 
		RandomAccessIterator<Container, DataType, Position, Traits>::end(const Container& container)
		throw(Exception::InvalidIterator)
	{
		RandomAccessIterator iterator(container);
		iterator.toEnd();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	RandomAccessIterator<Container, DataType, Position, Traits> 
		RandomAccessIterator<Container, DataType, Position, Traits>::rbegin(const Container& container)
		throw(Exception::InvalidIterator)
	{
		RandomAccessIterator iterator(container);
		iterator.toRBegin();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	RandomAccessIterator<Container, DataType, Position, Traits> 
		RandomAccessIterator<Container, DataType, Position, Traits>::rend(const Container& container)
		throw(Exception::InvalidIterator)
	{
		RandomAccessIterator iterator(container);
		iterator.toREnd();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	RandomAccessIterator<Container, DataType, Position, Traits>::RandomAccessIterator(const Container& container)
		throw()
		:	ConstRandomAccessIterator<Container, DataType, Position, Traits>(container)
	{
	}


} // namespace BALL 

#endif // BALL_CONCEPT_RANDOMACCESSITERATOR_H
