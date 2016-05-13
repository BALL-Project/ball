// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: aromaticityProcessor.C,v 1.13.20.1 2007/03/16 00:06:45 bertsch Exp $
//

#include <BALL/QSAR/aromaticityProcessor.h>

#include <BALL/QSAR/ringPerceptionProcessor.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/PTE.h>

#include <limits>

using namespace std;

namespace BALL
{
	const char* AromaticityProcessor::Option::OVERWRITE_BOND_ORDERS = "overwrite_bond_orders";
	const bool  AromaticityProcessor::Default::OVERWRITE_BOND_ORDERS = true;

	AromaticityProcessor::AromaticityProcessor()
		:	UnaryProcessor<AtomContainer>()
	{
		setDefaultOptions();
	}

	AromaticityProcessor::AromaticityProcessor(const AromaticityProcessor& aro)
		:	UnaryProcessor<AtomContainer>(aro)
	{
	}

	AromaticityProcessor::~AromaticityProcessor()
	{
	}

	AromaticityProcessor& AromaticityProcessor::operator = (const AromaticityProcessor& /* aro */)
	{
		return *this;
	}

	bool AromaticityProcessor::isValid_(const AtomContainer& ac)
	{
		static HashMap<Handle, PreciseTime> mod_times;
		PreciseTime last_mod = ac.getModificationTime();
		Handle mol_handle = ac.getHandle();
		if (mod_times.has(mol_handle))
		{
			if (mod_times[mol_handle] == last_mod)
			{
				return true;
			}
			else
			{
				mod_times[mol_handle] = last_mod;
				return false;
			}
		}
		else
		{
			mod_times.insert(make_pair(mol_handle, last_mod));
			return false;
		}
	}

	bool AromaticityProcessor::start()
	{
		overwrite_bond_orders_ = options.getBool(Option::OVERWRITE_BOND_ORDERS);
		return true;
	}

	Processor::Result AromaticityProcessor::operator () (AtomContainer& ac)
	{
		// we need a ring set if this is called via the processor (or directly with '()' )!
		if (!isValid_(ac))
		{
			RingPerceptionProcessor rpp;
			vector<vector<Atom*> > sssr;
			rpp.calculateSSSR(sssr, ac);
			aromatize(sssr, ac);
		}
		return Processor::CONTINUE;
	}

