// $Id: baseIterator.h,v 1.8 2001/05/30 22:27:41 oliver Exp $

#ifndef BALL_CONCEPT_BASEITERATOR_H
#define BALL_CONCEPT_BASEITERATOR_H

#ifndef BALL_CONFIG_CONFIG_H
#	include <BALL/CONFIG/config.h>
#endif

#ifndef BALL_COMMON_GLOBAL_H
#	include <BALL/COMMON/global.h>
#endif

#ifndef BALL_COMMON_CREATE_H
#	include <BALL/COMMON/create.h>
#endif

#ifndef BALL_COMMON_EXCEPTION_H
#	include <BALL/COMMON/exception.h>
#endif

namespace BALL 
{

	/**	Generic Iterator Class.
			This template class implements the basic behaviour of 
			an iterator. Iterators are basically STL-like iterators. They 
			provide the full STL iterator interface, but also offer additional
			features.
				
			An important difference exists for the iterators of the kernel objects. 
			For most kernel onjects, multiple iterators exist. Therefore, we could
			not simply use {\tt begin()} and {\tt end()} like in STL, but we
			introduced specialized methods like \Ref{AtomContainer::beginAtom} and
			\Ref{AtomContainer::endAtom}. For similar reasons, the iterators for
			kernel classes are not implemented as nested classes of the respective kernel
			classes, but as independent classes to avoid code replication.
			An exception is \Ref{Atom::BondIterator}, which is relevant to \Ref{Atom} 
			alone.

			Each BALL iterator can be bound to a container, so once the iteration
			has started, it "knows" about the end() of the container.
			Therefore, BALL iterators additionally implement the unary plus operator
			to check for the validity of the iterator.
			this allows the convenient implementation of for loops, e.g. as follows:
			\\
			\begin{verbatim}
				AtomIterator atom_it = system.beginAtom();
				for (; +atom_it; ++atom_it)
				{
					....
				}
			\end{verbatim}
			\\
			{\bf Definition:} \URL{BALL/CONCEPT/baseIterator.h}
			\\
	*/
	template <typename Container, typename DataType, typename Position, typename Traits>
	class BaseIterator
	{

		public:

		BALL_CREATE(BaseIterator)

		/**	@name Typedefs.
				The names of these typedefs deviate from the usual
				BALL class names due to restrictions imposed by STL compliance.
		*/
		//@{
		/**
		*/
		typedef DataType	value_type;
		/**
		*/
		typedef Position	difference_type;
		/**
		*/
		typedef	DataType*	pointer;
		/**
		*/
		typedef DataType&	reference;

		/**
		*/
		typedef std::bidirectional_iterator_tag iterator_category;

		//@}

		/**	@name	Constructors and Destructors 
		*/
		//@{

		/**	Default constructor.
				This constructor creates a new iterator and registers it.
		*/
		BaseIterator()
			throw();
	
		/**	Copy constructor.
				This constructor creates a new iterator from an existing one.
				@param iterator the iterator to be copied
		*/
		BaseIterator(const BaseIterator& iterator, bool /* deep */ = true)
			throw();

		/**	Destructor.
				Deregisters and destructs the iterator.
		*/
		virtual ~BaseIterator()
			throw();

		//@}

		/**	@name	Assignment
		*/
		//@{

		/**	Assignment operator.
				Assigns the contents of an iterator to another iterator.
				@param	iterator the iterator to be copied
		*/
		const BaseIterator& operator = (const BaseIterator &iterator)
			throw();

		/**	Swap two iterators.
				This method swaps the contents of two iterators of the same
				type.
		*/
		void swap(BaseIterator &iterator)
			throw();

		//@}

		/**	@name	 Accessors 
		*/
		//@{

		/** Return the total number of iterators of that type currently instantiated.
		*/
		static Size countIterators()
			throw();

		/**	Return the number of iterators of that type currently bound to a container.
		*/
		static Size countIterators(const Container& container)
			throw();

		/** Return the number of iterators of that type currently pointing to a certain position of a given container.
		*/
		static Size countIterators(const Container& container,
				const Position& position)
			throw();

