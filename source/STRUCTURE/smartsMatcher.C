// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/STRUCTURE/smartsMatcher.h>
#include <BALL/QSAR/ringPerceptionProcessor.h>

#include <stack>

using namespace std;

#define REC_STRUCT_POOL_GROWTH 0.3
#define REC_STRUCT_POOL_INITIAL_CAPACITY 10

#define SMARTS_MATCHER_DEBUG
#undef SMARTS_MATCHER_DEBUG

#define REC_DEBUG
#undef REC_DEBUG

namespace BALL
{
	boost::shared_ptr<SmartsMatcher::RecStructPool_> SmartsMatcher::pool_;

	SmartsMatcher::SmartsMatcher()
		: has_user_sssr_(false),
			depth_(0)
	{
		if (!pool_)
		{
			pool_ = boost::shared_ptr<RecStructPool_>(new RecStructPool_);
		}
	}

	SmartsMatcher::SmartsMatcher(const SmartsMatcher& matcher)
		:	rec_matches_(matcher.rec_matches_),
			has_user_sssr_(matcher.has_user_sssr_),
			sssr_(matcher.sssr_),
			depth_(matcher.depth_)
	{
	}

	SmartsMatcher::~SmartsMatcher()
	{
	}

	SmartsMatcher& SmartsMatcher::operator = (const SmartsMatcher& matcher)
	{
		if (&matcher != this)
		{
			rec_matches_ = matcher.rec_matches_;
			has_user_sssr_ = matcher.has_user_sssr_;
			sssr_ = matcher.sssr_;
			depth_ = matcher.depth_;
		}
		return *this;
	}

	void SmartsMatcher::setSSSR(const vector<vector<Atom*> >& sssr)
	{
		sssr_ = sssr;
		has_user_sssr_ = true;
	}

	void SmartsMatcher::unsetSSSR()
	{
		sssr_.clear();
		has_user_sssr_ = false;
	}

	void SmartsMatcher::match(vector<Match>& matches, Molecule& mol, const vector<String>& smarts)
	{
		// TODO implement clever SSSR handling, benefit from the smarts group
		for (vector<String>::const_iterator it = smarts.begin(); it != smarts.end(); ++it)
		{
			Match m;
			match(m, mol, *it);
			matches.push_back(m);
		}
	}

	void SmartsMatcher::match(vector<Match>& matches, Molecule& mol, const vector<String>& smarts, const set<const Atom*>& start_atoms)
	{
		for (vector<String>::const_iterator it = smarts.begin(); it != smarts.end(); ++it)
		{
			Match m;
			match(m, mol, *it, start_atoms);
			matches.push_back(m);
		}
	}

	void SmartsMatcher::match(Match& matches, Molecule& molecule, const String& smarts)
	{
		set<const Atom*> start_atoms;
		for (AtomConstIterator it = molecule.beginAtom(); +it; ++it)
		{
			start_atoms.insert(&*it);
		}
		match(matches, molecule, smarts, start_atoms);
	}