	void AromaticityProcessor::aromatizeSimple(vector<vector<Atom*> >& sssr)
	{
		vector<HashSet<Atom*> > aromatic_rings;
		vector<HashSet<Atom*> > can_be_rings;
		HashSet<Atom*> aromatic_atoms;
		HashMap<Atom*, Size> correct_n;
		vector<HashSet<Atom*> > sp2n_rings;
		HashMap<Atom*, vector<HashSet<Atom*> > > atom_to_rings;

		// for each
		for (vector<vector<Atom*> >::iterator it=sssr.begin();it!=sssr.end();++it)
		{
			HashSet<Atom*> ring;
			for (vector<Atom*>::iterator ait=it->begin();ait!=it->end();++ait)
			{
				ring.insert(*ait);
			}

			if (simpleCanBeAromatic_(ring))
			{
				// count pi-electrons
				Size num_pi = countPiElectrons_(ring);
				// aromatic? 
				if ((num_pi-2)%4 == 0)
				{
					// test if the rings contains a sp3 nitrogen
					bool has_sp2n(false);
					for (HashSet<Atom*>::ConstIterator nit=ring.begin(); +nit; ++nit)
					{
						if ((*nit)->getElement() == PTE[Element::N])
						{
							Size num_double(0);
							for (Atom::BondIterator bit=(*nit)->beginBond(); +bit; ++bit)
							{
								if (bit->getOrder() == Bond::ORDER__DOUBLE)
								{
									++num_double;
								}
							}
							if (num_double < 1)
							{
								atom_to_rings[*nit].push_back(ring);
								has_sp2n = true;
							}
						}
					}
					
					if (!has_sp2n)
					{
						aromatic_rings.push_back(ring);
						aromatic_atoms += ring;
					}
					else
					{
						sp2n_rings.push_back(ring);
					}
				}
			}
			else
			{
				if (simpleCanBeAromaticWeaker_(ring))
				{
					bool has_sp2n(false);
					for (HashSet<Atom*>::ConstIterator nit=ring.begin(); +nit; ++nit)
					{
						if ((*nit)->getElement() == PTE[Element::N])
						{
							Size num_double(0);
							for (Atom::BondIterator bit=(*nit)->beginBond(); +bit; ++bit)
							{
								if (bit->getOrder() == Bond::ORDER__DOUBLE)
								{
									++num_double;
								}
							}
							if (num_double < 1)
							{
								atom_to_rings[*nit].push_back(ring);
								has_sp2n = true;
							}
						}
					}
					if (!has_sp2n)
					{
						can_be_rings.push_back(ring);
					}
					else
					{
						sp2n_rings.push_back(ring);	
					}
				}
			}
		}

		// handle the sp2n containing rings, all the rings are stored in atom_to_rings
		for (HashMap<Atom*, vector<HashSet<Atom*> > >::Iterator it = atom_to_rings.begin(); it != atom_to_rings.end(); ++it)
		{
			if (it->second.size() > 1)
			{
				// now we must decide which ring to set aromatic
				if (it->second.size() == 2)
				{
					HashSet<Atom*> ring1(it->second[0]), ring2(it->second[1]);
					if (simpleCanBeAromatic_(ring1) && !simpleCanBeAromatic_(ring2))
					{
						if ((countPiElectrons_(ring1)-2)%4 == 0)
						{
							aromatic_rings.push_back(ring1);
							aromatic_atoms += ring1;
						}
						else
						{
							if (simpleCanBeAromaticWeaker_(ring2))
							{
								if ((countPiElectrons_(ring2)-2)%4 == 0)
								{
									can_be_rings.push_back(ring2);
								}
							}
						}
					}
					else
					{
						if (!simpleCanBeAromatic_(ring1) && simpleCanBeAromatic_(ring2))
						{
							if ((countPiElectrons_(ring2)-2)%4 == 0)
							{
								aromatic_rings.push_back(ring2);
								aromatic_atoms += ring2;
							}
							else
							{
								if (simpleCanBeAromatic_(ring1))
								{
									if ((countPiElectrons_(ring1)-2)%4 == 0)
									{
										aromatic_rings.push_back(ring1);
										aromatic_atoms += ring1;
									}
								}
							}
						}
						else
						{
							if (simpleCanBeAromatic_(ring1) && simpleCanBeAromatic_(ring2))
							{
								if ((countPiElectrons_(ring1)-2)%4 == 0)
								{
									aromatic_rings.push_back(ring1);
									aromatic_atoms += ring1;
								}
								else
								{
									if ((countPiElectrons_(ring2)-2)%4 == 0)
									{
										aromatic_rings.push_back(ring2);
										aromatic_atoms += ring2;
									}
								}
							}
							else
							{
								if (simpleCanBeAromaticWeaker_(ring1))
								{
									if ((countPiElectrons_(ring1)-2)%4 == 0)
									{
										can_be_rings.push_back(ring1);
									}
								}
								else
								{
									if (simpleCanBeAromaticWeaker_(ring2))
									{
										if ((countPiElectrons_(ring2)-2)%4 == 0)
										{
											can_be_rings.push_back(ring2);
										}
									}										
								}
							}
						}
					}
				}
				else
				{
					// number of participating rings is 3
					HashSet<Atom*> ring1(it->second[0]), ring2(it->second[1]), ring3(it->second[2]);
					if (simpleCanBeAromatic_(ring1) && (countPiElectrons_(ring1)-2)%4 == 0)
					{
						aromatic_rings.push_back(ring1);
						aromatic_atoms += ring1;
					}
					else
					{
						if (simpleCanBeAromatic_(ring2) && (countPiElectrons_(ring2)-2)%4 == 0)
						{
							aromatic_rings.push_back(ring2);
							aromatic_atoms += ring2;
						}
						else
						{
							if (simpleCanBeAromatic_(ring3) && (countPiElectrons_(ring3)-2)%4 == 0)
							{
								aromatic_rings.push_back(ring3);
								aromatic_atoms += ring3;
							}
							else
							{
								if (simpleCanBeAromaticWeaker_(ring1) && (countPiElectrons_(ring1)-2)%4 == 0)
								{
									can_be_rings.push_back(ring1);
								}
								else
								{
									if (simpleCanBeAromaticWeaker_(ring2) && (countPiElectrons_(ring2)-2)%4 == 0)
									{
										can_be_rings.push_back(ring2);
									}
									else
									{
										if (simpleCanBeAromaticWeaker_(ring3) && (countPiElectrons_(ring3)-2)%4 == 0)
										{
											can_be_rings.push_back(ring3);
										}	
									}
								}
							}
						}
					}
				}
			}
			else
			{
				HashSet<Atom*> ring = *it->second.begin();
				if (simpleCanBeAromatic_(ring))
				{
					if ((countPiElectrons_(ring)-2)%4 == 0)
					{
						aromatic_rings.push_back(ring);
						aromatic_atoms += ring;
					}
				}
				else
				{
					if (simpleCanBeAromaticWeaker_(ring))
					{
						can_be_rings.push_back(ring);
					}
				}
			}
		}

		// now handle the rings which can be aromatic 
		for (vector<HashSet<Atom*> >::const_iterator it=can_be_rings.begin(); it!=can_be_rings.end(); ++it)
		{
			bool can_be(true);
			HashSet<Atom*> ring = *it;
			for (HashSet<Atom*>::ConstIterator ait=ring.begin(); +ait; ++ait)
			{
				Size s_bonds(0), a_bonds(0), d_bonds(0);
				for (Atom::BondIterator bit=(*ait)->beginBond(); +bit; ++bit)
				{
					if (it->has(bit->getPartner(**ait)))
					{
						if (bit->getOrder() == Bond::ORDER__SINGLE) s_bonds++;
						if (bit->getOrder() == Bond::ORDER__AROMATIC) a_bonds++;
						if (bit->getOrder() == Bond::ORDER__DOUBLE) d_bonds++;
					}
					else
					{
						if (aromatic_atoms.has(bit->getPartner(**ait)))
						{
							a_bonds += 2;
						}
					}
				}
				if ((*ait)->getElement() == PTE[Element::C])
				{
					if (!((d_bonds == 1 && s_bonds > 0) || a_bonds > 1))
					{
						can_be = false;
						break;
					}
				}
			}
			if (can_be)
			{
				// intersect the ring with the aromatic rings
				for (vector<HashSet<Atom*> >::const_iterator aro_it=aromatic_rings.begin(); aro_it!=aromatic_rings.end(); ++aro_it)
				{
					if ((countPiElectrons_(ring)-2)%4 == 0)
					{
						aromatic_rings.push_back(ring);
						break;
					}
				}
			}
		}
	
		// write the aromatic rings back to the sssr set
		sssr.clear();
		for (vector<HashSet<Atom*> >::const_iterator it=aromatic_rings.begin();it!=aromatic_rings.end();++it)
		{
			vector<Atom*> ring;
			for (HashSet<Atom*>::ConstIterator ait=it->begin(); +ait; ++ait)
			{
				ring.push_back(*ait);
			}
			sssr.push_back(ring);
		}
	}

