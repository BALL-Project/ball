#include <BALL/KERNEL/molecularInteractions.h>

using namespace BALL;

using std::map;
using std::make_pair;

bool MolecularInteractions::SAVE_ONLY_SUMS = 0;


MolecularInteractions::MolecularInteractions()
{
	total_energy_ = 0;
	threshold_ = 0.01; // by default save only interaction with strength >= 10 J/mol
}

void MolecularInteractions::clear()
{
	interactions_map_.clear();
	interaction_energies_.clear();
	total_energy_ = 0;
}

void MolecularInteractions::setThreshold(double threshold)
{
	threshold_ = fabs(threshold);
}


void MolecularInteractions::addInteraction(const Atom* atom, String interaction_type, double energy)
{
	total_energy_+=energy;

	map<String,double>::iterator s1 = interaction_energies_.find(interaction_type);
	if(s1==interaction_energies_.end())
	{
		interaction_energies_.insert(make_pair(interaction_type,energy));
	}
	else
	{
		s1->second+=energy;
	}

	if(SAVE_ONLY_SUMS) return;

	if(fabs(energy)>=threshold_)
	{
		map<String, map<const Atom*,double> >::iterator search_it = interactions_map_.find(interaction_type);
		if(search_it==interactions_map_.end())
		{
			map<const Atom*,double> new_map;
			interactions_map_.insert(make_pair(interaction_type,new_map));
			search_it = interactions_map_.find(interaction_type);
		}
		search_it->second.insert(make_pair(atom,energy));
	}
}

void MolecularInteractions::addInteraction(String interaction_type, double energy)
{
	total_energy_+=energy;

	map<String,double>::iterator s1 = interaction_energies_.find(interaction_type);
	if(s1==interaction_energies_.end())
	{
		interaction_energies_.insert(make_pair(interaction_type,energy));
	}
	else
	{
		s1->second+=energy;
	}
}


bool MolecularInteractions::hasInteraction(const Atom* atom, String interaction_type) const
{
	map<String, map<const Atom*,double> >::const_iterator search_it = interactions_map_.find(interaction_type);
	if(search_it==interactions_map_.end()) return false;

	if(search_it->second.find(atom)!=search_it->second.end()) return true;
	else return false;
}


double MolecularInteractions::getInteractionEnergy(const Atom* atom, String interaction_type) const
{
	map<String, map<const Atom*,double> >::const_iterator search_it = interactions_map_.find(interaction_type);
	if(search_it==interactions_map_.end()) return 0;

	map<const Atom*,double>::const_iterator s2 = search_it->second.find(atom);
	if(s2==search_it->second.end()) return 0;
	else return s2->second;
}


double MolecularInteractions::getInteractionEnergy(const Atom* atom, const list<String>& interaction_types) const
{
	double energy = 0;
	for(list<String>::const_iterator it=interaction_types.begin(); it!=interaction_types.end(); it++)
	{
		energy+=getInteractionEnergy(atom,*it);
	}
	return energy;
}


const map<const Atom*,double>* MolecularInteractions::getInteractions(String interaction_type) const
{
	map<String, map<const Atom*,double> >::const_iterator search_it = interactions_map_.find(interaction_type);
	if(search_it!=interactions_map_.end()) return &search_it->second;
	else return NULL;
}


void MolecularInteractions::getInteractions(const list<String>& interaction_types, map<const Atom*,double>& interactions) const
{
	for(list<String>::const_iterator s_it=interaction_types.begin(); s_it!=interaction_types.end(); s_it++)
	{
		const map<const Atom*,double>* map_s = getInteractions(*s_it);
		if(!map_s) continue;

		if(s_it==interaction_types.begin())
		{
			interactions = *map_s;
			continue;
		}

		for(map<const Atom*,double>::const_iterator it=map_s->begin(); it!=map_s->end(); it++)
		{
			map<const Atom*,double>::iterator search_it=interactions.find(it->first);
			if(search_it==interactions.end())
			{
				interactions.insert(make_pair(it->first,it->second));
			}
			else
			{
				search_it->second+=it->second;
			}
		}
	}
}


double MolecularInteractions::getInteractionEnergy(String interaction_type) const
{
	map<String,double>::const_iterator s1 = interaction_energies_.find(interaction_type);
	if(s1!=interaction_energies_.end())
	{
		return s1->second;
	}
	else return 0;
}


double MolecularInteractions::getInteractionEnergy(const list<String>& interaction_types) const
{
	double energy=0;
	for(list<String>::const_iterator it=interaction_types.begin(); it!=interaction_types.end(); it++)
	{
		energy += getInteractionEnergy(*it);
	}
	return energy;
}


double MolecularInteractions::getInteractionEnergy() const
{
	return total_energy_;
}
