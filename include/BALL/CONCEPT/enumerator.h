// $Id: enumerator.h,v 1.2 2000/06/15 11:48:35 oliver Exp $

#ifndef BALL_CONCEPT_ENUMERATOR_H
#define BALL_CONCEPT_ENUMERATOR_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_COMMON_EXCEPTION_H
# include <BALL/COMMON/exception.h>
#endif

#ifndef BALL_CONCEPT_FORWARDITERATOR_H
#	include <BALL/CONCEPT/forwardIterator.h>
#endif

#include <vector>

using namespace std;

namespace BALL
{

	/** Enumerator Class.
			The Enumerator class provides means for enumerating all possible
			permutations of the variants given in a list (@see BAUSTELLE). Each
			permutation thus can be denoted by an integer.
			{\bf Definition:} \URL{BALL/CONCEPT/enumerator.h}
	 */
	
	class EnumeratorIndex
		: public vector<Position>
	{
		public:

		/** @name Nested Classes
		 */
		//@{
		/** Exception for reporting incompatible EnumeratorIndex instances, i.
		 * e. instances with different moduli. {\bf Definition:}
		 * \URL{BALL/CONCEPT/enumerator.h}
		 */
		 
		class IncompatibleIndex
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

		/** Detailled Constructor
		 *  @param variant_list the list of variants to be applied
		 */
		template <typename Variant, typename VariantIterator>
		EnumeratorIndex(const list<pair<VariantIterator, vector<Variant> > >& variant_list)
			: vector<Position>(variant_list.size()),
				modulus_(variant_list.size()),
				base_multipliers_(variant_list.size())
		{ 
			// compute the base multipliers for later usage 
			Index i;
			Size multiplier = 1;
			list<pair<VariantIterator, vector<Variant> > >::const_iterator list_it = variant_list.begin();
			for (i = size() - 1; i >= 0; i--, list_it++)
			{
				operator[](i) = 0;
				modulus_[i] = list_it->second.size();

				base_multipliers_[i] = multiplier;
				multiplier *= modulus_[i];
			}
		}

		/** Destructor
		 */
		~EnumeratorIndex();
		//@}

		
		/** @name Accessors
		 */
		//@{

		/** access the modulus part of the enumerator
		 */
		const vector<Size>& getModulus() const;

		/** increment an instance of EnumeratorIndex. Increment the least
		 * significant component and apply any overflow to more signficant
		 * components.
		 */
		EnumeratorIndex& operator ++ ();

		/** decrement an instance of EnumeratorIndex. Decrement the least
		 * significant component and apply any underflow to more signficant
		 * components.
		 */
		EnumeratorIndex& operator -- ();

		/** Set an instance of EnumeratorIndex to the value corresponding to
		 * the number {\tt index}.
		 * @param index the number of the permutation to which the instance
		 * should be set
		 */
		void set(Position index);

		/** Assign from a number. 
		 * @param index the number to assign from
		 * @see EnumeratorIndex::set
		 */
		EnumeratorIndex& operator = (Position index)
		{
			set(index);
			return *this;
		}

		//@}

		private:
		/* The EnumeratorIndex class is derived from vector and additionally
		 * contains two vectors of the same size which hold the modulus and the
		 * base for each cell. The values of the EnumeratorIndex vector itself
		 * are interpreted as an inhomogenous number consisting of figures
		 * that have different bases. The bases are the numbers of
		 * possibilities for each variant in the list. Most significant
		 * component is operator[](0), so incrementing starts with
		 * operator[](size())
		 */
			vector<Size>		 modulus_;
			vector<Size>		 base_multipliers_;
	};

	/** @name Predicates for EnumeratorIndex class
	 */
	//@{
	/** Equality of two instances of EnumeratorIndex.
	 * @param x
	 * @param y
	 * @return true, if the instances are equal, false ow.
	 */
	bool operator == (const EnumeratorIndex& x, const EnumeratorIndex& y);

	/** Inequality of two instances of EnumeratorIndex.
	 * @param x
	 * @param y
	 * @return true, if the instances are not equal, false ow.
	 */
	bool operator != (const EnumeratorIndex& x, const EnumeratorIndex& y);

	/** Test whether instance {\tt x} is less than {\tt y}
	 * @param x
	 * @param y
	 * @return true, if {\tt x} is less than {\tt y}
	 */
	bool operator < (const EnumeratorIndex& x, const EnumeratorIndex& y);

	/** Test whether instance {\tt x} is greater than {\tt y}
	 * @param x
	 * @param y
	 * @return true, if {\tt x} is greater than {\tt y}
	 */
	bool operator > (const EnumeratorIndex& x, const EnumeratorIndex& y);

	/** Test whether instance {\tt x} is less than or equal to {\tt y}
	 * @param x
	 * @param y
	 * @return true, if {\tt x} is less than or equal to {\tt y}
	 */
	bool operator <= (const EnumeratorIndex& x, const EnumeratorIndex& y);
	
	/** Test whether an instance {\tt x} is greater than or equal to {\tt y}
	 * @param x
	 * @param y
	 * @return true, if {\tt x} is greater than or equal to {\tt y}
	 */
	bool operator >= (const EnumeratorIndex& x, const EnumeratorIndex& y);

