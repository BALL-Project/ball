// $Id:

#include <BALL/VIEW/KERNEL/connectionObject.h>

using namespace std;

namespace BALL
{
	namespace VIEW
	{

		ConnectionObject::ConnectionObject()
			:
			processing_message_queue_(false),
			message_queue_(),
			parent_(0),
			children_connection_objects_()
		{
		}

		ConnectionObject::~ConnectionObject()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<ConnectionObject>() << endl;
			#endif 

			destroy();
		}

		void ConnectionObject::clear()
		{
			// clear the message queue
			List<Message*>::Iterator message_iterator;
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
			if (parent_ != 0)
			{
				parent_->unregisterConnectionObject(*this);
				clearParent_();
			}

			// unregister all children
			List<ConnectionObject*>::Iterator list_iterator;

			for(list_iterator = children_connection_objects_.begin();
					list_iterator != children_connection_objects_.end();
					++list_iterator)
			{
				(*list_iterator)->clearParent_();
			}

			children_connection_objects_.destroy();
		}

		void ConnectionObject::destroy()
		{
			clear();
		}

  	void ConnectionObject::registerConnectionObject
		  (ConnectionObject &object)
    {
			// register only once
			if (isConnectionObjectRegistered(object))
			{
				return;
			}

			children_connection_objects_.push_back(&object);

			object.setParent_(*this);
    } 

		void ConnectionObject::unregisterConnectionObject
		  (ConnectionObject &object)
    {
			// search object in list
			// if inserted cut connection between them
			List<ConnectionObject*>::Iterator list_iterator;
			List<ConnectionObject*>::Iterator to_be_deleted_iterator 
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

	  bool ConnectionObject::isConnectionObjectRegistered
		  (const ConnectionObject &object)
    {
			// search object in list
			// if already inserted return true
			// else return false
			List<ConnectionObject*>::ConstIterator list_iterator;

			for(list_iterator = children_connection_objects_.begin();
					list_iterator != children_connection_objects_.end();
					++list_iterator)
			{
				if (*list_iterator == &object)
				{
					return true;
				}
			}

			return false;
		}

		ConnectionObject *ConnectionObject::getRoot()
    {
			ConnectionObject *object = this;

			while (object->getParent() != 0)
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
			List<ConnectionObject*>::ConstIterator list_iterator;

			for(list_iterator = children_connection_objects_.begin();
					list_iterator != children_connection_objects_.end();
					++list_iterator)
			{
				if ((*list_iterator)->ConnectionObject::isValid() == false)
				{
					return false;
				}
			}
			
			//has parent a connection to this child
			if (parent_ != 0)
			{
				bool valid = parent_->isConnectionObjectRegistered(*this);

				if (valid == false)
				{
					return false;
				}
			}

			return true;
		}

		void ConnectionObject::dump
			(ostream& s, Size depth) const
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			BALL_DUMP_DEPTH(s, depth);
			s << "size of message queue: " << message_queue_.size() << endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "parent: " << (void*)parent_ << endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "number of registered objects: " << children_connection_objects_.size() << endl;

			BALL_DUMP_STREAM_SUFFIX(s);
		}
				
		void ConnectionObject::read(istream & /* s */)
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		void ConnectionObject::write(ostream & /* s */) const
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		void ConnectionObject::notify_(Message *message)
    {
			ConnectionObject *object = getRoot();

			message->setSender((void *)this);

			object->onNotify_(message);
		}

		void ConnectionObject::notify_(Message &message)
    {
			ConnectionObject *object = getRoot();

			message.setSender((void *)this);

			object->onNotify_(&message);
		}

		void ConnectionObject::onNotify_(Message* message)
    {
			// insert Message into queue (last position)
			message_queue_.push_back(message);

			// if already processing message queue => exit
			if (processing_message_queue_)
			{
				return;
			}

			processing_message_queue_ = true;

			// process messages in queue
			while (message_queue_.size() > 0)
			{
				// get first message
				Message *current_message = *(message_queue_.begin());

				// delete first message from queue
				message_queue_.pop_front();
				
				// process message, but not if sender = this
				if (current_message->getSender() != ((void *)this))
				{
					onNotify(current_message);
				}
				
				// notify all children
				List<ConnectionObject *>::Iterator list_iterator;

				for(list_iterator = children_connection_objects_.begin();
						list_iterator != children_connection_objects_.end();
						++list_iterator)
				{
					(*list_iterator)->onNotify_(current_message);
				}

				// delete message if it is deletable and this == root (only root has the right to delete messages)
				if (this == getRoot()
						&& current_message->isDeletable())
				{
					delete current_message;
				}
			}

			processing_message_queue_ = false;
    }

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/KERNEL/connectionObject.iC>
#		endif

	} // namespace VIEW

} // namespace BALL
