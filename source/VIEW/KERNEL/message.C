// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: message.C,v 1.20 2004/04/01 12:13:23 amoll Exp $

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
		deletable_(true)
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
}

CompositeMessage::CompositeMessage()
	throw()
	: Message(),
		type_(UNDEFINED),
		composite_(0),
		composite_name_(),
		update_representations_(true)
{
	#ifdef BALL_VIEW_DEBUG
		Log.error() << "new CompositeMessage " << type_ << std::endl;		
	#endif
}

CompositeMessage::CompositeMessage(const Composite& composite, CompositeMessageType type, bool update_representations)
	throw()
	: Message(),
		type_(type),
		composite_((Composite*)&composite),
		composite_name_(),
		update_representations_(update_representations)
{
	#ifdef BALL_VIEW_DEBUG
		Log.error() << "new CompositeMessage " << type_ << std::endl;		
	#endif
}

CompositeMessage::CompositeMessage(const CompositeMessage& message)
	throw()
	: Message(message),
		type_(message.type_),
		composite_(message.composite_),
		composite_name_(message.composite_name_),
		update_representations_(message.update_representations_)
{
	#ifdef BALL_VIEW_DEBUG
		Log.error() << "new CompositeMessage " << type_ << std::endl;		
	#endif
}

CompositeMessage::~CompositeMessage()
	throw()
{
}

SceneMessage::SceneMessage(SceneMessageType type)
	throw()
	: Message(),
		type_(type)
{
	#ifdef BALL_VIEW_DEBUG
		Log.error() << "new SceneMessage" << std::endl;		
	#endif
}

SceneMessage::SceneMessage(const SceneMessage& message)
	throw()
	: Message(message),
		type_(message.type_),
		camera_(message.camera_)
{
	#ifdef BALL_VIEW_DEBUG
		Log.error() << "new SceneMessage" << type_ << std::endl;		
	#endif
}

SceneMessage::~SceneMessage()
	throw()
{
	#ifdef BALL_VIEW_DEBUG
		Log.error() << "Destructing object " << (void *)this 
								<< " of class " << RTTI::getName<SceneMessage>() << endl;
	#endif 
}

void SceneMessage::setType(SceneMessageType type)
	throw() 
{ 
	#ifdef BALL_VIEW_DEBUG
		Log.error() << "SceneMessage::setType " << type << std::endl;
	#endif 

	type_ = type;
}


GenericSelectionMessage::GenericSelectionMessage()
	throw()
	: Message(),
		selection_()
{
}

GenericSelectionMessage::GenericSelectionMessage(const GenericSelectionMessage& message)
	throw()
	: Message(message),
		selection_(message.selection_)
{
}

GenericSelectionMessage::~GenericSelectionMessage()
	throw()
{
}


GeometricObjectSelectionMessage::GeometricObjectSelectionMessage()
	throw()
	: Message()
{
	#ifdef BALL_VIEW_DEBUG
		Log.error() << "new GeometricObjectSelectionMessage" << std::endl;		
	#endif
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
	: GenericSelectionMessage()
{
	#ifdef BALL_VIEW_DEBUG
		Log.error() << "new ControlSelectionMessage" << std::endl;
	#endif
}

NewSelectionMessage::NewSelectionMessage() 
	throw()
	: Message(),
		open_(false)
{ 
	#ifdef BALL_VIEW_DEBUG
		Log.error() << "new NewSelectionMessage" << std::endl;
	#endif
}

RepresentationMessage::RepresentationMessage()
	throw()
	: representation_(0),
		type_(UNDEFINED)
{
	#ifdef BALL_VIEW_DEBUG
		Log.error() << "new RepresentationMessage" << std::endl;		
	#endif
}

RepresentationMessage::RepresentationMessage(Representation& rep, RepresentationMessageType type)
	throw()
	: representation_(&rep),
		type_(type)
{
	#ifdef BALL_VIEW_DEBUG
		Log.error() << "new RepresentationMessage" << std::endl;		
	#endif
}

void RepresentationMessage::setType(RepresentationMessageType type)
	throw() 
{ 
	#ifdef BALL_VIEW_DEBUG
		Log.error() << "RepresentationMessage::setType " << type << std::endl;
	#endif 

	type_ = type;
}

MolecularTaskMessage::MolecularTaskMessage(MolecularTaskMessageType type)
	throw()
	: type_(type)
{
	#ifdef BALL_VIEW_DEBUG
		Log.error() << "new MolecularTaskMessage" << std::endl;
	#endif 
}

void MolecularTaskMessage::setType(MolecularTaskMessageType type)
	throw() 
{
	#ifdef BALL_VIEW_DEBUG
		Log.error() << "MolecularTaskMessage::setType " << type << std::endl;
	#endif 

	type_ = type;
}


NewTrajectoryMessage::NewTrajectoryMessage()
	throw()
	: CompositeMessage(),
		file_(0)
{
	#ifdef BALL_VIEW_DEBUG
		Log.error() << "new NewTrajectoryMessage" << std::endl;		
	#endif
}
	
RegularData3DMessage::RegularData3DMessage(RegularData3DMessageType type) 
	throw()
	: CompositeMessage(),
		data_(0)
{
	#ifdef BALL_VIEW_DEBUG
		Log.error() << "new RegularData3DMessage" << std::endl;		
	#endif
	setType((CompositeMessageType)type);
}


CreateRepresentationMessage::CreateRepresentationMessage()
	throw() 
	: Message(), 
		composites_(),
		model_type_(MODEL_UNKNOWN),
		coloring_method_(COLORING_UNKNOWN)
{
}

CreateRepresentationMessage::CreateRepresentationMessage(const List<Composite*>& composites, 
		ModelType model_type, ColoringMethod coloring_method)
 	throw() 
	: Message(),
		composites_(composites), 
		model_type_(model_type),
		coloring_method_(coloring_method)
{
}

#	ifdef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/VIEW/KERNEL/message.iC>
#	endif 

} } // namespaces
