#ifndef BALL_LINALG_RANDOMACCESSFILTERITERATOR_H
#define BALL_LINALG_RANDOMACCESSFILTERITERATOR_H

#ifndef BALL_LINALG_BIDIRECTIONALFILTERITERATOR_H
#	include <BALL/MATHS/LINALG/bidirectionalFilterIterator.h>
#endif

namespace BALL 
{
	  ///
	  typedef int Distance;
	  ///
	  typedef int Index;

	/** @name	Generic Constant Random Access Filter Iterator
		\ingroup  ConceptsIterators
	*/
	//@{
	template <class Predicate, class IteratorRandomAccess>
	class ConstRandomAccessFilterIterator
		: public ConstBidirectionalFilterIterator<Predicate, IteratorRandomAccess>
	{
		public:

                /** @name Typedefs
		 */
	        //@{

		
		///
		typedef typename IteratorRandomAccess::difference_type	difference_type;
		///
		typedef typename IteratorRandomAccess::value_type	value_type;
		///
		typedef typename IteratorRandomAccess::container_type	Container;
		///
		typedef typename IteratorRandomAccess::value_type	DataType;
		///
		typedef typename IteratorRandomAccess::difference_type	Position;
		///
		typedef typename IteratorRandomAccess::traits_type	Traits;
		///
		typedef	typename IteratorRandomAccess::pointer	pointer;
		///
		typedef typename IteratorRandomAccess::reference	reference;
	        ///
	        typedef std::random_access_iterator_tag iterator_category;
		// convenience typedef
		typedef ConstBidirectionalFilterIterator<Predicate, IteratorRandomAccess> Base;
		//@}

		/**	@name	Constructors and Destructors
		*/
		//@{

		///
		ConstRandomAccessFilterIterator() throw() {}

		///
		ConstRandomAccessFilterIterator(Predicate p, IteratorRandomAccess it) throw()
		  :     ConstBidirectionalFilterIterator<Predicate, IteratorRandomAccess>(p,it)
		{
		}
	
		///
		ConstRandomAccessFilterIterator(const ConstRandomAccessFilterIterator& iterator) throw()
			:	Base(iterator)
		{
		}

		///
		~ConstRandomAccessFilterIterator() throw() {}
		//@}



		/**	@name Assignment
		*/
		//@{
		
		/** Advance iterator a given distance.
		 */
		ConstRandomAccessFilterIterator& operator += (Distance distance)
			throw(Exception::InvalidIterator);

		/** Advance iterator a given distance in reverse direction.
		 */
		ConstRandomAccessFilterIterator& operator -= (Distance distance)
			throw(Exception::InvalidIterator);

		/** Return an iterator.
		 *  It points to the element with the given distance from the element of this iterator.
		 */
		ConstRandomAccessFilterIterator operator + (Distance distance) const
			throw(Exception::InvalidIterator);

		/** Return an iterator.
		 *  It points to the element with the given distance in reverse direction
		 *  from the element of this iterator.
		 */
		ConstRandomAccessFilterIterator operator - (Distance distance) const
			throw(Exception::InvalidIterator);

		/** Return an iterator.
		 *  It points to the element with the given distance in reverse direction
		 *  from the element of this iterator.
		 */
		Distance operator - (const ConstRandomAccessFilterIterator& iterator) const
			throw(Exception::InvalidIterator, Exception::IncompatibleIterators);

		/** Return a ConstRandomAccessFilterIterator for a given container. 
		 *  It points to the first element.
		 */
		static ConstRandomAccessFilterIterator begin(const Container& container) 
			throw(Exception::Precondition);

		/** Return a Const RandomAccessFilterIterator for a given container.
		 *  It points behind the last element of the container.
		 */
		static ConstRandomAccessFilterIterator end(const Container& container)
			throw(Exception::Precondition);

		/** Return a Const RandomAccessFilterIterator for a given container.
		 *  It points at the last element of the container.
		 */
		static ConstRandomAccessFilterIterator rbegin(const Container& container)
			throw(Exception::Precondition);

		/** Return a Const RandomAccessFilterIterator for a given container.
		 *  It points before the first element.
		 */
		static ConstRandomAccessFilterIterator rend(const Container& container) 
			throw(Exception::Precondition);

		//@}

		/**	@name Predicates
		*/
		//@{

		///
		bool operator + () const throw() { return Base::getTraits().isValid(); }

		///
		bool operator - () const throw() { return !Base::getTraits().isValid(); }

    /** Lesser operator.
		 *	Returns true if both iterators have the same container
		 *  and the element of this iterator resides before the elemen of the given iterator.
		 *  @exception InvalidIterator if one of the iterators is not valid
		 *  @exception IncompatibleIterators if the iterators show to different containers.
		 */
		bool operator < (const ConstRandomAccessFilterIterator& iterator) const
			throw(Exception::InvalidIterator, Exception::IncompatibleIterators);

    /** Lesser or equal operator.
		 *  Returns true if both iterators have the same container
		 *  and the element of this iterator resides before the elemen of the given iterator
		 *  or both iterators point to the same element.
		 *  @exception InvalidIterator if one of the iterators is not valid
		 *  @exception IncompatibleIterators if the iterators show to different containers.
		 */
		bool operator <= (const ConstRandomAccessFilterIterator& iterator) const
			throw(Exception::InvalidIterator, Exception::IncompatibleIterators);

    /** Greater or equal operator.
		 *  Returns true if both iterators have the same container
		 *  and the element of this iterator resides after the elemen of the given iterator
		 *  or both iterators point to the same element.
		 *  @exception InvalidIterator if one of the iterators is not valid
		 *  @exception IncompatibleIterators if the iterators show to different containers.
		 */
		bool operator >= (const ConstRandomAccessFilterIterator& iterator) const
			throw(Exception::InvalidIterator, Exception::IncompatibleIterators);

    /** Greater operator.
		 *  Returns true if both iterators have the same container
		 *  and the element of this iterator resides after the elemen of the given iterator.
		 *  @exception InvalidIterator if one of the iterators is not valid
		 *  @exception IncompatibleIterators if the iterators show to different containers.
		 */
		bool operator > (const ConstRandomAccessFilterIterator& iterator) const
			throw(Exception::InvalidIterator, Exception::IncompatibleIterators);

		//@}

			

		/**	@name Iterator dereferencing
		*/
		//@{
		/** Random access operator.
		 */
		const DataType& operator [] (Index index) const	throw(Exception::InvalidIterator)
		{
		  if (!Base::getTraits().isValid())
		  {
		    Exception::InvalidIterator e;
		    throw e;
		  }
		  
		  return Base::iterator_[index];
		};
		//@}

		protected:

		ConstRandomAccessFilterIterator(const Container& container) throw()
			:	Base(container)
		{
		}
	};
	//@}
	