	void SmartsMatcher::match(Match& matches, Molecule& molecule, const String& smarts, const set<const Atom*>& start_atoms)
	{
		// TODO:
		//  - what attributes of the molecule must be set, or external by the user?
		//  - component level grouping, connected components of the molecule graph
		//  - chirality (backends not implemented yet; only matches when properties would be set)
		//  - nested recursive SMARTS (i.e. [$([$(CC)],[$(C)])], why need this? )
	
		//vector<set<const Atom*> > matches;

		rec_matches_.clear();
		
		SmartsParser parser;
		parser.parse(smarts);
		SmartsParser::SPNode* root = parser.getRoot();

		if (parser.getNeedsSSSR())
		{
			if (!has_user_sssr_)
			{
				RingPerceptionProcessor rpp;
				std::vector<std::vector<Atom*> > sssr;
				rpp.calculateSSSR(sssr, molecule);
				parser.setSSSR(sssr);
			}
			else
			{
				parser.setSSSR(sssr_);
			}
		}

		if (parser.hasComponentGrouping())
		{
			// TODO
			// all SPNodes have an member ComponentGroupNumber which is set -1 if it
			// is not set, and and non-negative integer if set.
		}

		if (parser.isRecursive())
		{
			// collect all recursive environments of the tree
			stack<SPNode*> rec_nodes;
			set<SPNode*> tree_nodes = parser.getNodes();
			for (set<SPNode*>::const_iterator it = tree_nodes.begin(); it != tree_nodes.end(); ++it)
			{
				if ((*it)->isRecursive())
				{
					rec_nodes.push(*it);
				}
			}
	
			#ifdef REC_DEBUG
			cerr << "rec nodes: " << rec_nodes.size() << endl;
			#endif
		
			while (!rec_nodes.empty())
			{
				vector<set<const Atom*> > tmp;
				//for (AtomConstIterator it=molecule.beginAtom(); +it; ++it)
				for (set<const Atom*>::const_iterator it = start_atoms.begin(); it != start_atoms.end(); ++it)
				{
					RecStruct_ rs;
					evaluate_(rs, rec_nodes.top(), *it);
					if (rs.matched_atoms.size() != 0)
					{
						for (Size i = 0; i != rs.matched_atoms.size(); ++i)
						{
							if (evaluateRingEdges_(rs.matched_atoms[i], rs.mapped_atoms[i], smarts))
							{
								//tmp.push_back(rs.matched_atoms[i]);
								set<const Atom*> hm_tmp;
								hm_tmp.insert(rs.first_matches[i].second);
								tmp.push_back(hm_tmp);
							}
						}
					}
				}
				// eliminate double hits 
				
				vector<set<const Atom*> > tmp2;
				set<const Atom*> rec_atoms;
				for (vector<set<const Atom*> >::const_iterator it = tmp.begin(); it != tmp.end(); ++it)
				{
					if (rec_atoms.find(*it->begin()) == rec_atoms.end())
					{
						rec_atoms.insert(*it->begin());
						tmp2.push_back(*it);
					}					
				}
				tmp = tmp2;
				if (rec_nodes.top()->getNot())
				{
					//cerr << "not" << endl;
					set<const Atom*> non_match;
					for (vector<set<const Atom*> >::const_iterator jt = tmp.begin(); jt != tmp.end(); ++jt)
					{
						for (set<const Atom*>::const_iterator it = jt->begin(); it != jt->end(); ++it)
						{
							non_match.insert(*it);
						}
					}
					
					set<const Atom*> tmp_match;
					//for (AtomConstIterator it=molecule.beginAtom(); +it; ++it)
					for (set<const Atom*>::const_iterator it = start_atoms.begin(); it != start_atoms.end(); ++it)
					{
						if (non_match.find(*it) == non_match.end())
						{
							tmp_match.insert(*it);
						}
					}
					rec_matches_[rec_nodes.top()].push_back(tmp_match);
				}
				else
				{
					rec_matches_[rec_nodes.top()] = tmp;
				}
				rec_nodes.pop();
			}

			#ifdef REC_DEBUG
			cerr << "#rec env: " << rec_matches_.size() << endl;
			for (map<SPNode*, vector<set<const Atom*> > >::const_iterator it = rec_matches_.begin(); it != rec_matches_.end(); ++it)
			{
				cerr << it->second.size() << ": ";
				for (vector<set<const Atom*> >::const_iterator it1 = it->second.begin(); it1 != it->second.end(); ++it1)
				{
					cerr << it1->size() << " ";
					for (set<const Atom*>::const_iterator it2 = it1->begin(); it2 != it1->end(); ++it2)
					{
						cerr << (*it2)->getName() << " ";
					}
				}
				cerr << endl;
			}
			#endif			
		}

		//else
		//{
			// TODO: better selection heuristic, most discriminating part should be choosen first
			// 
			//for (AtomConstIterator it=molecule.beginAtom(); +it; ++it)
			for (set<const Atom*>::const_iterator it = start_atoms.begin(); it != start_atoms.end(); ++it)
			{
				RecStruct_ rs;
				evaluate_(rs, root, *it);
				#ifdef SMARTS_MATCHER_DEBUG
				cerr << "SM: found " << rs.matched_atoms.size() << " matchings without considering ring edges" << endl;
				#endif
				if (rs.matched_atoms.size() != 0)
				{
					for (Size i=0; i!=rs.matched_atoms.size(); ++i)
					{
						#ifdef SMARTS_MATCHER_DEBUG
						cerr << "size of matchings[" << i << "]: " << rs.matched_atoms[i].size() << ", size of mappings[" << i << "]: " << rs.mapped_atoms[i].size() << endl;
						#endif
						if (evaluateRingEdges_(rs.matched_atoms[i], rs.mapped_atoms[i], smarts))
						{
							matches.push_back(rs.matched_atoms[i]);
						}
					}
				}
			}
		//}

		// TODO eliminate double hits while matching, for example CC in alphatic chains, can match C1C2 and C2C1
		if (matches.size() != 0)
		{
			vector<set<const Atom*> > unique_matches;
			unique_matches.push_back(matches[0]);
			for (vector<set<const Atom*> >::const_iterator it1 = matches.begin(); it1 != matches.end(); ++it1)
			{
				bool has(false);
				for (vector<set<const Atom*> >::const_iterator it2 = unique_matches.begin(); it2 != unique_matches.end(); ++it2)
				{
					if (it1->size() == it2->size())
					{
						bool equal(true);
						for (set<const Atom*>::const_iterator it3 = it1->begin(); it3 != it1->end(); ++it3)
						{
							if (it2->find(*it3) == it2->end())
							{
								equal = false;
								break;
							}
						}
						if (equal)
						{
							has = true;
						}
					}
				}
				if (!has)
				{
					unique_matches.push_back(*it1);
				}
			}
			matches = unique_matches;
		}
#ifdef SMARTS_MATCHER_DEBUG
		cerr << "SM: found " << matches.size() << /* " matches from " << matched_atoms.size() << " seeds:" <<*/ endl;
		for (vector<set<const Atom*> >::const_iterator it=matches.begin(); it!=matches.end(); ++it)
		{
			cerr << "> size=" << it->size();
			for (set<const Atom*>::const_iterator it1 = it->begin(); it1 != it->end(); ++it1)
			{
				cerr << "\t" << (*it1)->getElement().getSymbol() << "(" << *it1 << ")";
			}
			cerr << endl;
		}
#endif

		//return matches;
	}

