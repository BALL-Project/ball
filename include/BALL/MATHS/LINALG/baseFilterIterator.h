#ifndef BALL_LINALG_BASEFILTERITERATOR_H
#define BALL_LINALG_BASEFILTERITERATOR_H

namespace BALL
{

	/**	Generic Iterator Class.
			This template class implements the basic behaviour of an iterator.
			Iterators are basically STL-like iterators. They provide the full STL
			iterator interface, but also offer additional features.
			\par

			BaseIterator itself is a base class to the other iterator classes only
			and should not be used by itself.
	*/
  template <class Predicate, class IteratorBase >
	class BaseFilterIterator
	{

		public:

		/// The instance of the iterator and the predicate
		Predicate predicate_;
		IteratorBase iterator_;
		
		///
		typedef typename IteratorBase::container_type	Container;
		///
		typedef typename IteratorBase::value_type	DataType;
		///
		typedef typename IteratorBase::difference_type	Position;
		///
		typedef typename IteratorBase::traits_type	Traits;
		///
		typedef typename IteratorBase::value_type	value_type;
		///
		typedef typename IteratorBase::difference_type	difference_type;
		///
		typedef	typename IteratorBase::pointer	pointer;
		///
		typedef typename IteratorBase::reference	reference;
		///
		typedef std::input_iterator_tag iterator_category;

		/**	@name Typedefs.
				The names of these typedefs deviate from the usual
				class names due to restrictions imposed by STL compliance.
		*/
		//@{	
		//@}

		/**	@name	Constructors and Destructors 
		*/
		//@{
		///	Default constructor
		BaseFilterIterator() throw() {}
		///	Default constructor
		BaseFilterIterator(Predicate p, IteratorBase it) throw()
		  :     predicate_(p), iterator_(it)
		{
		}
	
		///	Copy constructor
		BaseFilterIterator(const BaseFilterIterator& iterator) throw()
		  :	predicate_(iterator.predicate_), iterator_(iterator.iterator_)
		{
		}

		///	Destructor.
		~BaseFilterIterator() throw() {}
		//@}

		/**	@name	Assignment
		*/
		//@{

		/**	Assignment operator.
				Assigns the contents of an iterator to another iterator.
				@param	iterator the iterator to be copied
		*/
		BaseFilterIterator& operator = (const BaseFilterIterator& iterator) throw()
		{
			predicate_ = iterator.predicate_;
			iterator_  = iterator.iterator_;
			return *this;
		}
			
		///	Swap two iterators
		void swap(BaseFilterIterator& iterator) throw() 
		{ 
		  std::swap(predicate_, iterator.predicate_); 
		  std::swap(iterator_, iterator.iterator_);
		}
		//@}

		/**	@name	 Accessors 
		*/
		//@{

		/// Invalidate the iterator.
		void invalidate() throw() { iterator_.invalidate(); }

		/// Set the traits
		void setTraits(const Traits& traits) throw() { iterator_.setTraits(traits); }

		/// Get a constant reference to the traits of this iterator.
		const Traits& getTraits() const throw() { return iterator_.getTraits(); }

		/// Get a constant reference to the traits of this iterator.
		Traits& getTraits() throw() { return iterator_.getTraits(); }

		/// Set the predicates
		void setPredicate(const Predicate& predicate) throw() { predicate_ = predicate; }

		/// Get a constant reference to the traits of this iterator.
		const Predicate& getPredicate() const throw() { return predicate_; }

		/// Get a constant reference to the traits of this iterator.
		Predicate& getPredicate() throw() { return predicate_; }

		/// Get a constant pointer to the container of this iterator.
		const Container* getContainer() const	throw() { return iterator_.getContainer(); }
		//@}

		/** @name Converters
		*/
		//@{

		/** Convert an iterator to Position.
				This method returns the position of the iterator. 
				Position is a template within this context.
		*/
		  operator const Position& () const throw() { return iterator_(); }

		/// Convert an iterator to its Datatype by returning a reference to the current data.
		reference operator * () const throw() { return *iterator_; }

		/// Return a pointer to the current data.
		pointer operator -> () const throw() { return iterator_.operator->(); }
		//@}

		/**	@name	Predicates
		*/
		//@{

		/// Equality operator
		bool operator == (const BaseFilterIterator& iterator) const throw() { return (iterator_ == iterator.iterator_); }

		/// Inequality operator
		bool operator != (const BaseFilterIterator& iterator) const throw() { return !(iterator_ == iterator.iterator_); }

		/// Equality operator
		bool operator == (const IteratorBase& iterator) const throw() { return (iterator_ == iterator); }

		/// Inequality operator
		bool operator != (const IteratorBase& iterator) const throw() { return !(iterator_ == iterator); }

		/** Singularity predicate.
				This method returns <b>true</b> if the iterator is singular, i.e., 
				not associated with a container.
		*/
		bool isSingular() const	throw() { return iterator_.isSingular(); }

		/** Validity predicate
				@return true if the iterator is valid (pointing at an element in a container) 
		*/
		bool isValid() const throw() { return iterator_.isValid(); }

		/// Validity predicate
		bool operator + () const throw() { return +(iterator_); }

		/// Invalidity predicate
		bool operator - () const throw() { return -(iterator_); }
		//@}

		protected:

		/** Constructor.
				Protected to allow instantiation and use in derived classes only.
		*/
		BaseFilterIterator(const Container& container) throw()
			:       iterator_(container)
		{
		}
	};


} // namespace BALL

#endif // BALL_KERNEL_BASEFILTERITERATOR_H