		/** Invalidate an iterator.
		*/
		void invalidate()
			throw();

		/** Set the traits.
		*/
		void setTraits(const Traits& iterator_traits)
			throw();

		/** Get a mutable reference to the traits of this iterator.
		*/
		Traits& getTraits()
			throw();

		/** Get a constant reference to the traits of this iterator.
		*/
		const Traits& getTraits() const
			throw();

		/** Get a pointer to the container of this iterator.
		*/
		Container* getContainer()
			throw();

		/** Get a constant pointer to the container of this iterator.
		*/
		const Container* getContainer() const
			throw();

		//@}

		/** @name Converters
		*/
		//@{

		/** Convert an iterator to Position.
				This method returns the position of the iterator. Note that
				Position is a template within this context and not the BALL data
				type.
		*/
		operator Position& ()
			throw();

		/** Convert an iterator to its Datatype by returning a reference to 
				the current data.
		*/
		DataType& operator * ()
			throw(Exception::InvalidIterator);

		/** Convert an iterator to its Datatype by returning a const reference to 
				the current data.
		*/
		const DataType& operator * () const
			throw(Exception::InvalidIterator);

		/** Return a pointer to the current data.
		*/
		DataType* operator -> ()
			throw(Exception::InvalidIterator);

		/** Return a const pointer to the data the iterator is pointing at.
		*/
		const DataType* operator -> () const
			throw(Exception::InvalidIterator);

		//@}

		/**	@name	Predicates
		*/
		//@{

		/** Equality operator.
				@return {\bf true} if both iterators point to the same item.
		*/
		bool operator == (const BaseIterator& iterator) const
			throw(Exception::IncompatibleIterators);

		/** Inequality operator. 
				@see operator ==
		*/
		bool operator != (const BaseIterator &iterator) const
			throw();

		/** Singularity predicate.
				This method returns {\bf true} if the iterator is singular, i.e., 
				not associated with a container.
		*/
		bool isSingular() const
			throw();

		/** Validity predicate.
				@return {\bf true} if the iterator is valid, i. e. pointing at data
				BAUSTELLE
		*/
		bool isValid() const
			throw();

		//@}


		protected:

		/*_
		*/
		BaseIterator(const Container& container)
			throw();

		//_
		static BaseIterator*& getFirstIterator_();

		//_
		static Size& countIterators_();


		private:

		//_
		Traits					traits_;

		
		public:

		///
		Traits*					traits_ptr_;

		///
		BaseIterator* 	previous_;

		///
		BaseIterator* 	next_;


	};

	
	// method implementations

