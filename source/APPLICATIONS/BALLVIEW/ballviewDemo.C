// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id:

#include "ballviewDemo.h"
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/common.h>

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

	hide();
}

BALLViewDemo::~BALLViewDemo()
	throw()
{
#ifdef BALL_VIEW_DEBUG
	Log.error() << "deleting BALLViewDemo " << this << std::endl;
#endif
}

void BALLViewDemo::onNotify(Message *message)
	throw()
{
#ifdef BALL_VIEW_DEBUG
	Log.error() << "BALLViewDemo " << this << " onNotify " << message << std::endl;
#endif

	if (step_ == 0)
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
}

void BALLViewDemo::show()
{
	BALLViewDemoData::show();
	raise();
}

void BALLViewDemo::nextClicked()
{

}

void BALLViewDemo::enableNextStep_()
{
	step_ ++;
	label->setText(label->text() + "<blockquote><br><b>You finished this step. Click \"Next\" when you are ready to proceed.</b></blockquote>");
	next_button->setEnabled(true);
}


} } // namespaces
