// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: primitiveManager.C,v 1.22 2004/11/12 17:33:02 amoll Exp $

#include <BALL/VIEW/KERNEL/primitiveManager.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/threads.h>
#include <BALL/VIEW/KERNEL/message.h>

#include <qapplication.h>

namespace BALL
{
	namespace VIEW
	{

#ifdef BALL_QT_HAS_THREADS
	UpdateRepresentationThread PrimitiveManager::thread_;
#endif


PrimitiveManager::PrimitiveManager(MainControl* mc)
	throw()
	: Object(),
		main_control_(mc),
		update_still_to_be_started_(false)
{
}

PrimitiveManager::~PrimitiveManager()
	throw()
{
	clear();
}

PrimitiveManager::PrimitiveManager(const PrimitiveManager& pm)
	throw()
	: Object(pm)
{
	*this = pm;
}

void PrimitiveManager::clear()
	throw()
{
	// call clear for all stored representations to clear also their geometric objects
	RepresentationsIterator it = begin();
	for (; it != end(); it++)
	{
		(*it)->clear();
	}
	representations_.clear();
}


bool PrimitiveManager::insert(Representation& representation)
	throw()
{
	if (has(representation)) return false;
	representations_.push_back(&representation);

	RepresentationMessage* rm = new RepresentationMessage(representation, RepresentationMessage::ADD);
	main_control_->notify_(*rm);

	return true;
}


bool PrimitiveManager::has(const Representation& representation) const
	throw()
{
	RepresentationsConstIterator it = begin();
	for (; it != end(); it++)
	{
		if (*it == &representation) return true;
	}
	return false;
}


bool PrimitiveManager::remove(Representation& representation)
	throw()
{
	bool found = false;
	RepresentationsIterator it = begin();
	for( ; it != end(); it++)
	{
		if (*it == &representation)
		{
			found = true;
			break;
		}
	}

	if (!found) return false;

	representations_.erase(it);

	RepresentationMessage* rm = new RepresentationMessage(representation, RepresentationMessage::REMOVE);
	main_control_->notify_(*rm);

	if (willBeUpdated(representation))
	{
		representations_to_be_deleted_.insert(&representation);
	}
	else
	{
		delete &representation;
	}

	return true;
}

void PrimitiveManager::dump(std::ostream& s, Size depth) const
	throw()
{
	BALL_DUMP_STREAM_PREFIX(s);

	BALL_DUMP_DEPTH(s, depth);
	BALL_DUMP_HEADER(s, this, this);

	BALL_DUMP_DEPTH(s, depth);

	s << "number of representations: " << representations_.size() << std::endl;

	RepresentationsConstIterator it = begin();
	for (; it != end(); it++)
	{
		(*it)->dump(s, depth +1);
		s << std::endl;
	}

	BALL_DUMP_STREAM_SUFFIX(s);     
}

Representation* PrimitiveManager::createRepresentation()
	throw()
{
	Representation* rp = new Representation;
	insert(*rp);
	return rp;
}

const PrimitiveManager& PrimitiveManager::operator = (const PrimitiveManager& pm)
	throw()
{
	RepresentationsConstIterator it = pm.begin();

	for (; it != pm.end(); it++)
	{
		Representation* rp = new Representation(**it);
		representations_.push_back(rp);
	}

	return *this;
}

bool PrimitiveManager::operator == (const PrimitiveManager& pm) const
	throw()
{
	if (pm.getNumberOfRepresentations() != getNumberOfRepresentations()) return false;

	RepresentationsConstIterator it1 = begin();
	RepresentationsConstIterator it2 = pm.begin();
	for (; it1 != end() && it2 != pm.end(); it1++)
	{
		if (**it1 != **it2) return false;
		it2++;
	}

	return true;
}


List<Representation*> PrimitiveManager::removedComposite(const Composite& composite)
	throw()
{
	List<Representation*> removed_representations;
	RepresentationsIterator rep_it = begin();
	for (; rep_it != end(); rep_it++)
	{
		Representation::CompositesConstIterator composite_it = (*rep_it)->begin();
		for(; composite_it != (*rep_it)->end(); composite_it++)
		{
			if (&composite == *composite_it ||
					composite.isAncestorOf(**composite_it))
			{
				removed_representations.push_back(*rep_it);
				break;
			}
		}
	}

	rep_it = removed_representations.begin();
	for (; rep_it != removed_representations.end(); rep_it++)
	{
		remove(**rep_it);
	}
	return removed_representations;
}

List<Representation*> PrimitiveManager::getRepresentationsOf(const Composite& composite)
	throw()
{
	List<Representation*> changed_representations;
	RepresentationsIterator rep_it = begin();
	for (; rep_it != end(); rep_it++)
	{
		Representation::CompositesConstIterator composite_it = (*rep_it)->begin();
		for(; composite_it != (*rep_it)->end(); composite_it++)
		{
			if (&composite == *composite_it ||
					composite.isRelatedWith(**composite_it)) 
			{
				changed_representations.push_back(*rep_it);
				break;
			}
		}
	}

	return changed_representations;
}

void PrimitiveManager::update_(Representation& rep)
	throw()
{
#ifdef BALL_QT_HAS_THREADS
	if (rep.isHidden()) 
	{
		rep.needs_update_ = true;
		// update of GeometricControl, also if Representation is hidden
		RepresentationMessage* msg = new RepresentationMessage(rep, 
																				RepresentationMessage::UPDATE);
		main_control_->notify_(*msg);
		return;
	}

	if (!has(rep)) insert(rep);

	representations_to_be_updated_.push_back(&rep);

	if (!updateRunning()) startUpdateThread_();
#endif
}

void PrimitiveManager::startUpdateThread_()
	throw()
{
#ifdef BALL_QT_HAS_THREADS
	if (!representations_to_be_updated_.size()) return;

	if (updateRunning())
	{
		Log.error() << "Problem while updateing Representations in " 
								<< __FILE__ << " " << __LINE__ << std::endl;
		return;
	}

	Representation* rep = *representations_to_be_updated_.begin();
	if (!has(*rep)) 
	{
 		delete rep;
		representations_to_be_updated_.pop_front();
		startUpdateThread_();
		return;
	}

	update_still_to_be_started_ = false;

	// start the UpdateRepresentationThread
	thread_.setRepresentation(*rep);
	thread_.start();

	// no statusbar changes while beeing otherwise busy
	if (main_control_->compositesAreLocked()) return;

	// keep the user informed: we are still building the Representation -> Statusbar text
	Position pos = 3;
	String dots;
	while (thread_.running())
	{
		main_control_->setStatusbarText("Creating Model " + dots);
		qApp->wakeUpGuiThread();
		qApp->processEvents();
		if (pos < 40) 
		{
			pos ++;
			dots +="..";
		}
		else 
		{
			pos = 3;
			dots = "...";
		}
		thread_.wait(500); 
	}
		
	main_control_->setStatusbarText("");
#endif
}

void PrimitiveManager::finishedUpdate_()
	throw()
{
#ifdef BALL_QT_HAS_THREADS
	if (representations_to_be_updated_.size() == 0)
	{
		return;
	}

	Representation* rep = *representations_to_be_updated_.begin();
	representations_to_be_updated_.pop_front();

	// Representation might have been deleted
	if (has(*rep))
	{
		// no it wasnt, so update all widgets
		RepresentationMessage* msg = new RepresentationMessage(*rep, RepresentationMessage::UPDATE);
		main_control_->notify_(*msg);
	}
	else
	{
		delete rep;
	}

	// shouldnt happen
	if (updateRunning())
	{
		return;
	}

	startUpdateThread_();
#endif
}

bool PrimitiveManager::updateRunning() const
	throw() 
{
#ifdef BALL_QT_HAS_THREADS
	return (thread_.getRepresentation() != 0); 
#else
	return false;
#endif
}


bool PrimitiveManager::willBeUpdated(const Representation& rep) const
	throw()
{
	RepresentationList::ConstIterator it = representations_to_be_updated_.begin();
	for (; it != representations_to_be_updated_.end(); it++)
	{
 		if (*it == &rep) return true;
	}

	return false;
}

bool PrimitiveManager::updatePending() const
	throw()
{
	return update_still_to_be_started_;
}

void PrimitiveManager::willUpdateSoon()
	throw()
{
	update_still_to_be_started_ = true;
}


} } // namespaces
