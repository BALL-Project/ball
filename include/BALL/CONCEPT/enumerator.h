// $Id: enumerator.h,v 1.19.4.3 2002/06/06 22:21:59 oliver Exp $

#ifndef BALL_CONCEPT_ENUMERATOR_H
#define BALL_CONCEPT_ENUMERATOR_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_COMMON_EXCEPTION_H
# include <BALL/COMMON/exception.h>
#endif

#ifndef BALL_COMMON_RTTI_H
# include <BALL/COMMON/rtti.h>
#endif

#ifndef BALL_CONCEPT_FORWARDITERATOR_H
#	include <BALL/CONCEPT/forwardIterator.h>
#endif

#include <vector>
#include <list>
#include <algorithm>

namespace BALL
{

	/** EnumeratorIndex class.
			The Enumerator class provides a means for enumerating all possible
			combinations of objects. The counting is hereby done by the
			EnumeratorIndex class.  This class is derived from vector and
			additionally contains two vectors of the same size which hold the
			modulus and the base for each digit. The values of the EnumeratorIndex
			vector itself are interpreted as an inhomogenous number consisting of
			digits that have different bases. The bases are the numbers of
			possibilities for each variant in the list. Most significant component
			is operator [] (0), so incrementing starts at operator [] (size() - 1)
			\\
			{\bf Definition:} \URL{BALL/CONCEPT/enumerator.h}
	*/
	class EnumeratorIndex
		: private std::vector<Position>
	{
		public:

		/** @name Nested Classes
		 */
		//@{
		/** Exception for reporting incompatible EnumeratorIndex instances, 
				i.e. instances with different moduli. {\bf Definition:}
				\URL{BALL/CONCEPT/enumerator.h}
		*/	 
		class IncompatibleIndex
			: public Exception::GeneralException
		{
			public:

			IncompatibleIndex(const char* file, int line)
				throw();
		};

		//@}

		/** @name Constructors and Destructors
		 */
		//@{    
			
		/** Default Constructor
		 */
		EnumeratorIndex()
			throw();

		/** Detailed Constructor
			  @param variant_list the list of variants to be applied
		 */
		template <typename Variant, typename VariantIterator>
		EnumeratorIndex(const std::list<std::pair<VariantIterator, std::vector<Variant> > >& variant_list)
			throw();

		/** Destructor
		 */
		~EnumeratorIndex()
			throw();

		//@}

		/**	@name Assignment
		*/
		//@{
		/** Assignment operator.
		 */
		const EnumeratorIndex& operator = (const EnumeratorIndex& rhs)
			throw();

		/** Assignment operator for Position.
		 */
		const EnumeratorIndex& operator = (Position index)
			throw(Exception::IndexOverflow);
		//@}

		/** @name Accessors
		 */
		//@{

		/** Access the modulus part of the enumerator
		 */
		const std::vector<Size>& getModulus() const
			throw();

		/**	Access the modulus of a specific position
		*/
		Size getModulus(Position pos) const
			throw();
			
		/** Increment an instance of EnumeratorIndex. 
				Increment the least significant component and apply any overflow to
				more signficant components.
		 */
		EnumeratorIndex& operator ++ ()
			throw(Exception::IndexOverflow);

		/** Decrement an instance of EnumeratorIndex. 
				Decrement the least significant component and 
				apply any underflow to more signficant components.
		 */
		EnumeratorIndex& operator -- ()
			throw(Exception::IndexUnderflow);

		/**	Random access operator
		*/
		Position operator [] (Position pos) const
			throw();

		/**	Random access operator
		*/
		Position& operator [] (Position pos)
			throw();

		/**	Return the size of the array.
		*/
		Size getSize() const
			throw();

		/**	Add a new digit.	
				Push the modulus to the ack of the modulus array and
				initialize the corresponding digit with zero.
				\\
				{\bf Example:} To construct a hex counter with three
				digits, you can use something like:
				\begin{verbatim}
					EnumerationIndex counter;
					counter << 16 << 16 << 16;
				\end{verbatim}
				The counter will be initialized with all zeros.
			@exception OutOfRange if the modulus is less than 2
		*/
		EnumeratorIndex& operator << (Size modulus)
			throw(Exception::OutOfRange);
		//@}
		
		/** @name Predicates for EnumeratorIndex class
		 */
		//@{

		/** Equality operator.
		 * @param rhs
		 * @return true, if {\tt rhs} is equal to {\tt this} instance
		 */
		bool operator == (const EnumeratorIndex& rhs)
			throw();

		/** Inequality operator.
		 * @param rhs
		 * @return true, if {\tt rhs} is not equal to {\tt this} instance
		 */
		bool operator != (const EnumeratorIndex& rhs)
			throw();

		/** Greater than operator.
		 * @param rhs
		 * @return true, if {\tt rhs} is greater than {\tt this} instance
			 @exception EnumeratorIndex::Incompatible index if the modulus vector differs, i.e. the two indices aren't comparable
		 */
		bool operator > (const EnumeratorIndex& rhs)
			throw(EnumeratorIndex::IncompatibleIndex);

		/** Lesser than operator.
		 * @param rhs
		 * @return true, if {\tt rhs} is lesser than {\tt this} instance
		 *  @exception EnumeratorIndex::Incompatible index if the modulus vector differs, i.e. the two indices aren't comparable
		 */
		bool operator < (const EnumeratorIndex& rhs)
			throw(EnumeratorIndex::IncompatibleIndex);
				
		/** Greater or equal operator.
		 * @param rhs
		 * @return true, if {\tt rhs} is greater than or equal to {\tt this} instance
		 *  @exception EnumeratorIndex::Incompatible index if the modulus vector differs, i.e. the two indices aren't comparable
		 */
		bool operator >= (const EnumeratorIndex& rhs)
			throw(EnumeratorIndex::IncompatibleIndex);

		/** Lesser or equal operator.
		 * @param rhs
		 * @return true, if {\tt rhs} is lesser than or equal to {\tt this} instance
		 *  @exception EnumeratorIndex::Incompatible index if the modulus vector differs, i.e. the two indices aren't comparable
		 */
		bool operator <= (const EnumeratorIndex& rhs)
			throw(EnumeratorIndex::IncompatibleIndex);
		//@}

		private:
		///
		std::vector<Size>	modulus_;
		///
		std::vector<Size>	base_multipliers_;
	};