	bool AromaticityProcessor::simpleCanBeAromatic_(const HashSet<Atom*>& ring)
	{
		// first ensure that the rings has alternating double bonds
		Size destab(0);
		for (HashSet<Atom*>::ConstIterator ait=ring.begin(); +ait; ++ait)
		{
			Size s_bonds(0), d_bonds(0), a_bonds(0);
			for (Atom::BondIterator bit=(*ait)->beginBond(); +bit; ++bit)
			{
				if (ring.has(bit->getPartner(**ait)))
				{
					if (bit->getOrder() == Bond::ORDER__SINGLE) s_bonds++;
					if (bit->getOrder() == Bond::ORDER__DOUBLE) d_bonds++;
					if (bit->getOrder() == Bond::ORDER__AROMATIC) a_bonds++;
				}
			}
			if ((*ait)->getElement() == PTE[Element::C])
			{
				if (!((d_bonds == 1 && s_bonds > 0) || a_bonds > 1))
				{
					return false;
				}
			}
			else
			{
				if ((*ait)->getElement() == PTE[Element::S])
				{
					if ((*ait)->countBonds() > 2)
					{
						return false;
					}
				}
				if (!(d_bonds == 1 || a_bonds > 1))
				{
					destab++;
				}
			}
		}

		return destab < 2;
	}

