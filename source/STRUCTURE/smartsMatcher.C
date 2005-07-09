// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: smartsMatcher.C,v 1.1 2005/07/09 18:35:04 bertsch Exp $
//

#include <BALL/STRUCTURE/smartsMatcher.h>
#include <BALL/STRUCTURE/smartsParser.h>
#include <BALL/KERNEL/PTE.h>
//#include <BALL/QSAR/ringPerceptionProcessor.h>
//#include <BALL/QSAR/aromaticityProcessor.h>
//#include <BALL/QSAR/simpleDescriptors.h>

//#include <algorithm>
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
		// TODO ???
	}

	SmartsMatcher::~SmartsMatcher()
	{
	}

	vector<HashSet<const Atom*> > SmartsMatcher::match(const Molecule& molecule, const String& smarts) 
		throw(Exception::ParseError)
	{
		Molecule mol(molecule);
		//RingPerceptionProcessor rpp;
		//mol.apply(rpp);
		//AromaticityProcessor ap;
		//mol.apply(ap);

		// TODO what attributes of the molecule must be set?
		//NumberOfAromaticAtoms naa;
		//mol.apply(naa);
		
#ifdef SMARTS_MATCHER_DEBUG
		//cerr << "#aromatic atoms=" << mol.getProperty("NumberOfAromaticAtoms").getDouble() << endl;
#endif
		
		vector<HashSet<const Atom*> > matches;
		SmartsParser parser;
		parser.parse(smarts);
		SmartsParser::SPNode* root = parser.getRoot();

		// find first non-internal node, needed?
		// TODO: better selection heuristic, most discriminating part should be choosen first
		SmartsParser::SPAtom* begin = 0;
		SmartsParser::SPNode* begin_node = root;
		if (!root->isInternal())
		{
			begin = root->getSPAtom();
		}
		else
		{
			stack<SmartsParser::SPNode*> sp_nodes;
			sp_nodes.push(root->getFirstEdge()->getSecondSPNode());
			sp_nodes.push(root->getSecondEdge()->getSecondSPNode());
			while(!sp_nodes.empty())
			{
				SmartsParser::SPNode* node = sp_nodes.top();
				if (!node->isInternal())
				{
					begin = node->getSPAtom();
					begin_node = node;
					break;
				}
				else
				{
					sp_nodes.pop();
					sp_nodes.push(node->getFirstEdge()->getSecondSPNode());
					sp_nodes.push(node->getSecondEdge()->getSecondSPNode());
				}
			}
		}
			
		// now find all atoms that match that node
		HashSet<const Atom*> matched_atoms;
		for (AtomConstIterator it=mol.beginAtom(); +it; ++it)
		{
			if (begin->equals(&*it))
			{
				matched_atoms.insert(&*it);
			}
		}

#ifdef SMARTS_MATCHER_DEBUG
		cerr << "SM: found " << matched_atoms.size() << " seeds for matching" << endl;
#endif
		
		// extend seeds found
		for (HashSet<const Atom*>::Iterator it=matched_atoms.begin(); +it; ++it)
		{
			vector<HashSet<const Atom*> > match;
			evaluate_(match, begin_node, *it);
			if (match.size() != 0)
			{
				for (vector<HashSet<const Atom*> >::const_iterator it1=match.begin(); it1!=match.end(); ++it1)
				{
					matches.push_back(*it1);
				}
			}
		}

		// TODO elminate double hits, for exmaple CC in alphatic chains, can match C¹C² and C²C¹
		
#ifdef SMARTS_MATCHER_DEBUG
		cerr << "SM: found " << matches.size() << " matches from " << matched_atoms.size() << " seeds:" << endl;
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

	
	void SmartsMatcher::evaluate_(vector<HashSet<const Atom*> >& matched_atoms, SPNode* start_node, const Atom* start_atom)
	{
		vector<HashSet<const Atom*> > visited_atoms;
		vector<HashSet<SPNode*> > visited_nodes;
		evaluate_node_(matched_atoms, start_node, start_atom, visited_atoms, visited_nodes);
		return;
	}

	bool SmartsMatcher::evaluate_node_(vector<HashSet<const Atom*> >& matched_atoms, SPNode* start_node, const Atom* start_atom, vector<HashSet<const Atom*> >& visited_atoms, vector<HashSet<SPNode*> >& visited_nodes)
	{
		#ifdef SMARTS_MATCHER_DEBUG
		//cerr << "bool SmartsMatcher::evaluate_node_" << endl;
		#endif
		// the results are stored in here
		vector<HashSet<const Atom*> > result_matched_atoms;
		vector<HashSet<const Atom*> > result_visited_atoms;
		vector<HashSet<SPNode*> > result_visited_nodes;
		// if it is a internal node, we must consider the logical operator 
		if (start_node->isInternal())
		{
			// both edges must be set
			SPEdge* first_edge = start_node->getFirstEdge();
			SPEdge* second_edge = start_node->getSecondEdge();
			SmartsParser::LogicalOperator log_op = start_node->getLogicalOperator();
			if (log_op == SmartsParser::AND || log_op == SmartsParser::AND_LOW)
			{
				// for every possible beginning
				for (Size i=0; i!=matched_atoms.size(); ++i)
				{
					// create new data structures
					vector<HashSet<const Atom*> > first_new_matched_atoms;
					first_new_matched_atoms.push_back(matched_atoms[i]);
					vector<HashSet<const Atom*> > first_new_visited_atoms;
					first_new_visited_atoms.push_back(visited_atoms[i]);
					vector<HashSet<SPNode*> > first_new_visited_nodes;
					first_new_visited_nodes.push_back(visited_nodes[i]);
					// for each bond
					for (Atom::BondConstIterator it1=start_atom->beginBond(); it1!=start_atom->endBond(); ++it1)
					{
						// can it be matched?
						if (evaluate_edge_(first_new_matched_atoms, first_edge, start_atom, &*it1, first_new_visited_atoms, first_new_visited_nodes))
						{
							// now try to match the second edge
							for (Size j=0; j!=first_new_matched_atoms.size(); ++j)
							{
								vector<HashSet<const Atom*> > second_new_matched_atoms;
								second_new_matched_atoms.push_back(first_new_matched_atoms[j]);
								vector<HashSet<const Atom*> > second_new_visited_atoms;
								second_new_visited_atoms.push_back(first_new_visited_atoms[j]);
								vector<HashSet<SPNode*> > second_new_visited_nodes;
								second_new_visited_nodes.push_back(first_new_visited_nodes[j]);
								for (Atom::BondConstIterator it2=start_atom->beginBond(); it2!=start_atom->endBond(); ++it2)
								{
									if (evaluate_edge_(second_new_matched_atoms, second_edge, start_atom, &*it2, second_new_visited_atoms, second_new_visited_nodes))
									{
										for (vector<HashSet<const Atom*> >::const_iterator it=second_new_matched_atoms.begin(); it!=second_new_matched_atoms.end(); ++it)
										{
											result_matched_atoms.push_back(*it);
										}
										for (vector<HashSet<const Atom*> >::const_iterator it=second_new_visited_atoms.begin(); it!=second_new_visited_atoms.end(); ++it)
										{
											result_visited_atoms.push_back(*it);
										}
										for (vector<HashSet<SPNode*> >::const_iterator it=second_new_visited_nodes.begin(); it!=second_new_visited_nodes.end(); ++it)
										{
											result_visited_nodes.push_back(*it);
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
				if (log_op == SmartsParser::OR)
				{
	        vector<HashSet<const Atom*> > first_new_matched_atoms = matched_atoms;
					vector<HashSet<const Atom*> > first_new_visited_atoms = visited_atoms;
					vector<HashSet<SPNode*> > first_new_visited_nodes = visited_nodes;
					// TODO same as for AND
				}
			}
		}
		else // normal node
		{
			if (start_node->getSPAtom()->equals(start_atom))
			{
				#ifdef SMARTS_PARSER_DEBUG
				cerr << "matched atom (#edges=" << start_node->countEdges() << ")" << endl;
				#endif
				// first matched node?
				if (matched_atoms.size() == 0)
				{
					// just add the found atom and node to the structures
					// the details are further processed by the next for loop
					HashSet<const Atom*> matched_atom;
					matched_atom.insert(start_atom);
					matched_atoms.push_back(matched_atom);
					visited_atoms.push_back(matched_atom);
					HashSet<SPNode*> visited_node;
					visited_node.insert(start_node);
					visited_nodes.push_back(visited_node);
				}
				for (Size i=0; i!=matched_atoms.size(); ++i)
				{
					// are we at a leaf?
					if (start_node->countEdges() == 0)
					{
						// just add the node and atom to the results 
						result_matched_atoms.push_back(matched_atoms[i]);
						result_matched_atoms[result_matched_atoms.size()-1].insert(start_atom);
						result_visited_atoms.push_back(visited_atoms[i]);
						result_visited_atoms[result_visited_atoms.size()-1].insert(start_atom);
						result_visited_nodes.push_back(visited_nodes[i]);
						result_visited_nodes[result_visited_nodes.size()-1].insert(start_node);
					}
				
					for (SmartsParser::SPNode::EdgeIterator eit=start_node->begin(); eit!=start_node->end(); ++eit)
					{
						for (Atom::BondConstIterator bit=start_atom->beginBond(); bit!=start_atom->endBond(); ++bit)
						{
							//
							vector<HashSet<const Atom*> > new_matched_atoms;
							new_matched_atoms.push_back(matched_atoms[i]);
							new_matched_atoms[0].insert(start_atom);
							vector<HashSet<const Atom*> > new_visited_atoms;
							new_visited_atoms.push_back(visited_atoms[i]);
							new_visited_atoms[0].insert(start_atom);
							vector<HashSet<SPNode*> > new_visited_nodes;
							new_visited_nodes.push_back(visited_nodes[i]);
							new_visited_nodes[0].insert(start_node);
							if (evaluate_edge_(new_matched_atoms, *eit, start_atom, &*bit, new_visited_atoms, new_visited_nodes))
							{
								for (vector<HashSet<const Atom*> >::const_iterator it=new_matched_atoms.begin(); it!=new_matched_atoms.end(); ++it)
								{
									result_matched_atoms.push_back(*it);
								}
								for (vector<HashSet<const Atom*> >::const_iterator it=new_visited_atoms.begin(); it!=new_visited_atoms.end(); ++it)
								{
									result_visited_atoms.push_back(*it);
								}
								for (vector<HashSet<SPNode*> >::const_iterator it=new_visited_nodes.begin(); it!=new_visited_nodes.end(); ++it)
								{
									result_visited_nodes.push_back(*it);
								}
							}
						}
					}
				}
			}
		}
		
		matched_atoms = result_matched_atoms;
		visited_atoms = result_visited_atoms;
		visited_nodes = result_visited_nodes;
		return matched_atoms.size() != 0;
	}
	
	bool SmartsMatcher::evaluate_edge_(vector<HashSet<const Atom*> >& matched_atoms, SPEdge* start_edge, const Atom* start_atom, const Bond* start_bond, vector<HashSet<const Atom*> >& visited_atoms, vector<HashSet<SPNode*> >& visited_nodes)
	{
		#ifdef SMARTS_PARSER_DEBUG
		//cerr << "bool SmartsMatcher::evaluate_edge_" << endl;
		#endif
		vector<HashSet<const Atom*> > result_matched_atoms;
		vector<HashSet<const Atom*> > result_visited_atoms;
		vector<HashSet<SPNode*> > result_visited_nodes;
		
		if (start_edge->isInternal())
		{
			SmartsParser::LogicalOperator log_op = start_edge->getLogicalOperator();
			if (log_op == SmartsParser::AND || log_op == SmartsParser::AND_LOW)
			{
				// TODO
				// if (evaluate_edge_(..., start_edge->getFirstSPEdge(), ...))
				//{
				// 	if (evaluate_edge_(..., start_edge_->getSecondSPEdge(), ...))
				//	{
				//	}
				//}
				
			}
			else
			{
				if (log_op == SmartsParser::OR)
				{
					// TODO
				}
			}
		}
		else
		{
			// just call the evaluate_node_ methode with next node
			if (start_edge->getSPBond()->equals(start_bond))
			{
				// only non-ringclosure bonds are considered so far!
				const Atom* partner = start_bond->getPartner(*start_atom);
				for (Size i=0; i!=matched_atoms.size(); ++i)
				{
					if (!matched_atoms[i].has(partner))
					{
						vector<HashSet<const Atom*> > new_matched_atoms;
						new_matched_atoms.push_back(matched_atoms[i]);
						vector<HashSet<const Atom*> > new_visited_atoms;
						new_visited_atoms.push_back(visited_atoms[i]);
						vector<HashSet<SPNode*> > new_visited_nodes;
						new_visited_nodes.push_back(visited_nodes[i]);
						if (evaluate_node_(new_matched_atoms, start_edge->getSecondSPNode(), partner, new_visited_atoms, new_visited_nodes))
						{
							for (vector<HashSet<const Atom*> >::const_iterator it=new_matched_atoms.begin(); it!=new_matched_atoms.end(); ++it)
							{
								result_matched_atoms.push_back(*it);
							}
							for (vector<HashSet<const Atom*> >::const_iterator it=new_visited_atoms.begin(); it!=new_visited_atoms.end(); ++it)
							{
								result_visited_atoms.push_back(*it);
							}
							for (vector<HashSet<SPNode*> >::const_iterator it=new_visited_nodes.begin(); it!=new_visited_nodes.end(); ++it)
							{
								result_visited_nodes.push_back(*it);
							}
						}
					}
				}
			}
		}

		matched_atoms = result_matched_atoms;
		visited_atoms = result_visited_atoms;
		visited_nodes = result_visited_nodes;
		
		return matched_atoms.size() != 0;
	}
	
} // namespace BALL
