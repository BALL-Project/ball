// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: message.sip,v 1.5 2003/12/11 02:32:53 amoll Exp $
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

	enum CompositeMessageType
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
	CompositeMessage(const Composite& /composite/, CompositeMessageType /type/);
	CompositeMessage(const CompositeMessage& /message/);
	~CompositeMessage();
	void setComposite(const Composite& /composite/);
	Composite* getComposite() const;
	void setCompositeName(const String& /name/);
	const String& getCompositeName() const;
	void setType(CompositeMessageType /type/);
	CompositeMessageType getType() const;
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

	enum SceneMessageType
	{
		UNDEFINED = 0,
		REBUILD_DISPLAY_LISTS,
		REDRAW,
		UPDATE_CAMERA,
		REMOVE_COORDINATE_SYSTEM,
		EXPORT_PNG
	};
	SceneMessage(SceneMessageType) throw();
	SceneMessage(const SceneMessage&) throw();
	~SceneMessage() throw();
	
	void setType(SceneMessageType) throw();
	SceneMessageType getType() const throw();
	void setCamera(const Camera&) throw();
	Camera& getCamera() throw();
	const Camera& getCamera() const throw();
};


class SelectionMessage
	: Message
{
%TypeHeaderCode
	#include <BALL/VIEW/KERNEL/message.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End

	public:

	SelectionMessage() throw();
	SelectionMessage(const SelectionMessage& ) throw();
	~SelectionMessage() throw();
	void setSelection(const PyCompositeList&) throw();
	const PyCompositeList& getSelection() const throw();
};


class ControlSelectionMessage
	: SelectionMessage
{
%TypeHeaderCode
	#include <BALL/VIEW/KERNEL/message.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End

	public:
	ControlSelectionMessage() throw();
};


class NewSelectionMessage
	: Message
{
%TypeHeaderCode
	#include <BALL/VIEW/KERNEL/message.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End

	public:
	NewSelectionMessage() throw();
};


class GeometricObjectSelectionMessage
	: Message
{
%TypeHeaderCode
	#include <BALL/VIEW/KERNEL/message.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End

	public:
	GeometricObjectSelectionMessage() throw();
	~GeometricObjectSelectionMessage() throw();
	void setSelection(const PyGeometricObjectList& ) throw();
	const PyGeometricObjectList& getSelection() const throw();
	void setSelected(bool ) throw();
	bool isSelected() const throw();
};


class RepresentationMessage
	: Message
{
%TypeHeaderCode
	#include <BALL/VIEW/KERNEL/message.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End

	public:

	enum RepresentationMessageType
	{
		UNDEFINED = -1,
		ADD = 0,
		REMOVE,
		UPDATE,
		SELECTED
	};

	RepresentationMessage() throw();
	RepresentationMessage(Representation& , RepresentationMessageType ) throw();
	void setRepresentation(Representation& ) throw();
	Representation* getRepresentation() throw();
	void setType(RepresentationMessageType ) throw();
	RepresentationMessageType getType() const throw();
};


class MolecularTaskMessage
	: Message
{
%TypeHeaderCode
	#include <BALL/VIEW/KERNEL/message.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End

	public:

	enum MolecularTaskMessageType
	{
		UNDEFINED = -1,
		BUILD_BONDS,
		CHECK_RESIDUE,
		ADD_HYDROGENS,
		CREATE_DISTANCE_GRID
	};

	MolecularTaskMessage(MolecularTaskMessageType ) throw();
	void setType(MolecularTaskMessageType) throw();
	MolecularTaskMessageType getType() const throw();
};


class ShowDisplayPropertiesMessage
	:Message
{
%TypeHeaderCode
	#include <BALL/VIEW/KERNEL/message.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End

	public:
		ShowDisplayPropertiesMessage();
};


class CreateRepresentationMessage
	:Message
{
%TypeHeaderCode
	#include <BALL/VIEW/KERNEL/message.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End

	public:
		CreateRepresentationMessage() throw();
		CreateRepresentationMessage(const PyCompositeList&, 
																ModelType , 
																ColoringMethod ) throw();   
		const PyCompositeList& getComposites() const throw();
		ModelType getModelType() const throw();
		ColoringMethod getColoringMethod() const throw();
};
	

class NewTrajectoryMessage
	:CompositeMessage
{
%TypeHeaderCode
	#include <BALL/VIEW/KERNEL/message.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End

	public:
		NewTrajectoryMessage() throw();
		void setTrajectoryFile(TrajectoryFile&) throw();
		TrajectoryFile* getTrajectoryFile() throw();
};


class RegularData3DMessage
	:CompositeMessage
{
%TypeHeaderCode
	#include <BALL/VIEW/KERNEL/message.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End

	public:
	enum RegularData3DMessageType
	{
		UNDEFINED = -1,
		NEW = 100,
		REMOVE,
		UPDATE,
		SELECTED
	};
	
	RegularData3DMessage(RegularData3DMessageType ) throw();
	void setRegularData3D(RegularData3D&) throw();
	RegularData3D* getRegularData3D() throw();
};
