// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: randomAccessIterator.h,v 1.23 2003/05/28 16:50:24 oliver Exp $ 
//

#ifndef BALL_CONCEPT_RANDOMACCESSITERATOR_H
#define BALL_CONCEPT_RANDOMACCESSITERATOR_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_CONCEPT_BASEITERATOR_H
#	include <BALL/CONCEPT/baseIterator.h>
#endif

#ifndef BALL_CONCEPT_PREDICATE_H
#	include <BALL/CONCEPT/predicate.h>
#endif

namespace BALL 
{

	/**	@name	Generic Random Access Iterator Classes.
			 \par
			
	 	 \ingroup  ConceptsIterators
	*/
	//@{

	/**	Generic Random Access Iterator Class
	*/
	template <typename Container, typename DataType, typename Position, typename Traits>
	class RandomAccessIterator
		: public BaseIterator<Container, DataType, Position, Traits>
	{
		public:

		BALL_CREATE(RandomAccessIterator)

		/** @name Typedefs
		 */
		//@{

		/** Tag this iterator as a random access iterator according to the STL scheme.
		 */
		typedef std::random_access_iterator_tag iterator_category;

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

		/** Detailed constructor
		*/
		RandomAccessIterator(const BaseIterator<Container, DataType, Position, Traits>& iterator)
			throw();

		/** Destructor
		 */
		~RandomAccessIterator()
			throw();

		//@}
		/** @name Iterator methods
		 */
		//@{

		/** BALL specific shortcut for validity checking.
		 */
		bool operator + () const
			throw();

		/** Negated validity checking.
		 *  Returns false if valid, true if invalid.
		 */
		bool operator - () const
			throw();

		/** Move the iterator to the item at the beginning of the container.
		 */
		void toBegin()
			throw(Exception::InvalidIterator);

		/** Check whether the iterator points to the first item of the container
		 */
		bool isBegin() const
			throw(Exception::InvalidIterator);

		/** Move the iterator behind the last item of the container.
		 */
		void toEnd()
			throw(Exception::InvalidIterator);

		/** Check whether the iterator points behind the last item of the container.
		 */
		bool isEnd() const
			throw(Exception::InvalidIterator);

		/** Set iterator to reverse first element.
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

		/** Prefix increment operator.
		 */
		RandomAccessIterator& operator ++ ()
			throw(Exception::InvalidIterator);

		/** Postfix increment operator. 
		 */
		RandomAccessIterator operator ++ (int)
			throw(Exception::InvalidIterator);

		/** Prefix decrement operator.
		 */
		RandomAccessIterator& operator -- ()
			throw(Exception::InvalidIterator);

		/** Postfix decrement operator.
		 */
		RandomAccessIterator operator -- (int)
			throw(Exception::InvalidIterator);

		/** Advance iterator a given distance.
		 */
		RandomAccessIterator& operator += (Distance distance)
			throw(Exception::InvalidIterator);

		/** Advance iterator a given distance in reverse direction.
		 */
		RandomAccessIterator& operator -= (Distance distance)
			throw(Exception::InvalidIterator);

		/** Return an iterator.
		 * 	It points to the element with the given distance from the element of this iterator.
		 */
		RandomAccessIterator operator + (Distance distance) const
			throw(Exception::InvalidIterator);

		/** Return an iterator.
		 * 	It points to the element with the given distance in reverse direction 
		 * 	from the element of this iterator.
		 */
		RandomAccessIterator operator - (Distance distance) const
			throw(Exception::InvalidIterator);
		
		/** Return the distance between two iterators.
				@exception IncompatibleIterators if the iterators have different containers
		*/
		Distance operator -	(const RandomAccessIterator& iterator) const
			throw(Exception::InvalidIterator, Exception::IncompatibleIterators);
		
		/** Lesser operator.
		 * 	Returns true if both iterators have the same container
		 * 	and the element of this iterator resides before the element of the given iterator.
		 * 	@exception InvalidIterator if one of the iterators is not valid
		 * 	@exception IncompatibleIterators if the iterators show to different containers.
		 */
		bool operator < (const RandomAccessIterator& iterator) const
			throw(Exception::InvalidIterator, Exception::IncompatibleIterators);

		/** Lesser or equal operator.
		 *  Returns true if both iterators have the same container
		 * 	and the element of this iterator resides before the element of the given iterator
		 * 	or both iterators point to the same element.
		 * 	@exception InvalidIterator if one of the iterators is not valid
		 * 	@exception IncompatibleIterators if the iterators show to different containers.
		 */
		bool operator <= (const RandomAccessIterator& iterator) const
			throw(Exception::InvalidIterator, Exception::IncompatibleIterators);

		/** Greater or equal operator.
		 *  Returns true if both iterators have the same container
		 * 	and the element of this iterator resides after the elemen of the given iterator
		 * 	or both iterators point to the same element.
		 * 	@exception InvalidIterator if one of the iterators is not valid
		 * 	@exception IncompatibleIterators if the iterators show to different containers.
		 */
		bool operator >= (const RandomAccessIterator& iterator) const
			throw(Exception::InvalidIterator, Exception::IncompatibleIterators);
		
		/** Greater operator.
		 *  Returns true if both iterators have the same container
		 * 	and the element of this iterator resides after the elemen of the given iterator.
		 * 	@exception InvalidIterator if one of the iterators is not valid
		 * 	@exception IncompatibleIterators if the iterators show to different containers.
		 */
		bool operator > (const RandomAccessIterator& iterator) const
			throw(Exception::InvalidIterator, Exception::IncompatibleIterators);

		/** Random access operator.
		 */
		DataType& operator [] (Index index)
			throw(Exception::InvalidIterator);

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
		:	BaseIterator<Container, DataType, Position, Traits>()
	{
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	RandomAccessIterator<Container, DataType, Position, Traits>::RandomAccessIterator
		(const RandomAccessIterator& iterator)
		throw()
		:	BaseIterator<Container, DataType, Position, Traits>(iterator)
	{
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	RandomAccessIterator<Container, DataType, Position, Traits>::RandomAccessIterator
		(const BaseIterator<Container, DataType, Position, Traits>& iterator)
		throw()
		:	BaseIterator<Container, DataType, Position, Traits>(iterator)
	{
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	RandomAccessIterator<Container, DataType, Position, Traits>::~RandomAccessIterator()
		throw()
	{
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool RandomAccessIterator<Container, DataType, Position, Traits>::operator + () const
		throw()
	{
		return BaseIterator<Container, DataType, Position, Traits>::getTraits().isValid();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool RandomAccessIterator<Container, DataType, Position, Traits>::operator - () const
		throw()
	{
		return (BaseIterator<Container, DataType, Position, Traits>::getTraits().isValid() == false);
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	void RandomAccessIterator<Container, DataType, Position, Traits>::toBegin()
		throw(Exception::InvalidIterator)
	{
		if (BaseIterator<Container, DataType, Position, Traits>::getTraits().isSingular())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}

		BaseIterator<Container, DataType, Position, Traits>::getTraits().toBegin();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool RandomAccessIterator<Container, DataType, Position, Traits>::isBegin() const
		throw(Exception::InvalidIterator)
	{
		if (BaseIterator<Container, DataType, Position, Traits>::getTraits().isSingular())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}

		return BaseIterator<Container, DataType, Position, Traits>::getTraits().isBegin();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	void RandomAccessIterator<Container, DataType, Position, Traits>::toEnd()
		throw(Exception::InvalidIterator)
	{
		if (BaseIterator<Container, DataType, Position, Traits>::getTraits().isSingular())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}

		BaseIterator<Container, DataType, Position, Traits>::getTraits().toEnd();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool RandomAccessIterator<Container, DataType, Position, Traits>::isEnd() const
		throw(Exception::InvalidIterator)
	{
		if (BaseIterator<Container, DataType, Position, Traits>::getTraits().isSingular())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}

		return BaseIterator<Container, DataType, Position, Traits>::getTraits().isEnd();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	void RandomAccessIterator<Container, DataType, Position, Traits>::toRBegin()
		throw(Exception::InvalidIterator)
	{
		if (BaseIterator<Container, DataType, Position, Traits>::getTraits().isSingular())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}

		BaseIterator<Container, DataType, Position, Traits>::getTraits().toRBegin();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool RandomAccessIterator<Container, DataType, Position, Traits>::isRBegin() const
		throw(Exception::InvalidIterator)
	{
		if (BaseIterator<Container, DataType, Position, Traits>::getTraits().isSingular())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}

		return BaseIterator<Container, DataType, Position, Traits>::getTraits().isRBegin();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	void RandomAccessIterator<Container, DataType, Position, Traits>::toREnd()
		throw(Exception::InvalidIterator)
	{
		if (BaseIterator<Container, DataType, Position, Traits>::getTraits().isSingular())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}

		BaseIterator<Container, DataType, Position, Traits>::getTraits().toREnd();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool RandomAccessIterator<Container, DataType, Position, Traits>::isREnd() const
		throw(Exception::InvalidIterator)
	{
		if (BaseIterator<Container, DataType, Position, Traits>::getTraits().isSingular())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}

		return BaseIterator<Container, DataType, Position, Traits>::getTraits().isREnd();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	RandomAccessIterator<Container, DataType, Position, Traits>&
		RandomAccessIterator<Container, DataType, Position, Traits>::operator ++ ()
		throw(Exception::InvalidIterator)
	{
		if (BaseIterator<Container, DataType, Position, Traits>::getTraits().isSingular())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}

		BaseIterator<Container, DataType, Position, Traits>::getTraits().forward();
		return *this;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	RandomAccessIterator<Container, DataType, Position, Traits>
		RandomAccessIterator<Container, DataType, Position, Traits>::operator ++ (int)
		throw(Exception::InvalidIterator)
	{
		RandomAccessIterator iterator(*this);
		++(*this);
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	RandomAccessIterator<Container, DataType, Position, Traits>& 
		RandomAccessIterator<Container, DataType, Position, Traits>::operator -- ()
		throw(Exception::InvalidIterator)
	{
		if (BaseIterator<Container, DataType, Position, Traits>::getTraits().isSingular())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}

		BaseIterator<Container, DataType, Position, Traits>::getTraits().backward();
		return *this;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	RandomAccessIterator<Container, DataType, Position, Traits>
		RandomAccessIterator<Container, DataType, Position, Traits>::operator -- (int)
		throw(Exception::InvalidIterator)
	{
		RandomAccessIterator iterator(*this);
		--(*this);
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	RandomAccessIterator<Container, DataType, Position, Traits>& 
		RandomAccessIterator<Container, DataType, Position, Traits>::operator += (Distance distance)
		throw(Exception::InvalidIterator)
	{
		if (!BaseIterator<Container, DataType, Position, Traits>::getTraits().isValid())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}

		if (distance < (Distance)0)
		{
			return (*this -= -distance);
		}

		BaseIterator<Container, DataType, Position, Traits>::getTraits().forward(distance);
		return *this;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	RandomAccessIterator<Container, DataType, Position, Traits>& 
		RandomAccessIterator<Container, DataType, Position, Traits>::operator -= (Distance distance)
		throw(Exception::InvalidIterator)
	{
		if (BaseIterator<Container, DataType, Position, Traits>::getTraits().isSingular())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}

		if (distance < (Distance)0)
		{
			return (*this += -distance);
		}

		if (BaseIterator<Container, DataType, Position, Traits>::getTraits().isEnd() == true)
		{
			BaseIterator<Container, DataType, Position, Traits>::getTraits().toRBegin();
			BaseIterator<Container, DataType, Position, Traits>::getTraits().backward(distance - 1);
		}
		else 
		{
			BaseIterator<Container, DataType, Position, Traits>::getTraits().backward(distance);
		}
		return *this;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	RandomAccessIterator<Container, DataType, Position, Traits> 
		RandomAccessIterator<Container, DataType, Position, Traits>::operator + 
		(Distance distance) const
		throw(Exception::InvalidIterator)
	{
		RandomAccessIterator iterator(*this);
		return (iterator += distance);
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	RandomAccessIterator<Container, DataType, Position, Traits> 
		RandomAccessIterator<Container, DataType, Position, Traits>::operator - 
		(Distance distance) const
		throw(Exception::InvalidIterator)
	{
		RandomAccessIterator iterator(*this);
		return (iterator -= distance);
	}
	
	template <typename Container, typename DataType, typename Position, typename Traits>
	Distance RandomAccessIterator<Container, DataType, Position, Traits>::operator -
		 (const RandomAccessIterator<Container, DataType, Position, Traits>& iterator) const
		throw(Exception::InvalidIterator, Exception::IncompatibleIterators)
		{
			if (!BaseIterator<Container, DataType, Position, Traits>::getTraits().isValid())
			{
				throw Exception::InvalidIterator(__FILE__, __LINE__);
			}
			if (!iterator.isValid())
			{
				throw Exception::InvalidIterator(__FILE__, __LINE__);
			}
			if (BaseIterator<Container, DataType, Position, Traits>::getTraits().getContainer() != iterator.getContainer())
			{
				throw Exception::IncompatibleIterators(__FILE__, __LINE__);
			}
			return BaseIterator<Container, DataType, Position, Traits>::getTraits().getDistance(iterator.getTraits());
		}
	
	template <typename Container, typename DataType, typename Position, typename Traits>
	RandomAccessIterator<Container, DataType, Position, Traits> operator +
		(Distance distance, const RandomAccessIterator<Container, DataType, Position, Traits>& iterator)
		throw(Exception::InvalidIterator)
	{
		RandomAccessIterator<Container, DataType, Position, Traits> tmp_iterator(iterator);
		return (tmp_iterator += distance);
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool RandomAccessIterator<Container, DataType, Position, Traits>::operator < 
		(const RandomAccessIterator& iterator) const
		throw(Exception::InvalidIterator, Exception::IncompatibleIterators)
	{
		if (!BaseIterator<Container, DataType, Position, Traits>::getTraits().isValid())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}
		if (!iterator.isValid())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}
		if (BaseIterator<Container, DataType, Position, Traits>::getTraits().getContainer() != iterator.getContainer())
		{
			throw Exception::IncompatibleIterators(__FILE__, __LINE__);
		}

		return (BaseIterator<Container, DataType, Position, Traits>::getTraits().operator < (iterator.getTraits()));
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool RandomAccessIterator<Container, DataType, Position, Traits>::operator <= 
		(const RandomAccessIterator& iterator) const
		throw(Exception::InvalidIterator, Exception::IncompatibleIterators)
	{
		if (!BaseIterator<Container, DataType, Position, Traits>::getTraits().isValid())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}
		if (!iterator.isValid())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}
		if (BaseIterator<Container, DataType, Position, Traits>::getTraits().getContainer() != iterator.getContainer())
		{
			throw Exception::IncompatibleIterators(__FILE__, __LINE__);
		}

		return !(BaseIterator<Container, DataType, Position, Traits>::getTraits().operator > (iterator.getTraits()));
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool RandomAccessIterator<Container, DataType, Position, Traits>::operator >= 
		(const RandomAccessIterator& iterator) const
		throw(Exception::InvalidIterator, Exception::IncompatibleIterators)
	{
		if (!BaseIterator<Container, DataType, Position, Traits>::getTraits().isValid())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}
		if (!iterator.isValid())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}
		if (BaseIterator<Container, DataType, Position, Traits>::getTraits().getContainer() != iterator.getContainer())
		{
			throw Exception::IncompatibleIterators(__FILE__, __LINE__);
		}

		return !(BaseIterator<Container, DataType, Position, Traits>::getTraits().operator < (iterator.getTraits()));
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool RandomAccessIterator<Container, DataType, Position, Traits>::operator > 
		(const RandomAccessIterator& iterator) const
		throw(Exception::InvalidIterator, Exception::IncompatibleIterators)
	{
		if (!BaseIterator<Container, DataType, Position, Traits>::getTraits().isValid())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}
		if (!iterator.isValid())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}
		if (BaseIterator<Container, DataType, Position, Traits>::getTraits().getContainer() != iterator.getContainer())
		{
			throw Exception::IncompatibleIterators(__FILE__, __LINE__);
		}

		return (BaseIterator<Container, DataType, Position, Traits>::getTraits().operator > (iterator.getTraits()));
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	DataType& RandomAccessIterator<Container, DataType, Position, Traits>::operator [] (Index index)
		throw(Exception::InvalidIterator)
	{
		if (!BaseIterator<Container, DataType, Position, Traits>::getTraits().isValid())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}

		return (DataType&)BaseIterator<Container, DataType, Position, Traits>::getTraits().getData(index);
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
		:	BaseIterator<Container, DataType, Position, Traits>(container)
	{
	}


	/**	Generic Constant Random Access Iterator
			 \par
			
	*/
	template <typename Container, typename DataType, typename Position, typename Traits>
	class ConstRandomAccessIterator
		: public ConstBaseIterator<Container, DataType, Position, Traits>
	{
		public:

		BALL_CREATE(ConstRandomAccessIterator)

		/** @name Typedefs
		 */
		//@{

		/** Tag this iterator as a random access iterator according to the STL scheme.
		 */
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
		ConstRandomAccessIterator(const BaseIterator<Container, DataType, Position, Traits>& iterator)
			throw();
		
		/** Destructor.
		 */
		~ConstRandomAccessIterator()
			throw();

		//@}

    /** BALL specific shortcut for validity checking.
		 */
		bool operator + () const
			throw();

    /** Negated validity checking.
		 *  Returns false if valid, true if invalid.
		 */
		bool operator - () const
			throw();

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

		/** Prefix increment operator
		 */
		ConstRandomAccessIterator& operator ++ ()
			throw(Exception::InvalidIterator);

		/** Postfix increment operator
		 */
		ConstRandomAccessIterator operator ++ (int)
			throw(Exception::InvalidIterator);

		/** Prefix decrement operator
		 */
		ConstRandomAccessIterator& operator -- ()
			throw(Exception::InvalidIterator);

		/** Postfix decrement operator
		 */
		ConstRandomAccessIterator operator -- (int)
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
		Distance operator - (const ConstRandomAccessIterator& iterator ) const
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
		:	ConstBaseIterator<Container, DataType, Position, Traits>()
	{
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstRandomAccessIterator<Container, DataType, Position, Traits>::ConstRandomAccessIterator
		(const ConstRandomAccessIterator& iterator) throw()
		:	ConstBaseIterator<Container, DataType, Position, Traits>(iterator)
	{
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstRandomAccessIterator<Container, DataType, Position, Traits>::ConstRandomAccessIterator
		(const BaseIterator<Container, DataType, Position, Traits>& iterator) throw()
		:	ConstBaseIterator<Container, DataType, Position, Traits>(iterator)
	{
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstRandomAccessIterator<Container, DataType, Position, Traits>::~ConstRandomAccessIterator()
		throw()
	{
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool ConstRandomAccessIterator<Container, DataType, Position, Traits>::operator + () const
		throw()
	{
		return ConstBaseIterator<Container, DataType, Position, Traits>::getTraits().isValid();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool ConstRandomAccessIterator<Container, DataType, Position, Traits>::operator - () const
		throw()
	{
		return (!ConstBaseIterator<Container, DataType, Position, Traits>::getTraits().isValid());
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	void ConstRandomAccessIterator<Container, DataType, Position, Traits>::toBegin()
		throw(Exception::InvalidIterator)
	{
		if (ConstBaseIterator<Container, DataType, Position, Traits>::getTraits().isSingular())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}
		ConstBaseIterator<Container, DataType, Position, Traits>::getTraits().toBegin();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool ConstRandomAccessIterator<Container, DataType, Position, Traits>::isBegin() const
		throw(Exception::InvalidIterator)
	{
		if (ConstBaseIterator<Container, DataType, Position, Traits>::getTraits().isSingular())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}
		return ConstBaseIterator<Container, DataType, Position, Traits>::getTraits().isBegin();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	void ConstRandomAccessIterator<Container, DataType, Position, Traits>::toEnd()
		throw(Exception::InvalidIterator)
	{
		if (ConstBaseIterator<Container, DataType, Position, Traits>::getTraits().isSingular())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}
		ConstBaseIterator<Container, DataType, Position, Traits>::getTraits().toEnd();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool ConstRandomAccessIterator<Container, DataType, Position, Traits>::isEnd() const
		throw(Exception::InvalidIterator)
	{
		if (ConstBaseIterator<Container, DataType, Position, Traits>::getTraits().isSingular())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}
		return ConstBaseIterator<Container, DataType, Position, Traits>::getTraits().isEnd();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	void ConstRandomAccessIterator<Container, DataType, Position, Traits>::toRBegin()
		throw(Exception::InvalidIterator)
	{
		if (ConstBaseIterator<Container, DataType, Position, Traits>::getTraits().isSingular())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}
		ConstBaseIterator<Container, DataType, Position, Traits>::getTraits().toRBegin();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool ConstRandomAccessIterator<Container, DataType, Position, Traits>::isRBegin() const
		throw(Exception::InvalidIterator)
	{
		if (ConstBaseIterator<Container, DataType, Position, Traits>::getTraits().isSingular())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}

		return ConstBaseIterator<Container, DataType, Position, Traits>::getTraits().isRBegin();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	void ConstRandomAccessIterator<Container, DataType, Position, Traits>::toREnd()
		throw(Exception::InvalidIterator)
	{
		if (ConstBaseIterator<Container, DataType, Position, Traits>::getTraits().isSingular())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}	

		ConstBaseIterator<Container, DataType, Position, Traits>::getTraits().toREnd();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool ConstRandomAccessIterator<Container, DataType, Position, Traits>::isREnd() const
		throw(Exception::InvalidIterator)
	{
		if (ConstBaseIterator<Container, DataType, Position, Traits>::getTraits().isSingular())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}
		return ConstBaseIterator<Container, DataType, Position, Traits>::getTraits().isREnd();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstRandomAccessIterator<Container, DataType, Position, Traits>&
		ConstRandomAccessIterator<Container, DataType, Position, Traits>::operator ++ ()
		throw(Exception::InvalidIterator)
	{
		if (!ConstBaseIterator<Container, DataType, Position, Traits>::getTraits().isValid())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}
		ConstBaseIterator<Container, DataType, Position, Traits>::getTraits().forward();
		return *this;
	}

	
	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstRandomAccessIterator<Container, DataType, Position, Traits>
		ConstRandomAccessIterator<Container, DataType, Position, Traits>::operator ++ (int)
		throw(Exception::InvalidIterator)
	{
		ConstRandomAccessIterator iterator(*this);
		++(*this);
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstRandomAccessIterator<Container, DataType, Position, Traits>&
		ConstRandomAccessIterator<Container, DataType, Position, Traits>::operator -- ()
		throw(Exception::InvalidIterator)
	{
		if (ConstBaseIterator<Container, DataType, Position, Traits>::getTraits().isSingular())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}
		ConstBaseIterator<Container, DataType, Position, Traits>::getTraits().backward();
		return *this;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstRandomAccessIterator<Container, DataType, Position, Traits>
		ConstRandomAccessIterator<Container, DataType, Position, Traits>::operator -- (int)
		throw(Exception::InvalidIterator)
	{
		ConstRandomAccessIterator iterator(*this);
		--(*this);
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstRandomAccessIterator<Container, DataType, Position, Traits>&
		ConstRandomAccessIterator<Container, DataType, Position, Traits>::operator += (Distance distance)
		throw(Exception::InvalidIterator)
	{
		if (!ConstBaseIterator<Container, DataType, Position, Traits>::getTraits().isValid())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}
		if (distance < (Distance)0)
		{
			return (*this -= -distance);
		}
		ConstBaseIterator<Container, DataType, Position, Traits>::getTraits().forward(distance);
		return *this;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstRandomAccessIterator<Container, DataType, Position, Traits>&
		ConstRandomAccessIterator<Container, DataType, Position, Traits>::operator -= (Distance distance)
		throw(Exception::InvalidIterator)
	{
		if (ConstBaseIterator<Container, DataType, Position, Traits>::getTraits().isSingular())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}
		if (distance < (Distance)0)
		{
			return (*this += -distance);
		}
		if (ConstBaseIterator<Container, DataType, Position, Traits>::getTraits().isEnd() == true)
		{
			ConstBaseIterator<Container, DataType, Position, Traits>::getTraits().toRBegin();
			ConstBaseIterator<Container, DataType, Position, Traits>::getTraits().backward(distance - 1);
		}
		else 
		{
			ConstBaseIterator<Container, DataType, Position, Traits>::getTraits().backward(distance);
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
		if (!ConstBaseIterator<Container, DataType, Position, Traits>::getTraits().isValid())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}
		if (!iterator.isValid())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);				
		}
		if (ConstBaseIterator<Container, DataType, Position, Traits>::getTraits().getContainer() != iterator.getContainer())
		{
			throw Exception::IncompatibleIterators(__FILE__, __LINE__);
		}

		return (ConstBaseIterator<Container, DataType, Position, Traits>::getTraits().operator < (iterator.getTraits()));
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool ConstRandomAccessIterator<Container, DataType, Position, Traits>::operator <= 
		(const ConstRandomAccessIterator& iterator) const
		throw(Exception::InvalidIterator, Exception::IncompatibleIterators)
	{
		if (!ConstBaseIterator<Container, DataType, Position, Traits>::getTraits().isValid())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}
		if (!iterator.isValid())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}
		if (ConstBaseIterator<Container, DataType, Position, Traits>::getTraits().getContainer() != iterator.getContainer())
		{
			throw Exception::IncompatibleIterators(__FILE__, __LINE__);
		}
		return !(ConstBaseIterator<Container, DataType, Position, Traits>::getTraits().operator > (iterator.getTraits()));
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool ConstRandomAccessIterator<Container, DataType, Position, Traits>::operator >= 
		(const ConstRandomAccessIterator& iterator) const
		throw(Exception::InvalidIterator, Exception::IncompatibleIterators)
	{
		if (!ConstBaseIterator<Container, DataType, Position, Traits>::getTraits().isValid())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}
		if (!iterator.isValid())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}
		if (ConstBaseIterator<Container, DataType, Position, Traits>::getTraits().getContainer() != iterator.getContainer())
		{
			throw Exception::IncompatibleIterators(__FILE__, __LINE__);
		}

		return !(ConstBaseIterator<Container, DataType, Position, Traits>::getTraits().operator < (iterator.getTraits()));
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool ConstRandomAccessIterator<Container, DataType, Position, Traits>::operator > 
		(const ConstRandomAccessIterator& iterator) const
		throw(Exception::InvalidIterator, Exception::IncompatibleIterators)
	{
		if (!ConstBaseIterator<Container, DataType, Position, Traits>::getTraits().isValid())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}
		if (!iterator.isValid())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}
		if (ConstBaseIterator<Container, DataType, Position, Traits>::getTraits().getContainer() != iterator.getContainer())
		{
			throw Exception::IncompatibleIterators(__FILE__, __LINE__);
		}

		return (ConstBaseIterator<Container, DataType, Position, Traits>::getTraits().operator > (iterator.getTraits()));
	}

	
	template <typename Container, typename DataType, typename Position, typename Traits>
	const DataType& ConstRandomAccessIterator<Container, DataType, Position, Traits>::operator [] (Index index) const
		throw(Exception::InvalidIterator)
	{
		if (!ConstBaseIterator<Container, DataType, Position, Traits>::getTraits().isValid())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}

		return ConstBaseIterator<Container, DataType, Position, Traits>::getTraits().getData(index);
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
		:	ConstBaseIterator<Container, DataType, Position, Traits>(container)
	{
	}

	//@}

} // namespace BALL 

#endif // BALL_CONCEPT_RANDOMACCESSITERATOR_H