	bool AromaticityProcessor::simpleCanBeAromaticWeaker_(const HashSet<Atom*>& ring)
	{
		Size destab(0);
		for (HashSet<Atom*>::ConstIterator ait=ring.begin(); +ait; ++ait)
		{
			Size s_bonds(0), a_bonds(0), d_bonds(0);
			for (Atom::BondIterator bit=(*ait)->beginBond(); +bit; ++bit)
			{
				if (bit->getOrder() == Bond::ORDER__SINGLE) s_bonds++;
				if (bit->getOrder() == Bond::ORDER__AROMATIC) a_bonds++;
				if (bit->getOrder() == Bond::ORDER__DOUBLE)
				{
					// now we check additionally if the partner is in a ring,
					// else the double bond does _not_ count
					if (bit->getPartner(**ait)->getProperty("InRing").getBool())
					{
						++d_bonds;
					}
				}
			}
			if ((*ait)->getElement() == PTE[Element::C])
			{
				if (!(d_bonds == 1 && s_bonds > 0) || a_bonds > 1)
				{
					return false;
				}
			}
			else
			{
				if (d_bonds == 0)
				{
					destab++;
				}
        if ((*ait)->getElement() == PTE[Element::S])
				{
					if ((*ait)->countBonds() > 2)
					{
						return false;
					}
				}
			}
		}
		return destab < 2;
	}

