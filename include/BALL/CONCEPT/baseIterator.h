// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
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
			This template class implements the basic behaviour of an iterator.
			Iterators are basically STL-like iterators. They provide the full STL
			iterator interface, but also offer additional features.
			\par

			BaseIterator itself is a base class to the other iterator classes only
			and should not be used by itself.
			\par
	
	 		An important difference exists for the iterators of the kernel objects.
			For most kernel objects, multiple iterators exist.  Therefore, we could
			not simply use <tt>begin()</tt> and <tt>end()</tt> like in STL, but we
			introduced specialized methods like
			\link AtomContainer::beginAtom AtomContainer::beginAtom \endlink and
			\link AtomContainer::endAtom AtomContainer::endAtom \endlink. For
			similar reasons, the iterators for kernel classes are not implemented as
			nested classes of the respective kernel classes, but as independent
			classes to avoid code replication. An exception is
			\link Atom::BondIterator Atom::BondIterator \endlink , which is
			relevant to \link Atom Atom \endlink alone.
			\par

			Each BALL iterator can be bound to a container, so once the iteration
			has started, it "knows" about the end() of the container.  Therefore,
			BALL iterators additionally implement the unary plus operator to
			check for the validity of the iterator.  this allows the convenient
			implementation of for loops, e.g. as follows: 
			\par
 
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
	class BaseIterator
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
		///	Default constructor
		BALL_INLINE BaseIterator()  {}
	
		///	Copy constructor
		BALL_INLINE BaseIterator(const BaseIterator& iterator) 
			:	traits_(iterator.traits_)
		{
		}

		///	Destructor.
		BALL_INLINE ~BaseIterator()  {}
		//@}

		/**	@name	Assignment
		*/
		//@{

		/**	Assignment operator.
				Assigns the contents of an iterator to another iterator.
				@param	iterator the iterator to be copied
		*/
		BALL_INLINE BaseIterator& operator = (const BaseIterator& iterator) 
		{
			traits_ = iterator.traits_;
			return *this;
		}
			
		///	Swap two iterators
		BALL_INLINE void swap(BaseIterator& iterator)  { std::swap(traits_, iterator.traits_); }
		//@}

		/**	@name	 Accessors 
		*/
		//@{

		/// Invalidate the iterator.
		BALL_INLINE void invalidate()  { traits_.invalidate(); }

		/// Set the traits
		BALL_INLINE void setTraits(const Traits& traits)  { traits_ = traits; }

		/// Get a constant reference to the traits of this iterator.
		BALL_INLINE const Traits& getTraits() const  { return traits_; }

		/// Get a constant reference to the traits of this iterator.
		BALL_INLINE Traits& getTraits()  { return traits_; }

		/// Get a constant pointer to the container of this iterator.
		BALL_INLINE const Container* getContainer() const	 { return traits_.getContainer(); }
		//@}

		/** @name Converters
		*/
		//@{

		/** Convert an iterator to Position.
				This method returns the position of the iterator. Note that
				Position is a template within this context and not the BALL datatype.
		*/
		BALL_INLINE operator const Position& () const  { return traits_.getPosition(); }

		/// Convert an iterator to its Datatype by returning a reference to the current data.
		BALL_INLINE reference operator * () const  { return (reference)traits_.getData(); }

		/// Return a pointer to the current data.
		BALL_INLINE pointer operator -> () const  { return (pointer)&traits_.getData(); }
		//@}

		/**	@name	Predicates
		*/
		//@{

		/// Equality operator
		BALL_INLINE bool operator == (const BaseIterator& iterator) const  { return (traits_ == iterator.traits_); }

		/// Inequality operator
		BALL_INLINE bool operator != (const BaseIterator& iterator) const  { return !(traits_ == iterator.traits_); }

		/** Singularity predicate.
				This method returns <b>true</b> if the iterator is singular, i.e., 
				not associated with a container.
		*/
		BALL_INLINE bool isSingular() const	 { return traits_.isSingular(); }

		/** Validity predicate
				@return true if the iterator is valid (pointing at an element in a container) 
		*/
		BALL_INLINE bool isValid() const  { return traits_.isValid(); }

		/// Validity predicate
		BALL_INLINE bool operator + () const  { return traits_.isValid(); }

		/// Invalidity perdicate
		BALL_INLINE bool operator - () const  { return !traits_.isValid(); }
		//@}

		protected:

		/** Constructor.
				Protected to allow instantiation and use in derived classes only.
		*/
		BALL_INLINE BaseIterator(const Container& container) 
			:	traits_(container)
		{
		}

		private:

		/// The instance of the iterator's traits
		Traits traits_;
	};


} // namespace BALL

#endif // BALL_CONCEPT_BASEITERATOR_H
