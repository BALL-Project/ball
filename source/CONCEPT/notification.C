// $Id: notification.C,v 1.5 2000/12/19 12:51:05 amoll Exp $

#include <BALL/CONCEPT/notification.h>

namespace BALL 
{

	using std::ostream;
	using std::endl;

	NotificationTarget_* NotificationTarget_::getNextTarget()
		throw()
	{
		return next_;
	}

	void* NotificationTarget_::getTarget()
		throw()
	{
		return target_;
	}

	NotificationTarget_::NotificationTarget_()
		throw()
		:	next_(0),
			target_(0)
	{
	}

	NotificationTarget_::NotificationTarget_(NotificationTarget_ *notification_target, void *target)
		throw()
		:	next_(notification_target),
			target_(target)
	{
	}

	NotificationTarget_::~NotificationTarget_()
		throw()
	{
	}

	void  NotificationSource_::enable()
		throw()
	{
		enabled_ = true;
	}
		
	void NotificationSource_::disable()
		throw()
	{
		enabled_ = false;
	}

	bool NotificationSource_::isEnabled()
		throw()
	{
		return (enabled_ == true);
	}
		
	bool NotificationSource_::isDisabled()
		throw()
	{
		return (enabled_ == false);
	}
		
	NotificationSource_::NotificationSource_()
		throw()
		:	next_(0),
			first_(0),
			source_(0),
			number_of_targets_(0),
			enabled_(true)
	{
	}

	NotificationSource_::NotificationSource_(NotificationSource_ *notification_source, void *source)
		throw()
		:	next_(notification_source),
			first_(0),
			source_(source),
			number_of_targets_(0),
			enabled_(true)
	{
	}

	NotificationSource_::~NotificationSource_()
		throw()
	{
		destroy();
	}

	void NotificationSource_::destroy()
		throw()
	{
		NotificationTarget_ *next = 0;
		
		for (NotificationTarget_ *notification_target = first_;
				 notification_target != 0; notification_target = next)
		{
			next = notification_target->next_;
			delete notification_target;
		}
		
		number_of_targets_ = 0;
	}

	NotificationSlot_::~NotificationSlot_()
		throw()
	{
	}

	NotificationSlot_::NotificationSlot_()
		throw()
		:	first_(0)
	{
	}

	NotificationManager_::NotificationManager_()
		throw()
		:	number_of_targets_(0),
			number_of_slots_(INITIAL_NUMBER_OF_SLOTS),
			slots_(new NotificationSlot_[INITIAL_NUMBER_OF_SLOTS]),
			enabled_(true)
	{
	}

	NotificationManager_::~NotificationManager_()
		throw()
	{
		destroy();
		delete [] slots_;
	}

	Size NotificationManager_::getSize()
		throw()
	{
		return number_of_targets_;
	}

	void NotificationManager_::insert(void *source, void *target)
		throw()
	{
		if (needResize_() == true)
		{
			resize_();
		}
		
		++number_of_targets_;

		NotificationSource_ *notification_source = 0;
		NotificationSource_** first = &(slots_[hash_(source)].first_);
				
		for (notification_source = *first; notification_source != 0;
				 notification_source = notification_source->next_)
		{
			if (notification_source->source_ == source)
			{
				break;
			}
		}

		if (notification_source == 0)
		{
			*first = notification_source = new NotificationSource_(*first, source);
		}

		++(notification_source->number_of_targets_);

		notification_source->first_ = new NotificationTarget_(notification_source->first_, target);
	}

	void NotificationManager_::remove(void *source)
		throw()
	{
		NotificationSource_** first = &(slots_[hash_(source)].first_);
				
		NotificationSource_ *previous = 0;
		NotificationSource_ *next = 0;
		
		for (NotificationSource_ *notification_source = *first;
				 notification_source != 0; notification_source = next)
		{
			next = notification_source->next_;

			if (notification_source->source_ == source)
			{
				number_of_targets_ -= notification_source->number_of_targets_;
			
				if (previous != 0)
				{
					previous->next_ = notification_source->next_;
				}
				else
				{
					*first = notification_source->next_;
				}

				delete notification_source;
			}
			else 
			{
				previous = notification_source;
			}
		}
	}