	void AromaticityProcessor::aromatize(const vector<vector<Atom*> >& sssr_orig, AtomContainer& ac)
	{
		overwrite_bond_orders_ = options.getBool(Option::OVERWRITE_BOND_ORDERS);
		vector<HashSet<Atom*> > sssr;
		for (vector<vector<Atom*> >::const_iterator it1=sssr_orig.begin();it1!=sssr_orig.end();++it1)
		{
			HashSet<Atom*> ring;
			for (vector<Atom*>::const_iterator it2=it1->begin();it2!=it1->end();++it2)
			{
				ring.insert(*it2);
			}
			sssr.push_back(ring);
		}

		// first erase rings which have C atoms which cannot be aromatic
		vector<HashSet<Atom*> > sssr_new;
		for (vector<HashSet<Atom*> >::iterator it1=sssr.begin();it1!=sssr.end();++it1)
		{
			Size destab = 0;
			bool may_be = true;
			for (HashSet<Atom*>::iterator it2=it1->begin();it2!=it1->end();++it2)
			{
				if ((*it2)->getElement() == PTE[Element::C] || (*it2)->getElement() == PTE[Element::B])
				{
					int c_count = 0;
					bool is_charged = false;
						// testing if atom is charged then no double bond is needed
					if (((*it2)->getCharge() - (int)((*it2)->getCharge())) == 0 && (*it2)->getCharge() != 0)
					{
						is_charged = true;
					}
								
					for(Atom::BondIterator b_it=(*it2)->beginBond();b_it!=(*it2)->endBond();++b_it)
					{
						if (b_it->getOrder() == Bond::ORDER__DOUBLE)
						{
							++c_count;
						}
					}
					if (c_count != 1 && (c_count == 0 && !is_charged))
					{
						may_be = false;
					}
				}
				// more than one of these elements lead to a non-plane ring system
				if ((*it2)->getElement() == PTE[Element::O] || (*it2)->getElement() == PTE[Element::S])
				{
					if ((*it2)->countBonds() > 2)
					{
						may_be = false;
					}
					++destab;
				}
				if ((*it2)->getElement() == PTE[Element::N] || (*it2)->getElement() == PTE[Element::P])
				{
					Size d_b = 0;
					Size n_b = 0;
					for (Atom::BondIterator bond_it=(*it2)->beginBond();bond_it!=(*it2)->endBond();++bond_it)
					{
						if (bond_it->getOrder() == Bond::ORDER__DOUBLE)
						{
							++d_b;
						}
						++n_b;
					}
					if (d_b == 0)
						{
						++destab;
					}
					if (n_b > 3)
					{
						may_be = false;
					}
				}
			}
			if (may_be && destab < 2 && !(destab == 1 && it1->size() == 6) && !(destab == 1 && it1->size() == 7))
			{
					sssr_new.push_back(*it1);
			}
		}
		sssr = sssr_new;

		while(!sssr.empty())
		{			
			// calc intersection and sort the rings, for some reasons set_intersection does not work properly here
			// also accumulate does not work properly
			vector<int> inter_sections;
			for (vector<HashSet<Atom*> >::iterator i=sssr.begin();i!=sssr.end();++i)
			{
				int is2 = 0;
				for (vector<HashSet<Atom*> >::iterator j=sssr.begin();j!=sssr.end();++j)
				{
					if (j!=i)
					{
						for (HashSet<Atom*>::iterator k=i->begin();k!=i->end();++k)
						{
							for (HashSet<Atom*>::iterator l=j->begin();l!=j->end();++l)
							{
								if (*l == *k)
								{
									++is2;
								}
							}
						}
					}
				}
				inter_sections.push_back(is2);
			}
				// find ring with fewest intersection with other rings
			int min_is = std::numeric_limits<int>::max();
			HashSet<Atom*> min_is_ring = 0;
			Size sssr_it(0), tmp(0);
			int min_is_d(0);
			
			for (Size i=0;i!=inter_sections.size();++i, ++sssr_it)
			{
				// count the double bonds, this is for greater ring systems which a single ring has no double bond
				// but is part of the aromatic system, so we begin with a ring which has the most double bonds
				int d_count(0), c_count(0);
				for (HashSet<Atom*>::iterator it=sssr[i].begin();it!=sssr[i].end();++it)
				{
					for (Atom::BondIterator b_it=(*it)->beginBond();b_it!=(*it)->endBond();++b_it)
					{
						if (sssr[i].has(b_it->getPartner(**it)) && b_it->getOrder() == Bond::ORDER__DOUBLE)
						{
							++d_count;
						}
						if (b_it->getFirstAtom()->getElement() == PTE[Element::C])
						{
							++c_count;
						}
					}
				}

				if(min_is > inter_sections[i] || 
					(min_is == inter_sections[i] && (min_is_ring.size() < sssr[i].size() ||	min_is_d < d_count)))
				{
					min_is = inter_sections[i];
					min_is_ring = sssr[sssr_it];
					tmp = sssr_it;
					min_is_d = d_count;
				}
			}
			sssr_it = tmp;

			// try to find aromaticity of the selected ring
			if (hasConjugatedDoubleBonds_(min_is_ring))
			{
				extendAromaticSystem_(sssr, min_is_ring);
			}
			else
			{
				if (min_is != 0)
				{
						extendAromaticSystem_(sssr, min_is_ring);
				}
				else
				{
					// delete it, for some reason .erase does not work properly
					vector<HashSet<Atom*> > temp;
					for (Size i=0;i!=sssr.size();++i)
					{
						if (i!=sssr_it)
						{
							temp.push_back(sssr[i]);
						}
					}
					sssr = temp;
				}
			}
		}

		// if aromatic bonds are set, i.e. from a fragment db we must set the 
		// isAromatic property for the atoms, in most cases this is not necessary
		AtomIterator a_it = ac.beginAtom();
		Atom::BondIterator b_it = a_it->beginBond();
		BALL_FOREACH_BOND(ac, a_it, b_it)
		{
			if (b_it->getOrder() == Bond::ORDER__AROMATIC)
			{
				b_it->getPartner(*(b_it->getFirstAtom()))->setProperty("IsAromatic", true);
				b_it->getPartner(*(b_it->getSecondAtom()))->setProperty("IsAromatic", true);
			}
		}
	}