	template <typename Variant, typename VariantIterator>
	EnumeratorIndex::EnumeratorIndex(const std::list<std::pair<VariantIterator, std::vector<Variant> > >& variant_list)
		throw()
		: std::vector<Position>(variant_list.size()),
			modulus_(variant_list.size()),
			base_multipliers_(variant_list.size())
	{ 
		// compute the base multipliers for later usage 
		Index i;
		Size multiplier = 1;
		typename std::list<std::pair<VariantIterator, std::vector<Variant> > >::const_iterator list_it = variant_list.begin();
		for (i = (Size)(size() - 1); i >= 0; i--, list_it++)
		{
			operator[](i) = 0;
			modulus_[i] = (Size)list_it->second.size();

			base_multipliers_[i] = multiplier;
			multiplier *= modulus_[i];
		}
	}


	/** Enumerator class.
			The EnumeratorIndex class is designed to enumerate all possible
			combinations of things. Applications are e.g. the enumeration of
			all possible sequences defined through a multisequence or
			enumerating all possible rotamers of a peptide or a bindings site.
			\\
			Enumerator uses \Ref{EnumeratorIndex} as an inhomogeneous counter
			class. It is also highly templatized in order to be adaptable to
			most problem instances. In general, the enumeration problem can 
			be seen as counting with a mixed-base number. For an example of 
			the Enumerator's usage, please refer to the tutorial.
			\\
			The Enumerator's template arguments are 
			\begin{itemize}
				\item the {\tt Container}
					it operates on (e.g. the \Ref{String} representing the sequence
					or the protein containing the amino acids)
				\item the {\tt PositionIterator} (i.e. an iterator pointing to a 
					defined position within the container)
				\item the {\tt Variant} type (i.e. the type of the object to be enumerated).
					It has to be the same type as the dereferenced {\tt PositionIterator}.
			\end{itemize}
			In the case of a string sequence that has to be mutated, the {\tt Container}
			is of class \Ref{String}, the {\tt PositionIterator} is of type
			{\tt String::Iterator}, and {\tt Variant} is obviously of type {\tt char}.
			\\
			{\bf Definition:} \URL{BALL/CONCEPT/enumerator.h}
	*/
	template <class Container, class PositionIterator, class Variant>
	class Enumerator
	{	
		protected:
		class IteratorTraits_;
		
		public:

		/**	@name Type definitions
		*/
		//@{
			
		/**
		*/
		typedef void (*MutatorFunction)(Variant&, Variant&);

		/**
		*/
		typedef std::vector<Variant>							
			VariantVector;

		/**
		*/
		typedef std::pair<PositionIterator, VariantVector>
			Site;

		/**
		*/
		typedef std::list<Site>
			SiteList;

