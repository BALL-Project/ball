// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id:

#include "ballviewTutorial.h"
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/common.h>
#include <BALL/VIEW/DIALOGS/displayProperties.h>
#include <BALL/KERNEL/system.h>

#include <qlabel.h>
#include <qpushbutton.h>

namespace BALL
{
	namespace VIEW
	{

BALLViewTutorial::BALLViewTutorial(QWidget* parent, const char* name)
	throw()
	:	BALLViewTutorialData( parent, name ),
		ModularWidget(name)
{
#ifdef BALL_VIEW_DEBUG
	Log.error() << "new BALLViewTutorial " << this << std::endl;
#endif
	// register the widget with the MainControl
	ModularWidget::registerWidget(this);

	for (Index p = 0; p < pageCount(); p++)
	{
		setBackEnabled(page(p), false);
		setHelpEnabled(page(p), false);
	}

	setNextEnabled(page(pageCount() - 1), false);
	setFinishEnabled(page(pageCount() - 1), true);

	hide();
}

BALLViewTutorial::~BALLViewTutorial()
	throw()
{
#ifdef BALL_VIEW_DEBUG
	Log.error() << "deleting BALLViewTutorial " << this << std::endl;
#endif
}

void BALLViewTutorial::show()
{
	showPage(page(0));
	nextButton()->setEnabled(false);
	DisplayProperties::getInstance(0)->selectModel(MODEL_STICK);
	BALLViewTutorialData::show();
	raise();
}

void BALLViewTutorial::next()
{
	QWizard::next();
	nextButton()->setEnabled(false);
}


void BALLViewTutorial::enableNextStep_()
{
	nextButton()->setEnabled(true);
}


void BALLViewTutorial::onNotify(Message *message)
	throw()
{
#ifdef BALL_VIEW_DEBUG
	Log.error() << "BALLViewTutorial " << this << " onNotify " << message << std::endl;
#endif

	if (!isVisible()) return;

	CompositeMessage* cmsg = RTTI::castTo<CompositeMessage>(*message);
	RepresentationMessage* rmsg = RTTI::castTo<RepresentationMessage>(*message);

	QString title = this->title(currentPage());
	if (title == "Building a peptide from a given sequence")
	{
		if (cmsg != 0 && cmsg->getType() == CompositeMessage::NEW_MOLECULE)
		{
			enableNextStep_();
		}
	}

	else if (title == "Hierarchy of molecules")
	{
		if (RTTI::isKindOf<ControlSelectionMessage>(*message))
		{
			ControlSelectionMessage* msg = RTTI::castTo<ControlSelectionMessage>(*message);

			nextButton()->setEnabled(msg->getSelection().size() == 1 &&
															 RTTI::isKindOf<System>(**msg->getSelection().begin()));
		}
	}

	else if (title == "Molecular Dynamics Simulation")
	{
		if (RTTI::isKindOf<NewTrajectoryMessage>(*message))
		{
			enableNextStep_();
		}
	}

	else if (title == "Visualisation of trajectories")
	{
		if (cmsg != 0 && cmsg->getType() == CompositeMessage::CHANGED_COMPOSITE)
		{
			enableNextStep_();
		}
	}

	else if (title == "Calculation of electrostatics")
	{
		RegularData3DMessage* msg = RTTI::castTo<RegularData3DMessage>(*message);
		if (msg != 0)
		{
			enableNextStep_();
		}
	}

	else if (title == "Creating a Solvent Excluded Surface")
	{
		if (rmsg != 0 && 
				rmsg->getType() == RepresentationMessage::ADD_TO_GEOMETRIC_CONTROL &&
				rmsg->getRepresentation()->getModelType() == MODEL_SE_SURFACE)
		{
			enableNextStep_();
		}
	}

	else if (title == "Coloring a SES by electrostatics")
	{
		if (rmsg != 0 && 
				rmsg->getType() == RepresentationMessage::UPDATE &&
				rmsg->getRepresentation()->getModelType() == MODEL_SE_SURFACE)
		{
			enableNextStep_();
		}
	}

	else if (title == "Creating a isocontour surface")
	{
		if (rmsg != 0 && 
				rmsg->getRepresentation()->getModelType() == MODEL_CONTOUR_SURFACE)
		{
			enableNextStep_();
		}
	}
} // onNotify

} } // namespaces
