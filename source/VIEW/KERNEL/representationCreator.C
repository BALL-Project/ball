// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/KERNEL/representationCreator.h>

#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/VIEW/KERNEL/mainControl.h>

#include <BALL/CONCEPT/composite.h>
#include <BALL/COMMON/rtti.h>

#include <list>

namespace BALL
{
	namespace VIEW
	{

RepresentationCreator::RepresentationCreator(QObject* parent, const char* name)
	:	QObject(parent),
		ModularWidget(name),
		create_representations_for_new_molecules_(true)
{
#ifdef BALL_VIEW_DEBUG
	Log.error() << "new RepresentationCreator " << this << std::endl;
#endif
	setObjectName(name);
	ModularWidget::registerWidget(this);
}

RepresentationCreator::~RepresentationCreator()
{
#ifdef BALL_VIEW_DEBUG
	Log.error() << "deleting RepresentationCreator " << this << std::endl;
#endif
}

void RepresentationCreator::onNotify(Message* message)
{
#ifdef BALL_VIEW_DEBUG
	Log.error() << "RepresentationCreator::onNotify " << message << std::endl;
#endif
	// new molecule => build the default graphical representation and notify scene.
	//
	// This is the auto-create behaviour re-homed off DisplayProperties (Phase
	// 999.67 Plan 03). A DEFAULT-constructed RepresentationSpec is exactly the
	// legacy NEW_MOLECULE default representation (representationBuilder.h:62);
	// RepresentationBuilder::createRepresentation is the headless engine 999.65
	// already wired in. The Notifier_ adapter forwards the builder's
	// ADD_TO_GEOMETRIC_CONTROL / CENTER_CAMERA messages through our own
	// protected notify_().
	if (RTTI::isKindOf<CompositeMessage>(message))
	{
		CompositeMessage* composite_message = RTTI::castTo<CompositeMessage>(*message);
		if (!create_representations_for_new_molecules_ ||
				composite_message->getType() != CompositeMessage::NEW_MOLECULE)
		{
			return;
		}

		MainControl* mc = getMainControl();
		if (mc == 0) return;

		std::list<Composite*> clist;
		clist.push_back(composite_message->getComposite());

		RepresentationSpec spec;
		Notifier_ notifier(*this);
		RepresentationBuilder::createRepresentation(spec, clist, *mc, notifier,
		                                            mc->getModelInformation());
		return;
	}
}

	} // namespace VIEW
} // namespace BALL
