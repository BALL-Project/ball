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
	nextButton()->setEnabled(false);
	step_ ++;
	QWizard::next();
}


void BALLViewDemo::enableNextStep_()
{
//   	label->setText(label->text() + "<blockquote><br><b>You finished this step. Click \"Next\" when you are ready to proceed.</b></blockquote>");
	nextButton()->setEnabled(true);
}


void BALLViewDemo::onNotify(Message *message)
	throw()
{
#ifdef BALL_VIEW_DEBUG
	Log.error() << "BALLViewDemo " << this << " onNotify " << message << std::endl;
#endif

	switch (step_)
	{
		case 0:
		{
			if (RTTI::isKindOf<CompositeMessage>(*message))
			{
				CompositeMessage* msg = RTTI::castTo<CompositeMessage>(*message);
				if (msg->getType() == CompositeMessage::NEW_MOLECULE)
				{
					enableNextStep_();
					return;
				}
			}
		}

		case 1:
		{
			if (RTTI::isKindOf<ControlSelectionMessage>(*message))
			{
				ControlSelectionMessage* msg = RTTI::castTo<ControlSelectionMessage>(*message);
Log.error() << "#~~#   5 " << msg->getSelection().size()            << " "  << __FILE__ << "  " << __LINE__<< std::endl;

				if (msg->getSelection().size() == 1 &&
						RTTI::isKindOf<System>(**msg->getSelection().begin()))
				{
Log.error() << "#~~#   7 "             << " "  << __FILE__ << "  " << __LINE__<< std::endl;
					enableNextStep_();
					return;
				}
			}
		}

	} // switch
}

} } // namespaces