	void AromaticityProcessor::extendAromaticSystem_(vector<HashSet<Atom*> >& sssr, HashSet<Atom*> ring)
	{
		// calc intersections
		vector<unsigned int> is;
		for (vector<HashSet<Atom*> >::iterator it=sssr.begin();it!=sssr.end();++it)
		{
			unsigned int num_is = 0;
			for (HashSet<Atom*>::iterator it1=it->begin();it1!=it->end();++it1)
			{
				for (HashSet<Atom*>::iterator it2=ring.begin();it2!=ring.end();++it2)
				{
					if (*it1 == *it2)
					{
						++num_is;
					}
				}
			}
			is.push_back(num_is);
		}

		// get the ring with the greatest intersection
		unsigned int max_is = 0;
		HashSet<Atom*> max_is_ring;

		Size sssr_it(0), tmp(0);
		for (Size i=0;i!=is.size();++i,++sssr_it)
		{
			if(is[i] != ring.size() && is[i] > max_is)
			{
				max_is = is[i];
				max_is_ring = sssr[sssr_it];
				tmp = sssr_it;
			}
		}
		sssr_it = tmp;
		
		HashSet<Atom*> merge;
		// now merge the two rings, and avoid the atoms that are in both (except _two_), if any
		if (max_is != 0)
		{
			for (HashSet<Atom*>::iterator it1=max_is_ring.begin();it1!=max_is_ring.end();++it1)
			{
				// check if atom is part of both rings, if, try to avoid those atoms which are not part of the new ring
				int count = 0;
				if (ring.has(*it1))
				{
					for (Atom::BondIterator b_it=(*it1)->beginBond();b_it!=(*it1)->endBond();++b_it)
					{
						Atom * partner = b_it->getPartner(**it1);
						if (ring.has(partner) && max_is_ring.has(partner))
						{
							++count;
						}
					}
				}

				if (count != 4)
				{
					merge.insert(*it1);
				}
			}

			// merge the rings
			for (HashSet<Atom*>::iterator it1=ring.begin();it1!=ring.end();++it1)
			{
				int count = 0;
				if (max_is_ring.has(*it1))
				{
					for (Atom::BondIterator b_it=(*it1)->beginBond();b_it!=(*it1)->endBond();++b_it)
					{
						Atom * partner = b_it->getPartner(**it1);
						if (ring.has(partner) && max_is_ring.has(partner))
						{
							++count;
						}
					}
				}

				if (count != 4)
				{
					merge.insert(*it1);
				}
			}
		

			// test if N is in more than one ring...
			bool correct_n = false;
			for (HashSet<Atom*>::iterator a_it=merge.begin();a_it!=merge.end();++a_it)
			{
				if ((*a_it)->getElement() == PTE[Element::N])
				{
					Size num_in_rings = 0;
					for (Atom::BondIterator b_it=(*a_it)->beginBond();b_it!=(*a_it)->endBond();++b_it)
					{
						if (merge.has(b_it->getPartner(**a_it)))
						{
							++num_in_rings;
						}
					}
					if (num_in_rings == 3)
					{
						correct_n = true;
					}
				}
			}

			if (!correct_n)
			{
				// test the merged ring if it is an aromatic one, if not set aromaticity on the first and delete both, 
				// if delete both and insert the merged one
				if (hasConjugatedDoubleBonds_(merge))
				{
					Size num_pi = countPiElectrons_(merge);
					if ((num_pi-2)%4 == 0)
					{
						// erase the extension from the sssr
						vector<HashSet<Atom*> > temp;
						vector<unsigned int> is_tmp;
						for (Size i=0;i!=sssr.size();++i)
						{
							if (i!=sssr_it)
							{
								temp.push_back(sssr[i]);
								is_tmp.push_back(is[i]);
							}
						}
						sssr = temp;
						is = is_tmp;
						
						//add the merged ring
						sssr.push_back(merge);
					}
					else
					{
						// Hückel's rule not satifsfied for merge -> set aromaticity
						for (HashSet<Atom*>::iterator it=ring.begin();it!=ring.end();++it)
						{
							(*it)->setProperty("IsAromatic", true);
							for (Atom::BondIterator b_it=(*it)->beginBond();b_it!=(*it)->endBond();++b_it)
							{
								if (ring.has(b_it->getPartner(**it)))
								{
									b_it->setProperty(Bond::IS_AROMATIC);
									if (overwrite_bond_orders_)
									{
										b_it->setOrder(Bond::ORDER__AROMATIC);
									}
								}
							}
						}
					}
				}
				else
				{
					// merged ring has no conj. double bonds -> set aromaticity
					for (HashSet<Atom*>::iterator it=ring.begin();it!=ring.end();++it)
					{
						(*it)->setProperty("IsAromatic", true);
						for (Atom::BondIterator b_it=(*it)->beginBond();b_it!=(*it)->endBond();++b_it)
						{
							if (ring.has(b_it->getPartner(**it)))
							{
								b_it->setProperty(Bond::IS_AROMATIC);
								if (overwrite_bond_orders_)
								{	
									b_it->setOrder(Bond::ORDER__AROMATIC);
								}
							}
						}
					}
				}
			}
		}
		else
		{
			// ring has no further intersection with other rings -> check aromaticity
			if ((countPiElectrons_(ring)-2)%4 == 0)
			{
				for (HashSet<Atom*>::iterator it=ring.begin();it!=ring.end();++it)
				{
					(*it)->setProperty("IsAromatic", true);
					for (Atom::BondIterator b_it=(*it)->beginBond();b_it!=(*it)->endBond();++b_it)
					{
						if (ring.has(b_it->getPartner(**it)))
						{
							b_it->setProperty(Bond::IS_AROMATIC);
							if (overwrite_bond_orders_)
							{
								b_it->setOrder(Bond::ORDER__AROMATIC);
							}
						}
					}
				}
			}
		}
		
		// erase ring from sssr
		Size ring_pos = 0;
		for (Size i=0;i!=is.size();++i)
		{
			if (is[i] == ring.size())
			{
				ring_pos = i;
			}
		}

		vector<HashSet<Atom*> > temp;
		for (Size i=0;i!=sssr.size();++i)
		{
			if (i != ring_pos)
			{
				temp.push_back(sssr[i]);
			}
		}
		sssr = temp;
	}



