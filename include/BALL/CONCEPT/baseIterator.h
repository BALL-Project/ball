// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: baseIterator.h,v 1.29 2003/06/11 08:08:48 oliver Exp $
//

#ifndef BALL_CONCEPT_BASEITERATOR_H
#define BALL_CONCEPT_BASEITERATOR_H

#ifndef BALL_CONFIG_CONFIG_H
#	include <BALL/CONFIG/config.h>
#endif

#ifndef BALL_COMMON_GLOBAL_H
#	include <BALL/COMMON/global.h>
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
	 		An important difference exists for the iterators of the kernel
			objects.  For most kernel onjects, multiple iterators exist.
			Therefore, we could not simply use <tt>begin()</tt> and <tt>end()</tt> like
			in STL, but we introduced specialized methods like
			 \link AtomContainer::beginAtom AtomContainer::beginAtom \endlink  and  \link AtomContainer::endAtom AtomContainer::endAtom \endlink . For
			similar reasons, the iterators for kernel classes are not implemented
			as nested classes of the respective kernel classes, but as
			independent classes to avoid code replication. An exception is
			 \link Atom::BondIterator Atom::BondIterator \endlink , which is relevant to  \link Atom Atom \endlink  alone.

			Each BALL iterator can be bound to a container, so once the iteration
			has started, it "knows" about the end() of the container.
			Therefore, BALL iterators additionally implement the unary plus operator
			to check for the validity of the iterator.
			this allows the convenient implementation of for loops, e.g. as follows: \par
 
			\code
				AtomIterator atom_it = system.beginAtom();
				for (; +atom_it; ++atom_it)
				{
					....
				}
			\endcode
			
	 	 \ingroup  ConceptsIterators
	*/
	template <typename Container, typename DataType, typename Position, typename Traits>
	class ConstBaseIterator
	{
		public:

		/**	@name Typedefs.
				The names of these typedefs deviate from the usual
				BALL class names due to restrictions imposed by STL compliance.
		*/
		//@{
		
		///
		typedef DataType	value_type;
		///
		typedef Position	difference_type;
		///
		typedef	const DataType*	pointer;
		///
		typedef const DataType&	reference;
		///
		typedef std::input_iterator_tag iterator_category;
		//@}

		/**	@name	Constructors and Destructors 
		*/
		//@{

		/**	Default constructor.
				This constructor creates a new iterator and registers it.
		*/
		ConstBaseIterator()
			throw();
	
		/**	Copy constructor.
				This constructor creates a new iterator from an existing one.
				@param iterator the iterator to be copied
		*/
		ConstBaseIterator(const ConstBaseIterator& iterator)
			throw();

		/**	Destructor.
				Deregisters and destructs the iterator.
		*/
		~ConstBaseIterator()
			throw();

		//@}

		/**	@name	Assignment
		*/
		//@{

		/**	Assignment operator.
				Assigns the contents of an iterator to another iterator.
				@param	iterator the iterator to be copied
		*/
		const ConstBaseIterator& operator = (const ConstBaseIterator<Container, DataType, Position, Traits>& iterator)
			throw();

		///	Swap two iterators
		BALL_INLINE void swap(ConstBaseIterator& iterator) throw() { std::swap(traits_, iterator.traits_); }
		//@}

		/**	@name	 Accessors 
		*/
		//@{

		/// Invalidate the iterator.
		void invalidate() throw() { traits_.invalidate(); }

		/// Set the traits
		BALL_INLINE void setTraits(const Traits& traits) throw() { traits_ = traits; }

		/// Get a constant reference to the traits of this iterator.
		BALL_INLINE const Traits& getTraits() const throw() { return traits_; }

		/// Get a constant reference to the traits of this iterator.
		BALL_INLINE Traits& getTraits() throw() { return traits_; }

		/// Get a constant pointer to the container of this iterator.
		BALL_INLINE const Container* getContainer() const	throw() { return traits_.getContainer(); }
		//@}

		/** @name Converters
		*/
		//@{

		/** Convert an iterator to Position.
				This method returns the position of the iterator. Note that
				Position is a template within this context and not the BALL datatype.
		*/
		operator const Position& () const	throw();

		/// Convert an iterator to its Datatype by returning a reference to the current data.
		reference operator * () const throw(Exception::InvalidIterator);

		/// Return a pointer to the current data.
		pointer operator -> () const throw(Exception::InvalidIterator);			

		/// Increment operator
		ConstBaseIterator& operator ++ () throw(Exception::InvalidIterator)
		{
			#ifdef BALL_DEBUG
				if (!isValid())
				{
					throw Exception::InvalidIterator(__FILE__, __LINE__);
				}
			#endif

			traits_.forward();
			return *this;
		}

		/// Postfix increment operator
		ConstBaseIterator operator ++ (int) throw(Exception::InvalidIterator)
		{
			#ifdef BALL_DEBUG
				if (!isValid())
				{
					throw Exception::InvalidIterator(__FILE__, __LINE__);
				}
			#endif

			ConstBaseIterator tmp(*this);
			traits_.forward();
			return tmp;
		}
		//@}

		/**	@name	Predicates
		*/
		//@{

		/** Equality operator.
				@return true if both iterators point to the same item.
		*/
		bool operator == (const ConstBaseIterator& iterator) const throw();

		/** Inequality operator. 
				@see operator ==
		*/
		bool operator != (const ConstBaseIterator& iterator) const
			throw();

		/** Singularity predicate.
				This method returns <b>true</b> if the iterator is singular, i.e., 
				not associated with a container.
		*/
		BALL_INLINE bool isSingular() const	throw() { return traits_.isSingular(); }

		/** Validity predicate
				@return true if the iterator is valid (pointing at an element in a container) 
		*/
		BALL_INLINE bool isValid() const throw() { return traits_.isValid(); }
		/// Validity predicate
		BALL_INLINE bool operator + () const throw() { return traits_.isValid(); }
		/// Invalidity perdicate
		BALL_INLINE bool operator - () const throw() { return !traits_.isValid(); }
		//@}

		protected:

		/** Constructor.
				Protected to allow instantiation and use in derived classes only.
		*/
		ConstBaseIterator(const Container& container)
			throw();

		private:

		/// The instance of the iterator's traits
		Traits traits_;
	};


	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstBaseIterator<Container, DataType, Position, Traits>::ConstBaseIterator()
		throw()
	{
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstBaseIterator<Container, DataType, Position, Traits>::ConstBaseIterator(const ConstBaseIterator& iterator) throw()
	{
		traits_ = iterator.traits_;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	ConstBaseIterator<Container, DataType, Position, Traits>::~ConstBaseIterator()
		throw()
	{
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	const ConstBaseIterator<Container, DataType, Position, Traits>& 
		ConstBaseIterator<Container, DataType, Position, Traits>::operator = 
		(const ConstBaseIterator<Container, DataType, Position, Traits>& iterator)
		throw()
	{
		// avoid self assignment
		if (this != &iterator)
		{
			traits_ = iterator.traits_;
		}

		return *this;
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BALL_INLINE
	ConstBaseIterator<Container, DataType, Position, Traits>::operator const Position& () const
		throw()
	{
		return traits_.getPosition();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BALL_INLINE
	const DataType& ConstBaseIterator<Container, DataType, Position, Traits>::operator * () const
		throw(Exception::InvalidIterator)
	{
		#ifdef BALL_DEBUG
			if (!traits_.isValid())
			{
				throw Exception::InvalidIterator(__FILE__, __LINE__);
			}
		#endif

		return (reference)traits_.getData();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BALL_INLINE
	const DataType* ConstBaseIterator<Container, DataType, Position, Traits>::operator -> () const
		throw(Exception::InvalidIterator)
	{
		#ifdef BALL_DEBUG
			if (!traits_.isValid())
			{
				throw Exception::InvalidIterator(__FILE__, __LINE__);
			}
		#endif

		return (pointer)&traits_.getData();
	}


	template <typename Container, typename DataType, typename Position, typename Traits>
	BALL_INLINE
	bool ConstBaseIterator<Container, DataType, Position, Traits>::operator == (const ConstBaseIterator& iterator) const
		throw()
	{
		return (traits_ == iterator.traits_);
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BALL_INLINE
	bool ConstBaseIterator<Container, DataType, Position, Traits>::operator != (const ConstBaseIterator& iterator) const
		throw()
	{
		return !(traits_ == iterator.traits_);
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BALL_INLINE
	ConstBaseIterator<Container, DataType, Position, Traits>::ConstBaseIterator(const Container& container)
		throw()
		:	traits_(container)
	{
	}

	/**	Constant Basic Iterator.
			
	* 	 \addtogroup  ConceptsIterators
	*/
	template <typename Container, typename DataType, typename Position, typename Traits>
	class BaseIterator
		: public ConstBaseIterator<Container, DataType, Position, Traits>
	{
		public:

		/**	@name Typedefs.
				The names of these typedefs deviate from the usual
				BALL class names due to restrictions imposed by STL compliance.
		*/
		//@{
		
		///
		typedef DataType	value_type;
		///
		typedef Position	difference_type;
		///
		typedef	DataType*	pointer;
		///
		typedef DataType&	reference;
		///
		typedef std::input_iterator_tag iterator_category;
		//@}

		/**	@name	Constructors and Destructors
		*/
		//@{

		///	Default constructor
		BaseIterator() throw() {}
	
		/// Copy constructor
		BaseIterator(const BaseIterator& iterator)
			throw()
			:	ConstBaseIterator<Container, DataType, Position, Traits>(iterator)
		{
		}

		///
		BaseIterator(const ConstBaseIterator<Container, DataType, Position, Traits>& iterator)
			throw()
			:	ConstBaseIterator<Container, DataType, Position, Traits>(iterator)
		{
		}
		//@}

		/** @name Assignment
		*/
		//@{
		///
		BaseIterator& operator = (const BaseIterator& iterator) throw();
		///
		BALL_INLINE void swap(BaseIterator& iterator) throw() { std::swap(traits_, iterator.traits_); }
		//@}

		/**	@name	Accessors
		*/
		//@{

		/** Return a mutable pointer to the bound container.
		*/
		BALL_INLINE Container* getContainer() throw() { return ConstBaseIterator<Container, DataType, Position, Traits>::getTraits().getContainer(); }

		/** Return a non-mutable pointer to the bound container.
		*/
		BALL_INLINE const Container* getContainer() const throw() { return ConstBaseIterator<Container, DataType, Position, Traits>::getTraits().getContainer(); }

		/** Mutable dereferenciation of the iterator
		*/
		reference operator * () const throw(Exception::InvalidIterator);

		/// Return a mutable pointer to the current data
		pointer operator -> () const throw(Exception::InvalidIterator);
    /// Increment operator
    BaseIterator& operator ++ () throw(Exception::InvalidIterator)
    {
      #ifdef BALL_DEBUG
        if (!ConstBaseIterator<Container, DataType, Position, Traits>::isValid())
        {
          throw Exception::InvalidIterator(__FILE__, __LINE__);
				}
      #endif
			ConstBaseIterator<Container, DataType, Position, Traits>::operator ++ ();
      return *this;
		}

    /// Postfix increment operator
    BaseIterator operator ++ (int) throw(Exception::InvalidIterator)
    {
      #ifdef BALL_DEBUG
        if (!ConstBaseIterator<Container, DataType, Position, Traits>::isValid())
        {
          throw Exception::InvalidIterator(__FILE__, __LINE__);
				}
      #endif

      BaseIterator tmp(*this);
			++(*this);
      return tmp;
		}
		//@}

		protected:

		//_
		BaseIterator(const Container& container)
			throw();
	};


	template <typename Container, typename DataType, typename Position, typename Traits>
	BALL_INLINE
	DataType& BaseIterator<Container, DataType, Position, Traits>::operator * () const
		throw(Exception::InvalidIterator)
	{
		#ifdef BALL_DEBUG
			if (!ConstBaseIterator<Container, DataType, Position, Traits>::getTraits().isValid())
			{
				throw Exception::InvalidIterator(__FILE__, __LINE__);
			}
		#endif

		return (reference)ConstBaseIterator<Container, DataType, Position, Traits>::getTraits().getData();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BALL_INLINE
	DataType* BaseIterator<Container, DataType, Position, Traits>::operator -> () const
		throw(Exception::InvalidIterator)
	{
		#ifdef BALL_DEBUG
			if (!ConstBaseIterator<Container, DataType, Position, Traits>::getTraits().isValid())
			{
				throw Exception::InvalidIterator(__FILE__, __LINE__);
			}
		#endif

		return (pointer)&BaseIterator<Container, DataType, Position, Traits>::getTraits().getData();
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BALL_INLINE
	BaseIterator<Container, DataType, Position, Traits>::BaseIterator(const Container& container)
		throw()
		:	ConstBaseIterator<Container, DataType, Position, Traits>(container)
	{
	}

	template <typename Container, typename DataType, typename Position, typename Traits>
	BALL_INLINE
	BaseIterator<Container, DataType, Position, Traits>& 
	BaseIterator<Container, DataType, Position, Traits>::operator = 
		(const BaseIterator<Container, DataType, Position, Traits>& iterator)
		throw()
	{
		if (this != &iterator)
		{
			traits_ = iterator.traits_;
		}
		return *this;
	}

} // namespace BALL

#endif // BALL_CONCEPT_BASEITERATOR_H