	void NotificationManager_::remove(void *source, void *target)
		throw()
	{
		NotificationSource_** first = &(slots_[hash_(source)].first_);
				
		NotificationSource_* previous = 0;
		NotificationSource_* next = 0;

		for (NotificationSource_* notification_source = *first;
				 notification_source != 0; notification_source = next)
		{
			next = notification_source->next_;
			
			if (notification_source->source_ == source)
			{	  
				NotificationTarget_* previous_target = 0;
				NotificationTarget_* next_target = 0;
				
				for (NotificationTarget_ *notification_target = notification_source->first_;
						notification_target != 0; notification_target = next_target)
				{
					next_target = notification_target->next_;
			
					if (notification_target->target_ == target)
					{
						--number_of_targets_;
						--(notification_source->number_of_targets_);
			
						if (previous_target != 0)
						{
							previous_target->next_ = notification_target->next_;
						}
						else
						{
							notification_source->first_ = notification_target->next_;
						}
			
						delete notification_target;
			
						break;
					}
					else
					{
						previous_target = notification_target;
					}
				}
				
				if (notification_source->number_of_targets_ == 0)
				{
					if (previous != 0)
					{
						previous->next_ = notification_source->next_;
					}
					else
					{
						*first = notification_source->next_;
					}
		
					delete notification_source;
				}
			}
			else
			{
				previous = notification_source;
			}
		}
	}

	NotificationSource_ *NotificationManager_::findSource(void *source)
		throw()
	{
		for (NotificationSource_ *notification_source = slots_[hash_(source)].first_;
				 notification_source != 0; notification_source = notification_source->next_)
		{
			if (notification_source->source_ == source)
			{
				return notification_source;
			}
		}
		
		return 0;
	}

	NotificationTarget_* NotificationManager_::findFirstTarget(void *source)
		throw()
	{
		for (NotificationSource_ *notification_source = slots_[hash_(source)].first_;
				 notification_source != 0; notification_source = notification_source->next_)
		{
			if (notification_source->source_ == source)
			{
				return notification_source->first_;
			}
		}

		return 0;
	}

	NotificationTarget_* NotificationManager_::findTarget(void *source, void *target)
		throw()
	{
		for (NotificationTarget_ *notification_target = findFirstTarget(source);
				 notification_target != 0; notification_target = notification_target->next_)
		{
			if (notification_target->target_ == target)
			{
				return notification_target;
			}
		}
				
		return 0;
	}

	NotificationSource_* NotificationManager_::findEnabledSource(void *source)
		throw()
	{
		NotificationSource_ *notification_source = findSource(source);
		
		if (notification_source != 0 && notification_source->enabled_ == true)
		{
			return notification_source;
		}
		else
		{
			return 0;
		}
	}

	NotificationTarget_* NotificationManager_::findEnabledFirstTarget(void *source)
		throw()
	{
		NotificationSource_ *notification_source = findSource(source);
		
		if (notification_source != 0 && notification_source->enabled_ == true)
		{
			return notification_source->first_;
		}
		else
		{
			return 0;
		}
	}

	NotificationTarget_* NotificationManager_::findEnabledTarget(void *source, void *target)
		throw()
	{
		for (NotificationTarget_ *notification_target = findEnabledFirstTarget(source);
				 notification_target != 0; notification_target = notification_target->next_)
		{
			if (notification_target->target_ == target)
			{
				return notification_target;
			}
		}
		
		return 0;
	}

