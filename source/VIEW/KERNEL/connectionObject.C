// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/KERNEL/connectionObject.h>
#include <BALL/VIEW/KERNEL/message.h>

namespace BALL
{
	namespace VIEW
	{
		using std::ostream;

ConnectionObject::ConnectionObject()
: processing_message_queue_(false),
	message_queue_(),
	parent_(0),
	children_connection_objects_()
{
}

ConnectionObject::~ConnectionObject()
{
	#ifdef BALL_VIEW_DEBUG
		Log.info() << "Destructing object " << (void *)this 
							 << " of class ConnectionObject" << std::endl;
	#endif 

	destroy();
}

void ConnectionObject::clear()
{
	// clear the message queue
	list<Message*>::iterator message_iterator;
	for(message_iterator = message_queue_.begin();
			message_iterator != message_queue_.end();
			++message_iterator)
	{
		// order of this if-statement is important
		// only root has the right to delete deletable messages
		if (this == getRoot()
				&& (*message_iterator)->isDeletable())
		{
			delete *message_iterator;
		}
	}

	message_queue_.clear();

	// unregister this from parent if parent exists
	if (parent_)
	{
		parent_->unregisterConnectionObject(*this);
		clearParent_();
	}

	// unregister all children
	list<ConnectionObject*>::iterator list_iterator;

	for(list_iterator = children_connection_objects_.begin();
			list_iterator != children_connection_objects_.end();
			++list_iterator)
	{
		(*list_iterator)->clearParent_();
	}

	children_connection_objects_.clear();
}

void ConnectionObject::destroy()
{
	clear();
}

void ConnectionObject::registerConnectionObject(ConnectionObject &object)
{
	// register only once
	if (isConnectionObjectRegistered(object)) return;

	children_connection_objects_.push_back(&object);
	object.setParent_(*this);
} 

void ConnectionObject::unregisterConnectionObject(ConnectionObject &object)
{
	// search object in list
	// if inserted cut connection between them
	list<ConnectionObject*>::iterator list_iterator;
	list<ConnectionObject*>::iterator to_be_deleted_iterator
		= children_connection_objects_.end();

	for(list_iterator = children_connection_objects_.begin();
			list_iterator != children_connection_objects_.end();
			++list_iterator)
	{
		if (*list_iterator == &object)
		{
			to_be_deleted_iterator = list_iterator;
			(*list_iterator)->clearParent_();
			break;
		}
	}
	
	// if there is something to delete, delete it
	if (to_be_deleted_iterator != children_connection_objects_.end())
	{
		children_connection_objects_.erase(to_be_deleted_iterator);
	}
}

bool ConnectionObject::isConnectionObjectRegistered(const ConnectionObject &object)
{
	// search object in list
	// if already inserted return true else return false
	list<ConnectionObject*>::const_iterator list_iterator;

	for(list_iterator = children_connection_objects_.begin();
			list_iterator != children_connection_objects_.end();
			++list_iterator)
	{
		if (*list_iterator == &object) return true;
	}

	return false;
}

ConnectionObject *ConnectionObject::getRoot()
{
	ConnectionObject *object = this;

	while (object->getParent())
	{
		object = object->getParent();
	}

	return object;
}

void ConnectionObject::onNotify(Message * /* message */)
{
} 

bool ConnectionObject::isValid() const
{
	// check all children if parent will be ´this´
	list<ConnectionObject*>::const_iterator list_iterator;

	for(list_iterator = children_connection_objects_.begin();
			list_iterator != children_connection_objects_.end();
			++list_iterator)
	{
		if (!(*list_iterator)->ConnectionObject::isValid()) return false;
	}
	
	// has parent a connection to this child
	if (parent_)
	{
		return parent_->isConnectionObjectRegistered(*this);
	}

	return true;
}

void ConnectionObject::dump(ostream& s, Size depth) const
{
	BALL_DUMP_STREAM_PREFIX(s);
	
	BALL_DUMP_DEPTH(s, depth);
	BALL_DUMP_HEADER(s, this, this);

	BALL_DUMP_DEPTH(s, depth);
	s << "size of message queue: " << message_queue_.size() << std::endl;

	BALL_DUMP_DEPTH(s, depth);
	s << "parent: " << (void*)parent_ << std::endl;

	BALL_DUMP_DEPTH(s, depth);
	s << "number of registered objects: " << children_connection_objects_.size() << std::endl;
	list<ConnectionObject*>::const_iterator it = children_connection_objects_.begin();
	for (Position p = 0; p < children_connection_objects_.size(); p++)
	{
		s << String(p) + ": " << *it  << "  " << typeid(**it).name() << std::endl;
		(*it)->dump(s, depth +1);
		it++;
	}

	BALL_DUMP_STREAM_SUFFIX(s);
}

void ConnectionObject::notify_(Message *message)
{
	message->setSender(this);
	getRoot()->onNotify_(message);
}

void ConnectionObject::notify_(Message &message)
{
	message.setSender(this);
	getRoot()->onNotify_(&message);
}

void ConnectionObject::onNotify_(Message* message)
{
	// insert Message into queue (last position)
	message_queue_.push_back(message);

	// if already processing message queue => exit
	if (processing_message_queue_) return;

	processing_message_queue_ = true;

	// process messages in queue
	while (message_queue_.size())
	{
		// get first message
		Message *current_message = *(message_queue_.begin());

		// delete first message from queue
		message_queue_.pop_front();
		
		// process message, but not if sender = this
		if (current_message->getSender() != this)
		{
			onNotify(current_message);
		}
		
		// notify all children
		list<ConnectionObject *>::iterator list_iterator;

		for(list_iterator = children_connection_objects_.begin();
				list_iterator != children_connection_objects_.end();
				++list_iterator)
		{
			(*list_iterator)->onNotify_(current_message);
		}

		// delete message if it is deletable and this == root (only root has the right to delete messages)
		if (this == getRoot() &&
				current_message->isDeletable())
		{
			delete current_message;
		}
	}

	processing_message_queue_ = false;
}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/KERNEL/connectionObject.iC>
#		endif

} } // namespace