	bool AromaticityProcessor::hasConjugatedDoubleBonds_(HashSet<Atom*> ring_orig)
	{
		// the ring is considered to have conj. double bonds, if 
		// every carbon is sp2 hybridized, hetero atoms are not counted
		// sometimes it might be the case, that a triple bond substitutes a double bond
		// this is a weak criterion, and avoids some clear cases, were the ringed system
		// cannot be aromatic
		Size size = ring_orig.size();
		HashSet<Atom*> ring = ring_orig;

		// set bond_it to a ring bond
		Atom::BondIterator bond_it = (*(ring.begin()))->beginBond();
		for (;bond_it!=(*(ring.begin()))->endBond();++bond_it)
		{
			if (ring.has(bond_it->getPartner(**(ring.begin()))))
			{
				break;
			}
		}
		
		Atom * ancestor = *(ring.begin());
		for (Size i=0; i!=size;++i)
		{
			if (ancestor->getElement() == PTE[Element::C])
			{
				// count if it has _one_ double bond 
				// or two or more aromatic bonds
				// or one triple bond (ok more than one would be strange)
				int d_tmp(0), a_tmp(0), t_tmp(0);
				for (Atom::BondIterator it=ancestor->beginBond();it!=ancestor->endBond();++it)
				{
					if (it->getOrder() == Bond::ORDER__DOUBLE)
					{
						++d_tmp;
					}
					else 
					{
						if (it->getOrder() == Bond::ORDER__TRIPLE)
						{
							++t_tmp;
						} 
						else
						{
							if (it->getOrder() == Bond::ORDER__AROMATIC)
							{
								++a_tmp;
							}
						}
					}
				}
				if (!(d_tmp == 1 || t_tmp == 1 || a_tmp > 1))
				{
					// last chance if the failed carbon is charged!
					if (!((ancestor->getCharge() - (int)(ancestor->getCharge())) == 0 && ancestor->getCharge() != 0))
					{
						return false; // !
					}
				}
			}

			// het atoms are not taken into account, they are considered in the countPiElectrons(...) procedure		
			Atom * atom = bond_it->getPartner(*ancestor);
			// get next ring bond
			for (Atom::BondIterator it=atom->beginBond();it!=atom->endBond();++it)
			{
				if (ring.has(it->getPartner(*atom)) && it->getPartner(*atom) != ancestor)
				{
					bond_it = it;
					ancestor = atom;
					break;
				}
			}
		}
		return true;
	}


