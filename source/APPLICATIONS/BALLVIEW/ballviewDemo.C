// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id:

#include "ballviewDemo.h"
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/common.h>
#include <BALL/KERNEL/system.h>

#include <qlabel.h>
#include <qpushbutton.h>

namespace BALL
{
	namespace VIEW
	{

BALLViewDemo::BALLViewDemo(QWidget* parent, const char* name)
	throw()
	:	BALLViewDemoData( parent, name ),
		ModularWidget(name),
		first_selection_(true),
		step_(0)
{
#ifdef BALL_VIEW_DEBUG
	Log.error() << "new BALLViewDemo " << this << std::endl;
#endif
	// register the widget with the MainControl
	ModularWidget::registerWidget(this);

	for (Index p = 0; p < pageCount(); p++)
	{
		setBackEnabled(page(p), false);
		setHelpEnabled(page(p), false);
	}

	setNextEnabled(page(pageCount() - 1), false);
	setFinishEnabled(page(pageCount() - 1), false);

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
	showPage(page(0));
	nextButton()->setEnabled(false);
	BALLViewDemoData::show();
	raise();
}

void BALLViewDemo::next()
{
	step_ ++;
	QWizard::next();
	nextButton()->setEnabled(false);
}


void BALLViewDemo::enableNextStep_()
{
	nextButton()->setEnabled(true);
}


void BALLViewDemo::onNotify(Message *message)
	throw()
{
#ifdef BALL_VIEW_DEBUG
	Log.error() << "BALLViewDemo " << this << " onNotify " << message << std::endl;
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
			if (first_selection_) 
			{
				first_selection_ = false;
				return;
			}

			ControlSelectionMessage* msg = RTTI::castTo<ControlSelectionMessage>(*message);

			nextButton()->setEnabled(msg->getSelection().size() == 1 &&
															 RTTI::isKindOf<System>(**msg->getSelection().begin()));
	}

	else if (title == "Molecular Dynamics Simulation")
	{
Log.error() << "#~~#   9 "             << " "  << __FILE__ << "  " << __LINE__<< std::endl;
		if (RTTI::isKindOf<NewTrajectoryMessage>(*message))
		{
Log.error() << "#~~#   10 "             << " "  << __FILE__ << "  " << __LINE__<< std::endl;
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
				rmsg->getType() == RepresentationMessage::ADD &&
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




	} // switch
}

} } // namespaces