	/** Return an incremented iterator.
	 *  It points to the element with the given distance from the element 
	 *  of the given iterator.
	 */
	template <class Predicate, class IteratorRandomAccess>
	ConstRandomAccessFilterIterator<Predicate, IteratorRandomAccess> operator + 
		(Distance distance, const ConstRandomAccessFilterIterator<Predicate, IteratorRandomAccess>& iterator) 
		throw(Exception::InvalidIterator)
	{
		ConstRandomAccessFilterIterator<Predicate, IteratorRandomAccess> tmp_iterator(iterator);
		return (tmp_iterator += distance);
	}

	/** Return the distance between two iterators.
	 *  It points to the element with the given distance in reverse direction
	 *  of the given iterator.
	 */
	template <class Predicate, class IteratorRandomAccess>
	Distance ConstRandomAccessFilterIterator<Predicate, IteratorRandomAccess>::operator -
		(const ConstRandomAccessFilterIterator<Predicate, IteratorRandomAccess>& b) const
		throw(Exception::InvalidIterator, Exception::IncompatibleIterators)
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

	template <class Predicate, class IteratorRandomAccess>
	ConstRandomAccessFilterIterator<Predicate, IteratorRandomAccess>&
		ConstRandomAccessFilterIterator<Predicate, IteratorRandomAccess>::operator += (Distance distance)
		throw(Exception::InvalidIterator)
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
		  
