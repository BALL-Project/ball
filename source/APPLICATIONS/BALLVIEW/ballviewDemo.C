// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id:

#include "ballviewDemo.h"
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/common.h>
#include <BALL/KERNEL/system.h>
#include <BALL/VIEW/DIALOGS/displayProperties.h>
#include <BALL/FORMAT/PDBFile.h>

#include <qlabel.h>
#include <qpushbutton.h>
#include <qwidgetstack.h>

namespace BALL
{
	namespace VIEW
	{

BALLViewDemo::BALLViewDemo(QWidget* parent, const char* name)
	throw()
	:	BALLViewDemoData(parent, name),
		ModularWidget(name)
{
#ifdef BALL_VIEW_DEBUG
	Log.error() << "new BALLViewDemo " << this << std::endl;
#endif
	// register the widget with the MainControl
 	ModularWidget::registerWidget(this);
	hide();
}

BALLViewDemo::~BALLViewDemo()
	throw()
{
#ifdef BALL_VIEW_DEBUG
	Log.error() << "deleting BALLViewDemo " << this << std::endl;
#endif
}

void BALLViewDemo::show()
{
	DisplayProperties* dp = DisplayProperties::getInstance(0);
	dp->enableCreationForNewMolecules(false);
	System* system = new System();

	PDBFile file;
	try
	{
//   		file.open("bpti.pdb");
		file.open("AlaAla.pdb");
		file >> *system;
		getMainControl()->insert(*system, "demo");
	}
	catch(...)
	{
		Log.error() << "Could not open bpti.pdb" << std::endl;
	}

	composites_.clear();
	composites_.push_back(system);
	QDialog::show();
	raise();
}

void BALLViewDemo::onNotify(Message *message)
	throw()
{
#ifdef BALL_VIEW_DEBUG
	Log.error() << "BALLViewDemo " << this << " onNotify " << message << std::endl;
#endif

	if (!isVisible()) return;

//   	CompositeMessage* cmsg = RTTI::castTo<CompositeMessage>(*message);
	RepresentationMessage* rmsg = RTTI::castTo<RepresentationMessage>(*message);

	Index id = widget_stack->id(widget_stack->visibleWidget());

	if (id <= MODEL_HBONDS &&
		  rmsg != 0 && 
			rmsg->getType() == RepresentationMessage::UPDATE)
	{
		nextStep_();
	}
}

void BALLViewDemo::nextStep_()
{
	buttonOk->setEnabled(true);
	widget_stack->raiseWidget(widget_stack->id(widget_stack->visibleWidget()) + 1);
}

void BALLViewDemo::accept()
{
	Index id = widget_stack->id(widget_stack->visibleWidget());

	if (id <= MODEL_HBONDS)
	{
		// remove representations
		PrimitiveManager& pm = getMainControl()->getPrimitiveManager();
		Size nr = pm.getNumberOfRepresentations();
		list<Representation*> reps = pm.getRepresentations();
		for (Position p = 0; p < nr; p++)
		{
			getMainControl()->remove(**reps.begin());
			reps.pop_front();
		}

		CreateRepresentationMessage* crmsg = new CreateRepresentationMessage(composites_, (ModelType) id, COLORING_ELEMENT);
		notify_(crmsg);
	}

	buttonOk->setEnabled(false);
}


} } // namespaces
