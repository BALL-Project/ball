// $Id: object.h,v 1.4 1999/12/28 18:41:03 oliver Exp $ 

#ifndef BALL_CONCEPT_OBJECT_H
#define BALL_CONCEPT_OBJECT_H

#ifndef BALL_CONCEPT_AUTODELETABLE_H
#	include <BALL/CONCEPT/autoDeletable.h>
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

		BALL_CREATE(Object)


		/**	@name	Construction and Destruction */	
		//@{

		/// Default constructor.
		Object();
 
		/// Copy constructor
		Object(const Object& object, bool deep = true);
 
		/// Destructor
		virtual ~Object();

		///
		virtual void clear();

		///
		virtual void destroy();

		//@}
 
		
		/**	@name	Handle Management */
		//@{
		///
		Handle getHandle() const;
	
		///
		static Handle getNextHandle();
	
		///
		static Handle getNewHandle();
		//@}
	
	

		/**	@name Comparison operators */
		//@{
		///
		virtual bool operator == (const Object& object) const;

		///
		bool operator != (const Object& object) const;

		///
		virtual bool operator < (const Object& object) const;

		///
		bool operator <= (const Object& object) const;

		///
		bool operator >= (const Object& object) const;

		///
		bool operator > (const Object& object) const;

		///
		int compare(const Object& object) const;
		//@}

		/**	@name	I/O operations */
		//@{

		///
		virtual void read(::std::istream& s);

		///
		virtual void write(::std::ostream& s) const;

		///
		friend ::std::istream& operator >> (::std::istream& s, Object& object);

		///
		friend ::std::ostream& operator << (::std::ostream& s, const Object& object);
		//@}
	
		
		/**	@name Debugging and Diagnostics */
		//@{
		///
		virtual bool isValid() const;

		///
		virtual void dump(::std::ostream& s = std::cout, Size depth = 0) const;
		//@}

#		ifdef BALL_SUPPORT_OBJECT_MANAGER

		Object* getPrevious();
		
		const Object* getPrevious() const;
		
		Object* getNext();
		
		const Object* getNext() const;

#		endif // BALL_SUPPORT_OBJECT_MANAGER
		


		private:

		Handle handle_;
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
			static void insert(Object& object);
		
			///
			static void remove(Object& object);
			//@}
		
			/**	@name	Obejct Statistics */
			//@{
	
			/// Returns the number of object insertions
			static Size countInsertions();

			/// Returns the number of pbject removals
			static Size countRemovals();

			/// Returns the number of obejct updates
			static Size countUpdates();

			/// Returns the total number of registered objects
			static Size countObjects();

			/// Returns the total number of registered objects of a certain kind
			template <class U>
			Size countObjects(const U& u)
			{
				Size count = 0;

		    for (ObjectIterator object_it = begin(); object_it.isValid(); ++object_it)
    		  if (RTTI<U>::isKindOf(*object_it))
		        ++count;

		    return count;
			}
			//@}

			/**	@name	Debugging and Diagnostics */
			//@{
			///
			static bool isValid();

			///
			static void dump(::std::ostream& s = std::cout, Size depth = 0);
			//@}

			typedef Object* IteratorPosition;
		
			class IteratorTraits_
			{
				public:

				BALL_CREATE(IteratorTraits_)

				IteratorTraits_()
					:	bound_(0),
						position_(0)
				{
				}
			
				IteratorTraits_(const ObjectManager& objectManager)
					:	bound_((ObjectManager *)&objectManager),
						position_(0)
				{
				}
				
				IteratorTraits_(const IteratorTraits_& traits, bool /* deep */ = true)
					:	bound_(traits.bound_),
						position_(traits.position_)
				{
				}
			
				IteratorTraits_& operator = (const IteratorTraits_& traits)
				{
					bound_ = traits.bound_;
					position_ = traits.position_;
					return *this;
				}

				ObjectManager *getContainer()
				{
					return bound_;
				}

				const ObjectManager *getContainer() const
				{
					return bound_;
				}

				bool isSingular() const
				{
					return (bool)(bound_ == 0);
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

				bool operator !=(const IteratorTraits_& traits) const
				{
					return (bool)(position_ != traits.position_);
				}
			
				bool isValid() const
				{
					return (bool)(bound_ != 0 && position_ != 0);
				}

				void invalidate()
				{
					bound_ = 0;
					position_ = 0;
				}

				void toBegin()
				{
					position_ = bound_->first_;
				}

				bool isBegin() const
				{
					return (bool)(position_ == bound_->first_);
				}

				void toEnd()
				{
					position_ = 0;
				}

				bool isEnd() const
				{
					return (bool)(position_ == 0);
				}

				Object& getData()
				{
					return *position_;
				}

				const Object& getData() const
				{
					return *position_;
				}

				void forward()
				{	
					position_ = position_->getNext();
				}

				void toRBegin()
				{
					position_ = bound_->last_;
				}

				bool isRBegin() const
				{
					return (bool)(position_ == bound_->last_);
				}

				void toREnd()
				{
					position_ = 0;
				}

				bool isREnd() const
				{
					return (bool)(position_ == 0);
				}

				void backward()
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
			{
				ObjectManager object_manager;
				return ObjectIterator::begin(object_manager);
			}

			static ObjectIterator end()
			{
				ObjectManager object_manager;
				return ObjectIterator::end(object_manager);
			}



			typedef ReverseBidirectionalIterator<ObjectManager, Object, Object *, IteratorTraits_> ReverseObjectIterator;

			static ReverseObjectIterator rbegin()
			{
				ObjectManager object_manager;
				return ReverseObjectIterator::begin(object_manager);
			}

			static ReverseObjectIterator rend()
			{
				ObjectManager object_manager;
				return ReverseObjectIterator::end(object_manager);
			}

		protected:
		
		private:

			ObjectManager()
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