		while (!Base::isEnd() && !Base::predicate_(Base::getTraits().getData()))
		  Base::getTraits().forward();
		return *this;
	}

	template <class Predicate, class IteratorRandomAccess>
	ConstRandomAccessFilterIterator<Predicate, IteratorRandomAccess>&
		ConstRandomAccessFilterIterator<Predicate, IteratorRandomAccess>::operator -= (Distance distance)
		throw(Exception::InvalidIterator)
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
		  
			while (!Base::isBegin() && !Base::predicate_(Base::getTraits().getData()))
			  Base::getTraits().backward();
		}
		return *this;
	}

	template <class Predicate, class IteratorRandomAccess>
	ConstRandomAccessFilterIterator<Predicate, IteratorRandomAccess>
		ConstRandomAccessFilterIterator<Predicate, IteratorRandomAccess>::operator + (Distance distance) const
		throw(Exception::InvalidIterator)
	{
		ConstRandomAccessFilterIterator iterator(*this);
		return (iterator += distance);
	}

	template <class Predicate, class IteratorRandomAccess>
	ConstRandomAccessFilterIterator<Predicate, IteratorRandomAccess> 
		ConstRandomAccessFilterIterator<Predicate, IteratorRandomAccess>::operator - (Distance distance) const
		throw(Exception::InvalidIterator)
	{
		ConstRandomAccessFilterIterator iterator(*this);
		return (iterator -= distance);
	}

	template <class Predicate, class IteratorRandomAccess>
	bool ConstRandomAccessFilterIterator<Predicate, IteratorRandomAccess>::operator < 
		(const ConstRandomAccessFilterIterator& iterator) const
		throw(Exception::InvalidIterator, Exception::IncompatibleIterators)
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

	template <class Predicate, class IteratorRandomAccess>
	bool ConstRandomAccessFilterIterator<Predicate, IteratorRandomAccess>::operator <= 
		(const ConstRandomAccessFilterIterator& iterator) const
		throw(Exception::InvalidIterator, Exception::IncompatibleIterators)
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

	template <class Predicate, class IteratorRandomAccess>
	bool ConstRandomAccessFilterIterator<Predicate, IteratorRandomAccess>::operator >= 
		(const ConstRandomAccessFilterIterator& iterator) const
		throw(Exception::InvalidIterator, Exception::IncompatibleIterators)
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

	template <class Predicate, class IteratorRandomAccess>
	bool ConstRandomAccessFilterIterator<Predicate, IteratorRandomAccess>::operator > 
		(const ConstRandomAccessFilterIterator& iterator) const
		throw(Exception::InvalidIterator, Exception::IncompatibleIterators)
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

	template <class Predicate, class IteratorRandomAccess>
	ConstRandomAccessFilterIterator<Predicate, IteratorRandomAccess> 
		ConstRandomAccessFilterIterator<Predicate, IteratorRandomAccess>::begin(const Container& container)
		throw(Exception::Precondition)
	{
		ConstRandomAccessFilterIterator iterator(container);
		iterator.toBegin();
		return iterator;
	}

	template <class Predicate, class IteratorRandomAccess>
	ConstRandomAccessFilterIterator<Predicate, IteratorRandomAccess> 
		ConstRandomAccessFilterIterator<Predicate, IteratorRandomAccess>::end(const Container& container)
		throw(Exception::Precondition)
	{
		ConstRandomAccessFilterIterator iterator(container);
		iterator.toEnd();
		return iterator;
	}

	template <class Predicate, class IteratorRandomAccess>
	ConstRandomAccessFilterIterator<Predicate, IteratorRandomAccess> 
		ConstRandomAccessFilterIterator<Predicate, IteratorRandomAccess>::rbegin(const Container& container)
		throw(Exception::Precondition)
	{
		ConstRandomAccessFilterIterator iterator(container);
		iterator.toRBegin();
		return iterator;
	}

	template <class Predicate, class IteratorRandomAccess>
	ConstRandomAccessFilterIterator<Predicate, IteratorRandomAccess> 
		ConstRandomAccessFilterIterator<Predicate, IteratorRandomAccess>::rend(const Container& container)
		throw(Exception::Precondition)
	{
		ConstRandomAccessFilterIterator iterator(container);
		iterator.toREnd();
		return iterator;
	}

	/**	@name	Generic Random Access Iterator Classes.
	 	 \ingroup  ConceptsIterators
	*/
	//@{

	/**	Random Access Iterator Class
	*/
	template <class Predicate, class IteratorRandomAccess>
	class RandomAccessFilterIterator
		: public ConstRandomAccessFilterIterator<Predicate, IteratorRandomAccess>
	{
		public:

		/** @name Typedefs
		 */
		//@{
		
		///
		typedef typename IteratorRandomAccess::container_type	Container;
		///
		typedef typename IteratorRandomAccess::value_type	value_type;
		///
		typedef typename IteratorRandomAccess::difference_type	difference_type;
		///
		typedef	typename IteratorRandomAccess::pointer	pointer;
		///
		typedef typename IteratorRandomAccess::reference	reference;
		///
		typedef ConstRandomAccessFilterIterator<Predicate, IteratorRandomAccess> Base;
		//@}

		/**	@name Constructors and Destructors
		*/
		//@{

		///
		RandomAccessFilterIterator() throw() {}

		///
		RandomAccessFilterIterator(Predicate p, IteratorRandomAccess it) throw()
		  :     ConstRandomAccessFilterIterator<Predicate, IteratorRandomAccess>(p,it)
		{
		}
	
		///
		RandomAccessFilterIterator(const RandomAccessFilterIterator& iterator) throw()
			:	Base(iterator)
		{
		}

		///
		~RandomAccessFilterIterator() throw() {}
		//@}

		/**	@name Dereferenciation
		*/
		//@{
		///
		reference operator [] (Index index) const throw(Exception::InvalidIterator) { return const_cast<reference>(Base::getTraits().getData(index)); }
		///
		reference operator * () const throw() { return const_cast<reference>(Base::getTraits().getData()); }
		///
		pointer operator -> () const throw() { return const_cast<pointer>(&Base::getTraits().getData()); }
		//@}

		/**	@name Assignment
		*/
		//@{
		/** Return a RandomAccessFilterIterator for a given container.
		 *  It points at the first element.
		 */
		static RandomAccessFilterIterator begin(const Container& container)
			throw(Exception::InvalidIterator);

		/** Return a RandomAccessFilterIterator for a given container.
		 *  It points behind the last element of the container.
		 */
		static RandomAccessFilterIterator end(const Container& container)
			throw(Exception::InvalidIterator);

		/** Return a RandomAccessFilterIterator for a given container.
		 *  It points at the last element of the container.
		 */
		static RandomAccessFilterIterator rbegin(const Container& container)
			throw(Exception::InvalidIterator);

		/** Return a RandomAccessFilterIterator for a given container.
		 *  It points before the first element.
		 */
		static RandomAccessFilterIterator rend(const Container& container)
			throw(Exception::InvalidIterator);
		//@}

		protected:

		RandomAccessFilterIterator(const Container& container) throw()
			:	Base(container)
		{
		}

	};
	//@}
	
	template <class Predicate, class IteratorRandomAccess>
	RandomAccessFilterIterator<Predicate, IteratorRandomAccess> 
		RandomAccessFilterIterator<Predicate, IteratorRandomAccess>::begin(const Container& container)
		throw(Exception::InvalidIterator)
	{
		RandomAccessFilterIterator iterator(container);
		iterator.toBegin();
		return iterator;
	}

	template <class Predicate, class IteratorRandomAccess>
	RandomAccessFilterIterator<Predicate, IteratorRandomAccess> 
		RandomAccessFilterIterator<Predicate, IteratorRandomAccess>::end(const Container& container)
		throw(Exception::InvalidIterator)
	{
		RandomAccessFilterIterator iterator(container);
		iterator.toEnd();
		return iterator;
	}

	template <class Predicate, class IteratorRandomAccess>
	RandomAccessFilterIterator<Predicate, IteratorRandomAccess> 
		RandomAccessFilterIterator<Predicate, IteratorRandomAccess>::rbegin(const Container& container)
		throw(Exception::InvalidIterator)
	{
		RandomAccessFilterIterator iterator(container);
		iterator.toRBegin();
		return iterator;
	}

	template <class Predicate, class IteratorRandomAccess>
	RandomAccessFilterIterator<Predicate, IteratorRandomAccess> 
		RandomAccessFilterIterator<Predicate, IteratorRandomAccess>::rend(const Container& container)
		throw(Exception::InvalidIterator)
	{
		RandomAccessFilterIterator iterator(container);
		iterator.toREnd();
		return iterator;
	}

} // namespace BALL 

#endif // BALL_KERNEL_RANDOMACCESSFILTERITERATOR_H
