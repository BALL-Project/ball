// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: molecularControl.C,v 1.2 2003/08/26 16:07:03 amoll Exp $

#include <BALL/VIEW/WIDGETS/molecularControl.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/VIEW/DIALOGS/atomProperties.h>
#include <BALL/KERNEL/system.h>
#include <qmenubar.h>

using std::endl;
#define BALL_VIEW_DEBUG
namespace BALL
{
	namespace VIEW
	{

MolecularControl::MolecularControl(QWidget* parent, const char* name)
	throw()
		:	Control(parent, name)
{
}

MolecularControl::~MolecularControl()
	throw()
{
}

void MolecularControl::checkMenu(MainControl& main_control)
	throw()
{
	Control::checkMenu(main_control);

	List<Composite*>::Iterator it = selected_.begin();	
	for (; it != selected_.end(); it++)
	{
		if (!RTTI::isKindOf<System>(**it)) 
		{
			(main_control.menuBar())->setItemEnabled(copy_id_, true);	
			return;
		}
	}

	// copy is only available for top level selections
	(main_control.menuBar())->setItemEnabled(copy_id_, true);	
}


bool MolecularControl::reactToMessages_(Message* message)
	throw()
{
	// react only to NewMolecularMessage, but not to NewCompositeMessage
	if (RTTI::isKindOf<NewMolecularMessage>(*message))
	{
		NewMolecularMessage *composite_message = RTTI::castTo<NewMolecularMessage>(*message);
		addComposite(*(Composite *)composite_message->getComposite());
		return false;
	}

	if (RTTI::isKindOf<NewCompositeMessage>(*message))
	{ 
		return false;
	}

	return Control::reactToMessages_(message);
}


void MolecularControl::buildContextMenu(Composite& composite)
	throw()
{
	Control::buildContextMenu(composite);

	bool composites_muteable = getMainControl()->compositesAreMuteable();
	context_menu_.insertSeparator();

	if (RTTI::isKindOf<AtomContainer>(composite))
	{
		insertContextMenuEntry("Check residue", this, SLOT(checkResidue()), RESIDUE__CHECK);

		bool system_selected = true;
		List<Composite*>::Iterator it = selected_.begin();	
		for (; it != selected_.end(); it++)
		{
			if (!RTTI::isKindOf<System>(**it)) system_selected = false;
			break;
		}

		context_menu_.setItemEnabled(OBJECT__COPY, system_selected);

		insertContextMenuEntry("Build Bonds", this, SLOT(buildBonds()), BONDS__BUILD);
		context_menu_.setItemEnabled(BONDS__BUILD, composites_muteable);
		//insertContextMenuEntry("remove Bonds", this, SLOT(removeBonds()), BONDS__REMOVE);
	}

	insertContextMenuEntry("Focus camera", this, SLOT(centerCamera()), CAMERA__CENTER);

	if (RTTI::isKindOf<Atom>(composite))
	{
		insertContextMenuEntry("Properties", this, SLOT(atomProperties()), ATOM__PROPERTIES);
	}

	context_menu_.insertSeparator();
	insertContextMenuEntry("Show filename", this, SLOT(showFilename()), SHOW__FILENAME);
}


void MolecularControl::atomProperties()
{
	AtomProperties as((Atom*)context_composite_, this);
	as.exec();

	ChangedCompositeMessage* message = new ChangedCompositeMessage;
	message->setComposite(context_composite_);
	message->setUpdateControl(true);
	message->setDeletable();
	notify_(message);
}
	
void MolecularControl::buildBonds()
{
	MolecularTaskMessage* message = new MolecularTaskMessage;
	message->setType(MolecularTaskMessage::BUILD_BONDS);
	message->setDeletable();
	notify_(message);
}
	
void MolecularControl::centerCamera()
{
	CenterCameraMessage* message = new CenterCameraMessage;
	message->setDeletable();
	notify_(message);
}

void MolecularControl::checkResidue()
{
	MolecularTaskMessage* message = new MolecularTaskMessage;
	message->setType(MolecularTaskMessage::CHECK_RESIDUE);
	message->setDeletable();
	notify_(message);
}

void MolecularControl::showFilename()
{
	Composite* c_ptr = context_composite_;
	c_ptr = &context_composite_->getRoot();
	
	if (RTTI::isKindOf<AtomContainer>(*c_ptr))
	{
		setStatusbarText("Composite is from file  " +
											((const AtomContainer*)c_ptr)->getProperty("FROM_FILE").getString());
	}
	else
	{
		setStatusbarText("Composite is not from file");
	}
}


void MolecularControl::updateSelection()
{
	Control::updateSelection();
	if (selected_.size() == 1 && RTTI::isKindOf<System>(**selected_.begin()))
	{
		context_composite_ = *selected_.begin();
		showFilename();
	}
}
#undef BALL_VIEW_DEBUG
	} // namespace VIEW
} // namespace BALL