		/** Mutable forward iterator
		*/
		typedef ForwardIterator<Enumerator<Container, PositionIterator, Variant>, EnumeratorIndex, EnumeratorIndex*, IteratorTraits_>
			Iterator;

		/** Constant forward iterator
		*/
		typedef ConstForwardIterator<Enumerator<Container, PositionIterator, Variant>, EnumeratorIndex, EnumeratorIndex*, IteratorTraits_>
			ConstIterator;

		//@}
		/** @name Constructors and Destructors
		*/
		//@{

		/** Default Constructor
		*/
		Enumerator()
			throw();

		/** Detailed Constructor.
				The mutator function is set to a default mutator,
				using the assignment operator for {\tt Variant}.
				@param container a Container class to be mutated
		 */
		Enumerator(Container& container)
			throw();

		/** Detailed Constructor
				@param container a Container class to be mutated
				@param mutator the function defining the mutations to be applied
		 */
		Enumerator(Container& container, MutatorFunction mutator)
			throw();

		/** Default Destructor
		 */
		~Enumerator()
			throw()
		{
		}
		
		//@}
		/** @name Accessors
		 */
		//@{

		/** Add variants to the list of variants
		 */
		void addVariants(const PositionIterator& it, const VariantVector& variants)
			throw()
		{
			variant_sites_.push_back(Site(it, variants));
		}

		/** Delete variants from the list of variants
		 */
		void deleteVariants(const PositionIterator& it, const VariantVector& variants)
			throw()
		{
			typename SiteList::iterator var_it;
			var_it = std::find(variant_sites_.begin(), variant_sites_.end(), Site(it, variants));
			if (var_it != variant_sites_.end())
			{
				variant_sites_.erase(var_it);
			}
		}

		/** Count all variants.
				@return the number of all possible variants
		 */
		Size countVariants()
			throw()
		{
			Size total = 1;
			typename SiteList::iterator it;
			for (it = variant_sites_.begin(); it != variant_sites_.end(); ++it)
			{
				total *= it->second.size();
			}
			return total;
		}

		/** Access the current content
       @return a reference to the container class of this enumerator
		 */
		Container& getCurrent()
			throw();

		/** Create a permuatation denoted by its number.
		    @param index the number of the permutation to be created
		 */
		void createPermutation(const Position index)
			throw();

		/** Create a permutation denoted by an instance of EnumeratorIndex.
				@param index the instance of EnumeratorIndex that describes the
				permutation to be created
		 */
		void createPermutation(const EnumeratorIndex& index)
			throw(EnumeratorIndex::IncompatibleIndex);
		//@}

		/**	@name Iterators
		*/
		//@{
		
		///
		Iterator begin()
			throw();

		///
		Iterator end()
			throw();

		///
		ConstIterator begin()
			throw();

		///
		ConstIterator end()
			throw();
		//@}

		protected:

		/**
		*/
		class IteratorTraits_
		{
			friend class Enumerator<Container, PositionIterator, Variant>;
			
			public:
				
			typedef Enumerator<Container, PositionIterator, Variant>	
				ContainerType;

			typedef Enumerator<Container, PositionIterator, Variant>* 
				ContainerPointer;

			typedef EnumeratorIndex														
				IteratorPosition;

			typedef EnumeratorIndex
				ValueType;

			IteratorTraits_()
				throw()
				:	bound_(0),	
					position_(0)
			{
			}
				
			IteratorTraits_(const ContainerType& enumerator)
				throw()
				:	bound_(const_cast<ContainerPointer>(&enumerator)),
					position_(0)
			{
			}
			
			
			IteratorTraits_(const IteratorTraits_& traits)
				throw()
				:	bound_(traits.bound_),
					position_(traits.position_)
			{
			}
			
			const IteratorTraits_& operator = (const IteratorTraits_& traits)
				throw()
      {
        bound_ = traits.bound_;
        position_ = traits.position_;
 
        return *this;
			}

			const ContainerType* getContainer() const	
				throw()
			{
				return bound_;
			}
			
			ContainerPointer getContainer()
				throw()
			{
				return bound_;
			}

			bool isSingular() const
				throw()
			{
				return (bound_ == 0);
			}
 
      IteratorPosition& getPosition()
				throw()
      {
        return position_;
			}

      const IteratorPosition& getPosition() const
				throw()
      {
        return position_;
			}

      bool operator == (const IteratorTraits_& traits) const
				throw()
      {
        return (position_ == traits.position_);
			}

      bool operator != (const IteratorTraits_& traits) const
				throw()
      {
        return (position_ != traits.position_);
			}
      bool isValid() const
				throw()
      {
        return (bound_ != 0);
			}