	bool SmartsMatcher::evaluateRingEdges_(const set<const Atom*>& match, const map<const SPNode*, const Atom*>& mapping, const String& smarts)
	{
		#ifdef SMARTS_MATCHER_DEBUG
		cerr << "bool SmartsMatcher::evaluateRingEdges_( const set<const Atom*>& match, const map<const SPNode*, const Atom*>& mapping)" << endl;
		for (map<const SPNode*, const Atom*>::const_iterator it = mapping.begin(); it != mapping.end(); ++it)
		{
			cerr << it->second->getName() << " ";
		}
		cerr << endl;
		#endif
		const map<Size, vector<SPNode*> >& ring_bonds = SmartsParser::state.current_parser->getRingConnections();
		if (!ring_bonds.empty())
		{
		
			for (map<Size, vector<SPNode*> >::const_iterator it = ring_bonds.begin(); it != ring_bonds.end(); ++it)
			{
				if (it->second.size()%2 != 0)
				{
					throw Exception::ParseError(__FILE__, __LINE__, "wrong number of ring bond indices (was "+String(it->first)+"): "+smarts, "");
				}
				for (Size i = 0; i != it->second.size(); i += 2)
				{
					if (mapping.find(it->second[i]) != mapping.end() && mapping.find(it->second[i+1]) != mapping.end())
					{
						const Atom* first = mapping.find(it->second[i])->second;
						const Atom* second = mapping.find(it->second[i+1])->second;
						if (match.find(first) == match.end() || match.find(second) == match.end())
						{
							return false;
						}
						else
						{
							if (!first->isBoundTo(*second))
							{
								return false;
							}
							else
							{
								// TODO correct?!? only aromatic and single ring closure bonds are allowed?
								if (first->getBond(*second)->getOrder() != Bond::ORDER__SINGLE && !(first->getBond(*second)->isAromatic()))
								{
									return false;
								}
							}
						}
					}
					//else
					//{
						//cerr << "rings bonds not within this mapping" << endl;
					//}
				}
			}
		}
		return true;
	}
	
	void SmartsMatcher::evaluate_(RecStruct_& rs, SPNode* start_node, const Atom* start_atom)
	{
		#ifdef SMARTS_MATCHER_DEBUG
		depth_ = 0;
		cerr << "void SmartsMatcher::evaluate_(start_node=" << start_node << ", start_atom=" << start_atom->getName() << ")" << endl;
		#endif
		evaluate_node_(rs, start_node, start_atom);
	}