	//@}

	template <class Container, class VariantIterator, class Variant>
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
		typedef vector<Variant>							
			VariantVector;

		/**
		*/
		typedef pair<VariantIterator, VariantVector>
			Site;

		/**
		*/
		typedef list<Site>
			SiteList;

		/** Mutable forward iterator
		*/
		typedef ForwardIterator<Enumerator<Container, VariantIterator, Variant>, EnumeratorIndex, EnumeratorIndex*, IteratorTraits_>
			Iterator;

		/** Constant forward iterator
		*/
		typedef ConstForwardIterator<Enumerator<Container, VariantIterator, Variant>, EnumeratorIndex, EnumeratorIndex*, IteratorTraits_>
			ConstIterator;

		//@}

		/** @name Constructors and Destructors
		*/
		//@{
		/** Default Constructor
		*/
		Enumerator()
		{
		}
		
		/** Detailed Constructor
		 * @param container a Container class to be mutated
		 * @param mutator the function defining the mutations to be applied
		 */
		Enumerator(Container& container, MutatorFunction mutator)
			:	container_(container),
				mutator_(mutator)
		{
		}

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
		void addVariants(const VariantIterator& it, const VariantVector& variants)
		{
			variant_sites_.push_back(Site(it, variants));
		}

		/** Delete variants from the list of variants
		 */
		void deleteVariants(const VariantIterator& it, const VariantVector& variants)
		{
			// BAUSTELLE
		}

		/** Count all variants.
		 * @return the number of all possible variants
		 */
		Size countVariants()
		{
			Size total = 1;
			SiteList::iterator it;
			for (it = variant_sites_.begin(); it != variant_sites_.end(); ++it)
			{
				total *= it->second.size();
			}
			return total;
		}

		/** Access the current content
		 * @return a reference to the container class of this enumerator
		 */
		Container& getCurrent()
		{
			return container_;
		}

		/** Create a permuatation denoted by its number.
		 * @param index the number of the permutation to be created
		 */
		void createPermutation(const Position index)
		{
			EnumeratorIndex enumerator_index(variant_sites_);
			enumerator_index = index;
			createPermutation(enumerator_index);
		}

		/** Create a permutation denoted by an instance of EnumeratorIndex.
		 * @param index the instance of EnumeratorIndex that describes the
		 * permutation to be created
		 */
		void createPermutation(const EnumeratorIndex& index)
		{
			if (index.size() != variant_sites_.size())
			{
				throw EnumeratorIndex::IncompatibleIndex();
			}

			SiteList::iterator it = variant_sites_.begin();
			Position i = index.size() - 1;
			for (; it != variant_sites_.end(); ++it, --i)
			{
				mutate_(it->first, it->second[index[i]]);
			}
		}

		//@}

		protected:

		/**
		*/
		class IteratorTraits_
		{
			friend class Enumerator<Container, VariantIterator, Variant>;
			
			public:
				
			typedef Enumerator<Container, VariantIterator, Variant>	
				ContainerType;

			typedef Enumerator<Container, VariantIterator, Variant>* 
				ContainerPointer;

			typedef EnumeratorIndex														
				IteratorPosition;

			typedef EnumeratorIndex
				ValueType;

			IteratorTraits_()
				:	bound_(0),	
					position_(0)
			{
			}
				
			IteratorTraits_(const ContainerType& enumerator)
				:	bound_(const_cast<ContainerPointer>(&enumerator)),
					position_(0)
			{
			}
			
			
			IteratorTraits_(const IteratorTraits_& traits)
				:	bound_(traits.bound_)
					position_(traits.position_)
			{
			}
			
			IteratorTraits_& operator = (const IteratorTraits_& traits)
      {
        bound_ = traits.bound_;
        position_ = traits.position_;
 
        return *this;
			}

			const ContainerType* getContainer() const	
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
        return (bool)(position_ == traits.position_);
			}

      bool operator != (const IteratorTraits_& traits) const
      {
        return (bool)(position_ != traits.position_);
			}
      bool isValid() const
      {
        return (bound_ != 0);
			}

      void invalidate()
      {
        bound_ = 0;
        position_ = 0;
			}

			void toBegin()
			{
				position_ = 0;
			}

			bool isBegin() const
			{
				return (position_ == 0);
			}
			
			void toEnd()
			{
				position_ = position_.getNumberOfCombinations();
			}
			
			bool isEnd() const
			{
				return (position_ == position_.getNumberOfCombinations());
			}
			
			ValueType& getData()
			{
				return position_;
			}
			
			const ValueType& getData() const
			{
				return position_;
			}
			
			void forward()
			{
				position_++;
			}
			
			protected:
			
			ContainerPointer	bound_;
			IteratorPosition	position_;
		};
	

		void mutate_(VariantIterator& it, Variant& v)
		{
			mutator_(*it, v);
		}

		Container& container_;
		MutatorFunction mutator_;
		SiteList  variant_sites_;
	};
}

#endif // BALL_CONCEPT_ENUMERATOR_H
