//   // -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: primitiveManager.C,v 1.36.2.14 2005/08/17 14:35:36 amoll Exp $

#include <BALL/VIEW/KERNEL/primitiveManager.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/clippingPlane.h>
#include <BALL/VIEW/KERNEL/threads.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/VIEW/DIALOGS/displayProperties.h>
#include <BALL/FORMAT/INIFile.h>

#include <BALL/VIEW/PRIMITIVES/sphere.h>
#include <BALL/VIEW/PRIMITIVES/mesh.h>
#include <BALL/VIEW/PRIMITIVES/disc.h>
#include <BALL/VIEW/PRIMITIVES/box.h>
#include <BALL/VIEW/PRIMITIVES/simpleBox.h>
#include <BALL/VIEW/DATATYPE/vertex2.h>
#include <BALL/VIEW/DATATYPE/vertex1.h>

#include <qapplication.h>

namespace BALL
{
	namespace VIEW
	{

#ifdef BALL_QT_HAS_THREADS
	UpdateRepresentationThread PrimitiveManager::thread_;
	QMutex 										 PrimitiveManager::mutex_;
#endif


PrimitiveManager::PrimitiveManager(MainControl* mc)
	throw()
	: Object(),
		main_control_(mc),
		update_running_(false),
		update_pending_(false),
		multi_threading_mode_(true)
{
#ifndef BALL_QT_HAS_THREADS
	multi_threading_mode_ = false;
#endif
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
}


void PrimitiveManager::clear()
	throw()
{
	representations_to_be_updated_.clear();

#ifdef BALL_QT_HAS_THREADS
	if (thread_.running())
	{
		thread_.terminate();
		thread_.wait();
	}
#endif

	// call clear for all stored representations to clear also their geometric objects
	RepresentationsIterator it = begin();
	for (; it != end(); it++)
	{
		(*it)->clear();
	}
	representations_.clear();
}


bool PrimitiveManager::insert(Representation& representation, bool send_message)
	throw()
{
	if (has(representation)) return false;
	representations_.push_back(&representation);

	if (!send_message) return true;

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


bool PrimitiveManager::remove(Representation& representation, bool send_message)
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

	if (send_message)
	{
		RepresentationMessage* rm = new RepresentationMessage(representation, RepresentationMessage::REMOVE);
		main_control_->notify_(*rm);
	}

	if (!willBeUpdated(representation))
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
	insert(*rp, false);
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


PrimitiveManager::RepresentationList PrimitiveManager::removedComposite(const Composite& composite, bool update)
	throw()
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
		List<const Composite*> composites;

		List<const Composite*>::ConstIterator crit = rep.getComposites().begin();
		for(; crit != rep.getComposites().end(); crit++)
		{
			if (&composite != *crit && !composite.isAncestorOf(**crit))
			{
				composites.push_back(*crit);
			}
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


List<Representation*> PrimitiveManager::getRepresentationsOf(const Composite& composite)
	throw()
{
	List<Representation*> changed_representations;
	RepresentationsIterator rep_it = begin();
	for (; rep_it != end(); rep_it++)
	{
		List<const Composite*>::const_iterator cit = (**rep_it).getComposites().begin();
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

void PrimitiveManager::update_(Representation& rep)
	throw()
{
	if (!has(rep)) 
	{
		return;
	}
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

	if (!multi_threading_mode_)
	{
		rep.update_();
		RepresentationMessage* msg = new RepresentationMessage(rep, 
																	RepresentationMessage::UPDATE);
		main_control_->notify_(*msg);
		return;
	}

	representations_to_be_updated_.push_back(&rep);

	if (!updateRunning()) startUpdateThread_();
#endif
}

void PrimitiveManager::startUpdateThread_()
	throw()
{
#ifdef BALL_VIEW_DEBUG
	Log.error() << "starting Representation Update" << std::endl;
#endif

#ifdef BALL_QT_HAS_THREADS
	if (!representations_to_be_updated_.size()) return;

	if (!mutex_.tryLock()) return;

	update_running_ = true;
	// maybe the Representation to be updated is already deleted?
	Representation* rep = *representations_to_be_updated_.begin();
	if (!has(*rep)) 
	{
 		delete rep;
		representations_to_be_updated_.pop_front();
		mutex_.unlock();
		startUpdateThread_();
		return;
	}

	// start the UpdateRepresentationThread
	thread_.setRepresentation(*rep);

	#if BALL_QT_VERSION >=	0x030200
		thread_.start(QThread::LowPriority);
	#else
		thread_.start();
	#endif
				
	// no statusbar changes while beeing otherwise busy
	if (main_control_->compositesAreLocked()) return;

	thread_.wait(500);
	if (!thread_.running()) return;
	
	// keep the user informed: we are still building the Representation -> Statusbar text
	Position pos = 3;
	String dots;

	while (thread_.running())
	{
		main_control_->setStatusbarText("Creating Model ..." + dots);
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
		BALLVIEW_DEBUG
		return;
	}

	Representation* rep = *representations_to_be_updated_.begin();
	representations_to_be_updated_.pop_front();

	// Representation might have been deleted
	if (has(*rep))
	{
		// no it wasnt, so update all widgets, that this Representation was rebuild
		RepresentationMessage* msg = new RepresentationMessage(*rep, RepresentationMessage::UPDATE);
		main_control_->notify_(*msg);
	}
	else
	{
		delete rep;
	}

	mutex_.unlock();

	if (representations_to_be_updated_.size() == 0)
	{
		#ifdef BALL_VIEW_DEBUG
			Log.error() << "finished all Representations Update" << std::endl;
		#endif

		update_running_ = false;
		update_pending_ = false;
		update_finished_.wakeAll();
		main_control_->setPreferencesEnabled_(true);
		return;
	}

	startUpdateThread_();
#endif
}

bool PrimitiveManager::updateRunning() const
	throw() 
{
	return update_running_;
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

bool PrimitiveManager::usesMultithreading()
	throw() 
{ 
#ifndef BALL_QT_HAS_THREADS
	return false;
#endif
	return multi_threading_mode_;
}
			

HashSet<Representation*>& PrimitiveManager::getRepresentationsBeeingUpdated()
{
	return currently_updateing_;
}

HashSet<Representation*>& PrimitiveManager::getRepresentationsBeeingDrawn()
{
	return currently_drawing_;
}

bool PrimitiveManager::removeClippingPlane(ClippingPlane* plane)
{
	for (vector<ClippingPlane*>::iterator it = clipping_planes_.begin(); 
			 it != clipping_planes_.end(); it++)
	{
		if (*it == plane)
		{
			clipping_planes_.erase(it);
			delete (*it);
			getMainControl()->sendMessage(*new SyncClippingPlanesMessage());
			getMainControl()->sendMessage(*new SceneMessage(SceneMessage::REDRAW));

			return true;
		}
	}

	return false;
}

void PrimitiveManager::insertClippingPlane(ClippingPlane* plane)
{
	clipping_planes_.push_back(plane);

	getMainControl()->sendMessage(*new SyncClippingPlanesMessage());
	getMainControl()->sendMessage(*new SceneMessage(SceneMessage::REDRAW));
}

void PrimitiveManager::storeRepresentations(INIFile& out)
{
	Position nr_of_representations = 0;
	RepresentationsConstIterator it = begin();
	for (; it != end(); it++)
	{
		if ((**it).getComposites().size() == 0)
		{
			continue;
		}

		bool ok = true;

		List<const Composite*>::const_iterator cit = (**it).getComposites().begin();
		const Composite* root = &(**cit).getRoot();
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
			Log.error() << "Error while writing Project File in " << __FILE__ << " " << __LINE__ << std::endl;
			continue;
		}

		Index system_nr = -1;
		CompositeManager& cm = getMainControl()->getCompositeManager();
		CompositeManager::CompositeIterator cit2 = cm.begin();
		for (Position nr = 0; cit2 != cm.end(); cit2++)
		{
			if (root == *cit2) system_nr = nr;

			nr++;
		}

		if (system_nr == -1) continue;

		out.insertValue("BALLVIEW_PROJECT", 
										String("Representation") + String(nr_of_representations),  
										String(system_nr) + String(";") + (**it).toString());
		nr_of_representations++;
	}

	// create a numerical id for every representation
	HashMap<const Representation*, Position> rep_to_pos_map;
	
	const RepresentationList& reps = getRepresentations();
	PrimitiveManager::RepresentationList::const_iterator rep_it = reps.begin();
	for (Position i = 0; rep_it != reps.end(); rep_it++)
	{
		rep_to_pos_map[*rep_it] = i;
		i++;
	}

	for (Position plane_pos = 0; plane_pos < getClippingPlanes().size(); plane_pos++)
	{
		ClippingPlane* const plane = getClippingPlanes()[plane_pos];
		String data_string;

		data_string += vector3ToString(plane->getNormal());
		data_string += " ";
		data_string += vector3ToString(plane->getPoint());

		data_string += String(plane->isActive());
		data_string += " ";

		HashSet<const Representation*>::ConstIterator rit = 
			plane->getRepresentations().begin();

		for (; +rit; ++rit)
		{
			data_string += String(rep_to_pos_map[*rit]);
			data_string += " ";
		}
		
		out.insertValue("BALLVIEW_PROJECT", "ClippingPlane" + String(plane_pos), data_string);
	}
}
			
void PrimitiveManager::restoreRepresentations(const INIFile& in, const vector<const Composite*>& new_systems)
{
	try
	{
		for (Position p = 0; p < 9999999; p++)
		{
			if (!in.hasEntry("BALLVIEW_PROJECT", "Representation" + String(p))) break;

			String data_string = in.getValue("BALLVIEW_PROJECT", "Representation" + String(p));

			vector<String> string_vector;
			Size split_size;

			// Representation0=1;3 2 2 6.500000 0 0 [2]|Color|H
			// 								 ^ 																	System Number
			// 								         ^            							Model Settings
			// 								         							 ^            Composites numbers
			// 								         							     ^        Custom Color
			// 								         							     			^   Hidden Flag

			// split off information of system number
			split_size = data_string.split(string_vector, ";");
			Position system_pos = string_vector[0].toUnsignedInt();

			// split off between representation settings and composite numbers
			data_string = string_vector[1];
			vector<String> string_vector2;
			data_string.split(string_vector2, "[]");
			data_string = string_vector2[0];
			if (DisplayProperties::getInstance(0) != 0)
			{
				DisplayProperties::getInstance(0)->getSettingsFromString(data_string);
			}

			// Composite positions
			data_string = string_vector2[1];
			data_string.split(string_vector2, ",");
			HashSet<Position> hash_set;
			for (Position p = 0; p < string_vector2.size(); p++)
			{
				hash_set.insert(string_vector2[p].toUnsignedInt());
			}

			if (system_pos >= new_systems.size())
			{
				Log.error() << "Error while reading project file, invalid structure for Representation! Aborting..." << std::endl;
				continue;
			}

			const Composite* composite = new_systems[system_pos];

			data_string = string_vector[1];
			if (data_string.has('|'))
			{
				data_string.split(string_vector2, "|");
				ColorRGBA color;
				color = string_vector2[1];
				if (DisplayProperties::getInstance(0) != 0)
				{
					DisplayProperties::getInstance(0)->setCustomColor(color);
				}
			}

			ControlSelectionMessage* msg = new ControlSelectionMessage();
			Position current = 0;

			Composite::CompositeIterator ccit = ((Composite*)composite)->beginComposite();
			for (; +ccit; ++ccit)
			{
				if (hash_set.has(current)) msg->getSelection().push_back((Composite*)&*ccit);
				current++;
			}

			getMainControl()->sendMessage(*msg);
		
			if (DisplayProperties::getInstance(0) != 0)
			{
				DisplayProperties::getInstance(0)->apply();
			}	

			if (string_vector2.size() == 3 && string_vector2[2].has('H'))
			{
				Representation* rep = 0;
				RepresentationsIterator pit = begin();
				for (; pit != end(); pit++)
				{
					rep = *pit;
				}

				rep->setHidden(true);
				rep->update(false);

   #ifndef BALL_QT_HAS_THREADS
				getMainControl()->sendMessage(*new RepresentationMessage(*rep, RepresentationMessage::UPDATE));
   #endif
			}
		}

		// create a vector with all Representation
		vector<const Representation*> representations;
		PrimitiveManager::RepresentationList::const_iterator rit = 
			getRepresentations().begin();
		for (; rit != getRepresentations().end(); rit++)
		{
			representations.push_back(*rit);
		}

		for (Position p = 0; p < 9999999; p++)
		{
			if (!in.hasEntry("BALLVIEW_PROJECT", "ClippingPlane" + String(p))) break;

			String data_string = in.getValue("BALLVIEW_PROJECT", "ClippingPlane" + String(p));

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
	catch(Exception::InvalidFormat e)
	{
		Log.error() << "Error while reading project file! Aborting..." << std::endl;
		Log.error() << e << std::endl;
		return;
	}
}



void PrimitiveManager::focusRepresentation(const Representation& rep)
{
	List<Vector3> positions;

	Vector3 center;
	List<GeometricObject*>::ConstIterator it = rep.getGeometricObjects().begin();
	for (; it != rep.getGeometricObjects().end(); it++)
	{
		const GeometricObject& go = **it;

		// cant use Vertex or Vertex2 here, no idea why
		if (RTTI::isKindOf<Vertex2>(go))
		{
			const Vertex2& v = *dynamic_cast<const Vertex2*>(&go);
			positions.push_back(v.getVertex1());
			positions.push_back(v.getVertex2());
		}
		else if (RTTI::isKindOf<Vertex>(go))
		{
			const Vertex& v = *dynamic_cast<const Vertex*>(&go);
			positions.push_back(v.getVertex());
		}
		else if (RTTI::isKindOf<SimpleBox3>(go))
		{
			const SimpleBox3& b = reinterpret_cast<const SimpleBox3&>(go);
			positions.push_back(b.a);
			positions.push_back(b.b);
		}
		else if (RTTI::isKindOf<Sphere>(go))
		{
			const Sphere& s = reinterpret_cast<const Sphere&>(go);
			positions.push_back(s.getPosition());
		}
		else if (RTTI::isKindOf<Disc>(go))
		{
			const Disc& d = reinterpret_cast<const Disc&>(go);
			positions.push_back(d.getCircle().p);
		}
		else if (RTTI::isKindOf<Mesh>(go))
		{
			const Mesh& mesh = reinterpret_cast<const Mesh&>(go);

			for (Size index = 0; index < mesh.vertex.size(); ++index)
			{
				positions.push_back(mesh.vertex[index]);
			}
			continue;
		}
		else if (RTTI::isKindOf<BALL::VIEW::Box>(go))
		{
			const BALL::VIEW::Box& box = reinterpret_cast<const BALL::VIEW::Box&>(go);
			positions.push_back(box.getPoint());
			positions.push_back(box.getPoint() + box.getHeightVector());
			positions.push_back(box.getPoint() + box.getRightVector());
			positions.push_back(box.getPoint() + box.getDiagonalVector());
		}
		else
		{
			Log.error() << "Unknown geometric object: " << typeid(go).name() 
									<< "in " << __FILE__ << __LINE__ << std::endl;
			continue;
		}
	}

	VIEW::focusCamera(positions);
}

} } // namespaces