	bool SmartsMatcher::evaluate_node_(RecStruct_& rs, SPNode* start_node, const Atom* start_atom)
	{
		#ifdef SMARTS_MATCHER_DEBUG
		cerr << String('\t', depth_++) << "bool SmartsMatcher::evaluate_node_(start_node=" <<start_node << ", start_atom=" << start_atom->getName() << ")" << endl;
		#endif
		// the results are stored in here
		RecStruct_ result_rs;
		
		bool consider_as_noninternal(false);

		if (start_node->isRecursive())
		{
			//cerr << "node ist recursive " << endl;
			// if rec_matches does _not_ has the node, we are in the pre phase
			// (collecting all rec environment matches)
			if (rec_matches_.find(start_node) != rec_matches_.end()) 
			{
				RecStruct_ new_rs;
				for (vector<set<const Atom*> >::iterator it = rec_matches_[start_node].begin(); it != rec_matches_[start_node].end(); ++it)
				{
					if (it->find(start_atom) != it->end())
					{
						//cerr << start_atom->getName() << endl;
						if (rs.matched_atoms.size() == 0)
						{
							set<const Atom*> matched_atoms;
							matched_atoms.insert(start_atom);
							new_rs.matched_atoms.push_back(matched_atoms);
							new_rs.visited_atoms.push_back(matched_atoms);
							map<const SPNode*, const Atom*> mapped_atoms;
							mapped_atoms[start_node] = start_atom;
							new_rs.first_matches.push_back(make_pair(start_node, start_atom));
							new_rs.mapped_atoms.push_back(mapped_atoms);
							set<const SPEdge*> visited_edges;
							new_rs.visited_edges.push_back(visited_edges);
							set<const Bond*> visited_bonds;
							new_rs.visited_bonds.push_back(visited_bonds);
							// TODO

						}
					
						for (Size i = 0; i != rs.matched_atoms.size(); ++i)
						{
							// add the matched atom
							//if (!rs.matched_atoms[i].has(start_atom))
							//{
								new_rs.add(rs, i);
								new_rs.matched_atoms[new_rs.matched_atoms.size()-1].insert(start_atom);
								new_rs.visited_atoms[new_rs.visited_atoms.size()-1].insert(start_atom);
								new_rs.mapped_atoms[new_rs.mapped_atoms.size()-1][start_node] = start_atom;
							//}
						}	
					}
				}
				
				// count relevant edges TODO
				Size rel_edges_count(0);
				set<SPEdge*> relevant_edges;
				for (SmartsParser::SPNode::EdgeIterator eit = start_node->begin(); eit != start_node->end(); ++eit)
				{
					if (!SmartsParser::state.current_parser->hasRecursiveEdge(*eit))
					{
						rel_edges_count++;
						relevant_edges.insert(*eit);
					}
				}
				if (rel_edges_count == 0)
				{
					result_rs.add(new_rs);
				}
			
				// the node must be non-internal? TODO
				//cerr << new_rs.matched_atoms.size();
				//if (new_rs.matched_atoms.size() != 0)
				//{
				//	cerr << new_rs.matched_atoms[0].size() << endl;
				//}
				//cerr << endl;
				
				
				for (Size i = 0; i != new_rs.matched_atoms.size(); ++i)
				{
					for (Atom::BondConstIterator bit = start_atom->beginBond(); bit != start_atom->endBond(); ++bit)
          {
            if (new_rs.visited_bonds[i].find(&*bit) == new_rs.visited_bonds[i].end())
            {
              for (SmartsParser::SPNode::EdgeIterator eit = start_node->begin(); eit != start_node->end(); ++eit)
              {
                if (new_rs.visited_edges[i].find(*eit) == new_rs.visited_edges[i].end() && relevant_edges.find(*eit) !=  relevant_edges.end())
                {
									
                  RecStruct_ first_new_rs;
                  first_new_rs.add(new_rs, i);
                  first_new_rs.mapped_atoms[0][start_node] = start_atom;
                  first_new_rs.matched_atoms[0].insert(start_atom);
                  first_new_rs.visited_atoms[0].insert(start_atom);
                  first_new_rs.visited_bonds[0].insert(&*bit);
                  first_new_rs.visited_edges[0].insert(*eit);
#ifdef SMARTS_MATCHER_DEBUG
									first_new_rs.dump("A", depth_);
#endif
	                if (evaluate_edge_(first_new_rs, *eit, start_atom, &*bit))
                  {
										depth_--;
                    if (start_node->countEdges() > 1)
                    {
                      for (Size j = 0; j != first_new_rs.matched_atoms.size(); ++j)
                      {
                        RecStruct_ second_new_rs;
                        second_new_rs.add(first_new_rs, j);
#ifdef SMARTS_MATCHER_DEBUG
												second_new_rs.dump("B", depth_);
#endif
                        if (evaluate_node_(second_new_rs, start_node, start_atom))
                        {
													depth_--;
                          result_rs.add(second_new_rs);
                        }
                      }
                    }
                    else
                    {
                      result_rs.add(first_new_rs);
                    }
                  }
								}
							}
						}
					}
				}
				rs = result_rs;
				return result_rs.matched_atoms.size() != 0;
			}
		}

		// if it is a internal node, we must consider the logical operator
		if (start_node->isInternal())
		{
			#ifdef SMARTS_MATCHER_DEBUG
			cerr << String('\t', depth_) << "node is internal" << endl;
			#endif
			// both edges must be set
			SPEdge* first_edge = start_node->getFirstEdge();
			SPEdge* second_edge = start_node->getSecondEdge();
			SmartsParser::LogicalOperator log_op = start_node->getLogicalOperator();
			if (log_op == SmartsParser::AND || log_op == SmartsParser::AND_LOW)
			{
#ifdef SMARTS_MATCHER_DEBUG
				cerr << String('\t', depth_) << "log_op = AND" << endl;
#endif
				if (rs.matched_atoms.size() == 0)
				{
					RecStruct_ first_new_rs;
#ifdef SMARTS_MATCHER_DEBUG
					first_new_rs.dump("C", depth_);
#endif
					if (evaluate_node_(first_new_rs, first_edge->getSecondSPNode(), start_atom))
					{
						for (Size i = 0; i != first_new_rs.matched_atoms.size(); ++i)
						{
							RecStruct_ second_new_rs;
							second_new_rs.add(first_new_rs, i);
#ifdef SMARTS_MATCHER_DEBUG
							second_new_rs.dump("D", depth_);
#endif
							if (evaluate_node_(second_new_rs, second_edge->getSecondSPNode(), start_atom))
							{
								result_rs.add(second_new_rs);
							}
							depth_--;
						}
					}
					depth_--;
				}
				else
				{
					// for every possible beginning
					for (Size i=0; i!=rs.matched_atoms.size(); ++i)
					{
						// create new data structures
						RecStruct_ first_new_rs;
						first_new_rs.add(rs, i);
						// for each bond
						for (Atom::BondConstIterator it1 = start_atom->beginBond(); it1 != start_atom->endBond(); ++it1)
						{
#ifdef SMARTS_MATCHER_DEBUG
							first_new_rs.dump("E", depth_);
#endif
					
							// can it be matched?
							if (evaluate_edge_(first_new_rs, first_edge, start_atom, &*it1))
							{
								// now try to match the second edge
								for (Size j = 0; j != first_new_rs.matched_atoms.size(); ++j)
								{
									RecStruct_ second_new_rs;
									second_new_rs.add(first_new_rs, j);
									for (Atom::BondConstIterator it2=start_atom->beginBond(); it2!=start_atom->endBond(); ++it2)
									{
#ifdef SMARTS_MATCHER_DEBUG
										second_new_rs.dump("F", depth_);
#endif

										if (evaluate_edge_(second_new_rs, second_edge, start_atom, &*it2))
										{
											result_rs.add(second_new_rs);
										}
										depth_--;
									}
								}
							}
							depth_--;
						}
					}
				}
			}
			else
			{
				#ifdef SMARTS_MATCHER_DEBUG
				cerr << String('\t', depth_) << "log_op=OR" << endl;
				#endif
				if (log_op == SmartsParser::OR)
				{
					if (rs.matched_atoms.size() == 0)
					{
						RecStruct_ new_rs;
#ifdef SMARTS_MATCHER_DEBUG
						new_rs.dump("G", depth_);
#endif
						if (evaluate_node_(new_rs, first_edge->getSecondSPNode(), start_atom))
						{
							result_rs.add(new_rs);
						}
						depth_--;
						new_rs.clear();
#ifdef SMARTS_MATCHER_DEBUG
						new_rs.dump("H", depth_);
#endif
						if (evaluate_node_(new_rs, second_edge->getSecondSPNode(), start_atom))
						{
							result_rs.add(new_rs);
						}
						depth_--;
					}
					else
					{
						for (Size i = 0; i != rs.matched_atoms.size(); ++i)
						{
							RecStruct_ new_rs;
							new_rs.add(rs, i);
#ifdef SMARTS_MATCHER_DEBUG
							new_rs.dump("I", depth_);
#endif
							if (evaluate_node_(new_rs, first_edge->getSecondSPNode(), start_atom))
							{
								result_rs.add(new_rs);
							}
							depth_--;
											
							new_rs.clear();
							new_rs.add(rs, i);
#ifdef SMARTS_MATCHER_DEBUG
							new_rs.dump("J", depth_);
#endif
							if (evaluate_node_(new_rs, second_edge->getSecondSPNode(), start_atom))
							{
								result_rs.add(new_rs);
							}
							depth_--;
						}
						
					}
				}
			}
			if (start_node->countEdges() != 0)
			{
				//cerr << "consider node as noninternal" << endl;
				consider_as_noninternal = true;
			}
		}

		// normal mode		
		if (!start_node->isInternal() || consider_as_noninternal)
		{
			
			RecStruct_ new_rs(rs);
			if (consider_as_noninternal || start_node->getSPAtom()->equals(start_atom))
			{
				if (consider_as_noninternal)
				{
					if (result_rs.matched_atoms.size() == 0)
					{
						return false;
					}

					// prepare the matches
					new_rs = result_rs;
					result_rs.clear();
				}

				// first matched node?
				if (new_rs.matched_atoms.size() == 0)
				{
					#ifdef SMARTS_MATCHER_DEBUG
					//cerr << String('\t', depth_) << "matched_Atoms.size() == 0" << endl;
					#endif
					
					set<const Atom*> matched_atom;
					matched_atom.insert(start_atom);
					new_rs.matched_atoms.push_back(matched_atom);
					map<const SPNode*, const Atom*> mapped_atom;
					mapped_atom[start_node] = start_atom;
					new_rs.mapped_atoms.push_back(mapped_atom);
					new_rs.first_matches.push_back(make_pair(start_node, start_atom));
					new_rs.visited_atoms.push_back(matched_atom);
					set<const Bond*> visited_bond;
					new_rs.visited_bonds.push_back(visited_bond);
					set<const SPEdge*> visited_edge;
					new_rs.visited_edges.push_back(visited_edge);
				}
				for (Size i = 0; i != new_rs.matched_atoms.size(); ++i)
				{
					// are we at a leaf?
					if (start_node->countEdges() == 0)
					{
						// just add the node and atom to the results 
						#ifdef SMARTS_MATCHER_DEBUG
						//cerr << String('\t', depth_) << "leaf node, i=" << i << ", " << new_rs.matched_atoms.size() << " (size of first matches = " << new_rs.first_matches.size() << ", size of mapped atoms = " << new_rs.mapped_atoms.size() << ")" <<endl;
						#endif
						result_rs.add(new_rs, i);
						result_rs.matched_atoms[result_rs.matched_atoms.size()-1].insert(start_atom);
						result_rs.visited_atoms[result_rs.visited_atoms.size()-1].insert(start_atom);
						result_rs.mapped_atoms[result_rs.mapped_atoms.size()-1][start_node] = start_atom;

						continue;
					}
					
					Size matched_bonds(0), matched_edges(0);
					
					for (Atom::BondConstIterator bit = start_atom->beginBond(); bit != start_atom->endBond(); ++bit)
					{
						if (new_rs.visited_bonds[i].find(&*bit) == new_rs.visited_bonds[i].end())
						{
							for (SmartsParser::SPNode::EdgeIterator eit = start_node->begin(); eit != start_node->end(); ++eit)
							{
								if (new_rs.visited_edges[i].find(*eit) == new_rs.visited_edges[i].end())
								{
									RecStruct_ first_new_rs;
									first_new_rs.add(new_rs, i);
									first_new_rs.mapped_atoms[0][start_node] = start_atom;
									first_new_rs.matched_atoms[0].insert(start_atom);
									first_new_rs.visited_atoms[0].insert(start_atom);
									first_new_rs.visited_bonds[0].insert(&*bit);
									first_new_rs.visited_edges[0].insert(*eit);
									#ifdef SMARTS_MATCHER_DEBUG
									first_new_rs.dump("K", depth_);
									#endif
									if (evaluate_edge_(first_new_rs, *eit, start_atom, &*bit))
									{
										if (start_node->countEdges() > 1)
										{
											for (Size j = 0; j != first_new_rs.matched_atoms.size(); ++j)
											{
												RecStruct_ second_new_rs;
												second_new_rs.add(first_new_rs, j);
#ifdef SMARTS_MATCHER_DEBUG
												second_new_rs.dump("L", depth_);
#endif
												if (evaluate_node_(second_new_rs, start_node, start_atom))
												{
													result_rs.add(second_new_rs);
												}
												depth_--;
											}
										}
										else
										{
											result_rs.add(first_new_rs);
										}
									}
									depth_--;
								}
							}
						}
						else
						{
							matched_bonds++;
						}
					}

					set<SPEdge*> visited_edges;
					for (SmartsParser::SPNode::EdgeIterator eit = start_node->begin(); eit != start_node->end(); ++eit)
					{
						if (new_rs.visited_edges[i].find(*eit) != new_rs.visited_edges[i].end())
						{
							matched_edges++;
							visited_edges.insert(*eit);
						}
					}
					
					if (start_node->countEdges() != 0 && matched_edges != 0 && matched_edges == start_node->countEdges())
					{
						if (matched_bonds == matched_edges)
						{
							result_rs.add(new_rs, i);
						}
						else
						{
							if (matched_bonds - 1 == matched_edges)
							{
								result_rs.add(new_rs, i);
							}
						}
					}
				}
			}
		}
	
		rs = result_rs;
		
#ifdef SMARTS_MATCHER_DEBUG
		cerr << String('\t', depth_) << "stats for evaluate node: " << endl;
		rs.dump("M", depth_);
#endif
		
		return rs.matched_atoms.size() != 0;
	}
	
