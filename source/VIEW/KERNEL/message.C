// $Id: message.C,v 1.4 2001/05/13 14:28:38 hekl Exp $

#include <BALL/VIEW/KERNEL/message.h>

using namespace std;

namespace BALL
{

	namespace VIEW
	{

		Message::Message()
			throw()
			:
			connection_object_(0),
		  deletable_(false)
		{
		}

		Message::Message(const Message& message)
			throw()
			:
			connection_object_(message.connection_object_),
		  deletable_(message.deletable_)
		{
		}

		Message::~Message()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<Message>() << endl;
			#endif 
		}

		CompositeMessage::CompositeMessage()
			throw()
			:
			Message(),
			composite_(0),
			composite_name_()
		{
		}

		CompositeMessage::CompositeMessage(const CompositeMessage& message)
			throw()
			:
			Message(message),
			composite_(message.composite_),
			composite_name_(message.composite_name_)
		{
		}

		CompositeMessage::~CompositeMessage()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<CompositeMessage>() << endl;
			#endif 
		}

		NewCompositeMessage::NewCompositeMessage()
			throw()
			:
			CompositeMessage()
		{
		}

		NewCompositeMessage::NewCompositeMessage(const CompositeMessage& message)
			throw()
			:
			CompositeMessage(message)
		{
		}

		NewCompositeMessage::~NewCompositeMessage()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<NewCompositeMessage>() << endl;
			#endif 
		}

		RemovedCompositeMessage::RemovedCompositeMessage()
			throw()
			:
			CompositeMessage()
		{
		}

		RemovedCompositeMessage::RemovedCompositeMessage(const CompositeMessage& message)
			throw()
			:
			CompositeMessage(message)
		{
		}

		RemovedCompositeMessage::~RemovedCompositeMessage()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<RemovedCompositeMessage>() << endl;
			#endif 
		}

		ChangedCompositeMessage::ChangedCompositeMessage()
			throw()
			:
			CompositeMessage()
		{
		}

	  ChangedCompositeMessage::ChangedCompositeMessage(const CompositeMessage& message)
			throw()
			:
			CompositeMessage(message)
		{
		}

		ChangedCompositeMessage::~ChangedCompositeMessage()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<ChangedompositeMessage>() << endl;
			#endif 
		}
		/*
		DisplayMessage::DisplayMessage()
			throw()
			:
			CompositeMessage()
		{
		}

		DisplayMessage::DisplayMessage(const DisplayMessage& message)
			throw()
			:
			CompositeMessage(message)
		{
		}

		DisplayMessage::~DisplayMessage()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<DisplayMessage>() << endl;
			#endif 
		}
		*/
		SceneMessage::SceneMessage()
			throw()
			:
			Message(),
			update_only_(false),
			camera_look_at_(),
			camera_view_point_()
		{
		}

		SceneMessage::SceneMessage(const SceneMessage& message)
			throw()
			:
			Message(message),
			update_only_(message.update_only_),
			camera_look_at_(message.camera_look_at_),
			camera_view_point_(message.camera_view_point_)
		{
		}

		SceneMessage::~SceneMessage()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<SceneMessage>() << endl;
			#endif 
		}

		WindowMessage::WindowMessage()
			throw()
			:
			Message(),
			status_bar_()
		{
		}

		WindowMessage::WindowMessage(const WindowMessage& message)
			throw()
			:
			Message(message),
			status_bar_(message.status_bar_)
		{
		}

		WindowMessage::~WindowMessage()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<WindowMessage>() << endl;
			#endif 
		}

		SelectionMessage::SelectionMessage()
			throw()
			:
			Message(),
			selection_()
		{
		}

		SelectionMessage::SelectionMessage(const SelectionMessage& message)
			throw()
			:
			Message(message),
			selection_(message.selection_)
		{
		}

		SelectionMessage::~SelectionMessage()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<SelectionMessage>() << endl;
			#endif 
		}


		GeometricObjectSelectionMessage::GeometricObjectSelectionMessage()
			throw()
			:
			SelectionMessage()
		{
		}

		GeometricObjectSelectionMessage::GeometricObjectSelectionMessage(const SelectionMessage& message)
			throw()
			:
			SelectionMessage(message)
		{
		}

		GeometricObjectSelectionMessage::~GeometricObjectSelectionMessage()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<GeometricObjectSelectionMessage>() << endl;
			#endif 
		}


#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/KERNEL/message.iC>
#		endif 

	} // namespace VIEW

} // namespace BALL
