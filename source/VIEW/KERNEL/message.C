// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: message.C,v 1.4.2.1 2003/01/07 13:23:35 anker Exp $

#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/COMMON/rtti.h>

using namespace std;

namespace BALL
{

	namespace VIEW
	{

		Message::Message()
			throw()
			: connection_object_(0),
				deletable_(false)
		{
		}

		Message::Message(const Message& message)
			throw()
			: connection_object_(message.connection_object_),
				deletable_(message.deletable_)
		{
		}

		Message::~Message()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.error() << "Destructing object " << (void *)this 
										<< " of class " << RTTI::getName<Message>() << endl;
			#endif 
		}

		CompositeMessage::CompositeMessage()
			throw()
			: Message(),
				composite_(0),
				composite_name_()
		{
		}

		CompositeMessage::CompositeMessage(const CompositeMessage& message)
			throw()
			: Message(message),
				composite_(message.composite_),
				composite_name_(message.composite_name_)
		{
		}

		CompositeMessage::~CompositeMessage()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.error() << "Destructing object " << (void *)this 
										<< " of class " << RTTI::getName<CompositeMessage>() << endl;
			#endif 
		}

		NewCompositeMessage::NewCompositeMessage()
			throw()
			: CompositeMessage()
		{
		}

		NewCompositeMessage::NewCompositeMessage(const CompositeMessage& message)
			throw()
			: CompositeMessage(message)
		{
		}

		NewCompositeMessage::~NewCompositeMessage()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.error() << "Destructing object " << (void *)this 
										<< " of class " << RTTI::getName<NewCompositeMessage>() << endl;
			#endif 
		}

		RemovedCompositeMessage::RemovedCompositeMessage()
			throw()
			: CompositeMessage()
		{
		}

		RemovedCompositeMessage::RemovedCompositeMessage(const CompositeMessage& message)
			throw()
			: CompositeMessage(message)
		{
		}

		RemovedCompositeMessage::~RemovedCompositeMessage()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.error() << "Destructing object " << (void *)this 
										<< " of class " << RTTI::getName<RemovedCompositeMessage>() << endl;
			#endif 
		}

		ChangedCompositeMessage::ChangedCompositeMessage()
			throw()
			: CompositeMessage()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.error() << "new ChangedCompositeMessage" << std::endl;		
			#endif
		}

	  ChangedCompositeMessage::ChangedCompositeMessage(const CompositeMessage& message)
			throw()
			: CompositeMessage(message)
		{
		}

		ChangedCompositeMessage::~ChangedCompositeMessage()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.error() << "Destructing object " << (void *)this 
										<< " of class " << RTTI::getName<ChangedCompositeMessage>() << endl;
			#endif 
		}

		SceneMessage::SceneMessage()
			throw()
			: Message(),
				update_only_(false),
				camera_look_at_(),
				camera_view_point_()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.error() << "new SceneMessage" << std::endl;		
			#endif
		}

		SceneMessage::SceneMessage(const SceneMessage& message)
			throw()
			: Message(message),
				update_only_(message.update_only_),
				camera_look_at_(message.camera_look_at_),
				camera_view_point_(message.camera_view_point_)
		{
		}

		SceneMessage::~SceneMessage()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.error() << "Destructing object " << (void *)this 
										<< " of class " << RTTI::getName<SceneMessage>() << endl;
			#endif 
		}

		WindowMessage::WindowMessage()
			throw()
			: Message(),
				status_bar_()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.error() << "new WindowMessage" << std::endl;		
			#endif
		}

		WindowMessage::WindowMessage(const WindowMessage& message)
			throw()
			: Message(message),
				status_bar_(message.status_bar_)
		{
		}

		WindowMessage::~WindowMessage()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.error() << "Destructing object " << (void *)this 
										<< " of class " << RTTI::getName<WindowMessage>() << endl;
			#endif 
		}

		SelectionMessage::SelectionMessage()
			throw()
			: Message(),
				selection_()
		{
		}

		SelectionMessage::SelectionMessage(const SelectionMessage& message)
			throw()
			: Message(message),
				selection_(message.selection_)
		{
		}

		SelectionMessage::~SelectionMessage()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.error() << "Destructing object " << (void *)this 
										<< " of class " << RTTI::getName<SelectionMessage>() << endl;
			#endif 
		}


		GeometricObjectSelectionMessage::GeometricObjectSelectionMessage()
			throw()
			: SelectionMessage()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.error() << "new GeometricObjectSelectionMessage" << std::endl;		
			#endif
		}

		GeometricObjectSelectionMessage::GeometricObjectSelectionMessage(const SelectionMessage& message)
			throw()
			: SelectionMessage(message)
		{
		}

		GeometricObjectSelectionMessage::~GeometricObjectSelectionMessage()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.error() << "Destructing object " << (void *)this 
										<< " of class " << RTTI::getName<GeometricObjectSelectionMessage>() << endl;
			#endif 
		}

		ControlSelectionMessage::ControlSelectionMessage()
			throw()
			: SelectionMessage()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.error() << "new ControlSelectionMessage" << std::endl;
			#endif
		}

		NewSelectionMessage::NewSelectionMessage() 
			throw()
		{ 
			#ifdef BALL_VIEW_DEBUG
			  Log.error() << "new NewSelectionMessage" << std::endl;
			#endif
		}

		CompositeSelectedMessage::CompositeSelectedMessage(Composite* composite, bool selected) 
			throw()
			:	composite_(composite),
				selected_(selected)
		{
			#ifdef BALL_VIEW_DEBUG
			  Log.error() << "CompositeSelectedMessage" << std::endl;		
			#endif
		}


#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/KERNEL/message.iC>
#		endif 

	} // namespace VIEW

} // namespace BALL