	bool SmartsMatcher::evaluate_edge_(RecStruct_& rs, SPEdge* start_edge, const Atom* start_atom, const Bond* start_bond)
	{
		#ifdef SMARTS_MATCHER_DEBUG
		cerr << String('\t', depth_++) << "bool SmartsMatcher::evaluate_edge_(start_edge=" << start_edge << ", start_atom=" << start_atom->getName() << ", start_bond=" << start_bond << endl;
		#endif
		RecStruct_ result_rs;
		
		bool consider_as_non_internal(false);
				
		if (start_edge->isInternal())
		{
			SmartsParser::LogicalOperator log_op = start_edge->getLogicalOperator();
			if (log_op == SmartsParser::AND || log_op == SmartsParser::AND_LOW)
			{
				for (Size i = 0; i != rs.matched_atoms.size(); ++i)
				{
					RecStruct_ first_new_rs;
					first_new_rs.add(rs, i);
#ifdef SMARTS_MATCHER_DEBUG
					first_new_rs.dump("O", depth_);
#endif
					if (evaluate_edge_(first_new_rs, start_edge->getFirstSPEdge(), start_atom, start_bond))
					{
						for (Size j = 0; j != first_new_rs.matched_atoms.size(); ++j)
						{
							RecStruct_ second_new_rs;
							second_new_rs.add(first_new_rs, j);
#ifdef SMARTS_MATCHER_DEBUG
							second_new_rs.dump("P", depth_);
#endif
						 	if (evaluate_edge_(second_new_rs, start_edge->getSecondSPEdge(), start_atom, start_bond))
							{
								result_rs.add(second_new_rs);
							}
							depth_--;
						}
					}
					depth_--;
				}
			}
			else
			{
				if (log_op == SmartsParser::OR)
				{
					for (Size i = 0; i != rs.matched_atoms.size(); ++i)
					{
						RecStruct_ new_rs;
						new_rs.add(rs, i);
#ifdef SMARTS_MATCHER_DEBUG
						new_rs.dump("Q", depth_);
#endif
						if (evaluate_edge_(new_rs, start_edge->getFirstSPEdge(), start_atom, start_bond))
						{
							result_rs.add(new_rs);
						}
						depth_--;
						new_rs.clear();
						new_rs.add(rs, i);
#ifdef SMARTS_MATCHER_DEBUG
						new_rs.dump("R", depth_);
#endif
						if (evaluate_edge_(new_rs, start_edge->getSecondSPEdge(), start_atom, start_bond))
						{
							result_rs.add(new_rs);
						}
						depth_--;
					}
				}
				else
				{
					// in this case the edges are just internal edges without any logical op, sense ????
					for (Size i = 0; i != rs.matched_atoms.size(); ++i)
					{
						RecStruct_ new_rs;
						new_rs.add(rs, i);
#ifdef SMARTS_MATCHER_DEBUG
						new_rs.dump("S", depth_);
#endif
						if (evaluate_node_(new_rs, start_edge->getSecondSPNode(), start_atom))
						{
							result_rs.add(new_rs);
						}
						depth_--;
					}
				}
			}

			
			// now test if there is also a atom interconnection
			if (start_edge->getFirstSPEdge() != 0 && start_edge->getSecondSPEdge() != 0 && result_rs.matched_atoms.size() != 0)
			{
				consider_as_non_internal = true;
				result_rs.clear();
			}
		}

#ifdef SMARTS_MATCHER_DEBUG
		result_rs.dump("", depth_);
#endif
		
		if (!start_edge->isInternal() || consider_as_non_internal)
		{
			// just call the evaluate_node_ methode with next node
			if (consider_as_non_internal || start_edge->getSPBond()->equals(start_bond))
			{
				// TODO sense?
				if (start_edge->getFirstSPNode() == 0 && start_edge->getSecondSPNode() == 0)
				{
					return true;
				}
				// only non-ringclosure bonds are considered so far!
				const Atom* partner = start_bond->getPartner(*start_atom);
				for (Size i = 0; i != rs.matched_atoms.size(); ++i)
				{
					if (rs.matched_atoms[i].find(partner) == rs.matched_atoms[i].end())
					{
						RecStruct_ new_rs;
						new_rs.add(rs, i);
					
						// TODO first case correct?
						if (consider_as_non_internal || start_edge->getSPBond()->equals(start_bond))
						{
							new_rs.visited_bonds[0].insert(start_bond);
							new_rs.visited_edges[0].insert(start_edge);
						}
#ifdef SMARTS_MATCHER_DEBUG
						cerr << "size of first matches = " << new_rs.first_matches.size() << endl;
						new_rs.dump("T", depth_);
#endif
						if (evaluate_node_(new_rs, start_edge->getSecondSPNode(), partner))
						{
							result_rs.add(new_rs);
						}
						depth_--;
					}
				}
			}
		}

		rs = result_rs;
#ifdef SMARTS_MATCHER_DEBUG
		cerr << String('\t', depth_) << "stats for evaluate edge: " << endl;
		rs.dump("U", depth_);
#endif
		return rs.matched_atoms.size() != 0;
	}


