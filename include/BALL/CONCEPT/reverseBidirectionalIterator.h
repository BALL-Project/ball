// $Id: reverseBidirectionalIterator.h,v 1.7 2001/06/29 14:12:31 anker Exp $

#ifndef BALL_CONCEPT_REVERSEBIDIRECTIONALITERATOR_H
#define BALL_CONCEPT_REVERSEBIDIRECTIONALITERATOR_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_CONCEPT_BIDIRECTIONALITERATOR_H
#	include <BALL/CONCEPT/bidirectionalIterator.h>
#endif

#ifndef BALL_CONCEPT_PREDICATE_H
#	include <BALL/CONCEPT/predicate.h>
#endif

namespace BALL 
{

	/**	@name Generic Reverse Bidirectional Iterator Classes.
			\\
			{\bf Definition:} \URL{BALL/CONCEPT/reverseBidirectionalIterator.h}
	*/
	//@{

	/**	Generic Reverse Bidirectional Iterator Class
	*/
	template <typename Container, typename DataType, typename Position, typename Traits>
	class ReverseBidirectionalIterator
		: public BidirectionalIterator<Container, DataType, Position, Traits>
	{
		public:

		BALL_CREATE(ReverseBidirectionalIterator)

		/** @name Typedefs
		 */
		//@{

		/** Tag this iterator as a bidirectional iterator according to the STL
				scheme.
		 */
		typedef ::std::bidirectional_iterator_tag iterator_category;

		//@}

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**
		*/
		ReverseBidirectionalIterator()
			throw();

		/**
		*/
		ReverseBidirectionalIterator(const ReverseBidirectionalIterator &iterator)
			throw();

		/**
		*/
		ReverseBidirectionalIterator(const BaseIterator<Container, DataType, Position, Traits> &iterator)
			throw();

		/**
		 */
		~ReverseBidirectionalIterator()
			throw();

		//@}

		bool operator + () const
			throw();

		bool operator - () const
			throw();

		void toBegin()
			throw(Exception::InvalidIterator);

		bool isBegin() const
			throw(Exception::InvalidIterator);

		void toEnd()
			throw(Exception::InvalidIterator);

		bool isEnd() const
			throw(Exception::InvalidIterator);

		void toRBegin()
			throw(Exception::InvalidIterator);

		bool isRBegin() const
			throw(Exception::InvalidIterator);

		void toREnd()
			throw(Exception::InvalidIterator);

		bool isREnd() const
			throw(Exception::InvalidIterator);

		ReverseBidirectionalIterator &operator ++ ()
			throw(Exception::InvalidIterator);

		ReverseBidirectionalIterator operator ++ (int)
			throw(Exception::InvalidIterator);

		ReverseBidirectionalIterator &operator -- ()
			throw(Exception::InvalidIterator);

		ReverseBidirectionalIterator operator -- (int)
			throw(Exception::InvalidIterator);

		static ReverseBidirectionalIterator begin(const Container &container)
			throw(Exception::InvalidIterator);

		static ReverseBidirectionalIterator end(const Container &container)
			throw(Exception::InvalidIterator);

		static ReverseBidirectionalIterator rbegin(const Container &container)
			throw(Exception::InvalidIterator);

		static ReverseBidirectionalIterator rend(const Container &container)
			throw(Exception::InvalidIterator);

		protected:

		ReverseBidirectionalIterator(const Container &container)
			throw();

	};

