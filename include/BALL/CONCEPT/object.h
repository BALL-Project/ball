// $Id: object.h,v 1.10 2001/01/19 23:54:18 amoll Exp $ 

#ifndef BALL_CONCEPT_OBJECT_H
#define BALL_CONCEPT_OBJECT_H

#ifndef BALL_CONCEPT_AUTODELETABLE_H
#	include <BALL/CONCEPT/autoDeletable.h>
#endif

#ifndef BALL_COMMON_RTTI_H
#	include <BALL/COMMON/rtti.h>
#endif

#ifndef BALL_CONCEPT_BIDIRECTIONALITERATOR_H
#	include <BALL/CONCEPT/bidirectionalIterator.h>
#endif

#ifndef BALL_CONCEPT_REVERSEBIDIRECTIONALITERATOR_H
#	include <BALL/CONCEPT/reverseBidirectionalIterator.h>
#endif

namespace BALL 
{

	/**
	*/
	class Object
		: public AutoDeletable
	{
#		ifdef BALL_SUPPORT_OBJECT_MANAGER

			friend class ObjectManager;

#		endif // BALL_SUPPORT_OBJECT_MANAGER
		
		public:

		BALL_CREATE_DEEP(Object)


		/**	@name	Construction and Destruction */	
		//@{

		/// Default constructor.
		Object()
			throw();
 
		/// Copy constructor
		Object(const Object& object, bool deep = true)
			throw();
 
		/// Destructor
		virtual ~Object()
			throw();

		///
		virtual void clear()
			throw();

		///
		virtual void destroy()
			throw();

		//@}
 
		
		/**	@name	Handle Management */
		//@{
		///
		Handle getHandle() const
			throw();
	
		///
		static Handle getNextHandle()
			throw();
	
		///
		static Handle getNewHandle()
			throw();
		//@}
	
		/**	@name Comparison operators */
		//@{
		///
		bool operator == (const Object& object) const
			throw();

		///
		bool operator != (const Object& object) const
			throw();

		///
		virtual bool operator < (const Object& object) const
			throw();

		///
		bool operator <= (const Object& object) const
			throw();

		///
		bool operator >= (const Object& object) const
			throw();

		///
		bool operator > (const Object& object) const
			throw();

		///
		int compare(const Object& object) const
			throw();
		//@}

		/**	@name	I/O operations */
		//@{

		///
		virtual void read(::std::istream& s)
			throw();

		///
		virtual void write(::std::ostream& s) const
			throw();

		///
		friend ::std::istream& operator >> (::std::istream& s, Object& object)
			throw();

		///
		friend ::std::ostream& operator << (::std::ostream& s, const Object& object)
			throw();
		//@}
	
		
		/**	@name Debugging and Diagnostics */
		//@{
		///
		virtual bool isValid() const
			throw();

		///
		virtual void dump(::std::ostream& s = std::cout, Size depth = 0) const
			throw();
		//@}

#		ifdef BALL_SUPPORT_OBJECT_MANAGER

		Object* getPrevious()
			throw();
		
		const Object* getPrevious() const
			throw();
		
		Object* getNext()
			throw();
		
		const Object* getNext() const
			throw();

#		endif // BALL_SUPPORT_OBJECT_MANAGER
		


		private:

		Handle				handle_;
		static Handle global_handle_;

#		ifdef BALL_SUPPORT_OBJECT_MANAGER
	
		Object* previous_;
		Object* next_;

#		endif // BALL_SUPPORT_OBJECT_MANAGER
	};

#		ifdef BALL_SUPPORT_OBJECT_MANAGER

	/**
	*/
	class ObjectManager
	{
		public:

			/**	@name	Insertion and Removal of Objects */
			//@{

			///
			static void insert(Object& object)
				throw();
		
			///
			static void remove(Object& object)
				throw();
			//@}
		
			/**	@name	Obejct Statistics */
			//@{
	
			/// Returns the number of object insertions
			static Size countInsertions()
				throw();