	// rec struct core part
	SmartsMatcher::RecStructCore_::RecStructCore_()
	{
	}

	SmartsMatcher::RecStructCore_::RecStructCore_(const RecStructCore_& rec_struct)
		:	matched_atoms(rec_struct.matched_atoms),
			mapped_atoms(rec_struct.mapped_atoms),
			visited_atoms(rec_struct.visited_atoms),
			visited_bonds(rec_struct.visited_bonds),
			visited_edges(rec_struct.visited_edges),
			first_matches(rec_struct.first_matches)
	{
	}

	SmartsMatcher::RecStructCore_::~RecStructCore_()
	{
	}

	SmartsMatcher::RecStructCore_& SmartsMatcher::RecStructCore_::operator = (const RecStructCore_& rec_struct)
	{
		matched_atoms = rec_struct.matched_atoms;
		mapped_atoms = rec_struct.mapped_atoms;
		visited_atoms = rec_struct.visited_atoms;
		visited_bonds = rec_struct.visited_bonds;
		visited_edges = rec_struct.visited_edges;
		first_matches = rec_struct.first_matches;
		return *this;
	}

	void SmartsMatcher::RecStructCore_::clear()
	{
    matched_atoms.clear();
		mapped_atoms.clear();
		visited_atoms.clear();
		visited_bonds.clear();
		visited_edges.clear();
		first_matches.clear();
		return;
	}


