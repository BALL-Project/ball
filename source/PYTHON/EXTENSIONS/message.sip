// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: message.sip,v 1.1 2003/12/10 15:59:01 amoll Exp $
//

class Message
{
%TypeHeaderCode
	#include <BALL/VIEW/KERNEL/message.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End
  public:

	Message();
	Message(const Message& /message/);
	// void setSender(const void* object);
	// const void* getSender() const;
	// void setDeletable(bool flag = true);
	// bool isDeletable() const;
};

class CompositeMessage
 : Message
{
%TypeHeaderCode
	#include <BALL/VIEW/KERNEL/message.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End

	public:

	enum Types
	{
		UNDEFINED = -1,
		NEW_COMPOSITE,
		REMOVED_COMPOSITE,
		CHANGED_COMPOSITE,
		CHANGED_COMPOSITE_AND_UPDATE_MOLECULAR_CONTROL,
		SELECTED_COMPOSITE,
		DESELECTED_COMPOSITE,
		CENTER_CAMERA,
		NEW_MOLECULE
	};

	CompositeMessage();
	CompositeMessage(const Composite& /composite/, Index /type/);
	CompositeMessage(const CompositeMessage& /message/);
	~CompositeMessage();
	void setComposite(const Composite& /composite/);
	Composite* getComposite() const;
	void setCompositeName(const String& /name/);
	const String& getCompositeName() const;
	void setType(Index /type/);
	Index getType() const;
};


class SceneMessage
	: Message
{
%TypeHeaderCode
	#include <BALL/VIEW/KERNEL/message.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End

	public:

	enum Type
	{
		UNDEFINED = 0,
		REBUILD_DISPLAY_LISTS,
		REDRAW,
		UPDATE_CAMERA,
		REMOVE_COORDINATE_SYSTEM,
		EXPORT_PNG
	};
	SceneMessage(Type) throw();
	SceneMessage(const SceneMessage&) throw();
	~SceneMessage() throw();
	
	void setType(Type) throw();
	Type getType() const throw();
//	void setCamera(const Camera&) throw();
//	Camera& getCamera() throw();
//	const Camera& getCamera() const throw();
};