	void NotificationManager_::destroy()
		throw()
	{
		NotificationSource_ *next = 0;
		
		for (Index index = 0; index < (Index)number_of_slots_; ++index)
		{
			for (NotificationSource_ *notification_source = slots_[index].first_;
					 notification_source != 0; notification_source = next)
			{
				next = notification_source->next_;
				delete notification_source;
			}
			
			slots_[index].first_ = 0;
		}
		
		number_of_targets_ = 0;
	}

	void NotificationManager_::enable()
		throw()
	{
		enabled_ = true;
	}

	void NotificationManager_::disable()
		throw()
	{
		enabled_ = false;
	}

	bool NotificationManager_::isEnabled()
		throw()
	{
		return (enabled_ == true);
	}

	bool NotificationManager_::isDisabled()
		throw()
	{
		return (enabled_ == false);
	}
		
	bool NotificationManager_::isEmpty()
		throw()
	{
		return (number_of_targets_ == 0);
	}

	bool NotificationManager_::isInserted(void *source)
		throw()
	{
		return (findSource(source) != 0);
	}

	bool NotificationManager_::isInserted(void *source, void *target)
		throw()
	{
		return (findTarget(source, target) != 0);
	}

	void NotificationManager_::dump(ostream& s)
		throw()
	{
		Size number_of_sources = 0;
		
		for (Size index = 0; index < number_of_slots_; ++index)
		{
			s << "slot[" << index << "] :" << endl;

			for (NotificationSource_ *notification_source = slots_[index].first_;
					 notification_source != 0; notification_source = notification_source->next_)
			{
				s << "  source: " << notification_source->source_ << endl;

				++number_of_sources;

				for (NotificationTarget_ *notification_target = notification_source->first_;
						 notification_target != 0; notification_target = notification_target->next_)
				{
					s << "    target: " << notification_target->target_ << endl;
				}
			}
		}
		
		s << "number of slots: " << number_of_slots_ << endl;
		s << "number of sources: " << number_of_sources << endl;
		s << "number of targets: " << number_of_targets_ << endl;
		s << "growth threshold: " 
		  << BALL_NOTIFICATION_MANAGER_GROWTH_THRESHOLD(number_of_slots_) << endl;
	}

	bool NotificationManager_::needResize_()
		throw()
	{
		return ((number_of_targets_ + 1) >= BALL_NOTIFICATION_MANAGER_GROWTH_THRESHOLD(number_of_slots_));
	}

	void NotificationManager_::resize_()
		throw()
	{
		Size number_of_old_slots = number_of_slots_;
		number_of_slots_ = BALL_NOTIFICATION_MANAGER_GROWTH_THRESHOLD(number_of_slots_);
		
		NotificationSlot_ *old_slots = slots_;
		slots_ = new NotificationSlot_[number_of_slots_];

		Index hash_index;
		NotificationSource_ *next = 0;
		
		for (Index index = 0; index < (Index)number_of_old_slots; ++index)
		{
			for (NotificationSource_ *notification_source = old_slots[index].first_;
					 notification_source != 0; notification_source = next)
			{
				next = notification_source->next_;
				hash_index = hash_(notification_source->source_);
				notification_source->next_ = slots_[hash_index].first_;			
				slots_[hash_index].first_ = notification_source;
			}
		}

		delete [] old_slots;
	}

	Index NotificationManager_::hash_(void* ptr)
		throw()
	{
		return (Index)(long(ptr) % (number_of_slots_ - 1));
	}

	NotificationManager_& NotificationManager()
		throw()
	{
		static NotificationManager_ notification_manager;
		return notification_manager;
	}

	void NotificationManagerEnable()
		throw()
	{
		NotificationManager().enable();
	}

	void NotificationManagerDisable()
		throw()
	{
		NotificationManager().disable();
	}

	bool NotificationManagerIsEnabled()
		throw()
	{
		return NotificationManager().isEnabled();
	}

	bool NotificationManagerIsDisabled()
		throw()
	{
		return NotificationManager().isDisabled();
	}

	void NotificationUnregisterAll()
		throw()
	{
		NotificationManager().destroy();
	}

} // namespace BALL