	// rec struct pool parts
	SmartsMatcher::RecStructPool_::RecStructPool_()
		:	rec_struct_pool_(vector<RecStructCore_*>(REC_STRUCT_POOL_INITIAL_CAPACITY)),
			last_position_(0)
	{
		for (Position p = 0; p != rec_struct_pool_.size(); ++p)
		{
			rec_struct_pool_[p] = new RecStructCore_();
			free_list_.push_back(p);
		}
	}
	
	SmartsMatcher::RecStructPool_::~RecStructPool_()
	{
		for (Position p = 0; p != rec_struct_pool_.size(); ++p)
		{
			delete rec_struct_pool_[p];
		}
	}

	SmartsMatcher::RecStructCore_* SmartsMatcher::RecStructPool_::getNextFree()
	{
		if (free_list_.empty())
		{
			resize_();
		}
		Position pos(free_list_[free_list_.size()-1]);
		free_list_.pop_back();
		last_position_ = pos;
		return rec_struct_pool_[pos];
	}

	Position SmartsMatcher::RecStructPool_::getLastPosition()
	{
		return last_position_;
	}

	void SmartsMatcher::RecStructPool_::destroy(Position pos)
	{
		// clear the rec_struct_core
		rec_struct_pool_[pos]->clear();
		
		// append the pos to the free_list_
		free_list_.push_back(pos);
	}

