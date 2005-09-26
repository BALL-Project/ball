// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: smartsMatcher.C,v 1.2 2005/09/26 00:21:54 bertsch Exp $
//

#include <BALL/STRUCTURE/smartsMatcher.h>
#include <BALL/STRUCTURE/smartsParser.h>
//#include <BALL/KERNEL/PTE.h>

#include <stack>

using namespace std;

#define SMARTS_MATCHER_DEBUG
#undef SMARTS_MATCHER_DEBUG

namespace BALL
{
	SmartsMatcher::SmartsMatcher()
	{
	}

	SmartsMatcher::SmartsMatcher(const SmartsMatcher& /*matcher*/)
	{
	}

	SmartsMatcher::~SmartsMatcher()
	{
	}

	SmartsMatcher::SmartsMatcher& SmartsMatcher::operator = (const SmartsMatcher& matcher)
	{
		return *this;
	}
	
	vector<HashSet<const Atom*> > SmartsMatcher::match(const Molecule& molecule, const String& smarts) 
		throw(Exception::ParseError)
	{
		// TODO what attributes of the molecule must be set?
		
		vector<HashSet<const Atom*> > matches;
		SmartsParser parser;
		parser.parse(smarts);
		SmartsParser::SPNode* root = parser.getRoot();

		// TODO: better selection heuristic, most discriminating part should be choosen first
		// 
		for (AtomConstIterator it=molecule.beginAtom(); +it; ++it)
		{
			RecStruct_ rs;
			evaluate_(rs, root, &*it);
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
					if (evaluateRingEdges_(rs.matched_atoms[i], rs.mapped_atoms[i]))
					{
						matches.push_back(rs.matched_atoms[i]);
					}
				}
			}
		}

		// TODO eliminate double hits while matching, for example CC in alphatic chains, can match C1C2 and C2C1
		if (matches.size() != 0)
		{
			vector<HashSet<const Atom*> > unique_matches;
			unique_matches.push_back(matches[0]);
			for (vector<HashSet<const Atom*> >::const_iterator it1 = matches.begin(); it1 != matches.end(); ++it1)
			{
				bool has(false);
				for (vector<HashSet<const Atom*> >::const_iterator it2 = unique_matches.begin(); it2 != unique_matches.end(); ++it2)
				{
					if (it1->size() == it2->size())
					{
						bool equal(true);
						for (HashSet<const Atom*>::ConstIterator it3 = it1->begin(); it3 != it1->end(); ++it3)
						{
							if (!it2->has(*it3))
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
		for (vector<HashSet<const Atom*> >::const_iterator it=matches.begin(); it!=matches.end(); ++it)
		{
			cerr << "> size=" << it->size();
			for (HashSet<const Atom*>::ConstIterator it1=it->begin(); +it1; ++it1)
			{
				cerr << "\t" << (*it1)->getElement().getSymbol() << "(" << *it1 << ")";
			}
			cerr << endl;
		}
#endif

		return matches;
	}

	bool SmartsMatcher::evaluateRingEdges_(	const HashSet<const Atom*>& match, const HashMap<const SPNode*, const Atom*>& mapping)
	{
		HashMap<Size, vector<SmartsParser::SPNode*> > ring_bonds = SmartsParser::state.current_parser->getRingConnections();
		if (ring_bonds.size() > 0)
		{
			for (HashMap<Size, vector<SmartsParser::SPNode*> >::ConstIterator it=ring_bonds.begin(); +it; ++it)
			{
				if (it->second.size()%2 != 0)
				{
					throw Exception::ParseError(__FILE__, __LINE__, "wrong bond indices", String(it->first));
				}
				for (Size i=0; i!=it->second.size(); i+=2)
				{
					const Atom* first = mapping[it->second[i]];
					const Atom* second = mapping[it->second[i+1]];
					if (!match.has(first) || !match.has(second))
					{
						return false;
					}
					else
					{
						if (!first->isBoundTo(*second))
						{
							return false;
						}
					}
				}
			}
		}
		return true;
	}
	
	void SmartsMatcher::evaluate_(RecStruct_& rs, SPNode* start_node, const Atom* start_atom)
	{
		#ifdef SMARTS_MATCHER_DEBUG
		cerr << "void SmartsMatcher::evaluate_(start_node=" << start_node << ", start_atom=" << start_atom->getName() << ")" << endl;
		#endif
		evaluate_node_(rs, start_node, start_atom);
		return;
	}

	bool SmartsMatcher::evaluate_node_(RecStruct_& rs, SPNode* start_node, const Atom* start_atom)
	{
		#ifdef SMARTS_MATCHER_DEBUG
		cerr << "bool SmartsMatcher::evaluate_node_(start_node=" <<start_node << ", start_atom=" << start_atom->getName() << ")" << endl;
		#endif
		// the results are stored in here
		RecStruct_ result_rs;

		// if it is a internal node, we must consider the logical operator 
		if (start_node->isInternal())
		{
			#ifdef SMARTS_MATCHER_DEBUG
			cerr << "node is internal" << endl;
			#endif
			// both edges must be set
			SPEdge* first_edge = start_node->getFirstEdge();
			SPEdge* second_edge = start_node->getSecondEdge();
			SmartsParser::LogicalOperator log_op = start_node->getLogicalOperator();
			if (log_op == SmartsParser::AND || log_op == SmartsParser::AND_LOW)
			{
				if (rs.matched_atoms.size() == 0)
				{
					// TODO
					cerr << "TODO case not implemented" << endl;
					#ifdef SMARTS_MATCHER_DEBUG
					cerr << " not implemented " << endl;
					#endif
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
									if (evaluate_edge_(second_new_rs, second_edge, start_atom, &*it2))
									{
										result_rs.add(second_new_rs);
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
				#ifdef SMARTS_MATCHER_DEBUG
				cerr << "log_op=OR" << endl;
				#endif
				if (log_op == SmartsParser::OR)
				{
					if (rs.matched_atoms.size() == 0)
					{
						RecStruct_ new_rs;
						if (evaluate_node_(new_rs, first_edge->getSecondSPNode(), start_atom))
						{
							result_rs.add(new_rs);
						}
						new_rs.clear();
						if (evaluate_node_(new_rs, second_edge->getSecondSPNode(), start_atom))
						{
							result_rs.add(new_rs);
						}
					}
					else
					{
						for (Size i = 0; i != rs.matched_atoms.size(); ++i)
						{
							RecStruct_ new_rs;
							new_rs.add(rs, i);

							for (Atom::BondConstIterator it1 = start_atom->beginBond(); it1 != start_atom->endBond(); ++it1)
							{
								if (evaluate_edge_(new_rs, first_edge, start_atom, &*it1))
								{
									result_rs.add(new_rs);
								}
							}
											
							new_rs.clear();
							new_rs.add(rs, i);
						
							for (Atom::BondConstIterator it1 = start_atom->beginBond(); it1 != start_atom->endBond(); ++it1)
							{
								if (evaluate_edge_(new_rs, second_edge, start_atom, &*it1))
								{
									result_rs.add(new_rs);
								}
							}
						}
					}
				}
				else
				{
					// TODO ?? sense?
					cerr << "node: log_op=AND, not implemented" << endl;					
				}
			}
		}
		else // normal node
		{
			if (start_node->getSPAtom()->equals(start_atom))
			{
				#ifdef SMARTS_MATCHER_DEBUG
				cerr << "matched atom (#edges=" << start_node->countEdges() << "), size of matched atoms: " << rs.matched_atoms.size() << endl;
				#endif
				// first matched node?
				if (rs.matched_atoms.size() == 0)
				{
					// just add the found atom and node to the structures
					// the details are further processed by the next for loop
					#ifdef SMARTS_MATCHER_DEBUG
					cerr << "matched_Atoms.size() == 0" << endl;
					#endif
					
					HashSet<const Atom*> matched_atom;
					matched_atom.insert(start_atom);
					rs.matched_atoms.push_back(matched_atom);
					HashMap<const SPNode*, const Atom*> mapped_atom;
					mapped_atom[start_node] = start_atom;
					rs.mapped_atoms.push_back(mapped_atom);
					rs.visited_atoms.push_back(matched_atom);
					HashSet<const Bond*> visited_bond;
					rs.visited_bonds.push_back(visited_bond);
					HashSet<const SPEdge*> visited_edge;
					rs.visited_edges.push_back(visited_edge);
				}
				for (Size i = 0; i != rs.matched_atoms.size(); ++i)
				{
					// are we at a leaf?
					if (start_node->countEdges() == 0)
					{
						// just add the node and atom to the results 
						#ifdef SMARTS_MATCHER_DEBUG
						cerr << "leaf node" << endl;
						#endif
						result_rs.add(rs, i);
						result_rs.matched_atoms[result_rs.matched_atoms.size()-1].insert(start_atom);
						result_rs.visited_atoms[result_rs.visited_atoms.size()-1].insert(start_atom);
						result_rs.mapped_atoms[result_rs.mapped_atoms.size()-1][start_node] = start_atom;
					}

					Size matched_bonds(0), matched_edges(0);
					
					for (Atom::BondConstIterator bit = start_atom->beginBond(); bit != start_atom->endBond(); ++bit)
					{
						if (!rs.visited_bonds[i].has(&*bit))
						{
							for (SmartsParser::SPNode::EdgeIterator eit = start_node->begin(); eit != start_node->end(); ++eit)
							{
								if (!rs.visited_edges[i].has(*eit))
								{
									RecStruct_ first_new_rs;
									first_new_rs.add(rs, i);
									first_new_rs.mapped_atoms[0][start_node] = start_atom;
									first_new_rs.matched_atoms[0].insert(start_atom);
									first_new_rs.visited_atoms[0].insert(start_atom);
									first_new_rs.visited_bonds[0].insert(&*bit);
									first_new_rs.visited_edges[0].insert(*eit);
									#ifdef SMARTS_MATCHER_DEBUG
									cerr << "calling eval edge, size of result_matched_atoms: " << result_rs.matched_atoms.size() << endl;
									#endif
									if (evaluate_edge_(first_new_rs, *eit, start_atom, &*bit))
									{
										if (start_node->countEdges() > 1)
										{
											for (Size j = 0; j != first_new_rs.matched_atoms.size(); ++j)
											{
												RecStruct_ second_new_rs;
												second_new_rs.add(first_new_rs, j);
												if (evaluate_node_(second_new_rs, start_node, start_atom))
												{
													result_rs.add(second_new_rs);
												}
											}
										}
										else
										{
											result_rs.add(first_new_rs);
										}
									}
									#ifdef SMARTS_MATCHER_DEBUG
									cerr << "size of result_matched_atoms after edge_eval call: " << result_rs.matched_atoms.size() << ", ";
									for (Size bla = 0; bla != result_rs.matched_atoms.size(); ++bla)
									{
										cerr << result_rs.matched_atoms[bla].size() << " ";
									}
									cerr << endl;
									#endif
								}
							}
						}
						else
						{
							matched_bonds++;
						}
					}

					for (SmartsParser::SPNode::EdgeIterator eit = start_node->begin(); eit != start_node->end(); ++eit)
					{
						if (rs.visited_edges[i].has(*eit))
						{
							matched_edges++;
						}
					}
					
#ifdef SMARTS_MATCHER_DEBUG
					cerr << "#matched_bonds=" << matched_bonds << ", #matched_edges=" << matched_edges << endl;
#endif
					
					if (matched_bonds != 0 && matched_bonds == matched_edges && matched_edges == start_node->countEdges())
					{
#ifdef SMARTS_MATCHER_DEBUG
						cerr << "all bonds matched" << endl;
#endif
						result_rs.add(rs, i);
					}
				}
			}
		}
	
		#ifdef SMARTS_MATCHER_DEBUG
		if (start_node->countEdges() == 0)
		{
			//cerr << "#of matches = " << result_matched_atoms.size() << ", size=" << result_matched_atoms[0].size() << endl;
		}
		#endif

		rs = result_rs;
		
		return rs.matched_atoms.size() != 0;
	}
	
	bool SmartsMatcher::evaluate_edge_(	RecStruct_& rs, SPEdge* start_edge, const Atom* start_atom, const Bond* start_bond)
	{
		#ifdef SMARTS_MATCHER_DEBUG
		cerr << "bool SmartsMatcher::evaluate_edge_(start_edge=" << start_edge << ", start_atom=" << start_atom << ", start_bond=" << start_bond << endl;
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
					if (evaluate_edge_(first_new_rs, start_edge->getFirstSPEdge(), start_atom, start_bond))
					{
						for (Size j = 0; j != first_new_rs.matched_atoms.size(); ++j)
						{
							RecStruct_ second_new_rs;
							second_new_rs.add(first_new_rs, j);
						 	if (evaluate_edge_(second_new_rs, start_edge->getSecondSPEdge(), start_atom, start_bond))
							{
								result_rs.add(second_new_rs);
							}
						}
					}
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
						
						if (evaluate_edge_(new_rs, start_edge->getFirstSPEdge(), start_atom, start_bond))
						{
							result_rs.add(new_rs);
						}
						new_rs.clear();
						new_rs.add(rs, i);
						if (evaluate_edge_(new_rs, start_edge->getSecondSPEdge(), start_atom, start_bond))
						{
							result_rs.add(new_rs);
						}
					}
				}
				else
				{
					// in this case the edges are just internal edges without any logical op, sense ????
					//cerr << "TODO case not implemented: internal edge with no op" << endl;
					for (Size i = 0; i != rs.matched_atoms.size(); ++i)
					{
						RecStruct_ new_rs;
						new_rs.add(rs, i);
						if (evaluate_node_(new_rs, start_edge->getSecondSPNode(), start_atom))
						{
							result_rs.add(new_rs);
						}
					}
				}
			}

			// now test if there is also a atom interconnection
			if (start_edge->getFirstSPNode() != 0 && start_edge->getSecondSPNode() != 0 && result_rs.matched_atoms.size() != 0)
			{
				consider_as_non_internal = true;
				result_rs.clear();
			}
		}
		
		if (!start_edge->isInternal() || consider_as_non_internal)
		{
			#ifdef SMARTS_MATCHER_DEBUG
			cerr << "non-internal edge (consider_as_non_internal=" << consider_as_non_internal << ")" << endl;
			#endif
			//if (consider_as_non_internal)
			//{
			//	result_matched_atoms.clear();
			//	result_visited_atoms.clear();
			//	result_mapped_atoms.clear();
			//}

			// just call the evaluate_node_ methode with next node
			if (consider_as_non_internal || start_edge->getSPBond()->equals(start_bond))
			{
				// TODO sense?
				if (start_edge->getFirstSPNode() == 0 && start_edge->getSecondSPNode() == 0)
				{
					#ifdef SMARTS_MATCHER_DEBUG
					cerr << "size of matched_atoms " << rs.matched_atoms.size() << ", " << rs.matched_atoms[0].size() << "first node and second node are null" << endl;
					#endif
					//matched_atoms = result_matched_atoms;
					//visited_atoms = result_visited_atoms;
					//mappings = result_mapped_atoms;
					return true;
				}
				// only non-ringclosure bonds are considered so far!
				const Atom* partner = start_bond->getPartner(*start_atom);
				for (Size i = 0; i != rs.matched_atoms.size(); ++i)
				{
					if (!rs.matched_atoms[i].has(partner))
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
						cerr << "firstspNode=" << start_edge->getFirstSPNode() << ", secondspnode=" << start_edge->getSecondSPNode() << endl;
						#endif
						if (evaluate_node_(new_rs, start_edge->getSecondSPNode(), partner))
						{
							result_rs.add(new_rs);
						}
					}
				}
			}
		}

		#ifdef SMARTS_MATCHER_DEBUG
		cerr << "(edge) size of matched_atoms: " << rs.matched_atoms.size()<< " ";
		for (Size bla = 0; bla != rs.matched_atoms.size(); ++bla)
		{
			cerr << rs.matched_atoms[bla].size() << " ";
		}
		cerr << endl;
		#endif

		rs = result_rs;		
		return rs.matched_atoms.size() != 0;
	}

	SmartsMatcher::RecStruct_::RecStruct_()
	{
	}

	SmartsMatcher::RecStruct_::RecStruct_(const RecStruct_& rec_struct)
		:	matched_atoms(rec_struct.matched_atoms),
			mapped_atoms(rec_struct.mapped_atoms),
			visited_atoms(rec_struct.visited_atoms),
			visited_bonds(rec_struct.visited_bonds),
			visited_edges(rec_struct.visited_edges)
	{
	}

	SmartsMatcher::RecStruct_::~RecStruct_()
	{
	}

	void SmartsMatcher::RecStruct_::add(const RecStruct_& rec_struct)
	{
		for (vector<HashSet<const Atom*> >::const_iterator it = rec_struct.matched_atoms.begin(); it != rec_struct.matched_atoms.end(); ++it)
		{
			matched_atoms.push_back(*it);
		}
		for (vector<HashMap<const SPNode*, const Atom*> >::const_iterator it = rec_struct.mapped_atoms.begin(); it != rec_struct.mapped_atoms.end(); ++it)
		{
			mapped_atoms.push_back(*it);
		}
		for (vector<HashSet<const Atom*> >::const_iterator it = rec_struct.visited_atoms.begin(); it != rec_struct.visited_atoms.end(); ++it)
		{
			visited_atoms.push_back(*it);
		}
		for (vector<HashSet<const Bond*> >::const_iterator it = rec_struct.visited_bonds.begin(); it != rec_struct.visited_bonds.end(); ++it)
		{
			visited_bonds.push_back(*it);
		}
		for (vector<HashSet<const SPEdge*> >::const_iterator it = rec_struct.visited_edges.begin(); it != rec_struct.visited_edges.end(); ++it)
		{
			visited_edges.push_back(*it);
		}
		return;
	}

	void SmartsMatcher::RecStruct_::add(const RecStruct_& rec_struct, Size i)
	{
		matched_atoms.push_back(rec_struct.matched_atoms[i]);
		mapped_atoms.push_back(rec_struct.mapped_atoms[i]);
		visited_atoms.push_back(rec_struct.visited_atoms[i]);
		visited_bonds.push_back(rec_struct.visited_bonds[i]);
		visited_edges.push_back(rec_struct.visited_edges[i]);
		return;
	}

	void SmartsMatcher::RecStruct_::clear()
	{
		matched_atoms.clear();
		mapped_atoms.clear();
		visited_atoms.clear();
		visited_bonds.clear();
		visited_edges.clear();
		return;
	}

	SmartsMatcher::RecStruct_& SmartsMatcher::RecStruct_::operator = (const RecStruct_& rec_struct)
	{
		matched_atoms = rec_struct.matched_atoms;
		mapped_atoms = rec_struct.mapped_atoms;
		visited_atoms = rec_struct.visited_atoms;
		visited_bonds = rec_struct.visited_bonds;
		visited_edges = rec_struct.visited_edges;
		return *this;
	}
	
} // namespace BALL