	Size AromaticityProcessor::countPiElectrons_(HashSet<Atom*>& ring)
	{
		// here we count the pi electrons of the ringed system
		
		Size num_pi(0), d_count(0), t_count(0), a_count(0), s_count(0), het_count(0);
		Atom::BondIterator j;

		// we know here that the ring system has alternating double bonds
		for (HashSet<Atom*>::iterator i=ring.begin();i!=ring.end();++i)
		{
			// handles charged atoms, tests if the charge is an integer value
			// this is bc BALL knows nothing about formal charges, explicitely
			// i.e. cyclopropyl cation or tropylium cation 
			if (((*i)->getCharge() - (int)((*i)->getCharge())) == 0 && (*i)->getCharge() != 0)
			{
				switch ((int)(*i)->getCharge())
				{
					case 	1: num_pi -= 1; break;
					case 	2: num_pi -= 2; break;
					case 	3: num_pi -= 3; break;
					case   -1: num_pi += 1;  break;
					case   -2: num_pi += 2;  break;
					case   -3: num_pi += 3;  break;
				}
			}

			// add right number of pi electrons for every atom in the system
			switch((*i)->getElement().getAtomicNumber())
			{
				case 5:
					if ((*i)->countBonds() > 3)
					{
						return 0;
					}
				case 6: // C
				case 14: // Si, experimental
				case 32: // Ge, experimental
				case 50: // Sn, experimental
					d_count = 0;
					t_count = 0;
					a_count = 0;
					for (j=(*i)->beginBond();j!=(*i)->endBond();++j)
					{
						// note that double bonds were the partner is _not_ in the ringed
						// system are also counted! This differs much from MOE
						if (j->getOrder() == Bond::ORDER__DOUBLE)
						{
							d_count++;
						}	 
						else 
						{
							if (j->getOrder() == Bond::ORDER__TRIPLE &&
								j->getBoundAtom(**i)->getElement() == PTE[Element::C])
							{
								t_count++;
							}
							else 
							{
								if (j->getOrder() == Bond::ORDER__AROMATIC)
								{
									++a_count;
								}
							}
						}
					}

					// arines have one triple bond, maybe the bonds are set aromatic
					// already (i.e. from file), usual case would be one double bond
					if (d_count == 1 || t_count == 1 || a_count == 2)
					{
						++num_pi;
					}
					else
					{
						return 0;
					}
					break;
				case 7: // N
				case 15: //P
				case 33: // As, very experimental
				case 51: // Sb, same here
					d_count = 0;
					a_count = 0;
					s_count = 0;
					for (j=(*i)->beginBond();j!=(*i)->endBond();++j)
					{
						if (j->getOrder() == Bond::ORDER__DOUBLE)
						{
							++d_count;
						}
						else
						{
							if (j->getOrder() == Bond::ORDER__AROMATIC)
							{
								++a_count;
							}
							else
							{
								if (j->getOrder() == Bond::ORDER__SINGLE)
								{
									++s_count;
								}
							}
						}
					}
					// if this test is true we have a problem, maybe a P or N
					// which has more than 3 bonds
					if (d_count > 1 || s_count > 3)
					{
						return 0;
					}
					
					// sp2 
					if (d_count == 1 || (a_count == 2 && s_count == 0))
					{
						++num_pi;
					}
					
					// sp3
					if (d_count == 0 || (a_count == 2 && s_count == 1))
					{
						++het_count;
						num_pi += 2;
					}
					break;
				case 8: // O
				case 16: // S
				case 34: // Se, expermental
				case 52: // Te, experimental
					num_pi += 2;
					++het_count;
					break;
				default: 
					// this is very difficult to decide what to do here :), 
					// so we be passiv, do nothing and hope the user will fix that
					Log.error() << "AromaticityProcessor::countPiElectrons_: No pi-electron-handle for atom with element: " 
						<< (*i)->getElement().getAtomicNumber() << endl;
					break;
			}
		}	
		
		// these het atoms destabilize the ring, the ring isnt plane any more -> no pi system!
		// beside the fact this should not be possible, bc this is checked above, but only for first
		// level rings, not for merged ones! Returning 0 will result in not satisfying Hückels rule.
		if (het_count > 1)
		{
			return 0;
		}
		else
		{
			return num_pi;
		}
	}
	
	void AromaticityProcessor::setDefaultOptions()
	{		
	 	options.setDefaultBool(AromaticityProcessor::Option::OVERWRITE_BOND_ORDERS, 
	 												 AromaticityProcessor::Default::OVERWRITE_BOND_ORDERS); 
	}
} // namespace BALL