	void SmartsMatcher::RecStructPool_::resize_()
	{
		Size old_size(rec_struct_pool_.size());
		//cerr << "resize_(): " << old_size << "(new will be=" << old_size + (Size)(old_size * REC_STRUCT_POOL_GROWTH) << ")" << endl;
		
		// resize the vector with REC_STRUCT_POOL_GROWTH growthfactor
		rec_struct_pool_.resize(old_size + (Size)(old_size * REC_STRUCT_POOL_GROWTH));

		for (Position p = old_size; p != rec_struct_pool_.size(); ++p)
		{
			rec_struct_pool_[p] = new RecStructCore_();
			free_list_.push_back(p);
		}
		return;
	}

	// rec struct
	SmartsMatcher::RecStruct_::RecStruct_()
		:	rec_struct_core_(pool_->getNextFree()),
			matched_atoms(rec_struct_core_->matched_atoms),
			mapped_atoms(rec_struct_core_->mapped_atoms),
			visited_atoms(rec_struct_core_->visited_atoms),
			visited_bonds(rec_struct_core_->visited_bonds),
			visited_edges(rec_struct_core_->visited_edges),
			first_matches(rec_struct_core_->first_matches),
			pos_(pool_->getLastPosition())
	{
	}

	SmartsMatcher::RecStruct_::RecStruct_(const RecStruct_& rec_struct)
		:	rec_struct_core_(pool_->getNextFree()),
			matched_atoms(rec_struct_core_->matched_atoms),
			mapped_atoms(rec_struct_core_->mapped_atoms),
			visited_atoms(rec_struct_core_->visited_atoms),
			visited_bonds(rec_struct_core_->visited_bonds),
			visited_edges(rec_struct_core_->visited_edges),
			first_matches(rec_struct_core_->first_matches),
			pos_(pool_->getLastPosition())
	{
		//cerr << "SmartsMatcher::RecStruct_::RecStruct_(const RecStruct_& rec_struct)" << endl;
		add(rec_struct);
	}

	SmartsMatcher::RecStruct_::~RecStruct_()
	{
		pool_->destroy(pos_);
	}

	SmartsMatcher::RecStruct_& SmartsMatcher::RecStruct_::operator = (const RecStruct_& rec_struct)
	{
		if(this != &rec_struct)
		{
			clear();
			add(rec_struct);
		}

		return *this;
	}

	void SmartsMatcher::RecStruct_::add(const RecStruct_& rec_struct)
	{
		matched_atoms.insert(
			matched_atoms.end(),
			rec_struct.matched_atoms.begin(),
			rec_struct.matched_atoms.end()
		);

		mapped_atoms.insert(
			mapped_atoms.end(),
			rec_struct.mapped_atoms.begin(),
			rec_struct.mapped_atoms.end()
		);

		visited_atoms.insert(
			visited_atoms.end(),
			rec_struct.visited_atoms.begin(),
			rec_struct.visited_atoms.end()
		);

		visited_bonds.insert(
			visited_bonds.end(),
			rec_struct.visited_bonds.begin(),
			rec_struct.visited_bonds.end()
		);

		visited_edges.insert(
			visited_edges.end(),
			rec_struct.visited_edges.begin(),
			rec_struct.visited_edges.end()
		);

		first_matches.insert(
			first_matches.end(),
			rec_struct.first_matches.begin(),
			rec_struct.first_matches.end()
		);
	}

	void SmartsMatcher::RecStruct_::add(const RecStruct_& rec_struct, Size i)
	{
		matched_atoms.push_back(rec_struct.matched_atoms[i]);
		mapped_atoms.push_back(rec_struct.mapped_atoms[i]);
		visited_atoms.push_back(rec_struct.visited_atoms[i]);
		visited_bonds.push_back(rec_struct.visited_bonds[i]);
		visited_edges.push_back(rec_struct.visited_edges[i]);
		first_matches.push_back(rec_struct.first_matches[i]);
	}

	void SmartsMatcher::RecStruct_::clear()
	{
		matched_atoms.clear();
		mapped_atoms.clear();
		visited_atoms.clear();
		visited_bonds.clear();
		visited_edges.clear();
		first_matches.clear();
	}

	void SmartsMatcher::RecStruct_::dump(const String& name, Size depth)
	{
		cerr << String('\t', depth) << name << " " << matched_atoms.size() << " datasets:";
		for (Size i = 0; i != matched_atoms.size(); ++i)
		{
			cerr << " " << i+1 << ". (";
			for (set<const Atom*>::const_iterator it = matched_atoms[i].begin(); it != matched_atoms[i].end(); ++it)
			{
				cerr << (*it)->getName() << ", ";
			}
			cerr << ")";
		}
		cerr << endl;
	}

} // namespace BALL