      void invalidate()
				throw()
      {
        bound_ = 0;
        position_ = 0;
			}

			void toBegin()
				throw()
			{
				position_ = 0;
			}

			bool isBegin() const
				throw()
			{
				return (position_ == 0);
			}
			
			void toEnd()
				throw()
			{
				position_ = position_.getNumberOfCombinations();
			}
			
			bool isEnd() const
				throw()
			{
				return (position_ == position_.getNumberOfCombinations());
			}
			
			ValueType& getData()
				throw()
			{
				return position_;
			}
			
			const ValueType& getData() const
				throw()
			{
				return position_;
			}
			
			void forward()
				throw()
			{
				position_++;
			}
			
			protected:
			
			ContainerPointer	bound_;
			IteratorPosition	position_;
		};

		
		static void default_assign_(Variant& a, Variant& b)
		{
			a = b;
		}

		void mutate_(PositionIterator& it, Variant& v)
			throw()
		{
			mutator_(*it, v);
		}

		Container&			container_;
		MutatorFunction mutator_;
		SiteList				variant_sites_;
	};

	template <typename Container, typename PositionIterator, typename Variant>
	Enumerator<Container, PositionIterator, Variant>::Enumerator()
			throw()
		: container_(const_cast<Container&>(RTTI::getDefault<Container>())),
			mutator_(0)
	{
	}
		
	template <typename Container, typename PositionIterator, typename Variant>
	Enumerator<Container, PositionIterator, Variant>::Enumerator(Container& container)
			throw()
		: container_(container),
			mutator_(default_assign_)
	{
	}
		
	template <typename Container, typename PositionIterator, typename Variant>
	BALL_INLINE
	Enumerator<Container, PositionIterator, Variant>::Enumerator
		(Container& container, Enumerator<Container, PositionIterator, Variant>::MutatorFunction mutator)
		throw()
		: container_(container),
			mutator_(mutator)
	{
	}


	template <typename Container, typename PositionIterator, typename Variant>
	BALL_INLINE
	Container& Enumerator<Container, PositionIterator, Variant>::getCurrent()
		throw()
	{
		return container_;
	}
	
	template <typename Container, typename PositionIterator, typename Variant>
	void Enumerator<Container, PositionIterator, Variant>::createPermutation(const Position index)
		throw()
	{
		EnumeratorIndex enumerator_index(variant_sites_);
		enumerator_index = index;
		try
		{
			createPermutation(enumerator_index);
		}
		catch (EnumeratorIndex::IncompatibleIndex&)
		{
			throw Exception::IndexOverflow(__FILE__, __LINE__, index);
		}
	}

	template <typename Container, typename PositionIterator, typename Variant>
	void Enumerator<Container, PositionIterator, Variant>::createPermutation(const EnumeratorIndex& index)
		throw(EnumeratorIndex::IncompatibleIndex)
	{
		if (index.getSize() != variant_sites_.size())
		{
			throw EnumeratorIndex::IncompatibleIndex(__FILE__, __LINE__);
		}

		typename SiteList::iterator it = variant_sites_.begin();
		Position i((Position)(index.getSize() - 1));
		for (; it != variant_sites_.end(); ++it, --i)
		{
			mutate_(it->first, it->second[index[i]]);
		}
	}

	template <typename Container, typename PositionIterator, typename Variant>
	BALL_INLINE
	typename Enumerator<Container, PositionIterator, Variant>::Iterator Enumerator<Container, PositionIterator, Variant>::begin()
		throw()
	{
		return Iterator::begin(this);
	}

	template <typename Container, typename PositionIterator, typename Variant>
	BALL_INLINE
	typename Enumerator<Container, PositionIterator, Variant>::Iterator Enumerator<Container, PositionIterator, Variant>::end()
		throw()
	{
		return Iterator::end(this);
	}

	template <typename Container, typename VariantConstIterator, typename Variant>
	BALL_INLINE
	typename Enumerator<Container, VariantConstIterator, Variant>::ConstIterator Enumerator<Container, VariantConstIterator, Variant>::begin()
		throw()
	{
		return ConstIterator::begin(this);
	}

	template <typename Container, typename VariantConstIterator, typename Variant>
	BALL_INLINE
	typename Enumerator<Container, VariantConstIterator, Variant>::ConstIterator Enumerator<Container, VariantConstIterator, Variant>::end()
		throw()
	{
		return ConstIterator::end(this);
	}

	
#	ifndef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/CONCEPT/enumerator.iC>
#	endif

}

#endif // BALL_CONCEPT_ENUMERATOR_H
