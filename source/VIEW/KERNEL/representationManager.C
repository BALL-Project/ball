//   // -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/KERNEL/representationManager.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/clippingPlane.h>
#include <BALL/VIEW/KERNEL/threads.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/VIEW/KERNEL/modelInformation.h>
#include <BALL/VIEW/DIALOGS/displayProperties.h>
#include <BALL/FORMAT/INIFile.h>

#include <QtGui/QApplication>

//   #define BALL_VIEW_DEBUG

namespace BALL
{
	namespace VIEW
	{


RepresentationManager::RepresentationManager(MainControl* mc)
	: Object(),
		update_mutex_(),
		thread_(new UpdateRepresentationThread()),
		no_update_(false),
		still_to_notify_(false),
		main_control_(mc)
{
	thread_->setMainControl(mc);
	thread_->start();
}

RepresentationManager::~RepresentationManager()
{
	clear();
	delete thread_;
}

RepresentationManager::RepresentationManager(const RepresentationManager& pm)
	: Object(pm)
{
}


void RepresentationManager::clear()
{
	to_update_.clear();

	if (thread_ && thread_->isRunning())
	{
		thread_->terminate();
		thread_->wait();
	}

	// call clear for all stored representations to clear also their geometric objects
	RepresentationsIterator it = begin();
	for (; it != end(); it++)
	{
		(*it)->clear();
	}
	representations_.clear();

	clipping_planes_.clear();
}


bool RepresentationManager::insert(Representation& representation, bool send_message)
{
	if (has(representation)) return false;
	representations_.push_back(&representation);

	if (!send_message) return true;

	if (main_control_)
	{
		main_control_->notify_(new RepresentationMessage(representation, RepresentationMessage::ADD));
	}

	return true;
}


bool RepresentationManager::has(const Representation& representation) const
{
	RepresentationsConstIterator it = begin();
	for (; it != end(); it++)
	{
		if (*it == &representation) return true;
	}
	return false;
}


bool RepresentationManager::remove(Representation& representation, bool send_message)
{
	RepresentationsIterator it = begin();
	for( ; it != end(); it++)
	{
		if (*it != &representation) continue;
		
		// found it!
		representations_.erase(it);

		if (main_control_ && send_message)
		{
			main_control_->notify_(new RepresentationMessage(representation, RepresentationMessage::REMOVE));
		}

		if (!willBeUpdated(representation)) delete &representation;

		return true;
	}

	return false;
}

void RepresentationManager::dump(std::ostream& s, Size depth) const
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

Representation* RepresentationManager::createRepresentation()
{
	Representation* rp = new Representation;
	if (main_control_)
	{
		rp->setModelInformation(main_control_->getModelInformation());
	}
	insert(*rp, false);
	return rp;
}

const RepresentationManager& RepresentationManager::operator = (const RepresentationManager& pm)
{
	RepresentationsConstIterator it = pm.begin();

	for (; it != pm.end(); it++)
	{
		Representation* rp = new Representation(**it);
		representations_.push_back(rp);
	}

	return *this;
}

bool RepresentationManager::operator == (const RepresentationManager& pm) const
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


RepresentationList RepresentationManager::removedComposite(const Composite& composite, bool update)
{
	// Representations either to be updated or deleted
	RepresentationList changed_representations = getRepresentationsOf(composite);

	RepresentationList removed_representations;

	// iterate over all Representations
	RepresentationsIterator rep_it = changed_representations.begin();
	for (; rep_it != changed_representations.end(); rep_it++)
	{
		Representation& rep = **rep_it;

		// test if a Representation has Composites which are (not) to be removed
		list<const Composite*> composites;


		list<const Composite*>::const_iterator crit = rep.getComposites().begin();

		// special case for representations with composites of two different roots:
		// we have to update them manualy here!
		// otherwise no update will be done for this representations!
		const Composite* root = &(**crit).getRoot();
		bool must_be_updated = false;
		
		for(; crit != rep.getComposites().end(); crit++)
		{
			if (&composite != *crit && !composite.isAncestorOf(**crit))
			{
				composites.push_back(*crit);
			}

			if (&(**crit).getRoot() != root) must_be_updated = true;
		}
	

		rep.setComposites(composites);

		// if we have no more Composites in the Representation, it is to be deleted
		if (rep.getComposites().size() == 0) 
		{
			removed_representations.push_back(&rep);
			continue;
		}
		
		
		// if we have no model processor, remove all GeometricObjects
		if (rep.getModelProcessor() == 0)
		{
			rep.clearGeometricObjects();
		}

		// see above: manual update
		if (must_be_updated)
		{
			rep.update(true);
			continue;
		}

		// call update for the Representation
		if (update) update_(rep);
	}


	// Representations are to be deleted
	rep_it = removed_representations.begin();
	for (; rep_it != removed_representations.end(); rep_it++)
	{
		remove(**rep_it);
	}

	return removed_representations;
}


list<Representation*> RepresentationManager::getRepresentationsOf(const Composite& composite)
{
	list<Representation*> changed_representations;
	RepresentationsIterator rep_it = begin();
	for (; rep_it != end(); rep_it++)
	{
		list<const Composite*>::const_iterator cit = (**rep_it).getComposites().begin();
		for (; cit != (**rep_it).getComposites().end(); ++cit)
		{
			if (&composite == *cit ||
					composite.isRelatedWith(**cit)) 
			{
				changed_representations.push_back(*rep_it);
				break;
			}
		}
	}

	return changed_representations;
}

bool RepresentationManager::removeClippingPlane(ClippingPlane* plane)
{
	vector<ClippingPlane*>::iterator it = clipping_planes_.begin(); 
	for (; it != clipping_planes_.end(); it++)
	{
		if (*it == plane)
		{
			clipping_planes_.erase(it);
			if (main_control_)
			{
				main_control_->sendMessage(*new SyncClippingPlanesMessage());
				main_control_->sendMessage(*new SceneMessage(SceneMessage::REDRAW));
			}
			delete plane;

			return true;
		}
	}

	return false;
}

void RepresentationManager::insertClippingPlane(ClippingPlane* plane)
{
	clipping_planes_.push_back(plane);

	if (main_control_)
	{
		main_control_->sendMessage(*new SyncClippingPlanesMessage());
		main_control_->sendMessage(*new SceneMessage(SceneMessage::REDRAW));
	}
}

void RepresentationManager::rebuildAllRepresentations()
{
	update_mutex_.lock();
	RepresentationsIterator it = begin();
	for (;it != end(); it++)
	{
		to_update_.insert(*it);
	}
	update_mutex_.unlock();
}

void RepresentationManager::storeRepresentations(INIFile& out)
{
	if (!out.hasSection("BALLVIEW_PROJECT"))
	{
		out.appendSection("BALLVIEW_PROJECT");
	}

	Position nr_of_representations = 0;
	RepresentationsConstIterator it = begin();
	RepresentationList reps;
	for (; it != end(); it++)
	{
		// only store representations with composites!
		if (!(**it).getComposites().size() ||
				!main_control_->getModelInformation().
					modelMuteableByDisplayProperties((**it).getModelType())) 
		{
			continue;
		}

		bool ok = true;

		// we can only store reps for one system!
		list<const Composite*>::const_iterator cit = (**it).getComposites().begin();
		const Composite* root = &(**cit).getRoot();
		cit++;
		for (; cit != (**it).getComposites().end(); cit++)
		{
			if ((**cit).getRoot() != *root)
			{
				ok = false;
				break;
			}
		}

		if (!ok) 
		{	
			Log.error() << (String)qApp->tr("Can not store a representation for items of multiple systems.") << std::endl;
			continue;
		}

		Index system_nr = -1;
		CompositeManager& cm = main_control_->getCompositeManager();
		CompositeManager::CompositeIterator cit2 = cm.begin();
		for (Position nr = 0; cit2 != cm.end(); cit2++)
		{
			if (root == *cit2) 
			{ 
				system_nr = nr;
				break;
			}

			nr++;
		}

		if (system_nr == -1) continue;

		out.insertValue("BALLVIEW_PROJECT", 
										String("Representation") + String(nr_of_representations),  
										String(system_nr) + String(";") + (**it).toString());
		nr_of_representations++;
		reps.push_back(*it);
	}

	// create a numerical id for every representation
	HashMap<const Representation*, Position> rep_to_pos_map;
	
	RepresentationList::const_iterator rep_it = reps.begin();
	for (Position i = 0; rep_it != reps.end(); rep_it++)
	{
		rep_to_pos_map[*rep_it] = i;
		i++;
	}

	// write the clipping planes
	for (Position plane_pos = 0; plane_pos < getClippingPlanes().size(); plane_pos++)
	{
		ClippingPlane* const plane = getClippingPlanes()[plane_pos];
		String data_string;

		data_string += vector3ToString(plane->getNormal());
		data_string += " ";
		data_string += vector3ToString(plane->getPoint());

		data_string += String(plane->isActive());
		data_string += " ";

		HashSet<const Representation*>::ConstIterator rit = plane->getRepresentations().begin();
		for (; +rit; ++rit)
		{
			data_string += String(rep_to_pos_map[*rit]);
			data_string += " ";
		}
		
		String pname = "ClippingPlane";
		if (plane->cappingEnabled()) pname = "CappingPlane";

		out.insertValue("BALLVIEW_PROJECT", pname + String(plane_pos), data_string);
	}
}
			

void RepresentationManager::restoreRepresentations(const INIFile& in, const vector<const Composite*>& new_systems)
{
 	DisplayProperties* dp = DisplayProperties::getInstance(0);

	if (dp == 0) return;

	no_update_ = true;

	for (Position p = 0; p < 9999999; p++)
	{
		// stop condition
		if (!in.hasEntry("BALLVIEW_PROJECT", "Representation" + String(p))) break;

		String data_string = in.getValue("BALLVIEW_PROJECT", "Representation" + String(p));

		dp->createRepresentation(data_string, new_systems);
	}

	rebuildAllRepresentations();
	no_update_ = false;

	try
	{
		// create a vector with all Representations to access per numeric id
		vector<const Representation*> representations;

		RepresentationList::const_iterator rit = getRepresentations().begin();

		for (; rit != getRepresentations().end(); rit++)
		{
			representations.push_back(*rit);
		}

		// create clipping planes
		for (Position p = 0; p < 9999999; p++)
		{
			if (!in.hasEntry("BALLVIEW_PROJECT", "ClippingPlane" + String(p)) &&
			    !in.hasEntry("BALLVIEW_PROJECT", "CappingPlane" + String(p))) 
			{
				break;
			}

			bool cap = false;
			String data_string;
			if (in.hasEntry("BALLVIEW_PROJECT", "ClippingPlane" + String(p)))
			{
				data_string = in.getValue("BALLVIEW_PROJECT", "ClippingPlane" + String(p));
			}
			else
			{
				data_string = in.getValue("BALLVIEW_PROJECT", "CappingPlane" + String(p));
				cap = true;
			}

			vector<String> string_vector;
			Size split_size = data_string.split(string_vector);

			// we have a clipping plane
			if (split_size < 3) 
			{
				Log.error() << "Error in "  << __FILE__ << "  " << __LINE__<< std::endl;
				continue;
			}

			ClippingPlane* plane = new ClippingPlane();
			Vector3 v;
			stringToVector3(string_vector[0], v);
			plane->setNormal(v);
			stringToVector3(string_vector[1], v);
			plane->setPoint(v);

			bool is_active = string_vector[2].toBool();
			plane->setActive(is_active);
			plane->setCappingEnabled(cap);

			for (Position rep_pos = 3; rep_pos < split_size; rep_pos++)
			{
				Position rep_nr = string_vector[rep_pos].toUnsignedInt();
				if (rep_nr > getNumberOfRepresentations()) 
				{
					Log.error() << "Error in "  << __FILE__ << "  " << __LINE__<< std::endl;
					continue;
				}

				plane->getRepresentations().insert(representations[rep_nr]);
			}

			insertClippingPlane(plane);

		} // for all clipping planes

	}
	catch(Exception::InvalidFormat& e)
	{
		Log.error() << (String)qApp->tr("Error while reading project file! Aborting...") << std::endl;
		Log.error() << e << std::endl;
		return;
	}
}


void RepresentationManager::focusRepresentation(const Representation& rep)
{
	vector<Vector3> positions;

	Vector3 center;
	list<GeometricObject*>::const_iterator it = rep.getGeometricObjects().begin();
	for (; it != rep.getGeometricObjects().end(); it++)
	{
		const GeometricObject& go = **it;
		go.getVertices(positions);
	}

	// stupid one, but must be:
	list<Vector3> posl;
	posl.resize(positions.size());
	copy(positions.begin(), positions.end(), posl.begin());

	VIEW::focusCamera(posl);
}

bool RepresentationManager::isBeeingRendered(const Representation* rep) const
{
	if (!update_mutex_.tryLock()) return true;
	
	bool result = beeing_rendered_.has((Representation*)rep);

	update_mutex_.unlock();

	return result;
}

bool RepresentationManager::startRendering(Representation* rep)
{
	update_mutex_.lock();
	
	bool ok = !beeing_updated_.has(rep);

	if (ok) beeing_rendered_.insert(rep);
	update_mutex_.unlock();

	return ok;
}

void RepresentationManager::finishedRendering(Representation* rep)
{
	update_mutex_.lock();
	beeing_rendered_.erase(rep);
	update_mutex_.unlock();
}

Representation* RepresentationManager::popRepresentationToUpdate()
{
	update_mutex_.lock();

	if (to_update_.size() == 0 	||
			no_update_)
	{
		update_mutex_.unlock();
		return 0;
 	}

	Representation* rep = 0;
	RepresentationSet::Iterator it = to_update_.begin();
	for (; +it; ++it)
	{
		if (!beeing_rendered_.has(*it))
		{
			rep = *it;
			break;
		}
	}

	if (rep != 0) 
	{
		beeing_updated_.insert(rep);
		to_update_.erase(rep);
	}

	update_mutex_.unlock();

	return rep;
}

void RepresentationManager::update_(Representation& rep)
{
	if (!has(rep)) return;

	if (rep.isHidden()) 
	{
		rep.needs_update_ = true;
		// update of GeometricControl, also if Representation is hidden
		RepresentationMessage* msg = new RepresentationMessage(rep, RepresentationMessage::UPDATE);
		qApp->postEvent(main_control_, new MessageEvent(msg));
		return;
	}

	if (!main_control_->useMultithreading())
	{
		rep.update_();
		main_control_->notify_(new RepresentationMessage(rep, RepresentationMessage::UPDATE));	
		return;
	}

	update_mutex_.lock();
	to_update_.insert(&rep);
	update_mutex_.unlock();
}

void RepresentationManager::finishedUpdate_(Representation* rep)
{
	update_mutex_.lock();

#ifdef BALL_VIEW_DEBUG
	if (!beeing_updated_.has(rep))
	{
		BALLVIEW_DEBUG
	}
#endif

	beeing_updated_.erase(rep);
	bool update_menus = beeing_updated_.size() == 0;
	update_mutex_.unlock();
	still_to_notify_ = false;

	// Representation might have been deleted
	if (has(*rep))
	{
		// no it wasnt, so update all widgets, that this Representation was rebuild
 		main_control_->notify_(new RepresentationMessage(*rep, RepresentationMessage::UPDATE));
		main_control_->notify_(new RepresentationMessage(*rep, RepresentationMessage::UPDATE_PROPERTIES));
	}
	else
	{
		delete rep;
	}

	if (update_menus && !main_control_->compositesAreLocked())
	{
		main_control_->checkMenus();
	}
}

bool RepresentationManager::updateRunning() const
{
 	if (!update_mutex_.tryLock()) return true;

	bool running = to_update_.size() > 0 	|| 
			 beeing_updated_.size() > 0 			|| 
			 beeing_rendered_.size() > 0;

 	update_mutex_.unlock();
	return running;
}


bool RepresentationManager::willBeUpdated(const Representation& rep) const
{
	if (!update_mutex_.tryLock()) return true;

	bool will = to_update_.has((Representation*)&rep);
	update_mutex_.unlock();
	return will;
}


} } // namespaces