	template <typename Container, typename DataType, typename Position, typename Traits>
	BaseIterator<Container, DataType, Position, Traits>::BaseIterator()
		throw()
		:	previous_(0),
			next_(BaseIterator::getFirstIterator_())
	{
		traits_ptr_ = &traits_;

		if (next_ != 0)
		{
			next_->previous_ = this;
		}

		BaseIterator::getFirstIterator_() = this;
		++BaseIterator::countIterators_();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BaseIterator<Container, DataType, Position, Traits>::BaseIterator(const BaseIterator& iterator, bool /* deep */ = true)
		throw()
		:	previous_(0),
			next_(BaseIterator::getFirstIterator_())
	{
		traits_ptr_ = &traits_;
		*traits_ptr_ = *(iterator.traits_ptr_);

		if (next_ != 0)
		{
			next_->previous_ = this;
		}

		BaseIterator::getFirstIterator_() = this;
		++BaseIterator::countIterators_();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BaseIterator<Container, DataType, Position, Traits>::~BaseIterator()
		throw()
	{

		if (BaseIterator::getFirstIterator_() == this)
		{
			BaseIterator::getFirstIterator_() = next_;
		}

		if (previous_ != 0)
		{
			previous_->next_ = next_;
		}

		if (next_ != 0)
		{
			next_->previous_ = previous_;
		}

		--BaseIterator::countIterators_();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	const BaseIterator<Container, DataType, Position, Traits>& BaseIterator<Container, DataType, Position, Traits>::operator = (const BaseIterator<Container, DataType, Position, Traits>& iterator)
		throw()
	{
		if (this != &iterator)
		{
			*traits_ptr_ = *(iterator.traits_ptr_);
		}

		return *this;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	void BaseIterator<Container, DataType, Position, Traits>::swap(BaseIterator &iterator)
		throw()
	{
		Traits* tempiteratorTraits = traits_ptr_;
		traits_ptr_ = iterator.traits_ptr_;
		iterator.traits_ptr_ = tempiteratorTraits;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	Size BaseIterator<Container, DataType, Position, Traits>::countIterators()
		throw()
	{
		return BaseIterator::countIterators_();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	Size BaseIterator<Container, DataType, Position, Traits>::countIterators(const Container& container)
		throw()
	{
		Size size = 0;

		for (BaseIterator* iterator = BaseIterator::getFirstIterator_(); 
				 iterator != 0; iterator = iterator->next_)
		{
			if (iterator->traits_ptr_->getContainer() == &container)
			{
				++size;
			}
		}

		return size;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	Size BaseIterator<Container, DataType, Position, Traits>::countIterators(const Container& container, const Position& position)
		throw()
	{
		Size size = 0;

		for (BaseIterator *iterator = BaseIterator::getFirstIterator_();
				 iterator != 0; iterator = iterator->next_)
		{
			if (iterator->traits_ptr_->getContainer() == &container
					&& iterator->traits_ptr_->getPosition() == position)
			{
				++size;
			}
		}

		return size;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	void BaseIterator<Container, DataType, Position, Traits>::invalidate()
		throw()
	{
		traits_ptr_->invalidate();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	void BaseIterator<Container, DataType, Position, Traits>::setTraits(const Traits &iteratorTraits)
		throw()
	{
		*traits_ptr_ = iteratorTraits;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	Traits& BaseIterator<Container, DataType, Position, Traits>::getTraits()
		throw()
	{
		return *traits_ptr_;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	const Traits& BaseIterator<Container, DataType, Position, Traits>::getTraits() const
		throw()
	{
		return *traits_ptr_;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	Container* BaseIterator<Container, DataType, Position, Traits>::getContainer()
		throw()
	{
		return traits_ptr_->getContainer();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	const Container* BaseIterator<Container, DataType, Position, Traits>::getContainer() const
		throw()
	{
		return traits_ptr_->getContainer();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BaseIterator<Container, DataType, Position, Traits>::operator Position& ()
		throw()
	{
		return traits_ptr_->getPosition();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	DataType& BaseIterator<Container, DataType, Position, Traits>::operator * ()
		throw(Exception::InvalidIterator)
	{
		if (!traits_ptr_->isValid())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}

		return (DataType &)traits_ptr_->getData();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	const DataType& BaseIterator<Container, DataType, Position, Traits>::operator *() const
		throw(Exception::InvalidIterator)
	{
		if (!traits_ptr_->isValid())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}

		return (const DataType &)traits_ptr_->getData();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	DataType* BaseIterator<Container, DataType, Position, Traits>::operator -> ()
		throw(Exception::InvalidIterator)
	{
		if (!traits_ptr_->isValid())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}

		return (DataType *)&(traits_ptr_->getData());
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	const DataType* BaseIterator<Container, DataType, Position, Traits>::operator -> () const
		throw(Exception::InvalidIterator)
	{
		if (!traits_ptr_->isValid())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}

		return (const DataType *)&(traits_ptr_->getData());
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool BaseIterator<Container, DataType, Position, Traits>::operator == (const BaseIterator& iterator) const
		throw(Exception::IncompatibleIterators)
	{
		if (traits_ptr_->getContainer() != iterator.traits_ptr_->getContainer())
		{
			throw Exception::IncompatibleIterators(__FILE__, __LINE__);
		}

		return (*traits_ptr_ == *iterator.traits_ptr_);
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool BaseIterator<Container, DataType, Position, Traits>::operator != (const BaseIterator &iterator) const
		throw()
	{
		if (traits_ptr_->getContainer() != iterator.traits_ptr_->getContainer())
		{
			throw Exception::IncompatibleIterators(__FILE__, __LINE__);
		}

		return (*traits_ptr_ != *iterator.traits_ptr_);
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool BaseIterator<Container, DataType, Position, Traits>::isSingular() const
		throw()
	{
		return traits_ptr_->isSingular();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	bool BaseIterator<Container, DataType, Position, Traits>::isValid() const
		throw()
	{
		return traits_ptr_->isValid();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BaseIterator<Container, DataType, Position, Traits>::BaseIterator(const Container& container)
		throw()
		:	traits_(container),
			previous_(0),
			next_(0)
	{
		traits_ptr_ = &traits_;

		++BaseIterator::countIterators_();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BaseIterator<Container, DataType, Position, Traits>*&
	BaseIterator<Container, DataType, Position, Traits>::getFirstIterator_()
	{
		static BaseIterator* first = 0;
		
		return first;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	Size& BaseIterator<Container, DataType, Position, Traits>::countIterators_()
	{
		static Size size = 0;
		
		return size;
	}



	/**	Constant Basic Iterator.
			\\
			{\bf Definition:} \URL{BALL/CONCEPT/baseIterator.h}
	*/
	template <typename Container, typename DataType, typename Position, typename Traits>
	class ConstBaseIterator
		: public BaseIterator<Container, DataType, Position, Traits>
	{
		public:

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor
		*/
		ConstBaseIterator()
			throw();
	
		/**	Default constructor
		*/
		ConstBaseIterator(const ConstBaseIterator &iterator)
			throw();

		/**	Default constructor
		*/
		ConstBaseIterator(const BaseIterator<Container, DataType, Position, Traits> &iterator)
			throw();

		//@}

		/**	@name	Accessors
		*/
		//@{

		/**
		*/
		const Traits& getTraits()
			throw();

		/**
		*/
		const Container* getContainer()
			throw();

		/**
		*/
		operator const Position&  ()
			throw();

		/**
		*/
		const DataType& operator * ()
			throw(Exception::InvalidIterator);

		/**
		*/
		const DataType* operator -> ()
			throw(Exception::InvalidIterator);
		//@}


		protected:

		//_
		ConstBaseIterator(const Container& container)
			throw();

	};

	// method implementations

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstBaseIterator<Container, DataType, Position, Traits>::ConstBaseIterator()
		throw()
		: BaseIterator<Container, DataType, Position, Traits>()
	{
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstBaseIterator<Container, DataType, Position, Traits>::ConstBaseIterator(const ConstBaseIterator &iterator)
		throw()
		:	BaseIterator<Container,  DataType,  Position,  Traits>(iterator)
	{
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstBaseIterator<Container, DataType, Position, Traits>::ConstBaseIterator(const BaseIterator<Container, DataType, Position, Traits> &iterator)
		throw()
		:	BaseIterator<Container, DataType, Position, Traits>(iterator)
	{
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	const Traits& ConstBaseIterator<Container, DataType, Position, Traits>::getTraits()
		throw()
	{
		return *traits_ptr_;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	const Container* ConstBaseIterator<Container, DataType, Position, Traits>::getContainer()
		throw()
	{
		return traits_ptr_->getContainer();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstBaseIterator<Container, DataType, Position, Traits>::operator const Position&  ()
		throw()
	{
		return traits_ptr_->getPosition();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	const DataType& ConstBaseIterator<Container, DataType, Position, Traits>::operator * ()
		throw(Exception::InvalidIterator)
	{
		if (!traits_ptr_->isValid())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}

		return (const DataType &)traits_ptr_->getData();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	const DataType* ConstBaseIterator<Container, DataType, Position, Traits>::operator -> ()
		throw(Exception::InvalidIterator)
	{
		if (!traits_ptr_->isValid())
		{
			throw Exception::InvalidIterator(__FILE__, __LINE__);
		}

		return (const DataType *)&(traits_ptr_->getData());
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstBaseIterator<Container, DataType, Position, Traits>::ConstBaseIterator(const Container& container)
		throw()
		:	BaseIterator<Container, DataType, Position, Traits>(container)
	{
	}

} // namespace BALL

#endif // BALL_CONCEPT_BASEITERATOR_H
