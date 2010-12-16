// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

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
			 \par
			
	 	 \ingroup ConceptsMiscellaneous
	*/
	class BALL_EXPORT EnumeratorIndex
		: private std::vector<Position>
	{
		public:

		/** @name Nested Classes
		 */
		//@{
		/** Exception for reporting incompatible EnumeratorIndex instances, 
				i.e. instances with different moduli.
		*/	 
		class BALL_EXPORT IncompatibleIndex
			: public Exception::GeneralException
		{
			public:

			IncompatibleIndex(const char* file, int line);
		};

		//@}

		/** @name Constructors and Destructors
		 */
		//@{    
			
		/** Default Constructor
		 */
		EnumeratorIndex();

		/** Detailed Constructor
			  @param variant_list the list of variants to be applied
		 */
		template <typename Variant, typename VariantIterator>
		EnumeratorIndex(const std::list<std::pair<VariantIterator, std::vector<Variant> > >& variant_list);

		/** Destructor
		 */
		~EnumeratorIndex();

		//@}

		/**	@name Assignment
		*/
		//@{
		/** Assignment operator.
		 */
		const EnumeratorIndex& operator = (const EnumeratorIndex& rhs);

		/** Assignment operator for Position.
		 *  \throws Exception::IndexOverflow
		 */
		const EnumeratorIndex& operator = (Position index);

		/** Assignment operator for a variant list.
		*/
		template <typename Variant, typename VariantIterator>
		const EnumeratorIndex& operator = (const std::list<std::pair<VariantIterator, std::vector<Variant> > >& variant_list);
		//@}

		/** @name Accessors
		 */
		//@{

		/** Access the modulus part of the enumerator
		 */
		const std::vector<Size>& getModulus() const;

		/**	Access the modulus of a specific position
		*/
		Size getModulus(Position pos) const;
			
		/** Increment an instance of EnumeratorIndex. 
				Increment the least significant component and apply any overflow to
				more signficant components.
				\throws Exception::IndexOverflow
		 */
		EnumeratorIndex& operator ++ ();

		/** Decrement an instance of EnumeratorIndex. 
				Decrement the least significant component and 
				apply any underflow to more signficant components.
				\throws Exception::IndexUnderflow
		 */
		EnumeratorIndex& operator -- ();

		/**	Random access operator
		*/
		Position operator [] (Position pos) const;

		/**	Random access operator
		*/
		Position& operator [] (Position pos);

		/**	Return the size of the array.
		*/
		Size getSize() const;

		/**	Add a new digit.	
				Push the modulus to the ack of the modulus array and
				initialize the corresponding digit with zero.
				 \par
				<b>Example:</b> To construct a hex counter with three
				digits, you can use something like:
				<tt> EnumerationIndex counter;</tt>\par
				<tt>	counter << 16 << 16 << 16;</tt>\par
				The counter will be initialized with all zeros.
			@exception OutOfRange if the modulus is less than 2
		*/
		EnumeratorIndex& operator << (Size modulus);
		//@}
		
		/** @name Predicates for EnumeratorIndex class
		 */
		//@{

		/** Equality operator.
		 * @param rhs
		 * @return true, if <tt>rhs</tt> is equal to <tt>this</tt> instance
		 */
		bool operator == (const EnumeratorIndex& rhs) const;

		/** Inequality operator.
		 * @param rhs
		 * @return true, if <tt>rhs</tt> is not equal to <tt>this</tt> instance
		 */
		bool operator != (const EnumeratorIndex& rhs) const;

		/** Greater than operator.
		 * @param rhs
		 * @return true, if <tt>rhs</tt> is greater than <tt>this</tt> instance
			 @exception EnumeratorIndex::Incompatible index if the modulus vector differs, i.e. the two indices aren't comparable
		 */
		bool operator > (const EnumeratorIndex& rhs) const;

		/** Lesser than operator.
		 * @param rhs
		 * @return true, if <tt>rhs</tt> is lesser than <tt>this</tt> instance
		 *  @exception EnumeratorIndex::Incompatible index if the modulus vector differs, i.e. the two indices aren't comparable
		 */
		bool operator < (const EnumeratorIndex& rhs) const;
				
		/** Greater or equal operator.
		 * @param rhs
		 * @return true, if <tt>rhs</tt> is greater than or equal to <tt>this</tt> instance
		 *  @exception EnumeratorIndex::Incompatible index if the modulus vector differs, i.e. the two indices aren't comparable
		 */
		bool operator >= (const EnumeratorIndex& rhs) const;

		/** Lesser or equal operator.
		 * @param rhs
		 * @return true, if <tt>rhs</tt> is lesser than or equal to <tt>this</tt> instance
		 *  @exception EnumeratorIndex::Incompatible index if the modulus vector differs, i.e. the two indices aren't comparable
		 */
		bool operator <= (const EnumeratorIndex& rhs) const;
		//@}

		private:
		///
		std::vector<Size>	modulus_;
		///
		std::vector<Size>	base_multipliers_;
	};


	template <typename Variant, typename VariantIterator>
	EnumeratorIndex::EnumeratorIndex(const std::list<std::pair<VariantIterator, std::vector<Variant> > >& variant_list)
		: std::vector<Position>(variant_list.size()),
			modulus_(variant_list.size()),
			base_multipliers_(variant_list.size())
	{ 
		this->operator = (variant_list);
	}


	template <typename Variant, typename VariantIterator>
	const EnumeratorIndex& EnumeratorIndex::operator = 
		(const std::list<std::pair<VariantIterator, std::vector<Variant> > >& variant_list)
	{ 
		resize(variant_list.size());
		modulus_.resize(variant_list.size());
		base_multipliers_.resize(variant_list.size());

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
		
		return *this;
	}


	/** Enumerator class.
			The EnumeratorIndex class is designed to enumerate all possible
			combinations of things. Applications are e.g. the enumeration of
			all possible sequences defined through a multisequence or
			enumerating all possible rotamers of a peptide or a bindings site.
			 \par
			Enumerator uses  \link EnumeratorIndex EnumeratorIndex \endlink  as an inhomogeneous counter
			class. It is also highly templatized in order to be adaptable to
			most problem instances. In general, the enumeration problem can 
			be seen as counting with a mixed-base number. For an example of 
			the Enumerator's usage, please refer to the tutorial.
			 \par
			The Enumerator's template arguments are 

				- the <tt>Container</tt>
					it operates on (e.g. the  \link String String \endlink  representing the sequence
					or the protein containing the amino acids)
				- the <tt>SiteIterator</tt> (i.e. an iterator pointing to a 
					defined position within the container)
				- the <tt>Variant</tt> type (i.e. the type of the object to be enumerated).
					It has to be the same type as the dereferenced <tt>SiteIterator</tt>.
			
			In the case of a string sequence that has to be mutated, the <tt>Container</tt>
			is of class  \link String String \endlink , the <tt>SiteIterator</tt> is of type
			<tt>String::Iterator</tt>, and <tt>Variant</tt> is obviously of type <tt>char</tt>.
			 \par
			
	*/
	template <class Container, class SiteIterator, class Variant>
	class Enumerator
	{	
		protected:
		class IteratorTraits_;
		
		public:

		/**	@name Type definitions
		*/
		//@{
			
		/** Site mutator function.
				A function used to assing one variant of a site in the 
				container to another. In the trivial case, this is just
				the assignment operator (as implemented in the default case),
				but more involved enumeration problems (e.g. side chain
				rotamer enumeration) might require additional code for the
				assignment of the true variant.
		*/
		typedef void (*MutatorFunction) (Variant&, const Variant&);

		/**	A vector containing all possible variants for a specific site.
		*/
		typedef std::vector<Variant> VariantVector;

		/**	A combination of a site position and all its possible variants.
		*/
		typedef std::pair<SiteIterator, VariantVector> Site;

		/** A list of sites.
		*/
		typedef std::list<Site> SiteList;

		/** Mutable forward iterator
		*/
		typedef ForwardIterator<Enumerator<Container, SiteIterator, Variant>, Container, EnumeratorIndex*, IteratorTraits_>
			Iterator;

		/** Constant forward iterator
		*/
		typedef ConstForwardIterator<Enumerator<Container, SiteIterator, Variant>, Container, EnumeratorIndex*, IteratorTraits_>
			ConstIterator;
		//@}

		/** @name Constructors and Destructors
		*/
		//@{

		/** Default Constructor
		*/
		Enumerator();

		/** Detailed Constructor.
				The mutator function is set to a default mutator,
				using the assignment operator for <tt>Variant</tt>.
				@param container a Container class to be mutated
		 */
		Enumerator(Container& container);

		/** Detailed Constructor
				@param container a Container class to be mutated
				@param mutator the function defining the mutations to be applied
		 */
		Enumerator(Container& container, MutatorFunction mutator);

		/** Default Destructor
		 */
		~Enumerator()
		{
		}
		//@}

		/** @name Accessors
		 */
		//@{

		/** Add variants to the list of variants
		 */
		void addVariants(const SiteIterator& it, const VariantVector& variants)
		{
			variant_sites_.push_back(Site(it, variants));
			position_ = variant_sites_;
		}

		/** Delete variants from the list of variants
		 */
		void deleteVariants(const SiteIterator& it, const VariantVector& variants)
		{
			typename SiteList::iterator var_it;
			var_it = std::find(variant_sites_.begin(), variant_sites_.end(), Site(it, variants));
			if (var_it != variant_sites_.end())
			{
				variant_sites_.erase(var_it);
			}
			position_ = variant_sites_;
		}

		/** Count all variants.
				@return the number of all possible variants
		 */
		Size countVariants()
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
		Container& getCurrent();

		/** Create a combination denoted by its number.
		    @param index the number of the combination to be created
				\throws Exception::IndexOverflow
		 */
		void createCombination(const Position index);

		/** Create a combination denoted by an instance of EnumeratorIndex.
				@param index the instance of EnumeratorIndex that describes the
				combination to be created
				\throws EnumeratorIndex::IncompatibleIndex
		 */
		void createCombination(const EnumeratorIndex& index);
		//@}

		/**	@name Iterators
		*/
		//@{
		
		///
		Iterator begin();

		///
		Iterator end();

		///
		ConstIterator begin() const;

		///
		ConstIterator end() const;
		//@}

		protected:

		friend class IteratorTraits_;

		/**
		*/
		class IteratorTraits_
		{
			friend class Enumerator<Container, SiteIterator, Variant>;
			
			public:
				
			typedef Enumerator<Container, SiteIterator, Variant>
				ContainerType;

			typedef Enumerator<Container, SiteIterator, Variant>* 
				ContainerPointer;

			typedef const Enumerator<Container, SiteIterator, Variant>* 
				ContainerConstPointer;

			typedef EnumeratorIndex
				IteratorPosition;

			typedef Container
				ValueType;

			IteratorTraits_()
				:	bound_(0),	
					position_(),
					past_the_end_(false)
			{
			}
				
			IteratorTraits_(const ContainerType& enumerator)
				:	bound_(const_cast<ContainerPointer>(&enumerator)),
					position_(enumerator.variant_sites_),
					past_the_end_(false)
			{
			}
			
			ContainerConstPointer getContainer() const	
			{
				return bound_;
			}
			
			ContainerPointer getContainer()
			{
				return bound_;
			}

			bool isSingular() const
			{
				return (bound_ == 0);
			}
 
      IteratorPosition& getPosition()
      {
        return position_;
			}

      const IteratorPosition& getPosition() const
      {
        return position_;
			}

      bool operator == (const IteratorTraits_& traits) const
      {
        return ((bound_ == traits.bound_) && (position_ == traits.position_) && (past_the_end_ == traits.past_the_end_));
			}

      bool operator != (const IteratorTraits_& traits) const
      {
				return ((bound_ != traits.bound_) || (position_ != traits.position_) || (past_the_end_ != traits.past_the_end_));
			}

      bool isValid() const
      {
        return (bound_ != 0);
			}

      void invalidate()
      {
        bound_ = 0;
        position_ = 0;
				past_the_end_ = false;
			}

			void toBegin()
			{
				position_ = 0;
				past_the_end_ = false;
			}

			bool isBegin() const
			{
				return (position_ == EnumeratorIndex()) && (past_the_end_ == false);
			}
			
			void toEnd()
			{
				position_ = 0;
				past_the_end_ = true;
			}
			
			bool isEnd() const
			{
				return past_the_end_;
			}
			
			ValueType& getData()
			{
				validate();
				return bound_->getCurrent();
			}
			
			const ValueType& getData() const
			{
				// This is logically const only!
				const_cast<typename Enumerator<Container, SiteIterator, Variant>::IteratorTraits_*>(this)->validate();
				return bound_->getCurrent();
			}
			
			void forward()
			{
				try
				{
					++position_;
				}
				catch (Exception::IndexOverflow&)
				{
					past_the_end_ = true;
					position_ = 0;
				}
			}
			
			void validate()
			{
				if (!bound_->is_valid_position_
						|| (position_ != bound_->position_))
				{
					bound_->createCombination(position_);
				}
			}

			protected:
			ContainerPointer	bound_;
			EnumeratorIndex		position_;
			bool							past_the_end_;
		};

		// the default mutation method (calling asignment operator)
		static inline void defaultAssign_(Variant& a, const Variant& b)
		{
			a = b;
		}

		void mutate_(SiteIterator& it, const Variant& v)
		{
			mutator_(*it, v);
		}

		Container&			container_;
		MutatorFunction mutator_;
		SiteList				variant_sites_;
		EnumeratorIndex	position_;
		bool						is_valid_position_;
	};

	template <typename Container, typename SiteIterator, typename Variant>
	Enumerator<Container, SiteIterator, Variant>::Enumerator()
		: container_(const_cast<Container&>(RTTI::getDefault<Container>())),
			mutator_(0)
	{
	}
		
	template <typename Container, typename SiteIterator, typename Variant>
	Enumerator<Container, SiteIterator, Variant>::Enumerator(Container& container)
		: container_(container),
			mutator_(defaultAssign_)
	{
	}
		
	template <typename Container, typename SiteIterator, typename Variant>
	BALL_INLINE
	Enumerator<Container, SiteIterator, Variant>::Enumerator
		(Container& container, typename Enumerator<Container, SiteIterator, Variant>::MutatorFunction mutator)
		: container_(container),
			mutator_(mutator)
	{
	}


	template <typename Container, typename SiteIterator, typename Variant>
	BALL_INLINE
	Container& Enumerator<Container, SiteIterator, Variant>::getCurrent()
	{
		return container_;
	}
	
	template <typename Container, typename SiteIterator, typename Variant>
	void Enumerator<Container, SiteIterator, Variant>::createCombination(const Position index)
	{
		try
		{
			position_ = index;
			createCombination(position_);
		}
		catch (EnumeratorIndex::IncompatibleIndex&)
		{
			throw Exception::IndexOverflow(__FILE__, __LINE__, index);
		}
	}

	template <typename Container, typename SiteIterator, typename Variant>
	void Enumerator<Container, SiteIterator, Variant>::createCombination(const EnumeratorIndex& index)
	{
		if (&index != &position_)
		{
			position_ = index;
		}

		typename SiteList::iterator it = variant_sites_.begin();
		Position i((Position)(index.getSize() - 1));
		for (; it != variant_sites_.end(); ++it, --i)
		{
			mutate_(it->first, it->second[index[i]]);
		}
		
		is_valid_position_ = true;
	}

	template <typename Container, typename SiteIterator, typename Variant>
	BALL_INLINE
	typename Enumerator<Container, SiteIterator, Variant>::Iterator Enumerator<Container, SiteIterator, Variant>::begin()
	{
		return Iterator::begin(*this);
	}

	template <typename Container, typename SiteIterator, typename Variant>
	BALL_INLINE
	typename Enumerator<Container, SiteIterator, Variant>::Iterator Enumerator<Container, SiteIterator, Variant>::end()
	{
		return Iterator::end(*this);
	}

	template <typename Container, typename VariantConstIterator, typename Variant>
	BALL_INLINE
	typename Enumerator<Container, VariantConstIterator, Variant>::ConstIterator Enumerator<Container, VariantConstIterator, Variant>::begin() const
	{
		return ConstIterator::begin(*this);
	}

	template <typename Container, typename VariantConstIterator, typename Variant>
	BALL_INLINE
	typename Enumerator<Container, VariantConstIterator, Variant>::ConstIterator Enumerator<Container, VariantConstIterator, Variant>::end() const
	{
		return ConstIterator::end(*this);
	}

	
#	ifndef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/CONCEPT/enumerator.iC>
#	endif

}

#endif // BALL_CONCEPT_ENUMERATOR_H
