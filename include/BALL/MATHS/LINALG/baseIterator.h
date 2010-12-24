#ifndef BALL_LINALG_BASEITERATOR_H
#define BALL_LINALG_BASEITERATOR_H

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
	template <typename Container, typename DataType, typename Position, typename Traits>
	class BaseIterator
	{
		public:

		/**	@name Typedefs.
				The names of these typedefs deviate from the usual
				class names due to restrictions imposed by STL compliance.
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
		typedef Container	container_type;		
		///
		typedef Traits	traits_type;
		///
		typedef std::input_iterator_tag iterator_category;
		//@}

		/**	@name	Constructors and Destructors 
		*/
		//@{
		///	Default constructor
		BaseIterator() {}
	
		///	Copy constructor
		BaseIterator(const BaseIterator& iterator)
		 : traits_(iterator.traits_)
		{
		}

		///	Destructor.
		~BaseIterator() {}
		//@}

		/**	@name	Assignment
		*/
		//@{

		/**	Assignment operator.
				Assigns the contents of an iterator to another iterator.
				@param	iterator the iterator to be copied
		*/
		BaseIterator& operator = (const BaseIterator& iterator)
		{
			traits_ = iterator.traits_;
			return *this;
		}
			
		///	Swap two iterators
		void swap(BaseIterator& iterator) { std::swap(traits_, iterator.traits_); }
		//@}

		/**	@name	 Accessors 
		*/
		//@{

		/// Invalidate the iterator.
		void invalidate() { traits_.invalidate(); }

		/// Set the traits
		void setTraits(const Traits& traits) { traits_ = traits; }

		/// Get a constant reference to the traits of this iterator.
		const Traits& getTraits() const { return traits_; }

		/// Get a constant reference to the traits of this iterator.
		Traits& getTraits() { return traits_; }

		/// Get a constant pointer to the container of this iterator.
		const Container* getContainer() const { return traits_.getContainer(); }
		//@}

		/** @name Converters
		*/
		//@{

		/** Convert an iterator to Position.
				This method returns the position of the iterator. 
				Position is a template within this context.
		*/
		operator const Position& () const { return traits_.getPosition(); }

		/// Convert an iterator to its Datatype by returning a reference to the current data.
		reference operator * () const { return (reference)traits_.getData(); }

		/// Return a pointer to the current data.
		pointer operator -> () const { return (pointer)&traits_.getData(); }
		//@}

		/**	@name	Predicates
		*/
		//@{

		/// Equality operator
		bool operator == (const BaseIterator& iterator) const { return (traits_ == iterator.traits_); }

		/// Inequality operator
		bool operator != (const BaseIterator& iterator) const { return !(traits_ == iterator.traits_); }

		/** Singularity predicate.
				This method returns <b>true</b> if the iterator is singular, i.e., 
				not associated with a container.
		*/
		bool isSingular() const { return traits_.isSingular(); }

		/** Validity predicate
				@return true if the iterator is valid (pointing at an element in a container) 
		*/
		bool isValid() const { return traits_.isValid(); }

		/// Validity predicate
		bool operator + () const { return traits_.isValid(); }

		/// Invalidity predicate
		bool operator - () const { return !traits_.isValid(); }
		//@}

		protected:

		/** Constructor.
				Protected to allow instantiation and use in derived classes only.
		*/
		BaseIterator(const Container& container)
			:	traits_(container)
		{
		}

		private:

		/// The instance of the iterator's traits
		Traits traits_;
	};


} // namespace BALL

#endif // BALL_KERNEL_BASEITERATOR_H
