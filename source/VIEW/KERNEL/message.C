// $Id: message.C,v 1.3 2001/02/04 16:14:27 hekl Exp $

#include <BALL/VIEW/KERNEL/message.h>

using namespace std;

namespace BALL
{

	namespace VIEW
	{

		Message::Message()
			:
			connection_object_(0),
		  deletable_(false)
		{
		}

		Message::Message(const Message& message)
			:
			connection_object_(message.connection_object_),
		  deletable_(message.deletable_)
		{
		}

		Message::~Message()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<Message>() << endl;
			#endif 
		}

		CompositeMessage::CompositeMessage()
			:
			Message(),
			composite_(0),
			composite_name_()
		{
		}

		CompositeMessage::CompositeMessage(const CompositeMessage& message)
			:
			Message(message),
			composite_(message.composite_),
			composite_name_(message.composite_name_)
		{
		}

		CompositeMessage::~CompositeMessage()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<CompositeMessage>() << endl;
			#endif 
		}

		NewCompositeMessage::NewCompositeMessage()
			:
			CompositeMessage()
		{
		}

		NewCompositeMessage::NewCompositeMessage(const CompositeMessage& message)
			:
			CompositeMessage(message)
		{
		}

		NewCompositeMessage::~NewCompositeMessage()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<NewCompositeMessage>() << endl;
			#endif 
		}

		RemovedCompositeMessage::RemovedCompositeMessage()
			:
			CompositeMessage()
		{
		}

		RemovedCompositeMessage::RemovedCompositeMessage(const CompositeMessage& message)
			:
			CompositeMessage(message)
		{
		}

		RemovedCompositeMessage::~RemovedCompositeMessage()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<RemovedCompositeMessage>() << endl;
			#endif 
		}

		ChangedCompositeMessage::ChangedCompositeMessage()
			:
			CompositeMessage()
		{
		}

	  ChangedCompositeMessage::ChangedCompositeMessage(const CompositeMessage& message)
			:
			CompositeMessage(message)
		{
		}

		ChangedCompositeMessage::~ChangedCompositeMessage()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<ChangedompositeMessage>() << endl;
			#endif 
		}
		/*
		DisplayMessage::DisplayMessage()
			:
			CompositeMessage()
		{
		}

		DisplayMessage::DisplayMessage(const DisplayMessage& message)
			:
			CompositeMessage(message)
		{
		}

		DisplayMessage::~DisplayMessage()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<DisplayMessage>() << endl;
			#endif 
		}
		*/
		SceneMessage::SceneMessage()
			:
			Message(),
			update_only_(false),
			camera_look_at_(),
			camera_view_point_()
		{
		}

		SceneMessage::SceneMessage(const SceneMessage& message)
			:
			Message(message),
			update_only_(message.update_only_),
			camera_look_at_(message.camera_look_at_),
			camera_view_point_(message.camera_view_point_)
		{
		}

		SceneMessage::~SceneMessage()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<SceneMessage>() << endl;
			#endif 
		}

		WindowMessage::WindowMessage()
			:
			Message(),
			status_bar_()
		{
		}

		WindowMessage::WindowMessage(const WindowMessage& message)
			:
			Message(message),
			status_bar_(message.status_bar_)
		{
		}

		WindowMessage::~WindowMessage()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<WindowMessage>() << endl;
			#endif 
		}

		SelectionMessage::SelectionMessage()
			:
			Message(),
			selection_()
		{
		}

		SelectionMessage::SelectionMessage(const SelectionMessage& message)
			:
			Message(message),
			selection_(message.selection_)
		{
		}

		SelectionMessage::~SelectionMessage()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<SelectionMessage>() << endl;
			#endif 
		}


		GeometricObjectSelectionMessage::GeometricObjectSelectionMessage()
			:
			SelectionMessage()
		{
		}

		GeometricObjectSelectionMessage::GeometricObjectSelectionMessage(const SelectionMessage& message)
			:
			SelectionMessage(message)
		{
		}

		GeometricObjectSelectionMessage::~GeometricObjectSelectionMessage()
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