	template <typename Container, typename DataType, typename Position, typename Traits>
	ReverseBidirectionalIterator<Container, DataType, Position, Traits>::ReverseBidirectionalIterator()
		throw()
		:	BidirectionalIterator<Container, DataType, Position, Traits>()
	{
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ReverseBidirectionalIterator<Container, DataType, Position, Traits>::ReverseBidirectionalIterator(const ReverseBidirectionalIterator &iterator)
		throw()
		:	BidirectionalIterator<Container, DataType, Position, Traits>(iterator)
	{
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ReverseBidirectionalIterator<Container, DataType, Position, Traits>::ReverseBidirectionalIterator(const BaseIterator<Container, DataType, Position, Traits> &iterator)
		throw()
		:	BidirectionalIterator<Container, DataType, Position, Traits>(iterator)
	{	
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ReverseBidirectionalIterator<Container, DataType, Position, Traits>::~ReverseBidirectionalIterator()
		throw()
	{
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool ReverseBidirectionalIterator<Container, DataType, Position, Traits>::operator + () const
		throw()
	{
		return traits_ptr_->isValid();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool ReverseBidirectionalIterator<Container, DataType, Position, Traits>::operator - () const
		throw()
	{
		return (traits_ptr_->isValid() == false);
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	void ReverseBidirectionalIterator<Container, DataType, Position, Traits>::toBegin()
		throw(Exception::InvalidIterator)
	{
		BidirectionalIterator<Container, DataType, Position, Traits>::toRBegin();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool ReverseBidirectionalIterator<Container, DataType, Position, Traits>::isBegin() const
		throw(Exception::InvalidIterator)
	{
		return BidirectionalIterator<Container, DataType, Position, Traits>::isRBegin();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	void ReverseBidirectionalIterator<Container, DataType, Position, Traits>::toEnd()
		throw(Exception::InvalidIterator)
	{
		BidirectionalIterator<Container, DataType, Position, Traits>::toREnd();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool ReverseBidirectionalIterator<Container, DataType, Position, Traits>::isEnd() const
		throw(Exception::InvalidIterator)
	{
		return BidirectionalIterator<Container, DataType, Position, Traits>::isREnd();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	void ReverseBidirectionalIterator<Container, DataType, Position, Traits>::toRBegin()
		throw(Exception::InvalidIterator)
	{
		BidirectionalIterator<Container, DataType, Position, Traits>::toBegin();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool ReverseBidirectionalIterator<Container, DataType, Position, Traits>::isRBegin() const
		throw(Exception::InvalidIterator)
	{
		return BidirectionalIterator<Container, DataType, Position, Traits>::isBegin();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	void ReverseBidirectionalIterator<Container, DataType, Position, Traits>::toREnd()
		throw(Exception::InvalidIterator)
	{
		BidirectionalIterator<Container, DataType, Position, Traits>::toEnd();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool ReverseBidirectionalIterator<Container, DataType, Position, Traits>::isREnd() const
		throw(Exception::InvalidIterator)
	{
		return BidirectionalIterator<Container, DataType, Position, Traits>::isEnd();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ReverseBidirectionalIterator<Container, DataType, Position, Traits>& ReverseBidirectionalIterator<Container, DataType, Position, Traits>::operator ++ ()
		throw(Exception::InvalidIterator)
	{
		BidirectionalIterator<Container, DataType, Position, Traits>::operator--();
		return *this;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ReverseBidirectionalIterator<Container, DataType, Position, Traits> ReverseBidirectionalIterator<Container, DataType, Position, Traits>::operator ++ (int)
		throw(Exception::InvalidIterator)
	{
		ReverseBidirectionalIterator iterator(*this);
		BidirectionalIterator<Container, DataType, Position, Traits>::operator--();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ReverseBidirectionalIterator<Container, DataType, Position, Traits>& ReverseBidirectionalIterator<Container, DataType, Position, Traits>::operator -- ()
		throw(Exception::InvalidIterator)
	{
		BidirectionalIterator<Container, DataType, Position, Traits>::operator++();
		return *this;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ReverseBidirectionalIterator<Container, DataType, Position, Traits> ReverseBidirectionalIterator<Container, DataType, Position, Traits>::operator -- (int)
		throw(Exception::InvalidIterator)
	{
		ReverseBidirectionalIterator iterator(*this);
		BidirectionalIterator<Container, DataType, Position, Traits>::operator++();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ReverseBidirectionalIterator<Container, DataType, Position, Traits> ReverseBidirectionalIterator<Container, DataType, Position, Traits>::begin(const Container &container)
		throw(Exception::InvalidIterator)
	{
		ReverseBidirectionalIterator iterator(container);
		iterator.toBegin();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ReverseBidirectionalIterator<Container, DataType, Position, Traits> ReverseBidirectionalIterator<Container, DataType, Position, Traits>::end(const Container &container)
		throw(Exception::InvalidIterator)
	{
		ReverseBidirectionalIterator iterator(container);
		iterator.toEnd();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ReverseBidirectionalIterator<Container, DataType, Position, Traits> ReverseBidirectionalIterator<Container, DataType, Position, Traits>::rbegin(const Container &container)
		throw(Exception::InvalidIterator)
	{
		ReverseBidirectionalIterator iterator(container);
		iterator.toRBegin();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ReverseBidirectionalIterator<Container, DataType, Position, Traits> ReverseBidirectionalIterator<Container, DataType, Position, Traits>::rend(const Container &container)
		throw(Exception::InvalidIterator)
	{
		ReverseBidirectionalIterator iterator(container);
		iterator.toREnd();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ReverseBidirectionalIterator<Container, DataType, Position, Traits>::ReverseBidirectionalIterator(const Container &container)
		throw()
		: BidirectionalIterator<Container, DataType, Position, Traits>(container)
	{
	}


	/**	Generic Constant Reverse Bidirectional Iterator
	*/
	template <typename Container, typename DataType, typename Position, typename Traits>
	class ConstReverseBidirectionalIterator
		: public ConstBidirectionalIterator<Container, DataType, Position, Traits>
	{
		public:

		BALL_CREATE(ConstReverseBidirectionalIterator)

		/** @name Typedefs
		 */
		//@{

		/** Tag this iterator as a bidirectional iterator according to the STL
				scheme.
		 */
		typedef ::std::bidirectional_iterator_tag iterator_category;

		//@}

		/**	@name Constructors and Destructors
		*/
		//@{

		/**
		*/
		ConstReverseBidirectionalIterator()
			throw();

		/**
		*/
		ConstReverseBidirectionalIterator(const ConstReverseBidirectionalIterator &iterator)
			throw();

		/**
		*/
		ConstReverseBidirectionalIterator(const BaseIterator<Container, DataType, Position, Traits> &iterator)
			throw();

		/**
		 */
		~ConstReverseBidirectionalIterator()
			throw();
		//@}

		bool operator + () const
			throw();

		bool operator - () const
			throw();

		void toBegin()
			throw(Exception::InvalidIterator);

		bool isBegin() const
			throw(Exception::InvalidIterator);

		void toEnd()
			throw(Exception::InvalidIterator);

		bool isEnd() const
			throw(Exception::InvalidIterator);

		void toRBegin()
			throw(Exception::InvalidIterator);

		bool isRBegin() const
			throw(Exception::InvalidIterator);

		void toREnd()
			throw(Exception::InvalidIterator);

		bool isREnd() const
			throw(Exception::InvalidIterator);

		ConstReverseBidirectionalIterator &operator ++ ()
			throw(Exception::InvalidIterator);

		ConstReverseBidirectionalIterator operator ++ (int)
			throw(Exception::InvalidIterator);

		ConstReverseBidirectionalIterator &operator -- ()
			throw(Exception::InvalidIterator);

		ConstReverseBidirectionalIterator operator -- (int)
			throw(Exception::InvalidIterator);

		static ConstReverseBidirectionalIterator begin(const Container &container)
			throw(Exception::InvalidIterator);

		static ConstReverseBidirectionalIterator end(const Container &container)
			throw(Exception::InvalidIterator);

		static ConstReverseBidirectionalIterator rbegin(const Container &container)
			throw(Exception::InvalidIterator);

		static ConstReverseBidirectionalIterator rend(const Container &container)
			throw(Exception::InvalidIterator);

		protected:

		ConstReverseBidirectionalIterator(const Container &container)
			throw();

	};

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstReverseBidirectionalIterator<Container, DataType, Position, Traits>::ConstReverseBidirectionalIterator()
		throw()
		:	ConstBidirectionalIterator<Container, DataType, Position, Traits>()
	{
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstReverseBidirectionalIterator<Container, DataType, Position, Traits>::ConstReverseBidirectionalIterator(const ConstReverseBidirectionalIterator &iterator)
		throw()
		: ConstBidirectionalIterator<Container, DataType, Position, Traits>(iterator)
	{
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstReverseBidirectionalIterator<Container, DataType, Position, Traits>::ConstReverseBidirectionalIterator(const BaseIterator<Container, DataType, Position, Traits> &iterator)
		throw()
		:	ConstBidirectionalIterator<Container, DataType, Position, Traits>(iterator)
	{
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstReverseBidirectionalIterator<Container, DataType, Position, Traits>::~ConstReverseBidirectionalIterator()
		throw()
	{
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool ConstReverseBidirectionalIterator<Container, DataType, Position, Traits>::operator + () const
		throw()
	{
		return traits_ptr_->isValid();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool ConstReverseBidirectionalIterator<Container, DataType, Position, Traits>::operator - () const
		throw()
	{
		return (traits_ptr_->isValid() == false);
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	void ConstReverseBidirectionalIterator<Container, DataType, Position, Traits>::toBegin()
		throw(Exception::InvalidIterator)
	{
		ConstBidirectionalIterator<Container, DataType, Position, Traits>::toRBegin();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool ConstReverseBidirectionalIterator<Container, DataType, Position, Traits>::isBegin() const
		throw(Exception::InvalidIterator)
	{
		return ConstBidirectionalIterator<Container, DataType, Position, Traits>::isRBegin();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	void ConstReverseBidirectionalIterator<Container, DataType, Position, Traits>::toEnd()
		throw(Exception::InvalidIterator)
	{
		ConstBidirectionalIterator<Container, DataType, Position, Traits>::toREnd();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool ConstReverseBidirectionalIterator<Container, DataType, Position, Traits>::isEnd() const
		throw(Exception::InvalidIterator)
	{
		return ConstBidirectionalIterator<Container, DataType, Position, Traits>::isREnd();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	void ConstReverseBidirectionalIterator<Container, DataType, Position, Traits>::toRBegin()
		throw(Exception::InvalidIterator)
	{
		ConstBidirectionalIterator<Container, DataType, Position, Traits>::toBegin();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool ConstReverseBidirectionalIterator<Container, DataType, Position, Traits>::isRBegin() const
		throw(Exception::InvalidIterator)
	{
		return ConstBidirectionalIterator<Container, DataType, Position, Traits>::isBegin();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	void ConstReverseBidirectionalIterator<Container, DataType, Position, Traits>::toREnd()
		throw(Exception::InvalidIterator)
	{
		ConstBidirectionalIterator<Container, DataType, Position, Traits>::toEnd();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool ConstReverseBidirectionalIterator<Container, DataType, Position, Traits>::isREnd() const
		throw(Exception::InvalidIterator)
	{
		return ConstBidirectionalIterator<Container, DataType, Position, Traits>::isEnd();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstReverseBidirectionalIterator<Container, DataType, Position, Traits>& ConstReverseBidirectionalIterator<Container, DataType, Position, Traits>::operator ++ ()
		throw(Exception::InvalidIterator)
	{
		ConstBidirectionalIterator<Container, DataType, Position, Traits>::operator--();
		return *this;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstReverseBidirectionalIterator<Container, DataType, Position, Traits> ConstReverseBidirectionalIterator<Container, DataType, Position, Traits>::operator ++ (int)
		throw(Exception::InvalidIterator)
	{
		ConstReverseBidirectionalIterator iterator(*this);
		ConstBidirectionalIterator<Container, DataType, Position, Traits>::operator--();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstReverseBidirectionalIterator<Container, DataType, Position, Traits>& ConstReverseBidirectionalIterator<Container, DataType, Position, Traits>::operator -- ()
		throw(Exception::InvalidIterator)
	{
		ConstBidirectionalIterator<Container, DataType, Position, Traits>::operator++();
		return *this;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstReverseBidirectionalIterator<Container, DataType, Position, Traits> ConstReverseBidirectionalIterator<Container, DataType, Position, Traits>::operator -- (int)
		throw(Exception::InvalidIterator)
	{
		ConstReverseBidirectionalIterator iterator(*this);
		ConstBidirectionalIterator<Container, DataType, Position, Traits>::operator++();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstReverseBidirectionalIterator<Container, DataType, Position, Traits> ConstReverseBidirectionalIterator<Container, DataType, Position, Traits>::begin(const Container &container)
		throw(Exception::InvalidIterator)
	{
		ConstReverseBidirectionalIterator iterator(container);
		iterator.toBegin();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstReverseBidirectionalIterator<Container, DataType, Position, Traits> ConstReverseBidirectionalIterator<Container, DataType, Position, Traits>::end(const Container &container)
		throw(Exception::InvalidIterator)
	{
		ConstReverseBidirectionalIterator iterator(container);
		iterator.toEnd();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstReverseBidirectionalIterator<Container, DataType, Position, Traits> ConstReverseBidirectionalIterator<Container, DataType, Position, Traits>::rbegin(const Container &container)
		throw(Exception::InvalidIterator)
	{
		ConstReverseBidirectionalIterator iterator(container);
		iterator.toRBegin();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstReverseBidirectionalIterator<Container, DataType, Position, Traits> ConstReverseBidirectionalIterator<Container, DataType, Position, Traits>::rend(const Container &container)
		throw(Exception::InvalidIterator)
	{
		ConstReverseBidirectionalIterator iterator(container);
		iterator.toREnd();
		return iterator;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstReverseBidirectionalIterator<Container, DataType, Position, Traits>::ConstReverseBidirectionalIterator(const Container &container)
		throw()
		:	ConstBidirectionalIterator<Container, DataType, Position, Traits>(container)
	{
	}

	//@}

} // namespace BALL

#endif // BALL_CONCEPT_REVERSEBIDIRECTIONALITERATOR_H