			/// Returns the number of pbject removals
			static Size countRemovals()
				throw();

			/// Returns the number of obejct updates
			static Size countUpdates()
				throw();

			/// Returns the total number of registered objects
			static Size countObjects()
				throw();

			/// Returns the total number of registered objects of a certain kind
			template <typename U>
			Size countObjects(const U& u)
				throw()
			{
				Size count = 0;

		    for (ObjectIterator object_it = begin(); object_it.isValid(); ++object_it)
				{
    		  if (RTTI::isKindOf<U>(*object_it))
					{
		        ++count;
					}
				}

		    return count;
			}
			//@}

			/**	@name	Debugging and Diagnostics */
			//@{
			///
			static bool isValid()
				throw();

			///
			static void dump(::std::ostream& s = std::cout, Size depth = 0)
				throw();
			//@}

			typedef Object* IteratorPosition;
		
			class IteratorTraits_
			{
				public:

				BALL_CREATE_DEEP(IteratorTraits_)

				IteratorTraits_()
					throw()
					:	bound_(0),
						position_(0)
				{
				}
			
				IteratorTraits_(const ObjectManager& objectManager)
					throw()
					:	bound_((ObjectManager *)&objectManager),
						position_(0)
				{
				}
				
				IteratorTraits_(const IteratorTraits_& traits, bool /* deep */ = true)
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

				ObjectManager* getContainer()
					throw()
				{
					return bound_;
				}

				const ObjectManager* getContainer() const
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

				bool operator !=(const IteratorTraits_& traits) const
					throw()
				{
					return (position_ != traits.position_);
				}
			
				bool isValid() const
					throw()
				{
					return (bound_ != 0 && position_ != 0);
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
					position_ = bound_->first_;
				}

				bool isBegin() const
					throw()
				{
					return (position_ == bound_->first_);
				}

				void toEnd()
					throw()
				{
					position_ = 0;
				}

				bool isEnd() const
					throw()
				{
					return (position_ == 0);
				}

				Object& getData()
					throw()
				{
					return *position_;
				}

				const Object& getData() const
					throw()
				{
					return *position_;
				}

				void forward()
					throw()
				{	
					position_ = position_->getNext();
				}

				void toRBegin()
					throw()
				{
					position_ = bound_->last_;
				}

				bool isRBegin() const
					throw()
				{
					return (position_ == bound_->last_);
				}

				void toREnd()
					throw()
				{
					position_ = 0;
				}

				bool isREnd() const
					throw()
				{
					return (position_ == 0);
				}

				void backward()
					throw()
				{
					position_ = position_->getPrevious();
				}
	


				private:

				ObjectManager *bound_;
				IteratorPosition position_;
			};

			friend class IteratorTraits_;

			typedef BidirectionalIterator<ObjectManager, Object, Object *, IteratorTraits_> ObjectIterator;

			static ObjectIterator begin()
				throw()
			{
				ObjectManager object_manager;
				return ObjectIterator::begin(object_manager);
			}

			static ObjectIterator end()
				throw()
			{
				ObjectManager object_manager;
				return ObjectIterator::end(object_manager);
			}



			typedef ReverseBidirectionalIterator<ObjectManager, Object, Object *, IteratorTraits_> ReverseObjectIterator;

			static ReverseObjectIterator rbegin()
				throw()
			{
				ObjectManager object_manager;
				return ReverseObjectIterator::begin(object_manager);
			}

			static ReverseObjectIterator rend()
				throw()
			{
				ObjectManager object_manager;
				return ReverseObjectIterator::end(object_manager);
			}

		protected:
		
		private:

			ObjectManager()
				throw()
			{
			}

			static Object *first_;
			static Object *last_;
			static Size number_of_insertions_;
			static Size number_of_removals_;
			static Size number_of_objects_;
	};

#	endif // BALL_SUPPORT_OBJECT_MANAGER

#	ifndef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/CONCEPT/object.iC>
#	endif

} // namespace BALL

#endif // BALL_CONCEPT_OBJECT_H
